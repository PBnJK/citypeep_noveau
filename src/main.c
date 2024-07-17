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
	CP_PolyF3 polyf3 = { .rot = { 0, 0, 0, 0 },
		.trans = { 0, 0, 0, 0 },
		.scale = { ONE >> 1, ONE >> 1, ONE >> 1, 0 },
		.mat = { 0 } };

	gfxCheckRegion();

	LOG("=== GAME ENTERED ===\n");
	sysInit();

	polyf3.trans.vz = gSCR_CENTER_WIDTH;

	polyf3.data = gfxLoadM3("\\MDL\\CUBE.MF;1");

	while( 1 ) {

		gfxPrepare();

		gfxDrawPolyF3(&polyf3);

		gfxDisplay();
	}
	return 0;
}
