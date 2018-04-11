#include "stdafx.h"
#include "Emu/Cell/PPUModule.h"
#include "cellPerf.h"

logs::channel cellPerf("cellPerf");

void cellPerfStart()
{
	cellPerf.todo("cellPerfStart()");
}

void cellPerfStop()
{
	cellPerf.todo("cellPerfStop()");
}

s32 cellPerfAddCBEpm(vm::ptr<CellPerfCBEpmSetup> setup)
{
	cellPerf.todo("cellPerfAddCBEpm(setup=*0x%x)", setup);

	return CELL_OK;
}

void cellPerfInsertCBEpmBookmark(u64 data)
{
	cellPerf.todo("cellPerfInsertCBEpmBookmark(data=%d)", data);
}

s32 cellPerfGetCBEpmStatus(vm::ptr<CellPerfCBEpmStatus> status)
{
	cellPerf.todo("cellPerfGetCBEpmStatus(status=*0x%x)", status);

	return CELL_OK;
}

s32 cellPerfCopyCBEpmTraceData(u32 offset)
{
	cellPerf.todo("cellPerfCopyCBEpmTraceData(offset=%d)", offset);

	return CELL_OK;
}

s32 cellPerfCopyCBEpmTraceDataDma(u32 offset)
{
	cellPerf.todo("cellPerfCopyCBEpmTraceDataDma(offset=%d)", offset);

	return CELL_OK;
}

s32 cellPerfGetCBEpmWritePointer(vm::ptr<u32> wpointer)
{
	cellPerf.todo("cellPerfGetCBEpmWritePointer(wpointer=*0x%x)", wpointer);

	return CELL_OK;
}

s32 cellPerfChangeCBEpmCacheSize(u64 size)
{
	cellPerf.todo("cellPerfChangeCBEpmCacheSize(size=%d)", size);

	return CELL_OK;
}

s32 cellPerfDeleteAll()
{
	cellPerf.todo("cellPerfDeleteAll()");

	return CELL_OK;
}

void cellPerfDeleteCBEpm()
{
	cellPerf.todo("cellPerfDeleteCBEpm()");
}

s32 cellPerfAddLv2OSTrace(u32 type, u64 bufferSize, u32 mode, vm::pptr<u32> buffer)
{
	cellPerf.todo("cellPerfAddLv2OSTrace(type=%d, bufferSize=%d, mode=%d, buffer=**0x%x,)", type, bufferSize, mode, buffer);

	return CELL_OK;
}

s32 cellPerfDeleteAllLv2OSTrace()
{
	cellPerf.todo("cellPerfDeleteAllLv2OSTrace()");

	return CELL_OK;
}

s32 cellPerfDeleteLv2OSTrace(u32 type)
{
	cellPerf.todo("cellPerfDeleteLv2OSTrace(type=%d)", type);

	return CELL_OK;
}

s32 cellPerfReadAndResetCBEpmCounter(vm::ptr<CellPerfCBEpmCounter> counter)
{
	cellPerf.todo("cellPerfReadAndResetCBEpmCounter(counter=*0x%x)", counter);

	return CELL_OK;
}

DECLARE(ppu_module_manager::cellPerf)("cellPerf", []()
{
	REG_FUNC(cellPerf, cellPerfStart);
	REG_FUNC(cellPerf, cellPerfStop);
	REG_FUNC(cellPerf, cellPerfAddCBEpm);
	REG_FUNC(cellPerf, cellPerfInsertCBEpmBookmark);
	REG_FUNC(cellPerf, cellPerfGetCBEpmStatus);
	REG_FUNC(cellPerf, cellPerfCopyCBEpmTraceData);
	REG_FUNC(cellPerf, cellPerfCopyCBEpmTraceDataDma);
	REG_FUNC(cellPerf, cellPerfGetCBEpmWritePointer);
	REG_FUNC(cellPerf, cellPerfChangeCBEpmCacheSize);
	REG_FUNC(cellPerf, cellPerfDeleteAll);
	REG_FUNC(cellPerf, cellPerfDeleteCBEpm);
	REG_FUNC(cellPerf, cellPerfAddLv2OSTrace);
	REG_FUNC(cellPerf, cellPerfDeleteAllLv2OSTrace);
	REG_FUNC(cellPerf, cellPerfDeleteLv2OSTrace);
	REG_FUNC(cellPerf, cellPerfReadAndResetCBEpmCounter);
});
