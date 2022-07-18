#ifndef ACTOR_COW_H
#define ACTOR_COW_H

#include "actor.h"
#include "actor_ship.h"

class ActorCow: public ActorShip
{
	DECLARE_READABLE

private:
    int count;

public:
	ActorCow();
	~ActorCow() {}

	void activate() override {};
	void deactivate() override {};

	void takeDamage( int damage, Actor *who ) override;
	void tick() override;
	void draw() override;
};

#endif // ACTOR_COW_H


