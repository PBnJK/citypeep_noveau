#ifndef GUARD_CITYPEEP_DATA_MODEL_PLAYER_LEG_H_
#define GUARD_CITYPEEP_DATA_MODEL_PLAYER_LEG_H_

#include "gfx.h"

static const CP_Mesh player_leg = {
	.rot={0, 0, 0},
	.trans={0, 0, 0},
	.scale={4096, 4096, 4096},

	.type=2,
	.flags={
		.visible=1,
		.textured=1,
		.gouraud=0,
	},

	.vcount=16,
	.verts={
		{ -9, -16, -4, 0 },
		{ -9, 16, -12, 0 },
		{ -9, -16, 11, 0 },
		{ -9, 16, 11, 0 },
		{ 6, -16, -4, 0 },
		{ 6, 16, -12, 0 },
		{ 6, -16, 11, 0 },
		{ 6, 16, 11, 0 },
		{ -9, 0, -4, 0 },
		{ 6, 0, 11, 0 },
		{ -9, 0, 11, 0 },
		{ 6, 0, -4, 0 },
		{ -9, -8, -4, 0 },
		{ 6, -16, 3, 0 },
		{ 6, 16, 0, 0 },
		{ 0, 0, 0, 0 },
	},

	.ccount=0,
	.colors={
		{ -9, -16, -4, 0 },
		{ -9, 16, -12, 0 },
		{ -9, -16, 11, 0 },
		{ -9, 16, 11, 0 },
		{ 6, -16, -4, 0 },
		{ 6, 16, -12, 0 },
		{ 6, -16, 11, 0 },
		{ 6, 16, 11, 0 },
		{ -9, 0, -4, 0 },
		{ 6, 0, 11, 0 },
		{ -9, 0, 11, 0 },
		{ 6, 0, -4, 0 },
		{ -9, -8, -4, 0 },
		{ 6, -16, 3, 0 },
		{ 6, 16, 0, 0 },
		{ 0, 0, 0, 0 },
	},

	.fcount=26,
	.faces={
		{ 4, 2, 0, 0 },
		{ 2, 9, 10, 0 },
		{ 6, 11, 9, 0 },
		{ 1, 7, 14, 0 },
		{ 1, 14, 5, 0 },
		{ 0, 10, 8, 0 },
		{ 0, 8, 12, 0 },
		{ 4, 8, 11, 0 },
		{ 11, 1, 5, 0 },
		{ 8, 3, 1, 0 },
		{ 7, 11, 5, 0 },
		{ 7, 5, 14, 0 },
		{ 10, 7, 3, 0 },
		{ 4, 13, 6, 0 },
		{ 4, 6, 2, 0 },
		{ 2, 6, 9, 0 },
		{ 6, 13, 4, 0 },
		{ 6, 4, 11, 0 },
		{ 1, 3, 7, 0 },
		{ 0, 2, 10, 0 },
		{ 4, 0, 12, 0 },
		{ 4, 12, 8, 0 },
		{ 11, 8, 1, 0 },
		{ 8, 10, 3, 0 },
		{ 7, 9, 11, 0 },
		{ 10, 9, 7, 0 },
	},

	.ncount=8,
	.normals={
		{ 0, -4096, 0, 0 },
		{ 0, 0, 4096, 0 },
		{ 4096, 0, 0, 0 },
		{ 0, 4096, 0, 0 },
		{ -4096, 0, 0, 0 },
		{ 0, 0, -4096, 0 },
		{ 0, -1831, -3663, 0 },
		{ 0, 0, 0, 0 },
	},

	.nidxs={
		{ 0, 0, 0, 0 },
		{ 1, 1, 1, 0 },
		{ 2, 2, 2, 0 },
		{ 3, 3, 3, 0 },
		{ 3, 3, 3, 0 },
		{ 4, 4, 4, 0 },
		{ 4, 4, 4, 0 },
		{ 5, 5, 5, 0 },
		{ 6, 6, 6, 0 },
		{ 4, 4, 4, 0 },
		{ 2, 2, 2, 0 },
		{ 2, 2, 2, 0 },
		{ 1, 1, 1, 0 },
		{ 0, 0, 0, 0 },
		{ 0, 0, 0, 0 },
		{ 1, 1, 1, 0 },
		{ 2, 2, 2, 0 },
		{ 2, 2, 2, 0 },
		{ 3, 3, 3, 0 },
		{ 4, 4, 4, 0 },
		{ 5, 5, 5, 0 },
		{ 5, 5, 5, 0 },
		{ 6, 6, 6, 0 },
		{ 4, 4, 4, 0 },
		{ 2, 2, 2, 0 },
		{ 1, 1, 1, 0 },
	},

	.tcount=28,
	.uvs={
		{ 768, 1152 },
		{ 512, 1408 },
		{ 768, 1408 },
		{ 1024, 1024 },
		{ 512, 1472 },
		{ 1024, 1472 },
		{ 0, 1024 },
		{ 0, 1472 },
		{ 256, 512 },
		{ 0, 0 },
		{ 0, 256 },
		{ 0, 512 },
		{ 512, 1024 },
		{ 512, 1280 },
		{ 256, 0 },
		{ 768, 512 },
		{ 768, 1024 },
		{ 256, 1024 },
		{ 0, 2048 },
		{ 768, 2048 },
		{ 384, 2048 },
		{ 512, 2048 },
		{ 1024, 2048 },
		{ 640, 1152 },
		{ 512, 1152 },
		{ 768, 0 },
		{ 768, 256 },
		{ 0, 0 },
	},

	.uvidxs={
		{ 0, 1, 2, 0 },
		{ 3, 4, 5, 0 },
		{ 6, 4, 7, 0 },
		{ 8, 9, 10, 0 },
		{ 8, 10, 11, 0 },
		{ 12, 7, 4, 0 },
		{ 12, 4, 13, 0 },
		{ 14, 15, 8, 0 },
		{ 8, 16, 17, 0 },
		{ 4, 18, 19, 0 },
		{ 18, 4, 19, 0 },
		{ 18, 19, 20, 0 },
		{ 5, 21, 22, 0 },
		{ 0, 23, 24, 0 },
		{ 0, 24, 1, 0 },
		{ 3, 12, 4, 0 },
		{ 6, 17, 12, 0 },
		{ 6, 12, 4, 0 },
		{ 8, 14, 9, 0 },
		{ 12, 6, 7, 0 },
		{ 14, 25, 26, 0 },
		{ 14, 26, 15, 0 },
		{ 8, 15, 16, 0 },
		{ 4, 7, 18, 0 },
		{ 18, 7, 4, 0 },
		{ 5, 4, 21, 0 },
	},

};

#endif // !GUARD_CITYPEEP_DATA_MODEL_PLAYER_LEG_H_
