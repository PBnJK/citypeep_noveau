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
	LOG("=== GAME ENTERED ===\n");

	sysInit();

	VSyncCallback(gfxPrepare);

	while( 1 ) {
		gfxDisplay();
	}
	return 0;
}
