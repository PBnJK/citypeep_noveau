/* Citypeep: Actor */

#include <malloc.h>

#include "actor.h"
#include "gfx.h"
#include "cp_memory.h"
#include "system.h"

void actorInit(CP_Actor *actor, const u_int MESH_COUNT) {
	actor->meshCount = MESH_COUNT;
	actor->mesh = memAlloc(MESH_COUNT * sizeof(*actor->mesh));

	actor->flags.active = 1;
	actor->flags.visible = 1;

	setVector(&actor->rot, 0, 0, 0);
	setVector(&actor->trans, 0, 0, 0);
	setVector(&actor->scale, ONE, ONE, ONE);
}

void actorLoad(const char *PATH, CP_Actor *actor) {
	u_long *loaded = sysLoadFileFromCD(PATH);
	u_long *data = loaded;

	++data; /* Flags, parse later */

	actorInit(actor, *data++);

	for( int i = 0; i < actor->meshCount; ++i ) {
		if( *data == 0xFFFFFFFF ) {
			/* Same as previous model! */
			gfxCopyMeshT(&actor->mesh[i - 1], &actor->mesh[i]);
			++data;
		} else {
			data += gfxLoadMeshPtrT(data, "\\MDL\\TEX.TIM;1", &actor->mesh[i]);
		}

		/* SVECTOR (2 bytes per member) */
		actor->mesh[i].rot.vx = *data;
		actor->mesh[i].rot.vy = (*data++) >> 16;
		actor->mesh[i].rot.vz = *data++;

		/* VECTORs (4 bytes per member) */
		actor->mesh[i].trans.vx = *data++;
		actor->mesh[i].trans.vy = *data++;
		actor->mesh[i].trans.vz = *data++;

		actor->mesh[i].scale.vx = *data++;
		actor->mesh[i].scale.vy = *data++;
		actor->mesh[i].scale.vz = *data++;
	}
}

void actorDraw(CP_Actor *actor) {
	if( !actor->flags.visible ) {
		return;
	}

	for( int i = 0; i < actor->meshCount; ++i ) {
		gfxDrawMeshT(&actor->mesh[i]);
	}
}
