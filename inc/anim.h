#ifndef GUARD_CITYPEEP_ANIM_H_
#define GUARD_CITYPEEP_ANIM_H_

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

/* Keyframe type */
typedef enum {
	ROT,
	TRANS,
	SCALE,
	ROT_TRANS,
	ROT_SCALE,
	TRANS_SCALE,
	ROT_TRANS_SCALE,
} CP_KeyFType;

/* Animation type */
typedef enum {
	DIRECT,
	LERP,
} CP_AnimType;

typedef struct {
	u_char bone; /* Index to target bone */

	CP_KeyFType kfType;
	CP_AnimType animType;

	union {
		SVECTOR rot;
		VECTOR trans;
		VECTOR scale;
	} data;
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

#endif // !GUARD_CITYPEEP_ANIM_H_
