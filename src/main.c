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

static const char *DIALOG[] = {
	"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nullam pulvinar "
	"turpis tincidunt leo lobortis molestie. Maecenas gravida tempor mauris ac "
	"viverra. Maecenas at maximus est, laoreet tincidunt nunc.",
	""
};

static void _draw(void) {
	actorDrawAll();
	dialogueDraw();
	gfxDisplay();
}

int main(void) {
	LOG("=== GAME ENTERED ===\n\n");
	sysInit();

	dialogueStart(DIALOG);
	LOG("=== ENTERING MAIN LOOP ===\n\n");
	while( 1 ) {
		gfxDrawMeshNoMatrix(&PLAYER_PLAYER);
		_draw();
	}

	sysExit();

	return 0;
}
