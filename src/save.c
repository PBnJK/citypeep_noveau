/* Citypeep: save system */

#include "save.h"

#include <libmcrd.h>

/* 'B'   -> Magic number
 * 'I'   -> Japan region
 * 'SLPS-03837' -> SCE product number
 */
#define MCRD_FILENAME "BISLPS-03837"
#define MCRD_GAMENAME "CityPeeps!"

/*
 * Lib. Overview 47 pdf, page 61, table 5-6
 */
typedef struct {
	char magic[2];
	char type;
	char size;
	u_short title[32];
	char pad[28];
	char clut[32];
} CP_SaveHeader;

void saveInit(void) {
	MemCardInit(0);
	MemCardStart();
}

void saveExit(void) {
	MemCardStop();
	MemCardEnd();
}
