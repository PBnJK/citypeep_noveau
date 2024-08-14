/* Citypeep: player character */

#include "player.h"
#include "actor.h"
#include "camera.h"

CP_Player player = { 0 };

void playerInit(void) {
	// player.actor = &gActors[actorLoad("\\ACT\\CUBOID.ACT;1")];
	camInit(&player.camera);
}

void playerUpdate(void) {
	camUpdate(&player.camera);
}
