#ifndef GAME_STATES_H
#define GAME_STATES_H


class GameState
{
protected:
    static int rndCount;
    static int musicSoundSlot;
	static GameState *curState;
	static GameState *nextState;
	static int nextIParam;
	static void *nextVParam;
	static int pendingCountdown;
	static bool inline pending() { return pendingCountdown > 0; };

public:
	static void setNext( GameState *state, int iParam = 0,
					void *vParam = nullptr, int countdown = 0 );
	static void doTick();
	static void playMusic( int clip, int volume );
	static void stopMusic();

	virtual void activate( int iParam, void *vParam ) = 0;
	virtual void tick() = 0;
	virtual void deactivate() = 0;
};

#endif
