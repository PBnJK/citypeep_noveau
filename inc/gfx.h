#ifndef GUARD_CITYPEEP_GFX_H_
#define GUARD_CITYPEEP_GFX_H_

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

#define OT_LENGTH 1024
#define PACKET_LENGTH 32768

typedef struct {
	u_char u, v;
} CP_UV;

typedef struct {
	SVECTOR rot;
	VECTOR trans;
	VECTOR scale;

	/* Array of vertices (with size) */
	int vcount;
	SVECTOR *verts;

	/* Array of indices to vertices (with size) */
	int fcount;
	SVECTOR *faces;
} CP_MeshF;

typedef struct {
	SVECTOR rot;
	VECTOR trans;
	VECTOR scale;

	/* Array of vertices (with size) */
	int vcount;
	SVECTOR *verts;

	/* Array of indices to vertices (with size) */
	int fcount;
	SVECTOR *faces;

	/* Texture */
	TIM_IMAGE tex;
	u_short tpage;
	u_short clut;

	/* Array of UV data */
	int tcount;
	SVECTOR *uvidxs;
	CP_UV *uvs;

	/* Normals data */
	int ncount;
	SVECTOR *nidxs;
	SVECTOR *normals;
} CP_MeshT;

void gfxCheckRegion(void);
void gfxInit(void);

void gfxPrepare(void);
void gfxDisplay(void);

void gfxLoadMeshF(const char *PATH, CP_MeshF *mesh);
void gfxDrawMeshF(CP_MeshF *mesh);

void gfxLoadMeshT(const char *PATH, const char *TEX, CP_MeshT *mesh);
void gfxDrawMeshT(CP_MeshT *mesh);

#endif // !GUARD_CITYPEEP_GFX_H_
