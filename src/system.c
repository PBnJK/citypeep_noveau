/* Citypeep -- System functions */

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <libcd.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>

#include "common.h"
#include "gfx.h"
#include "system.h"

u_char sysInit(void) {
	LOG("Inititalizing everything...\n");

	LOG("* Inititalizing system callbacks... ");
	ResetCallback();
	LOG("Success!\n");

	LOG("* Allocating 1024KB of memory for the heap... ");
	InitHeap3((void *)0x800F8000, 0x00100000);
	LOG("Success!\n");

	LOG("* Initializing graphics... ");
	gfxInit();
	LOG("Success!\n");

	LOG("* Inititalizing the CD... ");
	CdInit();
	LOG("Success!\n");

	LOG("Everything was initialized succesfully!\n\n");

	return 1;
}
