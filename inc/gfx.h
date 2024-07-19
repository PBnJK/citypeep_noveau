#ifndef GUARD_CITYPEEP_GFX_H_
#define GUARD_CITYPEEP_GFX_H_

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

#define OT_LENGTH 1024
#define PACKET_LENGTH 32768

typedef struct {
	SVECTOR rot;
	VECTOR trans;
	VECTOR scale;

	int vcount;
	SVECTOR *verts;
	int fcount;
	SVECTOR *faces;
} CP_Mesh;

void gfxCheckRegion(void);
void gfxInit(void);

void gfxPrepare(void);
void gfxDisplay(void);

void gfxLoadMesh(const char *PATH, CP_Mesh *mesh);
void gfxDrawMesh(CP_Mesh *mesh);

#endif // !GUARD_CITYPEEP_GFX_H_
