#ifndef ACTOR_ENEMY_H
#define ACTOR_ENEMY_H

#include "actor.h"
#include "actor_ship.h"

class ActorEnemy: public ActorShip
{
public:
	ActorEnemy() { global = true; }
	/* virtual */ ~ActorEnemy() {};

	void tick() override;
};

#endif // ACTOR_ENEMY_H


