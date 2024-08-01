#ifndef GUARD_CITYPEEP_TEXT_H_
#define GUARD_CITYPEEP_TEXT_H_

#include <sys/types.h>

#include "math.h"

#define MAX_TEXT_SIZE 256

typedef struct {
	u_short tPage;
	u_short clut;
	CP_UV uv;
	CP_UV baseUV;
	u_char w, h;
	u_char cw, ch; /* Character width/height */
	u_char cr, cc; /* Characters per row/column */
} CP_Font;

void textInitFont(CP_Font *font, const char *PATH, u_char cw, u_char ch);

void textDraw(CP_Font *font, u_char x, u_char y, const char *TEXT);

#endif // !GUARD_CITYPEEP_TEXT_H_
