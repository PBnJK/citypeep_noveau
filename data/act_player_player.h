#ifndef GUARD_CITYPEEP_DATA_ACTOR_PLAYER_PLAYER_H_
#define GUARD_CITYPEEP_DATA_ACTOR_PLAYER_PLAYER_H_

#include "mdl_player_leg.h"

#include "mdl_player_skirt.h"

#include "mdl_player_shirt.h"

#include "mdl_player_arm.h"

#include "actor.h"

static CP_Actor act_player_player = {
	.rot={0, 0, 0},
	.trans={0, 0, 0},
	.scale={ONE, ONE, ONE},

	.flags={
		.active=1,
		.visible=1
	},
	.meshCount=6,
	.mesh={
		player_leg,
		player_leg,
		player_skirt,
		player_shirt,
		player_arm,
		player_arm,
	},
};

static const SVECTOR R_0 = { 0, 0, 0 };
static const VECTOR T_0 = { 10, 0, 0 };
static const VECTOR S_0 = { 4096, 4096, 4096 };

static const SVECTOR R_1 = { 0, 0, 0 };
static const VECTOR T_1 = { -10, 0, 0 };
static const VECTOR S_1 = { 4096, 4096, 4096 };

static const SVECTOR R_2 = { 0, 0, 0 };
static const VECTOR T_2 = { 0, -28, 0 };
static const VECTOR S_2 = { 4096, 4096, 4096 };

static const SVECTOR R_3 = { 0, 0, 0 };
static const VECTOR T_3 = { 0, -56, 0 };
static const VECTOR S_3 = { 4096, 4096, 4096 };

static const SVECTOR R_4 = { 0, 0, 0 };
static const VECTOR T_4 = { -20, -64, 0 };
static const VECTOR S_4 = { 4096, 4096, 4096 };

static const SVECTOR R_5 = { 0, 0, 0 };
static const VECTOR T_5 = { 20, -64, 0 };
static const VECTOR S_5 = { -4096, 4096, 4096 };

#endif // !GUARD_CITYPEEP_DATA_ACTOR_PLAYER_PLAYER_H_
