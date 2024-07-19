#ifndef GUARD_CITYPEEP_GFX_H_
#define GUARD_CITYPEEP_GFX_H_

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

#define OT_LENGTH 1024
#define PACKET_LENGTH 32768

/* Triangle (3) mesh */
typedef struct {
	int vcount;
	SVECTOR *verts;
	int fcount;
	SVECTOR *faces;
} CP_M3;

typedef struct {
	SVECTOR rot;
	VECTOR trans;
	VECTOR scale;

	CP_M3 data;
} CP_PolyF3;

void gfxCheckRegion(void);
void gfxInit(void);

void gfxPrepare(void);
void gfxDisplay(void);

void gfxLoadM3(const char *PATH, CP_M3 *mesh3);
void gfxDrawPolyF3(CP_PolyF3 *poly);

#endif // !GUARD_CITYPEEP_GFX_H_
