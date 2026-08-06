#pragma once

enum CellRescError : u32
{
	CELL_RESC_ERROR_NOT_INITIALIZED   = 0x80210301,
	CELL_RESC_ERROR_REINITIALIZED     = 0x80210302,
	CELL_RESC_ERROR_BAD_ALIGNMENT     = 0x80210303,
	CELL_RESC_ERROR_BAD_ARGUMENT      = 0x80210304,
	CELL_RESC_ERROR_LESS_MEMORY       = 0x80210305,
	CELL_RESC_ERROR_GCM_FLIP_QUE_FULL = 0x80210306,
	CELL_RESC_ERROR_BAD_COMBINATION   = 0x80210307,
	CELL_RESC_ERROR_x308              = 0x80210308, // TODO: find proper name
};

enum
{
	COLOR_BUFFER_ALIGNMENT = 128,
	VERTEX_BUFFER_ALIGNMENT = 4,
	FRAGMENT_SHADER_ALIGNMENT = 64,
	VERTEX_NUMBER_NORMAL = 4,

	SRC_BUFFER_NUM = 8,
	MAX_DST_BUFFER_NUM = 6
};

enum CellRescBufferMode : u32 // CellRescDisplayBufferMode
{
	CELL_RESC_720x480   = 0x1,
	CELL_RESC_720x576   = 0x2,
	CELL_RESC_1280x720  = 0x4,
	CELL_RESC_1920x1080 = 0x8,
};

enum CellRescPalTemporalMode
{
	CELL_RESC_PAL_50                            = 0,
	CELL_RESC_PAL_60_DROP                       = 1,
	CELL_RESC_PAL_60_INTERPOLATE                = 2,
	CELL_RESC_PAL_60_INTERPOLATE_30_DROP        = 3,
	CELL_RESC_PAL_60_INTERPOLATE_DROP_FLEXIBLE  = 4,
	CELL_RESC_PAL_60_FOR_HSYNC                  = 5,
};

enum CellRescRatioConvertMode
{
	CELL_RESC_FULLSCREEN             = 0,
	CELL_RESC_LETTERBOX              = 1,
	CELL_RESC_PANSCAN                = 2,
};

enum CellRescFlipMode
{
	CELL_RESC_DISPLAY_VSYNC          = 0,
	CELL_RESC_DISPLAY_HSYNC          = 1,
};

enum CellRescDstFormat
{
	CELL_RESC_SURFACE_A8R8G8B8       = 8,  // == CELL_GCM_SURFACE_A8R8G8B8
	CELL_RESC_SURFACE_F_W16Z16Y16X16 = 11, // == CELL_GCM_SURFACE_F_W16Z16Y16X16
};

enum CellRescTableElement
{
	CELL_RESC_ELEMENT_HALF = 0,
	CELL_RESC_ELEMENT_FLOAT = 1,
};

enum CellRescResourcePolicy
{
	CELL_RESC_CONSTANT_VRAM          = 0x0,
	CELL_RESC_MINIMUM_VRAM           = 0x1,
	CELL_RESC_CONSTANT_GPU_LOAD      = 0x0,
	CELL_RESC_MINIMUM_GPU_LOAD       = 0x2,
};

enum CellRescConvolutionFilterMode // CellRescInterlaceFilterMode
{
	CELL_RESC_NORMAL_BILINEAR        = 0,
	CELL_RESC_INTERLACE_FILTER       = 1,
	CELL_RESC_3X3_GAUSSIAN           = 2,
	CELL_RESC_2X3_QUINCUNX           = 3,
	CELL_RESC_2X3_QUINCUNX_ALT       = 4,
};

typedef CellRescConvolutionFilterMode CellRescInterlaceFilterMode;

struct CellRescDsts
{
	be_t<u32> format;
	be_t<u32> pitch;
	be_t<u32> heightAlign;
};

struct CellRescInitConfig
{
	be_t<u32> size; // usz
	be_t<u32> resourcePolicy;
	be_t<u32> supportModes;
	be_t<u32> ratioMode;
	be_t<u32> palTemporalMode;
	be_t<u32> interlaceMode;
	be_t<u32> flipMode;
};

struct CellRescSrc
{
	be_t<u32> format;
	be_t<u32> pitch;
	be_t<u16> width;
	be_t<u16> height;
	be_t<u32> offset;
};

typedef void(CellRescHandler)(u32 head);

struct fragment_shader // Not sure what this looks like
{
	u32 unk_0 = 0;
	u32 unk_1 = 0;
	vm::ptr<void> data = vm::null;
	u32 size = 0;
	u32 offset = 0;
};

#pragma pack (push, 1)
struct cell_resc_manager
{
	CellRescInitConfig config {};
	CellRescSrc srcs[SRC_BUFFER_NUM] {};
	CellRescDsts dsts[4] {};
	u32 activeDst {}; // 4 byte Pointer to CellRescDsts
	CellRescBufferMode bufferMode {};
	u32 tableLength = 0;
	u8 pad1[36];
	vm::ptr<fragment_shader> usedFragmentShader = vm::null;
	vm::ptr<void> colorBuffers = vm::null;
	vm::ptr<void> vertexArray = vm::null;
	vm::ptr<void> fragmentShader = vm::null;
	u32 table = 0;
	u8 pad2[4];
	u32 width = 0;
	u32 height = 0;
	u32 pitch = 0;
	u8 pad3[4];
	u32 bufferSize = 0;
	u32 buffersOffsets[MAX_DST_BUFFER_NUM] {};
	u8 pad4[4];
	u32 depth = 0;
	float horizontal = 0.0f;
	float vertical = 0.0f;
	atomic_t<bool> is_initialized = false;

	u8 pad4[95];
	u64 lastFlipTime = 0;
	u8 pad5[32];
	u8 flipStatus = 0;
	u8 pad6[7];
	float palInterpolateDropFlexRatio = 0.0f;

	vm::ptr<CellRescHandler> handler = vm::null;
};
#pragma pop
