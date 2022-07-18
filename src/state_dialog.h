#ifndef STATE_DIALOG_H
#define STATE_DIALOG_H

#include "state.h"

struct DialogPhraseDesc
{
    int character;
    const char* message;
};

struct DialogDesc
{
    DialogPhraseDesc phrases[10];
    int count;
};

class GameStateDialog: public GameState
{
private:
    int dialog;
    int phrase;
public:
	void activate( int iParam, void *vParam ) override;
	void deactivate() override;

	void tick() override;
	void redraw();
};

extern GameStateDialog gameStateDialog;

#endif // STATE_DIALOG_H

