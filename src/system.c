/* Citypeep -- System functions */

#include <stdio.h>

#include <sys/types.h>
#include <libcd.h>
#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <malloc.h>

#include "common.h"
#include "gfx.h"
#include "system.h"

void sysInit(void) {
	LOG("Inititalizing everything...\n");

	LOG("* Inititalizing system callbacks... ");
	ResetCallback();
	LOG("Success!\n");

	LOG("* Allocating 1024KB of memory for the heap... ");
	InitHeap3((void *)0x800F8000, 0x00100000);
	LOG("Success!\n");

	LOG("* Initializing graphics...\n. ");
	gfxInit();
	LOG(". Success!\n");

	LOG("* Inititalizing the CD...\n. ");
	CdInit();
	LOG(". Success!\n");

	LOG("Everything was initialized succesfully!\n");
}

u_long *sysLoadFileFromCD(const char *FILENAME) {
	CdlFILE fsFile;
	u_long *buffer;

	LOG("Reading file '%s'...\n", FILENAME);

	/* Search for file, if found: put "address" on fsFile */
	if( !CdSearchFile(&fsFile, (char *)FILENAME) ) {
		LOG("Error! File '%s' was not found on CD.\n", FILENAME);
		return 0;
	}

	buffer = malloc3(CALC_SECTOR_SIZE(fsFile.size) * CD_SECTOR_SIZE);

	/* Set the seek target to the file's position & read! */
	CdControl(CdlSetloc, (u_char *)&fsFile.pos, 0);
	CdRead((int)CALC_SECTOR_SIZE(fsFile.size), buffer, CdlModeSpeed);

	/* Wait for read to finish (CdRead is non-blocking) */
	(CdReadSync(0, 0) == 0)
		? LOG("SUCCESS -- Read file '%s' successfully!\n\n", FILENAME)
		: LOG("FAILURE -- Failed to read file '%s' ...\n\n", FILENAME);

	LOG("done baby!\n");
	return buffer;
}
