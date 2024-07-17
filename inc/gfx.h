#ifndef GUARD_CITYPEEP_GFX_H_
#define GUARD_CITYPEEP_GFX_H_

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

#define OT_LENGTH 8
#define OFFSET_OT_LENGTH 14 - OT_LENGTH

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
	MATRIX mat; /* per object? */
} CP_PolyF3;

void gfxCheckRegion(void);
void gfxInit(void);

void gfxPrepare(void);
void gfxDisplay(void);

CP_M3 gfxLoadM3(const char *PATH);
void gfxDrawPolyF3(CP_PolyF3 *poly);

#endif // !GUARD_CITYPEEP_GFX_H_
