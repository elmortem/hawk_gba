#include "state.h"
#include "gba_defs.h"
#include "audio.h"
#include "res_sounds.h"

// Vars

int GameState::rndCount = 0;
int GameState::musicSoundSlot = -1;
GameState *GameState::curState = nullptr;
GameState *GameState::nextState = nullptr;
int GameState::nextIParam = 0;
void *GameState::nextVParam = nullptr;
int GameState::pendingCountdown = 0;

// GameState

void GameState::setNext( GameState *state, int iParam,
							void *vParam, int countdown )
{
	if ( !nextState )
	{
		nextState = state;
		nextIParam = iParam;
		nextVParam = vParam;
		pendingCountdown = countdown;
	}
};

void GameState::doTick()
{
	if ( nextState )
	{
		if ( pendingCountdown <= 0 )
		{
			if ( curState )
				curState->deactivate();

			REG_DISPCNT |= MODE_DISPLAY_OFF;

			srand (rndCount + 1234567);
			rndCount = 0;
			curState = nextState;
			nextState = nullptr;
			curState->activate( nextIParam, nextVParam );

			VBlankIntrWait();

			REG_DISPCNT &= ~MODE_DISPLAY_OFF;
			soundPlayOnce( Snd_State );
		}
		else
		{
			pendingCountdown--;
		}
	}
	if ( curState )
		curState->tick();

    rndCount++;
};

void GameState::playMusic( int clip, int volume )
{
    if ( musicSoundSlot >= 0 )
    {
        if ( soundIsPlaying( musicSoundSlot, clip ) )
        {
            return;
        }
        soundStop( musicSoundSlot );
        musicSoundSlot = -1;
    }

    musicSoundSlot = soundPlay( clip, SoundModeRepeat, volume );
}

void GameState::stopMusic()
{
    if ( musicSoundSlot >= 0 )
    {
        soundStop( musicSoundSlot );
        musicSoundSlot = -1;
    }
}


