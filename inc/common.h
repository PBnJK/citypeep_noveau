#ifndef GUARD_CITYPEEP_COMMON_H_
#define GUARD_CITYPEEP_COMMON_H_

#include <stdio.h>
#include <sys/types.h>

#ifdef DEBUG
#define LOG(F, ...) printf(F, ##__VA_ARGS__)
#else
#define LOG(...)
#endif // DEBUG

#define V_ITEMS(V) (V)->vx, (V)->vy, (V)->vz

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

#define STRINGIFY_(S) #S
#define STRINGIFY(S) STRINGIFY_(S)

#define PLAYER_NAME_SIZE 8

extern char gPlayerName[PLAYER_NAME_SIZE];

#endif // !GUARD_CITYPEEP_COMMON_H_
