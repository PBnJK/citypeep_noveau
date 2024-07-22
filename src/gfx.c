/* Citypeep: Graphics handling */

#include <stdio.h>

#include <sys/types.h>

#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>

#include <stddef.h>

#include <inline_n.h>
#include "cpu_macros.h"

#include "common.h"
#include "malloc.h"
#include "system.h"
#include "image.h"

#include "gfx.h"

#define CLIP_LEFT 1
#define CLIP_RIGHT 2
#define CLIP_UP 4
#define CLIP_DOWN 8

DISPENV disp[2];
DRAWENV draw[2];

static u_long ot[2][OT_LENGTH];
static char primbuff[2][PACKET_LENGTH];

static volatile u_char activeBuffer = 0;
static char *nextPrimitive = primbuff[0];

RECT screen;
RECT vram = { 0, 0, 1024, 512 };

int gteResult;

POLY_F3 *polyf3;
POLY_FT3 *polyft3;

static MATRIX colorMatrix = {
	ONE * 3 / 4, 0, 0, /* Red   */
	ONE * 3 / 4, 0, 0, /* Green */
	ONE * 3 / 4, 0, 0 /* Blue  */
};

static MATRIX lightMatrix = { -2048, -2048, -2048, 0, 0, 0, 0, 0, 0 };

void gfxInit(void) {
	/* Initialize graphics (no debugging) */
	ResetGraph(0);
	SetGraphDebug(0);

	/* Clear all of VRAM to black */
	ClearImage(&vram, 0, 0, 0);

	/* Initialize contents of disp & draw environments */
	SetDefDispEnv(&disp[0], 0, 0, SCR_WIDTH, SCR_HEIGHT);
	SetDefDrawEnv(&draw[0], 0, SCR_HEIGHT, SCR_WIDTH, SCR_HEIGHT);
	SetDefDispEnv(&disp[1], 0, SCR_HEIGHT, SCR_WIDTH, SCR_HEIGHT);
	SetDefDrawEnv(&draw[1], 0, 0, SCR_WIDTH, SCR_HEIGHT);

	/* Set clear color and ensure that the background is cleared */
	setRGB0(&draw[0], 32, 128, 32);
	draw[0].isbg = 1;
	draw[0].dtd = 1;

	setRGB0(&draw[1], 32, 128, 32);
	draw[1].isbg = 1;
	draw[1].dtd = 1;

	/* Set current disp & draw environments */
	PutDispEnv(&disp[activeBuffer]);
	PutDrawEnv(&draw[activeBuffer]);

	setRECT(&screen, 0, 0, SCR_WIDTH, SCR_HEIGHT);

	/* Initialize the GTE */
	InitGeom();
	gte_SetGeomOffset(SCR_CENTER_WIDTH, SCR_CENTER_HEIGHT);
	gte_SetGeomScreen(SCR_CENTER_WIDTH);

	/* Fog setup */
	gte_SetBackColor(63, 63, 63);
	gte_SetColorMatrix(&colorMatrix);

	/* Prepare first OT */
	nextPrimitive = primbuff[activeBuffer];
	ClearOTagR(ot[activeBuffer], OT_LENGTH);

#ifdef DEBUG
	FntLoad(960, 0);
	FntOpen(32, 32, SCR_WIDTH, SCR_HEIGHT, 0, 256);
#endif

	/* Turn on drawing! */
	SetDispMask(1);
}

void gfxDisplay(void) {
	/* Wait for all drawing from previous frame to terminate
	 * We need to do this because DrawOTag is non-blocking
	 */
	DrawSync(0);
	VSync(0);

#ifdef DEBUG
	FntFlush(-1);
#endif

	/* Update disp & draw environments */
	PutDispEnv(&disp[activeBuffer]);
	PutDrawEnv(&draw[activeBuffer]);

	/* Draw contents of ordering table */
	DrawOTag(ot[activeBuffer] + OT_LENGTH - 1);

	activeBuffer ^= 1;
	nextPrimitive = primbuff[activeBuffer];

	ClearOTagR(ot[activeBuffer], OT_LENGTH);
}

void gfxInitMeshF(CP_MeshF *mesh) {
	setVector(&mesh->rot, 0, 0, 0);
	setVector(&mesh->trans, 0, 0, 0);
	setVector(&mesh->scale, ONE, ONE, ONE);

	mesh->flags.visible = 1;
}

void gfxLoadMeshF(const char *PATH, CP_MeshF *mesh) {
	int i = 0;

	u_long *loaded = sysLoadFileFromCD(PATH);
	u_long *data = loaded;

	mesh->vcount = *data++;
	mesh->fcount = *data++;

	mesh->verts = malloc3(mesh->vcount * sizeof(*mesh->verts));
	mesh->faces = malloc3(mesh->fcount * sizeof(*mesh->faces));

	for( ; i < mesh->vcount; ++i ) {
		mesh->verts[i].vx = *data;
		mesh->verts[i].vy = (*data++) >> 16;
		mesh->verts[i].vz = *data;
		++i;

		mesh->verts[i].vx = (*data++) >> 16;
		mesh->verts[i].vy = *data;
		mesh->verts[i].vz = (*data++) >> 16;
	}

	for( i = 0; i < mesh->fcount; ++i ) {
		mesh->faces[i].vx = *data;
		mesh->faces[i].vy = (*data++) >> 16;
		mesh->faces[i].vz = *data;
		++i;

		mesh->faces[i].vx = (*data++) >> 16;
		mesh->faces[i].vy = *data;
		mesh->faces[i].vz = (*data++) >> 16;
	}
}

void gfxCopyMeshF(CP_MeshF *from, CP_MeshF *to) {
	copyVector(&to->rot, &from->rot);
	copyVector(&to->trans, &from->trans);
	copyVector(&to->scale, &from->scale);

	to->flags = from->flags;

	to->vcount = from->vcount;
	to->fcount = from->fcount;

	to->verts = malloc3(from->vcount * sizeof(*from->verts));
	to->verts = from->verts;

	to->faces = malloc3(from->fcount * sizeof(*from->faces));
	to->faces = from->faces;
}

void gfxInitMeshT(CP_MeshT *mesh) {
	setVector(&mesh->rot, 0, 0, 0);
	setVector(&mesh->trans, 0, 0, 0);
	setVector(&mesh->scale, ONE, ONE, ONE);

	mesh->flags.visible = 1;
}

u_int gfxLoadMeshPtrT(u_long *data, const char *TEX, CP_MeshT *mesh) {
	u_int size = 0;
	int i = 0;

	gfxInitMeshT(mesh);

	imgLoad(TEX, &mesh->tex);

	int actualW = mesh->tex.prect->w;
	switch( mesh->tex.mode & 0x3 ) {
	case 0:
		actualW <<= 1;
	case 1:
		actualW <<= 1;
	}

	mesh->tpage = getTPage(
		mesh->tex.mode & 0x3, 0, mesh->tex.prect->x, mesh->tex.prect->y);
	mesh->clut = getClut(mesh->tex.crect->x, mesh->tex.crect->y);

	mesh->vcount = *data++;
	mesh->fcount = *data++;
	mesh->tcount = *data++;
	mesh->ncount = *data++;

	size += 4;

	mesh->verts = malloc3(mesh->vcount * sizeof(*mesh->verts));
	mesh->faces = malloc3(mesh->fcount * sizeof(*mesh->faces));

	mesh->uvidxs = malloc3(mesh->fcount * sizeof(*mesh->uvidxs));
	mesh->uvs = malloc3(mesh->tcount * sizeof(*mesh->uvs));

	mesh->nidxs = malloc3(mesh->fcount * sizeof(*mesh->nidxs));
	mesh->normals = malloc3(mesh->ncount * sizeof(*mesh->normals));

	for( ; i < mesh->vcount; ++i ) {
		mesh->verts[i].vx = *data;
		mesh->verts[i].vy = (*data++) >> 16;
		mesh->verts[i].vz = *data;
		++i;

		mesh->verts[i].vx = (*data++) >> 16;
		mesh->verts[i].vy = *data;
		mesh->verts[i].vz = (*data++) >> 16;

		size += 3;
	}

	for( i = 0; i < mesh->fcount; ++i ) {
		mesh->faces[i].vx = *data;
		mesh->faces[i].vy = (*data++) >> 16;
		mesh->faces[i].vz = *data;
		++i;

		mesh->faces[i].vx = (*data++) >> 16;
		mesh->faces[i].vy = *data;
		mesh->faces[i].vz = (*data++) >> 16;

		size += 3;
	}

	for( i = 0; i < mesh->tcount; ++i ) {
		mesh->uvs[i].u = (actualW * (*data)) / 4096;
		mesh->uvs[i].v = (mesh->tex.prect->h * ((*data++) >> 16)) / 4096;

		++size;
	}

	for( i = 0; i < mesh->fcount; ++i ) {
		mesh->uvidxs[i].vx = *data;
		mesh->uvidxs[i].vy = (*data++) >> 16;
		mesh->uvidxs[i].vz = *data;
		++i;

		mesh->uvidxs[i].vx = (*data++) >> 16;
		mesh->uvidxs[i].vy = *data;
		mesh->uvidxs[i].vz = (*data++) >> 16;

		size += 3;
	}

	for( i = 0; i < mesh->ncount; ++i ) {
		mesh->normals[i].vx = *data;
		mesh->normals[i].vy = (*data++) >> 16;
		mesh->normals[i].vz = *data;
		++i;

		mesh->normals[i].vx = (*data++) >> 16;
		mesh->normals[i].vy = *data;
		mesh->normals[i].vz = (*data++) >> 16;

		size += 3;
	}

	for( i = 0; i < mesh->fcount; ++i ) {
		mesh->nidxs[i].vx = *data;
		mesh->nidxs[i].vy = (*data++) >> 16;
		mesh->nidxs[i].vz = *data;
		++i;

		mesh->nidxs[i].vx = (*data++) >> 16;
		mesh->nidxs[i].vy = *data;
		mesh->nidxs[i].vz = (*data++) >> 16;

		size += 3;
	}

	LOG("model data %d\n", *data);

	return size;
}

u_int gfxLoadMeshT(const char *PATH, const char *TEX, CP_MeshT *mesh) {
	u_long *loaded = sysLoadFileFromCD(PATH);
	return gfxLoadMeshPtrT(loaded, TEX, mesh);
}

void gfxCopyMeshT(CP_MeshT *from, CP_MeshT *to) {
	copyVector(&to->rot, &from->rot);
	copyVector(&to->trans, &from->trans);
	copyVector(&to->scale, &from->scale);

	to->flags = from->flags;

	to->tpage = from->tpage;
	to->clut = from->clut;

	to->vcount = from->vcount;
	to->fcount = from->fcount;
	to->tcount = from->tcount;
	to->ncount = from->ncount;

	to->verts = malloc3(from->vcount * sizeof(*from->verts));
	to->verts = from->verts;

	to->faces = malloc3(from->fcount * sizeof(*from->faces));
	to->faces = from->faces;

	to->uvidxs = malloc3(from->fcount * sizeof(*from->uvidxs));
	to->uvidxs = from->uvidxs;

	to->uvs = malloc3(from->tcount * sizeof(*from->uvs));
	to->uvs = from->uvs;

	to->nidxs = malloc3(from->fcount * sizeof(*from->nidxs));
	to->nidxs = from->nidxs;

	to->normals = malloc3(from->ncount * sizeof(*from->normals));
	to->normals = from->normals;
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

void gfxDrawMeshF(CP_MeshF *poly) {
	MATRIX omtx;
	int otz;

	if( !poly->flags.visible ) {
		return;
	}

	RotMatrix_gte(&poly->rot, &omtx);
	TransMatrix(&omtx, &poly->trans);
	ScaleMatrix(&omtx, &poly->scale);

	gte_SetRotMatrix(&omtx);
	gte_SetTransMatrix(&omtx);

	polyf3 = (POLY_F3 *)nextPrimitive;

	for( int i = 0; i < poly->fcount; ++i ) {
		gte_ldv3(&poly->verts[poly->faces[i].vx],
			&poly->verts[poly->faces[i].vy], &poly->verts[poly->faces[i].vz]);

		/* Start RotTransPers of triangle */
		gte_rtpt();

		gte_nclip();
		gte_stopz(&gteResult);
		if( gteResult >= 0 ) {
			continue;
		}

		gte_avsz3();
		gte_stotz(&otz);

		otz >>= 2;
		if( otz <= 0 || otz >= OT_LENGTH ) {
			continue;
		}

		gte_stsxy3(&polyf3->x0, &polyf3->x1, &polyf3->x2);
		if( _testTriClip((DVECTOR *)&polyf3->x0, (DVECTOR *)&polyf3->x1,
				(DVECTOR *)&polyf3->x2) ) {
			continue;
		}

		setPolyF3(polyf3);
		setRGB0(polyf3, i * 12, i * 10, 127);

		/* gte_stdp(&gteResult);
		gte_stflg(&gteResult); */
		gte_stszotz(&gteResult);

		gteResult /= 3;

		if( gteResult > 0 && gteResult < OT_LENGTH ) {
			addPrim(ot[activeBuffer] + otz, polyf3);
		}

		++polyf3;
	}

	nextPrimitive = (char *)polyf3;
}

void gfxDrawMeshT(CP_MeshT *poly) {
	MATRIX omtx, lmtx;
	int otz;

	if( !poly->flags.visible ) {
		return;
	}

	RotMatrix_gte(&poly->rot, &omtx);
	TransMatrix(&omtx, &poly->trans);
	ScaleMatrix(&omtx, &poly->scale);

	MulMatrix0(&lightMatrix, &omtx, &lmtx);

	gte_SetRotMatrix(&omtx);
	gte_SetTransMatrix(&omtx);

	gte_SetLightMatrix(&lmtx);

	polyft3 = (POLY_FT3 *)nextPrimitive;

	draw[0].tpage = poly->tpage;
	draw[1].tpage = poly->tpage;

	for( int i = 0; i < poly->fcount; ++i ) {
		gte_ldv3(&poly->verts[poly->faces[i].vx],
			&poly->verts[poly->faces[i].vy], &poly->verts[poly->faces[i].vz]);

		/* Start RotTransPers of triangle */
		gte_rtpt();

		gte_nclip();
		gte_stopz(&gteResult);
		if( gteResult > 0 ) {
			continue;
		}

		gte_avsz3();
		gte_stotz(&otz);

		otz >>= 2;
		if( otz <= 0 || otz >= OT_LENGTH ) {
			continue;
		}

		setPolyFT3(polyft3);

		gte_stsxy3(&polyft3->x0, &polyft3->x1, &polyft3->x2);
		if( _testTriClip((DVECTOR *)&polyft3->x0, (DVECTOR *)&polyft3->x1,
				(DVECTOR *)&polyft3->x2) ) {
			continue;
		}

		setRGB0(polyft3, 128, 128, 128);

		gte_ldrgb(&polyft3->r0);
		gte_ldv0(&poly->normals[poly->nidxs[i].vx]);

		/* gte_stdp(&gteResult); */

		gte_ncs();

		gte_strgb(&polyft3->r0);

		setUV3(polyft3, poly->uvs[poly->uvidxs[i].vx].u,
			poly->uvs[poly->uvidxs[i].vx].v, poly->uvs[poly->uvidxs[i].vy].u,
			poly->uvs[poly->uvidxs[i].vy].v, poly->uvs[poly->uvidxs[i].vz].u,
			poly->uvs[poly->uvidxs[i].vz].v);

		polyft3->tpage = poly->tpage;
		polyft3->clut = poly->clut;

		/*	gte_stflg(&gteResult); */
		gte_stszotz(&gteResult);

		gteResult /= 3;

		if( gteResult > 0 && gteResult < OT_LENGTH ) {
			addPrim(ot[activeBuffer] + otz, polyft3);
		}

		++polyft3;
	}

	nextPrimitive = (char *)polyft3;
}
