#ifndef GUARD_CITYPEEP_AUDIO_H_
#define GUARD_CITYPEEP_AUDIO_H_

#include <sys/types.h>

/* Note: these values are always BIG endian! */
typedef struct {
	char magic[4]; /* 'VAGp' */
	u_int version; /* Format version */
	u_int interleave;
	u_int size; /* Data size */
	u_int freq; /* Sampling frequency */
	char pad2[12];
	char name[16]; /* Name of the VAG */
} CP_VAGHeader;

void audioInit(void);
void audioExit(void);

void audioSetDefaultVoiceAttr(u_int pitch, int channel, u_int addr);

u_int audioUploadSample(void *data, u_int size);
u_int audioLoadSample(void *data, u_int channel);
u_int audioLoadAt(const char *PATH, u_int channel);

void audioFreeChannel(u_int channel);

void audioPlay(u_int channels);
void audioStop(u_int channels);

#endif // !GUARD_CITYPEEP_AUDIO_H_
