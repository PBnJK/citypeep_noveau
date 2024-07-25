#ifndef GUARD_CITYPEEP_ANIM_H_
#define GUARD_CITYPEEP_ANIM_H_

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

/* Keyframe type */
typedef enum {
	K_ROT = 1,
	K_TRANS = 2,
	K_SCALE = 4,
} CP_KeyFType;

/* Animation type */
typedef enum {
	A_DIRECT,
	A_LERP,
} CP_AnimType;

typedef struct {
	u_char bone; /* Index to target bone */

	u_char kfType;
	CP_AnimType animType;

	SVECTOR rot;
	VECTOR trans;
	VECTOR scale;
} CP_Action;

/* A frame, list of actions */
typedef struct {
	u_int actionNum;
	CP_Action *actions;
} CP_Frame;

/* Struct representing an animation (series of 'CP_Action's) */
typedef struct {
	/* Ticks per frame */
	u_short resolution;

	/* Array of frames (which are just arrays of actions) */
	u_int frameNum;
	CP_Frame *frames;
} CP_Anim;

void animLoad(const char *PATH, CP_Anim *anim);

#endif // !GUARD_CITYPEEP_ANIM_H_
