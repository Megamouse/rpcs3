#include "stdafx.h"
#include "gcm_methods.h"
#include "Emu/RSX/gcm_enums.h"

namespace gcm
{
	void check_method_param_count(u32 method, u32 param_count)
	{
		switch (method)
		{
		case DRIVER_INTERRUPT:
		case DRIVER_QUEUE:
		case DRIVER_FLIP:
		case NV4097_SET_POLY_SMOOTH_ENABLE:
		case NV4097_SET_POLYGON_STIPPLE:
		case NV4097_SET_FRONT_POLYGON_MODE:
		case NV4097_SET_BACK_POLYGON_MODE:
		case NV4097_SET_SHADE_MODE:
		case NV4097_SET_TWO_SIDE_LIGHT_EN:
		case NV4097_SET_STENCIL_MASK:
		case NV4097_SET_BACK_STENCIL_MASK:
		case NV4097_SET_ALPHA_TEST_ENABLE:
		case NV4097_SET_BLEND_ENABLE:
		case NV4097_SET_LOGIC_OP_ENABLE:
		case NV4097_SET_CULL_FACE_ENABLE:
		case NV4097_SET_DEPTH_BOUNDS_TEST_ENABLE:
		case NV4097_SET_DEPTH_TEST_ENABLE:
		case NV4097_SET_DITHER_ENABLE:
		case NV4097_SET_STENCIL_TEST_ENABLE:
		case NV4097_SET_TWO_SIDED_STENCIL_TEST_ENABLE:
		case NV4097_SET_POLY_OFFSET_FILL_ENABLE:
		case NV4097_SET_RESTART_INDEX_ENABLE:
		case NV4097_SET_POINT_PARAMS_ENABLE:
		case NV4097_SET_CONTEXT_DMA_ZETA:
		case NV4097_SET_FOG_MODE:
		case NV4097_SET_CONTROL0:
		case NV4097_SET_REDUCE_DST_COLOR:
		case NV0039_BUFFER_NOTIFY:
		case NV4097_SET_POLY_OFFSET_LINE_ENABLE:
		case NV406E_SET_CONTEXT_DMA_SEMAPHORE:
		case NV4097_SET_CONTEXT_DMA_REPORT:
		case NV4097_ZCULL_SYNC:
		case NV4097_SET_CONTEXT_DMA_SEMAPHORE:
		case NV4097_SET_CONTEXT_DMA_NOTIFIES:
		case NV4097_NOTIFY:
		case NV4097_SET_NO_PARANOID_TEXTURE_FETCHES:
		case NV4097_SET_BLEND_COLOR:
		case NV4097_SET_BLEND_COLOR2:
		case NV4097_CLEAR_SURFACE:
		case NV4097_SET_COLOR_CLEAR_VALUE:
		case NV4097_SET_ZSTENCIL_CLEAR_VALUE:
		case NV4097_SET_COLOR_MASK:
		case NV4097_SET_COLOR_MASK_MRT:
		case NV4097_SET_CULL_FACE:
		case NV4097_SET_DEPTH_FUNC:
		case NV4097_SET_DEPTH_MASK:
		case NV4097_SET_FRONT_FACE:
		case NV4097_SET_LINE_WIDTH:
		case NV4097_SET_LINE_SMOOTH_ENABLE:
		case NV4097_SET_LINE_STIPPLE:
		case NV4097_SET_LOGIC_OP:
		case NV4097_SET_POINT_SIZE:
		case NV4097_INVALIDATE_L2:
		case NV4097_SET_CYLINDRICAL_WRAP:
		case NV4097_SET_CYLINDRICAL_WRAP1:
		case NV4097_SET_BEGIN_END:
		case NV4097_SET_RESTART_INDEX:
		case NV4097_SET_FREQUENCY_DIVIDER_OPERATION:
		case NV4097_SET_TRANSFORM_BRANCH_BITS:
		case NV4097_SET_VERTEX_ATTRIB_INPUT_MASK:
		case NV4097_SET_VERTEX_ATTRIB_OUTPUT_MASK:
		case NV4097_SET_TRANSFORM_PROGRAM_START:
		case NV4097_SET_SHADER_CONTROL:
		case NV4097_SET_SHADER_PACKER:
		case NV4097_SET_ZPASS_PIXEL_COUNT_ENABLE:
		case NV4097_CLEAR_REPORT_VALUE:
		case NV4097_SET_ZCULL_STATS_ENABLE:
		case NV4097_SET_SEMAPHORE_OFFSET:
		case NV4097_TEXTURE_READ_SEMAPHORE_RELEASE:
		case NV4097_BACK_END_WRITE_SEMAPHORE_RELEASE:
		case NV406E_SEMAPHORE_OFFSET:
		case NV406E_SEMAPHORE_ACQUIRE:
		case NV406E_SEMAPHORE_RELEASE:
		case NV4097_SET_CONTEXT_DMA_COLOR_A:
		case NV4097_SET_CONTEXT_DMA_COLOR_B:
		case NV4097_SET_CONTEXT_DMA_COLOR_D:
		case NV4097_SET_SURFACE_PITCH_Z:
		case NV4097_SET_SURFACE_COLOR_TARGET:
		case NV406E_SET_REFERENCE:
		case NV3062_SET_CONTEXT_DMA_IMAGE_DESTIN:
		case NV3062_SET_OFFSET_DESTIN:
		case NV3089_SET_CONTEXT_DMA_IMAGE:
		case NV3089_SET_CONTEXT_SURFACE:
		case NV0039_OFFSET_OUT:
		case NV309E_SET_CONTEXT_DMA_IMAGE:
		case NV4097_SET_ZCULL_EN:                    // (depth, stencil) { depth | (stencil << 1) }
		case NV4097_CLEAR_ZCULL_SURFACE:             // (depth, stencil) { depth | (stencil << 1) }
		case NV4097_SET_BLEND_EQUATION:              // (color, alpha) { color | (alpha << 1) }
		case NV4097_SET_LINE_STIPPLE_PATTERN:        // (pattern, factor) { factor | (pattern << 16) }
		case NV4097_SET_WINDOW_OFFSET:               // (x, y) { x | (y << 16) }
		case NV4097_PM_TRIGGER:                      // 1
		case NV4097_WAIT_FOR_IDLE:                   // 0
		case NV4097_INVALIDATE_ZCULL:                // 0
		case NV4097_INVALIDATE_VERTEX_CACHE_FILE:    // 0
		case NV4097_NO_OPERATION:                    // 0
		case NV4097_SET_BLEND_ENABLE_MRT:            // (mrt1, mrt2, mrt3) { (mrt1 << 1) | (mrt2 << 2) | (mrt3 << 3) }
		case NV4097_SET_POINT_SPRITE_CONTROL:        // (enable, rmode, texcoordMask) { enable | (rmode << 1) | texcoordMask }
		case NV4097_DRAW_ARRAYS:                     // (first, count) { first | (count << 24) }
		case NV4097_SET_SHADER_WINDOW:               // (height, origin, pixelCenters) { height | (origin << 12) | (pixelCenters << 16) }
		case NV4097_SET_ZMIN_MAX_CONTROL:            // (cullNearFarEnable, zclampEnable, cullIgnoreW) { cullNearFarEnable | (zclampEnable << 4) | (cullIgnoreW << 8) }
		case NV4097_SET_USER_CLIP_PLANE_CONTROL:     // (plane0, plane1, plane2, plane3, plane4, plane5) { plane0 | (plane1 << 4) | (plane2 << 8) | (plane3 << 12) | (plane4 << 16) | (plane5 << 20) }
		case NV4097_SET_RENDER_ENABLE:               // (mode, offset) { offset | (mode << 24) }
		case NV4097_GET_REPORT:                      // (type, offset) { offset | (type << 24) }
		case NV4097_SET_ZCULL_CONTROL0:              // (zCullDir, zCullFormat) { zCullDir | (zCullFormat << 4) }
		case NV4097_SET_ZCULL_CONTROL1:              // (moveForwardLimit, pushBackLimit) { (moveForwardLimit << 16) | pushBackLimit }
		case NV4097_SET_SCULL_CONTROL:               // (sFunc, sRef, sMask) { sFunc | (sRef << 16) | (sMask << 24) }
		case NV4097_SET_ANTI_ALIASING_CONTROL:       // (enable, alphaToCoverage, alphaToOne, sampleMask) { enable | (alphaToCoverage << 4) | (alphaToOne << 8) | (sampleMask << 16) }
		case NV4097_SET_TRANSFORM_TIMEOUT:           // (count, registerCount) { count | (registerCount << 16) }
		case NV4097_DRAW_INDEX_ARRAY:                // (first, count) { first | (count << 24) }
		case NV4097_SET_SHADER_PROGRAM:              // (location, offset) { location | offset }
		case NV4097_SET_TEXTURE_FILTER:              // (index, bias, min, mag, conv, as, rs, gs, bs) { bias | (conv << 13) | (min << 16) | (mag << 24) | (as << 28) | (rs << 29) | (gs << 30) | (bs << 31) }
		case NV4097_SET_TEXTURE_ADDRESS:             // (index, wraps, wrapt, wrapr, unsignedRemap, zfunc, gamma, anisoBias, signedRemap) { wraps| (anisoBias << 4) | (wrapt << 8) | (unsignedRemap << 12) | (wrapr << 16) | (gamma << 20) | (signedRemap << 24) | (zfunc << 28) }
		case NV4097_SET_TEXTURE_CONTROL0:            // (index, enable, minlod, maxlod, maxaniso, alphakill) { ((alphakill << 2) | maxaniso << 4) | (maxlod << 7) | (minlod << 19) | (enable << 31) }
		case NV4097_SET_TEXTURE_CONTROL2:            // (index, control) { control }
		case NV4097_SET_TEXTURE_CONTROL3:            // (index, control) { control }
		case NV4097_SET_TEXTURE_CONTROL1:            // (index, control) { control }
		case NV4097_SET_TEXTURE_IMAGE_RECT:          // (index, rect) { rect }
		case NV4097_SET_VERTEX_DATA_ARRAY_OFFSET:    // (index, location, offset) { (location << 31) | offset }
		case NV4097_SET_VERTEX_DATA_ARRAY_FORMAT:    // (index, frequency, stride, size, type) { (frequency << 16) | (stride << 8) | (size << 4) | type }
		case NV4097_SET_TEX_COORD_CONTROL:           // (index, control) { control }
		case NV4097_SET_VERTEX_TEXTURE_CONTROL3:     // (index, control) { control }
		case NV4097_SET_VERTEX_TEXTURE_IMAGE_RECT:   // (index, rect) { rect }
		case NV4097_SET_VERTEX_TEXTURE_ADDRESS:      // (index, wrapu, wrapv) { wrapu | (wrapv << 8) }
		case NV4097_SET_VERTEX_TEXTURE_FILTER:       // (index, bias) { bias }
		case NV4097_SET_VERTEX_TEXTURE_CONTROL0:     // (index, enable, minLod, maxLod) { (maxLod << 7) | (minLod << 19) | (enable << 31) }
		case NV4097_SET_VERTEX_TEXTURE_BORDER_COLOR: // (index, color) { color }
		case NV4097_SET_ANISO_SPREAD:                // (index, value) { value }
		case NV4097_SET_TEXTURE_BORDER_COLOR:        // (index, color) { color }
		{
			ensure(param_count == 1);
			break;
		}
		case NV4097_SET_CONTEXT_DMA_COLOR_C:
		case NV4097_SET_TRANSFORM_PROGRAM_LOAD:
		{
			ensure(param_count == 1 || param_count == 2);
			break;
		}
		case NV4097_INVALIDATE_VERTEX_FILE: // all 0
		{
			ensure(param_count == 1 || param_count == 3);
			break;
		}
		case NV4097_SET_CLIP_MIN:
		case NV4097_SET_VERTEX_DATA_BASE_OFFSET:
		case NV4097_SET_FOG_PARAMS:
		case NV4097_SET_ALPHA_FUNC:
		case NV4097_SET_DEPTH_BOUNDS_MIN:
		case NV4097_SET_POLYGON_OFFSET_SCALE_FACTOR:
		case NV0039_SET_CONTEXT_DMA_BUFFER_IN:
		case NV4097_SET_SURFACE_CLIP_HORIZONTAL: // (x, w, y, h) { x | (w << 16), y | (h << 16) }
		case NV4097_SET_SCISSOR_HORIZONTAL:      // (x, w, y, h) { x | (w << 16), y | (h << 16) }
		case NV4097_SET_VIEWPORT_HORIZONTAL:     // (x, w, y, h) { x | (w << 16), y | (h << 16) }
		case NV4097_SET_BLEND_FUNC_SFACTOR:      // (sfcolor, sfalpha, dfcolor, dfalpha) { sfcolor | (sfalpha << 16), dfcolor | (dfalpha << 16) }
		case NV309E_SET_FORMAT:                  // (format, width, height, offset) { format | (width << 16) | (height << 24), offset }
		case NV4097_SET_INDEX_ARRAY_ADDRESS:     // (location, offset, type) { offset, location | (type << 4) }
		case NV4097_SET_TEXTURE_OFFSET:          // (index, offset, format) { offset, format }
		case NV4097_SET_VERTEX_TEXTURE_OFFSET:   // (index, offset, format) { offset, format }
		{
			ensure(param_count == 2);
			break;
		}
		case NV4097_SET_STENCIL_FUNC:
		case NV4097_SET_BACK_STENCIL_FUNC:
		case NV4097_SET_STENCIL_OP_FAIL:
		case NV4097_SET_BACK_STENCIL_OP_FAIL:
		case NV308A_POINT: // (pointX, pointY, outSizeX, outSizeY, inSizeX, inSizeY) { (pointY << 16) | pointX, (outSizeY << 16) | outSizeX, (inSizeY << 16) | inSizeX }
		{
			ensure(param_count == 3);
			break;
		}
		case NV4097_SET_SURFACE_PITCH_C:
		case NV3089_IMAGE_IN_SIZE:         // (w, h, pitch, origin, interpolator, offset, u, v) {} { (h << 16) | w, pitch | (origin << 16) | (interpolator << 24), offset, (v << 16) | u }
		case NV4097_SET_VERTEX_DATA4F_M:   // (index, v0, v1, v2, v3) { v0, v1, v2, v3 }
		case NV4097_SET_TRANSFORM_PROGRAM: // (index, d0, d1, d2, d3) { d0, d1, d2, d3 }
		{
			ensure(param_count == 4);
			break;
		}
		case NV3062_SET_COLOR_FORMAT: // (format, srcPitch, dstPitch) { format, srcPitch | (dstPitch << 16) }
		                              // (format, pitch, offset) { format, 64 | (pitch << 16), 0, offset }
		{
			ensure(param_count == 2 || param_count == 4);
			break;
		}
		case NV0039_PITCH_IN: // (inPitch, outPitch, lineLength, lineCount, inFormat, outFormat) { ... , inFormat | (outFormat << 8) }
		{
			ensure(param_count == 5);
			break;
		}
		case NV4097_SET_SURFACE_FORMAT: // (colorFormat, depthFormat, antialias, type, width, height, pitchA, offsetA, offsetZ, offsetB, pitchB) { colorFormat | (depthFormat << 5) | (type << 8) | (antialias << 12) | (width << 16) | (height << 24), pitchA, offsetA, offsetZ, offsetB, pitchB }
		{
			ensure(param_count == 6);
			break;
		}
		case NV4097_SET_VIEWPORT_OFFSET: // (offset0, offset1, offset2, offset3, scale0, scale1, scale2, scale3) { offset0, offset1, offset2, offset3, scale0, scale1, scale2, scale3 }
		{
			ensure(param_count == 8);
			break;
		}
		case NV3089_SET_COLOR_CONVERSION: // (conv, fmt, op, inX, inY, inW, inH, outX, outY, outW, outH, dsdx, dtdy) { conv, fmt, op, (inY << 16) | inX, (inH << 16) | inW, (outY << 16) | outX, (outH << 16) | outW, dsdx, dtdy }
		{
			ensure(param_count == 9);
			break;
		}
		case NV0039_OFFSET_IN: // (inOffset) { inOffset }
		                       // (inOffset, outOffset, inPitch, outPitch, lineLength, lineCount, inFormat, outFormat, notify) { ... , inFormat | (outFormat << 8), notify}
		{
			ensure(param_count == 1 || param_count == 8);
			break;
		}
		case NV4097_SET_POLYGON_STIPPLE_PATTERN:
		{
			ensure(param_count == 32);
			break;
		}
		case NV4097_SET_TRANSFORM_CONSTANT_LOAD:
		{
			ensure(param_count == 5 || param_count == 13 || param_count == 17 || param_count == 33);
			break;
		}
		case NV4097_SET_OBJECT:
		case NV4097_SET_CONTEXT_DMA_A:
		case NV4097_SET_CONTEXT_DMA_B:
		case NV4097_SET_CONTEXT_DMA_STATE:
		case NV4097_SET_CONTEXT_DMA_VERTEX_A:
		case NV4097_SET_CONTEXT_DMA_VERTEX_B:
		case NV4097_SET_CONTEXT_DMA_CLIP_ID:
		case NV4097_SET_CONTEXT_DMA_CULL_DATA:
		case NV4097_SET_SURFACE_CLIP_VERTICAL:
		case NV4097_SET_SURFACE_PITCH_A:
		case NV4097_SET_SURFACE_COLOR_AOFFSET:
		case NV4097_SET_SURFACE_ZETA_OFFSET:
		case NV4097_SET_SURFACE_COLOR_BOFFSET:
		case NV4097_SET_SURFACE_PITCH_B:
		case NV4097_SET_SURFACE_PITCH_D:
		case NV4097_SET_SURFACE_COLOR_COFFSET:
		case NV4097_SET_SURFACE_COLOR_DOFFSET:
		case NV4097_SET_WINDOW_CLIP_TYPE:
		case NV4097_SET_WINDOW_CLIP_HORIZONTAL:
		case NV4097_SET_WINDOW_CLIP_VERTICAL:
		case NV4097_SET_ALPHA_REF:
		case NV4097_SET_BLEND_FUNC_DFACTOR:
		case NV4097_SET_STENCIL_FUNC_REF:
		case NV4097_SET_STENCIL_FUNC_MASK:
		case NV4097_SET_STENCIL_OP_ZFAIL:
		case NV4097_SET_STENCIL_OP_ZPASS:
		case NV4097_SET_BACK_STENCIL_FUNC_REF:
		case NV4097_SET_BACK_STENCIL_FUNC_MASK:
		case NV4097_SET_BACK_STENCIL_OP_ZFAIL:
		case NV4097_SET_BACK_STENCIL_OP_ZPASS:
		case NV4097_SET_DEPTH_BOUNDS_MAX:
		case NV4097_SET_CLIP_MAX:
		case NV4097_SET_SCISSOR_VERTICAL:
		case NV4097_SET_VERTEX_TEXTURE_FORMAT:
		case NV4097_SET_VIEWPORT_VERTICAL:
		case NV4097_SET_POINT_CENTER_MODE:
		case NV4097_SET_VIEWPORT_SCALE:
		case NV4097_SET_POLY_OFFSET_POINT_ENABLE:
		case NV4097_SET_POLYGON_OFFSET_BIAS:
		case NV4097_SET_VERTEX_DATA_SCALED4S_M:
		case NV4097_SET_SPECULAR_ENABLE:
		case NV4097_SET_PERFORMANCE_PARAMS:
		case NV4097_SET_FLAT_SHADE_OP:
		case NV4097_SET_EDGE_FLAG:
		case NV4097_SET_VERTEX_DATA3F_M:
		case NV4097_PIPE_NOP:
		case NV4097_SET_VERTEX_DATA_BASE_INDEX:
		case NV4097_ARRAY_ELEMENT16:
		case NV4097_ARRAY_ELEMENT32:
		case NV4097_INLINE_ARRAY:
		case NV4097_SET_INDEX_ARRAY_DMA:
		case NV4097_SET_VERTEX_DATA2F_M:
		case NV4097_SET_VERTEX_DATA2S_M:
		case NV4097_SET_VERTEX_DATA4UB_M:
		case NV4097_SET_VERTEX_DATA4S_M:
		case NV4097_SET_TEXTURE_FORMAT:
		case NV4097_SET_COLOR_KEY_COLOR:
		case NV4097_SET_INDEXED_CONSTANT_READ_LIMITS:
		case NV4097_SET_SURFACE_COMPRESSION:
		case NV4097_SET_CLEAR_RECT_HORIZONTAL:
		case NV4097_SET_CLEAR_RECT_VERTICAL:
		case NV4097_SET_CLIP_ID_TEST_ENABLE:
		case NV4097_SET_VERTEX_DATA1F_M:
		case NV4097_SET_TRANSFORM_EXECUTION_MODE:
		case NV4097_SET_TRANSFORM_CONSTANT:
		case NV4097_SET_ATTRIB_COLOR:
		case NV4097_SET_ATTRIB_TEX_COORD:
		case NV4097_SET_ATTRIB_TEX_COORD_EX:
		case NV4097_SET_ATTRIB_UCLIP0:
		case NV4097_SET_ATTRIB_UCLIP1:
		case NV0039_SET_OBJECT:
		case NV0039_SET_CONTEXT_DMA_NOTIFIES:
		case NV0039_SET_CONTEXT_DMA_BUFFER_OUT:
		case NV0039_PITCH_OUT:
		case NV0039_LINE_LENGTH_IN:
		case NV0039_LINE_COUNT:
		case NV0039_FORMAT:
		case NV3062_SET_OBJECT:
		case NV3062_SET_CONTEXT_DMA_NOTIFIES:
		case NV3062_SET_CONTEXT_DMA_IMAGE_SOURCE:
		case NV3062_SET_PITCH:
		case NV3062_SET_OFFSET_SOURCE:
		case NV309E_SET_OBJECT:
		case NV309E_SET_CONTEXT_DMA_NOTIFIES:
		case NV309E_SET_OFFSET:
		case NV308A_SET_OBJECT:
		case NV308A_SET_CONTEXT_DMA_NOTIFIES:
		case NV308A_SET_CONTEXT_COLOR_KEY:
		case NV308A_SET_CONTEXT_CLIP_RECTANGLE:
		case NV308A_SET_CONTEXT_PATTERN:
		case NV308A_SET_CONTEXT_ROP:
		case NV308A_SET_CONTEXT_BETA1:
		case NV308A_SET_CONTEXT_BETA4:
		case NV308A_SET_CONTEXT_SURFACE:
		case NV308A_SET_COLOR_CONVERSION:
		case NV308A_SET_OPERATION:
		case NV308A_SET_COLOR_FORMAT:
		case NV308A_SIZE_OUT:
		case NV308A_SIZE_IN:
		case NV308A_COLOR:
		case NV3089_SET_OBJECT:
		case NV3089_SET_CONTEXT_DMA_NOTIFIES:
		case NV3089_SET_CONTEXT_PATTERN:
		case NV3089_SET_CONTEXT_ROP:
		case NV3089_SET_CONTEXT_BETA1:
		case NV3089_SET_CONTEXT_BETA4:
		case NV3089_SET_COLOR_FORMAT:
		case NV3089_SET_OPERATION:
		case NV3089_CLIP_POINT:
		case NV3089_CLIP_SIZE:
		case NV3089_IMAGE_OUT_POINT:
		case NV3089_IMAGE_OUT_SIZE:
		case NV3089_DS_DX:
		case NV3089_DT_DY:
		case NV3089_IMAGE_IN_FORMAT:
		case NV3089_IMAGE_IN_OFFSET:
		case NV3089_IMAGE_IN:
		case GCM_SET_DRIVER_OBJECT:
		case GCM_FLIP_HEAD:
		case GCM_DRIVER_QUEUE:
		case GCM_SET_USER_COMMAND:
		case GCM_FLIP_COMMAND:
		{
			// TODO
			fmt::throw_exception("Unverified gcm method 0x%x", method);
		}
		default:
		{
			fmt::throw_exception("Unknown gcm method 0x%x", method);
		}
		}
	}

	u32 gcm_method(u32 method, u32 param_count)
	{
		check_method_param_count(method, param_count);
		method <<= 2; // We need to shift to get to actual CELL_GCM methods
		return (param_count << RSX_METHOD_COUNT_SHIFT) | method;
	}

	u32 gcm_method(u32 method, u32 index, u32 param_count)
	{
		check_method_param_count(method, param_count);

		switch (method)
		{
		case NV4097_SET_TEXTURE_CONTROL2:
		case NV4097_SET_TEXTURE_CONTROL3:
		{
			AUDIT(index < 16); // There are index debug asserts in cellGcm.
			index *= 4;
			break;
		}
		case NV4097_SET_VERTEX_DATA_ARRAY_OFFSET:
		case NV4097_SET_VERTEX_DATA_ARRAY_FORMAT:
		case NV4097_SET_TEX_COORD_CONTROL:
		case NV4097_SET_ANISO_SPREAD:
		{
			index *= 4;
			break;
		}
		case NV4097_SET_VERTEX_DATA4F_M:
		case NV4097_SET_TRANSFORM_PROGRAM:
		{
			index *= 16;
			break;
		}
		case NV4097_SET_TEXTURE_FILTER:
		case NV4097_SET_TEXTURE_ADDRESS:
		case NV4097_SET_TEXTURE_CONTROL0:
		case NV4097_SET_TEXTURE_OFFSET:
		case NV4097_SET_TEXTURE_CONTROL1:
		case NV4097_SET_TEXTURE_IMAGE_RECT:
		case NV4097_SET_TEXTURE_BORDER_COLOR:
		{
			AUDIT(index < 16); // There are index debug asserts in cellGcm.
			index *= 32;
			break;
		}
		case NV4097_SET_VERTEX_TEXTURE_OFFSET:
		case NV4097_SET_VERTEX_TEXTURE_CONTROL3:
		case NV4097_SET_VERTEX_TEXTURE_IMAGE_RECT:
		case NV4097_SET_VERTEX_TEXTURE_ADDRESS:
		case NV4097_SET_VERTEX_TEXTURE_FILTER:
		case NV4097_SET_VERTEX_TEXTURE_CONTROL0:
		case NV4097_SET_VERTEX_TEXTURE_BORDER_COLOR:
		{
			AUDIT(index < 4); // There are index debug asserts in cellGcm.
			index *= 32;
			break;
		}
		default:
		{
			fmt::throw_exception("Unexpected gcm method 0x%x", method);
		}
		}

		method <<= 2; // We need to shift to get to actual CELL_GCM methods
		method += index;

		return (param_count << RSX_METHOD_COUNT_SHIFT) | method;
	}

	void gcm_method_set(vm::bptr<u32>& dst, u32 method, u32 data)
	{
		write_to_ptr_unsafe(dst++, gcm_method(method, 1));
		write_to_ptr_unsafe(dst++, data);
	}

	void gcm_method_set(vm::bptr<u32>& dst, u32 method, const std::vector<u32>& data)
	{
		write_to_ptr_unsafe(dst++, gcm_method(method, ::size32(data)));
		write_to_ptr_unsafe(dst, data);
		dst += data.size();
	}
}
