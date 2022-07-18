#ifndef ACTOR_SPAWNER_H
#define ACTOR_SPAWNER_H

#include "actor.h"

class ActorSpawner: public Actor
{
	DECLARE_READABLE

public:
    static int waitCount;

private:
	const char * const *readerData;

public:
	ActorSpawner() { global = true; };
	void activate() override {};
	void deactivate() override {};

	void tick() override;
};

#endif // ACTOR_SPAWNER_H
