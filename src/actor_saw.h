#ifndef ACTOR_SAW_H
#define ACTOR_SAW_H

#include "actor.h"
#include "actor_enemy.h"

class ActorSaw: public ActorEnemy
{
	DECLARE_READABLE

private:
    int vx, vy;
    int speed;

public:
	ActorSaw();
	~ActorSaw() {}

	void activate() override {};

	void tick() override;
	void draw() override;
};

#endif // ACTOR_SAW_H

