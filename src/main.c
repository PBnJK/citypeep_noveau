/* Citypeep: Entry-point */

#include <stdio.h>
#include <sys/types.h>

#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libpad.h>

#include "anim.h"
#include "audio.h"
#include "common.h"
#include "actor.h"
#include "gfx.h"
#include "menu.h"
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
	u_int c = 240;
	CP_Font font;

	LOG("=== GAME ENTERED ===\n\n");

	sysInit();

	actorLoad("\\ACT\\CUBOID.ACT;1");
	animLoad("\\ANI\\TEST.ANI;1", gActors[0].anim);
	audioLoadAt("\\AUD\\SKR.VAG;1", 1);

	/* Update on VSync, since it's time sensitive */
	VSyncCallback(_vsyncUpdate);

	LOG("=== ENTERING MAIN LOOP ===\n\n");
	while( 1 ) {
		if( c == 0 ) {
			audioPlay(1);
			c = 180;
		}

		menuDrawText(FNT_SMALL, 0, 0, "Cool beans\n");
		menuDrawBox(32, 8, 6, 8);
		_draw();

		--c;
	}

	_exit();

	return 0;
}
