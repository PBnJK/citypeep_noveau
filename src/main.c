/* Citypeep: Entry-point */

#include <sys/types.h>

#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libpad.h>

#include "common.h"
#include "actor.h"
#include "dialogue.h"
#include "gfx.h"
#include "system.h"

static void _draw(void) {
	actorDrawAll();
	dialogueDraw();
	gfxDisplay();
}

int main(void) {
	LOG("=== GAME ENTERED ===\n\n");
	sysInit();

	LOG("=== ENTERING MAIN LOOP ===\n\n");
	while( 1 ) {
		_draw();
	}

	sysExit();

	return 0;
}
