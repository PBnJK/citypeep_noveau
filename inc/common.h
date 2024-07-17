#ifndef GUARD_CITYPEEP_COMMON_H_
#define GUARD_CITYPEEP_COMMON_H_

#include <sys/types.h>

#ifdef DEBUG
#define LOG(F, ...) printf(F, ##__VA_ARGS__)
#else
#define LOG(...)
#endif // DEBUG

extern int gSCR_WIDTH;
extern int gSCR_CENTER_WIDTH;

extern int gSCR_HEIGHT;
extern int gSCR_CENTER_HEIGHT;

#endif // !GUARD_CITYPEEP_COMMON_H_
