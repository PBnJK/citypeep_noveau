#ifndef GUARD_CITYPEEP_GFX_H_
#define GUARD_CITYPEEP_GFX_H_

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

#include "math.h"
#include "sprite.h"
#include "text.h"

#define OT_PRECISION 11
#define OT_LENGTH (1 << OT_PRECISION)

#define PACKET_LENGTH 32768

#define MAX_VERTS 256
#define MAX_COLORS 128

#define MAX_FACES 128

#define MAX_UVIDXS 64
#define MAX_UVS 64

#define MAX_NIDXS 64
#define MAX_NORMALS 64

typedef enum {
	MT_F3 = 0,
	MT_G3 = 1,
	MT_FT3 = 2,
	MT_GT3 = 3,
} CP_MeshType;

typedef struct {
	u_int visible : 1;
	u_int textured : 1;
	u_int gouraud : 1;
	u_int copy : 2; /* Copy type (0 = not a copy, 1 = shallow copy, 2 = deep) */
} CP_MeshFlags;

typedef struct CP_Mesh {
	SVECTOR rot;
	VECTOR trans;
	VECTOR scale;

	CP_MeshType type;
	CP_MeshFlags flags;

	/* Array of vertices */
	int vcount;
	SVECTOR verts[MAX_VERTS];

	/* Array of colors */
	int ccount;
	CVECTOR colors[MAX_COLORS];

	/* Array of indices to vertices */
	int fcount;
	SVECTOR faces[MAX_FACES];

	/* Texture */
	TIM_IMAGE tex;
	u_short tpage;
	u_short clut;

	/* Array of UV data */
	int tcount;
	SVECTOR uvidxs[MAX_UVIDXS];
	CP_UV uvs[MAX_UVS];

	/* Normals data */
	int ncount;
	SVECTOR nidxs[MAX_NIDXS];
	SVECTOR normals[MAX_NORMALS];
} CP_Mesh;

void gfxInit(void);

void gfxDisplay(void);

void gfxInitMesh(CP_Mesh *mesh);
void gfxFreeMesh(CP_Mesh *mesh);

void gfxSetupTex(CP_Mesh *mesh);
int gfxLoadTex(CP_Mesh *mesh, const char *TEX, int *width);

u_int gfxLoadMeshPtr(u_long *data, const char *TEX, CP_Mesh *mesh);
u_int gfxLoadMesh(const char *PATH, const char *TEX, CP_Mesh *mesh);

void gfxCopyMesh(CP_Mesh *from, CP_Mesh *to);

void gfxDrawMesh(CP_Mesh *mesh);
void gfxDrawMeshNoMatrix(CP_Mesh *poly);
void gfxDrawMeshWithMatrix(CP_Mesh *poly, MATRIX *matrix);

void gfxDrawSprite(CP_Sprite *spr);
void gfxDrawTile(TILE *tile);
void gfxDrawTranspTile(TILE *tile);

void gfxDrawFont(CP_Font *font, u_short x, u_short y);

void gfxSetTPage(u_short tpage);
void gfxSetSTP(int stp);

#endif // !GUARD_CITYPEEP_GFX_H_
