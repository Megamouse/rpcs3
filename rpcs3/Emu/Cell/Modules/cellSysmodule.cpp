#include "stdafx.h"
#include "Emu/Cell/PPUModule.h"
#include "Emu/Cell/lv2/sys_process.h"

LOG_CHANNEL(cellSysmodule);

constexpr auto CELL_SYSMODULE_LOADED = CELL_OK;

enum CellSysmoduleError : u32
{
	CELL_SYSMODULE_ERROR_DUPLICATED           = 0x80012001,
	CELL_SYSMODULE_ERROR_UNKNOWN              = 0x80012002,
	CELL_SYSMODULE_ERROR_UNLOADED             = 0x80012003,
	CELL_SYSMODULE_ERROR_INVALID_MEMCONTAINER = 0x80012004,
	CELL_SYSMODULE_ERROR_FATAL                = 0x800120ff,
};

template<>
void fmt_class_string<CellSysmoduleError>::format(std::string& out, u64 arg)
{
	format_enum(out, arg, [](auto error)
	{
		switch (error)
		{
			STR_CASE(CELL_SYSMODULE_ERROR_DUPLICATED);
			STR_CASE(CELL_SYSMODULE_ERROR_UNKNOWN);
			STR_CASE(CELL_SYSMODULE_ERROR_UNLOADED);
			STR_CASE(CELL_SYSMODULE_ERROR_INVALID_MEMCONTAINER);
			STR_CASE(CELL_SYSMODULE_ERROR_FATAL);
		}

		return unknown;
	});
}

static const char* get_module_name(u16 id)
{
	switch (id)
	{
	case 0x0000: return "sys_net";
	case 0x0001: return "cellHttp";
	case 0x0002: return "cellHttpUtil";
	case 0x0003: return "cellSsl";
	case 0x0004: return "cellHttps";
	case 0x0005: return "libvdec";
	case 0x0006: return "cellAdec";
	case 0x0007: return "cellDmux";
	case 0x0008: return "cellVpost";
	case 0x0009: return "cellRtc";
	case 0x000a: return "cellSpurs";
	case 0x000b: return "cellOvis";
	case 0x000c: return "cellSheap";
	case 0x000d: return "cellSync";
	case 0x000e: return "sys_fs";
	case 0x000f: return "cellJpgDec";
	case 0x0010: return "cellGcmSys";
	case 0x0011: return "cellAudio";
	case 0x0012: return "cellPamf";
	case 0x0013: return "cellAtrac";
	case 0x0014: return "cellNetCtl";
	case 0x0015: return "cellSysutil";
	case 0x0016: return "sceNp";
	case 0x0017: return "sys_io";
	case 0x0018: return "cellPngDec";
	case 0x0019: return "cellFont";
	case 0x001a: return "cellFontFT";
	case 0x001b: return "cell_FreeType2";
	case 0x001c: return "cellUsbd";
	case 0x001d: return "cellSail";
	case 0x001e: return "cellL10n";
	case 0x001f: return "cellResc";
	case 0x0020: return "cellDaisy";
	case 0x0021: return "cellKey2char";
	case 0x0022: return "cellMic";
	case 0x0023: return "cellCamera";
	case 0x0024: return "cellVdecMpeg2";
	case 0x0025: return "cellVdecAvc";
	case 0x0026: return "cellAdecLpcm";
	case 0x0027: return "cellAdecAc3";
	case 0x0028: return "cellAdecAtx";
	case 0x0029: return "cellAdecAt3";
	case 0x002a: return "cellDmuxPamf";
	case 0x002b: return nullptr;
	case 0x002c: return nullptr;
	case 0x002d: return nullptr;
	case 0x002e: return "sys_lv2dbg";
	case 0x002f: return "cellSysutilAvcExt";
	case 0x0030: return "cellUsbPspcm";
	case 0x0031: return "cellSysutilAvconfExt";
	case 0x0032: return "cellUserInfo";
	case 0x0033: return "cellSaveData";
	case 0x0034: return "cellSubDisplay";
	case 0x0035: return "cellRec";
	case 0x0036: return "cellVideoExportUtility";
	case 0x0037: return "cellGameExec";
	case 0x0038: return "sceNp2";
	case 0x0039: return "cellSysutilAp";
	case 0x003a: return "sceNpClans";
	case 0x003b: return "cellOskExtUtility";
	case 0x003c: return "cellVdecDivx";
	case 0x003d: return "cellJpgEnc";
	case 0x003e: return "cellGame";
	case 0x003f: return "cellBGDLUtility";
	case 0x0040: return "cell_FreeType2";
	case 0x0041: return "cellVideoUpload";
	case 0x0042: return "cellSysconfExtUtility";
	case 0x0043: return "cellFiber";
	case 0x0044: return "sceNpCommerce2";
	case 0x0045: return "sceNpTus";
	case 0x0046: return "cellVoice";
	case 0x0047: return "cellAdecCelp8";
	case 0x0048: return "cellCelp8Enc";
	case 0x0049: return "cellSysutilMisc";
	case 0x004a: return "cellMusicUtility";
	// TODO: Check if those libad are correctly matched.
	// They belong to those IDs but actual order is unknown.
	case 0x004b: return "libad_core";
	case 0x004c: return "libad_async";
	case 0x004d: return "libad_billboard_util";
	case 0x004e: return "cellScreenShotUtility";
	case 0x004f: return "cellMusicDecodeUtility";
	case 0x0050: return "cellSpursJq";
	case 0x0052: return "cellPngEnc";
	case 0x0053: return "cellMusicDecodeUtility";
	case 0x0054: return "libmedi";
	case 0x0055: return "cellSync2";
	case 0x0056: return "sceNpUtil";
	case 0x0057: return "cellRudp";
	case 0x0059: return "sceNpSns";
	case 0x005a: return "libgem";
	case 0x005c: return "cellCrossController";
	case 0xf00a: return "cellCelpEnc";
	case 0xf010: return "cellGifDec";
	case 0xf019: return "cellAdecCelp";
	case 0xf01b: return "cellAdecM2bc";
	case 0xf01d: return "cellAdecM4aac";
	case 0xf01e: return "cellAdecMp3";
	case 0xf023: return "cellImeJpUtility";
	case 0xf028: return "cellMusicUtility";
	case 0xf029: return "cellPhotoUtility";
	case 0xf02a: return "cellPrintUtility";
	case 0xf02b: return "cellPhotoImportUtil";
	case 0xf02c: return "cellMusicExportUtility";
	case 0xf02e: return "cellPhotoDecodeUtil";
	case 0xf02f: return "cellSearchUtility";
	case 0xf030: return "cellSysutilAvc2";
	case 0xf034: return "cellSailRec";
	case 0xf035: return "sceNpTrophy";
	case 0xf044: return "cellSysutilNpEula";
	case 0xf053: return "cellAdecAt3multi";
	case 0xf054: return "cellAtracMulti";
	}

	return nullptr;
}

enum CellSysmoduleID
{
	CELL_SYSMODULE_INVALID                  = 0xffff,
	CELL_SYSMODULE_NET                      = 0x0000,
	CELL_SYSMODULE_HTTP                     = 0x0001,
	CELL_SYSMODULE_HTTP_UTIL                = 0x0002,
	CELL_SYSMODULE_SSL                      = 0x0003,
	CELL_SYSMODULE_HTTPS                    = 0x0004,
	CELL_SYSMODULE_VDEC                     = 0x0005,
	CELL_SYSMODULE_ADEC                     = 0x0006,
	CELL_SYSMODULE_DMUX                     = 0x0007,
	CELL_SYSMODULE_VPOST                    = 0x0008,
	CELL_SYSMODULE_RTC                      = 0x0009,
	CELL_SYSMODULE_SPURS                    = 0x000a,
	CELL_SYSMODULE_OVIS                     = 0x000b,
	CELL_SYSMODULE_SHEAP                    = 0x000c,
	CELL_SYSMODULE_SYNC                     = 0x000d,
	CELL_SYSMODULE_FS                       = 0x000e,
	CELL_SYSMODULE_JPGDEC                   = 0x000f,
	CELL_SYSMODULE_GCM_SYS                  = 0x0010,
	CELL_SYSMODULE_GCM                      = 0x0010,
	CELL_SYSMODULE_AUDIO                    = 0x0011,
	CELL_SYSMODULE_PAMF                     = 0x0012,
	CELL_SYSMODULE_ATRAC3PLUS               = 0x0013,
	CELL_SYSMODULE_NETCTL                   = 0x0014,
	CELL_SYSMODULE_SYSUTIL                  = 0x0015,
	CELL_SYSMODULE_SYSUTIL_NP               = 0x0016,
	CELL_SYSMODULE_IO                       = 0x0017,
	CELL_SYSMODULE_PNGDEC                   = 0x0018,
	CELL_SYSMODULE_FONT                     = 0x0019,
	CELL_SYSMODULE_FONTFT                   = 0x001a,
	CELL_SYSMODULE_FREETYPE                 = 0x001b,
	CELL_SYSMODULE_USBD                     = 0x001c,
	CELL_SYSMODULE_SAIL                     = 0x001d,
	CELL_SYSMODULE_L10N                     = 0x001e,
	CELL_SYSMODULE_RESC                     = 0x001f,
	CELL_SYSMODULE_DAISY                    = 0x0020,
	CELL_SYSMODULE_KEY2CHAR                 = 0x0021,
	CELL_SYSMODULE_MIC                      = 0x0022,
	CELL_SYSMODULE_CAMERA                   = 0x0023,
	CELL_SYSMODULE_VDEC_MPEG2               = 0x0024,
	CELL_SYSMODULE_VDEC_AVC                 = 0x0025,
	CELL_SYSMODULE_ADEC_LPCM                = 0x0026,
	CELL_SYSMODULE_ADEC_AC3                 = 0x0027,
	CELL_SYSMODULE_ADEC_ATX                 = 0x0028,
	CELL_SYSMODULE_ADEC_AT3                 = 0x0029,
	CELL_SYSMODULE_DMUX_PAMF                = 0x002a,
	CELL_SYSMODULE_VDEC_AL                  = 0x002b,
	CELL_SYSMODULE_ADEC_AL                  = 0x002c,
	CELL_SYSMODULE_DMUX_AL                  = 0x002d,
	CELL_SYSMODULE_LV2DBG                   = 0x002e,
	CELL_SYSMODULE_USBPSPCM                 = 0x0030,
	CELL_SYSMODULE_AVCONF_EXT               = 0x0031,
	CELL_SYSMODULE_SYSUTIL_USERINFO         = 0x0032,
	CELL_SYSMODULE_SYSUTIL_SAVEDATA         = 0x0033,
	CELL_SYSMODULE_SUBDISPLAY               = 0x0034,
	CELL_SYSMODULE_SYSUTIL_REC              = 0x0035,
	CELL_SYSMODULE_VIDEO_EXPORT             = 0x0036,
	CELL_SYSMODULE_SYSUTIL_GAME_EXEC        = 0x0037,
	CELL_SYSMODULE_SYSUTIL_NP2              = 0x0038,
	CELL_SYSMODULE_SYSUTIL_AP               = 0x0039,
	CELL_SYSMODULE_SYSUTIL_NP_CLANS         = 0x003a,
	CELL_SYSMODULE_SYSUTIL_OSK_EXT          = 0x003b,
	CELL_SYSMODULE_VDEC_DIVX                = 0x003c,
	CELL_SYSMODULE_JPGENC                   = 0x003d,
	CELL_SYSMODULE_SYSUTIL_GAME             = 0x003e,
	CELL_SYSMODULE_BGDL                     = 0x003f,
	CELL_SYSMODULE_FREETYPE_TT              = 0x0040,
	CELL_SYSMODULE_SYSUTIL_VIDEO_UPLOAD     = 0x0041,
	CELL_SYSMODULE_SYSUTIL_SYSCONF_EXT      = 0x0042,
	CELL_SYSMODULE_FIBER                    = 0x0043,
	CELL_SYSMODULE_SYSUTIL_NP_COMMERCE2     = 0x0044,
	CELL_SYSMODULE_SYSUTIL_NP_TUS           = 0x0045,
	CELL_SYSMODULE_VOICE                    = 0x0046,
	CELL_SYSMODULE_ADEC_CELP8               = 0x0047,
	CELL_SYSMODULE_CELP8ENC                 = 0x0048,
	CELL_SYSMODULE_SYSUTIL_LICENSEAREA      = 0x0049,
	CELL_SYSMODULE_SYSUTIL_MUSIC2           = 0x004a,
	CELL_SYSMODULE_SYSUTIL_SCREENSHOT       = 0x004e,
	CELL_SYSMODULE_SYSUTIL_MUSIC_DECODE     = 0x004f,
	CELL_SYSMODULE_SPURS_JQ                 = 0x0050,
	CELL_SYSMODULE_PNGENC                   = 0x0052,
	CELL_SYSMODULE_SYSUTIL_MUSIC_DECODE2    = 0x0053,
	CELL_SYSMODULE_SYNC2                    = 0x0055,
	CELL_SYSMODULE_SYSUTIL_NP_UTIL          = 0x0056,
	CELL_SYSMODULE_RUDP                     = 0x0057,
	CELL_SYSMODULE_SYSUTIL_NP_SNS           = 0x0059,
	CELL_SYSMODULE_GEM                      = 0x005a,
	CELL_SYSMODULE_SYSUTIL_CROSS_CONTROLLER = 0x005c,
	CELL_SYSMODULE_CELPENC                  = 0xf00a,
	CELL_SYSMODULE_GIFDEC                   = 0xf010,
	CELL_SYSMODULE_ADEC_CELP                = 0xf019,
	CELL_SYSMODULE_ADEC_M2BC                = 0xf01b,
	CELL_SYSMODULE_ADEC_M4AAC               = 0xf01d,
	CELL_SYSMODULE_ADEC_MP3                 = 0xf01e,
	CELL_SYSMODULE_IMEJP                    = 0xf023,
	CELL_SYSMODULE_SYSUTIL_MUSIC            = 0xf028,
	CELL_SYSMODULE_PHOTO_EXPORT             = 0xf029,
	CELL_SYSMODULE_PRINT                    = 0xf02a,
	CELL_SYSMODULE_PHOTO_IMPORT             = 0xf02b,
	CELL_SYSMODULE_MUSIC_EXPORT             = 0xf02c,
	CELL_SYSMODULE_PHOTO_DECODE             = 0xf02e,
	CELL_SYSMODULE_SYSUTIL_SEARCH           = 0xf02f,
	CELL_SYSMODULE_SYSUTIL_AVCHAT2          = 0xf030,
	CELL_SYSMODULE_SAIL_REC                 = 0xf034,
	CELL_SYSMODULE_SYSUTIL_NP_TROPHY        = 0xf035,
	CELL_SYSMODULE_LIBATRAC3MULTI           = 0xf054,
};

static const char* get_module_id(u16 id)
{
	static thread_local char tls_id_name[8]; // for test

	switch (id)
	{
	case 0x0000: return "CELL_SYSMODULE_NET";
	case 0x0001: return "CELL_SYSMODULE_HTTP";
	case 0x0002: return "CELL_SYSMODULE_HTTP_UTIL";
	case 0x0003: return "CELL_SYSMODULE_SSL";
	case 0x0004: return "CELL_SYSMODULE_HTTPS";
	case 0x0005: return "CELL_SYSMODULE_VDEC";
	case 0x0006: return "CELL_SYSMODULE_ADEC";
	case 0x0007: return "CELL_SYSMODULE_DMUX";
	case 0x0008: return "CELL_SYSMODULE_VPOST";
	case 0x0009: return "CELL_SYSMODULE_RTC";
	case 0x000a: return "CELL_SYSMODULE_SPURS";
	case 0x000b: return "CELL_SYSMODULE_OVIS";
	case 0x000c: return "CELL_SYSMODULE_SHEAP";
	case 0x000d: return "CELL_SYSMODULE_SYNC";
	case 0x000e: return "CELL_SYSMODULE_FS";
	case 0x000f: return "CELL_SYSMODULE_JPGDEC";
	case 0x0010: return "CELL_SYSMODULE_GCM_SYS";
	case 0x0011: return "CELL_SYSMODULE_AUDIO";
	case 0x0012: return "CELL_SYSMODULE_PAMF";
	case 0x0013: return "CELL_SYSMODULE_ATRAC3PLUS";
	case 0x0014: return "CELL_SYSMODULE_NETCTL";
	case 0x0015: return "CELL_SYSMODULE_SYSUTIL";
	case 0x0016: return "CELL_SYSMODULE_SYSUTIL_NP";
	case 0x0017: return "CELL_SYSMODULE_IO";
	case 0x0018: return "CELL_SYSMODULE_PNGDEC";
	case 0x0019: return "CELL_SYSMODULE_FONT";
	case 0x001a: return "CELL_SYSMODULE_FONTFT";
	case 0x001b: return "CELL_SYSMODULE_FREETYPE";
	case 0x001c: return "CELL_SYSMODULE_USBD";
	case 0x001d: return "CELL_SYSMODULE_SAIL";
	case 0x001e: return "CELL_SYSMODULE_L10N";
	case 0x001f: return "CELL_SYSMODULE_RESC";
	case 0x0020: return "CELL_SYSMODULE_DAISY";
	case 0x0021: return "CELL_SYSMODULE_KEY2CHAR";
	case 0x0022: return "CELL_SYSMODULE_MIC";
	case 0x0023: return "CELL_SYSMODULE_CAMERA";
	case 0x0024: return "CELL_SYSMODULE_VDEC_MPEG2";
	case 0x0025: return "CELL_SYSMODULE_VDEC_AVC";
	case 0x0026: return "CELL_SYSMODULE_ADEC_LPCM";
	case 0x0027: return "CELL_SYSMODULE_ADEC_AC3";
	case 0x0028: return "CELL_SYSMODULE_ADEC_ATX";
	case 0x0029: return "CELL_SYSMODULE_ADEC_AT3";
	case 0x002a: return "CELL_SYSMODULE_DMUX_PAMF";
	case 0x002b: return "CELL_SYSMODULE_VDEC_AL";
	case 0x002c: return "CELL_SYSMODULE_ADEC_AL";
	case 0x002d: return "CELL_SYSMODULE_DMUX_AL";
	case 0x002e: return "CELL_SYSMODULE_LV2DBG";
	case 0x002f: return "CELL_SYSMODULE_SYSUTIL_AVCHAT";
	case 0x0030: return "CELL_SYSMODULE_USBPSPCM";
	case 0x0031: return "CELL_SYSMODULE_AVCONF_EXT";
	case 0x0032: return "CELL_SYSMODULE_SYSUTIL_USERINFO";
	case 0x0033: return "CELL_SYSMODULE_SYSUTIL_SAVEDATA";
	case 0x0034: return "CELL_SYSMODULE_SUBDISPLAY";
	case 0x0035: return "CELL_SYSMODULE_SYSUTIL_REC";
	case 0x0036: return "CELL_SYSMODULE_VIDEO_EXPORT";
	case 0x0037: return "CELL_SYSMODULE_SYSUTIL_GAME_EXEC";
	case 0x0038: return "CELL_SYSMODULE_SYSUTIL_NP2";
	case 0x0039: return "CELL_SYSMODULE_SYSUTIL_AP";
	case 0x003a: return "CELL_SYSMODULE_SYSUTIL_NP_CLANS";
	case 0x003b: return "CELL_SYSMODULE_SYSUTIL_OSK_EXT";
	case 0x003c: return "CELL_SYSMODULE_VDEC_DIVX";
	case 0x003d: return "CELL_SYSMODULE_JPGENC";
	case 0x003e: return "CELL_SYSMODULE_SYSUTIL_GAME";
	case 0x003f: return "CELL_SYSMODULE_BGDL";
	case 0x0040: return "CELL_SYSMODULE_FREETYPE_TT";
	case 0x0041: return "CELL_SYSMODULE_SYSUTIL_VIDEO_UPLOAD";
	case 0x0042: return "CELL_SYSMODULE_SYSUTIL_SYSCONF_EXT";
	case 0x0043: return "CELL_SYSMODULE_FIBER";
	case 0x0044: return "CELL_SYSMODULE_SYSUTIL_NP_COMMERCE2";
	case 0x0045: return "CELL_SYSMODULE_SYSUTIL_NP_TUS";
	case 0x0046: return "CELL_SYSMODULE_VOICE";
	case 0x0047: return "CELL_SYSMODULE_ADEC_CELP8";
	case 0x0048: return "CELL_SYSMODULE_CELP8ENC";
	case 0x0049: return "CELL_SYSMODULE_SYSUTIL_LICENSEAREA";
	case 0x004a: return "CELL_SYSMODULE_SYSUTIL_MUSIC2";
	// TODO: Check if those libad are correctly matched.
	// They belong to those IDs but actual order is unknown.
	case 0x004b: return "CELL_SYSMODULE_AD_CORE";
	case 0x004c: return "CELL_SYSMODULE_AD_ASYNC";
	case 0x004d: return "CELL_SYSMODULE_AD_BILLBOARD_UTIL";
	case 0x004e: return "CELL_SYSMODULE_SYSUTIL_SCREENSHOT";
	case 0x004f: return "CELL_SYSMODULE_SYSUTIL_MUSIC_DECODE";
	case 0x0050: return "CELL_SYSMODULE_SPURS_JQ";
	case 0x0052: return "CELL_SYSMODULE_PNGENC";
	case 0x0053: return "CELL_SYSMODULE_SYSUTIL_MUSIC_DECODE2";
	case 0x0054: return "CELL_SYSMODULE_MEDI";
	case 0x0055: return "CELL_SYSMODULE_SYNC2";
	case 0x0056: return "CELL_SYSMODULE_SYSUTIL_NP_UTIL";
	case 0x0057: return "CELL_SYSMODULE_RUDP";
	case 0x0059: return "CELL_SYSMODULE_SYSUTIL_NP_SNS";
	case 0x005a: return "CELL_SYSMODULE_GEM";
	case 0x005c: return "CELL_SYSMODULE_SYSUTIL_CROSS_CONTROLLER";
	case 0xf00a: return "CELL_SYSMODULE_CELPENC";
	case 0xf010: return "CELL_SYSMODULE_GIFDEC";
	case 0xf019: return "CELL_SYSMODULE_ADEC_CELP";
	case 0xf01b: return "CELL_SYSMODULE_ADEC_M2BC";
	case 0xf01d: return "CELL_SYSMODULE_ADEC_M4AAC";
	case 0xf01e: return "CELL_SYSMODULE_ADEC_MP3";
	case 0xf023: return "CELL_SYSMODULE_IMEJP";
	case 0xf028: return "CELL_SYSMODULE_SYSUTIL_MUSIC";
	case 0xf029: return "CELL_SYSMODULE_PHOTO_EXPORT";
	case 0xf02a: return "CELL_SYSMODULE_PRINT";
	case 0xf02b: return "CELL_SYSMODULE_PHOTO_IMPORT";
	case 0xf02c: return "CELL_SYSMODULE_MUSIC_EXPORT";
	case 0xf02e: return "CELL_SYSMODULE_PHOTO_DECODE";
	case 0xf02f: return "CELL_SYSMODULE_SYSUTIL_SEARCH";
	case 0xf030: return "CELL_SYSMODULE_SYSUTIL_AVCHAT2";
	case 0xf034: return "CELL_SYSMODULE_SAIL_REC";
	case 0xf035: return "CELL_SYSMODULE_SYSUTIL_NP_TROPHY";
	case 0xf044: return "CELL_SYSMODULE_SYSUTIL_NP_EULA";
	case 0xf053: return "CELL_SYSMODULE_ADEC_AT3MULTI";
	case 0xf054: return "CELL_SYSMODULE_LIBATRAC3MULTI";
	case 0xffff: return "CELL_SYSMODULE_INVALID";
	}

	std::snprintf(tls_id_name, sizeof(tls_id_name), "0x%04X", id);
	return tls_id_name;
}

error_code cellSysmoduleInitialize()
{
	cellSysmodule.warning("cellSysmoduleInitialize()");
	return CELL_OK;
}

error_code cellSysmoduleFinalize()
{
	cellSysmodule.warning("cellSysmoduleFinalize()");
	return CELL_OK;
}

error_code cellSysmoduleSetMemcontainer(u32 ct_id)
{
	cellSysmodule.todo("cellSysmoduleSetMemcontainer(ct_id=0x%x)", ct_id);

	int iStack116;
	undefined auStack44[4];
	uint uStack40;

	error_code error = CELL_OK;
	if (error = sys_lwmutex_lock((sys_lwmutex_t*)PTR_DAT_00009e98, 0))
	{
		return CELL_SYSMODULE_ERROR_FATAL;
	}

	if (ct_id == 0xffffffff)
	{
		**(undefined4**)(iStack116 + -0x7ff4) = 0;
	}
	else
	{
		if (error = FUN_00004740(auStack44, ct_id))
		{
			**(undefined4**)(iStack116 + -0x7ff4) = 0;
			error = CELL_SYSMODULE_ERROR_INVALID_MEMCONTAINER;
		}
		else
		{
			if (uStack40 < 0x60000)
			{
				**(undefined4**)(iStack116 + -0x7ff4) = 0;
				error = CELL_SYSMODULE_ERROR_INVALID_MEMCONTAINER;
			}
			else
			{
				**(uint32_t**)(iStack116 + -0x7ff0)   = ct_id;
				**(undefined4**)(iStack116 + -0x7ff4) = 1;
			}
		}
	}

	sys_lwmutex_unlock(*(sys_lwmutex_t**)(iStack116 + -0x7ff8));
	return error;
}

error_code cellSysmoduleLoadModule(u16 id)
{
	cellSysmodule.warning("cellSysmoduleLoadModule(id=0x%04X=%s)", id, get_module_id(id));

	const auto name = get_module_name(id);

	if (!name)
	{
		return CELL_SYSMODULE_ERROR_UNKNOWN;
	}

	//if (Module<>* m = Emu.GetModuleManager().GetModuleById(id))
	//{
	//	// CELL_SYSMODULE_ERROR_DUPLICATED shouldn't be returned
	//	m->Load();
	//}

	if (id == 0x58)
	{
		return CELL_SYSMODULE_ERROR_UNKNOWN;
	}

	if (id == CELL_SYSMODULE_ADEC_CELP ||
		id == CELL_SYSMODULE_CELPENC ||
		id == CELL_SYSMODULE_ADEC_MP3 ||
		id == CELL_SYSMODULE_PHOTO_EXPORT ||
		id == CELL_SYSMODULE_PRINT ||
		id == CELL_SYSMODULE_SYSUTIL_SEARCH ||
		id == CELL_SYSMODULE_IMEJP ||
		id == CELL_SYSMODULE_PHOTO_IMPORT ||
		id == CELL_SYSMODULE_MUSIC_EXPORT ||
		id == CELL_SYSMODULE_PHOTO_DECODE ||
		id == CELL_SYSMODULE_ADEC_M2BC ||
		id == CELL_SYSMODULE_SYSUTIL_AVCHAT2 ||
		id == CELL_SYSMODULE_ADEC_M4AAC ||
		id == CELL_SYSMODULE_GIFDEC ||
		id == CELL_SYSMODULE_SYSUTIL_NP_TROPHY ||
		id == CELL_SYSMODULE_SAIL_REC ||
		id == 0xf044 ||
		id == CELL_SYSMODULE_LIBATRAC3MULTI ||
		id == CELL_SYSMODULE_SYSUTIL_MUSIC)
	{
		return cellSysmoduleLoadModuleInternal(id);
	}

	if (id >= 0x5d)
	{
		return CELL_SYSMODULE_ERROR_UNKNOWN;
	}

	error_code error = CELL_OK;

	u64 uVar1 = ZEXT48(&TOC_BASE);
	undefined* puVar4 = &TOC_BASE;
	undefined* puVar3 = &TOC_BASE;
	undefined* puVar2 = &TOC_BASE;

	if (id == CELL_SYSMODULE_LV2DBG)
	{
		const u16 other_id = FUN_00000450(0);
		if (other_id == CELL_SYSMODULE_INVALID)
		{
			return CELL_SYSMODULE_ERROR_FATAL;
		}
		error = FUN_00000000((u32)other_id * 0x10 + *(s32*)(puVar4 + -0x7fcc), 0, 0);
		*(undefined4*)(*(s32*)(puVar4 + -0x7fc8) + (u32)id * 0x10) = *(undefined4*)(*(s32*)(puVar4 + -0x7fcc) + (u32)other_id * 0x10);
	}
	else if (id == CELL_SYSMODULE_GCM)
	{
		const u16 other_id = FUN_00000450(1);
		if (other_id == CELL_SYSMODULE_INVALID)
		{
			return CELL_SYSMODULE_ERROR_FATAL;
		}
		error = FUN_00000000((u32)other_id * 0x10 + *(s32*)(puVar4 + -0x7fc4), 0, 0);
		*(undefined4*)(*(s32*)(puVar4 + -0x7fc8) + (u32)id * 0x10) = *(undefined4*)(*(s32*)(puVar4 + -0x7fc4) + (u32)other_id * 0x10);
	}
	else if (id == CELL_SYSMODULE_FS)
	{
		const u16 other_id = FUN_00000450(2);
		if (other_id == CELL_SYSMODULE_INVALID)
		{
			return CELL_SYSMODULE_ERROR_FATAL;
		}
		error = FUN_00000000((u32)other_id * 0x10 + *(s32*)(puVar4 + -0x7fc0), 0, 0);
		*(undefined4*)(*(s32*)(puVar4 + -0x7fc8) + (u32)id * 0x10) = *(undefined4*)(*(s32*)(puVar4 + -0x7fc0) + (u32)other_id * 0x10);
	}
	else if (id == CELL_SYSMODULE_SAIL)
	{
		const s16 sVar6 = FUN_00000450(4);

		if (sVar6 == -1)
		{
			return CELL_SYSMODULE_ERROR_FATAL;
		}

		if (sVar6 == 1)
		{
			undefined local_30[16];
			error = FUN_00000000((u32)id * 0x10 + *(s32*)(puVar3 + -0x7fc8), 8, local_30);
		}
		else
		{
			error = FUN_00000000((u32)id * 0x10 + *(s32*)(puVar3 + -0x7fc8), 0, 0);
		}
	}
	else
	{
		if (u16* id_ptr = *(u16**)(PTR_s__00009ec8 + (u32)id * 0x10 + 0xc))
		{
			for (; puVar2 = (undefined*)uVar1, *id_ptr != CELL_SYSMODULE_INVALID; id_ptr++)
			{
				if (*id_ptr < 0xf000)
				{
					error = cellSysmoduleLoadModule(*id_ptr);
				}
				else
				{
					error = CELL_SYSMODULE_ERROR_UNKNOWN;
				}

				if (error != CELL_OK && error != CELL_SYSMODULE_ERROR_DUPLICATED)
				{
					return error;
				}
			}
		}

		return FUN_00000000((u32)id * 0x10 + *(s32*)(puVar2 + -0x7fc8), 0, 0);
	}

	return error;
}

error_code cellSysmoduleUnloadModuleInternal(u16 id); // Forward declaration

error_code cellSysmoduleUnloadModule(u16 id)
{
	cellSysmodule.warning("cellSysmoduleUnloadModule(id=0x%04X=%s)", id, get_module_id(id));

	const auto name = get_module_name(id);

	if (!name)
	{
		return CELL_SYSMODULE_ERROR_UNKNOWN;
	}

	//if (Module<>* m = Emu.GetModuleManager().GetModuleById(id))
	//{
	//	if (!m->IsLoaded())
	//	{
	//		cellSysmodule.error("cellSysmoduleUnloadModule() failed: module not loaded (id=0x%04x)", id);
	//		return CELL_SYSMODULE_ERROR_FATAL;
	//	}

	//	m->Unload();
	//}

	if (id == 0x58)
	{
		return CELL_SYSMODULE_ERROR_UNKNOWN;
	}

	if (id == CELL_SYSMODULE_CELPENC ||
		id == CELL_SYSMODULE_GIFDEC ||
		id == CELL_SYSMODULE_ADEC_CELP ||
		id == CELL_SYSMODULE_ADEC_M2BC ||
		id == CELL_SYSMODULE_ADEC_M4AAC ||
		id == CELL_SYSMODULE_ADEC_MP3 ||
		id == CELL_SYSMODULE_IMEJP ||
		id == CELL_SYSMODULE_SYSUTIL_MUSIC ||
		id == CELL_SYSMODULE_PHOTO_EXPORT ||
		id == CELL_SYSMODULE_PRINT ||
		id == CELL_SYSMODULE_PHOTO_IMPORT ||
		id == CELL_SYSMODULE_MUSIC_EXPORT ||
		id == CELL_SYSMODULE_PHOTO_DECODE ||
		id == CELL_SYSMODULE_SYSUTIL_SEARCH ||
		id == CELL_SYSMODULE_SYSUTIL_AVCHAT2 ||
		id == CELL_SYSMODULE_SAIL_REC ||
		id == CELL_SYSMODULE_SYSUTIL_NP_TROPHY ||
		id == 0xf044 ||
		id == CELL_SYSMODULE_LIBATRAC3MULTI)
	{
		return cellSysmoduleUnloadModuleInternal(id);
	}

	if (id >= 0x5d)
	{
		return CELL_SYSMODULE_ERROR_UNKNOWN;
	}

	u64 uVar2;
	u64 local_88;
	s32 sdk_ver = SYS_PROCESS_PARAM_VERSION_INVALID;
	process_get_sdk_version(process_getpid(), sdk_ver);
	const s32 max_iterations = (sdk_ver < SYS_PROCESS_PARAM_VERSION_330_0) ? 5 : 7;

	s32 iVar3;
	for (s32 i = 0; iVar3 = (s32)uVar2, i < max_iterations; i++)
	{
		if (error_code error = sys_lwmutex_lock(*(sys_lwmutex_t**)(iVar3 + -0x7ff8), 0))
		{
			return CELL_SYSMODULE_ERROR_FATAL;
		}

		iVar3 = (int)local_88;
		if ((*(u16*)(*(s32*)(iVar3 + -0x7fbc) + i * 2) == id) &&
			(*(s32*)(*(s32*)(iVar3 + -0x7fc8) + (u32)id * 0x10) == 1))
		{
			sys_lwmutex_unlock(*(sys_lwmutex_t**)(iVar3 + -0x7ff8));
			return CELL_OK;
		}
		sys_lwmutex_unlock(*(sys_lwmutex_t**)(iVar3 + -0x7ff8));
		uVar2 = local_88;
	}

	error_code error = CELL_OK;

	if (id == CELL_SYSMODULE_LV2DBG)
	{
		const u16 other_id = FUN_00000450(0);
		if (other_id == CELL_SYSMODULE_INVALID)
		{
			return CELL_SYSMODULE_ERROR_FATAL;
		}

		error = FUN_000002cc((u32)other_id * 0x10 + *(s32*)(iVar3 + -0x7fcc), 0);
		*(undefined4*)(*(int*)(iVar3 + -0x7fc8) + (u32)id * 0x10) = *(undefined4*)(*(s32*)(iVar3 + -0x7fcc) + (u32)other_id * 0x10);
	}
	else if (id == CELL_SYSMODULE_GCM)
	{
		const u16 other_id = FUN_00000450(1);
		if (other_id == CELL_SYSMODULE_INVALID)
		{
			return CELL_SYSMODULE_ERROR_FATAL;
		}

		error = FUN_000002cc((u32)other_id * 0x10 + *(s32*)(iVar3 + -0x7fc4), 0);
		*(undefined4*)(*(s32*)(iVar3 + -0x7fc8) + (u32)id * 0x10) = *(undefined4*)(*(s32*)(iVar3 + -0x7fc4) + (u32)other_id * 0x10);
	}
	else if (id == CELL_SYSMODULE_FS)
	{
		const u16 other_id = FUN_00000450(2);
		if (other_id == CELL_SYSMODULE_INVALID)
		{
			return CELL_SYSMODULE_ERROR_FATAL;
		}

		error = FUN_000002cc((u32)other_id * 0x10 + *(s32*)(iVar3 + -0x7fc0), 0);
		*(undefined4*)(*(int*)(iVar3 + -0x7fc8) + (u32)id * 0x10) = *(undefined4*)(*(s32*)(iVar3 + -0x7fc0) + (u32)other_id * 0x10);
	}
	else
	{
		error = FUN_000002cc((u32)id * 0x10 + *(s32*)(iVar3 + -0x7fc8), 0);
	}

	if (error != CELL_OK)
	{
		return error;
	}

	if (u16* id_begin = *(u16**)(*(s32*)(iVar3 + -0x7fc8) + (u32)id * 0x10 + 0xc))
	{
		u16* id_it = id_begin;

		for (; *id_it != CELL_SYSMODULE_INVALID; id_it++)
		{
		}

		while (id_it != id_begin)
		{
			id_it--;

			if (*id_it < 0xf000)
			{
				if (error = cellSysmoduleUnloadModule(*id_it))
				{
					return error;
				}
			}
			else
			{
				return CELL_SYSMODULE_ERROR_UNKNOWN;
			}
		}
	}

	return error;
}

error_code cellSysmoduleIsLoaded(u16 id)
{
	cellSysmodule.warning("cellSysmoduleIsLoaded(id=0x%04X=%s)", id, get_module_id(id));

	const auto name = get_module_name(id);

	if (!name)
	{
		return CELL_SYSMODULE_ERROR_UNKNOWN;
	}

	//if (Module<>* m = Emu.GetModuleManager().GetModuleById(id))
	//{
	//	if (!m->IsLoaded())
	//	{
	//		cellSysmodule.warning("cellSysmoduleIsLoaded(): module not loaded (id=0x%04x)", id);
	//		return CELL_SYSMODULE_ERROR_UNLOADED;
	//	}
	//}

	if (id == 0x58)
	{
		return CELL_SYSMODULE_ERROR_UNKNOWN;
	}

	if (id == CELL_SYSMODULE_ADEC_CELP ||
		id == CELL_SYSMODULE_CELPENC ||
		id == CELL_SYSMODULE_ADEC_MP3 ||
		id == CELL_SYSMODULE_PHOTO_EXPORT ||
		id == CELL_SYSMODULE_PRINT ||
		id == CELL_SYSMODULE_SYSUTIL_SEARCH ||
		id == CELL_SYSMODULE_IMEJP ||
		id == CELL_SYSMODULE_PHOTO_IMPORT ||
		id == CELL_SYSMODULE_MUSIC_EXPORT ||
		id == CELL_SYSMODULE_PHOTO_DECODE ||
		id == CELL_SYSMODULE_ADEC_M2BC ||
		id == CELL_SYSMODULE_SYSUTIL_AVCHAT2 ||
		id == CELL_SYSMODULE_ADEC_M4AAC ||
		id == CELL_SYSMODULE_GIFDEC ||
		id == CELL_SYSMODULE_SYSUTIL_NP_TROPHY ||
		id == CELL_SYSMODULE_SAIL_REC ||
		id == 0xf044 ||
		id == CELL_SYSMODULE_LIBATRAC3MULTI ||
		id == CELL_SYSMODULE_SYSUTIL_MUSIC)
	{
		return cellSysmoduleIsLoadedEx(id);
	}

	if (id >= 0x5d)
	{
		return CELL_SYSMODULE_ERROR_UNKNOWN;
	}

	if (error_code error = sys_lwmutex_lock((sys_lwmutex_t*)PTR_DAT_00009e98, 0))
	{
		return CELL_SYSMODULE_ERROR_FATAL;
	}

	s32 iStack116;
	u16 other_id = 0;

	if (*(s32*)(*(s32*)(iStack116 + -0x7fc8) + (u32)id * 0x10 + 8) == 0)
	{
		for (s32 i = 0; id_array[i] != CELL_SYSMODULE_INVALID; i++)
		{
			other_id = id_array[i];
		}
	}
	else
	{
		other_id = id;
	}

	error_code error = CELL_SYSMODULE_LOADED;

	if (*(s32*)(*(s32*)(iStack116 + -0x7fc8) + (u32)other_id * 0x10) == 0)
	{
		error = CELL_SYSMODULE_ERROR_UNLOADED;
	}

	sys_lwmutex_unlock(*(sys_lwmutex_t**)(iStack116 + -0x7ff8));
	return error;
}

error_code cellSysmoduleGetImagesize(u16 id, vm::ptr<u32> size)
{
	cellSysmodule.todo("cellSysmoduleGetImagesize(id=0x%04X=%s, size=*0x%x)", id, get_module_id(id), size);

	*size = 0;

	if (id < 0xff00)
	{
		return CELL_SYSMODULE_ERROR_FATAL;
	}

	const u16 other_id = id + 0x100;
	if (other_id >= 2)
	{
		return CELL_SYSMODULE_ERROR_FATAL;
	}

	u32 uStack92;
	if (error_code error = FUN_00003e00("PTR_s_/dev_flash/sys/external/flashATR_00009ef0" + (u32)other_id * 0x80, 0, &uStack92, 0, 0, 0))
	{
		return CELL_SYSMODULE_ERROR_FATAL;
	}

	undefined auStack88[40];
	if (error_code error = FUN_00003f00(uStack92, auStack88))
	{
		FUN_00003e60(uStack92);
		return CELL_SYSMODULE_ERROR_FATAL;
	}

	u64 uStack48;
	*size = (u32)((u64)uStack48 >> 0x20);
	FUN_00003e60(uStack92);
	return CELL_OK;
}

error_code cellSysmoduleFetchImage(u16 id, u32 param_2, vm::ptr<u32> param_3)
{
	cellSysmodule.todo("cellSysmoduleFetchImage(id=0x%04X=%s, param_2=0x%x, param_3=*0x%x)", id, get_module_id(id), param_2, param_3);

	if (id < 0xff00)
	{
		return CELL_SYSMODULE_ERROR_FATAL;
	}

	const u16 other_id = id + 0x100;
	if (other_id >= 2)
	{
		*param_3 = 0;
		return CELL_SYSMODULE_ERROR_FATAL;
	}

	undefined4 auStack120[2];
	if (error_code error = FUN_00003e00("PTR_s_/dev_flash/sys/external/flashATR_00009ef0" + (u32)other_id * 0x80, 0, auStack120, 0, 0, 0))
	{
		*param_3 = 0;
		return CELL_SYSMODULE_ERROR_FATAL;
	}

	undefined auStack104[32];
	if (error_code error = FUN_00003f00(auStack120[0], auStack104))
	{
		FUN_00003e60(auStack120[0]);
		*param_3 = 0;
		EStack32         = CELL_SYSMODULE_ERROR_FATAL;
	}

	u64 uStack64;
	s64 lStack72;
	u64 uVar1 = uStack64 >> 0x20 | lStack72 << 0x20;
	u64 uStack48 = (u64)*param_3;
	if (uVar1 < *param_3)
	{
		uStack48 = uVar1;
	}

	undefined auStack112[4];
	if (error_code error = FUN_00003e20(auStack120[0], param_2, (s32)uStack48, auStack112))
	{
		*param_3 = 0;
	}
	else
	{
		uint uStack108;
		*param_3 = uStack108;
	}

	FUN_00003e60(auStack120[0]);
	return CELL_OK;
}

error_code cellSysmoduleUnloadModuleInternal(u16 id)
{
	cellSysmodule.todo("cellSysmoduleUnloadModuleInternal(id=0x%04X=%s)", id, get_module_id(id));

	if ((id < 0xf000) || (id == CELL_SYSMODULE_INVALID))
	{
		return CELL_SYSMODULE_ERROR_UNKNOWN;
	}

	u16 other_id = id + 0x1000;
	if (other_id >= 0x57)
	{
		return CELL_SYSMODULE_ERROR_UNKNOWN;
	}

	if (error_code error = FUN_000002cc(PTR_DAT_00009ee0 + (u32)other_id * 0x10, 0))
	{
		return error;
	}

	void* puVar2 = &TOC_BASE;
	if (u16* id_begin = *(u16**)(*(s32*)(puVar2 + -0x7fb0) + (u32)other_id * 0x10 + 0xc))
	{
		u16* id_it = id_begin;

		for (; *id_it != CELL_SYSMODULE_INVALID; id_it++)
		{
		}

		while (id_it != id_begin)
		{
			id_it--;

			if (*id_it < 0xf000)
			{
				if (error_code error = cellSysmoduleUnloadModule(*id_it))
				{
					return error;
				}
			}
			else
			{
				if (error_code error = cellSysmoduleUnloadModuleInternal(*id_it))
				{
					return error;
				}
			}
		}
	}

	return CELL_OK;
}

error_code cellSysmoduleLoadModuleInternal(u16 id)
{
	cellSysmodule.todo("cellSysmoduleLoadModuleInternal(id=0x%04X=%s)", id, get_module_id(id));

	u64 uVar1;
	u64 local_b8;

	if ((id < 0xf000) || (id == CELL_SYSMODULE_INVALID))
	{
		return CELL_SYSMODULE_ERROR_UNKNOWN;
	}

	if (((id == 0xf024) || (id == 0xf03e)) || (uVar1 = ZEXT48(&TOC_BASE), id == 0xf020))
	{
		char sfo[40] {};

		if (error_code error = _sys_process_get_paramsfo(sfo))
		{
			return CELL_SYSMODULE_ERROR_UNKNOWN;
		}

		uVar1 = local_b8;
		if (((sfo[24] & 1) == 0) && ((sfo[32] & 1) == 0))
		{
			return CELL_SYSMODULE_ERROR_UNKNOWN;
		}
	}

	s32 iVar2 = (s32)uVar1;
	u16 other_id = id + 0x1000;

	if (other_id >= 0x57)
	{
		return CELL_SYSMODULE_ERROR_UNKNOWN;
	}

	if (u16* id_ptr = *(u16**)(*(s32*)(iVar2 + -0x7fb0) + (u32)other_id * 0x10 + 0xc))
	{
		for (; iVar2 = (s32)uVar1, *id_ptr != CELL_SYSMODULE_INVALID; id_ptr++)
		{
			error_code error = CELL_OK;

			if (*id_ptr < 0xf000)
			{
				error = cellSysmoduleLoadModule(*id_ptr);
			}
			else
			{
				error = cellSysmoduleLoadModuleInternal(*id_ptr);
			}

			if (error != CELL_OK && error != CELL_SYSMODULE_ERROR_DUPLICATED)
			{
				return error;
			}
		}
	}

	return FUN_00000000((u32)other_id * 0x10 + *(s32*)(iVar2 + -0x7fb0), 0, 0);
}

error_code cellSysmoduleUnloadModuleEx(u16 id)
{
	cellSysmodule.todo("cellSysmoduleUnloadModuleEx(id=0x%04X=%s)", id, get_module_id(id));

	u16 id_to_unload = CELL_SYSMODULE_INVALID;

	for (s32 i = 0; i < 0x33; i++)
	{
		const u16 other_id = id_array[i];
		if (other_id == id)
		{
			id_to_unload = other_id;
			break;
		}
	}

	return cellSysmoduleUnloadModuleInternal(id_to_unload);
}

error_code cellSysmoduleLoadModuleEx(u16 id)
{
	cellSysmodule.todo("cellSysmoduleLoadModuleEx(id=0x%04X=%s)", id, get_module_id(id));

	u16 id_to_load = CELL_SYSMODULE_INVALID;

	for (s32 i = 0; i < 0x33; i++)
	{
		const u16 other_id = id_array[i];
		if (other_id == id)
		{
			id_to_load = other_id;
			break;
		}
	}

	return cellSysmoduleLoadModuleInternal(id_to_load);
}

error_code cellSysmoduleIsLoadedEx(u16 id)
{
	cellSysmodule.todo("cellSysmoduleIsLoadedEx(id=0x%04X=%s)", id, get_module_id(id));

	if ((id < 0xf000) || (id == 0xffff))
	{
		return CELL_SYSMODULE_ERROR_UNKNOWN;
	}

	const u16 other_id = id + 0x1000;
	if (other_id >= 0x57)
	{
		return CELL_SYSMODULE_ERROR_UNKNOWN;
	}

	if (error_code error = sys_lwmutex_lock((sys_lwmutex_t*)PTR_DAT_00009e98, 0))
	{
		return CELL_SYSMODULE_ERROR_FATAL;
	}

	error_code error = CELL_OK;

	s32 iStack116;
	if (*(s32*)(*(s32*)(iStack116 + -0x7fb0) + (uint)other_id * 0x10) == 0)
	{
		error = CELL_SYSMODULE_ERROR_UNLOADED;
	}

	sys_lwmutex_unlock(*(sys_lwmutex_t**)(iStack116 + -0x7ff8));
	return error;
}

DECLARE(ppu_module_manager::cellSysmodule)("cellSysmodule", []()
{
	REG_FUNC(cellSysmodule, cellSysmoduleInitialize);
	REG_FUNC(cellSysmodule, cellSysmoduleFinalize);
	REG_FUNC(cellSysmodule, cellSysmoduleSetMemcontainer);
	REG_FUNC(cellSysmodule, cellSysmoduleLoadModule);
	REG_FUNC(cellSysmodule, cellSysmoduleUnloadModule);
	REG_FUNC(cellSysmodule, cellSysmoduleIsLoaded);
	REG_FUNC(cellSysmodule, cellSysmoduleGetImagesize);
	REG_FUNC(cellSysmodule, cellSysmoduleFetchImage);
	REG_FUNC(cellSysmodule, cellSysmoduleUnloadModuleInternal);
	REG_FUNC(cellSysmodule, cellSysmoduleLoadModuleInternal);
	REG_FUNC(cellSysmodule, cellSysmoduleUnloadModuleEx);
	REG_FUNC(cellSysmodule, cellSysmoduleLoadModuleEx);
	REG_FUNC(cellSysmodule, cellSysmoduleIsLoadedEx);
});
