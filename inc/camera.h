#ifndef GUARD_CITYPEEP_CAMERA_H_
#define GUARD_CITYPEEP_CAMERA_H_

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

typedef struct {
	SVECTOR rot;
	SVECTOR trot;

	VECTOR trans;
	VECTOR ttrans;

	MATRIX mat;
} CP_Camera;

extern CP_Camera camera;

void camInit(void);

void camUpdate(void);

#endif // !GUARD_CITYPEEP_CAMERA_H_
