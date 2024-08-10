/* Citypeep: Nine-patch */

#include <sys/types.h>

#include "ninepatch.h"
#include "gfx.h"
#include "sprite.h"

#include "common.h"

void ninePatchDraw(CP_Sprite *np, u_short x, u_short y, u_short w, u_short h) {
	const u_short LAST_X = (x + ((w - 2) * np->w));
	const u_short LAST_Y = (y + ((h - 2) * np->h));

	for( np->y = y; np->y < y + (h * np->h); np->y += np->h ) {
		for( np->x = x; np->x < x + (w * np->w); np->x += np->w ) {
			gfxDrawSprite(np);

			if( np->x == LAST_X ) {
				np->uv.u = np->baseuv.u + (np->w * 2);
			} else {
				np->uv.u = np->baseuv.u + np->w;
			}
		}

		np->uv.u = np->baseuv.u;

		if( np->y == LAST_Y ) {
			np->uv.v = np->baseuv.v + (np->h * 2);
		} else {
			np->uv.v = np->baseuv.v + np->h;
		}
	}

	gfxSetTPage(np->tpage);
	np->uv.v = np->baseuv.v;
}
