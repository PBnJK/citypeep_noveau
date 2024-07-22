/* Citypeep: Entry-point */

#include <stdio.h>
#include <sys/types.h>

#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libpad.h>

#include "common.h"
#include "actor.h"
#include "gfx.h"
#include "system.h"

static void _exit(void) {
	PadStopCom();
	ResetGraph(3);
	StopCallback();
}

static void _test(CP_Actor *actor) {
	static int acnt = 0;

	++acnt;
}

int main(void) {
	CP_Actor actor;

	LOG("=== GAME ENTERED ===\n\n");

	sysInit();

	actorInit(&actor, 3);

	gfxLoadMeshT("\\MDL\\CUBE.MF;1", "\\MDL\\TEX.TIM;1", &actor.mesh[0]);
	actor.mesh[0].trans.vz = 120;

	gfxCopyMeshT(&actor.mesh[0], &actor.mesh[1]);
	gfxCopyMeshT(&actor.mesh[0], &actor.mesh[2]);

	actor.mesh[1].trans.vx -= 48;
	actor.mesh[2].trans.vx += 48;

	LOG("=== ENTERING MAIN LOOP ===\n\n");
	while( 1 ) {
		_test(&actor);

		actorDraw(&actor);

		gfxDisplay();
	}

	_exit();

	return 0;
}
