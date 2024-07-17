#ifndef GUARD_CITYPEEP_SYSTEM_H_
#define GUARD_CITYPEEP_SYSTEM_H_

#include <sys/types.h>

#define CD_SECTOR_SIZE 2048
#define CALC_SECTOR_SIZE(LEN) (((LEN) + CD_SECTOR_SIZE - 1) / CD_SECTOR_SIZE)

void sysInit(void);

u_long *sysLoadFileFromCD(const char *FILENAME);

#endif // !GUARD_CITYPEEP_SYSTEM_H_
