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
#include "gfx.h"
#include "input.h"
#include "menu.h"
#include "system.h"
#include "text.h"

/* For dev units with more RAM */
unsigned long __ramsize = 0x00200000; /* 2MB RAM */
unsigned long __stacksize = 0x00004000; /* 16Kb Stack */

static void _vsyncUpdate(void) {
	audioUpdate();
	actorUpdateAll();
}

static void _draw(void) {
	actorDrawAll();
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

	u_long *vh = sysLoadFileFromCD("\\AUD\\TEST.VH;1");
	u_long *vb = sysLoadFileFromCD("\\AUD\\TEST.VB;1");
	u_long *sq = sysLoadFileFromCD("\\AUD\\TEST.SEQ;1");

	audioLoadSeq((u_char *)vh, (u_char *)vb, sq);

	/* Update on VSync, since it's time sensitive */
	VSyncCallback(_vsyncUpdate);

	LOG("=== ENTERING MAIN LOOP ===\n\n");
	while( 1 ) {
		menuDrawText(FNT_SMALL, 0, 0, "Cool beans\n");
		menuDrawBox(32, 8, 6, 8);

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
