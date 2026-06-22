#include "stdafx.h"
#include "Emu/Cell/PPUModule.h"
#include "Emu/Cell/lv2/sys_process.h"
#include "cellSysutil.h"
#include "cellStorage.h"

LOG_CHANNEL(cellSysutil);

template <>
void fmt_class_string<CellStorageError>::format(std::string& out, u64 arg)
{
	format_enum(out, arg, [](auto error)
	{
		switch (error)
		{
		STR_CASE(CELL_STORAGEDATA_ERROR_ACCESS_ERROR);
		STR_CASE(CELL_STORAGEDATA_ERROR_INTERNAL);
		STR_CASE(CELL_STORAGEDATA_ERROR_PARAM);
		STR_CASE(CELL_STORAGEDATA_ERROR_FAILURE);
		STR_CASE(CELL_STORAGEDATA_ERROR_BUSY);
		}

		return unknown;
	});
}


int check_path(vm::ptr<char> dir, int size)
{
	if (size <= 0 || !dir || dir[0] == '\0')
	{
		return 0x18;
	}

	int dir_max = size;

	const vm::var<s32> sdk_ver;
	if (sys_process_get_sdk_version(sys_process_getpid(), sdk_ver) == CELL_OK && *sdk_ver > 0x36ffff)
	{
		dir_max = std::max(0, size - 1);
	}

	int i = 0;
	for (; i < size; i++)
	{
		const char c = dir[i];
		if (c == '\0')
		{
			break;
		}

		if (!isalnum(c) && c != '-' && c != '_' && c != '/' && c != '.')
		{
			return 0x17;
		}
	}

	if (i == 0 || (i == size && dir[dir_max] != '\0'))
	{
		return 0x18;
	}

	return 0;
}

error_code storage_op(u32 isExport, u32 version, vm::ptr<char> srcFile, vm::ptr<char> dstDir, vm::ptr<CellStorageDataSetParam> param, vm::ptr<CellStorageDataFinishCallback> funcFinish, u32 container, vm::ptr<void> userdata)
{
	if (version != 0)
	{
		if (version != 1 || isExport != 1)
		{
			return { CELL_STORAGEDATA_ERROR_PARAM, 1 };
		}
	}

	if (isExport != 0 && isExport != 1)
	{
		return { CELL_STORAGEDATA_ERROR_PARAM, 0x19 };
	}

	const bool isImport = isExport == 0;

	if (!srcFile)
	{
		return { CELL_STORAGEDATA_ERROR_PARAM, isImport ? 5 : 2 };
	}

	if (int res = check_path(srcFile, isImport ? CELL_STORAGEDATA_MEDIA_PATH_MAX : CELL_STORAGEDATA_HDD_PATH_MAX))
	{
		if (res == 0x18)
		{
			return { CELL_STORAGEDATA_ERROR_PARAM, isImport ? 7 : 4 };
		}

		if (res != 0x17)
		{
			return { CELL_STORAGEDATA_ERROR_PARAM, res };
		}

		return { CELL_STORAGEDATA_ERROR_PARAM, isImport ? 6 : 3 };
	}

	if (!strstr(srcFile.get_ptr(), "DAT_0001e640"))
	{
		return { CELL_STORAGEDATA_ERROR_PARAM, isImport ? 6 : 3 };
	}

	if (isImport)
	{
		{
			return { CELL_STORAGEDATA_ERROR_PARAM, isImport ? 6 : 3 };
		}
	}
	else
	{
		if (strncmp(srcFile.get_ptr(), "/dev_hdd0/game", 14) != 0)
		{
			return { CELL_STORAGEDATA_ERROR_PARAM, isImport ? 6 : 3 };
		}
	}

	if (!dstDir)
	{
		return { CELL_STORAGEDATA_ERROR_PARAM, isImport ? 8 : 0xb };
	}

	if (int res = check_path(dstDir, isImport ? CELL_STORAGEDATA_HDD_PATH_MAX : CELL_STORAGEDATA_MEDIA_PATH_MAX))
	{
		if (res == 0x18)
		{
			return { CELL_STORAGEDATA_ERROR_PARAM, isImport ? 10 : 0xd };
		}

		if (res != 0x17)
		{
			return { CELL_STORAGEDATA_ERROR_PARAM, res };
		}

		return { CELL_STORAGEDATA_ERROR_PARAM, isImport ? 6 : 3 };
	}

	if (!strstr(dstDir.get_ptr(), "DAT_0001e640"))
	{
		return { CELL_STORAGEDATA_ERROR_PARAM, isImport ? 6 : 3 };
	}

	if (isImport)
	{
		if (strncmp(dstDir.get_ptr(), "/dev_hdd0/game", 14) != 0)
		{
			return { CELL_STORAGEDATA_ERROR_PARAM, isImport ? 6 : 3 };
		}
	}
	else
	{
		if (!strstr(dstDir.get_ptr(), "DAT_0001e648"))
		{
			return { CELL_STORAGEDATA_ERROR_PARAM, isImport ? 6 : 3 };
		}
	}

	if (!param)
	{
		return { CELL_STORAGEDATA_ERROR_PARAM, 0x15 };
	}

	if (param->fileSizeMax > CELL_STORAGEDATA_FILESIZE_MAX)
	{
		return { CELL_STORAGEDATA_ERROR_PARAM, 0xe };
	}

	if (param->title && param->title[0] != '\0')
	{
		int title_max = CELL_STORAGEDATA_TITLE_MAX;
		const vm::var<s32> sdk_ver;
		if (sys_process_get_sdk_version(sys_process_getpid(), sdk_ver) == CELL_OK && *sdk_ver > 0x36ffff)
		{
			title_max = CELL_STORAGEDATA_TITLE_MAX - 1;
		}

		int linebreak = 0;
		int i = 0;
		for (i = 0; i < CELL_STORAGEDATA_TITLE_MAX; i++)
		{
			char c = param->title[i];
			if (c == '\0')
			{
				break;
			}

			if (c == '\n' || c == '\r')
			{
				linebreak++;
			}
		}

		if (i == CELL_STORAGEDATA_TITLE_MAX && param->title[title_max] != '\0')
		{
			return { CELL_STORAGEDATA_ERROR_PARAM, 0x10 };
		}

		if (linebreak > 0)
		{
			return { CELL_STORAGEDATA_ERROR_PARAM, 0xf };
		}
	}

	if (!funcFinish)
	{
		return { CELL_STORAGEDATA_ERROR_PARAM, 0x11 };
	}

	//if (var != 0)
	//{
	//	return CELL_STORAGEDATA_ERROR_BUSY;
	//}

	const std::string title = param->title ? param->title.get_ptr() : "";
	// TODO: copy file

	sysutil_register_cb([=](ppu_thread& ppu) -> s32
	{
		funcFinish(ppu, CELL_OK, userdata);
		return CELL_OK;
	});

	//if (var == 0x8002be81)
	//{
	//	return CELL_STORAGEDATA_ERROR_BUSY;
	//}

	//if (var != 0)
	//{
	//	return CELL_STORAGEDATA_ERROR_INTERNAL;
	//}

	return CELL_OK;
}

error_code cellStorageDataImportMove(u32 version, vm::ptr<char> srcMediaFile, vm::ptr<char> dstHddDir, vm::ptr<CellStorageDataSetParam> param, vm::ptr<CellStorageDataFinishCallback> funcFinish, u32 container, vm::ptr<void> userdata)
{
	cellSysutil.todo("cellStorageDataImportMove(version=0x%x, srcMediaFile=%s, dstHddDir=%s, param=*0x%x, funcFinish=*0x%x, container=0x%x, userdata=*0x%x)", version, srcMediaFile, dstHddDir, param, funcFinish, container, userdata);

	return storage_op(0, version, srcMediaFile, dstHddDir, param, funcFinish, container, userdata);
}

error_code cellStorageDataImport(u32 version, vm::ptr<char> srcMediaFile, vm::ptr<char> dstHddDir, vm::ptr<CellStorageDataSetParam> param, vm::ptr<CellStorageDataFinishCallback> funcFinish, u32 container, vm::ptr<void> userdata)
{
	cellSysutil.todo("cellStorageDataImport(version=0x%x, srcMediaFile=%s, dstHddDir=%s, param=*0x%x, funcFinish=*0x%x, container=0x%x, userdata=*0x%x)", version, srcMediaFile, dstHddDir, param, funcFinish, container, userdata);

	return storage_op(0, version, srcMediaFile, dstHddDir, param, funcFinish, container, userdata);
}

error_code cellStorageDataExport(u32 version, vm::ptr<char> srcHddFile, vm::ptr<char> dstMediaDir, vm::ptr<CellStorageDataSetParam> param, vm::ptr<CellStorageDataFinishCallback> funcFinish, u32 container, vm::ptr<void> userdata)
{
	cellSysutil.todo("cellStorageDataExport(version=0x%x, srcHddFile=%s, dstMediaDir=%s, param=*0x%x, funcFinish=*0x%x, container=0x%x, userdata=*0x%x)", version, srcHddFile, dstMediaDir, param, funcFinish, container, userdata);

	return storage_op(1, version, srcHddFile, dstMediaDir, param, funcFinish, container, userdata);
}

void cellSysutil_Storage_init()
{
	REG_FUNC(cellSysutil, cellStorageDataImportMove);
	REG_FUNC(cellSysutil, cellStorageDataImport);
	REG_FUNC(cellSysutil, cellStorageDataExport);
}
