/* Citypeep -- System functions */

#include <stdio.h>
#include <sys/types.h>

#include <libds.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <libapi.h>
#include <libpad.h>
#include <malloc.h>

#include "common.h"
#include "audio.h"
#include "gfx.h"
#include "input.h"
#include "menu.h"
#include "save.h"
#include "system.h"
#include "cp_memory.h"

static void cdInit(void) {
	if( !DsInit() ) {
		LOG("Error initializing CD!\n");
		return;
	}

	DsSetDebug(0);
	DsControl(DslNop, 0, 0);
	DsStatus();

	DsControlB(DslSetmode, (u_char *)DslModeSpeed, 0);
	VSync(3);
}

void sysInit(void) {
	LOG("Inititalizing everything...\n");

	SetVideoMode(MODE_NTSC);

	ResetCallback();

	gfxInit();
	inputInit();
	saveInit();
	memInit();
	cdInit();
	audioInit();
	menuInit();

	LOG("Everything was initialized succesfully!\n\n");
}

void sysExit(void) {
	PadStopCom();

	saveExit();
	audioExit();

	ResetGraph(3);
	StopCallback();
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
	buf = memAlloc(CALC_SECTOR_SIZE(file.size) * CD_SECTOR_SIZE);

	/* Start read to buffer */
	DsRead(&file.pos, (int)CALC_SECTOR_SIZE(file.size), buf, DslModeSpeed);

	/* Wait for read to finish (DsRead is non-blocking) */
	while( DsReadSync(0) )
		;

	LOG("Success!\n");

	return buf;
}
