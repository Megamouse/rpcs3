#include "stdafx.h"
#include "Emu/IdManager.h"
#include "Emu/Cell/PPUModule.h"
#include "Emu/Cell/Modules/cellGcmSys.h"
#include "Emu/Cell/lv2/sys_process.h"

#include "Emu/RSX/GCM.h"
#include "Emu/RSX/gcm_enums.h"
#include "cellResc.h"
#include "cellVideoOut.h"

error_code cellGcmSetSecondVFrequency(u32 freq);
error_code cellGcmSetVBlankHandler(vm::ptr<void(u32)> handler);
error_code cellGcmSetSecondVHandler(vm::ptr<void(u32)> handler);
u32 cellGcmGetFlipStatus();
u64 cellGcmGetLastFlipTime();
u32 cellGcmGetTiledPitchSize(u32 size);
void cellGcmResetFlipStatus();
void cellGcmSetFlipHandler(vm::ptr<void(u32)> handler);
void cellGcmSetFlipMode(u32 mode);
u32 cellGcmGetLabelAddress(u8 index);
error_code cellGcmSetPrepareFlip(ppu_thread& ppu, vm::ptr<CellGcmContextData> ctxt, u32 id);
error_code cellGcmAddressToOffset(u32 address, vm::ptr<u32> offset);
error_code cellGcmSetDisplayBuffer(u8 id, u32 offset, u32 pitch, u32 width, u32 height);

error_code cellVideoOutConfigure(u32 videoOut, vm::ptr<CellVideoOutConfiguration> config, vm::ptr<CellVideoOutOption> option, u32 waitForEvent);

LOG_CHANNEL(cellResc);

template <>
void fmt_class_string<CellRescError>::format(std::string& out, u64 arg)
{
	format_enum(out, arg, [](CellRescError value)
	{
		switch (value)
		{
		STR_CASE(CELL_RESC_ERROR_NOT_INITIALIZED);
		STR_CASE(CELL_RESC_ERROR_REINITIALIZED);
		STR_CASE(CELL_RESC_ERROR_BAD_ALIGNMENT);
		STR_CASE(CELL_RESC_ERROR_BAD_ARGUMENT);
		STR_CASE(CELL_RESC_ERROR_LESS_MEMORY);
		STR_CASE(CELL_RESC_ERROR_GCM_FLIP_QUE_FULL);
		STR_CASE(CELL_RESC_ERROR_BAD_COMBINATION);
		STR_CASE(CELL_RESC_ERROR_x308);
		}

		return unknown;
	});
}

u32 get_dst_index_by_buffer_mode(u32 buffer_mode)
{
	switch (buffer_mode)
	{
	case CELL_RESC_720x480: return 0;
	case CELL_RESC_720x576: return 1;
	case CELL_RESC_1280x720: return 2;
	case CELL_RESC_1920x1080: return 3;
	default: fmt::throw_exception("unexpected buffer mode 0x%x", buffer_mode);
	}
}

void get_buffer_dimensions(u32 bufferMode, u32& width, u32& height)
{
	switch (bufferMode)
	{
	case CELL_RESC_720x480:
		width  = 720;
		height = 480;
		break;
	case CELL_RESC_720x576:
		width  = 720;
		height = 576;
		break;
	case CELL_RESC_1280x720:
		width  = 1280;
		height = 720;
		break;
	case CELL_RESC_1920x1080:
		width  = 1920;
		height = 1080;
		break;
	default:
		width  = 0;
		height = 0;
		break;
	}
}

u32 get_color_buffers_count(u32 bufferMode, u32 palTemporalMode)
{
	if (bufferMode == CELL_RESC_720x576)
	{
		if (palTemporalMode >= CELL_RESC_PAL_60_INTERPOLATE &&
			palTemporalMode <= CELL_RESC_PAL_60_INTERPOLATE_DROP_FLEXIBLE)
		{
			return 6;
		}
		else if (palTemporalMode == CELL_RESC_PAL_60_DROP)
		{
			return 3;
		}
	}

	return 2;
}

s32 get_color_buffer_size(u32 bufferMode, const CellRescDsts& dsts)
{
	u32 width = 0, height = 0;
	get_buffer_dimensions(bufferMode, width, height);

	return ((height + dsts.heightAlign) - 1 & -dsts.heightAlign) * dsts.pitch;
}

s32 get_max_color_buffer_size()
{
	auto& resc_manager = g_fxo->get<cell_resc_manager>();

	s32 size = 0;

	for (u32 bufferMode : {CELL_RESC_720x480, CELL_RESC_720x576, CELL_RESC_1280x720, CELL_RESC_1920x1080})
	{
		if ((bufferMode & resc_manager.config.supportModes) == 0) continue;

		const u32 dst_index = get_dst_index_by_buffer_mode(bufferMode);
		const s32 buffer_size = get_color_buffer_size(bufferMode, ::at32(resc_manager.dsts, dst_index));
		const s32 num_buffers = cellRescGetNumColorBuffers(bufferMode, resc_manager.config.palTemporalMode, 0);

		size = std::max(size, buffer_size * num_buffers);
	}

	return size;
}

u8 get_texture_format(u8 colorFormat, u8 type)
{
	u8 texture_format = 0xff;

	if (colorFormat == CELL_GCM_SURFACE_A8R8G8B8)
	{
		texture_format = CELL_GCM_TEXTURE_A8R8G8B8;
	}
	else if (colorFormat == CELL_GCM_SURFACE_F_W16Z16Y16X16)
	{
		texture_format = CELL_GCM_TEXTURE_W16_Z16_Y16_X16_FLOAT;
	}
	else
	{
		return 0xff;
	}

	if (type == CELL_GCM_SURFACE_PITCH)
	{
		texture_format |= CELL_GCM_TEXTURE_LN;
	}
	else if (type != CELL_GCM_SURFACE_SWIZZLE)
	{
		return 0xff;
	}

	return texture_format;
}

void fill_vertex_array()
{
	auto& resc_manager = g_fxo->get<cell_resc_manager>();

	const float half_texel_x = 0.5f / resc_manager.horizontal;
	const float half_texel_y = 0.5f / resc_manager.vertical;

	const float tex_coord_min_x = 0.5f - half_texel_x;
	const float tex_coord_max_x = 0.5f + half_texel_x;

	const float tex_coord_min_y = 0.5f - half_texel_y;
	const float tex_coord_max_y = 0.5f + half_texel_y;

	f32* vertexArray = reinterpret_cast<f32*>(resc_manager.vertexArray.get_ptr());

	if (resc_manager.bufferMode == CELL_RESC_720x480 || resc_manager.bufferMode == CELL_RESC_720x576)
	{
		if (resc_manager.config.ratioMode == CELL_RESC_LETTERBOX)
		{
			float letterbox_offset_y = (2.0f / 3.0f) / resc_manager.vertical;

			if (resc_manager.config.size != 28)
			{
				letterbox_offset_y /= 1.062091588973999f;
			}

			const float letterbox_tex_coord_min_y = 0.5f - letterbox_offset_y;
			const float letterbox_tex_coord_max_y = 0.5f + letterbox_offset_y;

			vertexArray[0] = -1.0f;
			vertexArray[1] = 1.0f;
			vertexArray[2] = tex_coord_min_x;
			vertexArray[3] = letterbox_tex_coord_min_y;
			vertexArray[4] = 0;
			vertexArray[5] = 0;

			vertexArray[6] = 1.0f;
			vertexArray[7] = 1.0f;
			vertexArray[8] = tex_coord_max_x;
			vertexArray[9] = letterbox_tex_coord_min_y;
			vertexArray[10] = 1.0f;
			vertexArray[11] = 0;

			vertexArray[12] = 1.0f;
			vertexArray[13] = -1.0f;
			vertexArray[14] = tex_coord_max_x;
			vertexArray[15] = letterbox_tex_coord_max_y;
			vertexArray[16] = 1.0f;
			vertexArray[17] = 1.0f;

			vertexArray[18] = -1.0f;
			vertexArray[19] = -1.0f;
			vertexArray[20] = tex_coord_min_x;
			vertexArray[21] = letterbox_tex_coord_max_y;
			vertexArray[22] = 0;
			vertexArray[23] = 1.0f;

			*(undefined4*)&state->field_0x140 = 4;
			return;
		}

		if (resc_manager.config.ratioMode == CELL_RESC_PANSCAN)
		{
			const float panscan_offset_x = 0.375f / resc_manager.horizontal;

			const float panscan_tex_coord_min_x = 0.5f - panscan_offset_x;
			const float panscan_tex_coord_max_x = 0.5f + panscan_offset_x;

			vertexArray[0] = -1.0f;
			vertexArray[1] = 1.0f;
			vertexArray[2] = panscan_tex_coord_min_x;
			vertexArray[3] = tex_coord_min_y;
			vertexArray[4] = 0;
			vertexArray[5] = 0;

			vertexArray[6] = 1.0f;
			vertexArray[7] = 1.0f;
			vertexArray[8] = panscan_tex_coord_max_x;
			vertexArray[9] = tex_coord_min_y;
			vertexArray[10] = 1.0f;
			vertexArray[11] = 0;

			vertexArray[12] = 1.0f;
			vertexArray[13] = -1.0f;
			vertexArray[14] = panscan_tex_coord_max_x;
			vertexArray[15] = tex_coord_max_y;
			vertexArray[16] = 1.0f;
			vertexArray[17] = 1.0f;

			vertexArray[18] = -1.0f;
			vertexArray[19] = -1.0f;
			vertexArray[20] = panscan_tex_coord_min_x;
			vertexArray[21] = tex_coord_max_y;
			vertexArray[22] = 0;
			vertexArray[23] = 1.0f;

			*(undefined4*)&state->field_0x140 = 4;
			return;
		}
	}

	vertexArray[0] = -1.0f;
	vertexArray[1] = 1.0f;
	vertexArray[2] = tex_coord_min_x;
	vertexArray[3] = tex_coord_min_y;
	vertexArray[4] = 0;
	vertexArray[5] = 0;

	vertexArray[6] = 1.0f;
	vertexArray[7] = 1.0f;
	vertexArray[8] = tex_coord_max_x;
	vertexArray[9] = tex_coord_min_y;
	vertexArray[10] = 1.0f;
	vertexArray[11] = 0;

	vertexArray[12] = 1.0f;
	vertexArray[13] = -1.0f;
	vertexArray[14] = tex_coord_max_x;
	vertexArray[15] = tex_coord_max_y;
	vertexArray[16] = 1.0f;
	vertexArray[17] = 1.0f;

	vertexArray[18] = -1.0f;
	vertexArray[19] = -1.0f;
	vertexArray[20] = tex_coord_min_x;
	vertexArray[21] = tex_coord_max_y;
	vertexArray[22] = 0;
	vertexArray[23] = 1.0f;

	*(undefined4*)&state->field_0x140 = 4;
}

void init_config()
{
	auto& resc_manager = g_fxo->get<cell_resc_manager>();

	undefined8 uVar2;
	undefined4* puVar4;
	int iVar5;
	longlong lVar6;

	*(undefined4*)&state->field_0x1d0 = 2;
	*(undefined4*)&state->field_0x1cc = 5;
	*(undefined4*)&state->field_0x140 = 0;
	*(undefined4*)&state->field_0x1c8 = 4;
	resc_manager.bufferMode = static_cast<CellRescBufferMode>(0);
	resc_manager.tableLength = 1;
	*(undefined4*)&state->field_0x110 = 0;
	*(undefined2*)&state->field_0x120 = 0;
	*(undefined2*)&state->field_0x122 = 0;
	state->field_0x151 = 0;
	*(undefined8*)&state->field_0x168 = 0;
	*(undefined8*)&state->field_0x1b8 = 0;
	*(undefined8*)&state->field_0x1c0 = 0;
	*(undefined4*)&state->field_0x1d4 = 0;
	*(undefined8*)&state->field_0x1e8 = 1;
	*(undefined8*)&state->field_0x1f0 = 0;
	*(undefined8*)&state->field_0x1f8 = 0;
	uVar2 = FUN_0000802c();
	*(undefined8*)&state[1].config.supportModes = 0;
	*(undefined8*)&state[1].config = uVar2;
	uVar2 = FUN_0000802c();
	puVar4 = (undefined4*)&state->field_0x1c;
	*(undefined8*)&state[1].config.palTemporalMode = uVar2;
	state[1].config.flipMode = 1;
	lVar6 = 0xf;
	iVar5 = 0;
	do
	{
		(&state->field_0x152)[iVar5] = 0xff;
		lVar6 = lVar6 + -1;
		iVar5 = iVar5 + 1;
	} while (lVar6 != 0);

	for (u32 i = 0; i < 8; i++)
	{
		resc_manager.srcs[i].format = 0;
		resc_manager.srcs[i].pitch = 0;
		resc_manager.srcs[i].width = 0;
		resc_manager.srcs[i].height = 0;
		resc_manager.srcs[i].offset = 0;
	}

	resc_manager.dsts[0].format = CELL_RESC_SURFACE_A8R8G8B8;
	resc_manager.dsts[0].heightAlign = 8;
	resc_manager.dsts[0].pitch = cellGcmGetTiledPitchSize(0xb40);

	resc_manager.dsts[1].format = CELL_RESC_SURFACE_A8R8G8B8;
	resc_manager.dsts[1].heightAlign = 8;
	resc_manager.dsts[1].pitch = cellGcmGetTiledPitchSize(0xb40);

	resc_manager.dsts[2].format = CELL_RESC_SURFACE_A8R8G8B8;
	resc_manager.dsts[2].heightAlign = 8;
	resc_manager.dsts[2].pitch = cellGcmGetTiledPitchSize(0x1400);

	resc_manager.dsts[3].format = CELL_RESC_SURFACE_A8R8G8B8;
	resc_manager.dsts[3].heightAlign = 8;
	resc_manager.dsts[3].pitch = cellGcmGetTiledPitchSize(0x1e00);

	resc_manager.colorBuffers = vm::null;
	resc_manager.vertexArray = vm::null;
	resc_manager.fragmentShader = vm::null;
	resc_manager.table = 0;

	resc_manager.width = 0;
	resc_manager.height = 0;
	resc_manager.pitch = 0;
	resc_manager.bufferSize = 0;

	for (u32 i = 0; i < MAX_DST_BUFFER_NUM; i++)
	{
		resc_manager.buffersOffsets[i] = 0;
	}

	resc_manager.depth = 32;
	resc_manager.horizontal = 1.0f;
	resc_manager.vertical = 1.0f;

	resc_manager.lastFlipTime = 0;
	resc_manager.flipStatus = 1;

	resc_manager.palInterpolateDropFlexRatio = 0.0f;

	resc_manager.is_initialized = 0;
}

error_code cellRescInit(vm::cptr<CellRescInitConfig> initConfig)
{
	cellResc.todo("cellRescInit(initConfig=*0x%x)", initConfig);

	auto& resc_manager = g_fxo->get<cell_resc_manager>();

	if (resc_manager.is_initialized)
	{
		return CELL_RESC_ERROR_REINITIALIZED;
	}

	if (!initConfig || initConfig->size > 28 || (initConfig->size != 20 && initConfig->size != 24 && initConfig->size != 28))
	{
		return CELL_RESC_ERROR_BAD_ARGUMENT;
	}

	if ((initConfig->resourcePolicy & 0xfffffffc) != 0 ||
		(initConfig->supportModes & 0xf) == 0 ||
		(initConfig->ratioMode > CELL_RESC_PANSCAN) ||
		(initConfig->palTemporalMode > CELL_RESC_PAL_60_FOR_HSYNC))
	{
		return CELL_RESC_ERROR_BAD_ARGUMENT;
	}

	if (resc_manager.config.size == 24)
	{
		if (initConfig->flipMode > CELL_RESC_INTERLACE_FILTER)
		{
			return CELL_RESC_ERROR_BAD_ARGUMENT;
		}
	}
	else if (resc_manager.config.size == 28)
	{
		if (initConfig->interlaceMode > CELL_RESC_2X3_QUINCUNX_ALT ||
			initConfig->flipMode > CELL_RESC_INTERLACE_FILTER)
		{
			return CELL_RESC_ERROR_BAD_ARGUMENT;
		}
	}

	init_config();

	std::memset(&resc_manager.config, 0, sizeof(resc_manager.config));
	std::memcpy(&resc_manager.config, initConfig.get_ptr(), initConfig->size);

	if (*(int*)&state->field_0xd8 == 0)
	{
		iVar1 = FUN_00007b58(0, *(undefined4*)(iStack100 + -0x7f74));
		*(int*)&state->field_0xd8 = iVar1;
		if (iVar1 == 0)
		{
			return CELL_RESC_ERROR_LESS_MEMORY;
		}
	}
	if (*(int*)&state->field_0xdc == 0)
	{
		iVar1 = FUN_00007b58(1, *(undefined4*)(iStack100 + -0x7f70));
		*(int*)&state->field_0xdc = iVar1;
		if (iVar1 == 0)
		{
			return CELL_RESC_ERROR_LESS_MEMORY;
		}
	}
	if (*(int*)&state->field_0xe0 == 0)
	{
		iVar1 = FUN_00007b58(1, *(undefined4*)(iStack100 + -0x7f6c));
		*(int*)&state->field_0xe0 = iVar1;
		if (iVar1 == 0)
		{
			return CELL_RESC_ERROR_LESS_MEMORY;
		}
	}
	if (*(int*)&state->field_0xe4 == 0)
	{
		iVar1 = FUN_00007b58(1, *(undefined4*)(iStack100 + -0x7f68));
		*(int*)&state->field_0xe4 = iVar1;
		if (iVar1 == 0)
		{
			return CELL_RESC_ERROR_LESS_MEMORY;
		}
	}
	if (*(int*)&state->field_0xe8 == 0)
	{
		iVar1 = FUN_00007b58(1, *(undefined4*)(iStack100 + -0x7f64));
		*(int*)&state->field_0xe8 = iVar1;
		if (iVar1 == 0)
		{
			return CELL_RESC_ERROR_LESS_MEMORY;
		}
	}

	resc_manager.usedFragmentShader = *(undefined4*)&state->field_0xdc;
	FUN_00002490(state);

	resc_manager.is_initialized = true;

	return CELL_OK;
}

void cellRescExit()
{
	cellResc.todo("cellRescExit()");

	auto& resc_manager = g_fxo->get<cell_resc_manager>();
	if (!resc_manager.is_initialized) return;

	if (resc_manager.bufferMode == CELL_RESC_720x576)
	{
		if (resc_manager.config.palTemporalMode != CELL_RESC_PAL_50)
		{
			cellGcmSetSecondVFrequency(3);
			cellGcmSetVBlankHandler(vm::null);
			cellGcmSetSecondVHandler(vm::null);

			if (resc_manager.config.palTemporalMode >= CELL_RESC_PAL_60_INTERPOLATE)
			{
				iVar1 = FUN_00003210(state);
				if (iVar1 != 0)
				{
					FUN_00007c5c(*(undefined4*)(iStack100 + -0x7f78), iVar1);
				}
			}
		}
	}

	resc_manager.is_initialized = false;
}

error_code cellRescVideoOutResolutionId2RescBufferMode(u32 resolutionId, vm::ptr<u32> bufferMode)
{
	cellResc.trace("cellRescVideoOutResolutionId2RescBufferMode(resolutionId=0x%x, bufferMode=*0x%x)", resolutionId, bufferMode);

	if (!bufferMode)
	{
		return CELL_RESC_ERROR_BAD_ARGUMENT;
	}

	switch (resolutionId)
	{
	case CELL_VIDEO_OUT_RESOLUTION_1080: *bufferMode = CELL_RESC_1920x1080; break;
	case CELL_VIDEO_OUT_RESOLUTION_720: *bufferMode = CELL_RESC_1280x720; break;
	case CELL_VIDEO_OUT_RESOLUTION_480: *bufferMode = CELL_RESC_720x480; break;
	case CELL_VIDEO_OUT_RESOLUTION_576: *bufferMode = CELL_RESC_720x576; break;
	default: return CELL_RESC_ERROR_BAD_ARGUMENT;
	}

	return CELL_OK;
}

error_code cellRescSetDsts(u32 bufferMode, vm::cptr<CellRescDsts> dsts)
{
	cellResc.notice("cellRescSetDsts(bufferMode=0x%x, dsts=*0x%x)", bufferMode, dsts);

	auto& resc_manager = g_fxo->get<cell_resc_manager>();

	if (!resc_manager.is_initialized)
	{
		return CELL_RESC_ERROR_NOT_INITIALIZED;
	}

	if (!dsts)
	{
		return CELL_RESC_ERROR_BAD_ARGUMENT;
	}

	switch (bufferMode)
	{
	case CELL_RESC_720x480:
	case CELL_RESC_720x576:
	case CELL_RESC_1280x720:
	case CELL_RESC_1920x1080:
		break;
	default:
		return CELL_RESC_ERROR_BAD_ARGUMENT;
	}

	const u32 index = get_dst_index_by_buffer_mode(bufferMode);

	resc_manager.dsts[index].format = dsts->format;
	resc_manager.dsts[index].pitch = dsts->pitch;
	resc_manager.dsts[index].heightAlign = dsts->heightAlign;
	return CELL_OK;
}

error_code cellRescSetDisplayMode(u32 bufferMode)
{
	cellResc.todo("cellRescSetDisplayMode(bufferMode=0x%x)", bufferMode);

	auto& resc_manager = g_fxo->get<cell_resc_manager>();

	if (!resc_manager.is_initialized)
	{
		return CELL_RESC_ERROR_NOT_INITIALIZED;
	}

	if ((bufferMode != CELL_RESC_720x480 && bufferMode != CELL_RESC_720x576 && bufferMode != CELL_RESC_1280x720 && bufferMode != CELL_RESC_1920x1080) ||
		!(resc_manager.config.supportModes & bufferMode))
	{
		return CELL_RESC_ERROR_BAD_ARGUMENT;
	}

	resc_manager.bufferMode = static_cast<CellRescBufferMode>(bufferMode);

	if (bufferMode == CELL_RESC_720x576)
	{
		const u32 pal_mode  = resc_manager.config.palTemporalMode;
		const u32 flip_mode = resc_manager.config.flipMode;

		switch (pal_mode)
		{
		case CELL_RESC_PAL_60_DROP:
		case CELL_RESC_PAL_60_INTERPOLATE:
		case CELL_RESC_PAL_60_INTERPOLATE_30_DROP:
		case CELL_RESC_PAL_60_INTERPOLATE_DROP_FLEXIBLE:
		{
			if (flip_mode == CELL_RESC_DISPLAY_HSYNC)
			{
				return CELL_RESC_ERROR_BAD_COMBINATION;
			}
			break;
		}
		case CELL_RESC_PAL_60_FOR_HSYNC:
		{
			if (flip_mode == CELL_RESC_DISPLAY_VSYNC)
			{
				return CELL_RESC_ERROR_BAD_COMBINATION;
			}
			break;
		}
		default:
			break;
		}
	}

	int iStack132;

	const u32 dst_index = get_dst_index_by_buffer_mode(bufferMode);
	resc_manager.activeDst = dst_index;

	get_buffer_dimensions(resc_manager.bufferMode, resc_manager.width, resc_manager.height);

	const u32 pitch = resc_manager.dsts[resc_manager.activeDst].pitch;
	const u32 heightAlign = resc_manager.dsts[resc_manager.activeDst].heightAlign;

	resc_manager.pitch = pitch;
	resc_manager.bufferSize = pitch * ((heightAlign + resc_manager.height) - 1U & -heightAlign);

	if (resc_manager.bufferMode == CELL_RESC_720x576 &&
		resc_manager.config.palTemporalMode >= CELL_RESC_PAL_60_INTERPOLATE &&
		resc_manager.config.palTemporalMode <= CELL_RESC_PAL_60_INTERPOLATE_DROP_FLEXIBLE)
	{
		if (resc_manager.config.interlaceMode == CELL_RESC_INTERLACE_FILTER)
		{
			resc_manager.usedFragmentShader = *(undefined4*)&state->field_0xe8;
		}
		else
		{
			resc_manager.usedFragmentShader = *(undefined4*)&state->field_0xe0;
		}
	}
	else if (resc_manager.config.interlaceMode == CELL_RESC_INTERLACE_FILTER)
	{
		resc_manager.usedFragmentShader = *(undefined4*)&state->field_0xe4;
	}
	else
	{
		resc_manager.usedFragmentShader = *(undefined4*)&state->field_0xdc;
	}

	FUN_00007c60(resc_manager.bufferMode);
	FUN_00007c98(resc_manager.dsts[resc_manager.activeDst].format);

	vm::ptr<CellVideoOutConfiguration> videoOutConfig = vm::make_var<CellVideoOutConfiguration>({});
	std::memset(videoOutConfig.get_ptr(), 0, sizeof(CellVideoOutConfiguration));
	cellVideoOutConfigure(0, videoOutConfig, vm::null, 0);

	if (resc_manager.bufferMode == CELL_RESC_720x576)
	{
		switch (resc_manager.config.palTemporalMode)
		{
		case CELL_RESC_PAL_60_DROP:
		{
			u32 addr;
			addr = cellGcmGetLabelAddress(0x11);
			*addr = 0;
			addr = cellGcmGetLabelAddress(0x12);
			*addr = 0;
			addr = cellGcmGetLabelAddress(0x13);
			*addr = 0;
			cellGcmSetSecondVFrequency(1);
			cellGcmSetVBlankHandler(vm::null);
			cellGcmSetSecondVHandler(*(undefined4*)(iStack132 + -0x7f44));
			cellGcmSetFlipHandler(vm::null);
			break;
		}
		case CELL_RESC_PAL_60_INTERPOLATE:
		case CELL_RESC_PAL_60_INTERPOLATE_30_DROP:
		case CELL_RESC_PAL_60_INTERPOLATE_DROP_FLEXIBLE:
		{
			int iVar4 = iStack132;
			EVar5 = FUN_00003328(state);
			if (EVar5 != CELL_OK)
			{
				FUN_00007c5c(*(undefined4*)(iVar4 + -0x7f54), *(undefined4*)(iVar4 + -0x7f50), 0xfa, EVar5);
				return EVar5;
			}
			u32 addr;
			addr = cellGcmGetLabelAddress(0x11);
			*addr = 0;
			addr = cellGcmGetLabelAddress(0x12);
			*addr = 0;
			addr = cellGcmGetLabelAddress(0x13);
			*addr = 0;
			cellGcmSetSecondVFrequency(1);
			cellGcmSetVBlankHandler(*(undefined4*)(iStack132 + -0x7f4c));
			cellGcmSetSecondVHandler(*(undefined4*)(iStack132 + -0x7f48));
			cellGcmSetFlipHandler(vm::null);
			break;
		}
		case CELL_RESC_PAL_60_FOR_HSYNC:
		{
			cellGcmSetSecondVFrequency(1);
			cellGcmSetVBlankHandler(vm::null);
			break;
		}
		default:
			break;
		}
	}

	int iVar4 = *(int*)(iStack132 + -0x7fdc);
	int iVar2 = *(int*)(iVar4 + 8);
	if (iVar2 != 0)
	{
		FUN_00003794(state, iVar2);
	}

	CellRescHandler handler = *(CellRescHandler *)(iVar4 + 4);
	if (handler != FUN_00000000)
	{
		FUN_00003704(state, handler);
	}

	const u32 flip_mode = resc_manager.config.flipMode;
	ulonglong uVar6 = (ulonglong)((int)flip_mode >> 0x1f);
	cellGcmSetFlipMode(((longlong)(uVar6 - (uVar6 ^ flip_mode) << 0x20) >> 0x3f) + 2U & 0xffffffff);

	return CELL_OK;
}

error_code cellRescAdjustAspectRatio(f32 horizontal, f32 vertical)
{
	cellResc.todo("cellRescAdjustAspectRatio(horizontal=%f, vertical=%f)", horizontal, vertical);

	auto& resc_manager = g_fxo->get<cell_resc_manager>();

	if (!resc_manager.is_initialized)
	{
		return CELL_RESC_ERROR_NOT_INITIALIZED;
	}

	if (horizontal < 0.5f || horizontal > 2.0f || vertical < 0.5f || vertical > 2.0f)
	{
		return CELL_RESC_ERROR_BAD_ARGUMENT;
	}

	resc_manager.horizontal = horizontal;
	resc_manager.vertical = vertical;

	if (!resc_manager.vertexArray)
	{
		return CELL_OK;
	}

	if (resc_manager.config.interlaceMode == CELL_RESC_INTERLACE_FILTER)
	{
		state->field_0x151 = 1;
		return CELL_OK;
	}

	fill_vertex_array();
	return CELL_OK;
}

error_code cellRescSetPalInterpolateDropFlexRatio(f32 ratio)
{
	cellResc.notice("cellRescSetPalInterpolateDropFlexRatio(ratio=%f)", ratio);

	auto& resc_manager = g_fxo->get<cell_resc_manager>();

	if (!resc_manager.is_initialized)
	{
		return CELL_RESC_ERROR_NOT_INITIALIZED;
	}

	if (ratio < 0.0f || ratio > 1.0f)
	{
		return CELL_RESC_ERROR_BAD_ARGUMENT;
	}

	resc_manager.palInterpolateDropFlexRatio = ratio;

	return CELL_OK;
}

error_code cellRescGetBufferSize(vm::ptr<s32> colorBuffers, vm::ptr<s32> vertexArray, vm::ptr<s32> fragmentShader)
{
	cellResc.todo("cellRescGetBufferSize(colorBuffers=*0x%x, vertexArray=*0x%x, fragmentShader=*0x%x)", colorBuffers, vertexArray, fragmentShader);

	auto& resc_manager = g_fxo->get<cell_resc_manager>();

	if (!resc_manager.is_initialized)
	{
		return CELL_RESC_ERROR_NOT_INITIALIZED;
	}

	vm::var<s32> sdk_version;
	if (error_code error = sys_process_get_sdk_version(sys_process_getpid(), sdk_version); error != CELL_OK)
	{
		return CELL_RESC_ERROR_x308;
	}

	s32 colorBuffersSize = 0;
	s32 fragmentShaderSize = 0;

	if ((resc_manager.config.resourcePolicy & 1) == 0)
	{
		colorBuffersSize = get_max_color_buffer_size();
		fragmentShaderSize = 768;

		if (*sdk_version < 0x280000)
		{
			if (resc_manager.config.size == 24 || resc_manager.config.size == 28)
			{
				if (resc_manager.config.interlaceMode == CELL_RESC_INTERLACE_FILTER)
				{
					if (resc_manager.bufferMode == CELL_RESC_720x576 && resc_manager.config.palTemporalMode != CELL_RESC_PAL_50)
					{
						fragmentShaderSize = 640;
					}
					else
					{
						fragmentShaderSize = 512;
					}
				}
				else if ((resc_manager.config.supportModes & 2) != 0 &&
				          resc_manager.config.palTemporalMode >= CELL_RESC_PAL_60_INTERPOLATE &&
				          resc_manager.config.palTemporalMode <= CELL_RESC_PAL_60_INTERPOLATE_DROP_FLEXIBLE)
				{
					fragmentShaderSize = 96;
				}
				else
				{
					fragmentShaderSize = 16;
				}
			}
			else if (resc_manager.config.size == 20)
			{
				fragmentShaderSize = resc_manager.usedFragmentShader->size;
			}
			else
			{
				fragmentShaderSize = 0;
			}
		}
	}
	else
	{
		colorBuffersSize = resc_manager.bufferSize * get_color_buffers_count(resc_manager.bufferMode, resc_manager.config.palTemporalMode);
		fragmentShaderSize = resc_manager.usedFragmentShader->size;
	}

	if (colorBuffers)
	{
		*colorBuffers = colorBuffersSize;
	}

	if (vertexArray)
	{
		*vertexArray = 0x180;
	}

	if (fragmentShader)
	{
		*fragmentShader = fragmentShaderSize;
	}

	return CELL_OK;
}

s32 cellRescGetNumColorBuffers(u32 dstMode, u32 palTemporalMode, u32 reserved)
{
	cellResc.trace("cellRescGetNumColorBuffers(dstMode=0x%x, palTemporalMode=0x%x, reserved=%d)", dstMode, palTemporalMode, reserved);

	if (reserved != 0)
	{
		return CELL_RESC_ERROR_BAD_ARGUMENT;
	}

	return get_color_buffers_count(dstMode, palTemporalMode);
}

error_code cellRescGcmSurface2RescSrc(vm::cptr<CellGcmSurface> gcmSurface, vm::ptr<CellRescSrc> rescSrc)
{
	cellResc.notice("cellRescGcmSurface2RescSrc(gcmSurface=*0x%x, rescSrc=*0x%x)", gcmSurface, rescSrc);

	if (!gcmSurface || !rescSrc)
	{
		return CELL_RESC_ERROR_BAD_ARGUMENT;
	}

	auto& resc_manager = g_fxo->get<cell_resc_manager>();

	const u8 texture_format = get_texture_format(gcmSurface->colorFormat, gcmSurface->type);

	std::memset(rescSrc.get_ptr(), 0, sizeof(CellRescSrc));

	short factor_width = 1;
	short factor_height = 1;

	if (resc_manager.config.size == 24 || resc_manager.config.size == 28)
	{
		const u8 antialias = gcmSurface->antialias;
		if (antialias == CELL_GCM_SURFACE_DIAGONAL_CENTERED_2)
		{
			factor_width = 2;
		}
		else if (antialias == CELL_GCM_SURFACE_SQUARE_CENTERED_4 || antialias == CELL_GCM_SURFACE_SQUARE_ROTATED_4)
		{
			factor_width = 2;
			factor_height = 2;
		}
	}

	rescSrc->format = texture_format;
	rescSrc->pitch = gcmSurface->colorPitch[0];
	rescSrc->width = gcmSurface->width * factor_width;
	rescSrc->offset = gcmSurface->colorOffset[0];
	rescSrc->height = gcmSurface->height * factor_height;

	return CELL_OK;
}

error_code cellRescSetSrc(s32 idx, vm::cptr<CellRescSrc> src)
{
	cellResc.notice("cellRescSetSrc(idx=0x%x, src=*0x%x)", idx, src);

	auto& resc_manager = g_fxo->get<cell_resc_manager>();

	if (!resc_manager.is_initialized)
	{
		return CELL_RESC_ERROR_NOT_INITIALIZED;
	}

	if (idx >= SRC_BUFFER_NUM || !src || !src->width || src->width > 4096 || !src->height || src->height > 4096)
	{
		return CELL_RESC_ERROR_BAD_ARGUMENT;
	}

	const u32 texture_format = src->format & 0x9f;
	if (texture_format != CELL_GCM_TEXTURE_A8R8G8B8 && texture_format != CELL_GCM_TEXTURE_W16_Z16_Y16_X16_FLOAT)
	{
		return CELL_RESC_ERROR_BAD_ARGUMENT;
	}

	resc_manager.srcs[idx] = *src;

	return CELL_OK;
}

error_code cellRescSetConvertAndFlip(ppu_thread& ppu, vm::ptr<CellGcmContextData> con, s32 idx)
{
	cellResc.todo("cellRescSetConvertAndFlip(con=*0x%x, idx=0x%x)", con, idx);

	auto& resc_manager = g_fxo->get<cell_resc_manager>();

	if (!resc_manager.is_initialized)
	{
		return CELL_RESC_ERROR_NOT_INITIALIZED;
	}

	if (idx >= SRC_BUFFER_NUM)
	{
		return CELL_RESC_ERROR_BAD_ARGUMENT;
	}

	if (resc_manager.config.interlaceMode == CELL_RESC_INTERLACE_FILTER)
	{
		puVar8 = &TOC_BASE;
		FUN_00001ac0(state, idx);
	}

	local_d0 = (longlong)(int)con & 0xffffffff;
	FUN_00002978(state, local_d0);
	FUN_00002680(state, local_d0);
	FUN_00000d00(state, local_d0);
	FUN_0000251c(state, local_d0, idx);

	if (resc_manager.bufferMode == CELL_RESC_720x576 &&
		resc_manager.config.palTemporalMode >= CELL_RESC_PAL_60_INTERPOLATE &&
		resc_manager.config.palTemporalMode <= CELL_RESC_PAL_60_INTERPOLATE_DROP_FLEXIBLE)
	{
		piVar3 = *(int**)(puVar8 + -0x7fdc);
		iVar10 = sys_lwmutex_lock(*piVar3 + 0x170, 0);
		if (iVar10 != 0)
		{
			FUN_00007c5c(*(undefined4*)(puStack420 + -0x7fac), iVar10);
		}
		uVar2 = state[1].config.flipMode;
		iVar10 = sys_lwmutex_unlock(*piVar3 + 0x170);
		if (iVar10 != 0)
		{
			FUN_00007c5c(*(undefined4*)(puStack420 + -0x7f9c), iVar10);
		}
		iVar10 = sys_lwmutex_lock(*piVar3 + 0x188, 0);
		puVar8 = puStack420;
		if (iVar10 != 0)
		{
			FUN_00007c5c(*(undefined4*)(puStack420 + -0x7fac), iVar10);
		}
		lVar18 = 0;
		uVar19 = 0;
		uVar20 = (ulonglong) * *(uint**)(puVar8 + -0x7fa8);
		if (**(uint**)(puVar8 + -0x7fa8) != **(uint**)(puVar8 + -0x7fa4))
		{
			do
			{
				uVar11 = uVar20 & 0xffffffff;
				iVar14 = (int)(uVar20 + 10);
				iVar10 = iVar14 >> 0x1f;
				uVar12 = (longlong)(iVar14 / 9 + iVar10) - (longlong)iVar10;
				uVar20 = (uVar20 + 10) - ((uVar12 & 0x1fffffff) * 8 + uVar12);
				lVar18 = lVar18 - ((longlong)((ulonglong) * (byte*)((int)(uVar11 << 5) + *(int*)(puVar8 + -0x7fa0) + 0x1a) - 1 << 0x20) >> 0x3f);
				uVar19 = (undefined4)lVar18;
			} while (**(uint**)(puVar8 + -0x7fa4) != (uint)uVar20);
		}
		iVar10 = sys_lwmutex_unlock(*piVar3 + 0x188);
		puVar8 = puStack420;
		if (iVar10 != 0)
		{
			FUN_00007c5c(*(undefined4*)(puStack420 + -0x7f9c), iVar10);
		}
		local_c0 = uVar16 - 0x160;
		FUN_00001dfc(state, local_c0, uVar16 - 0x158, uVar16 - 0x148, uVar16 - 0x138, uVar16 - 0x128, uVar2,
			uVar19);
		puVar22 = con->current;
		if (con->end < puVar22 + 2)
		{
			iVar10 = (**(code**)con->callback)(local_d0, 2);
			if (iVar10 == 0)
			{
				puVar22 = con->current;
				goto LAB_00005388;
			}
		}
		else
		{
		LAB_00005388:
			*puVar22 = 0x46188;
			puVar22[1] = 0xfeed0000;
			con->current = puVar22 + 2;
		}
		if (**(int**)&state->field_0x1d4 - 0x415U < 0x5a)
		{
			/* WARNING: Could not recover jumptable at 0x000053e4. Too many branches */
			/* WARNING: Treating indirect jump as call */
			EVar9 = (*(code*)(*(int*)((**(int**)&state->field_0x1d4 + -0x415) * 4 +
									  *(int*)(puVar8 + -0x7f40)) +
							  *(int*)(puVar8 + -0x7f40)))();
			return EVar9;
		}
	}
	(***(code***)(**(int**)&state->field_0xd8 + 8))(*(int**)&state->field_0xd8, local_d0);
	(***(code ***)(*state->usedFragmentShader + 8))(state->usedFragmentShader,local_d0)
	uVar16 = ZEXT48(con->current);
	puVar22 = con->end;
	uVar1 = *(uint*)&state->field_0x140;
	if (ZEXT48(puVar22) < (uVar16 + 0x20 & 0xffffffff))
	{
		iVar10 = (**(code**)con->callback)(local_d0, 8);
		if (iVar10 == 0)
		{
			uVar16 = ZEXT48(con->current);
			puVar22 = con->end;
			goto LAB_00004f1c;
		}
	}
	else
	{
	LAB_00004f1c:
		uVar11 = (ulonglong)uVar1 - 1;
		uVar20 = (uVar11 << 0x20) >> 0x28;
		puVar6 = (undefined4*)uVar16;
		iVar10 = (int)(uVar20 * 0x200401 >> 0x20);
		uVar1 = (((uint)uVar11 >> 8) - iVar10 >> 1) + iVar10 >> 10;
		uVar16 = uVar16 + 0x20;
		puVar6[3] = 0;
		*puVar6 = 0x400c1714;
		puVar6[1] = 0;
		puVar6[2] = 0;
		con->current = (uint32_t*)uVar16;
		puVar6[5] = 8;
		puVar6[4] = 0x41808;
		lVar18 = (uVar11 & 0xff) + 1;
		puVar6[7] = (int)((uVar11 & 0xff) << 0x18);
		puVar6[6] = 0x41814;
		if (uVar1 != 0)
		{
			uVar21 = 0;
			do
			{
				if (ZEXT48(puVar22) < (uVar16 + 0x2000 & 0xffffffff))
				{
					iVar10 = (**(code**)con->callback)(local_d0, 0x800);
					if (iVar10 != 0)
						goto LAB_000050dc;
					uVar16 = ZEXT48(con->current);
					puVar22 = con->end;
				}
				puVar6 = (undefined4*)uVar16;
				uVar16 = uVar16 + 4;
				lVar24 = 0x7ff;
				con->current = (uint32_t*)uVar16;
				*puVar6 = 0x5ffc1814;
				lVar15 = lVar18;
				do
				{
					puVar7 = (uint*)uVar16;
					uVar5 = (uint)lVar15;
					uVar16 = uVar16 + 4;
					lVar15 = lVar15 + 0x100;
					*puVar7 = uVar5 | 0xff000000;
					lVar24 = lVar24 + -1;
				} while (lVar24 != 0);
				uVar21 = uVar21 + 1;
				con->current = (uint32_t*)uVar16;
				lVar18 = lVar18 + 0x7ff00;
			} while (uVar1 != uVar21);
		}
		uVar11 = uVar20 * 0x200401 >> 0x20;
		uVar11 = (((uVar20 - uVar11 << 0x20) >> 0x21) + uVar11 << 0x20) >> 0x2a;
		uVar20 = uVar20 - ((uVar11 & 0x1fffff) * 0x800 - uVar11);
		if ((int)uVar20 != 0)
		{
			if (ZEXT48(puVar22) < ((uVar20 & 0x3fffffff) * 4 + uVar16 + 4 & 0xffffffff))
			{
				iVar10 = (**(code**)con->callback)(local_d0, uVar20 + 1 & 0xffffffff);
				if (iVar10 != 0)
					goto LAB_000050dc;
				uVar16 = ZEXT48(con->current);
				puVar22 = con->end;
			}
			puVar7 = (uint*)uVar16;
			uVar16 = uVar16 + 4;
			uVar11 = uVar20 & 0xffffffff;
			*puVar7 = (uint)((uVar20 & 0xffffffff) << 0x12) | 0x40001814;
			con->current = (uint32_t*)uVar16;
			do
			{
				puVar7 = (uint*)uVar16;
				uVar1 = (uint)lVar18;
				uVar16 = uVar16 + 4;
				lVar18 = lVar18 + 0x100;
				*puVar7 = uVar1 | 0xff000000;
				uVar11 = uVar11 - 1;
			} while (uVar11 != 0);
			con->current = (uint32_t*)uVar16;
		}
		puVar23 = (uint32_t*)uVar16;
		if (ZEXT48(puVar22) < (uVar16 + 8 & 0xffffffff))
		{
			iVar10 = (**(code**)con->callback)(local_d0, 2);
			if (iVar10 != 0)
				goto LAB_000050dc;
			puVar23 = con->current;
		}
		*puVar23 = 0x41808;
		puVar23[1] = 0;
		con->current = puVar23 + 2;
	}
LAB_000050dc:
	if (resc_manager.bufferMode != CELL_RESC_720x576 ||
		resc_manager.config.palTemporalMode == CELL_RESC_PAL_50 ||
		resc_manager.config.palTemporalMode == CELL_RESC_PAL_60_FOR_HSYNC)
	{
		FUN_00008780(local_d0, state->field_0x113);
		*(uint*)&state->field_0x110 = *(uint*)&state->field_0x110 ^ 1;
		return CELL_OK;
	}

	if (resc_manager.config.palTemporalMode != CELL_RESC_PAL_60_DROP)
	{
		piVar3 = *(int**)(puVar8 + -0x7fa4);
		iVar10 = 2;
		piVar4 = *(int**)(puVar8 + -0x7fa8);
		do
		{
			iVar14 = (*piVar3 - *piVar4) + 9;
			uVar16 = (longlong)(iVar14 / 9 + (iVar14 >> 0x1f)) - (longlong)(iVar14 >> 0x1f);
			if (iVar14 - ((int)((uVar16 & 0xffffffff) << 3) + (int)uVar16) < 8)
			{
				iVar10 = cellGcmSetPrepareFlip(ppu, local_d0, state->field_0x113);
				EVar9 = CELL_RESC_ERROR_GCM_FLIP_QUE_FULL;
				if (iVar10 != -0x7fdeff01)
				{
					local_108 = *(undefined8*)&state->field_0x1e8;
					local_100 = local_158;
					local_f8 = local_148;
					local_f0 = (undefined) * (undefined4*)&state->field_0x110;
					local_ef = (undefined)iVar10;
					local_ee = 0;
					iVar10 = sys_lwmutex_lock(&state->field_0x188, 0);
					if (iVar10 == 0)
					{
						iVar10 = *piVar3 + 10;
						uVar16 = (longlong)(iVar10 / 9 + (iVar10 >> 0x1f)) - (longlong)(iVar10 >> 0x1f);
						iVar10 = iVar10 - ((int)((uVar16 & 0xffffffff) << 3) + (int)uVar16);
						if (iVar10 != *piVar4)
						{
							puVar13 = (undefined8*)(*piVar3 * 0x20 + *(int*)(puVar8 + -0x7fa0));
							if (puVar13 != &local_108)
							{
								*puVar13 = local_108;
								*(undefined*)((int)puVar13 + 0x1a) = local_ee;
								puVar13[1] = local_100;
								puVar13[2] = local_f8;
								*(undefined*)(puVar13 + 3) = local_f0;
								*(undefined*)((int)puVar13 + 0x19) = local_ef;
							}
							*piVar3 = iVar10;
						}
						sys_lwmutex_unlock(&state->field_0x188);
					}
					puVar22 = con->current;
					puVar23 = con->end;
					uVar16 = *(ulonglong*)&state->field_0x1e8;
					if (puVar23 < puVar22 + 4)
					{
						iVar10 = (**(code**)con->callback)(local_d0, 4);
						if (iVar10 == 0)
						{
							puVar22 = con->current;
							puVar23 = con->end;
							goto LAB_00005718;
						}
						puVar17 = con->current;
						puVar23 = con->end;
					}
					else
					{
					LAB_00005718:
						puVar17 = puVar22 + 4;
						*puVar22 = 0x41d6c;
						puVar22[1] = 0x110;
						con->current = puVar17;
						puVar22[3] = (uint)uVar16 & 0xff00ff00 | (uint)(uVar16 >> 0x10) & 0xff |
						             (uint)((uVar16 & 0xffffffff) << 0x10) & 0xff0000;
						puVar22[2] = 0x41d70;
					}
					uVar21 = *(uint*)&state->field_0x1dc ^ 1;
					uVar1 = (int)uVar21 >> 0x1f;
					if (puVar23 < puVar17 + 4)
					{
						iVar10 = (**(code**)con->callback)(local_d0, 4);
						if (iVar10 == 0)
						{
							puVar17 = con->current;
							goto LAB_0000579c;
						}
					}
					else
					{
					LAB_0000579c:
						puVar17[1] = (0x12 - ((int)(((uVar1 ^ uVar21) - uVar1) + -1) >> 0x1f)) * 0x10;
						*puVar17 = 0x40064;
						con->current = puVar17 + 4;
						puVar17[3] = 1;
						puVar17[2] = 0x4006c;
					}
					EVar9 = CELL_OK;
					*(uint*)&state->field_0x110 = *(int*)&state->field_0x110 + 1U & 3;
				}
				goto LAB_0000516c;
			}
			iVar10 = iVar10 + -1;
			syscall_sys_timer_usleep(20000);
		} while (iVar10 != 0);
		EVar9 = CELL_RESC_ERROR_GCM_FLIP_QUE_FULL;
	LAB_0000516c:
		uVar19 = *(undefined4*)&state->field_0x1cc;
		*(undefined4*)&state->field_0x1cc = *(undefined4*)&state->field_0x1c8;
		*(longlong*)&state->field_0x1e8 = *(longlong*)&state->field_0x1e8 + 1;
		*(undefined4*)&state->field_0x1c8 = uVar19;
		return EVar9;
	}
	FUN_00008780(local_d0, state->field_0x113);
	puVar22 = con->current;
	puVar23 = con->end;
	uVar16 = *(ulonglong*)&state->field_0x1e8;
	if (puVar23 < puVar22 + 4)
	{
		iVar10 = (**(code**)con->callback)(local_d0, 4);
		if (iVar10 == 0)
		{
			puVar22 = con->current;
			puVar23 = con->end;
			goto LAB_00005818;
		}
		puVar17 = con->current;
		puVar23 = con->end;
	}
	else
	{
	LAB_00005818:
		puVar17 = puVar22 + 4;
		*puVar22 = 0x41d6c;
		puVar22[1] = 0x110;
		con->current = puVar17;
		puVar22[3] = (uint)uVar16 & 0xff00ff00 | (uint)(uVar16 >> 0x10) & 0xff |
		             (uint)((uVar16 & 0xffffffff) << 0x10) & 0xff0000;
		puVar22[2] = 0x41d70;
	}
	uVar21 = *(uint*)&state->field_0x1dc ^ 1;
	uVar1 = (int)uVar21 >> 0x1f;
	if (puVar23 < puVar17 + 4)
	{
		iVar10 = (**(code**)con->callback)(local_d0, 4);
		if (iVar10 != 0)
			goto LAB_000058d8;
		puVar17 = con->current;
	}
	puVar17[1] = (0x12 - ((int)(((uVar1 ^ uVar21) - uVar1) + -1) >> 0x1f)) * 0x10;
	*puVar17 = 0x40064;
	con->current = puVar17 + 4;
	puVar17[3] = 1;
	puVar17[2] = 0x4006c;
LAB_000058d8:
	iVar10 = *(int*)&state->field_0x110;
	*(longlong*)&state->field_0x1e8 = *(longlong*)&state->field_0x1e8 + 1;
	*(int*)&state->field_0x110 = 3 - (iVar10 + *(int*)&state->field_0x1d0);
	*(int*)&state->field_0x1d0 = iVar10;

	return CELL_OK;
}

void cellRescSetWaitFlip(vm::ptr<CellGcmContextData> con)
{
	cellResc.todo("cellRescSetWaitFlip(con=*0x%x)", con);

	auto& resc_manager = g_fxo->get<cell_resc_manager>();

	vm::bptr<u32> current = con->current;

	if (current + 4 > con->end)
	{
		if (con->callback(con, 4))
		{
			return;
		}
		current = con->current;
	}

	if (resc_manager.bufferMode == CELL_RESC_720x576 &&
		resc_manager.config.palTemporalMode != CELL_RESC_PAL_50 &&
		resc_manager.config.palTemporalMode != CELL_RESC_PAL_60_FOR_HSYNC)
	{
		s64 val = 0x13;
		if (*(int*)&state->field_0x1dc != 1)
		{
			val = 0x12;
		}

		current[0] = 0x40064;
		current[1] = static_cast<u32>(val << 4);
		current[2] = 0x40068;
		current[3] = 0;
	}
	else
	{
		current[0] = 0x40064;
		current[1] = 0x10;
		current[2] = 0x40068;
		current[3] = 0;
	}

	con->current = current + 4;
}

error_code cellRescSetBufferAddress(vm::ptr<void> colorBuffers, vm::ptr<void> vertexArray, vm::ptr<void> fragmentShader)
{
	cellResc.todo("cellRescSetBufferAddress(colorBuffers=*0x%x, vertexArray=*0x%x, fragmentShader=*0x%x)", colorBuffers, vertexArray, fragmentShader);

	auto& resc_manager = g_fxo->get<cell_resc_manager>();

	if (!resc_manager.is_initialized)
	{
		return CELL_RESC_ERROR_NOT_INITIALIZED;
	}

	if (!colorBuffers || !vertexArray || !fragmentShader)
	{
		return CELL_RESC_ERROR_BAD_ARGUMENT;
	}

	if (!colorBuffers.aligned(128) || !vertexArray.aligned(4) || !fragmentShader.aligned(64)) // clrlwi with 25, 30, 26
	{
		return CELL_RESC_ERROR_BAD_ALIGNMENT;
	}

	resc_manager.colorBuffers = colorBuffers;
	resc_manager.vertexArray = vertexArray;
	resc_manager.fragmentShader = fragmentShader;

	int iStack116;

	vm::var<u32> offset;
	cellGcmAddressToOffset(colorBuffers.addr(), offset);

	u32 color_buffers = get_color_buffers_count(resc_manager.bufferMode, resc_manager.config.palTemporalMode);

	for (u32 i = 0; i < color_buffers; i++)
	{
		resc_manager.buffersOffsets[i] = *offset + i * resc_manager.bufferSize;
	}

	for (u32 i = 0; true; i++)
	{
		color_buffers = get_color_buffers_count(resc_manager.bufferMode, resc_manager.config.palTemporalMode);

		if (i >= color_buffers)
		{
			if (resc_manager.config.interlaceMode != CELL_RESC_INTERLACE_FILTER)
			{
				fill_vertex_array();
			}

			if (resc_manager.usedFragmentShader->unk_1 != 0)
			{
				if (resc_manager.usedFragmentShader->size != 0)
				{
					std::memcpy(resc_manager.fragmentShader.get_ptr(), resc_manager.usedFragmentShader->data.get_ptr(), resc_manager.usedFragmentShader->size);

					vm::var<u32> fs_offset;
					cellGcmAddressToOffset(resc_manager.fragmentShader.addr(), fs_offset);
					resc_manager.usedFragmentShader->offset = *fs_offset;
				}
			}

			undefined uVar6;
			undefined4 uVar5;
			undefined4* puVar7;
			undefined4* puVar8;
			undefined* puVar9;

			u64 uVar2 = ZEXT48(&TOC_BASE);
			u32 uVar1 = resc_manager.usedFragmentShader->unk_1;
			puVar9 = &resc_manager.field_0x155;
			puVar7 = (undefined4*)(PTR_PTR_s_samplerSrc0_0000c328 + 0x2c);
			puVar8 = (undefined4*)PTR_PTR_s_samplerSrc0_0000c328;

			s32 iVar3;
			do
			{
				while (true)
				{
					s32 iVar4 = FUN_00008868(uVar1, *puVar8);
					iVar3 = (int)uVar2;
					if (iVar4 != 0)
						break;
					puVar8++;
					*puVar9 = 0xff;
					puVar9++;
					if (puVar8 == puVar7)
						goto LAB_00002454;
				}
				puVar8++;
				uVar6 = FUN_000089ec(uVar1, iVar4);
				iVar3 = (int)uVar2;
				*puVar9 = uVar6;
				puVar9++;
			} while (puVar8 != puVar7);
		LAB_00002454:
			uVar5 = FUN_00008868(uVar1, *(undefined4*)(iVar3 + -0x7f94));
			*(undefined4*)&state->field_0x1d4 = uVar5;
			return CELL_OK;
		}

		const error_code error = cellGcmSetDisplayBuffer(i, resc_manager.buffersOffsets[i], resc_manager.pitch, resc_manager.width, resc_manager.height);
		if (error != CELL_OK)
		{
			// Something is called here before the return. Not sure if there's a NOP here or something wasn't decompiled correctly.
			return error;
		}
	}

	return CELL_OK;
}

void cellRescSetFlipHandler(vm::ptr<CellRescHandler> handler)
{
	cellResc.todo("cellRescSetFlipHandler(handler=*0x%x)", handler);

	auto& resc_manager = g_fxo->get<cell_resc_manager>();

	if (!resc_manager.is_initialized || resc_manager.bufferMode == 0)
	{
		*(CellRescHandler*)(PTR_DAT_0000c2e4 + 4) = handler;
		return;
	}

	if (resc_manager.bufferMode == CELL_RESC_720x576 &&
		resc_manager.config.palTemporalMode != CELL_RESC_PAL_50 &&
		resc_manager.config.palTemporalMode != CELL_RESC_PAL_60_FOR_HSYNC)
	{
		*(CellRescHandler*)(PTR_DAT_0000c2e4 + 4) = handler;
		return;
	}

	int iStack68;
	cellGcmSetFlipHandler(handler);
	*(undefined4*)(*(int*)(iStack68 + -0x7fdc) + 4) = 0;
}

void cellRescResetFlipStatus()
{
	cellResc.todo("cellRescResetFlipStatus()");

	auto& resc_manager = g_fxo->get<cell_resc_manager>();

	if (resc_manager.bufferMode == CELL_RESC_720x576 &&
		resc_manager.config.palTemporalMode != CELL_RESC_PAL_50 &&
		resc_manager.config.palTemporalMode != CELL_RESC_PAL_60_FOR_HSYNC)
	{
		resc_manager.flipStatus = 0;
		return;
	}

	cellGcmResetFlipStatus();
}

u32 cellRescGetFlipStatus()
{
	cellResc.trace("cellRescGetFlipStatus()");

	auto& resc_manager = g_fxo->get<cell_resc_manager>();

	if (resc_manager.bufferMode == CELL_RESC_720x576 &&
		resc_manager.config.palTemporalMode != CELL_RESC_PAL_50 &&
		resc_manager.config.palTemporalMode != CELL_RESC_PAL_60_FOR_HSYNC)
	{
		return resc_manager.flipStatus ^ 1;
	}

	return cellGcmGetFlipStatus();
}

u64 cellRescGetLastFlipTime()
{
	cellResc.trace("cellRescGetLastFlipTime()");

	auto& resc_manager = g_fxo->get<cell_resc_manager>();

	if (resc_manager.bufferMode == CELL_RESC_720x576 &&
		resc_manager.config.palTemporalMode != CELL_RESC_PAL_50 &&
		resc_manager.config.palTemporalMode != CELL_RESC_PAL_60_FOR_HSYNC)
	{
		return resc_manager.lastFlipTime;
	}

	return cellGcmGetLastFlipTime();
}

s32 cellRescGetRegisterCount()
{
	cellResc.todo("cellRescGetRegisterCount()");
	return 0;
}

void cellRescSetRegisterCount(s32 regCount)
{
	cellResc.todo("cellRescSetRegisterCount(regCount=0x%x)", regCount);
}

void cellRescSetVBlankHandler(vm::ptr<CellRescHandler> handler)
{
	cellResc.todo("cellRescSetVBlankHandler(handler=*0x%x)", handler);

	auto& resc_manager = g_fxo->get<cell_resc_manager>();

	int iStack68;

	if (!resc_manager.is_initialized || resc_manager.bufferMode == 0)
	{
		*(CellRescHandler*)(PTR_DAT_0000c2e4 + 8) = handler;
	}
	else
	{
		if (resc_manager.bufferMode != CELL_RESC_720x576 || resc_manager.config.palTemporalMode == CELL_RESC_PAL_50)
		{
			cellGcmSetVBlankHandler(handler);
			*(undefined4*)(*(int*)(iStack68 + -0x7fdc) + 8) = 0;
			return;
		}
		if (resc_manager.config.palTemporalMode == CELL_RESC_PAL_60_FOR_HSYNC)
		{
			cellGcmSetSecondVHandler();
			*(undefined4*)(*(int*)(iStack68 + -0x7fdc) + 8) = 0;
		}
		else
		{
			*(CellRescHandler*)(PTR_DAT_0000c2e4 + 8) = handler;
		}
	}
}

u32 FUN_000080f4(float value)
{
	u32 uVar5 = 0;
	const u8 bStack00000030 = (u8)((u32)value >> 0x18);
	const u8 bStack00000031 = (u8)((u32)value >> 0x10);
	const u8 bStack00000033 = SUB41(value, 0);
	const u8 bStack00000032 = (u8)((u32)value >> 8);
	u64 uVar2 = (u64)bStack00000030 << 0x18 | (u64)bStack00000031 << 0x10;
	const u64 uVar3 = (u64)bStack00000032 << 8 | uVar2 | bStack00000033;
	const u32 uVar4 = (u32)uVar3;
	u32 uVar2 = (uVar2 >> 0x17 & 0xff) - 0x70;
	if (uVar4 != 0)
	{
		if (-1 < (int)uVar2)
		{
			if (0x1f < (int)uVar2)
			{
				uVar2 = 0x1f;
			}
			uVar5 = (u32)((uVar2 & 0xffffffff) << 10) & 0x7c00 | uVar4 >> 0x10 & 0x8000 | (u32)(uVar3 >> 0xd) & 0x3ff;
		}
	}
	return uVar5;
}

error_code cellRescCreateInterlaceTable(vm::ptr<void> ea_addr, f32 srcH, CellRescTableElement depth, s32 length)
{
	cellResc.todo("cellRescCreateInterlaceTable(ea_addr=*0x%x, srcH=%f, depth=0x%x, length=%d)", ea_addr, srcH, +depth, length);

	auto& resc_manager = g_fxo->get<cell_resc_manager>();

	if (!resc_manager.is_initialized)
	{
		return CELL_RESC_ERROR_NOT_INITIALIZED;
	}

	if (!ea_addr || srcH <= 0.0f || (int)depth < CELL_RESC_ELEMENT_FLOAT || length > 1)
	{
		return CELL_RESC_ERROR_BAD_ARGUMENT;
	}

	if (resc_manager.height == 0)
	{
		return CELL_RESC_ERROR_BAD_COMBINATION;
	}

	const float factor = (resc_manager.config.ratioMode == CELL_RESC_LETTERBOX)
		 ? (resc_manager.config.size == 28 ? 0.7646154f : 0.75f)
		 : 1.0f;

	const float dst_height = factor * resc_manager.height * resc_manager.vertical;

	ulonglong uVar5;
	undefined2 uVar6;
	byte in_cr0;
	byte in_cr1;
	byte in_cr2;
	byte in_cr3;
	byte unaff_cr4;
	byte in_cr5;
	byte in_cr6;
	byte in_cr7;
	double dVar16 = (double)((float)(0.5f / (double)(float)(srcH / dst_height)) * 6.2831855f);
	double dVar9 = (double)FUN_0000a278(1.8849556f, ea_addr, ea_addr_long);
	double dVar19 = (double)*(float*)((int)uVar5 + -0x7ee4);
	double dVar20 = (double)(float)(dVar9 * 0.5f + dVar19);
	dVar9 = (double)FUN_0000a278((double)*(float*)((int)uVar5 + -0x7ee0));
	double dVar23 = (double)*(float*)((int)uVar5 + -0x7edc);
	double dVar22 = (double)(float)(dVar9 * dVar23);
	dVar9 = (double)FUN_0000a278((double)*(float*)((int)uVar5 + -0x7ed8));
	dVar19 = (double)(float)(dVar9 * (double)*(float*)((int)uVar5 + -0x7f20) + dVar19);
	dVar9 = (double)FUN_0000a278((double)*(float*)((int)uVar5 + -0x7ed4));
	int iVar4 = (int)uVar5;
	dVar22 = (double)((float)(dVar20 + dVar22) * *(float*)(iVar4 + -0x7ed0) +
					  *(float*)(iVar4 + -0x7ecc));
	dVar9 = (double)((float)(dVar9 * dVar23 + dVar19) * *(float*)(iVar4 + -0x7ed0) +
					 *(float*)(iVar4 + -0x7ecc));

	if ((int)depth > CELL_RESC_ELEMENT_HALF)
	{
		double dVar15 = *(double*)(iVar4 + -0x7ec0);
		double dVar14 = (double)*(float*)(iVar4 + -0x7f10);
		u32 uVar8 = 0;
		dVar20 = (double)*(float*)(iVar4 + -0x7f18);
		dVar19 = (double)*(float*)(iVar4 + -0x7f0c);
		double dVar12 = (double)*(float*)(iVar4 + -0x7efc);
		double dVar11 = (double)*(float*)(iVar4 + -0x7ef8);
		dVar23 = (double)*(float*)(iVar4 + -0x7ef4);
		double dVar13 = (double)(longlong)(int)depth;
		u16* table = reinterpret_cast<u16*>(ea_addr.get_ptr());
		u32 tableIndex = 0;

		do
		{
			while (true)
			{
				iVar4 = (int)uVar5;
				float fVar2 = (float)((double)(longlong)(int)uVar8 / dVar13);
				float fVar3 = (float)(dVar16 * (double)(*(float*)(iVar4 + -0x7ec8) - fVar2));
				double dVar24 = (double)fVar3;
				double dVar17 = (double)(float)(dVar16 * (double)(*(float*)(iVar4 + -0x7ec4) - fVar2));
				double dVar21 = (double)(float)(dVar16 * (double)(*(float*)(iVar4 + -0x7f1c) - fVar2));
				double dVar18 = (double)(float)(dVar16 * (double)(*(float*)(iVar4 + -0x7f20) - fVar2));
				double dVar10 = dVar22;
				if (dVar15 < ABS(dVar24))
				{
					if ((int)fVar3 < 0)
					{
						dVar10 = (double)FUN_00009aa0(-dVar24);
						dVar10 = -dVar10;
					}
					else
					{
						uVar1 = (uint)(dVar24 * dVar14);
						dVar10 = (double)((float)(dVar24 * dVar14) - (float)(longlong)(int)uVar1);
						if ((uVar1 & 1) != 0)
						{
							dVar10 = (double)(float)(dVar20 - dVar10);
						}
						if ((uVar1 + 1 & 2) == 0)
						{
							dVar10 = dVar10 * (double)(float)(dVar10 * (double)(float)(dVar10 * (double)(float)(dVar10 * (double)(float)(dVar10 * (double)(float)(dVar10 * (double)(float)(dVar10 * dVar12) + dVar11)) - dVar23)) +
															  (double)*(float*)(iVar4 + -0x7ef0));
						}
						else
						{
							dVar10 = dVar10 * (double)(float)(dVar10 * (double)(float)(dVar10 * (double)(float)(dVar10 * (double)(float)(dVar10 * (double)(float)(dVar10 * dVar19) +
																																		 (double)*(float*)(iVar4 + -0x7f08))) -
																					   (double)*(float*)(iVar4 + -0x7f04))) +
							         (double)*(float*)(iVar4 + -0x7f00);
						}
						dVar10 = (double)(float)dVar10;
						if (3 < (uVar1 & 7))
						{
							dVar10 = -dVar10;
						}
					}
					dVar10 = (double)(float)(dVar22 * (double)(float)(dVar10 / dVar24));
				}
				dVar24 = dVar9;
				if (dVar15 < ABS(dVar21))
				{
					if ((int)(float)dVar21 < 0)
					{
						dVar24 = (double)FUN_00009aa0(-dVar21);
						dVar24 = -dVar24;
					}
					else
					{
						uint uVar1 = (uint)(dVar21 * dVar14);
						dVar24 = (double)((float)(dVar21 * dVar14) - (float)(longlong)(int)uVar1);
						if ((uVar1 & 1) != 0)
						{
							dVar24 = (double)(float)(dVar20 - dVar24);
						}
						iVar4 = (int)uVar5;
						if ((uVar1 + 1 & 2) == 0)
						{
							dVar24 = dVar24 * (double)(float)(dVar24 * (double)(float)(dVar24 * (double)(float)(dVar24 * (double)(float)(dVar24 * (double)(float)(dVar24 * (double)(float)(dVar24 * dVar12) + dVar11)) - dVar23)) +
															  (double)*(float*)(iVar4 + -0x7ef0));
						}
						else
						{
							dVar24 = dVar24 * (double)(float)(dVar24 * (double)(float)(dVar24 * (double)(float)(dVar24 * (double)(float)(dVar24 * (double)(float)(dVar24 * dVar19) +
																																		 (double)*(float*)(iVar4 + -0x7f08))) -
																					   (double)*(float*)(iVar4 + -0x7f04))) +
							         (double)*(float*)(iVar4 + -0x7f00);
						}
						dVar24 = (double)(float)dVar24;
						if (3 < (uVar1 & 7))
						{
							dVar24 = -dVar24;
						}
					}
					dVar24 = (double)(float)(dVar9 * (double)(float)(dVar24 / dVar21));
				}
				iVar4 = (int)uVar5;
				fVar2 = *(float*)(iVar4 + -0x7eb8);
				if (dVar15 < ABS(dVar18))
				{
					if ((int)(float)dVar18 < 0)
					{
						dVar21 = (double)FUN_00009aa0(-dVar18);
						dVar21 = -dVar21;
					}
					else
					{
						uVar1 = (uint)(dVar18 * dVar14);
						dVar21 = (double)((float)(dVar18 * dVar14) - (float)(longlong)(int)uVar1);
						if ((uVar1 & 1) != 0)
						{
							dVar21 = (double)(float)(dVar20 - dVar21);
						}
						if ((uVar1 + 1 & 2) == 0)
						{
							dVar21 = dVar21 * (double)(float)(dVar21 * (double)(float)(dVar21 * (double)(float)(dVar21 * (double)(float)(dVar21 * (double)(float)(dVar21 * (double)(float)(dVar21 * dVar12) + dVar11)) - dVar23)) +
															  (double)*(float*)(iVar4 + -0x7ef0));
						}
						else
						{
							dVar21 = dVar21 * (double)(float)(dVar21 * (double)(float)(dVar21 * (double)(float)(dVar21 * (double)(float)(dVar21 * (double)(float)(dVar21 * dVar19) +
																																		 (double)*(float*)(iVar4 + -0x7f08))) -
																					   (double)*(float*)(iVar4 + -0x7f04))) +
							         (double)*(float*)(iVar4 + -0x7f00);
						}
						dVar21 = (double)(float)dVar21;
						if (3 < (uVar1 & 7))
						{
							dVar21 = -dVar21;
						}
					}
					fVar2 = *(float*)((int)uVar5 + -0x7eb8) * (float)(dVar21 / dVar18);
				}
				dVar18 = (double)fVar2;
				iVar4 = (int)uVar5;
				fVar2 = *(float*)(iVar4 + -0x7eb4);
				if (dVar15 < ABS(dVar17))
				{
					if ((int)(float)dVar17 < 0)
					{
						dVar21 = (double)FUN_00009aa0(-dVar17);
						dVar21 = -dVar21;
					}
					else
					{
						uVar1 = (uint)(dVar17 * dVar14);
						dVar21 = (double)((float)(dVar17 * dVar14) - (float)(longlong)(int)uVar1);
						if ((uVar1 & 1) != 0)
						{
							dVar21 = (double)(float)(dVar20 - dVar21);
						}
						if ((uVar1 + 1 & 2) == 0)
						{
							dVar21 = dVar21 * (double)(float)(dVar21 * (double)(float)(dVar21 * (double)(float)(dVar21 * (double)(float)(dVar21 * (double)(float)(dVar21 * (double)(float)(dVar21 * dVar12) + dVar11)) - dVar23)) +
															  (double)*(float*)(iVar4 + -0x7ef0));
						}
						else
						{
							dVar21 = dVar21 * (double)(float)(dVar21 * (double)(float)(dVar21 * (double)(float)(dVar21 * (double)(float)(dVar21 * (double)(float)(dVar21 * dVar19) +
																																		 (double)*(float*)(iVar4 + -0x7f08))) -
																					   (double)*(float*)(iVar4 + -0x7f04))) +
							         (double)*(float*)(iVar4 + -0x7f00);
						}
						dVar21 = (double)(float)dVar21;
						if (3 < (uVar1 & 7))
						{
							dVar21 = -dVar21;
						}
					}
					fVar2 = *(float*)((int)uVar5 + -0x7eb4) * (float)(dVar21 / dVar17);
				}
				dVar17 = (double)fVar2;
				dVar21 = (double)(float)(dVar17 + (double)(float)(dVar18 + (double)(float)(dVar24 + dVar10)));

				if (length != 0)
					break;

				uVar8++;

				table[tableIndex++] = FUN_000080f4((double)(float)(dVar10 / dVar21));
				table[tableIndex++] = FUN_000080f4((double)(float)(dVar24 / dVar21));
				table[tableIndex++] = FUN_000080f4((double)(float)(dVar18 / dVar21));
				table[tableIndex++] = FUN_000080f4((double)(float)(dVar17 / dVar21));

				if (depth == uVar8)
				{
					return 0;
				}
			}

			uVar8++;

			*(float*)((int)ea_addr + 4) = (float)(dVar24 / dVar21);
			*(float*)((int)ea_addr + 8) = (float)(dVar18 / dVar21);
			*(float*)ea_addr = (float)(dVar10 / dVar21);
			*(float*)((int)ea_addr + 0xc) = (float)(dVar17 / dVar21);
			ea_addr = (void*)((int)ea_addr + 0x10);
		} while (depth != uVar8);
	}

	if (success)
	{
		resc_manager.depth = depth;
		resc_manager.table = ea_addr.addr();
		resc_manager.tableLength = length;
	}

	return CELL_OK;
}


DECLARE(ppu_module_manager::cellResc)("cellResc", []()
{
	REG_FUNC(cellResc, cellRescSetConvertAndFlip);
	REG_FUNC(cellResc, cellRescSetWaitFlip);
	REG_FUNC(cellResc, cellRescSetFlipHandler);
	REG_FUNC(cellResc, cellRescGcmSurface2RescSrc);
	REG_FUNC(cellResc, cellRescGetNumColorBuffers);
	REG_FUNC(cellResc, cellRescSetDsts);
	REG_FUNC(cellResc, cellRescResetFlipStatus);
	REG_FUNC(cellResc, cellRescSetPalInterpolateDropFlexRatio);
	REG_FUNC(cellResc, cellRescGetRegisterCount);
	REG_FUNC(cellResc, cellRescAdjustAspectRatio);
	REG_FUNC(cellResc, cellRescSetDisplayMode);
	REG_FUNC(cellResc, cellRescExit);
	REG_FUNC(cellResc, cellRescInit);
	REG_FUNC(cellResc, cellRescGetBufferSize);
	REG_FUNC(cellResc, cellRescGetLastFlipTime);
	REG_FUNC(cellResc, cellRescSetSrc);
	REG_FUNC(cellResc, cellRescSetRegisterCount);
	REG_FUNC(cellResc, cellRescSetBufferAddress);
	REG_FUNC(cellResc, cellRescGetFlipStatus);
	REG_FUNC(cellResc, cellRescVideoOutResolutionId2RescBufferMode);
	REG_FUNC(cellResc, cellRescSetVBlankHandler);
	REG_FUNC(cellResc, cellRescCreateInterlaceTable);
});
