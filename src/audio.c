/* Citypeep: audio handler */

#include <sys/types.h>

#include <libsnd.h>
#include <libspu.h>

#include "common.h"
#include "audio.h"
#include "cp_memory.h"
#include "system.h"

#define S_MAX 2
#define T_MAX 3

static SpuCommonAttr cattr = { 0 };

char seqTable[SS_SEQ_TABSIZ * S_MAX * T_MAX];

void audioInit(void) {
	SsInit();

	/* Stop boot-up chime */
	SsUtSetReverbDepth(0, 0);
	SsUtReverbOff();
	SpuClearReverbWorkArea(SPU_REV_MODE_OFF);

	SsSetTableSize(seqTable, S_MAX, T_MAX);

	SsSetTickMode(SS_NOTICK);

	cattr.mask = SPU_COMMON_MVOLL | SPU_COMMON_MVOLR;
	cattr.mvol.left = 0x3FFF;
	cattr.mvol.right = 0x3FFF;
	SpuSetCommonAttr(&cattr);
}

void audioExit(void) {
	for( u_int i = 0; i < 24; ++i ) {
		audioFreeChannel(0x1L << i);
	}

	SsEnd();
	SsQuit();
}

void audioUpdate(void) {
	SsSeqCalledTbyT();
}

void audioSetDefaultVoiceAttr(u_int pitch, int channel, u_int addr) {
	SpuVoiceAttr vattr;

	vattr.mask = (SPU_VOICE_VOLL | SPU_VOICE_VOLR | SPU_VOICE_PITCH
		| SPU_VOICE_WDSA | SPU_VOICE_ADSR_AMODE | SPU_VOICE_ADSR_SMODE
		| SPU_VOICE_ADSR_RMODE | SPU_VOICE_ADSR_AR | SPU_VOICE_ADSR_DR
		| SPU_VOICE_ADSR_SR | SPU_VOICE_ADSR_RR | SPU_VOICE_ADSR_SL);
	vattr.voice = channel;
	vattr.volume.left = 0x1000;
	vattr.volume.right = 0x1000;
	vattr.pitch = pitch;
	vattr.addr = addr;
	vattr.a_mode = SPU_VOICE_LINEARIncN;

	vattr.s_mode = SPU_VOICE_LINEARIncN;
	vattr.r_mode = SPU_VOICE_LINEARDecN;

	vattr.ar = 0x0;
	vattr.dr = 0x0;
	vattr.rr = 0x0;
	vattr.sr = 0x0;
	vattr.sl = 0xF;

	SpuSetVoiceAttr(&vattr);
}

u_int audioUploadSample(void *data, u_int size) {
	/* Perform DMA transfer of VAG data to the sound buffer */
	SpuSetTransferMode(SpuTransByDMA);
	u_int vagSize = SpuWrite(data + sizeof(CP_VAGHeader), size);

	/* Write is non-blocking, wait for it to finish */
	SpuIsTransferCompleted(SPU_TRANSFER_WAIT);

	return vagSize;
}

u_int audioLoadSample(void *data, u_int channel) {
	const CP_VAGHeader *VAG_HEADER = (const CP_VAGHeader *)data;
	const u_int BE_SIZE = LTOB32(VAG_HEADER->size);
	const u_int PITCH = (LTOB32(VAG_HEADER->freq) << 12) / 44100L;

	u_int vagSpuAddr = SpuMalloc(BE_SIZE);
	SpuSetTransferStartAddr(vagSpuAddr);

	const u_int VAG_SIZE = audioUploadSample(data, BE_SIZE);
	audioSetDefaultVoiceAttr(PITCH, channel, vagSpuAddr);

	return VAG_SIZE;
}

u_int audioLoadAt(const char *PATH, u_int channel) {
	u_long *data = sysLoadFileFromCD(PATH);
	return audioLoadSample((void *)data, channel);
}

void audioFreeChannel(u_int channel) {
	u_long addr;

	SpuGetVoiceStartAddr(channel, &addr);
	if( addr >= 0x1010 ) {
		SpuFree(addr);
	}
}

void audioPlay(u_int channels) {
	SpuSetKey(SpuOn, channels);
}

void audioStop(u_int channels) {
	SpuSetKey(SpuOff, channels);
}

int audioLoadSeq(u_char *vh, u_char *vb, u_long *seq) {
	short vab = SsVabOpenHead(vh, -1);
	if( vab == -1 ) {
		LOG("=== FATAL ERROR: BAD_VH ===\n");
		LOG("Couldn't open provided VAB header");
		return -1;
	}

	vab = SsVabTransBody(vb, vab);
	if( vab == -1 ) {
		LOG("=== FATAL ERROR: BAD_VB ===\n");
		LOG("Couldn't open provided VAB body");
		return -1;
	}
	SsVabTransCompleted(SS_WAIT_COMPLETED);

	short loadedSeq = SsSeqOpen(seq, vab);

	SsSetMVol(127, 127);
	SsSeqSetVol(loadedSeq, 127, 127);

	SsSeqPlay(loadedSeq, SSPLAY_PLAY, SSPLAY_INFINITY);

	memFree(vh);
	memFree(vb);
	memFree(seq);

	return 0;
}
