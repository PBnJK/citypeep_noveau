/* Citypeep: Actor */

#include <stdlib.h>
#include <sys/types.h>

#include <libetc.h>
#include <libgte.h>
#include <libgpu.h>
#include <malloc.h>

#include <inline_n.h>

#include "actor.h"
#include "anim.h"
#include "cp_memory.h"
#include "gfx.h"
#include "image.h"
#include "stddef.h"
#include "system.h"

#define ACTOR_LIST_SIZE 8

u_int gLoadedActors = 0;
CP_Actor gActors[ACTOR_LIST_SIZE] = { 0 };

void actorInit(CP_Actor *actor, const u_int MESH_COUNT) {
	actor->meshCount = MESH_COUNT;

	actor->flags.active = 1;
	actor->flags.visible = 1;

	setVector(&actor->rot, 0, 0, 0);
	setVector(&actor->trans, 0, 0, 0);
	setVector(&actor->scale, ONE, ONE, ONE);

	actor->currFrame = 0;
	actor->animCounter = 0;
}

void actorExit(void) {
	for( u_int i = 0; i < ACTOR_LIST_SIZE; ++i ) {
		actorFreeAt(i);
	}
}

int actorLoad(const char *PATH, const char *TEX) {
	if( gLoadedActors >= ACTOR_LIST_SIZE ) {
		return -1;
	}

	actorLoadInto(PATH, TEX, &gActors[gLoadedActors++]);
	return gLoadedActors - 1;
}

int actorLoadPtr(CP_Actor *actor, const char *TEX) {
	if( gLoadedActors >= ACTOR_LIST_SIZE ) {
		return -1;
	}

	TIM_IMAGE img;
	CP_Mesh *mesh;
	imgLoad(TEX, &img);

	int width = img.prect->w;
	switch( img.mode & 0x3 ) {
	case 0:
		width <<= 1;
	case 1:
		width <<= 1;
	}

	for( u_int i = 0; i < actor->meshCount; ++i ) {
		mesh = &actor->mesh[i];

		mesh->tex = img;
		gfxSetupTex(mesh);

		for( u_int j = 0; j < mesh->tcount; ++j ) {
			mesh->uvs[j].u = (width * mesh->uvs[j].u) / 4096;
			mesh->uvs[j].v = (img.prect->h * mesh->uvs[j].v) / 4096;
		}
	}

	actor->flags.textured = 1;

	gActors[gLoadedActors++] = *actor;

	return gLoadedActors - 1;
}

void actorLoadInto(const char *PATH, const char *TEX, CP_Actor *actor) {
	CP_Mesh *mesh;

	u_long *loaded = sysLoadFileFromCD(PATH);
	u_long *data = loaded;

	++data; /* Flags, parse later */

	actorInit(actor, *data++);

	for( int i = 0; i < actor->meshCount; ++i ) {
		mesh = &actor->mesh[i];

		if( *data == 0xFFFFFFFF ) {
			/* Same as previous model! */
			gfxCopyMesh(&actor->mesh[i - 1], mesh);
			++data;
		} else {
			data += gfxLoadMeshPtr(data, TEX, mesh);
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

	data = NULL;
	memFree(loaded);
}

void actorFreePointer(CP_Actor *actor) {
	for( u_int i = 0; i < actor->meshCount; ++i ) {
		gfxFreeMesh(&actor->mesh[i]);
	}

	actor->meshCount = 0;

	actor->flags.active = 0;
	actor->flags.visible = 0;

	setVector(&actor->rot, 0, 0, 0);
	setVector(&actor->trans, 0, 0, 0);
	setVector(&actor->scale, ONE, ONE, ONE);

	actor->currFrame = 0;
	actor->animCounter = 0;

	actor = NULL;
}

u_int actorFreeLast(void) {
	if( gLoadedActors == NULL ) {
		return 1;
	}

	actorFreePointer(&gActors[--gLoadedActors]);
	return 0;
}

u_int actorFreeAt(const u_int POSITION) {
	if( POSITION >= gLoadedActors ) {
		return 1;
	}

	actorFreePointer(&gActors[POSITION]);
	return 0;
}

void actorDoFrame(CP_Actor *actor, CP_Frame *frame) {
	CP_Action *action;
	CP_Mesh *mesh;

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

	TransMatrix(&actor->mat, &actor->trans);
	ScaleMatrix(&actor->mat, &actor->scale);
	RotMatrix_gte(&actor->rot, &actor->mat);

	for( int i = 0; i < actor->meshCount; ++i ) {
		gfxDrawMeshWithMatrix(&actor->mesh[i], &actor->mat);
	}

	gfxSetTPage(actor->mesh[0].tpage);
}

void actorDrawAll(void) {
	for( u_int i = 0; i < gLoadedActors; ++i ) {
		actorDraw(&gActors[i]);
	}
}
