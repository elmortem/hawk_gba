#ifndef ACTOR_SHIP_H
#define ACTOR_SHIP_H

#include "actor.h"
#include "video.h"
#include "res_sounds.h"

class ActorShip: public Actor
{
protected:
    int level;
    int health = 1;
    int damage;
    int blink;
    SoundEntries sndDeath = Snd_Explode;
    int explType = 0;

public:
	ActorShip();
	/* virtual */ ~ActorShip() {};
	void read( ActorReader &reader ) override;

	void activate() override {};
	void deactivate() override;

	int getHealth() { return health; }

	bool needKeepFar() override { return global; };
	int getDamage() override { return damage; }
	void takeDamage( int damage, Actor *who ) override;

	void tick() override;
};

#endif // ACTOR_SHIP_H
