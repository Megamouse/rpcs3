#include "stdafx.h"
#include "Emu/Cell/PPUModule.h"

#include "cellMS.h"
#include "cellAudio.h"

logs::channel cellMS("cellMS");

void cellMSATRACClose()
{
	cellMS.todo("cellMSATRACClose()");
}

s32 cellMSATRACGetNeededMemorySize(s32 channels)
{
	cellMS.todo("cellMSATRACGetNeededMemorySize(channels=%d)", channels);

	return CELL_OK;
}

s32 cellMSATRACInit(s32 channels, vm::ptr<void> ATRACMemory)
{
	cellMS.todo("cellMSATRACInit(channels=%d, ATRACMemory=*0x%x)", channels, ATRACMemory);

	if(!ATRACMemory)
		return -1;

	return CELL_OK;
}

void cellMSCoreBypassDSP(s32 chNo, const CELL_MS_DSPSLOTS slot, const CELL_MS_BYPASS bypass)
{
	cellMS.todo("cellMSCoreBypassDSP(chNo=%d, slot=%d, bypass=%d)", chNo, slot, bypass);
}

CELL_MS_BYPASS cellMSCoreGetBypass(s32 chNo, const CELL_MS_PATH type)
{
	cellMS.todo("cellMSCoreGetBypass(chNo=%d, type=%d)", chNo, type);

	return CELL_MS_NOTBYPASSED;
}

s32 cellMSCoreGetDSPHandle(s32 chNo, const CELL_MS_DSPSLOTS slot)
{
	cellMS.todo("cellMSCoreGetDSPHandle(chNo=%d, slot=%d)", chNo, slot);

	return CELL_MS_NOTBYPASSED;
}

vm::ptr<void> cellMSCoreGetDSPParamAddr(s32 chNo, const CELL_MS_DSPSLOTS slot)
{
	cellMS.todo("cellMSCoreGetDSPParamAddr(chNo=%d, slot=%d)", chNo, slot);

	return vm::null;
}

vm::ptr<f32> cellMSCoreGetEQ(s32 chNo)
{
	cellMS.todo("cellMSCoreGetEQ(chNo=%d)", chNo);

	return vm::null;
}

CELL_MS_EQSTATE cellMSCoreGetEQEnable(s32 chNo, const CELL_MS_PATH type)
{
	cellMS.todo("cellMSCoreGetEQEnable(chNo=%d, type=%d)", chNo, type);

	return CELL_MS_EQDISABLED;
}

s32 cellMSCoreGetMask(s32 chNo, const CELL_MS_DSPMASKS dspMask, const CELL_MS_DSPSLOTS dspSlot, const s32 maskSlot)
{
	cellMS.todo("cellMSCoreGetMask(chNo=%d, dspMask=%d, dspSlot=%d, maskSlot=%d)", chNo, dspMask, dspSlot, maskSlot);

	return CELL_OK;
}

vm::ptr<char> cellMSCoreGetMasks(
	s32 chNo,
	const CELL_MS_DSPMASKS dspMask,
	const CELL_MS_DSPSLOTS dspSlot
)
{
	cellMS.todo("cellMSCoreGetMasks(chNo=%d, dspMask=%d, dspSlot=%d)", chNo, dspMask, dspSlot);

	return vm::null;
}

CELL_MS_PAUSE cellMSCoreGetPause(s32 chNo)
{
	cellMS.todo("cellMSCoreGetPause(chNo=%d)", chNo);

	return CELL_MS_NOTPAUSED;
}

s32 cellMSCoreGetRouting(s32 busNo, s32 destBus, vm::ptr<s32> dryStatus, vm::ptr<s32> wetStatus, vm::ptr<f32> dryVolume, vm::ptr<f32> wetVolume)
{
	cellMS.todo("cellMSCoreGetRouting(busNo=%d, destBus=%d, dryStatus=*0x%x, dryVolume=*0x%x, wetVolume=*0x%x)", busNo, destBus, dryStatus, dryVolume, wetVolume);

	return CELL_OK;
}

s32 cellMSCoreGetRouting8(s32 busNo, s32 destBus, vm::ptr<s32> dryStatus, vm::ptr<s32> wetStatus, vm::ptr<f32> dryVolumes, vm::ptr<f32> wetVolumes)
{
	cellMS.todo("cellMSCoreGetRouting8(busNo=%d, destBus=%d, dryStatus=*0x%x, dryVolumes=*0x%x, wetVolumes=*0x%x)", busNo, destBus, dryStatus, dryVolumes, wetVolumes);

	return CELL_OK;
}

f32 cellMSCoreGetVolume1(s32 chNo, const CELL_MS_PATH type, const CELL_MS_SPEAKERS speakerNo, const CELL_MS_CHANNELS channelNo)
{
	cellMS.todo("cellMSCoreGetVolume1(chNo=%d, type=%d, speakerNo=%d, channelNo=%d)", chNo, type, speakerNo, channelNo);

	return 1.0f;
}

f32 cellMSCoreGetVolume64(s32 chNo, const CELL_MS_PATH type)
{
	cellMS.todo("cellMSCoreGetVolume64(chNo=%d, type=%d)", chNo, type);

	return 1.0f;
}

f32 cellMSCoreGetVolume8(s32 chNo, const CELL_MS_PATH type, const CELL_MS_CHANNELS channelNo)
{
	cellMS.todo("cellMSCoreGetVolume8(chNo=%d, type=%d, channelNo=%d)", chNo, type, channelNo);

	return 1.0f;
}

f32 cellMSCoreGetWetInGain(s32 chNo)
{
	cellMS.todo("cellMSCoreGetVolume8(chNo=%d)", chNo);

	return 1.0f;
}

s32 cellMSCoreInit(s32 chNo)
{
	cellMS.todo("cellMSCoreInit(chNo=%d)", chNo);

	return CELL_OK;
}

s32 cellMSCoreRoutingInit(s32 typeBitMask)
{
	cellMS.todo("cellMSCoreRoutingInit(typeBitMask=%d)", typeBitMask);

	return CELL_OK;
}

void cellMSCoreSetBypass(s32 chNo, const CELL_MS_PATH type, const CELL_MS_BYPASS bypass)
{
	cellMS.todo("cellMSCoreSetBypass(chNo=%d, type=%d, bypass=%d)", chNo, type, bypass);
}

void cellMSCoreSetDSP(s32 chNo, const CELL_MS_DSPSLOTS slot, const s32 dspHandle)
{
	cellMS.todo("cellMSCoreSetDSP(chNo=%d, slot=%d, dspHandle=%d)", chNo, slot, dspHandle);
}

s32 cellMSCoreSetEQ(s32 chNo, vm::cptr<f32> eqData)
{
	cellMS.todo("cellMSCoreSetEQ(chNo=%d, eqData=*0x%x)", chNo, eqData);

	return CELL_OK;
}

void cellMSCoreSetEQEnable(s32 chNo, const CELL_MS_PATH type, const CELL_MS_EQSTATE eqSwitch)
{
	cellMS.todo("cellMSCoreSetEQEnable(chNo=%d, type=%d, eqSwitch=%d)", chNo, type, eqSwitch);
}

void cellMSCoreSetMask(s32 chNo, const CELL_MS_DSPMASKS dspMask, const CELL_MS_DSPSLOTS dspSlot, const s32 maskSlot, const char bitMask)
{
	cellMS.todo("cellMSCoreSetMask(chNo=%d, dspMask=%d, dspSlot=%d, maskSlot=%d, bitMask=%d)", chNo, dspMask, dspSlot, maskSlot, bitMask);
}

void cellMSCoreSetMasks(s32 chNo, const CELL_MS_DSPMASKS dspMask, const CELL_MS_DSPSLOTS dspSlot, vm::cptr<char> bitMasks)
{
	cellMS.todo("cellMSCoreSetMask(chNo=%d, dspMask=%d, dspSlot=%d, maskSlot=%d, bitMasks=*0x%x)", chNo, dspMask, dspSlot, bitMasks);
}

void cellMSCoreSetPause(s32 chNo, const CELL_MS_PAUSE pause)
{
	cellMS.todo("cellMSCoreSetPause(chNo=%d, pause=%d)", chNo, pause);
}

s32 cellMSCoreSetRouting(s32 busNo, s32 destBus, const CELL_MS_PATH type, const CELL_MS_ROUTES onOff, const f32 dryVolume, f32 wetVolume)
{
	cellMS.todo("cellMSCoreSetRouting(busNo=%d, destBus=%d, type=%d, onOff=%d, dryVolume=%f, wetVolume=%f)", busNo, destBus, type, onOff, dryVolume, wetVolume);

	return CELL_OK;
}

s32 cellMSCoreSetRouting8(s32 busNo, s32 destBus, const CELL_MS_PATH type, const CELL_MS_ROUTES onOff, vm::cptr<f32> dryVolume, vm::cptr<f32> wetVolume)
{
	cellMS.todo("cellMSCoreSetRouting8(busNo=%d, destBus=%d, type=%d, onOff=%d, dryVolume=*0x%x, wetVolume=*0x%x)", busNo, destBus, type, onOff, dryVolume, wetVolume);

	return CELL_OK;
}

s32 cellMSCoreSetVolume1(s32 chNo, const CELL_MS_PATH type, const CELL_MS_SPEAKERS speakerNo, const CELL_MS_CHANNELS channelNo, const f32 volume)
{
	cellMS.todo("cellMSCoreSetVolume1(chNo=%d, type=%d, speakerNo=%d, channelNo=%d, volume=%f)", chNo, type, speakerNo, channelNo, volume);

	return CELL_OK;
}

s32 cellMSCoreSetVolume64(s32 chNo, const CELL_MS_PATH type, vm::cptr<f32> volumes)
{
	cellMS.todo("cellMSCoreSetVolume64(chNo=%d, type=%d, volumes=*0x%x)", chNo, type, volumes);

	return CELL_OK;
}

s32 cellMSCoreSetVolume8(s32 chNo, const CELL_MS_PATH type, const CELL_MS_CHANNELS channelNo, vm::cptr<f32> volumes)
{
	cellMS.todo("cellMSCoreSetVolume8(chNo=%d, type=%d, channelNo=%d, volumes=*0x%x)", chNo, type, channelNo, volumes);

	return CELL_OK;
}

void cellMSCoreSetWetInGain(s32 chNo, const f32 inGain)
{
	cellMS.todo("cellMSCoreSetWetInGain(chNo=%d, inGain=%f)", chNo, inGain);
}

void cellMSCoreStop(s32 chNo, const s32 flag)
{
	cellMS.todo("cellMSCoreStop(chNo=%d, flag=%d)", chNo, flag);
}

s32 cellMSDSPCalcMemoryNeededForChunks(const s32 sizeOfFileInBytes)
{
	cellMS.todo("cellMSDSPCalcMemoryNeededForChunks(sizeOfFileInBytes=%d)", sizeOfFileInBytes);

	return CELL_OK;
}

s32 cellMSDSPCreateFFTChunks(vm::ptr<s16> sampleData, const s32 fileSize, vm::ptr<CellMSComplexNum> chunks, s32 flag)
{
	cellMS.todo("cellMSDSPCreateFFTChunks(sampleData=*0x%x, fileSize=%d, chunks=*0x%x, flag=%d)", sampleData, fileSize, chunks, flag);

	return CELL_OK;
}

s32 cellMSDSPLoadDSPFromFile(vm::cptr<char> DSPName, vm::cptr<void> WS, vm::ptr<CellMSDSP> user, const s32 handle, const s32 page)
{
	cellMS.todo("cellMSDSPLoadDSPFromFile(DSPName=*0x%x, WS=*0x%x, user=*0x%x, handle=%d, page=%d)", DSPName, WS, user, handle, page);

	return CELL_OK;
}

s32 cellMSDSPLoadDSPFromMemory(vm::cptr<void> DSPEffect, vm::ptr<CellMSDSP> user, const s32 handle, const s32 page)
{
	cellMS.todo("cellMSDSPLoadDSPFromMemory(DSPEffect=*0x%x, user=*0x%x, handle=%d, page=%d)", DSPEffect, user, handle, page);

	return CELL_OK;
}

s32 cellMSDSPResetPage(const s32 pageNum)
{
	cellMS.todo("cellMSDSPResetPage(pageNum=%d)", pageNum);

	return CELL_OK;
}

s32 cellMSEnvSetup(const s32 streamNo, vm::ptr<CellMSEnvelope> env)
{
	cellMS.todo("cellMSEnvSetup(streamNo=%d, env=*0x%x)", streamNo, env);

	return CELL_OK;
}

s32 cellMSEQGet512Points(vm::ptr<CellEQSection> section, vm::ptr<f32> values)
{
	cellMS.todo("cellMSEnvSetup(streamNo=%d, env=*0x%x)", section, values);

	return CELL_OK;
}

s32 cellMSEQGet512PointsMorphed(vm::ptr<CellEQMorphSection> mSection, f32 pos, vm::ptr<f32> values)
{
	cellMS.todo("cellMSEQGet512PointsMorphed(mSection=*0x%x, pos=%f, values=*0x%x)", mSection, pos, values);

	return CELL_OK;
}

s32 cellMSEQGetFFTBand(f32 hz)
{
	cellMS.todo("cellMSEQGetFFTBand(hz=%f)", hz);

	return CELL_OK;
}

s32 cellMSEQGetNumberOfPoints(vm::ptr<CellEQSection> section)
{
	cellMS.todo("cellMSEQGetNumberOfPoints(section=*0x%x)", section);

	return CELL_OK;
}

vm::ptr<CellEQPoint> cellMSEQGetPoint(vm::ptr<CellEQSection> section, s32 index)
{
	cellMS.todo("cellMSEQGetPoint(section=*0x%x, index=d)", section, index);

	return vm::null;
}

s32 cellMSEQInitMorphEQ(vm::ptr<CellEQMorphSection> mSection, vm::ptr<CellEQSection> start, vm::ptr<CellEQSection> end)
{
	cellMS.todo("cellMSEQInitMorphEQ(mSection=*0x%x, start=*0x%x, end=*0x%x)", mSection, start, end);

	return CELL_OK;
}

s32 cellMSEQOffsetPoint(vm::ptr<CellEQSection> section, s32 index, f32 offX, f32 offY)
{
	cellMS.todo("cellMSEQOffsetPoint(section=*0x%x, index=%d, offX=%f, offY=%f)", section, index, offX, offY);

	return CELL_OK;
}

s32 cellMSEQOffsetTag(vm::ptr<CellEQSection> section, vm::ptr<u8> tag, f32 offX, f32 offY)
{
	cellMS.todo("cellMSEQOffsetTag(section=*0x%x, tag=*0x%x, offX=%f, offY=%f)", section, tag, offX, offY);

	return CELL_OK;
}

s32 cellMSEQSetPoint(vm::ptr<CellEQSection> section, s32 index, f32 x, f32 y)
{
	cellMS.todo("cellMSEQSetPoint(section=*0x%x, index=%d, x=%f, y=%f)", section, index, x, y);

	return CELL_OK;
}

s32 cellMSEQSetTag(vm::ptr<CellEQSection> section, vm::ptr<u8> tag, f32 x, f32 y)
{
	cellMS.todo("cellMSEQSetTag(section=*0x%x, tag=*0x%x, x=%f, y=%f)", section, tag, x, y);

	return CELL_OK;
}

s32 cellMSFXAuxReturnGetNeededMemorySize(const s32 nReturnChannels)
{
	cellMS.todo("cellMSFXAuxReturnGetNeededMemorySize(nReturnChannels=%d)", nReturnChannels);

	return CELL_OK;
}

s32 cellMSFXAuxReturnInit(vm::ptr<void> paramList, vm::ptr<void> pAuxReturnMemory, const s32 nReturnChannels, const s32 nCoreId)
{
	cellMS.todo("cellMSFXAuxReturnInit(paramList=*0x%x, pAuxReturnMemory=*0x%x, nReturnChannels=%d, nCoreId=%d)", paramList, pAuxReturnMemory, nReturnChannels, nCoreId);

	return CELL_OK;
}

s32 cellMSFXAuxReturnSetThruMix(vm::ptr<void> paramList, const s32 nReturnChan, const f32 fThruMix)
{
	cellMS.todo("cellMSFXAuxReturnSetThruMix(paramList=*0x%x, nReturnChan=%d, fThruMix=%d)", paramList, nReturnChan, fThruMix);

	return CELL_OK;
}

s32 cellMSFXAuxSendGetNeededMemorySize(vm::cptr<CellMSFXAuxSend> pInfo)
{
	cellMS.todo("cellMSFXAuxSendGetNeededMemorySize(pInfo=*0x%x)", pInfo);

	return CELL_OK;
}

s32 cellMSFXAuxSendInit(vm::ptr<void> paramList, vm::ptr<void> pAuxSendMemory, vm::cptr<CellMSFXAuxSend> pInfo)
{
	cellMS.todo("cellMSFXAuxSendInit(paramList=*0x%x, pAuxSendMemory=*0x%x, pInfo=*0x%x)", paramList, pAuxSendMemory, pInfo);

	return CELL_OK;
}

s32 cellMSFXAuxSendSet(vm::ptr<void> pAuxSendMemory, const s32 nSendChannel, const s32 nSendIndex, vm::cptr<void> pAuxReturnMemory, const s32 nReturnChannel, const f32 fVolume)
{
	cellMS.todo("cellMSFXAuxSendSet(pAuxSendMemory=*0x%x, nSendChannel=%d, nSendIndex=%d, pAuxReturnMemory=*0x%x, nReturnChannel=%d, fVolume=%f)", pAuxSendMemory, nSendChannel, nSendIndex, pAuxReturnMemory, nReturnChannel, fVolume);

	return CELL_OK;
}

s32 cellMSFXAuxSendSetVolumes(vm::ptr<void> pAuxSendMemory, const s32 nSendChannel, vm::cptr<f32> pfVolumes, const s32 nVolumes)
{
	cellMS.todo("cellMSFXAuxSendSetVolumes(pAuxSendMemory=*0x%x, nSendChannel=%d, pfVolumes=*0x%x, nVolumes=%d)", pAuxSendMemory, nSendChannel, pfVolumes, nVolumes);

	return CELL_OK;
}

s32 cellMSFXCompressorInitApply(vm::ptr<void> addr, vm::ptr<CellMSFXCompressorInfo> buffer)
{
	cellMS.todo("cellMSFXCompressorInitApply(addr=*0x%x, buffer=*0x%x)", addr, buffer);

	return CELL_OK;
}

s32 cellMSFXCompressorInitCalc(vm::ptr<void> addr, vm::ptr<CellMSFXCompressorInfo> buffer)
{
	cellMS.todo("cellMSFXCompressorInitCalc(addr=*0x%x, buffer=*0x%x)", addr, buffer);

	return CELL_OK;
}

s32 cellMSFXDelayGetNeededMemorySize(s32 numDelays)
{
	cellMS.todo("cellMSFXDelayGetNeededMemorySize(numDelays=%d)", numDelays);

	return CELL_OK;
}

s32 cellMSFXDelayInit(vm::ptr<void> paramList, vm::ptr<void> pDelayMemBlock, vm::cptr<CellMSFXDelay> pParams, s32 numActiveDelayChannels)
{
	cellMS.todo("cellMSFXDelayInit(paramList=*0x%x, pDelayMemBlock=*0x%x, pParams=*0x%x, numActiveDelayChannels=%d)", paramList, pDelayMemBlock, pParams, numActiveDelayChannels);

	return CELL_OK;
}

s32 cellMSFXDelaySetParams(vm::ptr<void> pDelayMemBlock, vm::cptr<CellMSFXDelay> pParams, s32 nSlots)
{
	cellMS.todo("cellMSFXDelaySetParams(pDelayMemBlock=*0x%x, pParams=*0x%x, numActiveDelayChannels=%d)", pDelayMemBlock, pParams, nSlots);

	return CELL_OK;
}

s32 cellMSFXDistortionGet(vm::ptr<void> addr, vm::ptr<CellMSFXDistortion> data)
{
	cellMS.todo("cellMSFXDistortionGet(addr=*0x%x, data=*0x%x)", addr, data);

	return CELL_OK;
}

s32 cellMSFXDistortionInit(vm::ptr<void> addr, vm::ptr<CellMSFXDistortion> data)
{
	cellMS.todo("cellMSFXDistortionInit(addr=*0x%x, data=*0x%x)", addr, data);

	return CELL_OK;
}

s32 cellMSFXDistortionSet(vm::ptr<void> addr, vm::ptr<CellMSFXDistortion> data)
{
	cellMS.todo("cellMSFXDistortionSet(addr=*0x%x, data=*0x%x)", addr, data);

	return CELL_OK;
}

s32 cellMSFXEQGet(vm::ptr<void> addr, s32 slot)
{
	cellMS.todo("cellMSFXEQGet(addr=*0x%x, slot=%d)", addr, slot);

	return CELL_OK;
}

s32 cellMSFXEQInit(vm::ptr<void> addr, vm::ptr<f32> buffer)
{
	cellMS.todo("cellMSFXEQInit(addr=*0x%x, buffer=*0x%x)", addr, buffer);

	return CELL_OK;
}

s32 cellMSFXEQSet(vm::ptr<void> addr, vm::ptr<f32> buffer, s32 slot)
{
	cellMS.todo("cellMSFXEQSet(addr=*0x%x, buffer=*0x%x, slot=%d)", addr, buffer, slot);

	return CELL_OK;
}

s32 cellMSFXFdnGetNeededMemorySize(const s32 numChannels, const s32 nLateReverb)
{
	cellMS.todo("cellMSFXFdnGetNeededMemorySize(numChannels=%d, nLateReverb=d)", numChannels, nLateReverb);

	return CELL_OK;
}

s32 cellMSFXFdnInit(vm::ptr<void> pParamAddrSlot1, vm::ptr<void> pParamAddrSlot2, vm::ptr<void> pAllocatedMemory, vm::cptr<CellMSFXI3DL2Params> pParams, const CELL_MSFX_SLOTMASK_MODE eChannelMode, const f32 fDryVolmB, vm::cptr<CellMSFXEarlyReflection> pReflectionPattern)
{
	cellMS.todo("cellMSFXFdnInit(pParamAddrSlot1=*0x%x, pParamAddrSlot2=*0x%x, pAllocatedMemory=*0x%x, pParams=*0x%x, eChannelMode=%d, fDryVolmB=%f, pReflectionPattern=*0x%x)", pParamAddrSlot1, pParamAddrSlot2, pAllocatedMemory, pParams, eChannelMode, fDryVolmB, pReflectionPattern);

	return CELL_OK;
}

s32 cellMSFXFdnSetParams(vm::ptr<void> pAllocatedMemory, vm::cptr<CellMSFXI3DL2Params> pParams, const s32 slot, const f32 fDryVolmB, vm::cptr<CellMSFXEarlyReflection> pReflectionPattern)
{
	cellMS.todo("cellMSFXFdnSetParams(pAllocatedMemory=*0x%x, pParams=*0x%x, slot=%d, fDryVolmB=%f, pReflectionPattern=*0x%x)", pAllocatedMemory, pParams, slot, fDryVolmB, pReflectionPattern);

	return CELL_OK;
}

s32 cellMSFXFilterGet(vm::ptr<void> addr, vm::ptr<CellMSFXFilter> pSettings, s32 slot)
{
	cellMS.todo("cellMSFXFilterSet(addr=*0x%x, pSettings=*0x%x, slot=d)", addr, pSettings, slot);

	return CELL_OK;
}

s32 cellMSFXFilterGetNeededMemorySize(const s32 maxChannel)
{
	cellMS.todo("cellMSFXFilterGetNeededMemorySize(maxChannel=%d)", maxChannel);

	return CELL_OK;
}

s32 cellMSFXFilterInit(vm::ptr<void> paramList, vm::ptr<void> pFilterMemBlock, vm::cptr<CellMSFXFilter> pSettings)
{
	cellMS.todo("cellMSFXFilterInit(paramList=*0x%x, pFilterMemBlock=*0x%x, pSettings=*0x%x)", paramList, pFilterMemBlock, pSettings);

	return CELL_OK;
}

s32 cellMSFXFilterSet(vm::ptr<void> paramList, vm::cptr<CellMSFXFilter> pSettings, s32 slot)
{
	cellMS.todo("cellMSFXFilterSet(paramList=*0x%x, pSettings=*0x%x, slot=d)", paramList, pSettings, slot);

	return CELL_OK;
}

s32 cellMSFXI3DL2GetNeededMemorySize(const s32 numChannels)
{
	cellMS.todo("cellMSFXI3DL2GetNeededMemorySize(numChannels=%d)", numChannels);

	return CELL_OK;
}

s32 cellMSFXI3DL2Init(vm::ptr<void> addr, vm::ptr<void> pAllocatedMemory, vm::cptr<CellMSFXI3DL2Params> pParams, const s32 numChannels)
{
	cellMS.todo("cellMSFXI3DL2Init(addr=*0x%x, pAllocatedMemory=*0x%x, pParams=*0x%x, numChannels=%d)", addr, pAllocatedMemory, pParams, numChannels);

	return CELL_OK;
}

s32 cellMSFXI3DL2SetParams(vm::ptr<void> addr, vm::ptr<void> pAllocatedMemory, vm::cptr<CellMSFXI3DL2Params> pParams, const s32 slot)
{
	cellMS.todo("cellMSFXI3DL2SetParams(addr=*0x%x, pAllocatedMemory=*0x%x, pParams=*0x%x, slot=%d)", addr, pAllocatedMemory, pParams, slot);

	return CELL_OK;
}

s32 cellMSFXIRDSPClearAccumBuffers(vm::ptr<void> reverb_dsp_info)
{
	cellMS.todo("cellMSFXIRDSPClearAccumBuffers(reverb_dsp_info=*0x%x)", reverb_dsp_info);

	return CELL_OK;
}

s32 cellMSFXIRDSPCreateImpulse(vm::ptr<void> impulse_mem, s32 fileSize, vm::ptr<char> fileData, s32 endian)
{
	cellMS.todo("cellMSFXIRDSPCreateImpulse(impulse_mem=*0x%x, fileSize=%d, fileData=*0x%x, endian=%d)", impulse_mem, fileSize, fileData, endian);

	return CELL_OK;
}

s32 cellMSFXIRDSPImpulseGetNeededMemorySize(s32 fileSize)
{
	cellMS.todo("cellMSFXIRDSPImpulseGetNeededMemorySize(fileSize=%d)", fileSize);

	return CELL_OK;
}

s32 cellMSFXIRDSPInfoGetNeededMemorySize(s32 num_impulses, vm::pptr<void> impulses)
{
	cellMS.todo("cellMSFXIRDSPInfoGetNeededMemorySize(num_impulses=%d, impulses=*0x%x)", num_impulses, impulses);

	return CELL_OK;
}

s32 cellMSFXIRDSPInit(vm::ptr<void> paramList, vm::ptr<void> info)
{
	cellMS.todo("cellMSFXIRDSPInit(paramList=*0x%x, info=*0x%x)", paramList, info);

	return CELL_OK;
}

s32 cellMSFXIRDSPInitInfo(vm::ptr<void> reverb_dsp_info, s32 num_impulses, vm::pptr<void> array_impulses)
{
	cellMS.todo("cellMSFXIRDSPInitInfo(reverb_dsp_info=*0x%x, num_impulses=%d, array_impulses=**0x%x)", reverb_dsp_info, num_impulses, array_impulses);

	return CELL_OK;
}

s32 cellMSFXMeterInit(vm::ptr<void> pParamAddrSlot1, const s32 nChans, const f32 fThreshold, vm::ptr<CellMSFXMeter> pOutputArray)
{
	cellMS.todo("cellMSFXMeterInit(pParamAddrSlot1=*0x%x, nChans=%d, fThreshold=%f, pOutputArray=*0x%x)", pParamAddrSlot1, nChans, fThreshold, pOutputArray);

	return CELL_OK;
}

s32 cellMSFXMultiCompressorApplyAddInput(vm::ptr<void> addr, const s32 nApplyChannel, const s32 nCalcChannel)
{
	cellMS.todo("cellMSFXMultiCompressorApplyAddInput(addr=*0x%x, nApplyChannel=%d, nCalcChannel=%d)", addr, nApplyChannel, nCalcChannel);

	return CELL_OK;
}

s32 cellMSFXMultiCompressorApplyResetInputs(vm::ptr<void> addr, const s32 nApplyChannel)
{
	cellMS.todo("cellMSFXMultiCompressorApplyResetInputs(addr=*0x%x, nApplyChannel=%d)", addr, nApplyChannel);

	return CELL_OK;
}

s32 cellMSFXMultiCompressorInitApply(vm::ptr<void> addr, vm::ptr<CellMSFXCompressorInfo> pBuffers, const s32 nChannels)
{
	cellMS.todo("cellMSFXMultiCompressorInitApply(addr=*0x%x, pBuffers=*0x%x, nChannels=%d)", addr, pBuffers, nChannels);

	return CELL_OK;
}

s32 cellMSFXMultiCompressorInitCalc(vm::ptr<void> addr, vm::ptr<CellMSFXCompressorInfo> pBuffers, const s32 nChannels)
{
	cellMS.todo("cellMSFXMultiCompressorInitCalc(addr=*0x%x, pBuffers=*0x%x, nChannels=%d)", addr, pBuffers, nChannels);

	return CELL_OK;
}

s32 cellMSFXParaEQGetNeededMemorySize(const s32 maxChannel)
{
	cellMS.todo("cellMSFXParaEQGetNeededMemorySize(maxChannel=%d)", maxChannel);

	return CELL_OK;
}

s32 cellMSFXParaEQInit(
	vm::ptr<void> paramList,
	vm::ptr<void> pParaEQMemBlock,
	vm::cptr<CellMSFXParaEQ> pSettings,
	const s32 numActiveChannels
)
{
	cellMS.todo("cellMSFXParaEQInit(paramList=*0x%x, pParaEQMemBlock=*0x%x, pSettings=*0x%x, numActiveChannels=%d)", paramList, pParaEQMemBlock, pSettings, numActiveChannels);

	return CELL_OK;
}

s32 cellMSFXParaEQSet(vm::ptr<void> paramList, vm::cptr<CellMSFXParaEQ> pSettings, s32 slot)
{
	cellMS.todo("cellMSFXParaEQSet(paramList=*0x%x, pSettings=*0x%x, slot=%d)", paramList, pSettings, slot);

	return CELL_OK;
}

s32 cellMSFXParaEQSetLFE(vm::ptr<void> paramList, const s32 slot, const CELL_MSFX_FILTERMODE nLFE_Mode)
{
	cellMS.todo("cellMSFXParaEQSetLFE(paramList=*0x%x, slot=%f, nLFE_Mode=%d)", paramList, slot, nLFE_Mode);

	return CELL_OK;
}

s32 cellMSFXPitchShiftGet(vm::ptr<void> paramList, vm::ptr<f32> cents, s32 slot)
{
	cellMS.todo("cellMSFXPitchShiftGet(paramList=*0x%x, cents=*0x%x, slot=%d)", paramList, cents, slot);

	return CELL_OK;
}

s32 cellMSFXPitchShiftGetNeededMemorySize(s32 numActivePitches)
{
	cellMS.todo("cellMSFXPitchShiftGetNeededMemorySize(numActivePitches=%d)", numActivePitches);

	return CELL_OK;
}

s32 cellMSFXPitchShiftInit(vm::ptr<void> paramList, vm::ptr<void> pitchMemBlock, f32 pitch, s32 numActivePitches)
{
	cellMS.todo("cellMSFXPitchShiftInit(paramList=*0x%x, pitchMemBlock=*0x%x, pitch=%f, numActivePitches=%d)", paramList, pitchMemBlock, pitch, numActivePitches);

	return CELL_OK;
}

s32 cellMSFXPitchShiftSet(vm::ptr<void> paramList, f32 cents, s32 slot)
{
	cellMS.todo("cellMSFXPitchShiftInit(paramList=*0x%x, cents=%f, slot=%d)", paramList, cents, slot);

	return CELL_OK;
}

s32 cellMSFXReverbGetNeededMemorySize(s32 numReverbs)
{
	cellMS.todo("cellMSFXReverbGetNeededMemorySize(numReverbs=%d)", numReverbs);

	return CELL_OK;
}

s32 cellMSFXReverbInit(vm::ptr<void> addr, vm::ptr<void> pReverbMemBlock, vm::cptr<CellMSFXReverbParams> pParams, s32 numActiveReverbChannels)
{
	cellMS.todo("cellMSFXReverbInit(addr=*0x%x, pReverbMemBlock=*0x%x, pParams=*0x%x, numActiveReverbChannels=%d)", addr, pReverbMemBlock, pParams, numActiveReverbChannels);

	return CELL_OK;
}

s32 cellMSFXReverbSetParams(vm::ptr<void> pReverbMemBlock, vm::cptr<CellMSFXReverbParams> pParams, s32 slot)
{
	cellMS.todo("cellMSFXReverbSetParams(pReverbMemBlock=*0x%x, pParams=*0x%x, slot=%d)", pReverbMemBlock, pParams, slot);

	return CELL_OK;
}

s32 cellMSFXRingModGetAmplitude(vm::ptr<void> addr, vm::ptr<f32> amplitude, s32 slot)
{
	cellMS.todo("cellMSFXRingModGetAmplitude(addr=*0x%x, amplitude=*0x%x, slot=%d)", addr, amplitude, slot);

	return CELL_OK;
}

s32 cellMSFXRingModGetFrequency(vm::ptr<void> addr, vm::ptr<f32> hz, s32 slot)
{
	cellMS.todo("cellMSFXRingModGetFrequency(addr=*0x%x, hz=*0x%x, slot=%d)", addr, hz, slot);

	return CELL_OK;
}

s32 cellMSFXRingModInit(vm::ptr<void> addr, f32 hz, f32 amplitude)
{
	cellMS.todo("cellMSFXRingModInit(addr=*0x%x, hz=%f, amplitude=%f)", addr, hz, amplitude);

	return CELL_OK;
}

s32 cellMSFXRingModSetAmplitude(vm::ptr<void> addr, f32 amplitude, s32 slot)
{
	cellMS.todo("cellMSFXRingModSetAmplitude(addr=*0x%x, amplitude=%f, slot=%d)", addr, amplitude, slot);

	return CELL_OK;
}

s32 cellMSFXRingModSetFrequency(vm::ptr<void> addr, f32 hz, s32 slot)
{
	cellMS.todo("cellMSFXRingModSetFrequency(addr=*0x%x, hz=%f, slot=%d)", addr, hz, slot);

	return CELL_OK;
}

s32 cellMSFXSetupSlotMasks(const s32 nCore, const CELL_MS_DSPSLOTS nDspSlot, const CELL_MSFX_SLOTMASK_MODE eMode)
{
	cellMS.todo("cellMSFXSetupSlotMasks(nCore=%d, nDspSlot=%d, eMode=%d)", nCore, nDspSlot, eMode);

	return CELL_OK;
}

s32 cellMSFXSoftClipInit(vm::ptr<void> paramList, vm::cptr<CellMSFXSoftClipper> pSettings, const s32 nChannels)
{
	cellMS.todo("cellMSFXSoftClipInit(paramList=*0x%x, pSettings=*0x%x, nChannels=%d)", paramList, pSettings, nChannels);

	return CELL_OK;
}

s32 cellMSFXSoftClipSet(vm::ptr<void> paramList, vm::cptr<CellMSFXSoftClipper> pSettings, const s32 nSlot)
{
	cellMS.todo("cellMSFXSoftClipSet(paramList=*0x%x, pSettings=*0x%x, nSlot=%d)", paramList, pSettings, nSlot);

	return CELL_OK;
}

s32 cellMSFXVocoderInitCarrier(vm::ptr<void> addr, vm::ptr<CellMSFXVocoderInfo> buffer)
{
	cellMS.todo("cellMSFXVocoderInitCarrier(addr=*0x%x, buffer=*0x%x)", addr, buffer);

	return CELL_OK;
}

s32 cellMSFXVocoderInitModulator(vm::ptr<void> addr, vm::ptr<CellMSFXVocoderInfo> buffer)
{
	cellMS.todo("cellMSFXVocoderInitModulator(addr=*0x%x, buffer=*0x%x)", addr, buffer);

	return CELL_OK;
}

s32 cellMSFXVocoderSetAllBandsBypass(vm::ptr<void> addr, s64 switches)
{
	cellMS.todo("cellMSFXVocoderSetAllBandsBypass(addr=*0x%x, switches=%d)", addr, switches);

	return CELL_OK;
}

s32 cellMSFXVocoderSetAllEQBands(vm::ptr<void> addr, vm::ptr<f32> values)
{
	cellMS.todo("cellMSFXVocoderSetAllEQBands(addr=*0x%x, values=*0x%x)", addr, values);

	return CELL_OK;
}

s32 cellMSFXVocoderSetBandBypass(vm::ptr<void> addr, u32 band, u8 vSwitch)
{
	cellMS.todo("cellMSFXVocoderSetBandBypass(addr=*0x%x, band=%d, value=%d)", addr, band, vSwitch);

	return CELL_OK;
}

s32 cellMSFXVocoderSetEQBand(vm::ptr<void> addr, u32 band, f32 value)
{
	cellMS.todo("cellMSFXVocoderSetEQBand(addr=*0x%x, band=%d, value=%f)", addr, band, value);

	return CELL_OK;
}

s32 cellMSFXVocoderSetModulatorVolume(vm::ptr<void> addr, f32 volume)
{
	cellMS.todo("cellMSFXVocoderSetModulatorVolume(addr=*0x%x, volume=%f)", addr, volume);

	return CELL_OK;
}

s32 cellMSMP3DecoderAddStruct(vm::ptr<void> pDecoderMem, vm::cptr<CellMSMP3DecoderStreamInstance> pInstance)
{
	cellMS.todo("cellMSMP3DecoderAddStruct(pDecoderMem=*0x%x, pInstance=*0x%x)", pDecoderMem, pInstance);

	if (!pDecoderMem || !pInstance)
		return -1;

	return CELL_OK;
}

s32 cellMSMP3DecoderGetPacketInfo(vm::cptr<void> pMp3Data, vm::ptr<CellMSMP3DecoderPacketInfo> pPacket)
{
	cellMS.todo("cellMSMP3DecoderGetPacketInfo(pMp3Data=*0x%x, pPacket=*0x%x)", pMp3Data, pPacket);

	if (!pMp3Data || !pPacket)
		return -1;

	return CELL_OK;
}

s32 cellMSMP3DecoderInit(vm::ptr<void> pDecoderMem, const s32 nDecoderMemSize, vm::ptr<void> pTempMemory, s32 nTempMemSize, vm::ptr<CellMSMP3DecoderInitParams> pParams)
{
	cellMS.todo("cellMSMP3DecoderInit(pDecoderMem=*0x%x, nDecoderMemSize=%d, pTempMemory=*0x%x, nTempMemSize=%d, pParams=*0x%x)", pDecoderMem, nDecoderMemSize, pTempMemory, nTempMemSize, pParams);

	if (!pDecoderMem || !pParams || !pParams)
		return -1;

	if (nTempMemSize < CELL_MSMP3_TEMP_INIT_BUFFER_SIZE)
		return -4;

	return CELL_OK;
}

s32 cellMSMP3DecoderInitStruct(vm::ptr<CellMSMP3DecoderStreamInstance> pInstance)
{
	cellMS.todo("cellMSMP3DecoderInitStruct(pInstance=*0x%x)", pInstance);

	if (!pInstance)
		return -1;

	return CELL_OK;
}

s32 cellMSMP3DecoderLibGetRequiredSize(vm::cptr<CellMSMP3DecoderInitParams> pParams, vm::ptr<s32> pnRequiredSize)
{
	cellMS.todo("cellMSMP3DecoderLibGetRequiredSize(pParams=*0x%x, pnRequiredSize=*0x%x)", pParams, pnRequiredSize);

	if (!pParams || !pnRequiredSize)
		return -1;

	return CELL_OK;
}

s32 cellMSMP3DecoderProcess(vm::ptr<void> pDecoderMem)
{
	cellMS.todo("cellMSMP3DecoderProcess(pDecoderMem=*0x%x)", pDecoderMem);

	if (!pDecoderMem)
		return -1;

	return CELL_OK;
}

s32 cellMSMP3DecoderShutdown(vm::ptr<void> pDecoderMem)
{
	cellMS.todo("cellMSMP3DecoderShutdown(pDecoderMem=*0x%x)", pDecoderMem);

	if (!pDecoderMem)
		return -1;

	return CELL_OK;
}

s32 cellMSMP3GetFrameInfo(vm::ptr<void> data, vm::ptr<CellMSMP3FrameHeader> hdr)
{
	cellMS.todo("cellMSMP3GetFrameInfo(data=*0x%x, hdr=*0x%x)", data, hdr);

	if (!data || !hdr)
		return -1;

	return CELL_OK;
}

s32 cellMSMP3GetNeededMemorySize(s32 channels)
{
	cellMS.todo("cellMSMP3GetNeededMemorySize(channels=%d)", channels);

	return 1;
}

s32 cellMSMP3Init(s32 channels, vm::ptr<void> memory)
{
	cellMS.todo("cellMSMP3Init(channels=%d, memory=*0x%x)", channels, memory);

	return CELL_OK;
}

s32 cellMSMP3IntegratedDecoderGetRequiredSize(vm::ptr<s32> pnSize)
{
	cellMS.todo("cellMSMP3IntegratedDecoderGetRequiredSize(pnSize=*0x%x)", pnSize);

	return CELL_OK;
}

s32 cellMSMP3IntegratedDecoderInit(vm::ptr<void> pDecoderMem, const s32 nDecoderMemSize)
{
	cellMS.todo("cellMSMP3IntegratedDecoderInit(pDecoderMem=*0x%x, nDecoderMemSize=%d)", pDecoderMem, nDecoderMemSize);

	if (!pDecoderMem)
		return -1;

	return CELL_OK;
}

s32 cellMSMP3IntegratedDecoderSpuDecode(vm::ptr<void> pLocalWorkspace, vm::ptr<void> pMainRamAddr, vm::ptr<void> pInBuffer, vm::ptr<s32> pnBytesRead, vm::ptr<s32> pnBytesWritten, vm::ptr<CellMSMP3Context> pContext0, vm::ptr<CellMSMP3Context> pContext1, vm::ptr<s32> pnCount)
{
	cellMS.todo("cellMSMP3IntegratedDecoderSpuDecode(pLocalWorkspace=*0x%x, pMainRamAddr=*0x%x, pInBuffer=*0x%x, pnBytesRead=*0x%x, pnBytesWritten=*0x%x, pContext0=*0x%x, pContext1=*0x%x, pnCount=*0x%x)", pLocalWorkspace, pMainRamAddr, pInBuffer, pnBytesRead, pnBytesWritten, pContext0, pContext1, pnCount);

	return CELL_OK;
}

void cellMSStreamClearRouting(s32 streamNum, CELL_MS_PATH path)
{
	cellMS.todo("cellMSStreamClearRouting(streamNum=%d, path=%s)", streamNum, path);
}

void cellMSStreamClose(const s32 streamNo)
{
	cellMS.todo("cellMSStreamClose(streamNo=%d)", streamNo);
}

vm::ptr<void> cellMSStreamGetCallbackData(const s32 streamNo)
{
	cellMS.todo("cellMSStreamGetCallbackData(streamNo=%d)", streamNo);

	return vm::null;
}

vm::ptr<void> cellMSStreamGetCallbackFunc(const s32 streamNo)
{
	cellMS.todo("cellMSStreamGetCallbackFunc(streamNo=%d)", streamNo);

	return vm::null;
}

f32 cellMSStreamGetCurrentEnv(const s32 streamNo)
{
	cellMS.todo("cellMSStreamGetCurrentEnv(streamNo=%d)", streamNo);

	return 1.;
}

void cellMSStreamGetCurrentPlayPos(const s32 streamNo, vm::ptr<s32> currentOffset, vm::ptr<s32> bufferAddr)
{
	cellMS.todo("cellMSStreamGetCurrentPlayPos(streamNo=%d, currentOffset=*0x%x, bufferAddr=*0x%x)", streamNo, currentOffset, bufferAddr);
}

CELL_MS_BYPASS cellMSStreamGetEnvBypass(const s32 streamNo)
{
	cellMS.todo("cellMSStreamGetEnvBypass(streamNo=%d)", streamNo);

	return CELL_MS_NOTBYPASSED;
}

s32 cellMSStreamGetInfo(const s32 streamNo, vm::ptr<CellMSInfo> data)
{
	cellMS.todo("cellMSStreamGetInfo(streamNo=%d, data=*0x%x)", streamNo, data);

	return CELL_MS_STREAM_CLOSED;
}

s32 cellMSStreamGetMP3FrameInfo(s32 streamNum, vm::ptr<s32> numFrames, vm::ptr<s32> totalSizeOfFrames)
{
	cellMS.todo("cellMSStreamGetMP3FrameInfo(streamNum=%d, numFrames=*0x%x, totalSizeOfFrames=*0x%x)", streamNum, numFrames, totalSizeOfFrames);

	return CELL_OK;
}

s32 cellMSStreamGetMP3FramePos(const s32 streamNo, vm::ptr<s32> currentSampleOffset, vm::ptr<s32> packetSampleSize, vm::ptr<s32> currentSeekAddr)
{
	cellMS.todo("cellMSStreamGetMP3FramePos(streamNo=%d, currentSampleOffset=*0x%x, packetSampleSize=*0x%x, currentSeekAddr=*0x%x)", streamNo, currentSampleOffset, packetSampleSize, currentSeekAddr);

	return 0;
}

s32 cellMSStreamGetPriorityBank(s32 ch)
{
	cellMS.todo("cellMSStreamGetPriorityBank(ch=%d)", ch);

	return CELL_MS_PRIORITY_BANK_0;
}

s32 cellMSStreamGetPriorityBankMaxLimit(const CELL_MS_PRIORITY_BANKS priorityBank)
{
	cellMS.todo("cellMSStreamGetPriorityBankMaxLimit(priorityBank=%d)", priorityBank);

	return 0;
}

f32 cellMSStreamGetPriorityBankThreshold(const CELL_MS_PRIORITY_BANKS priorityBank)
{
	cellMS.todo("cellMSStreamGetPriorityBankThreshold(priorityBank=%d)", priorityBank);

	return 0.;
}

f32 cellMSStreamGetPriorityLevel(s32 ch)
{
	cellMS.todo("cellMSStreamGetPriorityLevel(ch=%d)", ch);

	return 0.;
}

f32 cellMSStreamGetRouting(const s32 streamNo, const s32 busNo)
{
	cellMS.todo("cellMSStreamGetRouting(streamNo=%d, busNo=%d)", streamNo, busNo);

	return 0.;
}

s32 cellMSStreamGetRoutingMask(const s32 streamNo)
{
	cellMS.todo("cellMSStreamGetRoutingMask(streamNo=%d)", streamNo);

	return 0;
}

s32 cellMSStreamGetRoutingWetDry(const s32 streamNo, const s32 busNo, vm::ptr<f32> dryGain, vm::ptr<f32> wetGain)
{
	cellMS.todo("cellMSStreamGetRoutingWetDry(streamNo=%d, busNo=%d, dryGain=*0x%x, wetGain=*0x%x)", streamNo, busNo, dryGain, wetGain);

	return CELL_OK;
}

s32 cellMSStreamGetStatus(const s32 streamNo)
{
	cellMS.todo("cellMSStreamGetStatus(streamNo=%d)", streamNo);

	return CELL_MS_STREAM_OFF;
}

s32 cellMSStreamGetSurroundInfo(s32 streamNo, vm::ptr<CellMSSurroundInfo> info)
{
	cellMS.todo("cellMSStreamGetSurroundInfo(streamNo=%d, info=*0x%x)", streamNo, info);

	return CELL_OK;
}

s32 cellMSStreamOpen()
{
	cellMS.todo("cellMSStreamOpen()");

	return CELL_OK;
}

void cellMSStreamPlay(const s32 streamNo)
{
	cellMS.todo("cellMSStreamPlay(streamNo=%d)", streamNo);
}

void cellMSStreamSetCallbackData(const s32 streamNo, vm::cptr<void> Data)
{
	cellMS.todo("cellMSStreamSetCallbackData(streamNo=%d, Data=*0x%x)", streamNo, Data);
}

void cellMSStreamSetCallbackFunc(const s32 streamNo, vm::cptr<CELL_MS_CALLBACK_FUNC> func)
{
	cellMS.todo("cellMSStreamSetCallbackFunc(streamNo=%d, func=*0x%x)", streamNo, func);
}

void cellMSStreamSetEnvBypass(const s32 streamNo, const CELL_MS_BYPASS Switch)
{
	cellMS.todo("cellMSStreamSetEnvBypass(streamNo=%d, Switch=*0x%x)", streamNo, Switch);
}

void cellMSStreamSetFirstRead(const s32 streamNo, vm::cptr<void> Addr, const s32 size)
{
	cellMS.todo("cellMSStreamSetFirstRead(streamNo=%d, Addr=*0x%x, size=%d)", streamNo, Addr, size);
}

void cellMSStreamSetFirstReadSkip(const s32 streamNo, vm::cptr<void> Addr, const s32 size, const s32 sampleSkip, const s32 reserved)
{
	cellMS.todo("cellMSStreamSetFirstReadSkip(streamNo=%d, Addr=*0x%x, size=%d, sampleSkip=%d, reserved=%d)", streamNo, Addr, size, sampleSkip, reserved);
}

s32 cellMSStreamSetInfo(const s32 StreamNo, vm::cptr<CellMSInfo> data)
{
	cellMS.todo("cellMSStreamSetInfo(StreamNo=%d, data=*0x%x)", StreamNo, data);

	return CELL_OK;
}

void cellMSStreamSetPitch(const s32 streamNo, const s32 pitch)
{
	cellMS.todo("cellMSStreamSetPitch(streamNo=%d, pitch=%d)", streamNo, pitch);
}

s32 cellMSStreamSetPriorityBank(s32 ch, const CELL_MS_PRIORITY_BANKS priorityBank)
{
	cellMS.todo("cellMSStreamSetPriorityBank(ch=%d, priorityBank=%d)", ch, priorityBank);

	return CELL_OK;
}

s32 cellMSStreamSetPriorityBankMaxLimit(const CELL_MS_PRIORITY_BANKS priorityBank, s32 limit)
{
	cellMS.todo("cellMSStreamSetPriorityBankMaxLimit(priorityBank=%d, limit=%d)", priorityBank, limit);

	return CELL_OK;
}

s32 cellMSStreamSetPriorityBankThreshold(const CELL_MS_PRIORITY_BANKS priorityBank, f32 threshold)
{
	cellMS.todo("cellMSStreamSetPriorityBankThreshold(priorityBank=%d, threshold=%f)", priorityBank, threshold);

	return CELL_OK;
}

s32 cellMSStreamSetPriorityLevel(s32 ch, f32 priorityLevel)
{
	cellMS.todo("cellMSStreamSetPriorityLevel(ch=%d, priorityLevel=%f)", ch, priorityLevel);

	return CELL_OK;
}

s32 cellMSStreamSetRouting(const s32 streamNo, const s32 busNo, f32 subGain)
{
	cellMS.todo("cellMSStreamSetRouting(streamNo=%d, busNo=%d, subGain=%f)", streamNo, busNo, subGain);

	return CELL_OK;
}

s32 cellMSStreamSetRouting8(const s32 streamNo, const s32 busNo, vm::ptr<f32> drySubGain)
{
	cellMS.todo("cellMSStreamSetRouting8(streamNo=%d, busNo=%d, drySubGain=*0x%x)", streamNo, busNo, drySubGain);

	return CELL_OK;
}

s32 cellMSStreamSetRoutingDry(const s32 streamNo, const s32 busNo, f32 drySubGain)
{
	cellMS.todo("cellMSStreamSetRoutingDry(streamNo=%d, busNo=%d, drySubGain=%f)", streamNo, busNo, drySubGain);

	return CELL_OK;
}

s32 cellMSStreamSetRoutingDry8(const s32 streamNo, const s32 busNo, vm::ptr<f32> drySubGain)
{
	cellMS.todo("cellMSStreamSetRoutingDry8(streamNo=%d, busNo=%d, drySubGain=*0x%x)", streamNo, busNo, drySubGain);

	return CELL_OK;
}

s32 cellMSStreamSetRoutingWet(const s32 streamNo, const s32 busNo, f32 wetSubGain)
{
	cellMS.todo("cellMSStreamSetRoutingWet(streamNo=%d, busNo=%d, wetSubGain=%f)", streamNo, busNo, wetSubGain);

	return CELL_OK;
}

s32 cellMSStreamSetRoutingWet8(const s32 streamNo, const s32 busNo, vm::ptr<f32> wetSubGain)
{
	cellMS.todo("cellMSStreamSetRoutingWet8(streamNo=%d, busNo=%d, wetSubGain=*0x%x)", streamNo, busNo, wetSubGain);

	return CELL_OK;
}

void cellMSStreamSetSecondRead(const s32 streamNo, vm::cptr<void> Addr, const s32 size)
{
	cellMS.todo("cellMSStreamSetSecondRead(streamNo=%d, Addr=*0x%x, size=%d)", streamNo, Addr, size);
}

void cellMSStreamSetSecondReadSkip(const s32 streamNo, vm::cptr<void> Addr, const s32 size, const s32 sampleSkip, const s32 reserved)
{
	cellMS.todo("cellMSStreamSetSecondReadSkip(streamNo=%d, Addr=*0x%x, size=%d, sampleSkip=%d, reserved=%d)", streamNo, Addr, size, sampleSkip, reserved);
}

s32 cellMSStreamSetSub(const s32 streamNo, const s32 busNo)
{
	cellMS.todo("cellMSStreamSetSub(streamNo=%d, busNo=%d)", streamNo, busNo);

	return CELL_OK;
}

s32 cellMSStreamSetSurroundInfo(s32 streamNo, vm::ptr<f32> vols, s32 pitch)
{
	cellMS.todo("cellMSStreamSetSurroundInfo(streamNo=%d, vols=*0x%x, pitch=%d)", streamNo, vols, pitch);

	return CELL_OK;
}

void cellMSStreamSetWetInGains(s32 chNo, vm::cptr<f32> ptr)
{
	cellMS.todo("cellMSStreamSetWetInGains(chNo=%d, ptr=*0x%x)", chNo, ptr);
}

s32 cellMSSulphaAdd3dShape(vm::ptr<CellMSSulpha3dObject> object)
{
	cellMS.todo("cellMSSulphaAdd3dShape(object=*0x%x)", object);

	return CELL_OK;
}

s32 cellMSSulphaAddComment(vm::cptr<char> str)
{
	cellMS.todo("cellMSSulphaAddComment(str=%s)", str);

	return CELL_OK;
}

s32 cellMSSulphaClearName(s32 ID)
{
	cellMS.todo("cellMSSulphaClearName(ID=%d)", ID);

	return CELL_OK;
}

s32 cellMSSulphaDECI3Start()
{
	cellMS.todo("cellMSSulphaDECI3Start()");

	return CELL_OK;
}

void cellMSSulphaDelete3dShape(s32 id)
{
	cellMS.todo("cellMSSulphaDelete3dShape(id=%d)", id);
}

s32 cellMSSulphaFileConnect(vm::cptr<char> fileName, vm::cptr<CellMSSulphaConfiguration> configuration)
{
	cellMS.todo("cellMSSulphaFileConnect(fileName=%s, configuration=*0x%x)", fileName, configuration);

	return CELL_OK;
}

s32 cellMSSulphaFileDisconnect()
{
	cellMS.todo("cellMSSulphaFileDisconnect()");

	return CELL_OK;
}

s32 cellMSSulphaGetStatus()
{
	cellMS.todo("cellMSSulphaGetStatus()");

	return CELL_MS_SULPHA_NOT_ENABLED;
}

s32 cellMSSulphaInit(vm::ptr<void> memoryAddr, s32 memorySize, s32 namedObjects)
{
	cellMS.todo("cellMSSulphaInit(memoryAddr=*0x%x, memorySize=%d, namedObjects=%d)", memoryAddr, memorySize, namedObjects);

	return CELL_OK;
}

s32 cellMSSulphaNameObject(s32 type, s32 location, vm::cptr<char> Name)
{
	cellMS.todo("cellMSSulphaNameObject(type=%d, location=%d, Name=%s)", type, location, Name);

	return CELL_OK;
}

s32 cellMSSulphaNameStaticSample(vm::ptr<void> location, s32 size, vm::cptr<char> Name)
{
	cellMS.todo("cellMSSulphaNameStaticSample(location=*0x%x, size,=%d, Name=%s)", location, size, Name);

	return CELL_OK;
}

s32 cellMSSulphaSetDefaultConfig(vm::ptr<CellMSSulphaConfiguration> config)
{
	cellMS.todo("cellMSSulphaSetDefaultConfig(config=*0x%x)", config);

	return CELL_OK;
}

void cellMSSulphaShutdown()
{
	cellMS.todo("cellMSSulphaShutdown()");
}

s32 cellMSSurroundActive(u32 ch, s32 flag, u32 listener, CELL_MS_PATH type)
{
	cellMS.todo("cellMSSurroundActive(ch=%d, flag=%d, listener=%d, type=%d)", ch, flag, listener, type);

	return CELL_OK;
}

s32 cellMSSurroundApplyToStreams()
{
	cellMS.todo("cellMSSurroundApplyToStreams()");

	return CELL_OK;
}

s32 cellMSSurroundClose()
{
	cellMS.todo("cellMSSurroundClose()");

	return CELL_OK;
}

s32 cellMSSurroundConfig(u32 Output, u32 Layout)
{
	cellMS.todo("cellMSSurroundConfig(Output=%d, Layout=%d)", Output, Layout);

	return CELL_OK;
}

s32 cellMSSurroundDistanceModel(u32 value)
{
	cellMS.todo("cellMSSurroundDistanceModel(value=%d)", value);

	return CELL_OK;
}

s32 cellMSSurroundDopplerFactor(f32 value)
{
	cellMS.todo("cellMSSurroundDopplerFactor(value=%f)", value);

	return CELL_OK;
}

s32 cellMSSurroundDopplerVelocity(f32 value)
{
	cellMS.todo("cellMSSurroundDopplerVelocity(value=%f)", value);

	return CELL_OK;
}

s32 cellMSSurroundGetInfoSize(s32 sourceMax, s32 listenerMax)
{
	cellMS.todo("cellMSSurroundGetInfoSize(sourceMax=%d, listenerMax=%d)", sourceMax, listenerMax);

	return CELL_OK;
}

s32 cellMSSurroundInit(vm::ptr<void> memoryHeap, s32 sourceMax, s32 listenerMax)
{
	cellMS.todo("cellMSSurroundInit(memoryHeap=*0x%x, sourceMax=%d, listenerMax=%d)", memoryHeap, sourceMax, listenerMax);

	return CELL_OK;
}

s32 cellMSSurroundInitListener(u32 listener)
{
	cellMS.todo("cellMSSurroundInitListener(listener=%d)", listener);

	return CELL_OK;
}

s32 cellMSSurroundInitSource(u32 ch)
{
	cellMS.todo("cellMSSurroundInitSource(ch=%d)", ch);

	return CELL_OK;
}

s32 cellMSSurroundListener3f(u32 listener, s32 eParam, f32 flValue1, f32 flValue2, f32 flValue3)
{
	cellMS.todo("cellMSSurroundListener3f(listener=%d, eParam=%d, flValue1=%f, listener=%f, listener=%f)", listener, eParam, flValue1, flValue2, flValue3);

	return CELL_OK;
}

s32 cellMSSurroundListenerf(u32 listener, s32 eParam, f32 flValue)
{
	cellMS.todo("cellMSSurroundListenerf(listener=%d, eParam=%d, flValue=%f)", listener, eParam, flValue);

	return CELL_OK;
}

s32 cellMSSurroundListenerfv(u32 listener, s32 eParam, vm::ptr<f32> pflValues)
{
	cellMS.todo("cellMSSurroundListenerfv(listener=%d, eParam=%d, pflValues=*0x%x)", listener, eParam, pflValues);

	return CELL_OK;
}

s32 cellMSSurroundSetAutoUpdate(s32 type)
{
	cellMS.todo("cellMSSurroundSetAutoUpdate(type=%d)", type);

	return CELL_OK;
}

s32 cellMSSurroundSetListener(u32 ch, u32 listener)
{
	cellMS.todo("cellMSSurroundSetListener(ch=%d, listener=%d)", ch, listener);

	return CELL_OK;
}

s32 cellMSSurroundSource3f(u32 ch, s32 eParam, f32 flValue1, f32 flValue2, f32 flValue3)
{
	cellMS.todo("cellMSSurroundSource3f(ch=%d, eParam=%d, flValue1=%f, flValue2=%f, flValue3=%f)", ch, eParam, flValue1, flValue2, flValue3);

	return CELL_OK;
}

s32 cellMSSurroundSourceDistanceModel(u32 ch, u32 value)
{
	cellMS.todo("cellMSSurroundSourceDistanceModel(ch=%d, value=%d)", ch, value);

	return CELL_OK;
}

s32 cellMSSurroundSourcef(u32 ch, s32 eParam, f32 flValue)
{
	cellMS.todo("cellMSSurroundSourcef(ch=%d, eParam=%d, flValue=%f)", ch, eParam, flValue);

	return CELL_OK;
}

s32 cellMSSurroundSourcefv(u32 ch, s32 eParam, vm::ptr<f32> pflValues)
{
	cellMS.todo("cellMSSurroundSourcefv(ch=%d, eParam=%d, pflValues=*0x%x)", ch, eParam, pflValues);

	return CELL_OK;
}

s32 cellMSSurroundSourceOutput(u32 ch, u32 Output)
{
	cellMS.todo("cellMSSurroundSourceOutput(ch=%d, Output=%d)", ch, Output);

	return CELL_OK;
}

s32 cellMSSurroundSourceRollOff(u32 ch, u32 points, vm::ptr<CellMSSurroundPoint> data)
{
	cellMS.todo("cellMSSurroundSourceRollOff(ch=%d, points=%d, data=*0x%x)", ch, points, data);

	return CELL_OK;
}

s32 cellMSSurroundSpeedOfSound(f32 flSpeedOfSound)
{
	cellMS.todo("cellMSSurroundSpeedOfSound(flSpeedOfSound=%f)", flSpeedOfSound);

	return CELL_OK;
}

s32 cellMSSurroundUpdate()
{
	cellMS.todo("cellMSSurroundUpdate()");

	return CELL_OK;
}

vm::ptr<void> cellMSSystemClose()
{
	cellMS.todo("cellMSSystemClose()");

	return vm::null;
}

s32 cellMSSystemConfigureLibAudio(vm::ptr<CellAudioPortParam> audioParam, vm::ptr<CellAudioPortConfig> portConfig)
{
	cellMS.todo("cellMSSystemConfigureLibAudio(audioParam=*0x%x, portConfig=*0x%x)", audioParam, portConfig);

	return CELL_OK;
}

s32 cellMSSystemConfigureSysUtil()
{
	cellMS.todo("cellMSSystemConfigureSysUtil()");

	return 2;
}

s32 cellMSSystemConfigureSysUtilEx(u32 _flags)
{
	cellMS.todo("cellMSSystemConfigureSysUtilEx(_flags=0x%x)", _flags);

	return 2;
}

void cellMSSystemFreeCallbacksNeeded()
{
	cellMS.todo("cellMSSystemFreeCallbacksNeeded()");
}

void cellMSSystemGenerateCallbacks()
{
	cellMS.todo("cellMSSystemGenerateCallbacks()");
}

vm::ptr<char> cellMSSystemGetCallbacksNeeded()
{
	cellMS.todo("cellMSSystemGetCallbacksNeeded()");

	return vm::null;
}

s32 cellMSSystemGetDebugPointer(vm::ptr<CellMSDebugInformation> dbgInfo)
{
	cellMS.todo("cellMSSystemGetDebugPointer(dbgInfo=*0x%x)", dbgInfo);

	return CELL_OK;
}

void cellMSSystemGetInfo(vm::ptr<s32> freeStreams, vm::ptr<s32> usedStreams)
{
	cellMS.todo("cellMSSystemGetInfo(freeStreams=*0x%x, usedStreams=*0x%x)", freeStreams, usedStreams);
}

s32 cellMSSystemGetLastError()
{
	cellMS.todo("cellMSSystemGetLastError()");

	return CELL_OK;
}

s32 cellMSSystemGetNeededMemorySize(vm::cptr<CellMSSystemConfig> systemConfig)
{
	cellMS.todo("cellMSSystemGetNeededMemorySize(systemConfig=*0x%x)", systemConfig);

	return CELL_OK;
}

CELL_MS_PAUSE cellMSSystemGetPause()
{
	cellMS.todo("cellMSSystemGetPause()");

	return CELL_MS_NOTPAUSED;
}

s32 cellMSSystemInitSPURS(vm::cptr<void> systemMemory, vm::cptr<CellMSSystemConfig> systemConfig, vm::cptr<void/*CellSpurs*/> spurs, vm::cptr<u8> priorities)
{
	cellMS.todo("cellMSSystemInitSPURS(systemMemory=*0x%x, systemConfig=*0x%x, spurs=*0x%x, priorities=*0x%x)", systemMemory, systemConfig, spurs, priorities);

	return CELL_OK;
}

s32 cellMSSystemInitSPUThread(vm::cptr<void> systemMemory, vm::cptr<CellMSSystemConfig> systemConfig, const s32 priority)
{
	cellMS.todo("cellMSSystemInitSPUThread(systemMemory=*0x%x, systemConfig=*0x%x, priority=%d)", systemMemory, systemConfig, priority);

	return CELL_OK;
}

void cellMSSystemSetGlobalCallbackData(vm::ptr<void> Data)
{
	cellMS.todo("cellMSSystemSetGlobalCallbackData(Data=*0x%x)", Data);
}

void cellMSSystemSetGlobalCallbackFunc(vm::cptr<CELL_MS_CALLBACK_FUNC> func)
{
	cellMS.todo("cellMSSystemSetGlobalCallbackFunc(func=*0x%x)", func);
}

void cellMSSystemSetPause(const CELL_MS_PAUSE pause)
{
	cellMS.todo("cellMSSystemSetPause(pause=%d)", pause);
}

vm::ptr<f32> cellMSSystemSignalSPU()
{
	cellMS.todo("cellMSSystemSignalSPU()");

	return vm::null;
}

DECLARE(ppu_module_manager::cellMS)("cellMS", []()
{
	REG_FUNC(cellMS, cellMSATRACClose);
	REG_FUNC(cellMS, cellMSATRACGetNeededMemorySize);
	REG_FUNC(cellMS, cellMSATRACInit);

	REG_FUNC(cellMS, cellMSCoreBypassDSP);
	REG_FUNC(cellMS, cellMSCoreGetBypass);
	REG_FUNC(cellMS, cellMSCoreGetDSPHandle);
	REG_FUNC(cellMS, cellMSCoreGetDSPParamAddr);
	REG_FUNC(cellMS, cellMSCoreGetEQ);
	REG_FUNC(cellMS, cellMSCoreGetEQEnable);
	REG_FUNC(cellMS, cellMSCoreGetMask);
	REG_FUNC(cellMS, cellMSCoreGetMasks);
	REG_FUNC(cellMS, cellMSCoreGetPause);
	REG_FUNC(cellMS, cellMSCoreGetRouting);
	REG_FUNC(cellMS, cellMSCoreGetRouting8);
	REG_FUNC(cellMS, cellMSCoreGetVolume1);
	REG_FUNC(cellMS, cellMSCoreGetVolume64);
	REG_FUNC(cellMS, cellMSCoreGetVolume8);
	REG_FUNC(cellMS, cellMSCoreGetWetInGain);
	REG_FUNC(cellMS, cellMSCoreInit);
	REG_FUNC(cellMS, cellMSCoreRoutingInit);
	REG_FUNC(cellMS, cellMSCoreSetBypass);
	REG_FUNC(cellMS, cellMSCoreSetDSP);
	REG_FUNC(cellMS, cellMSCoreSetEQ);
	REG_FUNC(cellMS, cellMSCoreSetEQEnable);
	REG_FUNC(cellMS, cellMSCoreSetMask);
	REG_FUNC(cellMS, cellMSCoreSetMasks);
	REG_FUNC(cellMS, cellMSCoreSetPause);
	REG_FUNC(cellMS, cellMSCoreSetRouting);
	REG_FUNC(cellMS, cellMSCoreSetRouting8);
	REG_FUNC(cellMS, cellMSCoreSetVolume1);
	REG_FUNC(cellMS, cellMSCoreSetVolume64);
	REG_FUNC(cellMS, cellMSCoreSetVolume8);
	REG_FUNC(cellMS, cellMSCoreSetWetInGain);
	REG_FUNC(cellMS, cellMSCoreStop);

	REG_FUNC(cellMS, cellMSDSPCalcMemoryNeededForChunks);
	REG_FUNC(cellMS, cellMSDSPCreateFFTChunks);
	REG_FUNC(cellMS, cellMSDSPLoadDSPFromFile);
	REG_FUNC(cellMS, cellMSDSPLoadDSPFromMemory);
	REG_FUNC(cellMS, cellMSDSPResetPage);

	REG_FUNC(cellMS, cellMSEnvSetup);

	REG_FUNC(cellMS, cellMSEQGet512Points);
	REG_FUNC(cellMS, cellMSEQGet512PointsMorphed);
	REG_FUNC(cellMS, cellMSEQGetFFTBand);
	REG_FUNC(cellMS, cellMSEQGetNumberOfPoints);
	REG_FUNC(cellMS, cellMSEQGetPoint);
	REG_FUNC(cellMS, cellMSEQInitMorphEQ);
	REG_FUNC(cellMS, cellMSEQOffsetPoint);
	REG_FUNC(cellMS, cellMSEQOffsetTag);
	REG_FUNC(cellMS, cellMSEQSetPoint);
	REG_FUNC(cellMS, cellMSEQSetTag);

	REG_FUNC(cellMS, cellMSFXAuxReturnGetNeededMemorySize);
	REG_FUNC(cellMS, cellMSFXAuxReturnInit);
	REG_FUNC(cellMS, cellMSFXAuxReturnSetThruMix);
	REG_FUNC(cellMS, cellMSFXAuxSendGetNeededMemorySize);
	REG_FUNC(cellMS, cellMSFXAuxSendInit);
	REG_FUNC(cellMS, cellMSFXAuxSendSet);
	REG_FUNC(cellMS, cellMSFXAuxSendSetVolumes);
	REG_FUNC(cellMS, cellMSFXCompressorInitApply);
	REG_FUNC(cellMS, cellMSFXCompressorInitCalc);
	REG_FUNC(cellMS, cellMSFXDelayGetNeededMemorySize);
	REG_FUNC(cellMS, cellMSFXDelayInit);
	REG_FUNC(cellMS, cellMSFXDelaySetParams);
	REG_FUNC(cellMS, cellMSFXDistortionGet);
	REG_FUNC(cellMS, cellMSFXDistortionInit);
	REG_FUNC(cellMS, cellMSFXDistortionSet);
	REG_FUNC(cellMS, cellMSFXEQGet);
	REG_FUNC(cellMS, cellMSFXEQInit);
	REG_FUNC(cellMS, cellMSFXEQSet);
	REG_FUNC(cellMS, cellMSFXFdnGetNeededMemorySize);
	REG_FUNC(cellMS, cellMSFXFdnInit);
	REG_FUNC(cellMS, cellMSFXFdnSetParams);
	REG_FUNC(cellMS, cellMSFXFilterGet);
	REG_FUNC(cellMS, cellMSFXFilterGetNeededMemorySize);
	REG_FUNC(cellMS, cellMSFXFilterInit);
	REG_FUNC(cellMS, cellMSFXFilterSet);
	REG_FUNC(cellMS, cellMSFXI3DL2GetNeededMemorySize);
	REG_FUNC(cellMS, cellMSFXI3DL2Init);
	REG_FUNC(cellMS, cellMSFXI3DL2SetParams);
	REG_FUNC(cellMS, cellMSFXIRDSPClearAccumBuffers);
	REG_FUNC(cellMS, cellMSFXIRDSPCreateImpulse);
	REG_FUNC(cellMS, cellMSFXIRDSPImpulseGetNeededMemorySize);
	REG_FUNC(cellMS, cellMSFXIRDSPInfoGetNeededMemorySize);
	REG_FUNC(cellMS, cellMSFXIRDSPInit);
	REG_FUNC(cellMS, cellMSFXIRDSPInitInfo);
	REG_FUNC(cellMS, cellMSFXMeterInit);
	REG_FUNC(cellMS, cellMSFXMultiCompressorApplyAddInput);
	REG_FUNC(cellMS, cellMSFXMultiCompressorApplyResetInputs);
	REG_FUNC(cellMS, cellMSFXMultiCompressorInitApply);
	REG_FUNC(cellMS, cellMSFXMultiCompressorInitCalc);
	REG_FUNC(cellMS, cellMSFXParaEQGetNeededMemorySize);
	REG_FUNC(cellMS, cellMSFXParaEQInit);
	REG_FUNC(cellMS, cellMSFXParaEQSet);
	REG_FUNC(cellMS, cellMSFXParaEQSetLFE);
	REG_FUNC(cellMS, cellMSFXPitchShiftGet);
	REG_FUNC(cellMS, cellMSFXPitchShiftGetNeededMemorySize);
	REG_FUNC(cellMS, cellMSFXPitchShiftInit);
	REG_FUNC(cellMS, cellMSFXPitchShiftSet);
	REG_FUNC(cellMS, cellMSFXReverbGetNeededMemorySize);
	REG_FUNC(cellMS, cellMSFXReverbInit);
	REG_FUNC(cellMS, cellMSFXReverbSetParams);
	REG_FUNC(cellMS, cellMSFXRingModGetAmplitude);
	REG_FUNC(cellMS, cellMSFXRingModGetFrequency);
	REG_FUNC(cellMS, cellMSFXRingModInit);
	REG_FUNC(cellMS, cellMSFXRingModSetAmplitude);
	REG_FUNC(cellMS, cellMSFXRingModSetFrequency);
	REG_FUNC(cellMS, cellMSFXSetupSlotMasks);
	REG_FUNC(cellMS, cellMSFXSoftClipInit);
	REG_FUNC(cellMS, cellMSFXSoftClipSet);
	REG_FUNC(cellMS, cellMSFXVocoderInitCarrier);
	REG_FUNC(cellMS, cellMSFXVocoderInitModulator);
	REG_FUNC(cellMS, cellMSFXVocoderSetAllBandsBypass);
	REG_FUNC(cellMS, cellMSFXVocoderSetAllEQBands);
	REG_FUNC(cellMS, cellMSFXVocoderSetBandBypass);
	REG_FUNC(cellMS, cellMSFXVocoderSetEQBand);
	REG_FUNC(cellMS, cellMSFXVocoderSetModulatorVolume);

	REG_FUNC(cellMS, cellMSMP3DecoderAddStruct);
	REG_FUNC(cellMS, cellMSMP3DecoderGetPacketInfo);
	REG_FUNC(cellMS, cellMSMP3DecoderInit);
	REG_FUNC(cellMS, cellMSMP3DecoderInitStruct);
	REG_FUNC(cellMS, cellMSMP3DecoderLibGetRequiredSize);
	REG_FUNC(cellMS, cellMSMP3DecoderProcess);
	REG_FUNC(cellMS, cellMSMP3DecoderShutdown);
	REG_FUNC(cellMS, cellMSMP3GetFrameInfo);
	REG_FUNC(cellMS, cellMSMP3GetNeededMemorySize);
	REG_FUNC(cellMS, cellMSMP3Init);
	REG_FUNC(cellMS, cellMSMP3IntegratedDecoderGetRequiredSize);
	REG_FUNC(cellMS, cellMSMP3IntegratedDecoderInit);
	REG_FUNC(cellMS, cellMSMP3IntegratedDecoderSpuDecode);

	REG_FUNC(cellMS, cellMSStreamClearRouting);
	REG_FUNC(cellMS, cellMSStreamClose);
	REG_FUNC(cellMS, cellMSStreamGetCallbackData);
	REG_FUNC(cellMS, cellMSStreamGetCallbackFunc);
	REG_FUNC(cellMS, cellMSStreamGetCurrentEnv);
	REG_FUNC(cellMS, cellMSStreamGetCurrentPlayPos);
	REG_FUNC(cellMS, cellMSStreamGetEnvBypass);
	REG_FUNC(cellMS, cellMSStreamGetInfo);
	REG_FUNC(cellMS, cellMSStreamGetMP3FrameInfo);
	REG_FUNC(cellMS, cellMSStreamGetMP3FramePos);
	REG_FUNC(cellMS, cellMSStreamGetPriorityBank);
	REG_FUNC(cellMS, cellMSStreamGetPriorityBankMaxLimit);
	REG_FUNC(cellMS, cellMSStreamGetPriorityBankThreshold);
	REG_FUNC(cellMS, cellMSStreamGetPriorityLevel);
	REG_FUNC(cellMS, cellMSStreamGetRouting);
	REG_FUNC(cellMS, cellMSStreamGetRoutingMask);
	REG_FUNC(cellMS, cellMSStreamGetRoutingWetDry);
	REG_FUNC(cellMS, cellMSStreamGetStatus);
	REG_FUNC(cellMS, cellMSStreamGetSurroundInfo);
	REG_FUNC(cellMS, cellMSStreamOpen);
	REG_FUNC(cellMS, cellMSStreamPlay);
	REG_FUNC(cellMS, cellMSStreamSetCallbackData);
	REG_FUNC(cellMS, cellMSStreamSetCallbackFunc);
	REG_FUNC(cellMS, cellMSStreamSetEnvBypass);
	REG_FUNC(cellMS, cellMSStreamSetFirstRead);
	REG_FUNC(cellMS, cellMSStreamSetFirstReadSkip);
	REG_FUNC(cellMS, cellMSStreamSetInfo);
	REG_FUNC(cellMS, cellMSStreamSetPitch);
	REG_FUNC(cellMS, cellMSStreamSetPriorityBank);
	REG_FUNC(cellMS, cellMSStreamSetPriorityBankMaxLimit);
	REG_FUNC(cellMS, cellMSStreamSetPriorityBankThreshold);
	REG_FUNC(cellMS, cellMSStreamSetPriorityLevel);
	REG_FUNC(cellMS, cellMSStreamSetRouting);
	REG_FUNC(cellMS, cellMSStreamSetRouting8);
	REG_FUNC(cellMS, cellMSStreamSetRoutingDry);
	REG_FUNC(cellMS, cellMSStreamSetRoutingDry8);
	REG_FUNC(cellMS, cellMSStreamSetRoutingWet);
	REG_FUNC(cellMS, cellMSStreamSetRoutingWet8);
	REG_FUNC(cellMS, cellMSStreamSetSecondRead);
	REG_FUNC(cellMS, cellMSStreamSetSecondReadSkip);
	REG_FUNC(cellMS, cellMSStreamSetSub);
	REG_FUNC(cellMS, cellMSStreamSetSurroundInfo);
	REG_FUNC(cellMS, cellMSStreamSetWetInGains);

	REG_FUNC(cellMS, cellMSSulphaAdd3dShape);
	REG_FUNC(cellMS, cellMSSulphaAddComment);
	REG_FUNC(cellMS, cellMSSulphaClearName);
	REG_FUNC(cellMS, cellMSSulphaDECI3Start);
	REG_FUNC(cellMS, cellMSSulphaDelete3dShape);
	REG_FUNC(cellMS, cellMSSulphaFileConnect);
	REG_FUNC(cellMS, cellMSSulphaFileDisconnect);
	REG_FUNC(cellMS, cellMSSulphaGetStatus);
	REG_FUNC(cellMS, cellMSSulphaInit);
	REG_FUNC(cellMS, cellMSSulphaNameObject);
	REG_FUNC(cellMS, cellMSSulphaNameStaticSample);
	REG_FUNC(cellMS, cellMSSulphaSetDefaultConfig);
	REG_FUNC(cellMS, cellMSSulphaShutdown);

	REG_FUNC(cellMS, cellMSSurroundActive);
	REG_FUNC(cellMS, cellMSSurroundApplyToStreams);
	REG_FUNC(cellMS, cellMSSurroundClose);
	REG_FUNC(cellMS, cellMSSurroundConfig);
	REG_FUNC(cellMS, cellMSSurroundDistanceModel);
	REG_FUNC(cellMS, cellMSSurroundDopplerFactor);
	REG_FUNC(cellMS, cellMSSurroundDopplerVelocity);
	REG_FUNC(cellMS, cellMSSurroundGetInfoSize);
	REG_FUNC(cellMS, cellMSSurroundInit);
	REG_FUNC(cellMS, cellMSSurroundInitListener);
	REG_FUNC(cellMS, cellMSSurroundInitSource);
	REG_FUNC(cellMS, cellMSSurroundListener3f);
	REG_FUNC(cellMS, cellMSSurroundListenerf);
	REG_FUNC(cellMS, cellMSSurroundListenerfv);
	REG_FUNC(cellMS, cellMSSurroundSetAutoUpdate);
	REG_FUNC(cellMS, cellMSSurroundSetListener);
	REG_FUNC(cellMS, cellMSSurroundSource3f);
	REG_FUNC(cellMS, cellMSSurroundSourceDistanceModel);
	REG_FUNC(cellMS, cellMSSurroundSourcef);
	REG_FUNC(cellMS, cellMSSurroundSourcefv);
	REG_FUNC(cellMS, cellMSSurroundSourceOutput);
	REG_FUNC(cellMS, cellMSSurroundSourceRollOff);
	REG_FUNC(cellMS, cellMSSurroundSpeedOfSound);
	REG_FUNC(cellMS, cellMSSurroundUpdate);

	REG_FUNC(cellMS, cellMSSystemClose);
	REG_FUNC(cellMS, cellMSSystemConfigureLibAudio);
	REG_FUNC(cellMS, cellMSSystemConfigureSysUtil);
	REG_FUNC(cellMS, cellMSSystemConfigureSysUtilEx);
	REG_FUNC(cellMS, cellMSSystemFreeCallbacksNeeded);
	REG_FUNC(cellMS, cellMSSystemGenerateCallbacks);
	REG_FUNC(cellMS, cellMSSystemGetCallbacksNeeded);
	REG_FUNC(cellMS, cellMSSystemGetDebugPointer);
	REG_FUNC(cellMS, cellMSSystemGetInfo);
	REG_FUNC(cellMS, cellMSSystemGetLastError);
	REG_FUNC(cellMS, cellMSSystemGetNeededMemorySize);
	REG_FUNC(cellMS, cellMSSystemGetPause);
	REG_FUNC(cellMS, cellMSSystemInitSPURS);
	REG_FUNC(cellMS, cellMSSystemInitSPUThread);

	REG_FUNC(cellMS, cellMSSystemSetGlobalCallbackData);
	REG_FUNC(cellMS, cellMSSystemSetGlobalCallbackFunc);
	REG_FUNC(cellMS, cellMSSystemSetPause);
	REG_FUNC(cellMS, cellMSSystemSignalSPU);
});
