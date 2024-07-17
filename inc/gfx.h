#ifndef GUARD_CITYPEEP_GFX_H_
#define GUARD_CITYPEEP_GFX_H_

#include <sys/types.h>

#define OT_LENGTH 8
#define OFFSET_OT_LENGTH 14 - OT_LENGTH

#define PACKET_LENGTH 32768

u_char gfxInit(void);

void gfxPrepare(void);
void gfxDisplay(void);

#endif // !GUARD_CITYPEEP_GFX_H_
