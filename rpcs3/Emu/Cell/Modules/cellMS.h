#pragma once

//#include "cellSpurs.h"

#define CELL_MS_VERSION 2.70f,

enum
{
	CELL_MS_MAX_STREAMS = 512,
	CELL_MS_MAX_CHANNEL = 7,

	CELL_MS_DSP_PAGESIZE = 12288,

	CELL_MS_EQ_LINE  = 0,
	CELL_MS_EQ_CURVE = 1,
	CELL_MS_EQ_CUBIC = 2,

	CELL_MS_EQ_MAX_BANDS = 512,

	CELL_MS_AUDIOMODESELECT_SUPPORTSLPCM  = 1,
	CELL_MS_AUDIOMODESELECT_SUPPORTSDOLBY = 2,
	CELL_MS_AUDIOMODESELECT_SUPPORTSDTS   = 4,
	CELL_MS_AUDIOMODESELECT_PREFERDOLBY   = 8,
	CELL_MS_AUDIOMODESELECT_PREFERDTS     = 16,

	CELL_MS_CALLBACK_MOREDATA       = 1,
	CELL_MS_CALLBACK_CLOSESTREAM    = 2,
	CELL_MS_CALLBACK_FINISHEDDRY    = 3,
	CELL_MS_CALLBACK_FINISHSTREAM   = 4,
	CELL_MS_CALLBACK_MP3DECODEERROR = 5,

	CELL_MS_BUS_FLAG = 0x80000,

	CELL_MS_MASTER_BUS = CELL_MS_BUS_FLAG | 0,
	CELL_MS_SUBBUS_1   = CELL_MS_BUS_FLAG | 1,
	CELL_MS_SUBBUS_2   = CELL_MS_BUS_FLAG | 2,
	CELL_MS_SUBBUS_3   = CELL_MS_BUS_FLAG | 3,
	CELL_MS_SUBBUS_4   = CELL_MS_BUS_FLAG | 4,
	CELL_MS_SUBBUS_5   = CELL_MS_BUS_FLAG | 5,
	CELL_MS_SUBBUS_6   = CELL_MS_BUS_FLAG | 6,
	CELL_MS_SUBBUS_7   = CELL_MS_BUS_FLAG | 7,
	CELL_MS_SUBBUS_8   = CELL_MS_BUS_FLAG | 8,
	CELL_MS_SUBBUS_9   = CELL_MS_BUS_FLAG | 9,
	CELL_MS_SUBBUS_10  = CELL_MS_BUS_FLAG | 10,
	CELL_MS_SUBBUS_11  = CELL_MS_BUS_FLAG | 11,
	CELL_MS_SUBBUS_12  = CELL_MS_BUS_FLAG | 12,
	CELL_MS_SUBBUS_13  = CELL_MS_BUS_FLAG | 13,
	CELL_MS_SUBBUS_14  = CELL_MS_BUS_FLAG | 14,
	CELL_MS_SUBBUS_15  = CELL_MS_BUS_FLAG | 15,
	CELL_MS_SUBBUS_16  = CELL_MS_BUS_FLAG | 16,
	CELL_MS_SUBBUS_17  = CELL_MS_BUS_FLAG | 17,
	CELL_MS_SUBBUS_18  = CELL_MS_BUS_FLAG | 18,
	CELL_MS_SUBBUS_19  = CELL_MS_BUS_FLAG | 19,
	CELL_MS_SUBBUS_20  = CELL_MS_BUS_FLAG | 20,
	CELL_MS_SUBBUS_21  = CELL_MS_BUS_FLAG | 21,
	CELL_MS_SUBBUS_22  = CELL_MS_BUS_FLAG | 22,
	CELL_MS_SUBBUS_23  = CELL_MS_BUS_FLAG | 23,
	CELL_MS_SUBBUS_24  = CELL_MS_BUS_FLAG | 24,
	CELL_MS_SUBBUS_25  = CELL_MS_BUS_FLAG | 25,
	CELL_MS_SUBBUS_26  = CELL_MS_BUS_FLAG | 26,
	CELL_MS_SUBBUS_27  = CELL_MS_BUS_FLAG | 27,
	CELL_MS_SUBBUS_28  = CELL_MS_BUS_FLAG | 28,
	CELL_MS_SUBBUS_29  = CELL_MS_BUS_FLAG | 29,
	CELL_MS_SUBBUS_30  = CELL_MS_BUS_FLAG | 30,
	CELL_MS_SUBBUS_31  = CELL_MS_BUS_FLAG | 31,

	CELL_MS_STREAM_NOFLAGS   = 0,
	CELL_MS_STREAM_AUTOCLOSE = 1,

	CELL_MS_NOFLAGS                    = 0,
	CELL_MS_DEBUG_FLAG                 = 1,
	CELL_MS_ROUTABLE_STREAMS_FLAG      = 2,
	CELL_MS_EXTERNAL_BUFFERS           = 4,
	CELL_MS_DISABLE_SPU_PRINTF_SERVER  = 8,
	CELL_MS_TD_ONLY_128                = 16,
	CELL_MS_TD_ONLY_256                = 32,
	CELL_MS_TD_ONLY_512                = 64,
	CELL_MS_SEPARATE_VOLUMES_PER_ROUTE = 128,
	CELL_MS_FORCE_OPENAL_PPU           = 256,
	CELL_MS_USE_SMOOTH_ENVELOPE        = 4096,
	
	CELL_MS_ALLOC_DSPHANDLE = -1,
	CELL_MS_ALLOC_PAGE      = -1,

	CELL_MS_DSPOFF      = -1,
	CELL_MS_DSPALLSLOTS = -1,

	CELL_MS_16BIT_BIG    = 0,
	CELL_MS_16BIT_LITTLE = 1,
	CELL_MS_32BIT_FLOAT  = 2,
	CELL_MS_ADPCM        = 3,
	CELL_MS_ATRAC_LOW    = 4,
	CELL_MS_ATRAC_MEDIUM = 5,
	CELL_MS_ATRAC_HIGH   = 6,
	CELL_MS_MP3          = 7,

	CELL_MS_STOP = 0,

	CELL_MS_KEYOFF = -1,

	CELL_MS_ENVELOPE_LIN = 0,
	CELL_MS_ENVELOPE_EXP = 1,
	CELL_MS_ENVELOPE_NO_LOOP = 255,
	CELL_MS_ENVELOPE_MAX_POINTS = 33,

	CELL_MSFX_COMPRESSOR_RMS_MODE  = 0,
	CELL_MSFX_COMPRESSOR_PEAK_MODE = 1,

	CELL_MSFX_COMPRESSOR_CHAN_VOLUMES_AVERAGE = 0,
	CELL_MSFX_COMPRESSOR_CHAN_VOLUMES_MAX     = 2,

	CELL_MSFX_VOCODER_BYPASS_OFF = 0,
	CELL_MSFX_VOCODER_BYPASS_ON  = 1,
	CELL_MSFX_VOCODER_BANDS      = 64,

	CELL_MSFX_IR_BUFFER_SIZE = 4096,

	CELL_MS_PRIORITY_OFF           = -1,
	CELL_MS_PRIORITY_UNLIMITED     = CELL_MS_MAX_STREAMS,
	CELL_MS_PRIORITY_MAX_THRESHOLD = 10000000,
};

enum
{
	CELL_MSMP3_DISABLE_SPU_PRINTF_SERVER = 8,
	CELL_MSMP3_TEMP_INIT_BUFFER_SIZE     = 1024 * 96
};

enum
{
	CELL_MS_STREAM_OFF           = 0,
	CELL_MS_STREAM_ON            = 1,
	CELL_MS_STREAM_PRIORITY_MUTE = 2,
	CELL_MS_STREAM_PAUSE         = 4,
	CELL_MS_STREAM_PAUSE_SUB     = 8,
	CELL_MS_STREAM_PENDING       = 16,
	CELL_MS_STREAM_CLOSED        = 32
};

enum
{
	CELL_MS_SULPHA_NOT_ENABLED      = 0,
	CELL_MS_SULPHA_ENABLED_INACTIVE = 1,
	CELL_MS_SULPHA_FILE_CONNECTED   = 2,
	CELL_MS_SULPHA_DECI3_CONNECTED  = 3
};

enum
{
	CELL_MS_SULPHA_NAMED_TYPE_DSP    = 0,
	CELL_MS_SULPHA_NAMED_TYPE_STREAM = 1,
	CELL_MS_SULPHA_NAMED_TYPE_SUB    = 2
};

enum
{
	CELL_MS_SULPHA_WIREFRAME_BOX    = 0,
	CELL_MS_SULPHA_BOX              = 1,
	CELL_MS_SULPHA_WIREFRAME_SPHERE = 2,
	CELL_MS_SULPHA_SPHERE           = 3
};

enum CELL_MS_BYPASS
{
	CELL_MS_NOTBYPASSED = 0,
	CELL_MS_BYPASSED    = 1
};

enum CELL_MS_CHANNELS
{
	CELL_MS_WET_STREAM = 0,
	CELL_MS_CHANNEL_0  = 0,
	CELL_MS_CHANNEL_1  = 1,
	CELL_MS_CHANNEL_2  = 2,
	CELL_MS_CHANNEL_3  = 3,
	CELL_MS_CHANNEL_4  = 4,
	CELL_MS_CHANNEL_5  = 5,
	CELL_MS_CHANNEL_6  = 6,
	CELL_MS_CHANNEL_7  = 7,
};

enum CELL_MS_DOMAINS
{
	CELL_MS_TIMEDOMAIN = 0,
	CELL_MS_FREQDOMAIN = 1
};

enum CELL_MS_DSPMASKS
{
	CELL_MS_INMASK  = 0,
	CELL_MS_OUTMASK = 1
};

enum CELL_MS_DSPSLOTS
{
	CELL_MS_DSP_SLOT_0 = 0,
	CELL_MS_DSP_SLOT_1 = 1,
	CELL_MS_DSP_SLOT_2 = 2,
	CELL_MS_DSP_SLOT_3 = 3,
	CELL_MS_DSP_SLOT_4 = 4,
	CELL_MS_DSP_SLOT_5 = 5,
	CELL_MS_DSP_SLOT_6 = 6,
	CELL_MS_DSP_SLOT_7 = 7
};

enum CELL_MS_ENVSTATE
{
	CELL_MS_ENVELOPE_OFF = 0,
	CELL_MS_ENVELOPE_ON  = 1
};

enum CELL_MS_EQSTATE
{
	CELL_MS_EQDISABLED = 0,
	CELL_MS_EQENABLED  = 1
};

enum CELL_MS_PATH
{
	CELL_MS_DRY         = 0,
	CELL_MS_WET         = 1,
	CELL_MS_WET_AND_DRY = 2
};

enum CELL_MS_PAUSE
{
	CELL_MS_NOTPAUSED = 0,
	CELL_MS_PAUSED    = 1
};

enum CELL_MS_PRIORITY_BANKS
{
	CELL_MS_PRIORITY_BANK_0  = 0,
	CELL_MS_PRIORITY_BANK_1  = 1,
	CELL_MS_PRIORITY_BANK_2  = 2,
	CELL_MS_PRIORITY_BANK_3  = 3,
	CELL_MS_PRIORITY_BANK_4  = 4,
	CELL_MS_PRIORITY_BANK_5  = 5,
	CELL_MS_PRIORITY_BANK_6  = 6,
	CELL_MS_PRIORITY_BANK_7  = 7,
	CELL_MS_PRIORITY_BANK_8  = 8,
	CELL_MS_PRIORITY_BANK_9  = 9,
	CELL_MS_PRIORITY_BANK_10 = 10,
	CELL_MS_PRIORITY_BANK_11 = 11,
	CELL_MS_PRIORITY_BANK_12 = 12,
	CELL_MS_PRIORITY_BANK_13 = 13,
	CELL_MS_PRIORITY_BANK_14 = 14,
	CELL_MS_PRIORITY_BANK_15 = 15,
	CELL_MS_PRIORITY_BANK_16 = 16,
	CELL_MS_PRIORITY_BANK_17 = 17,
	CELL_MS_PRIORITY_BANK_18 = 18,
	CELL_MS_PRIORITY_BANK_19 = 19,
	CELL_MS_PRIORITY_BANK_20 = 20,
	CELL_MS_PRIORITY_BANK_21 = 21,
	CELL_MS_PRIORITY_BANK_22 = 22,
	CELL_MS_PRIORITY_BANK_23 = 23,
	CELL_MS_PRIORITY_BANK_24 = 24,
	CELL_MS_PRIORITY_BANK_25 = 25,
	CELL_MS_PRIORITY_BANK_26 = 26,
	CELL_MS_PRIORITY_BANK_27 = 27,
	CELL_MS_PRIORITY_BANK_28 = 28,
	CELL_MS_PRIORITY_BANK_29 = 29,
	CELL_MS_PRIORITY_BANK_30 = 30,
	CELL_MS_PRIORITY_BANK_31 = 31
};

enum CELL_MS_ROUTES
{
	CELL_MS_ROUTE_OFF         = 0,
	CELL_MS_ROUTE_ON          = 1,
	CELL_MS_ROUTE_ALL_OFF     = 2,
	CELL_MS_ROUTE_VOLUME_ONLY = 3
};

enum CELL_MS_SPEAKERS
{
	CELL_MS_SPEAKER_FL  = 0,
	CELL_MS_SPEAKER_FR  = 1,
	CELL_MS_SPEAKER_FC  = 2,
	CELL_MS_SPEAKER_LFE = 3,
	CELL_MS_SPEAKER_RL  = 4,
	CELL_MS_SPEAKER_RR  = 5,
	CELL_MS_SPEAKER_EL  = 6,
	CELL_MS_SPEAKER_ER  = 7
};

enum CELL_MSFX_FILTERMODE
{
	CELL_MSFX_FILTERMODE_OFF                         = 0,
	CELL_MSFX_FILTERMODE_LOWPASS_RESONANT            = 1,
	CELL_MSFX_FILTERMODE_HIGHPASS_RESONANT           = 2,
	CELL_MSFX_FILTERMODE_BANDPASS_PEAK               = 3,
	CELL_MSFX_FILTERMODE_BANDPASS_ZERO               = 4,
	CELL_MSFX_FILTERMODE_NOTCH                       = 5,
	CELL_MSFX_FILTERMODE_PEAK                        = 6,
	CELL_MSFX_FILTERMODE_HIGHSHELF                   = 7,
	CELL_MSFX_FILTERMODE_LOWSHELF                    = 8,
	CELL_MSFX_FILTERMODE_LOWPASS_ONEPOLE             = 9,
	CELL_MSFX_FILTERMODE_HIGHPASS_ONEPOLE            = 10,
	CELL_MSFX_FILTERMODE_ALLPASS                     = 11,
	CELL_MSFX_FILTERMODE_LOWPASS_RESONANT_NORMALIZED = 12,
	CELL_MSFX_FILTERMODE_LFE_6_POLE                  = 13,
	CELL_MSFX_FILTERMODE_LFE_8_POLE                  = 14
};

enum CELL_MSFX_I3DL2_LATE_MODEL
{
	CELL_MSFX_ROOM1_LATE_LEFT  = 0,
	CELL_MSFX_ROOM1_LATE_RIGHT = 1
};

enum CELL_MSFX_I3DL2_REFLECTIONS
{
	CELL_MSFX_ROOM1_LEFT  = 0,
	CELL_MSFX_ROOM1_RIGHT = 1,
	CELL_MSFX_ROOM2_LEFT  = 2,
	CELL_MSFX_ROOM2_RIGHT = 3,
	CELL_MSFX_ROOM3_LEFT  = 4,
	CELL_MSFX_ROOM3_RIGHT = 5
};

enum CELL_MSFX_SLOTMASK_MODE
{
	CELL_MSFX_SLOTMASK_MODE_MONO   = 1,
	CELL_MSFX_SLOTMASK_MODE_STEREO = 2,
	CELL_MSFX_SLOTMASK_MODE_2_1    = 3,
	CELL_MSFX_SLOTMASK_MODE_4_0    = 4,
	CELL_MSFX_SLOTMASK_MODE_5_0    = 5,
	CELL_MSFX_SLOTMASK_MODE_5_1    = 6,
	CELL_MSFX_SLOTMASK_MODE_6_0    = 7,
	CELL_MSFX_SLOTMASK_MODE_7_0    = 8,
	CELL_MSFX_SLOTMASK_MODE_7_1    = 9
};

enum CELL_MSMP3_DECODER_RESULT
{
	CELL_MSMP3_RESULT_OK                     = 0,
	CELL_MSMP3_RESULT_ERR_TOO_FEW_CONTEXTS   = 1,
	CELL_MSMP3_RESULT_ERR_INTERNAL_DECODING  = 2,
	CELL_MSMP3_RESULT_ERR_UNHANDLED_CHANNELS = 3,
	CELL_MSMP3_RESULT_ERR_CHANNEL_MISMATCH   = 4,
	CELL_MSMP3_RESULT_ERR_INVALID_DATA       = 5
};

using CELL_MS_CALLBACK_FUNC = void(s32 streamNumber, vm::ptr<void> userData, s32 callbackType, vm::ptr<void> readBuffer, s32 readSize);

struct CellMSSystemInformation;

struct CellEQMorphSection
{
	f32 startVals[512];
	f32 endVals[512];
};

struct CellEQPoint
{
	u8 Tag[8];
	f32 x;
	f32 y;
};

struct CellEQSection
{
	u8 curveSwitch;
	u8 nPoints;
	vm::ptr<CellEQPoint> xy;
};

struct CellMSComplexNum
{
	f32 r;
	f32 i;
};

struct CellMSDebugInformation
{
	vm::ptr<CellMSSystemInformation> systemInfo;
	vm::ptr<s32> subStreamCnt;
	vm::ptr<s32> streamCycleCount;
	vm::ptr<s32> streamDSPCycleCount;
	vm::ptr<s32> subCycleCount;
	vm::ptr<s32> subDSPCycleCount;
};

struct CellMSDSP
{
	s32 memoryUsed;
	s32 memoryAvail;
	s32 handle;
	s32 page;
};

struct CellMSEnvPoint
{
	u32 X;
	f32 Y;
};

struct CellMSEnvelope
{
	CellMSEnvPoint Point[CELL_MS_ENVELOPE_MAX_POINTS];
	u32 ReleaseRate;
	u32 NumPoints;
	u32 LoopStart;
	u32 Type;
};

struct CellMSFXAuxSend
{
	s32 nSendChannels;
	s32 nSendsPerChannel;
};

struct CellMSFXCompressorInfo
{
	f32 AnalysisPower;
	s32 firstBand;
	s32 lastBand;
	f32 ratio;
	f32 threshold;
	f32 attack;
	f32 release;
	f32 makeupGain;
	f32 CompressorCur;
	f32 multBy;
	f32 softKnee;
	s32 flags;
};

struct CellMSFXFilter
{
	CELL_MSFX_FILTERMODE FilterMode;
	f32 fFrequency;
	f32 fResonance;
	f32 fGain;
};

struct CellMSFXDelay
{
	CellMSFXFilter InFilters[2];
	CellMSFXFilter TapFilters[4];
	f32 TapDelayMs[4];
	f32 TapVolume[4];
	f32 TapFeedback[4];
	f32 DryVolume;
	f32 InputVolume;
};

struct CellMSFXDistortion
{
	f32 fA;
	f32 fB;
	f32 fLimit;
	f32 fGate;
	f32 fWetGain;
	f32 fDryGain;
};

struct CellMSFXEarlyReflection
{
	f32 fVol;
	s32 nOffsetSamps;
};

struct CellMSFXI3DL2Params
{
	f32 Room;
	f32 Room_HF;
	f32 Decay_time;
	f32 Decay_HF_ratio;
	f32 Reflections;
	f32 Reflections_delay;
	f32 Reverb;
	f32 Reverb_delay;
	f32 Diffusion;
	f32 Density;
	f32 HF_reference;
	s32 MixChannel;
	f32 MixAmount;
	CELL_MSFX_I3DL2_REFLECTIONS EarlyReflectionPattern;
	CELL_MSFX_I3DL2_LATE_MODEL LateReverbPattern;
	f32 EarlyReflectionScaler;
	f32 LF_reference;
	f32 Room_LF;
};

struct CellMSFXMeter
{
	f32 fThresholdLinear;
	f32 fPeakLinearLastFrame;
	f32 fRMSLinearLastFrame;
	s32 nSampsOverThresholdFrame;
	f32 fAverageRMS;
	f32 fMaxPeakLinear;
	f32 fMaxRMSLinear;
	s32 nMaxSampsOverThreshold;
};

struct CellMSFXParaEQ
{
	CellMSFXFilter Filters[4];
};

struct CellMSFXPitchShift
{
	f32 fCents;
};

struct CellMSFXReverbParams
{
	f32 fWetVolume;
	f32 fDryVolume;
	f32 fPreDelayMs;
	f32 fLowPassHz;
	f32 fHighPassHz;
	f32 fRoomSize;
	f32 fDamping;
	f32 fDiffusion;
	f32 fEarlyLate;
	f32 fLeftRight;
	f32 fFrontBack;
};

struct CellMSFXRingMod
{
	f32 fFrequency;
	f32 fAmplitude;
};

struct CellMSFXSoftClipper
{
	f32 fDrive;
	f32 fGain;
};

struct CellMSFXVocoderInfo
{
	f32 FFTBuffer[512 * 2];
	f32 Band[CELL_MSFX_VOCODER_BANDS];
	u64 Bypass;
	f32 ModulatorMix;
};

struct CellMSInfo
{
	s32 SubBusGroup;
	vm::ptr<void> FirstBuffer;
	u32 FirstBufferSize;
	vm::ptr<void> SecondBuffer;
	u32 SecondBufferSize;
	s32 Pitch;
	s32 numChannels;
	s32 initialOffset;
	s32 inputType;
	s32 flags;
};

struct CellMSMP3Context
{
	f32 decodeBuff[544];
	f32 oldBlock[576];
	char oldFrame[512];
};

struct CellMSMP3DecoderInitParams
{
	//vm::ptr<CellSpurs> pSpurs;
	vm::ptr<u8> pPriorities;
	s32 flags;
	s32 nMaxStreams;
};

struct CellMSMP3DecoderPacketInfo
{
	u32 Sync;
	u32 ID;
	u32 Layer;
	u32 ProtBit;
	u32 BitRate;
	u32 Frequency;
	u32 PadBit;
	u32 PrivBit;
	u32 Mode;
	u32 ModeExt;
	u32 Copy;
	u32 Home;
	u32 Emphasis;
	u32 OutputSizeBytes;
	u32 PacketSizeBytes;
	u32 ID3;
	u32 Tag;
};

struct CellMSMP3DecoderStreamHead
{
	s32 nChannels;
	vm::ptr<void> pMp3DataBuffer;
	s32 nMp3BufferSizeBytes;
	s32 nMp3BufferBytesRead;

	vm::ptr<s16> pPcmBuffer;
	s32 nPcmBufferSizeBytes;
	s32 nPcmBufferBytesWritten;
	s32 internalCount;

	vm::ptr<s16> pPcmBuffer2;
	CELL_MSMP3_DECODER_RESULT eReturnCode;
	s32 nBlocksDecoded;
	s32 nSkipFirstPacket;
};

struct CellMSMP3DecoderStreamInstance
{
	CellMSMP3DecoderStreamHead head;
	vm::ptr<CellMSMP3Context[2]> pContexts;
	s32 pad[2];
};

struct CellMSMP3FrameHeader
{
	u32 Sync;
	u32 ID;
	u32 Layer;
	u32 ProtBit;
	u32 BitRate;
	u32 Frequency;
	u32 PadBit;
	u32 PrivBit;
	u32 Mode;
	u32 ModeExt;
	u32 Copy;
	u32 Home;
	u32 Emphesis;
	f32 PacketTime;
	u32 PacketSize;
	u32 ID3;
	u32 Tag;
};

struct CellMSMSFHeader
{
	char header[3];
	char version;
	s32 compressionType;
	s32 channels;
	s32 sampleSize;
	s32 sampleRate;
	s32 miscInfo;
	s32 LoopMarkers[10];
};

struct CellMSSection1
{
	u8 Header[4];
	u32 DataSize;
	u8 ID[8];
	u16 IDValue;
	u8 Version[4];
	u8 Date[6];
	u32 InfoSize;
	u8 SVersion[4];
	u32 InitData[8];
};

struct CellMSSulpha3dObjectDimensions
{
	f32 width;
	f32 height;
	f32 depth;
};

struct CellMSSulpha3dObjectPosition
{
	f32 x;
	f32 y;
	f32 z;
};

struct CellMSSulpha3dObjectRGBA
{
	u8 red;
	u8 green;
	u8 blue;
	u8 alpha;
};

struct CellMSSulpha3dObject
{
	s32 type;
	CellMSSulpha3dObjectPosition position;
	CellMSSulpha3dObjectDimensions dimension;
	CellMSSulpha3dObjectRGBA colour;
};

struct CellMSSulphaConfiguration
{
	char sendSubData[32][2][8];
	char sendStreamData[512];
	char compressBusToADPCM;
	char sendDebugInformation;
	s32 namedObjects;
};

struct CellMSSurroundInfo
{
	f32 volumes[8];
	s32 pitch;
	f32 distance;
};

struct CellMSSurroundPoint
{
	f32 X;
	f32 Y;
};

struct CellMSSystemConfig
{
	s32 channelCount;
	s32 subCount;
	s32 dspPageCount;
	s32 flags;
};

struct CellMSSystemInformation
{
	s32 totalCycleCount;
	s32 miscCycleCount;
	s32 totalNumberOfStreamsProcessed;
	s32 totalMemoryBandwidth;
	s32 MemoryBandwidthIn;
	s32 MemoryBandwidthOut;
	s32 DSPBandwidthIn;
	s32 DSPBandwidthOut;
};

struct CellMSWaveSection
{
	u32 numberOfChannels;
	u32 lengthOfChannel;
	u32 sampleRate;
	u8 endian;
	u8 reserved;
};
