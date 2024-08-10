/* Citypeep: Graphics handling */

#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>

#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

#include <inline_n.h>
#include "cpu_macros.h"

#include "common.h"
#include "cp_memory.h"
#include "image.h"
#include "sprite.h"
#include "system.h"

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

static int otz;
static int gteResult;

static POLY_F3 *polyf3;
static POLY_G3 *polyg3;
static POLY_FT3 *polyft3;
static POLY_GT3 *polygt3;

static SPRT *sprt;

static DR_TPAGE *dr_tpage;
static DR_STP *dr_stp;

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

	/* Turn on drawing! */
	SetDispMask(1);
}

void gfxDisplay(void) {
	/* Wait for all drawing from previous frame to terminate
	 * We need to do this because DrawOTag is non-blocking
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

void gfxInitMesh(CP_Mesh *mesh) {
	setVector(&mesh->rot, 0, 0, 0);
	setVector(&mesh->trans, 0, 0, 0);
	setVector(&mesh->scale, ONE, ONE, ONE);

	mesh->flags.visible = 1;
}

void gfxFreeMesh(CP_Mesh *mesh) {
	if( mesh->flags.textured ) {
		memFree(mesh->uvidxs);
		memFree(mesh->uvs);
	}

	memFree(mesh->verts);
	memFree(mesh->faces);
	memFree(mesh->colors);
	memFree(mesh->nidxs);
	memFree(mesh->normals);

	mesh = NULL;
}

u_int gfxLoadMeshPtr(u_long *data, const char *TEX, CP_Mesh *mesh) {
	u_int size = 4;
	int i = 0, actualW = 0;

	gfxInitMesh(mesh);

	mesh->type = *data++;

	mesh->flags.textured = (mesh->type == MT_FT3 || mesh->type == MT_GT3);
	mesh->flags.gouraud = (mesh->type == MT_G3 || mesh->type == MT_GT3);

	mesh->vcount = *data++;
	mesh->fcount = *data++;
	mesh->ncount = *data++;

	if( mesh->flags.textured ) {
		imgLoad(TEX, &mesh->tex);

		actualW = mesh->tex.prect->w;
		switch( mesh->tex.mode & 0x3 ) {
		case 0:
			actualW <<= 1;
		case 1:
			actualW <<= 1;
		}

		mesh->tpage = getTPage(
			mesh->tex.mode & 0x3, 0, mesh->tex.prect->x, mesh->tex.prect->y);
		mesh->clut = getClut(mesh->tex.crect->x, mesh->tex.crect->y);

		mesh->tcount = *data++;

		mesh->uvidxs = memAlloc(mesh->fcount * sizeof(*mesh->uvidxs));
		mesh->uvs = memAlloc(mesh->tcount * sizeof(*mesh->uvs));
		++size;
	}

	mesh->verts = memAlloc(mesh->vcount * sizeof(*mesh->verts));
	mesh->faces = memAlloc(mesh->fcount * sizeof(*mesh->faces));
	mesh->ccount = (mesh->flags.gouraud) ? mesh->vcount : mesh->fcount;

	mesh->colors = memAlloc(mesh->ccount * sizeof(*mesh->colors));

	mesh->nidxs = memAlloc(mesh->fcount * sizeof(*mesh->nidxs));
	mesh->normals = memAlloc(mesh->ncount * sizeof(*mesh->normals));

	if( mesh->flags.gouraud ) {
		for( ; i < mesh->vcount; ++i ) {
			mesh->verts[i].vx = *data;
			mesh->verts[i].vy = (*data++) >> 16;
			mesh->verts[i].vz = *data;

			mesh->colors[i].r = (*data) >> 16;
			mesh->colors[i].g = (*data++) >> 24;
			mesh->colors[i].b = *data;

			++i;

			mesh->verts[i].vx = (*data++) >> 16;
			mesh->verts[i].vy = *data;
			mesh->verts[i].vz = (*data++) >> 16;

			mesh->colors[i].r = *data;
			mesh->colors[i].g = (*data) >> 8;
			mesh->colors[i].b = (*data++) >> 16;

			size += 5;
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
	} else {
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

			mesh->colors[i].r = (*data) >> 16;
			mesh->colors[i].g = (*data++) >> 24;
			mesh->colors[i].b = *data;

			++i;

			mesh->faces[i].vx = (*data++) >> 16;
			mesh->faces[i].vy = *data;
			mesh->faces[i].vz = (*data++) >> 16;

			mesh->colors[i].r = *data;
			mesh->colors[i].g = (*data) >> 8;
			mesh->colors[i].b = (*data++) >> 16;

			size += 5;
		}
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

	if( mesh->flags.textured ) {
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
	}

	return size;
}

u_int gfxLoadMesh(const char *PATH, const char *TEX, CP_Mesh *mesh) {
	u_long *loaded = sysLoadFileFromCD(PATH);
	u_int size = gfxLoadMeshPtr(loaded, TEX, mesh);

	memFree(loaded);
	return size;
}

void gfxCopyMesh(CP_Mesh *from, CP_Mesh *to) {
	copyVector(&to->rot, &from->rot);
	copyVector(&to->trans, &from->trans);
	copyVector(&to->scale, &from->scale);

	to->flags = from->flags;
	to->type = from->type;

	to->tpage = from->tpage;
	to->clut = from->clut;

	to->vcount = from->vcount;
	to->fcount = from->fcount;
	to->ccount = from->ccount;
	to->tcount = from->tcount;
	to->ncount = from->ncount;

	to->verts = from->verts;
	to->faces = from->faces;
	to->colors = from->colors;
	to->nidxs = from->nidxs;
	to->normals = from->normals;

	if( from->flags.textured ) {
		to->uvidxs = from->uvidxs;
		to->uvs = from->uvs;
	}
}

void gfxDeepCopyMesh(CP_Mesh *from, CP_Mesh *to) {
	copyVector(&to->rot, &from->rot);
	copyVector(&to->trans, &from->trans);
	copyVector(&to->scale, &from->scale);

	to->flags = from->flags;
	to->type = from->type;

	to->tpage = from->tpage;
	to->clut = from->clut;

	to->vcount = from->vcount;
	to->verts = memAlloc(from->vcount * sizeof(*from->verts));
	cp_memcpy(to->verts, from->verts, from->vcount);

	to->fcount = from->fcount;
	to->faces = memAlloc(from->fcount * sizeof(*from->faces));
	cp_memcpy(to->faces, from->faces, from->fcount);

	to->nidxs = memAlloc(from->fcount * sizeof(*from->nidxs));
	cp_memcpy(to->nidxs, from->nidxs, from->fcount);

	to->ccount = from->ccount;
	to->colors = memAlloc(from->ccount * sizeof(*from->colors));
	cp_memcpy(to->colors, from->colors, from->ccount);

	to->ncount = from->ncount;
	to->normals = memAlloc(from->ncount * sizeof(*from->normals));
	cp_memcpy(to->normals, from->normals, from->ncount);

	if( from->flags.textured ) {
		to->uvidxs = memAlloc(from->fcount * sizeof(*from->uvidxs));
		cp_memcpy(to->uvidxs, from->uvidxs, from->fcount);

		to->tcount = from->tcount;
		to->uvs = memAlloc(from->tcount * sizeof(*from->uvs));
		cp_memcpy(to->uvs, from->uvs, from->tcount);
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

static void _drawPolyF3(CP_Mesh *poly, const u_int i) {
	u_long otc = 0;

	setPolyF3(polyf3);

	gte_stsxy3(&polyf3->x0, &polyf3->x1, &polyf3->x2);
	if( _testTriClip((DVECTOR *)&polyf3->x0, (DVECTOR *)&polyf3->x1,
			(DVECTOR *)&polyf3->x2) ) {
		return;
	}

	setRGB0(polyf3, 128, 128, 128);

	/* gte_stdp(&gteResult);
	gte_stflg(&gteResult); */

	gte_stszotz(&gteResult);
	gteResult /= 3;

	if( gteResult > 0 && gteResult < OT_LENGTH ) {
		addPrim(ot[activeBuffer] + otz, polyf3);
	}

	++polyf3;
}

static void _drawPolyG3(CP_Mesh *poly, u_int i) {
	setPolyG3(polyg3);

	gte_stsxy3(&polyg3->x0, &polyg3->x1, &polyg3->x2);
	if( _testTriClip((DVECTOR *)&polyg3->x0, (DVECTOR *)&polyg3->x1,
			(DVECTOR *)&polyg3->x2) ) {
		return;
	}

	setRGB0(polyg3, poly->colors[poly->faces[i].vx].r,
		poly->colors[poly->faces[i].vx].g, poly->colors[poly->faces[i].vx].b);
	setRGB1(polyg3, poly->colors[poly->faces[i].vy].r,
		poly->colors[poly->faces[i].vy].g, poly->colors[poly->faces[i].vy].b);
	setRGB2(polyg3, poly->colors[poly->faces[i].vz].r,
		poly->colors[poly->faces[i].vz].g, poly->colors[poly->faces[i].vz].b);

	/* gte_stdp(&gteResult);
	gte_stflg(&gteResult); */
	gte_stszotz(&gteResult);

	gteResult /= 3;

	if( gteResult > 0 && gteResult < OT_LENGTH ) {
		addPrim(ot[activeBuffer] + otz, polyg3);
	}

	++polyg3;
}

static void _drawPolyFT3(CP_Mesh *poly, const u_int i) {
	setPolyFT3(polyft3);

	gte_stsxy3(&polyft3->x0, &polyft3->x1, &polyft3->x2);
	if( _testTriClip((DVECTOR *)&polyft3->x0, (DVECTOR *)&polyft3->x1,
			(DVECTOR *)&polyft3->x2) ) {
		return;
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

static void _drawPolyGT3(CP_Mesh *poly, const u_int i) {
	setPolyGT3(polygt3);

	gte_stsxy3(&polygt3->x0, &polygt3->x1, &polygt3->x2);
	if( _testTriClip((DVECTOR *)&polygt3->x0, (DVECTOR *)&polygt3->x1,
			(DVECTOR *)&polygt3->x2) ) {
		return;
	}

	setRGB0(polygt3, 128, 128, 128);
	setRGB1(polygt3, 128, 128, 128);
	setRGB2(polygt3, 128, 128, 128);

	gte_ldrgb(&polygt3->r0);
	gte_ldv0(&poly->normals[poly->nidxs[i].vx]);

	/* gte_stdp(&gteResult); */

	gte_ncs();

	gte_strgb(&polygt3->r0);

	setUV3(polygt3, poly->uvs[poly->uvidxs[i].vx].u,
		poly->uvs[poly->uvidxs[i].vx].v, poly->uvs[poly->uvidxs[i].vy].u,
		poly->uvs[poly->uvidxs[i].vy].v, poly->uvs[poly->uvidxs[i].vz].u,
		poly->uvs[poly->uvidxs[i].vz].v);

	polygt3->tpage = poly->tpage;
	polygt3->clut = poly->clut;

	/*	gte_stflg(&gteResult); */
	gte_stszotz(&gteResult);

	gteResult /= 3;

	if( gteResult > 0 && gteResult < OT_LENGTH ) {
		addPrim(ot[activeBuffer] + otz, polygt3);
	}

	++polygt3;
}

void gfxDrawMesh(CP_Mesh *poly) {
	switch( poly->type ) {
	case MT_F3:
		polyf3 = (POLY_F3 *)nextPrimitive;
		break;
	case MT_G3:
		polyg3 = (POLY_G3 *)nextPrimitive;
		break;
	case MT_FT3:
		polyft3 = (POLY_FT3 *)nextPrimitive;
		break;
	case MT_GT3:
		polygt3 = (POLY_GT3 *)nextPrimitive;
	}

	draw[0].tpage = poly->tpage;
	draw[1].tpage = poly->tpage;

	for( u_int i = 0; i < poly->fcount; ++i ) {
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
		if( otz <= 3 || otz >= OT_LENGTH ) {
			continue;
		}

		switch( poly->type ) {
		case MT_F3:
			_drawPolyF3(poly, i);
			break;
		case MT_G3:
			_drawPolyG3(poly, i);
			break;
		case MT_FT3:
			_drawPolyFT3(poly, i);
			break;
		case MT_GT3:
			_drawPolyGT3(poly, i);
		}
	}

	switch( poly->type ) {
	case MT_F3:
		nextPrimitive = (char *)polyf3;
		break;
	case MT_G3:
		nextPrimitive = (char *)polyg3;
		break;
	case MT_FT3:
		nextPrimitive = (char *)polyft3;
		break;
	case MT_GT3:
		nextPrimitive = (char *)polygt3;
	}
}

void gfxDrawMeshNoMatrix(CP_Mesh *poly) {
	if( !poly->flags.visible ) {
		return;
	}

	MATRIX omtx, lmtx;
	int otz;

	RotMatrix_gte(&poly->rot, &omtx);
	TransMatrix(&omtx, &poly->trans);
	ScaleMatrix(&omtx, &poly->scale);

	MulMatrix0(&lightMatrix, &omtx, &lmtx);

	gte_SetRotMatrix(&omtx);
	gte_SetTransMatrix(&omtx);

	gte_SetLightMatrix(&lmtx);

	gfxDrawMesh(poly);
}

void gfxDrawMeshWithMatrix(CP_Mesh *poly, MATRIX *matrix) {
	if( !poly->flags.visible ) {
		return;
	}

	MATRIX omtx, lmtx;
	int otz;

	RotMatrix_gte(&poly->rot, &omtx);
	TransMatrix(&omtx, &poly->trans);
	ScaleMatrix(&omtx, &poly->scale);

	CompMatrixLV(&omtx, matrix, &omtx);

	MulMatrix0(&lightMatrix, &omtx, &lmtx);

	gte_SetRotMatrix(&omtx);
	gte_SetTransMatrix(&omtx);

	gte_SetLightMatrix(&lmtx);

	gfxDrawMesh(poly);
}

void gfxDrawSprite(CP_Sprite *spr) {
	sprt = (SPRT *)nextPrimitive;
	setSprt(sprt);

	setXY0(sprt, spr->x, spr->y);
	setWH(sprt, spr->w, spr->h);
	setUV0(sprt, spr->uv.u, spr->uv.v);
	setRGB0(sprt, spr->col.r, spr->col.g, spr->col.b);
	sprt->clut = spr->clut;

	addPrim(&ot[activeBuffer], sprt);

	++sprt;
	nextPrimitive = (char *)sprt;
}

void gfxDrawTile(TILE *tile) {
	setTile(tile);
	addPrim(&ot[activeBuffer], tile);

	nextPrimitive += sizeof(TILE);
}

void gfxDrawTranspTile(TILE *tile) {
	setTile(tile);
	setSemiTrans(tile, 1);
	addPrim(&ot[activeBuffer], tile);

	nextPrimitive += sizeof(TILE);
}

void gfxDrawFont(CP_Font *font, u_short x, u_short y) {
	sprt = (SPRT *)nextPrimitive;
	setSprt(sprt);

	setXY0(sprt, x, y);
	setUV0(sprt, font->uv.u, font->uv.v);
	setRGB0(sprt, 127, 127, 127);
	setWH(sprt, font->cw, font->ch);
	sprt->clut = font->clut;

	addPrim(&ot[activeBuffer], sprt);

	++sprt;
	nextPrimitive = (char *)sprt;
}

void gfxSetTPage(u_short tpage) {
	dr_tpage = (DR_TPAGE *)nextPrimitive;
	setDrawTPage(dr_tpage, 0, 1, tpage);

	addPrim(ot[activeBuffer], dr_tpage);
	++dr_tpage;

	nextPrimitive = (char *)dr_tpage;
}

void gfxSetSTP(int stp) {
	dr_stp = (DR_STP *)nextPrimitive;
	setDrawStp(dr_stp, stp);

	addPrim(ot[activeBuffer], dr_stp);
	++dr_stp;

	nextPrimitive = (char *)dr_stp;
}
