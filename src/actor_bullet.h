#ifndef ACTOR_BULLET_H
#define ACTOR_BULLET_H

#include "actor.h"

class ActorBullet: public Actor
{
private:
    int vx, vy;
    int damage;
public:
	ActorBullet( int x, int y, int vx, int vy, int subtype, int group, int damage );
	~ActorBullet() {}
	void read( ActorReader &reader ) override {};

	void activate() override;
	void deactivate() override;

	int getDamage() override { return damage; }
	void takeDamage( int damage, Actor *who ) override;

	void tick() override;
	void draw() override;
};

#endif // ACTOR_BULLET_H

