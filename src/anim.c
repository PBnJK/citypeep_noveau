/* Citypeep: Animation library */

#include <stddef.h>

#include "common.h"
#include "system.h"
#include "cp_memory.h"
#include "anim.h"

void animLoad(const char *PATH, CP_Anim *anim) {
	CP_Frame *frame;
	CP_Action *action;

	u_long *loaded = sysLoadFileFromCD(PATH);
	u_long *data = loaded;

	anim->resolution = *data++;

	anim->frameNum = *data++;
	anim->frames = memAlloc(anim->frameNum * sizeof(CP_Frame));

	LOG("%d!!\n", anim->frameNum);

	for( int i = 0; i < anim->frameNum; ++i ) {
		frame = &anim->frames[i];

		frame->actionNum = *data++;
		frame->actions = memAlloc(frame->actionNum * sizeof(CP_Action));

		for( int j = 0; j < frame->actionNum; ++j ) {
			action = &frame->actions[j];

			/* 1 byte each */
			action->bone = *data;
			action->kfType = (*data++) >> 8;
			action->animType = A_DIRECT;

			/* SVECTOR (2 bytes per member) */
			action->rot.vx = *data;
			action->rot.vy = (*data++) >> 16;
			action->rot.vz = *data++;

			/* VECTORs (4 bytes per member) */
			action->trans.vx = *data++;
			action->trans.vy = *data++;
			action->trans.vz = *data++;

			action->scale.vx = *data++;
			action->scale.vy = *data++;
			action->scale.vz = *data++;
		}
	}
}
