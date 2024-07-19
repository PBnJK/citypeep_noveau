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
#include "malloc.h"
#include "system.h"

#include "gfx.h"

#define CLIP_LEFT 1
#define CLIP_RIGHT 2
#define CLIP_UP 4
#define CLIP_DOWN 8

DISPENV disp[2];
DRAWENV draw[2];

static u_long ot[2][OT_LENGTH] = { { 0 } };
static char primbuff[2][PACKET_LENGTH] = { 0 };

static volatile u_char activeBuffer = 0;
static char *nextPrimitive = primbuff[0];

RECT screen = { 0 };

int gteResult;

POLY_F3 *polyf3 = { 0 };

void gfxCheckRegion(void) {
	/* This string changes depending on the system region! */
	const char *SCEE_STRING_ADRESS = (char *)0xbfc7ff52;
	if( *SCEE_STRING_ADRESS == 'E' ) {
		LOG("(PAL detected)\n");

		SetVideoMode(MODE_PAL);
		gSCR_HEIGHT = 256;
		gSCR_CENTER_HEIGHT = gSCR_HEIGHT >> 1;
	} else {
		LOG("(NTSC detected)\n");
		SetVideoMode(MODE_NTSC);
	}
}

void gfxInit(void) {
	/* Initialize graphics (no debugging) */
	ResetGraph(0);
	SetGraphDebug(0);

	/* Initialize contents of disp & draw environments */
	SetDefDispEnv(&disp[0], 0, 0, gSCR_WIDTH, gSCR_HEIGHT);
	SetDefDrawEnv(&draw[0], 0, gSCR_HEIGHT, gSCR_WIDTH, gSCR_HEIGHT);
	SetDefDispEnv(&disp[1], 0, gSCR_HEIGHT, gSCR_WIDTH, gSCR_HEIGHT);
	SetDefDrawEnv(&draw[1], 0, 0, gSCR_WIDTH, gSCR_HEIGHT);

	if( GetVideoMode() == MODE_PAL ) {
		disp[0].screen.y = 16;
		disp[1].screen.y = 16;
	}

	/* Set clear color and ensure that the background is cleared */
	setRGB0(&draw[0], 50, 60, 70);
	draw[0].isbg = 1;
	draw[0].dtd = 1;

	setRGB0(&draw[1], 50, 60, 70);
	draw[1].isbg = 1;
	draw[1].dtd = 1;

	/* Set current disp & draw environments */
	PutDispEnv(&disp[activeBuffer]);
	PutDrawEnv(&draw[activeBuffer]);

	nextPrimitive = primbuff[activeBuffer];

	setRECT(&screen, 0, 0, gSCR_WIDTH, gSCR_HEIGHT);

	InitGeom();
	gte_SetGeomOffset(gSCR_CENTER_WIDTH, gSCR_CENTER_HEIGHT);
	gte_SetGeomScreen(gSCR_CENTER_WIDTH);

#ifdef DEBUG
	FntLoad(960, 0);
	FntOpen(0, 0, gSCR_WIDTH, gSCR_HEIGHT, 0, 256);
#endif

	/* Turn on drawing */
	SetDispMask(1);
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
	DrawOTag(ot[activeBuffer] + OT_LENGTH - 1);

	activeBuffer ^= 1;
	nextPrimitive = primbuff[activeBuffer];

	ClearOTagR(ot[activeBuffer], OT_LENGTH);
}

void gfxLoadM3(const char *PATH, CP_M3 *mesh3) {
	int i = 0;

	u_long *loaded = sysLoadFileFromCD(PATH);
	u_long *data = loaded;

	mesh3->vcount = *data++;
	mesh3->fcount = *data++;

	mesh3->verts = malloc3(mesh3->vcount * sizeof(*mesh3->verts));
	mesh3->faces = malloc3(mesh3->fcount * sizeof(*mesh3->faces));

	for( ; i < mesh3->vcount; ++i ) {
		mesh3->verts[i].vx = *data;
		mesh3->verts[i].vy = (*data++) >> 16;
		mesh3->verts[i].vz = *data;
		++i;

		mesh3->verts[i].vx = (*data++) >> 16;
		mesh3->verts[i].vy = *data;
		mesh3->verts[i].vz = (*data++) >> 16;
	}

	for( i = 0; i < mesh3->fcount; ++i ) {
		mesh3->faces[i].vx = *data;
		mesh3->faces[i].vy = (*data++) >> 16;
		mesh3->faces[i].vz = *data;
		++i;

		mesh3->faces[i].vx = (*data++) >> 16;
		mesh3->faces[i].vy = *data;
		mesh3->faces[i].vz = (*data++) >> 16;
	}
}

static short _testClip(short x, short y) {
	short result = 0;

	if( x < screen.x ) {
		result |= CLIP_LEFT;
	}

	if( x >= (screen.x + (screen.w - 1)) ) {
		result |= CLIP_RIGHT;
	}

	if( y < screen.y ) {
		result |= CLIP_UP;
	}

	if( y >= (screen.y + (screen.h - 1)) ) {
		result |= CLIP_DOWN;
	}

	return result;
}

static int _testTriClip(DVECTOR *v0, DVECTOR *v1, DVECTOR *v2) {
	short c[3] = { _testClip(v0->vx, v0->vy), _testClip(v1->vx, v1->vy),
		_testClip(v2->vx, v2->vy) };

	if( (c[0] & c[1]) == 0 ) {
		return 0;
	}

	if( (c[1] & c[2]) == 0 ) {
		return 0;
	}

	if( (c[2] & c[0]) == 0 ) {
		return 0;
	}

	return 1;
}

void gfxDrawPolyF3(CP_PolyF3 *poly) {
	MATRIX omtx;

	RotMatrix_gte(&poly->rot, &omtx);
	TransMatrix(&omtx, &poly->trans);
	ScaleMatrix(&omtx, &poly->scale);

	gte_SetRotMatrix(&omtx);
	gte_SetTransMatrix(&omtx);

	polyf3 = (POLY_F3 *)nextPrimitive;

	for( int i = 0; i < poly->data.fcount; ++i ) {
		gte_ldv3(&poly->data.verts[poly->data.faces[i].vx],
			&poly->data.verts[poly->data.faces[i].vy],
			&poly->data.verts[poly->data.faces[i].vz]);

		/* Start RotTransPers of triangle */
		gte_rtpt();

		gte_nclip();
		gte_stopz(&gteResult);
		if( gteResult <= 0 ) {
			continue;
		}

		gte_avsz3();
		gte_stotz(&gteResult);

		gteResult >>= 2;
		if( gteResult <= 0 || gteResult >= OT_LENGTH ) {
			continue;
		}

		gte_stsxy3(&polyf3->x0, &polyf3->x1, &polyf3->x2);
		if( _testTriClip((DVECTOR *)&polyf3->x0, (DVECTOR *)&polyf3->x1,
				(DVECTOR *)&polyf3->x2) ) {
			continue;
		}

		setPolyF3(polyf3);
		setRGB0(polyf3, i * 12, i * 10, 127);

		gte_stdp(&gteResult);
		gte_stflg(&gteResult);
		gte_stszotz(&gteResult);

		gteResult /= 3;

		if( gteResult > 0 && gteResult < OT_LENGTH ) {
			addPrim(ot[activeBuffer] + gteResult, polyf3);
		}

		++polyf3;
	}

	nextPrimitive = (char *)polyf3;
}
