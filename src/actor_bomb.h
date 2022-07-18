#ifndef ACTOR_BOMB_H
#define ACTOR_BOMB_H

#include "actor.h"
#include "actor_enemy.h"

class ActorBomb: public ActorEnemy
{
	DECLARE_READABLE

public:
	ActorBomb();
	~ActorBomb() {}

	void activate() override {};

	void tick() override;
	void draw() override;
	void takeDamage( int damage, Actor *who ) override;
};

#endif // ACTOR_BOMB_H
