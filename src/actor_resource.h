#ifndef ACTOR_RESOURCE_H
#define ACTOR_RESOURCE_H

#include "actor.h"
#include "actor_ship.h"

class ActorResource: public ActorShip
{
	DECLARE_READABLE

private:
    int count;

public:
	ActorResource();
	ActorResource( int centerX, int centerY, int subtype, int count );
	~ActorResource() {}

	void activate() override {};
	void deactivate() override {};

	void takeDamage( int damage, Actor *who ) override;
	void tick() override;
	void draw() override;
};

#endif // ACTOR_RESOURCE_H

