/* Citypeep: Entry-point */

#include <stdio.h>
#include <sys/types.h>

#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libpad.h>

#include "common.h"
#include "gfx.h"
#include "input.h"
#include "system.h"

static void _exit(void) {
	PadStopCom();
	ResetGraph(3);
	StopCallback();
}

int main(void) {
	CP_MeshT mesh = { .rot = { 0, 0, 0, 0 },
		.trans = { 0, 0, 120, 0 },
		.scale = { ONE, ONE, ONE, 0 } };

	LOG("=== GAME ENTERED ===\n\n");

	sysInit();

	gfxLoadMeshT("\\MDL\\CUBE.MF;1", "\\MDL\\TEX.TIM;1", &mesh);

	LOG("=== ENTERING MAIN LOOP ===\n\n");
	while( 1 ) {
		if( !PAD_P1.up ) {
			mesh.rot.vx -= 16;
		}

		if( !PAD_P1.down ) {
			mesh.rot.vx += 16;
		}

		if( !PAD_P1.right ) {
			mesh.rot.vy -= 16;
		}

		if( !PAD_P1.left ) {
			mesh.rot.vy += 16;
		}

		if( !PAD_P1.l1 ) {
			mesh.rot.vz -= 16;
		}

		if( !PAD_P1.r1 ) {
			mesh.rot.vz += 16;
		}

		gfxDrawMeshT(&mesh);

		gfxDisplay();
	}

	_exit();

	return 0;
}
