/* Citypeep: camera */

#include <sys/types.h>

#include <libgte.h>
#include <libgpu.h>
#include <inline_n.h>

#include "camera.h"
#include "input.h"
#include "math.h"

CP_Camera camera = { 0 };

void camInit(void) {
	setVector(&camera.trans, 0, 0, 0);
	setVector(&camera.ttrans, 0, 0, 0);
	setVector(&camera.rot, 0, 0, 0);
	setVector(&camera.trot, 0, 0, 0);
}

void camUpdate() {
	setVector(&camera.trot, camera.rot.vx >> 12, camera.rot.vy >> 12,
		camera.rot.vz >> 12);

	if( !PAD_P1.up ) {
		camera.rot.vy += 4;
	}

	if( !PAD_P1.down ) {
		camera.rot.vy -= 4;
	}

	if( !PAD_P1.left ) {
		camera.rot.vx += 4;
	}

	if( !PAD_P1.right ) {
		camera.rot.vx -= 4;
	}

	if( !PAD_P1.l1 ) {
		camera.trans.vx -= ((isin(camera.trot.vy) * icos(camera.trot.vx)) >> 12)
			<< 2;
		camera.trans.vy += isin(camera.trot.vx) << 2;
		camera.trans.vz += ((icos(camera.trot.vy) * icos(camera.trot.vx)) >> 12)
			<< 2;
	}

	if( !PAD_P1.r1 ) {
		camera.trans.vx += ((isin(camera.trot.vy) * icos(camera.trot.vx)) >> 12)
			<< 2;
		camera.trans.vy -= isin(camera.trot.vx) << 2;
		camera.trans.vz -= ((icos(camera.trot.vy) * icos(camera.trot.vx)) >> 12)
			<< 2;
	}

	/*
	if( !PAD_P1.triangle ) {
		camera.rot.vx -= 4;
	}

	if( !PAD_P1.cross ) {
		camera.rot.vx += 4;
	}

	if( !PAD_P1.square ) {
		camera.rot.vy -= 4;
	}

	if( !PAD_P1.circle ) {
		camera.rot.vy += 4;
	}

	*/

	VECTOR vec;

	RotMatrix(&camera.rot, &camera.mat);
	ApplyMatrixLV(&camera.mat, &camera.trans, &vec);
	TransMatrix(&camera.mat, &vec);
}
