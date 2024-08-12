/* Citypeep: player character */

#include "player.h"
#include "actor.h"

CP_Player player = { 0 };

void playerInit(void) {
	player.actor = &gActors[actorLoad("\\ACT\\CUBOID.ACT;1")];
}
