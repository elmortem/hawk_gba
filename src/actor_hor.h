#ifndef ACTOR_HOR_H
#define ACTOR_HOR_H

#include "actor.h"
#include "actor_enemy.h"

class ActorHor: public ActorEnemy
{
	DECLARE_READABLE

private:
    int dir;
    int hor;
    int speed;
    int weaponTime;
    int weaponTimer;

public:
	ActorHor();
	~ActorHor() {}

	void activate() override {};

	void tick() override;
	void draw() override;
	void takeDamage( int damage, Actor *who ) override;
};

#endif // ACTOR_HOR_H


