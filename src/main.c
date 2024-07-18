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
		.trans = { 0, 0, 196, 0 },
		.scale = { ONE, ONE, ONE, 0 },
		.mat = { 0 } };

	LOG("* Inititalizing system callbacks... ");
	ResetCallback();
	LOG("Success!\n");

	gfxCheckRegion();

	LOG("=== GAME ENTERED ===\n");
	sysInit();

	gfxLoadM3("\\MDL\\CUBE.MF;1", &polyf3.data);
	while( 1 ) {
		gfxDrawPolyF3(&polyf3);

		gfxDisplay();
	}

	StopCallback();
	ResetGraph(3);

	return 0;
}
