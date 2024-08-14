#ifndef GUARD_CITYPEEP_PLAYER_H_
#define GUARD_CITYPEEP_PLAYER_H_

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

#include "actor.h"
#include "camera.h"

typedef struct {
	CP_Actor *actor;
	CP_Camera camera;
} CP_Player;

extern CP_Player player;

void playerInit(void);
void playerUpdate(void);

#endif // !GUARD_CITYPEEP_PLAYER_H_
