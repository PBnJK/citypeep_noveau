#ifndef GUARD_CITYPEEP_COMMON_H_
#define GUARD_CITYPEEP_COMMON_H_

#include <stdio.h>
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

/* Swap endianness (Little -> Big) */
#define LTOB32(x)                                                              \
	(((x) >> 24) | (((x) >> 8) & 0xFF00) | (((x) << 8) & 0x00FF0000)           \
		| ((x) << 24))

void *cp_memcpy(void *restrict dst, const void *restrict src, size_t cnt);
size_t cp_strlen(const char *STR);

#endif // !GUARD_CITYPEEP_COMMON_H_
