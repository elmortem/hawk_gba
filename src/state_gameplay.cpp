#include "state_gameplay.h"
#include "state_mainmenu.h"
#include "state_dialog.h"
#include "state_meta.h"
#include "video.h"
#include "audio.h"
#include "res_graphics.h"
#include "res_sounds.h"
#include "mission.h"
#include "actor.h"
#include "actor_player.h"
#include "actor_spawner.h"
#include "gamedata.h"
#include <string.h>

MissionDesc missions[ 5 ] =
{
    // mission1
    {
        map_mission1_bg_width, map_mission1_bg_height,
        {
            TILE_MAP2,
            map_mission1_bg_width, map_mission1_bg_height,
            map_mission1_bg,
            false
        },
        {
            TILE_MAP1,
            map_mission1_fg_width, map_mission1_fg_height,
            map_mission1_fg,
            true
        },
        2,
        map_mission1_obj,
        1,
        5
    },
    // mission2
    {
        map_mission2_bg_width, map_mission2_bg_height,
        {
            TILE_MAP2,
            map_mission2_bg_width, map_mission2_bg_height,
            map_mission2_bg,
            false
        },
        {
            TILE_MAP1,
            map_mission2_fg_width, map_mission2_fg_height,
            map_mission2_fg,
            true
        },
        1,
        map_mission2_obj,
        10,
        10
    },
    // mission3
    {
        map_mission3_bg_width, map_mission3_bg_height,
        {
            TILE_MAP2,
            map_mission3_bg_width, map_mission3_bg_height,
            map_mission3_bg,
            false
        },
        {
            TILE_MAP1,
            map_mission3_fg_width, map_mission3_fg_height,
            map_mission3_fg,
            true
        },
        1,
        map_mission3_obj,
        15,
        15
    },
    // mission4
    {
        map_mission4_bg_width, map_mission4_bg_height,
        {
            TILE_MAP2,
            map_mission4_bg_width, map_mission4_bg_height,
            map_mission4_bg,
            false
        },
        {
            TILE_MAP1,
            map_mission4_fg_width, map_mission4_fg_height,
            map_mission4_fg,
            true
        },
        2,
        map_mission4_obj,
        25,
        15
    },
    // mission5
    {
        map_mission5_bg_width, map_mission5_bg_height,
        {
            TILE_MAP2,
            map_mission5_bg_width, map_mission5_bg_height,
            map_mission5_bg,
            false
        },
        {
            TILE_MAP1,
            map_mission5_fg_width, map_mission5_fg_height,
            map_mission5_fg,
            true
        },
        1,
        map_mission5_obj,
        45,
        5
    }
};


GameStateGameplay gameStateGameplay;

void GameStateGameplay::activate( int iParam, void *vParam )
{
    missionNumber = iParam;

    lastPlayerShield = 0;
    lastPlayerHealth = 0;

    applyTileSet( &globalTileSet );
    fillTileMap( TILE_MAP0, 0 );

    currentMission.activate( &missions[ missionNumber ] );

	playMusic( Snd_Gameplay, 0x80 );
};

void GameStateGameplay::deactivate()
{
    fillTileMap( TILE_MAP0, 30 );
    fillTileMap( TILE_MAP1, 30 );
    fillTileMap( TILE_MAP2, 30 );

	currentMission.deactivate();
};

void GameStateGameplay::tick()
{
    if( !pending() )
    {
        if ( ActorSpawner::waitCount > 0 )
            ActorSpawner::waitCount--;

        currentMission.tick();

        if ( currentMission.uLine() <= 0 )
        {
            actors.player->goOut();
            if ( actors.player->pBottom() <= 0 )
            {
                int nowMissionAccess = missionNumber + 1;
                if ( nowMissionAccess > gameData.missionAccess )
                {
                    gameData.missionAccess = nowMissionAccess;
                    GameState::setNext( &gameStateDialog, gameData.missionAccess, nullptr, 10 );
                }
                else
                {
                    GameState::setNext( &gameStateMeta, 0, nullptr, 10 );
                }
                return;
            }
        }

        if ( actors.player->getHealth() <= 0 )
        {
            GameState::setNext( &gameStateMeta, 0, nullptr, 10 );
            return;
        }
    }

    // ui

    if ( lastPlayerShield != actors.player->getShield() )
    {
        lastPlayerShield = actors.player->getShield();
        redrawBar( 1, 13, lastPlayerShield, gameData.playerShieldMax, 0 );

        printTile( 1, 18, actors.player->isShielded() ? 13 : 9 );
    }

    if ( lastPlayerHealth != actors.player->getHealth() )
    {
        lastPlayerHealth = actors.player->getHealth();
        redrawBar( 27, 13, lastPlayerHealth, gameData.playerArmorMax, 1 );

        printTile( 27, 18, actors.player->isPowered() ? 14 : 11 );
    }

	beginSpriteDraw();
	currentMission.draw();
	endSpriteDraw();
};

inline int clamp( int v, int min, int max )
{
    if ( v < min )
        return min;
    if ( v > max )
        return max;
    return v;
}

void GameStateGameplay::redrawBar( int x, int y, int value, int max, int type )
{
    if ( max <= 0 )
    {
        value = max = 100;
    }

    int v = ((value * 100) / max) / 10;
    if ( v == 0 && value > 0 )
        v = 1;

    /*char buf[ 64 ];
    write2buf w2b( buf );
    w2b << "VAL=" << value << " MAX=" << max << " V=" << v << "          ";
	printString( 0, 5 + type, buf, 0 );*/

    int s = clamp(v - 8, 0, 2); if ( s > 0 ) s += type * 2;
    printTile(x, y, 102 + s);

    s = clamp(v - 6, 0, 2); if ( s > 0 ) s += type * 2;
    printTile(x, y + 1, 134 + s);

    s = clamp(v - 4, 0, 2); if ( s > 0 ) s += type * 2;
    printTile(x, y + 2, 134 + s);

    s = clamp(v - 2, 0, 2); if ( s > 0 ) s += type * 2;
    printTile(x, y + 3, 134 + s);

    s = clamp(v, 0, 2); if ( s > 0 ) s += type * 2;
    printTile(x, y + 4, 166 + s);
};
