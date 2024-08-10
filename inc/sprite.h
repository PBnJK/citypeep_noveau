#ifndef GUARD_CITYPEEP_SPRITE_H_
#define GUARD_CITYPEEP_SPRITE_H_

#include <sys/types.h>

#include <libgte.h>

#include "libgpu.h"
#include "math.h"

typedef struct {
	u_short tpage;
	u_short clut;
	CP_UV baseuv;
	CP_UV uv;
	u_short x, y;
	u_short w, h;
	CVECTOR col;
} CP_Sprite;

void sprtInitFromImage(TIM_IMAGE *tim, CP_Sprite *spr);
void sprtInit(const char *PATH, CP_Sprite *spr);

#endif // !GUARD_CITYPEEP_SPRITE_H_
