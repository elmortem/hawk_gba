#ifndef STATE_END_H
#define STATE_END_H

#include "state.h"

class GameStateEnd: public GameState
{
public:
	void activate( int iParam, void *vParam ) override;
	void deactivate() override;

	void tick() override;
};

extern GameStateEnd gameStateEnd;

#endif // STATE_END_H

