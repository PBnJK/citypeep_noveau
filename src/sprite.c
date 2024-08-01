/* Citypeep: handles sprites */

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

#include "image.h"
#include "sprite.h"

void sprtInitFromImage(TIM_IMAGE *tim, CP_Sprite *spr) {
	spr->tpage = getTPage(tim->mode & 0x3, 0, tim->prect->x, tim->prect->y);

	if( tim->mode & 0x8 ) {
		spr->clut = getClut(tim->crect->x, tim->crect->y);
	}

	spr->w = tim->prect->w << (2 - tim->mode & 0x3);
	spr->h = tim->prect->h;

	spr->uv.u = spr->baseuv.u = (tim->prect->x & 0x3f) << (2 - tim->mode & 0x3);
	spr->uv.v = spr->baseuv.v = tim->prect->y & 0xff;

	spr->col.r = 128;
	spr->col.g = 128;
	spr->col.b = 128;
}

void sprtInit(const char *PATH, CP_Sprite *spr) {
	TIM_IMAGE tim;
	imgLoad(PATH, &tim);

	sprtInitFromImage(&tim, spr);
}
