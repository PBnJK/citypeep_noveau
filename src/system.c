/* Citypeep -- System functions */

#include <stdio.h>

#include <sys/types.h>
#include <libds.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libapi.h>
#include <malloc.h>

#include "common.h"
#include "gfx.h"
#include "input.h"
#include "system.h"

static void _initCD(void) {
	if( !DsInit() ) {
		LOG("Error initializing CD!\n");
		return;
	}

	DsControl(DslNop, 0, 0);
	DsStatus();

	DsControlB(DslSetmode, (u_char *)DslModeSpeed, 0);
	VSync(3);
}

void sysInit(void) {
	LOG("Inititalizing everything...\n");

	SetVideoMode(MODE_NTSC);

	ResetCallback();

	EnterCriticalSection();
	InitHeap3((void *)0x800F8000, 0x00100000);
	ExitCriticalSection();

	gfxInit();

	_initCD();

	inputInit();

	LOG("Everything was initialized succesfully!\n");
}

u_long *sysLoadFileFromCD(const char *FILENAME) {
	DslFILE file;
	u_long *buf;

	LOG("Reading file '%s'... ", FILENAME);

	/* Search for file, if found: put "address" on file */
	if( !DsSearchFile(&file, (char *)FILENAME) ) {
		LOG("NOT FOUND!\n");
		return 0;
	}

	/* Convert size of file to number of sectors and alloc space for it */
	buf = malloc3(CALC_SECTOR_SIZE(file.size) * CD_SECTOR_SIZE);

	/* Start read to buffer */
	DsRead(&file.pos, (int)CALC_SECTOR_SIZE(file.size), buf, DslModeSpeed);

	/* Wait for read to finish (DsRead is non-blocking) */
	while( DsReadSync(0) )
		;

	LOG("Success!\n");

	return buf;
}
