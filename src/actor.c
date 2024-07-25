/* Citypeep: Actor */

#include <stdio.h>
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

void actorInit(CP_Actor *actor, const u_int MESH_COUNT) {
	actor->meshCount = MESH_COUNT;
	LOG("mt: 1\n");
	actor->mesh = memAlloc(MESH_COUNT * sizeof(*actor->mesh));

	actor->flags.active = 1;
	actor->flags.visible = 1;

	setVector(&actor->rot, 0, 0, 0);
	setVector(&actor->trans, 0, 0, 0);
	setVector(&actor->scale, ONE, ONE, ONE);

	actor->currFrame = 0;
	actor->animCounter = 0;

	LOG("mt: 2\n");
	actor->anim = memAlloc(sizeof(CP_Anim));
}

void actorLoad(const char *PATH, CP_Actor *actor) {
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

	if( actor->currFrame == actor->anim->frameNum ) {
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
