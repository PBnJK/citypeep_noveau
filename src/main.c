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
#include "save.h"
#include "system.h"

/* For dev units with more RAM */
unsigned long __ramsize = 0x00200000; /* 2MB RAM */
unsigned long __stacksize = 0x00004000; /* 16Kb Stack */

static void _exit(void) {
	PadStopCom();
	saveExit();
	ResetGraph(3);
	StopCallback();
}

static void _update(void) {
	actorUpdateAll();
}

int main(void) {
	LOG("=== GAME ENTERED ===\n\n");

	sysInit();

	actorLoad("\\ACT\\CUBOID.ACT;1");
	animLoad("\\ANI\\TEST.ANI;1", gActors[0].anim);

	/* Update actors on VSync, since it's time sensitive */
	VSyncCallback(_update);

	LOG("=== ENTERING MAIN LOOP ===\n\n");
	while( 1 ) {
		actorDrawAll();

		gfxDisplay();
	}

	_exit();

	return 0;
}
