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

	actorLoad("\\ACT\\CUBOID.ACT;1", &actor);

	LOG("=== ENTERING MAIN LOOP ===\n\n");
	while( 1 ) {
		_test(&actor);

		actorDraw(&actor);

		gfxDisplay();
	}

	_exit();

	return 0;
}
