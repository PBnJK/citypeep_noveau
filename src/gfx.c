/* Citypeep -- Graphics handling */

#include <stdio.h>

#include <sys/types.h>

#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>

#include <inline_n.h>
#include "cpu_macros.h"

#include "common.h"

#include "gfx.h"

DISPENV disp[2];
DRAWENV draw[2];

static u_long ot[2][OT_LENGTH] = { { 0 } };
static char primbuff[2][PACKET_LENGTH] = { 0 };

static volatile u_char activeBuffer = 0;
static char *nextPrimitive = primbuff[0];

// D-Cache (ScratchPad) setup
#define dc_camdirp ((sshort *)getScratchAddr(0))
#define dc_ip ((uchar *)getScratchAddr(1))
#define dc_opzp ((slong *)getScratchAddr(2))
#define dc_wmatp ((MATRIX *)getScratchAddr(3))
#define dc_cmatp ((MATRIX *)getScratchAddr(9))
#define dc_sxytbl ((DVECTOR *)getScratchAddr(15))

register ulong ur0 asm("$16");
register ulong ur1 asm("$17");
register ulong ur2 asm("$18");
register ulong ur3 asm("$19");
register ulong ur4 asm("$20");
register ulong ur5 asm("$21");

void gfxCheckRegion(void) {
	/* This string changes depending on the system region! */
	const char *SCEE_STRING_ADRESS = (char *)0xbfc7ff52;
	if( *SCEE_STRING_ADRESS == 'E' ) {
		LOG("PAL detected;");

		SetVideoMode(MODE_PAL);
		gSCR_HEIGHT = 256;
		gSCR_CENTER_HEIGHT = gSCR_HEIGHT >> 1;
	} else {
		LOG("* * NTSC detected;");
		SetVideoMode(MODE_NTSC);
	}
}

void gfxInit(void) {
	/* Initialize graphics (no debugging) */
	ResetGraph(0);
	SetGraphDebug(0);

	/* Set offset & projection */
	gte_SetGeomOffset(gSCR_CENTER_WIDTH, gSCR_CENTER_HEIGHT);
	gte_SetGeomScreen(gSCR_CENTER_WIDTH);

	/* Initialize contents of disp & draw environments */
	SetDefDispEnv(&disp[0], 0, 0, gSCR_WIDTH, gSCR_HEIGHT);
	SetDefDispEnv(&disp[1], 0, gSCR_HEIGHT, gSCR_WIDTH, gSCR_HEIGHT);
	SetDefDrawEnv(&draw[0], 0, gSCR_HEIGHT, gSCR_WIDTH, gSCR_HEIGHT);
	SetDefDrawEnv(&draw[1], 0, 0, gSCR_WIDTH, gSCR_HEIGHT);

	if( GetVideoMode() == MODE_PAL ) {
		disp[0].screen.y = 16;
		disp[1].screen.y = 16;
	}

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
}

void gfxPrepare(void) {
	activeBuffer ^= 1;
	nextPrimitive = primbuff[activeBuffer];

	ClearOTagR(ot[activeBuffer], OT_LENGTH);
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

void gfxDrawPolyF3(CP_PolyF3 *poly) {
	long p, flag, otz;
	poly->prim = (POLY_F3 *)nextPrimitive;

	poly->rot.vy += 4;
	poly->rot.vz += 4;

	RotMatrix_gte(&poly->rot, &poly->mat);
	TransMatrix(&poly->mat, &poly->trans);
	ScaleMatrix(&poly->mat, &poly->scale);

	gte_SetRotMatrix(&poly->mat);
	gte_SetTransMatrix(&poly->mat);

	cpu_ldr(ur0, (u_long *)&poly->data[0].vx); /* Place vx & vy into ur0 */
	cpu_ldr(ur1, (u_long *)&poly->data[0].vz); /* Place vz & pad into ur1 */
	cpu_ldr(ur2, (u_long *)&poly->data[1].vx); /* Repeat... */
	cpu_ldr(ur3, (u_long *)&poly->data[1].vz);
	cpu_ldr(ur4, (u_long *)&poly->data[2].vx);
	cpu_ldr(ur5, (u_long *)&poly->data[2].vz);

	cpu_gted0(ur0); /* Transfer from CPU to GTE */
	cpu_gted1(ur1);
	cpu_gted2(ur2);
	cpu_gted3(ur3);
	cpu_gted4(ur4);
	cpu_gted5(ur5);

	/* Start RotTransPers of triangle */
	gte_rtpt();

	gte_nclip();
	gte_stsxy3c(&dc_sxytbl[0]);

	gte_stdp(&p);
	gte_stflg(&flag);
	gte_stszotz(&otz);

	*(unsigned long long *)&poly->prim->x0
		= *(unsigned long long *)&dc_sxytbl[0];
	*(ulong *)&poly->prim->x2 = *(ulong *)&dc_sxytbl[2];

	setPolyF3(poly->prim);
	setRGB0(poly->prim, 255, 255, 0);

	addPrim(ot[activeBuffer], poly->prim);
	nextPrimitive += sizeof(*poly->prim);
}
