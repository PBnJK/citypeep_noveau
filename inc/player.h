#ifndef GUARD_CITYPEEP_PLAYER_H_
#define GUARD_CITYPEEP_PLAYER_H_

#include "actor.h"
#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

typedef struct {
	CP_Actor *actor;
} CP_Player;

extern CP_Player player;

void playerInit(void);

#endif // !GUARD_CITYPEEP_PLAYER_H_
