#ifndef ACTOR_MIN_H
#define ACTOR_MIN_H

#include "actor.h"
#include "actor_enemy.h"

class ActorMin: public ActorEnemy
{
	DECLARE_READABLE

public:
	ActorMin();
	~ActorMin() {}

	void activate() override {};

	void tick() override;
	void draw() override;
};

#endif // ACTOR_MIN_H

