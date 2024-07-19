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
	CP_Mesh mesh = { .rot = { 0, 0, 0, 0 },
		.trans = { 0, 0, 120, 0 },
		.scale = { ONE, ONE, ONE, 0 } };

	LOG("=== GAME ENTERED ===\n\n");

	LOG("* Inititalizing system callbacks... ");
	ResetCallback();
	LOG("Success!\n");

	gfxCheckRegion();

	sysInit();

	gfxLoadMesh("\\MDL\\CUBE.MF;1", &mesh);

	LOG("=== ENTERING MAIN LOOP ===\n\n");
	while( 1 ) {
		gfxDrawMesh(&mesh);

		gfxDisplay();
	}

	StopCallback();
	ResetGraph(3);

	return 0;
}
