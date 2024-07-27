/* Citypeep: save system */

#include "save.h"

#include <libmcrd.h>

void saveInit(void) {
	MemCardInit(0);
	MemCardStart();
}

void saveExit(void) {
	MemCardStop();
	MemCardEnd();
}
