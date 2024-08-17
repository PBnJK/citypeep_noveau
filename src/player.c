/* Citypeep: player character */

#include "player.h"
#include "actor.h"
#include "camera.h"
#include "act_player.h"

CP_Player player = { 0 };

void playerInit(void) {
	act_player.mesh[0].rot = R_0;
	act_player.mesh[0].trans = T_0;
	act_player.mesh[0].scale = S_0;

	act_player.mesh[1].rot = R_1;
	act_player.mesh[1].trans = T_1;
	act_player.mesh[1].scale = S_1;

	actorLoadPtr(&act_player);

	camInit(&player.camera);
}

void playerUpdate(void) {
	camUpdate(&player.camera);
}
