#ifndef ACTOR_SIN_H
#define ACTOR_SIN_H

#include "actor.h"
#include "actor_enemy.h"

class ActorSin: public ActorEnemy
{
	DECLARE_READABLE

private:
    int startX;
    int length;
    int dir;
    int speed;
    int speedH;
    int weaponTime;
    int weaponTimer;

public:
	ActorSin();
	~ActorSin() {}

	void activate() override {};

	void tick() override;
	void draw() override;
	void takeDamage( int damage, Actor *who ) override;
};

#endif // ACTOR_SIN_H


