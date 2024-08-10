/* Citypeep: Entry-point */

#include <stdio.h>
#include <sys/types.h>

#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>
#include <libpad.h>

#include "audio.h"
#include "common.h"
#include "actor.h"
#include "dialogue.h"
#include "gfx.h"
#include "input.h"
#include "system.h"
#include "text.h"

static const char *lines[] = { "Thanks everyone!", NULL };

static void _vsyncUpdate(void) {
	audioUpdate();
	dialogueUpdate();
	actorUpdateAll();
}

static void _draw(void) {
	actorDrawAll();
	dialogueDraw();
	gfxDisplay();
}

int main(void) {
	CP_Font font;
	CP_Mesh mesh;
	MATRIX omtx;

	LOG("=== GAME ENTERED ===\n\n");

	sysInit();

	gfxLoadMesh("\\MDL\\PLAYER.M;1", NULL, &mesh);
	mesh.trans.vy = 48;
	mesh.trans.vz = 120;

	/* Update on VSync, since it's time sensitive */
	VSyncCallback(_vsyncUpdate);

	dialogueStart(lines);

	LOG("=== ENTERING MAIN LOOP ===\n\n");
	while( 1 ) {
		if( PAD_P1.up ) {
			mesh.trans.vy -= 4;
		}

		if( PAD_P1.down ) {
			mesh.trans.vy += 4;
		}

		if( PAD_P1.left ) {
			mesh.trans.vz += 4;
		}

		if( PAD_P1.right ) {
			mesh.trans.vz -= 4;
		}

		gfxDrawMeshNoMatrix(&mesh);
		_draw();
	}

	sysExit();

	return 0;
}
