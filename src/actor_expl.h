#ifndef ACTOR_EXPL_H
#define ACTOR_EXPL_H

#include "actor.h"

class ActorExpl: public Actor
{
private:
    int timer;
    int blink;

public:
	ActorExpl( int x, int y, int type );
	~ActorExpl() {}
	void read( ActorReader &reader );

	void activate() override {};
	void deactivate() override {};

	void tick() override;
	void draw() override;
};

#endif // ACTOR_EXPL_H


