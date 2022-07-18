#ifndef ACTOR_ROCKET_H
#define ACTOR_ROCKET_H

#include "actor.h"
#include "actor_enemy.h"

class ActorRocket: public ActorEnemy
{
	DECLARE_READABLE

private:
    int phase;
    int speed;
    int tailBlink;

public:
	ActorRocket();
	~ActorRocket() {}

	void activate() override {};

	void tick() override;
	void draw() override;
};

#endif // ACTOR_ROCKET_H
