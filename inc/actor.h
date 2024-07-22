#ifndef GUARD_CITYPEEP_ACTOR_H_
#define GUARD_CITYPEEP_ACTOR_H_

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

#include "gfx.h"

typedef struct {
	MATRIX mat;
	SVECTOR rot;
	VECTOR trans;
	VECTOR scale;

	u_int meshCount;
	CP_MeshT *mesh;
} CP_Actor;

void actorInit(CP_Actor *actor, const u_int MESH_COUNT);

void actorDraw(CP_Actor *actor);

#endif // !GUARD_CITYPEEP_ACTOR_H_
