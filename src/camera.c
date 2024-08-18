/* Citypeep: camera->*/

#include "camera.h"
#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>
#include <inline_n.h>

#include "camera.h"
#include "input.h"

void camInit(CP_Camera *camera) {
	setVector(&camera->trans, 0, 0, 0);
	setVector(&camera->ttrans, 0, 0, 0);
	setVector(&camera->rot, 0, 0, 0);
}

void camUpdate(CP_Camera *camera) {
	if( !PAD_P1.up ) {
		camera->rot.vx += 8;
	}

	if( !PAD_P1.down ) {
		camera->rot.vx -= 8;
	}

	if( !PAD_P1.left ) {
		camera->rot.vy += 8;
	}

	if( !PAD_P1.right ) {
		camera->rot.vy -= 8;
	}

	if( !PAD_P1.l1 ) {
		camera->trans.vz -= 4;
	}

	if( !PAD_P1.r1 ) {
		camera->trans.vz += 4;
	}

	if( !PAD_P1.triangle ) {
		camera->trans.vy -= 4;
	}

	if( !PAD_P1.cross ) {
		camera->trans.vy += 4;
	}

	if( !PAD_P1.square ) {
		camera->trans.vx -= 4;
	}

	if( !PAD_P1.circle ) {
		camera->trans.vx += 4;
	}

	RotMatrix(&camera->rot, &camera->mat);

	ApplyMatrixLV(&camera->mat, &camera->trans, &camera->ttrans);
	TransMatrix(&camera->mat, &camera->ttrans);
}
