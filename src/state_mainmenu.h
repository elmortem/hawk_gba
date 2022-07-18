#ifndef STATE_MAINMENU_H
#define STATE_MAINMENU_H

#include "state.h"

class GameStateMainmenu: public GameState
{
private:
    int cursor;
    int scrollBg;

public:
	void activate( int iParam, void *vParam ) override;
	void deactivate() override;

	void tick() override;
};

extern GameStateMainmenu gameStateMainmenu;

#endif // STATE_MAINMENU_H
