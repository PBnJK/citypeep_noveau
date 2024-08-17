#ifndef GUARD_CITYPEEP_DATA_ACTOR_PLAYER_H_
#define GUARD_CITYPEEP_DATA_ACTOR_PLAYER_H_

#include "mdl_player_leg.h"

#include "actor.h"

static CP_Actor act_player = {
	.rot={0, 0, 0},
	.trans={0, 0, 0},
	.scale={ONE, ONE, ONE},

	.flags={
		.active=1,
		.visible=1
	},
	.meshCount=2,
	.mesh={
		player_leg,
		player_leg,
	},
};

static const SVECTOR R_0 = { 0, 0, 0 };
static const VECTOR T_0 = { 32, 0, 0 };
static const VECTOR S_0 = { 4096, 4096, 4096 };

static const SVECTOR R_1 = { 0, 0, 0 };
static const VECTOR T_1 = { -32, 0, 0 };
static const VECTOR S_1 = { 4096, 4096, 4096 };

#endif // !GUARD_CITYPEEP_DATA_ACTOR_PLAYER_H_
