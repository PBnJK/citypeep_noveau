/* Citypeep: Handles menus and dialogue */

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

#include "menu.h"
#include "gfx.h"
#include "sprite.h"
#include "text.h"

CP_Font fonts[2] = { 0 };
CP_Sprite icon;

TILE tile;

/* Initializes the menu
 * Loads global textures (fonts, dialogue box, etc) into the lower-right
 * corner of VRAM
 */
void menuInit(void) {
	textInitFont(&fonts[FNT_SMALL], "\\FNT\\SERIF.TIM;1", 8, 8);
	textInitFont(&fonts[FNT_BIG], "\\FNT\\BIG.TIM;1", 10, 12);

	sprtInit("\\SPR\\DICON.TIM;1", &icon);
	icon.w = 16;
	icon.h = 16;

	tile.r0 = 0;
	tile.g0 = 0;
	tile.b0 = 0;
}

void menuDrawText(CP_FontType type, u_char x, u_char y, const char *TEXT) {
	textDraw(&fonts[type], x, y, TEXT);
}

void menuDrawTextN(
	CP_FontType type, u_char x, u_char y, const char *TEXT, u_short n) {
	textDrawN(&fonts[type], x, y, TEXT, n);
}

void menuDrawBox(u_char x, u_char y, u_char w, u_char h) {
	setXY0(&tile, x, y);
	setWH(&tile, w << 3, h << 3);

	gfxSetTPage(64);
	gfxDrawTranspTile(&tile);
}
