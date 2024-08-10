/* Citypeep: camera */

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>

#include "camera.h"

void camInit(CP_Camera *cam) {
	setVector(&cam->trans, 0, 0, 0);
	setVector(&cam->rot, 0, 0, 0);
}
