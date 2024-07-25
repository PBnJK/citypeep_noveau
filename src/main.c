/* Citypeep: Entry-point */

#include <stdio.h>
#include <sys/types.h>

#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libpad.h>

#include "anim.h"
#include "common.h"
#include "actor.h"
#include "gfx.h"
#include "input.h"
#include "system.h"

/* Mostly for dev units, but whatever */
unsigned long __ramsize = 0x00200000; /* 2MB RAM */
unsigned long __stacksize = 0x00004000; /* 16Kb Stack */

CP_Actor actor;

static void _exit(void) {
	PadStopCom();
	ResetGraph(3);
	StopCallback();
}

static void _update(void) {
	actorUpdate(&actor);
	FntPrint("ANIM CNT: %d", actor.animCounter);
}

int main(void) {
	LOG("=== GAME ENTERED ===\n\n");

	sysInit();

	actorLoad("\\ACT\\CUBOID.ACT;1", &actor);
	animLoad("\\ANI\\TEST.ANI;1", actor.anim);

	VSyncCallback(_update);

	LOG("=== ENTERING MAIN LOOP ===\n\n");
	while( 1 ) {
		actorDraw(&actor);

		if( !PAD_P1.up ) {
			actor.trans.vz += 4;
		} else if( !PAD_P1.down ) {
			actor.trans.vz -= 4;
		}

		if( !PAD_P1.left ) {
			actor.trans.vx -= 4;
		} else if( !PAD_P1.right ) {
			actor.trans.vx += 4;
		}

		gfxDisplay();
	}

	_exit();

	return 0;
}
