#ifndef STATE_META_H
#define STATE_META_H

#include "state.h"

class GameStateMeta: public GameState
{
private:
    int cursor;
    int missionCursor;
public:
	void activate( int iParam, void *vParam ) override;
	void deactivate() override;

	void tick() override;
	void redraw();
	void redrawCursor(int x, int y);
	void redrawMissionPoint(int num, int x, int y);
};

extern GameStateMeta gameStateMeta;

#endif // STATE_META_H
