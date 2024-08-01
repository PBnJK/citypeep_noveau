/* Citypeep: Handles menus and dialogue */

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

#include "menu.h"
#include "ninepatch.h"
#include "text.h"

CP_Font fonts[2] = { 0 };
CP_Sprite box;

/* Initializes the menu
 * Loads global textures (fonts, dialogue box, etc) into the lower-right
 * corner of VRAM
 */
void menuInit(void) {
	textInitFont(&fonts[FNT_SMALL], "\\FNT\\SERIF.TIM;1", 8, 8);
	textInitFont(&fonts[FNT_BIG], "\\FNT\\BIG.TIM;1", 10, 12);

	sprtInit("\\SPR\\BOX.TIM;1", &box);
	box.w = 16;
	box.h = 16;
}

void menuDrawText(CP_FontType type, u_char x, u_char y, const char *TEXT) {
	textDraw(&fonts[type], x, y, TEXT);
}

void menuDrawBox(u_char x, u_char y, u_char w, u_char h) {
	ninePatchDraw(&box, x, y, w, h);
}
