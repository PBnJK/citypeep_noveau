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
#include "text.h"

/* For dev units with more RAM */
unsigned long __ramsize = 0x00200000; /* 2MB RAM */
unsigned long __stacksize = 0x00004000; /* 16Kb Stack */

static void _vsyncUpdate(void) {
	actorUpdateAll();
}

static void _draw(void) {
	actorDrawAll();
	gfxDisplay();
}

static void _exit(void) {
	PadStopCom();
	saveExit();
	ResetGraph(3);
	StopCallback();
}

int main(void) {
	CP_Font font;

	LOG("=== GAME ENTERED ===\n\n");

	sysInit();

	actorLoad("\\ACT\\CUBOID.ACT;1");
	animLoad("\\ANI\\TEST.ANI;1", gActors[0].anim);
	textInitFont(&font, "\\FNT\\BIG.TIM;1", 10, 12);

	/* Update on VSync, since it's time sensitive */
	VSyncCallback(_vsyncUpdate);

	LOG("=== ENTERING MAIN LOOP ===\n\n");
	while( 1 ) {
		textDraw(&font, 8, 8, "My arse?");
		_draw();
	}

	_exit();

	return 0;
}
