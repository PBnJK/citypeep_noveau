#ifndef GUARD_CITYPEEP_DATA_MODEL_PLAYER_ARM_H_
#define GUARD_CITYPEEP_DATA_MODEL_PLAYER_ARM_H_

#include "gfx.h"

static const CP_Mesh player_arm = {
	.rot={0, 0, 0},
	.trans={0, 0, 0},
	.scale={4096, 4096, 4096},

	.type=2,
	.flags={
		.visible=1,
		.textured=1,
		.gouraud=0,
	},

	.vcount=20,
	.verts={
		{ -22, 15, -32, 0 },
		{ -3, -35, -32, 0 },
		{ -22, 15, 32, 0 },
		{ -3, -35, 32, 0 },
		{ 21, 15, -32, 0 },
		{ 21, -48, -32, 0 },
		{ 21, 15, 32, 0 },
		{ 21, -48, 32, 0 },
		{ 2, -16, 0, 0 },
		{ 2, 53, 12, 0 },
		{ 2, 9, 12, 0 },
		{ 14, 53, 3, 0 },
		{ 14, 9, 3, 0 },
		{ 10, 53, -10, 0 },
		{ 10, 9, -10, 0 },
		{ -4, 53, -10, 0 },
		{ -4, 9, -10, 0 },
		{ -9, 53, 3, 0 },
		{ -9, 9, 3, 0 },
		{ 0, 0, 0, 0 },
	},

	.ccount=0,
	.colors={
		{ -22, 15, -32, 0 },
		{ -3, -35, -32, 0 },
		{ -22, 15, 32, 0 },
		{ -3, -35, 32, 0 },
		{ 21, 15, -32, 0 },
		{ 21, -48, -32, 0 },
		{ 21, 15, 32, 0 },
		{ 21, -48, 32, 0 },
		{ 2, -16, 0, 0 },
		{ 2, 53, 12, 0 },
		{ 2, 9, 12, 0 },
		{ 14, 53, 3, 0 },
		{ 14, 9, 3, 0 },
		{ 10, 53, -10, 0 },
		{ 10, 9, -10, 0 },
		{ -4, 53, -10, 0 },
		{ -4, 9, -10, 0 },
		{ -9, 53, 3, 0 },
		{ -9, 9, 3, 0 },
		{ 0, 0, 0, 0 },
	},

	.fcount=24,
	.faces={
		{ 1, 2, 0, 0 },
		{ 3, 6, 2, 0 },
		{ 4, 1, 0, 0 },
		{ 6, 8, 2, 0 },
		{ 3, 5, 7, 0 },
		{ 1, 3, 2, 0 },
		{ 3, 7, 6, 0 },
		{ 4, 5, 1, 0 },
		{ 6, 4, 8, 0 },
		{ 3, 1, 5, 0 },
		{ 8, 4, 0, 0 },
		{ 2, 8, 0, 0 },
		{ 9, 10, 12, 0 },
		{ 9, 12, 11, 0 },
		{ 11, 12, 14, 0 },
		{ 11, 14, 13, 0 },
		{ 13, 14, 16, 0 },
		{ 13, 16, 15, 0 },
		{ 12, 10, 18, 0 },
		{ 15, 16, 18, 0 },
		{ 15, 18, 17, 0 },
		{ 17, 18, 10, 0 },
		{ 17, 10, 9, 0 },
		{ 9, 11, 13, 0 },
	},

	.ncount=14,
	.normals={
		{ -3835, -1438, 0, 0 },
		{ 0, 0, 4096, 0 },
		{ 0, 0, -4096, 0 },
		{ 0, 2896, -2896, 0 },
		{ -1831, -3663, 0, 0 },
		{ -3512, 2107, 0, 0 },
		{ 0, 2896, 2896, 0 },
		{ 3198, 2558, 0, 0 },
		{ 2407, 0, 3313, 0 },
		{ 3895, 0, -1265, 0 },
		{ 0, -4096, 0, 0 },
		{ -3895, 0, -1265, 0 },
		{ -2407, 0, 3313, 0 },
		{ 0, 4096, 0, 0 },
	},

	.nidxs={
		{ 0, 0, 0, 0 },
		{ 1, 1, 1, 0 },
		{ 2, 2, 2, 0 },
		{ 3, 3, 3, 0 },
		{ 4, 4, 4, 0 },
		{ 0, 0, 0, 0 },
		{ 1, 1, 1, 0 },
		{ 2, 2, 2, 0 },
		{ 5, 5, 5, 0 },
		{ 4, 4, 4, 0 },
		{ 6, 6, 6, 0 },
		{ 7, 7, 7, 0 },
		{ 8, 8, 8, 0 },
		{ 8, 8, 8, 0 },
		{ 9, 9, 9, 0 },
		{ 9, 9, 9, 0 },
		{ 2, 2, 2, 0 },
		{ 2, 2, 2, 0 },
		{ 10, 10, 10, 0 },
		{ 11, 11, 11, 0 },
		{ 11, 11, 11, 0 },
		{ 12, 12, 12, 0 },
		{ 12, 12, 12, 0 },
		{ 13, 13, 13, 0 },
	},

	.tcount=40,
	.uvs={
		{ 128, 2368 },
		{ 896, 3008 },
		{ 128, 3008 },
		{ 576, 2368 },
		{ 768, 3008 },
		{ 192, 3008 },
		{ 576, 2176 },
		{ 512, 2112 },
		{ 448, 2176 },
		{ 384, 2304 },
		{ 640, 2880 },
		{ 640, 2304 },
		{ 896, 2368 },
		{ 832, 2368 },
		{ 576, 2048 },
		{ 384, 2880 },
		{ 448, 2048 },
		{ 2944, 576 },
		{ 2944, 704 },
		{ 3008, 704 },
		{ 3008, 576 },
		{ 2880, 704 },
		{ 2880, 576 },
		{ 1958, 2769 },
		{ 1024, 2089 },
		{ 89, 2769 },
		{ 446, 3868 },
		{ 1601, 3868 },
		{ 3136, 576 },
		{ 3136, 704 },
		{ 3200, 704 },
		{ 3200, 576 },
		{ 3264, 704 },
		{ 3264, 576 },
		{ 3072, 2089 },
		{ 4006, 2769 },
		{ 3649, 3868 },
		{ 2494, 3868 },
		{ 2137, 2769 },
		{ 0, 0 },
	},

	.uvidxs={
		{ 0, 1, 2, 0 },
		{ 3, 4, 5, 0 },
		{ 4, 3, 5, 0 },
		{ 6, 7, 8, 0 },
		{ 9, 10, 11, 0 },
		{ 0, 12, 1, 0 },
		{ 3, 13, 4, 0 },
		{ 4, 13, 3, 0 },
		{ 6, 14, 7, 0 },
		{ 9, 15, 10, 0 },
		{ 7, 14, 16, 0 },
		{ 8, 7, 16, 0 },
		{ 17, 18, 19, 0 },
		{ 17, 19, 20, 0 },
		{ 17, 18, 21, 0 },
		{ 17, 21, 22, 0 },
		{ 22, 21, 18, 0 },
		{ 22, 18, 17, 0 },
		{ 23, 24, 25, 0 },
		{ 28, 29, 30, 0 },
		{ 28, 30, 31, 0 },
		{ 31, 30, 32, 0 },
		{ 31, 32, 33, 0 },
		{ 34, 35, 36, 0 },
	},

};

#endif // !GUARD_CITYPEEP_DATA_MODEL_PLAYER_ARM_H_
