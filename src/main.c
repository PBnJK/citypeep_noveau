/* Citypeep: Entry-point */

#include <stdio.h>
#include <sys/types.h>

#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>

#include "common.h"
#include "system.h"
#include "gfx.h"

int main(void) {
	CP_PolyF3 p3 = { .prim = 0,
		.rot = { 0, 0, 0 },
		.trans = { 0, 0, 0, 0 },
		.scale = { ONE, ONE, ONE },
		.data = { { 0, -32, 0, 0 }, { 32, 0, 0, 0 }, { -32, -0, 0, 0 } },
		.mat = { 0 } };

	gfxCheckRegion();

	LOG("=== GAME ENTERED ===\n");
	sysInit();
	p3.trans.vz = gSCR_CENTER_WIDTH;

	while( 1 ) {
		gfxPrepare();

		FntPrint("\n\nHello poly!");
		FntFlush(-1);

		gfxDrawPolyF3(&p3);

		gfxDisplay();
	}
	return 0;
}
