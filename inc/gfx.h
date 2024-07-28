#ifndef GUARD_CITYPEEP_GFX_H_
#define GUARD_CITYPEEP_GFX_H_

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

#define OT_LENGTH 2048
#define PACKET_LENGTH 32768

typedef struct {
	u_char u;
	u_char v;
} CP_UV;

typedef enum {
	MT_F3,
	MT_G3,
	MT_FT3,
	MT_GT3,
} CP_MeshType;

typedef struct {
	u_int visible : 1;
} CP_MeshFlags;

typedef struct CP_Mesh {
	SVECTOR rot;
	VECTOR trans;
	VECTOR scale;

	CVECTOR color;

	CP_MeshType type;
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
} CP_Mesh;

void gfxInit(void);

void gfxDisplay(void);

void gfxInitMesh(CP_Mesh *mesh);
u_int gfxLoadMeshPtr(u_long *data, const char *TEX, CP_Mesh *mesh);
u_int gfxLoadMesh(const char *PATH, const char *TEX, CP_Mesh *mesh);
void gfxCopyMesh(CP_Mesh *from, CP_Mesh *to);

void gfxDrawMesh(CP_Mesh *mesh);
void gfxDrawMeshNoMatrix(CP_Mesh *poly);
void gfxDrawMeshWithMatrix(CP_Mesh *poly, MATRIX *matrix);

#endif // !GUARD_CITYPEEP_GFX_H_
