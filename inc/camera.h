#ifndef GUARD_CITYPEEP_CAMERA_H_
#define GUARD_CITYPEEP_CAMERA_H_

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

typedef struct {
	SVECTOR rot;
	VECTOR trans;
	MATRIX mat;
} CP_Camera;

void camInit(CP_Camera *cam);

void camUpdate(void);

#endif // !GUARD_CITYPEEP_CAMERA_H_
