/* Citypeep: Actor */

#include <malloc.h>

#include "actor.h"
#include "gfx.h"

void actorInit(CP_Actor *actor, const u_int MESH_COUNT) {
	actor->meshCount = MESH_COUNT;
	actor->mesh = malloc3(MESH_COUNT * sizeof(*actor->mesh));

	setVector(&actor->rot, 0, 0, 0);
	setVector(&actor->trans, 0, 0, 0);
	setVector(&actor->scale, ONE, ONE, ONE);
}

void actorDraw(CP_Actor *actor) {
	for( int i = 0; i < actor->meshCount; ++i ) {
		gfxDrawMeshT(&actor->mesh[i]);
	}
}
