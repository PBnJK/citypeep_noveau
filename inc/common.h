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

#define SCR_WIDTH 320
#define SCR_CENTER_WIDTH (SCR_WIDTH / 2)

#define SCR_HEIGHT 240
#define SCR_CENTER_HEIGHT (SCR_HEIGHT / 2)

#endif // !GUARD_CITYPEEP_COMMON_H_
