/* Citypeep: Animation library */

#include <stddef.h>
#include <malloc.h>

#include "anim.h"
/*
void animNewBone(SVECTOR rot, VECTOR trans, VECTOR scale, CP_Bone *bone) {
	bone->rot = rot;
	bone->trans = trans;
	bone->scale = scale;

	bone->childNum = 0;
	bone->children = NULL;
}

void animAppendBone(CP_Bone *parent, CP_Bone *child) {
	const int NEW_SIZE = (parent->childNum + 1) * sizeof(CP_Bone *);
	parent->children = realloc3(parent->children, NEW_SIZE);

	parent->children[parent->childNum++] = child;
	parent->children[parent->childNum] = NULL;
}
*/
