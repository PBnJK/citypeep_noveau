/* Citypeep: Entry-point */

#include <stdio.h>
#include <sys/types.h>

#include <libgte.h>
#include <libetc.h>
#include <libgpu.h>

#include "common.h"
#include "system.h"
#include "gfx.h"

int main(void) {
	gfxCheckRegion();

	LOG("=== GAME ENTERED ===\n");
	sysInit();

	gfxLoadM3("\\MDL\\CUBE.MF;1");

	while( 1 ) {
		gfxPrepare();

		// gfxDrawPolyF3(&p3);

		gfxDisplay();
	}
	return 0;
}
