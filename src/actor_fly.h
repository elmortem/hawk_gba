#ifndef ACTOR_FLY_H
#define ACTOR_FLY_H

#include "actor.h"
#include "actor_enemy.h"

class ActorFly: public ActorEnemy
{
	DECLARE_READABLE

private:
    int dir;
    int speed;
    int weaponTime;
    int weaponTimer;

public:
	ActorFly();
	~ActorFly() {}

	void activate() override {};

	void tick() override;
	void draw() override;
	void takeDamage( int damage, Actor *who ) override;
};

#endif // ACTOR_FLY_H
