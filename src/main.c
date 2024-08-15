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

#include "mdl_player.h"

static const char *DIALOG[]
	= { "This is a really " DCMD_COLOR(
			"\xFF", "\x00", "\x00") "really" DCMD_RESET_COLOR " long line!",
		  "" };

static void _draw(void) {
	actorDrawAll();
	dialogueDraw();
	gfxDisplay();
}

int main(void) {
	LOG("=== GAME ENTERED ===\n\n");
	sysInit();

	LOG("=== ENTERING MAIN LOOP ===\n\n");

	dialogueStart(DIALOG);
	while( 1 ) {
		gfxDrawMeshNoMatrix(&PLAYER_PLAYER);
		_draw();
	}

	sysExit();

	return 0;
}
