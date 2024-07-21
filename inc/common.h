#ifndef GUARD_CITYPEEP_COMMON_H_
#define GUARD_CITYPEEP_COMMON_H_

#include <sys/types.h>

#ifdef DEBUG
#define LOG(F, ...) printf(F, ##__VA_ARGS__)
#else
#define LOG(...)
#endif // DEBUG

typedef int bool;
#define false 0
#define true 1

extern int gSCR_WIDTH;
extern int gSCR_CENTER_WIDTH;

extern int gSCR_HEIGHT;
extern int gSCR_CENTER_HEIGHT;

#endif // !GUARD_CITYPEEP_COMMON_H_
