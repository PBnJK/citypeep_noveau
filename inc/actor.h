#ifndef GUARD_CITYPEEP_ACTOR_H_
#define GUARD_CITYPEEP_ACTOR_H_

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

#include "gfx.h"
#include "anim.h"

typedef struct {
	u_int active : 1;
	u_int visible : 1;
} CP_ActorFlags;

typedef struct {
	SVECTOR rot;
	VECTOR trans;
	VECTOR scale;

	CP_ActorFlags flags;

	u_int meshCount;
	CP_Mesh *mesh;

	u_short animCounter;
	u_short currFrame;

	CP_Anim *anim;
} CP_Actor;

extern u_int gLoadedActors;
extern CP_Actor gActors[8];

void actorInit(CP_Actor *actor, const u_int MESH_COUNT);

u_int actorLoad(const char *PATH);
void actorLoadInto(const char *PATH, CP_Actor *actor);

void actorFreePointer(CP_Actor *actor);
u_int actorFreeLast(void);
u_int actorFreeAt(const u_int POSITION);

void actorNextFrame(CP_Actor *actor);

void actorUpdate(CP_Actor *actor);
void actorUpdateAll(void);

void actorDraw(CP_Actor *actor);
void actorDrawAll(void);

#endif // !GUARD_CITYPEEP_ACTOR_H_
