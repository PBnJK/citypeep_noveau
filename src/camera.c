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
		camera.rot.vx += 8;
	}

	if( !PAD_P1.down ) {
		camera.rot.vx -= 8;
	}

	if( !PAD_P1.left ) {
		camera.rot.vy += 8;
	}

	if( !PAD_P1.right ) {
		camera.rot.vy -= 8;
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
	}

	if( !PAD_P1.cross ) {
	}

	if( !PAD_P1.square ) {
	}

	if( !PAD_P1.circle ) {
	}
	*/

	RotMatrix(&camera.rot, &camera.mat);

	camera.ttrans.vx = -camera.trans.vx >> 12;
	camera.ttrans.vy = -camera.trans.vy >> 12;
	camera.ttrans.vz = -camera.trans.vz >> 12;
	ApplyMatrixLV(&camera.mat, &camera.ttrans, &camera.ttrans);
	TransMatrix(&camera.mat, &camera.ttrans);
}
