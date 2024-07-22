#ifndef GUARD_CITYPEEP_GFX_H_
#define GUARD_CITYPEEP_GFX_H_

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

#define OT_LENGTH 2048
#define PACKET_LENGTH 32768

typedef struct {
	u_char u, v;
} CP_UV;

typedef struct {
	u_int visible : 1;
} CP_MeshFlags;

typedef struct CP_MeshF {
	SVECTOR rot;
	VECTOR trans;
	VECTOR scale;

	CP_MeshFlags flags;

	/* Array of vertices (with size) */
	int vcount;
	SVECTOR *verts;

	/* Array of indices to vertices (with size) */
	int fcount;
	SVECTOR *faces;
} CP_MeshF;

typedef struct CP_MeshT {
	SVECTOR rot;
	VECTOR trans;
	VECTOR scale;

	CP_MeshFlags flags;

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

void gfxInit(void);

void gfxDisplay(void);

void gfxInitMeshF(CP_MeshF *mesh);
void gfxLoadMeshF(const char *PATH, CP_MeshF *mesh);
void gfxCopyMeshF(CP_MeshF *from, CP_MeshF *to);
void gfxDrawMeshF(CP_MeshF *mesh);

void gfxInitMeshT(CP_MeshT *mesh);
u_int gfxLoadMeshPtrT(u_long *data, const char *TEX, CP_MeshT *mesh);
u_int gfxLoadMeshT(const char *PATH, const char *TEX, CP_MeshT *mesh);
void gfxCopyMeshT(CP_MeshT *from, CP_MeshT *to);
void gfxDrawMeshT(CP_MeshT *mesh);

#endif // !GUARD_CITYPEEP_GFX_H_
