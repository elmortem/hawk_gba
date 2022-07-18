#ifndef ACTOR_PLAYER_H
#define ACTOR_PLAYER_H

#include "actor.h"
#include "actor_ship.h"

class ActorPlayer: public ActorShip
{
	DECLARE_READABLE

private:
    int start;
    int velX;
    int velY;
    int shield;
    bool out;
    int weaponTime;
    int weaponTimer;
    bool powered;
    bool shielded;

public:
	ActorPlayer();
	~ActorPlayer() {}

	void deactivate() override;

	int getShield() { return shield; }
	bool isPowered() { return powered; }
	bool isShielded() { return shielded; }

	void tick() override;
	void draw() override;
	void takeDamage( int damage, Actor *who ) override;
	void goOut();
};

#endif // ACTOR_PLAYER_H
