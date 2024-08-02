/* Citypeep: Nine-patch */

#include <sys/types.h>

#include "ninepatch.h"
#include "gfx.h"
#include "sprite.h"

void ninePatchDraw(CP_Sprite *np, u_char x, u_char y, u_char w, u_char h) {
	u_char px = 0;
	u_char py = 0;

	const u_char LAST_X = (x + (w * np->w)) - (np->w * 2);
	const u_char LAST_Y = (y + (h * np->h)) - (np->h * 2);

	for( u_char cy = y; cy < y + (h * np->h); cy += np->w ) {
		for( u_char cx = x; cx < x + (w * np->w); cx += np->h ) {
			np->x = cx;
			np->y = cy;

			gfxDrawSprite(np);

			if( cx == LAST_X ) {
				np->uv.u = np->baseuv.u + (np->w * 2);
			} else {
				np->uv.u = np->baseuv.u + np->w;
			}
		}

		np->uv.u = np->baseuv.u;

		if( cy == LAST_Y ) {
			np->uv.v = np->baseuv.v + (np->h * 2);
		} else {
			np->uv.v = np->baseuv.v + np->h;
		}
	}

	gfxSetTPage(np->tpage);
	np->uv.v = np->baseuv.v;
}
