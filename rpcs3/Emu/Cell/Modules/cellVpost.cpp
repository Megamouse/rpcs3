#include "stdafx.h"
#include "Emu/IdManager.h"
#include "Emu/Cell/PPUModule.h"
#include "util/asm.hpp"

#ifdef _MSC_VER
#pragma warning(push, 0)
#else
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wall"
#pragma GCC diagnostic ignored "-Wextra"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif
extern "C"
{
#include "libswscale/swscale.h"
}
#ifdef _MSC_VER
#pragma warning(pop)
#else
#pragma GCC diagnostic pop
#endif

#include "cellVpost.h"

LOG_CHANNEL(cellVpost);

template<>
void fmt_class_string<CellVpostError>::format(std::string& out, u64 arg)
{
	format_enum(out, arg, [](auto error)
	{
		switch (error)
		{
			STR_CASE(CELL_VPOST_ERROR_Q_ARG_CFG_NULL);
			STR_CASE(CELL_VPOST_ERROR_Q_ARG_CFG_INVALID);
			STR_CASE(CELL_VPOST_ERROR_Q_ARG_ATTR_NULL);
			STR_CASE(CELL_VPOST_ERROR_O_ARG_CFG_NULL);
			STR_CASE(CELL_VPOST_ERROR_O_ARG_CFG_INVALID);
			STR_CASE(CELL_VPOST_ERROR_O_ARG_RSRC_NULL);
			STR_CASE(CELL_VPOST_ERROR_O_ARG_RSRC_INVALID);
			STR_CASE(CELL_VPOST_ERROR_O_ARG_HDL_NULL);
			STR_CASE(CELL_VPOST_ERROR_O_FATAL_QUERY_FAIL);
			STR_CASE(CELL_VPOST_ERROR_O_FATAL_CREATEMON_FAIL);
			STR_CASE(CELL_VPOST_ERROR_O_FATAL_INITSPURS_FAIL);
			STR_CASE(CELL_VPOST_ERROR_C_ARG_HDL_NULL);
			STR_CASE(CELL_VPOST_ERROR_C_ARG_HDL_INVALID);
			STR_CASE(CELL_VPOST_ERROR_C_FATAL_LOCKMON_FAIL);
			STR_CASE(CELL_VPOST_ERROR_C_FATAL_UNLOCKMON_FAIL);
			STR_CASE(CELL_VPOST_ERROR_C_FATAL_DESTROYMON_FAIL);
			STR_CASE(CELL_VPOST_ERROR_C_FATAL_FINSPURS_FAIL);
			STR_CASE(CELL_VPOST_ERROR_E_ARG_HDL_NULL);
			STR_CASE(CELL_VPOST_ERROR_E_ARG_HDL_INVALID);
			STR_CASE(CELL_VPOST_ERROR_E_ARG_INPICBUF_NULL);
			STR_CASE(CELL_VPOST_ERROR_E_ARG_INPICBUF_INVALID);
			STR_CASE(CELL_VPOST_ERROR_E_ARG_CTRL_NULL);
			STR_CASE(CELL_VPOST_ERROR_E_ARG_CTRL_INVALID);
			STR_CASE(CELL_VPOST_ERROR_E_ARG_OUTPICBUF_NULL);
			STR_CASE(CELL_VPOST_ERROR_E_ARG_OUTPICBUF_INVALID);
			STR_CASE(CELL_VPOST_ERROR_E_ARG_PICINFO_NULL);
			STR_CASE(CELL_VPOST_ERROR_E_FATAL_LOCKMON_FAIL);
			STR_CASE(CELL_VPOST_ERROR_E_FATAL_UNLOCKMON_FAIL);
			STR_CASE(CELL_VPOST_ENT_ERROR_Q_ARG_ATTR_NULL);
			STR_CASE(CELL_VPOST_ENT_ERROR_O_ARG_RSRC_NULL);
			STR_CASE(CELL_VPOST_ENT_ERROR_O_ARG_RSRC_INVALID);
			STR_CASE(CELL_VPOST_ENT_ERROR_O_ARG_HDL_NULL);
			STR_CASE(CELL_VPOST_ENT_ERROR_O_FATAL_QUERY_FAIL);
			STR_CASE(CELL_VPOST_ENT_ERROR_O_FATAL_CSPUCORE_FAIL);
			STR_CASE(CELL_VPOST_ENT_ERROR_C_ARG_HDL_NULL);
			STR_CASE(CELL_VPOST_ENT_ERROR_C_ARG_HDL_INVALID);
			STR_CASE(CELL_VPOST_ENT_ERROR_C_FATAL_SNDCMD_FAIL);
			STR_CASE(CELL_VPOST_ENT_ERROR_C_FATAL_RCVRES_FAIL);
			STR_CASE(CELL_VPOST_ENT_ERROR_C_FATAL_DSPUCORE_FAIL);
			STR_CASE(CELL_VPOST_ENT_ERROR_E_ARG_HDL_NULL);
			STR_CASE(CELL_VPOST_ENT_ERROR_E_ARG_HDL_INVALID);
			STR_CASE(CELL_VPOST_ENT_ERROR_E_ARG_INPICBUF_NULL);
			STR_CASE(CELL_VPOST_ENT_ERROR_E_ARG_INPICBUF_INVALID);
			STR_CASE(CELL_VPOST_ENT_ERROR_E_ARG_INPICINFO_NULL);
			STR_CASE(CELL_VPOST_ENT_ERROR_E_ARG_INPICINFO_INVALID);
			STR_CASE(CELL_VPOST_ENT_ERROR_E_ARG_CTRL_NULL);
			STR_CASE(CELL_VPOST_ENT_ERROR_E_ARG_CTRL_INVALID);
			STR_CASE(CELL_VPOST_ENT_ERROR_E_ARG_COMB_INVALID);
			STR_CASE(CELL_VPOST_ENT_ERROR_E_ARG_OUTPICBUF_NULL);
			STR_CASE(CELL_VPOST_ENT_ERROR_E_ARG_OUTPICBUF_INVALID);
			STR_CASE(CELL_VPOST_ENT_ERROR_E_ARG_OUTPICINFO_NULL);
			STR_CASE(CELL_VPOST_ENT_ERROR_E_FATAL_SNDCMD_FAIL);
			STR_CASE(CELL_VPOST_ENT_ERROR_E_FATAL_RCVRES_FAIL);
			STR_CASE(CELL_VPOST_ENT_ERROR_E_FATAL_SPUCORE_FAIL);
			STR_CASE(CELL_VPOST_IPC_ERROR_Q_ARG_ATTR_NULL);
			STR_CASE(CELL_VPOST_IPC_ERROR_O_ARG_RSRC_NULL);
			STR_CASE(CELL_VPOST_IPC_ERROR_O_ARG_RSRC_INVALID);
			STR_CASE(CELL_VPOST_IPC_ERROR_O_ARG_HDL_NULL);
			STR_CASE(CELL_VPOST_IPC_ERROR_O_FATAL_QUERY_FAIL);
			STR_CASE(CELL_VPOST_IPC_ERROR_O_FATAL_CSPUCORE_FAIL);
			STR_CASE(CELL_VPOST_IPC_ERROR_C_ARG_HDL_NULL);
			STR_CASE(CELL_VPOST_IPC_ERROR_C_ARG_HDL_INVALID);
			STR_CASE(CELL_VPOST_IPC_ERROR_C_FATAL_SNDCMD_FAIL);
			STR_CASE(CELL_VPOST_IPC_ERROR_C_FATAL_RCVRES_FAIL);
			STR_CASE(CELL_VPOST_IPC_ERROR_C_FATAL_DSPUCORE_FAIL);
			STR_CASE(CELL_VPOST_IPC_ERROR_E_ARG_HDL_NULL);
			STR_CASE(CELL_VPOST_IPC_ERROR_E_ARG_HDL_INVALID);
			STR_CASE(CELL_VPOST_IPC_ERROR_E_ARG_INPICBUF_NULL);
			STR_CASE(CELL_VPOST_IPC_ERROR_E_ARG_INPICBUF_INVALID);
			STR_CASE(CELL_VPOST_IPC_ERROR_E_ARG_INPICINFO_NULL);
			STR_CASE(CELL_VPOST_IPC_ERROR_E_ARG_INPICINFO_INVALID);
			STR_CASE(CELL_VPOST_IPC_ERROR_E_ARG_CTRL_NULL);
			STR_CASE(CELL_VPOST_IPC_ERROR_E_ARG_CTRL_INVALID);
			STR_CASE(CELL_VPOST_IPC_ERROR_E_ARG_COMB_INVALID);
			STR_CASE(CELL_VPOST_IPC_ERROR_E_ARG_OUTPICBUF_NULL);
			STR_CASE(CELL_VPOST_IPC_ERROR_E_ARG_OUTPICBUF_INVALID);
			STR_CASE(CELL_VPOST_IPC_ERROR_E_ARG_OUTPICINFO_NULL);
			STR_CASE(CELL_VPOST_IPC_ERROR_E_FATAL_SNDCMD_FAIL);
			STR_CASE(CELL_VPOST_IPC_ERROR_E_FATAL_RCVRES_FAIL);
			STR_CASE(CELL_VPOST_IPC_ERROR_E_FATAL_SPUCORE_FAIL);
			STR_CASE(CELL_VPOST_VSC_ERROR_Q_ARG_ATTR_NULL);
			STR_CASE(CELL_VPOST_VSC_ERROR_O_ARG_RSRC_NULL);
			STR_CASE(CELL_VPOST_VSC_ERROR_O_ARG_RSRC_INVALID);
			STR_CASE(CELL_VPOST_VSC_ERROR_O_ARG_HDL_NULL);
			STR_CASE(CELL_VPOST_VSC_ERROR_O_FATAL_QUERY_FAIL);
			STR_CASE(CELL_VPOST_VSC_ERROR_O_FATAL_CSPUCORE_FAIL);
			STR_CASE(CELL_VPOST_VSC_ERROR_C_ARG_HDL_NULL);
			STR_CASE(CELL_VPOST_VSC_ERROR_C_ARG_HDL_INVALID);
			STR_CASE(CELL_VPOST_VSC_ERROR_C_FATAL_SNDCMD_FAIL);
			STR_CASE(CELL_VPOST_VSC_ERROR_C_FATAL_RCVRES_FAIL);
			STR_CASE(CELL_VPOST_VSC_ERROR_C_FATAL_DSPUCORE_FAIL);
			STR_CASE(CELL_VPOST_VSC_ERROR_E_ARG_HDL_NULL);
			STR_CASE(CELL_VPOST_VSC_ERROR_E_ARG_HDL_INVALID);
			STR_CASE(CELL_VPOST_VSC_ERROR_E_ARG_INPICBUF_NULL);
			STR_CASE(CELL_VPOST_VSC_ERROR_E_ARG_INPICBUF_INVALID);
			STR_CASE(CELL_VPOST_VSC_ERROR_E_ARG_INPICINFO_NULL);
			STR_CASE(CELL_VPOST_VSC_ERROR_E_ARG_INPICINFO_INVALID);
			STR_CASE(CELL_VPOST_VSC_ERROR_E_ARG_CTRL_NULL);
			STR_CASE(CELL_VPOST_VSC_ERROR_E_ARG_CTRL_INVALID);
			STR_CASE(CELL_VPOST_VSC_ERROR_E_ARG_COMB_INVALID);
			STR_CASE(CELL_VPOST_VSC_ERROR_E_ARG_OUTPICBUF_NULL);
			STR_CASE(CELL_VPOST_VSC_ERROR_E_ARG_OUTPICBUF_INVALID);
			STR_CASE(CELL_VPOST_VSC_ERROR_E_ARG_OUTPICINFO_NULL);
			STR_CASE(CELL_VPOST_VSC_ERROR_E_FATAL_SNDCMD_FAIL);
			STR_CASE(CELL_VPOST_VSC_ERROR_E_FATAL_RCVRES_FAIL);
			STR_CASE(CELL_VPOST_VSC_ERROR_E_FATAL_SPUCORE_FAIL);
			STR_CASE(CELL_VPOST_CSC_ERROR_Q_ARG_ATTR_NULL);
			STR_CASE(CELL_VPOST_CSC_ERROR_O_ARG_RSRC_NULL);
			STR_CASE(CELL_VPOST_CSC_ERROR_O_ARG_RSRC_INVALID);
			STR_CASE(CELL_VPOST_CSC_ERROR_O_ARG_HDL_NULL);
			STR_CASE(CELL_VPOST_CSC_ERROR_O_FATAL_QUERY_FAIL);
			STR_CASE(CELL_VPOST_CSC_ERROR_O_FATAL_CSPUCORE_FAIL);
			STR_CASE(CELL_VPOST_CSC_ERROR_C_ARG_HDL_NULL);
			STR_CASE(CELL_VPOST_CSC_ERROR_C_ARG_HDL_INVALID);
			STR_CASE(CELL_VPOST_CSC_ERROR_C_FATAL_SNDCMD_FAIL);
			STR_CASE(CELL_VPOST_CSC_ERROR_C_FATAL_RCVRES_FAIL);
			STR_CASE(CELL_VPOST_CSC_ERROR_C_FATAL_DSPUCORE_FAIL);
			STR_CASE(CELL_VPOST_CSC_ERROR_E_ARG_HDL_NULL);
			STR_CASE(CELL_VPOST_CSC_ERROR_E_ARG_HDL_INVALID);
			STR_CASE(CELL_VPOST_CSC_ERROR_E_ARG_INPICBUF_NULL);
			STR_CASE(CELL_VPOST_CSC_ERROR_E_ARG_INPICBUF_INVALID);
			STR_CASE(CELL_VPOST_CSC_ERROR_E_ARG_INPICINFO_NULL);
			STR_CASE(CELL_VPOST_CSC_ERROR_E_ARG_INPICINFO_INVALID);
			STR_CASE(CELL_VPOST_CSC_ERROR_E_ARG_CTRL_NULL);
			STR_CASE(CELL_VPOST_CSC_ERROR_E_ARG_CTRL_INVALID);
			STR_CASE(CELL_VPOST_CSC_ERROR_E_ARG_COMB_INVALID);
			STR_CASE(CELL_VPOST_CSC_ERROR_E_ARG_OUTPICBUF_NULL);
			STR_CASE(CELL_VPOST_CSC_ERROR_E_ARG_OUTPICBUF_INVALID);
			STR_CASE(CELL_VPOST_CSC_ERROR_E_ARG_OUTPICINFO_NULL);
			STR_CASE(CELL_VPOST_CSC_ERROR_E_FATAL_SNDCMD_FAIL);
			STR_CASE(CELL_VPOST_CSC_ERROR_E_FATAL_RCVRES_FAIL);
			STR_CASE(CELL_VPOST_CSC_ERROR_E_FATAL_SPUCORE_FAIL);
		}

		return unknown;
	});
}

// Even number between 32 and 1920
bool check_width(u32 width)
{
	return width >= 32 && width <= 1920 && (width % 2) == 0;
};

// Even number between 32 and 1088
bool check_height(u32 height)
{
	return height >= 32 && height <= 1088 && (height % 2) == 0;
};

bool check_cfg_param(const CellVpostCfgParam* cfgParam)
{
	if (!check_width(cfgParam->inMaxWidth) ||
		!check_height(cfgParam->inMaxHeight) ||
		cfgParam->inDepth != CELL_VPOST_PIC_DEPTH_8 ||
		cfgParam->inPicFmt != CELL_VPOST_PIC_FMT_IN_YUV420_PLANAR ||
		!check_width(cfgParam->outMaxWidth) ||
		!check_height(cfgParam->outMaxHeight) ||
		cfgParam->outDepth != CELL_VPOST_PIC_DEPTH_8 ||
		cfgParam->outPicFmt > CELL_VPOST_PIC_FMT_OUT_YUV420_PLANAR ||
		cfgParam->reserved1 != 0 ||
		cfgParam->reserved2 != 0)
	{
		return false;
	}

	return true;
}

bool check_resource(const CellVpostResource* resource)
{
	if (!resource->memAddr || resource->ppuThreadPriority + 0U >= 3072 || resource->spuThreadPriority + 0U >= 256 || resource->numOfSpus != 1)
	{
		return false;
	}
	return true;
}

bool check_resource_ex(const CellVpostResourceEx* resource)
{
	if (!resource->memAddr || !resource->spurs_addr || resource->maxContention != 1)
	{
		return false;
	}

	for (int i = 0; i < 8; i++)
	{
		// TODO: Is this correct ?
		if (!resource->priority[i] || resource->priority[i] > 0xf)
		{
			return false;
		}
	}

	return true;
}

u32 get_YUV420_size(u32 width, u32 height, u32 bit_depth)
{
	const u32 bytes_per_pixel    = (bit_depth + 7U) / 8U;
	const u32 aligned_width      = utils::align(width, 16);
	const u32 aligned_half_width = utils::align(((width + 1) / 2), 16);
	const u32 half_height        = (height + 2) / 2;

	return utils::align(bytes_per_pixel * (aligned_width * height + (aligned_half_width * half_height * 2)), 16);
}

uint get_frame_size(const CellVpostCfgParam* cfgParam)
{
	constexpr u32 bit_depth = 8; // CellVpostPictureDepth is always CELL_VPOST_PIC_DEPTH_8
	const u32 bytes_per_pixel = (bit_depth + 7U) / 8U;
	const u32 in_size = get_YUV420_size(cfgParam->inMaxWidth, cfgParam->inMaxHeight, bit_depth);

	if (cfgParam->outPicFmt == 4 || cfgParam->outPicFmt == 7)
	{
		const u32 aligned_width   = utils::align(cfgParam->outMaxWidth.get(), 128);
		const u32 size = bytes_per_pixel * (((int)(aligned_width << 4) - aligned_width) + aligned_width * cfgParam->outMaxHeight * 3);
		return std::max(in_size, size);
	}

	if (cfgParam->outPicFmt == 3 || cfgParam->outPicFmt == 6)
	{
		const u32 aligned_width      = utils::align(cfgParam->outMaxWidth.get(), 128);
		const u32 aligned_half_width = utils::align((cfgParam->outMaxWidth + 1) / 2, 128);
		const u32 size = bytes_per_pixel * (cfgParam->outMaxHeight * aligned_half_width * 2 + ((int)(aligned_half_width << 5) - aligned_half_width) + cfgParam->outMaxHeight * aligned_width);
		return std::max(in_size, size);
	}

	const u32 size = get_YUV420_size(cfgParam->outMaxWidth, cfgParam->outMaxHeight, bit_depth);
	return std::max(in_size, size);
}

error_code cellVpostQueryAttr(vm::cptr<CellVpostCfgParam> cfgParam, vm::ptr<CellVpostAttr> attr)
{
	cellVpost.warning("cellVpostQueryAttr(cfgParam=*0x%x, attr=*0x%x)", cfgParam, attr);

	if (!cfgParam)
	{
		return CELL_VPOST_ERROR_Q_ARG_CFG_NULL;
	}

	if (!check_cfg_param(cfgParam.get_ptr()))
	{
		return CELL_VPOST_ERROR_Q_ARG_CFG_INVALID;
	}

	if (!attr)
	{
		return CELL_VPOST_ERROR_Q_ARG_ATTR_NULL;
	}

	const u32 frame_size = get_frame_size(cfgParam.get_ptr());

	attr->memSize       = frame_size * 2 + local_80 * 4 + 17533;
	attr->delay         = 0; // TODO: maybe also something * 4;
	attr->vpostVerLower = 0x280000; // TODO: this value is from dmux and not from vpost
	attr->vpostVerUpper = 0x40000;

	return CELL_OK;
}

error_code cellVpostOpen(vm::cptr<CellVpostCfgParam> cfgParam, vm::cptr<CellVpostResource> resource, vm::ptr<u32> handle)
{
	cellVpost.warning("cellVpostOpen(cfgParam=*0x%x, resource=*0x%x, handle=*0x%x)", cfgParam, resource, handle);

	if (!cfgParam)
	{
		return CELL_VPOST_ERROR_O_ARG_CFG_NULL;
	}

	if (!check_cfg_param(cfgParam.get_ptr()))
	{
		return CELL_VPOST_ERROR_O_ARG_CFG_INVALID;
	}

	if (!resource)
	{
		return CELL_VPOST_ERROR_O_ARG_RSRC_NULL;
	}

	if (!check_resource(resource.get_ptr()))
	{
		return CELL_VPOST_ERROR_O_ARG_RSRC_INVALID;
	}

	vm::var<CellVpostAttr> local_attr; // TODO: move to fxo

	if (error_code error = cellVpostQueryAttr(cfgParam, local_attr))
	{
		return CELL_VPOST_ERROR_O_FATAL_QUERY_FAIL;
	}

	if (local_attr->memSize > resource->memSize)
	{
		return CELL_VPOST_ERROR_O_ARG_RSRC_INVALID;
	}

	if (!handle)
	{
		return CELL_VPOST_ERROR_O_ARG_HDL_NULL;
	}

	if (false) // sys_lwmutex_create
	{
		return CELL_VPOST_ERROR_O_FATAL_CREATEMON_FAIL;
	}

	if (false) // initialize spurs
	{
		return CELL_VPOST_ERROR_O_FATAL_INITSPURS_FAIL;
	}

	*handle = idm::make<VpostInstance>(cfgParam->outPicFmt == CELL_VPOST_PIC_FMT_OUT_RGBA_ILV);

	handle[0x00] = idm::get<VpostInstance>(*handle);
	handle[0x01] = 0x4080;
	handle[0x02] = sys_lwmutex_t;
	handle[0x09] = cfgParam->inMaxWidth;
	handle[0x0a] = cfgParam->inMaxHeight;
	handle[0x0b] = cfgParam->inDepth;
	handle[0x0c] = cfgParam->inPicFmt;
	handle[0x0d] = cfgParam->outMaxWidth;
	handle[0x0e] = cfgParam->outMaxHeight;
	handle[0x0f] = cfgParam->outDepth;
	handle[0x10] = cfgParam->outPicFmt;
	handle[0x11] = cfgParam->reserved1;
	handle[0x12] = cfgParam->reserved2;
	handle[0x13] = resource->memAddr;
	handle[0x14] = resource->memSize;
	handle[0x15] = resource->ppuThreadPriority;
	handle[0x16] = resource->ppuThreadStackSize;
	handle[0x17] = resource->spuThreadPriority;
	handle[0x18] = resource->numOfSpus;
	handle[0x19] = handle[0x20];
	handle[0x1c] = resource->numOfSpus;
	handle[0x1a] = 1; // spu priority 0;
	handle[0x69] = 1; // spu priority 1;
	handle[0x6a] = 1; // spu priority 2;
	handle[0x6b] = 1; // spu priority 3;
	handle[0x1b] = 1; // spu priority 4;
	handle[0x6d] = 1; // spu priority 5;
	handle[0x6e] = 1; // spu priority 6;
	handle[0x6f] = 1; // spu priority 7;

	return CELL_OK;
}

error_code cellVpostOpenEx(vm::cptr<CellVpostCfgParam> cfgParam, vm::cptr<CellVpostResourceEx> resource, vm::ptr<u32> handle)
{
	cellVpost.warning("cellVpostOpenEx(cfgParam=*0x%x, resource=*0x%x, handle=*0x%x)", cfgParam, resource, handle);

	if (!cfgParam)
	{
		return CELL_VPOST_ERROR_O_ARG_CFG_NULL;
	}

	if (!check_cfg_param(cfgParam.get_ptr()))
	{
		return CELL_VPOST_ERROR_O_ARG_CFG_INVALID;
	}

	if (!resource)
	{
		return CELL_VPOST_ERROR_O_ARG_RSRC_NULL;
	}

	if (!check_resource_ex(resource.get_ptr()))
	{
		return CELL_VPOST_ERROR_O_ARG_RSRC_INVALID;
	}

	vm::var<CellVpostAttr> local_attr; // TODO: move to fxo

	if (error_code error = cellVpostQueryAttr(cfgParam, local_attr))
	{
		return CELL_VPOST_ERROR_O_FATAL_QUERY_FAIL;
	}

	if (local_attr->memSize > resource->memSize)
	{
		return CELL_VPOST_ERROR_O_ARG_RSRC_INVALID;
	}

	if (!handle)
	{
		return CELL_VPOST_ERROR_O_ARG_HDL_NULL;
	}

	if (false) // sys_lwmutex_create
	{
		return CELL_VPOST_ERROR_O_FATAL_CREATEMON_FAIL;
	}

	if (false) // initialize spurs
	{
		return CELL_VPOST_ERROR_O_FATAL_INITSPURS_FAIL;
	}

	*handle = idm::make<VpostInstance>(cfgParam->outPicFmt == CELL_VPOST_PIC_FMT_OUT_RGBA_ILV);

	handle[0x00] = idm::get<VpostInstance>(*handle);
	handle[0x01] = 0x4080;
	handle[0x02] = sys_lwmutex_t;
	handle[0x09] = cfgParam->inMaxWidth;
	handle[0x0a] = cfgParam->inMaxHeight;
	handle[0x0b] = cfgParam->inDepth;
	handle[0x0c] = cfgParam->inPicFmt;
	handle[0x0d] = cfgParam->outMaxWidth;
	handle[0x0e] = cfgParam->outMaxHeight;
	handle[0x0f] = cfgParam->outDepth;
	handle[0x10] = cfgParam->outPicFmt;
	handle[0x11] = cfgParam->reserved1;
	handle[0x12] = cfgParam->reserved2;
	handle[0x13] = resource->memAddr;
	handle[0x14] = resource->memSize;
	handle[0x15] = 1200;                    // ppu thread priority
	handle[0x16] = 0x2800;                  // ppu thread stack size
	handle[0x17] = 120;                     // spu thread priority
	handle[0x18] = resource->maxContention; // num of spurs
	handle[0x19] = resource->spurs_addr;
	handle[0x1c] = resource->maxContention; // num of spurs
	handle[0x1a] = resource->priority[0];
	handle[0x69] = resource->priority[1];
	handle[0x6a] = resource->priority[2];
	handle[0x6b] = resource->priority[3];
	handle[0x1b] = resource->priority[4];
	handle[0x6d] = resource->priority[5];
	handle[0x6e] = resource->priority[6];
	handle[0x6f] = resource->priority[7];

	return CELL_OK;
}

error_code cellVpostClose(u32 handle)
{
	cellVpost.warning("cellVpostClose(handle=0x%x)", handle);

	if (false && !handle) // Error disabled due to idm
	{
		return CELL_VPOST_ERROR_C_ARG_HDL_NULL;
	}

	const auto vpost = idm::get<VpostInstance>(handle);

	if (!vpost)
	{
		return CELL_VPOST_ERROR_C_ARG_HDL_INVALID;
	}

	if (false) // sys_lwmutex_lock
	{
		return CELL_VPOST_ERROR_C_FATAL_LOCKMON_FAIL;
	}

	handle[0x00] = 0; // handle
	handle[0x01] = 0; // ?
	handle[0x08] = 0x7fffffff; // ?

	if (false) // finalize cellSpurs
	{
		return CELL_VPOST_ERROR_C_FATAL_FINSPURS_FAIL;
	}

	if (false) // sys_lwmutex_unlock
	{
		return CELL_VPOST_ERROR_C_FATAL_UNLOCKMON_FAIL;
	}

	if (false) // sys_lwmutex_destroy
	{
		return CELL_VPOST_ERROR_C_FATAL_DESTROYMON_FAIL;
	}

	idm::remove<VpostInstance>(handle);
	return CELL_OK;
}

bool check_window(const CellVpostWindow* window, uint width, uint height)
{
	if (window->x < width &&
		window->y < height &&
		window->width >= 32 &&
		window->height >= 32 &&
		(window->x & 1) == 0 &&
		(window->y & 1) == 0 &&
		(window->width & 1) == 0 &&
		(window->height & 1) == 0 &&
		(window->x + window->width <= width) &&
		(window->y + window->height <= height))
	{
		return true;
	}

	return false;
}

undefined4 FUN_00001eb0(uint maxWidth, CellVpostCtrlParam* ctrlParam)
{
	if (ctrlParam->inWindow.x == 0 &&
		ctrlParam->inWindow.y == 0 &&
		ctrlParam->outWindow.x == 0 &&
		ctrlParam->outWindow.y == 0 &&
		ctrlParam->inWindow.width == ctrlParam->inWidth &&
		ctrlParam->inWindow.height == ctrlParam->inHeight &&
		ctrlParam->outWindow.width == ctrlParam->outWidth &&
		ctrlParam->outWindow.height == ctrlParam->outHeight &&
		ctrlParam->inWindow.width == ctrlParam->outWindow.width &&
		ctrlParam->inWindow.height == ctrlParam->outWindow.height &&
		((7 < (ulonglong) * (uint*)(maxWidth + 0x1c) ||
		((1 << ((ulonglong) * (uint*)(maxWidth + 0x1c) & 0x7f) & 0xd8U) == 0))))))
	{
		return 0;
	}
	return 1;
}

int FUN_00001fa0(u32* handle, u8* inPicBuf, CellVpostCtrlParam* ctrlParam, u8* outPicBuf, u32* width)
{
	if (1 < ctrlParam->execType + ~CELL_VPOST_EXEC_TYPE_PBTM_IBTM)
	{
		if (2 < ctrlParam->execType + ~CELL_VPOST_EXEC_TYPE_IBTM_PFRM)
		{
			uVar7  = 1;
			uVar8  = 0;
			uVar13 = uVar3 - 0xa0;
			goto LAB_000020a8;
		}
		cVar6  = FUN_00001eb0(handle[0x09], ctrlParam);
		puVar2 = puVar4;
		if (cVar6)
		{
			uVar7  = 1;
			uVar8  = 0;
			uVar13 = uVar3 - 0xa0;
			goto LAB_000020a8;
		}
	}
	puVar4 = puVar2;
	uVar9   = uVar3 - 0xa0;
	uVar7   = 0;
	uVar8   = 1;
	uVar13  = uVar11;
LAB_000020a8:
	cVar6 = FUN_00001eb0(handle[0x09], ctrlParam);
	if (cVar6)
	{
		if (handle[0x10] < 8)
		{
			return iVar5;
		}
		uVar9   = (uVar8 ^ 1) * 0x20 + uVar11;
		uVar13  = (uVar7 ^ 1) * 0x20 + uVar11;
	}
	FUN_00001f68(handle[0x0f]);
	if (handle[0x10] > 7)
	{
		return iVar5;
	}
	return CELL_OK;
}


error_code cellVpostExec(u32 handle, vm::cptr<u8> inPicBuff, vm::cptr<CellVpostCtrlParam> ctrlParam, vm::ptr<u8> outPicBuff, vm::ptr<CellVpostPictureInfo> picInfo)
{
	cellVpost.trace("cellVpostExec(handle=0x%x, inPicBuff=*0x%x, ctrlParam=*0x%x, outPicBuff=*0x%x, picInfo=*0x%x)", handle, inPicBuff, ctrlParam, outPicBuff, picInfo);

	if (false && !handle) // Error disabled due to idm
	{
		return CELL_VPOST_ERROR_E_ARG_HDL_NULL;
	}

	const auto vpost = idm::get<VpostInstance>(handle);

	if (!vpost)
	{
		return CELL_VPOST_ERROR_E_ARG_HDL_INVALID;
	}

	if (false) // sys_lwmutex_lock
	{
		return CELL_VPOST_ERROR_E_FATAL_LOCKMON_FAIL;
	}

	if (!inPicBuff)
	{
		return CELL_VPOST_ERROR_E_ARG_INPICBUF_NULL;
	}

	if (!inPicBuff.aligned(128))
	{
		return CELL_VPOST_ERROR_E_ARG_INPICBUF_INVALID;
	}

	if (!ctrlParam)
	{
		return CELL_VPOST_ERROR_E_ARG_CTRL_NULL;
	}

	if (ctrlParam->execType > CELL_VPOST_EXEC_TYPE_IBTM_IBTM ||
		ctrlParam->scalerType > CELL_VPOST_SCALER_TYPE_8X4TAP ||
		ctrlParam->ipcType > CELL_VPOST_IPC_TYPE_MAVG ||
		ctrlParam->inChromaPosType > CELL_VPOST_CHROMA_POS_TYPE_B ||
		ctrlParam->inQuantRange > CELL_VPOST_QUANT_RANGE_BROADCAST ||
		ctrlParam->inColorMatrix > CELL_VPOST_COLOR_MATRIX_BT709 ||
		!check_width(ctrlParam->inWidth) ||
		!check_height(ctrlParam->inHeight) ||
		!check_window(&ctrlParam->inWindow, ctrlParam->inWidth, ctrlParam->inHeight) ||
		!check_width(ctrlParam->outWidth) ||
		!check_height(ctrlParam->outHeight) ||
		!check_window(&ctrlParam->outWindow, ctrlParam->outWidth, ctrlParam->outHeight) ||
		ctrlParam->inWidth > handle[0x09] ||
		ctrlParam->inHeight > handle[0x0a] ||
		ctrlParam->outWidth > handle[0x0d] ||
		ctrlParam->outHeight > handle[0x0e] ||
		ctrlParam->reserved1 != 0||
		ctrlParam->reserved2 != 0)
	{
		return CELL_VPOST_ERROR_E_ARG_CTRL_INVALID;
	}

	if (!outPicBuff)
	{
		return CELL_VPOST_ERROR_E_ARG_OUTPICBUF_NULL;
	}

	if (!outPicBuff.aligned(128))
	{
		return CELL_VPOST_ERROR_E_ARG_OUTPICBUF_INVALID;
	}

	if (!picInfo)
	{
		return CELL_VPOST_ERROR_E_ARG_PICINFO_NULL;
	}

	if (FUN_00001fa0(handle, inPicBuff, ctrlParam, outPicBuff, &ctrlParam->inWidth))
	{
		return CELL_VPOST_ERROR_E_ARG_CTRL_INVALID;
	}

	if (ctrlParam->execType == CELL_VPOST_EXEC_TYPE_IFRM_IFRM)
	{
		CellVpostCtrlParam ctrl_param{};
		memcpy(&ctrl_param, ctrlParam.get_ptr(), sizeof(CellVpostCtrlParam));
		ctrl_param.execType = CELL_VPOST_EXEC_TYPE_IBTM_IBTM;

		if (FUN_00001fa0(handle, inPicBuff, ctrlParam, outPicBuff, &ctrlParam->inWidth))
		{
			return CELL_VPOST_ERROR_E_ARG_CTRL_INVALID;
		}
	}

	picInfo->inWidth         = ctrlParam->inWidth;
	iVar19                   = ctrlParam->execType * 4 + iVar19;
	picInfo->inHeight        = ctrlParam->inHeight;
	picInfo->outPicFmt       = handle[0x10];
	picInfo->inDepth         = handle[0x0b];
	picInfo->inColorMatrix   = ctrlParam->inColorMatrix;
	picInfo->outWidth        = ctrlParam->outWidth;
	picInfo->inPicFmt        = handle[0x0c];
	picInfo->inScanType      = *(CellVpostScanType*)(iVar19 + 0x60);
	picInfo->inPicStruct     = *(CellVpostPictureStructure*)(iVar19 + 0x80);
	picInfo->inChromaPosType = ctrlParam->inChromaPosType;
	picInfo->inQuantRange    = ctrlParam->inQuantRange;
	picInfo->outHeight       = ctrlParam->outHeight;
	picInfo->outDepth        = handle[0x0f];
	picInfo->outScanType     = *(CellVpostScanType*)(iVar19 + 0xa0);
	picInfo->outPicStruct    = *(CellVpostPictureStructure*)(iVar19 + 0xc0);
	picInfo->outColorMatrix  = ctrlParam->inColorMatrix;
	picInfo->outChromaPosType = ctrlParam->inChromaPosType;
	picInfo->outQuantRange    = ctrlParam->inQuantRange;

	if ((u64)handle[0x10] < 8)
	{
		uVar11 = 1 << ((u64)handle[0x10] & 0x7f);

		if ((uVar11 & 0xd8) != 0)
		{
			picInfo->outChromaPosType = CELL_VPOST_CHROMA_POS_TYPE_B;
			picInfo->outQuantRange    = ctrlParam->inQuantRange;
		}
		else if ((uVar11 & 1) != 0)
		{
			picInfo->outChromaPosType = ctrlParam->inChromaPosType;
			picInfo->outQuantRange    = CELL_VPOST_QUANT_RANGE_FULL;
		}
	}

	picInfo->userData  = ctrlParam->userData;
	picInfo->reserved1 = 0;
	picInfo->reserved2 = 0;

	if (false) // sys_lwmutex_unlock
	{
		return CELL_VPOST_ERROR_E_FATAL_UNLOCKMON_FAIL;
	}


	u32 w = ctrlParam->inWidth;
	u32 h = ctrlParam->inHeight;
	u32 ow = ctrlParam->outWidth;
	u32 oh = ctrlParam->outHeight;

	picInfo->inWidth = w; // copy
	picInfo->inHeight = h; // copy
	picInfo->inDepth = CELL_VPOST_PIC_DEPTH_8; // fixed
	picInfo->inScanType = CELL_VPOST_SCAN_TYPE_P; // TODO
	picInfo->inPicFmt = CELL_VPOST_PIC_FMT_IN_YUV420_PLANAR; // fixed
	picInfo->inChromaPosType = ctrlParam->inChromaPosType; // copy
	picInfo->inPicStruct = CELL_VPOST_PIC_STRUCT_PFRM; // TODO
	picInfo->inQuantRange = ctrlParam->inQuantRange; // copy
	picInfo->inColorMatrix = ctrlParam->inColorMatrix; // copy

	picInfo->outWidth = ow; // copy
	picInfo->outHeight = oh; // copy
	picInfo->outDepth = CELL_VPOST_PIC_DEPTH_8; // fixed
	picInfo->outScanType = CELL_VPOST_SCAN_TYPE_P; // TODO
	picInfo->outPicFmt = CELL_VPOST_PIC_FMT_OUT_RGBA_ILV; // TODO
	picInfo->outChromaPosType = ctrlParam->inChromaPosType; // ignored
	picInfo->outPicStruct = picInfo->inPicStruct; // ignored
	picInfo->outQuantRange = ctrlParam->inQuantRange; // ignored
	picInfo->outColorMatrix = ctrlParam->inColorMatrix; // ignored

	picInfo->userData = ctrlParam->userData; // copy
	picInfo->reserved1 = 0;
	picInfo->reserved2 = 0;

	std::unique_ptr<u8[]> pA(new u8[w*h]);

	memset(pA.get(), ctrlParam->outAlpha, w*h);

	vpost->sws = sws_getCachedContext(vpost->sws, w, h, AV_PIX_FMT_YUVA420P, ow, oh, AV_PIX_FMT_RGBA, SWS_BILINEAR, nullptr, nullptr, nullptr);

	const u8* in_data[4] = { &inPicBuff[0], &inPicBuff[w * h], &inPicBuff[w * h * 5 / 4], pA.get() };
	int ws = w;
	int in_line[4] = { ws, ws/2, ws/2, ws };
	u8* out_data[4] = { outPicBuff.get_ptr(), nullptr, nullptr, nullptr };
	int out_line[4] = { static_cast<int>(ow * 4), 0, 0, 0 };

	sws_scale(vpost->sws, in_data, in_line, 0, h, out_data, out_line);

	return CELL_OK;
}

DECLARE(ppu_module_manager::cellVpost)("cellVpost", []()
{
	REG_FUNC(cellVpost, cellVpostQueryAttr);
	REG_FUNC(cellVpost, cellVpostOpen);
	REG_FUNC(cellVpost, cellVpostOpenEx);
	//REG_FUNC(cellVpost, cellVpostOpenExt); // 0x9f1795df
	REG_FUNC(cellVpost, cellVpostClose);
	REG_FUNC(cellVpost, cellVpostExec);
});
