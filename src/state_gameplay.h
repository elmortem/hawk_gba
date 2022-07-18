#ifndef STATE_GAMEPLAY_H
#define STATE_GAMEPLAY_H

#include "state.h"

class GameStateGameplay: public GameState
{
private:
    int missionNumber;
    int lastPlayerHealth;
    int lastPlayerShield;

public:
	void activate( int iParam, void *vParam ) override;
	void deactivate() override;

	void tick() override;

	void redrawBar( int x, int y, int value, int max, int type );
};

extern GameStateGameplay gameStateGameplay;

#endif // STATE_GAMEPLAY_H
