/* Citypeep -- Graphics handling */

#include <stdio.h>

#include <sys/types.h>

#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>

#include "common.h"

#include "gfx.h"

DISPENV disp[2];
DRAWENV draw[2];

static u_long ot[2][OT_LENGTH] = { { 0 } };
static char primbuff[2][PACKET_LENGTH] = { 0 };

static volatile u_char activeBuffer = 0;
static char *nextPrimitive = primbuff[0];

u_char gfxInit(void) {
	LOG("* Initializing graphics:\n");

	/* This string changes depending on the system region! */
	const char *SCEE_STRING_ADRESS = (char *)0xbfc7ff52;
	if( *SCEE_STRING_ADRESS == 'E' ) {
		LOG("* * PAL detected;");

		SetVideoMode(MODE_PAL);
		gSCR_HEIGHT = 256;
		gSCR_CENTER_HEIGHT = gSCR_HEIGHT >> 1;

		disp[0].screen.y += 8;
		disp[1].screen.y += 8;
	} else {
		LOG("* * NTSC detected;");
		SetVideoMode(MODE_PAL);
	}

	/* Initialize graphics (no debugging) */
	ResetGraph(0);
	SetGraphDebug(0);

	/* Set offset & projection */
	SetGeomOffset(gSCR_CENTER_WIDTH, gSCR_CENTER_HEIGHT);
	SetGeomScreen(gSCR_CENTER_WIDTH);

	/* Initialize contents of disp & draw environments */
	SetDefDispEnv(&disp[0], 0, 0, gSCR_WIDTH, gSCR_HEIGHT);
	SetDefDispEnv(&disp[1], 0, gSCR_HEIGHT, gSCR_WIDTH, gSCR_HEIGHT);
	SetDefDrawEnv(&draw[0], 0, gSCR_HEIGHT, gSCR_WIDTH, gSCR_HEIGHT);
	SetDefDrawEnv(&draw[1], 0, 0, gSCR_WIDTH, gSCR_HEIGHT);

	/* Turn on drawing */
	SetDispMask(1);

	/* Set clear color and ensure that the background is cleared */
	setRGB0(&draw[0], 50, 60, 70);
	draw[0].isbg = 1;

	setRGB0(&draw[1], 50, 60, 70);
	draw[1].isbg = 1;

	/* Set current disp & draw environments */
	PutDispEnv(&disp[activeBuffer]);
	PutDrawEnv(&draw[activeBuffer]);

#ifdef DEBUG
	FntLoad(960, 0);
	FntOpen(0, 0, gSCR_WIDTH, gSCR_HEIGHT, 0, 256);
#endif

	return 1;
}

void gfxPrepare(void) {
	ClearOTagR(ot[activeBuffer], OT_LENGTH);

	activeBuffer ^= 1;
	nextPrimitive = primbuff[activeBuffer];
}

void gfxDisplay(void) {
	/* Wait for all drawing from previous frame to terminate
	 * We need to do this, since DrawOTag is non-blocking
	 */
	DrawSync(0);
	VSync(0);

	/* Update disp & draw environments */
	PutDispEnv(&disp[activeBuffer]);
	PutDrawEnv(&draw[activeBuffer]);

	/* Draw contents of ordering table */
	DrawOTag(&ot[activeBuffer][OT_LENGTH - 1]);
}
