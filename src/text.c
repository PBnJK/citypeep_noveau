/* Citypeep: Text-drawing utility */

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

#include "text.h"
#include "image.h"
#include "gfx.h"

void textInitFont(CP_Font *font, const char *PATH, u_char cw, u_char ch) {
	TIM_IMAGE img;
	imgLoad(PATH, &img);

	const u_char TIM_MODE = 2 - (img.mode & 0x3);

	font->tPage = getTPage(img.mode & 0x3, 0, img.prect->x, img.prect->y);

	if( img.mode & 0x8 ) { // This image has a CLUT
		font->clut = getClut(img.crect->x, img.crect->y);
	}

	setWH(font, img.prect->w << TIM_MODE, img.prect->h);

	font->color.r = 128;
	font->color.g = 128;
	font->color.b = 128;

	font->cw = cw;
	font->ch = ch;

	font->cr = font->w / font->cw;
	font->cc = font->h / font->ch;

	font->uv.u = (img.prect->x & 0x3f) << TIM_MODE;
	font->uv.v = img.prect->y & 0xff;

	font->baseUV = font->uv;
}

void textDraw(CP_Font *font, u_short x, u_short y, const char *TEXT) {
	textDrawN(font, x, y, TEXT, MAX_TEXT_SIZE);
}

void textDrawN(
	CP_Font *font, u_short x, u_short y, const char *TEXT, u_short n) {
	u_short i = 0, init_pos = x;
	char c = 0;

	font->color.r = 128;
	font->color.g = 128;
	font->color.b = 128;

	for( ; i < n; ++i ) {
		c = TEXT[i];

		if( c == '\0' ) {
			break;
		}

		if( c < 0 ) {
			switch( c ) {
			case '\x8A':
				font->color.r = TEXT[++i];
				font->color.g = TEXT[++i];
				font->color.b = TEXT[++i];
				break;
			}

			continue;
		}

		switch( c ) {
		case '\n': /* Newline */
			x = init_pos;
			y += font->ch;
			break;
		case ' ': /* Skip whitespace */
			x += font->cw;
			break;
		default:
			c -= 33;

			font->uv.u = font->cw * (c % font->cr);
			font->uv.u += font->baseUV.u;

			font->uv.v = font->ch * (c / font->cr);
			font->uv.v += font->baseUV.v;

			gfxDrawFont(font, x, y + 8);

			x += font->cw;
			break;
		}
	}

	gfxSetTPage(font->tPage);
}
