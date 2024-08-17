/* Citypeep: player character */

#include "player.h"
#include "actor.h"
#include "camera.h"
#include "act_player_player.h"

CP_Player player = { 0 };

void playerInit(void) {
	act_player_player.mesh[0].rot = R_0;
	act_player_player.mesh[0].trans = T_0;
	act_player_player.mesh[0].scale = S_0;

	act_player_player.mesh[1].rot = R_1;
	act_player_player.mesh[1].trans = T_1;
	act_player_player.mesh[1].scale = S_1;

	act_player_player.mesh[2].rot = R_2;
	act_player_player.mesh[2].trans = T_2;
	act_player_player.mesh[2].scale = S_2;

	act_player_player.mesh[3].rot = R_3;
	act_player_player.mesh[3].trans = T_3;
	act_player_player.mesh[3].scale = S_3;

	act_player_player.mesh[4].rot = R_4;
	act_player_player.mesh[4].trans = T_4;
	act_player_player.mesh[4].scale = S_4;

	act_player_player.mesh[5].rot = R_5;
	act_player_player.mesh[5].trans = T_5;
	act_player_player.mesh[5].scale = S_5;

	actorLoadPtr(&act_player_player, "\\ACT\\PLAYER.TIM;1");

	camInit(&player.camera);
}

void playerUpdate(void) {
	camUpdate(&player.camera);
}
