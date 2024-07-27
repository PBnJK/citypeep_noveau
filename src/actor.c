/* Citypeep: Actor */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <malloc.h>

#include <inline_n.h>

#include "common.h"
#include "anim.h"
#include "gfx.h"
#include "cp_memory.h"
#include "system.h"
#include "actor.h"

#define ACTOR_LIST_SIZE 8

u_int gLoadedActors = 0;
CP_Actor gActors[ACTOR_LIST_SIZE] = { 0 };

void actorInit(CP_Actor *actor, const u_int MESH_COUNT) {
	actor->meshCount = MESH_COUNT;
	actor->mesh = memAlloc(MESH_COUNT * sizeof(*actor->mesh));

	actor->flags.active = 1;
	actor->flags.visible = 1;

	setVector(&actor->rot, 0, 0, 0);
	setVector(&actor->trans, 0, 0, 0);
	setVector(&actor->scale, ONE, ONE, ONE);

	actor->currFrame = 0;
	actor->animCounter = 0;

	actor->anim = memAlloc(sizeof(CP_Anim));
}

void actorLoad(const char *PATH) {
	if( gLoadedActors >= ACTOR_LIST_SIZE ) {
		LOG("=== FATAL ERROR: TOO_MANY_ACTORS ===\n");
		LOG("Tried to load an actor into a full actor list!\n");
		exit();
	}

	actorLoadInto(PATH, &gActors[gLoadedActors++]);
}

void actorLoadInto(const char *PATH, CP_Actor *actor) {
	CP_MeshT *mesh;

	u_long *loaded = sysLoadFileFromCD(PATH);
	u_long *data = loaded;

	++data; /* Flags, parse later */

	actorInit(actor, *data++);

	for( int i = 0; i < actor->meshCount; ++i ) {
		mesh = &actor->mesh[i];

		if( *data == 0xFFFFFFFF ) {
			/* Same as previous model! */
			gfxCopyMeshT(&actor->mesh[i - 1], mesh);
			++data;
		} else {
			data += gfxLoadMeshPtrT(data, "\\MDL\\TEX.TIM;1", mesh);
		}

		/* SVECTOR (2 bytes per member) */
		mesh->rot.vx = *data;
		mesh->rot.vy = (*data++) >> 16;
		mesh->rot.vz = *data++;

		/* VECTORs (4 bytes per member) */
		mesh->trans.vx = *data++;
		mesh->trans.vy = *data++;
		mesh->trans.vz = *data++;

		mesh->scale.vx = *data++;
		mesh->scale.vy = *data++;
		mesh->scale.vz = *data++;
	}
}

void actorFreePointer(CP_Actor *actor) {
	actor->meshCount = 0;
	memFree(actor->mesh);

	actor->flags.active = 0;
	actor->flags.visible = 0;

	setVector(&actor->rot, 0, 0, 0);
	setVector(&actor->trans, 0, 0, 0);
	setVector(&actor->scale, ONE, ONE, ONE);

	actor->currFrame = 0;
	actor->animCounter = 0;

	memFree(actor->anim);
}

void actorFreeLast(void) {
	if( gLoadedActors == 0 ) {
		LOG("=== FATAL ERROR: NO_ACTORS_TO_FREE ===\n");
		LOG("Tried to free an actor from an empty actor list!\n");
		exit();
	}

	actorFreePointer(&gActors[--gLoadedActors]);
}

void actorFreeAt(const u_int POSITION) {
	if( POSITION >= gLoadedActors ) {
		LOG("=== FATAL ERROR: NO_ACTOR_THERE ===\n");
		LOG("Tried to free an actor from an empty position in the actor "
			"list!\n");
		exit();
	}

	actorFreePointer(&gActors[POSITION]);
}

void actorDoFrame(CP_Actor *actor, CP_Frame *frame) {
	CP_Action *action;
	CP_MeshT *mesh;

	for( int i = 0; i < frame->actionNum; ++i ) {
		action = &frame->actions[i];
		mesh = &actor->mesh[action->bone];

		if( action->kfType & K_ROT ) {
			copyVector(&mesh->rot, &action->rot);
		}

		if( action->kfType & K_TRANS ) {
			copyVector(&mesh->trans, &action->trans);
		}

		if( action->kfType & K_SCALE ) {
			copyVector(&mesh->scale, &action->scale);
		}
	}
}

void actorNextFrame(CP_Actor *actor) {
	++actor->currFrame;

	if( actor->currFrame >= actor->anim->frameNum ) {
		actor->currFrame = 0;
	}

	actorDoFrame(actor, &actor->anim->frames[actor->currFrame]);
}

void actorUpdate(CP_Actor *actor) {
	if( actor->anim == NULL ) {
		return;
	}

	if( actor->animCounter > actor->anim->resolution ) {
		actorNextFrame(actor);
		actor->animCounter = 0;
	}

	++actor->animCounter;
}

void actorUpdateAll(void) {
	for( u_int i = 0; i < gLoadedActors; ++i ) {
		actorUpdate(&gActors[i]);
	}
}

void actorDraw(CP_Actor *actor) {
	if( !actor->flags.visible ) {
		return;
	}

	MATRIX omtx;

	RotMatrix_gte(&actor->rot, &omtx);
	TransMatrix(&omtx, &actor->trans);
	ScaleMatrix(&omtx, &actor->scale);

	for( int i = 0; i < actor->meshCount; ++i ) {
		gfxDrawMeshTWithMatrix(&actor->mesh[i], &omtx);
	}
}

void actorDrawAll(void) {
	for( u_int i = 0; i < gLoadedActors; ++i ) {
		actorDraw(&gActors[i]);
	}
}
