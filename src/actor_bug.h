#ifndef ACTOR_BUG_H
#define ACTOR_BUG_H

#include "actor.h"
#include "actor_enemy.h"

class ActorBug: public ActorEnemy
{
	DECLARE_READABLE
private:
    int speed;

public:
	ActorBug();
	~ActorBug() {}

	void activate() override {};

	void tick() override;
	void draw() override;
	void takeDamage( int damage, Actor *who ) override;
};

#endif // ACTOR_BUG_H



