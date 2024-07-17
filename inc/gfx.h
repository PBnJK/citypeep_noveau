#ifndef GUARD_CITYPEEP_GFX_H_
#define GUARD_CITYPEEP_GFX_H_

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

#define OT_LENGTH 8
#define OFFSET_OT_LENGTH 14 - OT_LENGTH

#define PACKET_LENGTH 32768

typedef struct {
	POLY_F4 *prim; /* Poly F4 primtive */

	SVECTOR rot;
	VECTOR trans;
	VECTOR scale;

	SVECTOR data[4]; /* TEMP */
	MATRIX mat; /* per object? */
} CP_PolyF4;

void gfxCheckRegion(void);
void gfxInit(void);

void gfxPrepare(void);
void gfxDisplay(void);

void gfxDrawPolyF4(CP_PolyF4 *poly);

#endif // !GUARD_CITYPEEP_GFX_H_
