#include "state_meta.h"
#include "video.h"
#include "audio.h"
#include "gamedata.h"
#include "res_graphics.h"
#include "res_sounds.h"
#include "state_gameplay.h"
#include "state_mainmenu.h"
#include <string>

TileSet metaTileSet
{
    gr_meta_til, gr_meta_til_size,
    gr_meta_opt, gr_meta_opt_size,
    gr_meta_pal, gr_meta_pal_size,

    gr_sprites_til, gr_sprites_til_size,
    gr_sprites_pal, gr_sprites_pal_size,

    0
};

GameMapLayerDesc gmlMetaBg =
{
    TILE_MAP2,
    map_meta_bg_width, map_meta_bg_height,
    map_meta_bg
};

GameMapLayerDesc gmlMetaFg =
{
    TILE_MAP1,
    map_meta_fg_width, map_meta_fg_height,
    map_meta_fg
};

GameStateMeta gameStateMeta;


std::string int2str( int n )
{
    char buf[10];
    write2buf w2b( buf );
    w2b << n;
    return std::string( buf );
}

void GameStateMeta::activate( int iParam, void *vParam )
{
    //setDefaultVideoMode();

	applyTileSet( &metaTileSet );
	resetGameMapLayer( &gmlMetaBg );
	resetGameMapLayer( &gmlMetaFg );

	refreshGameMapLayer( &gmlMetaBg, true );
	refreshGameMapLayer( &gmlMetaFg, true );

	gameData.calc( false );

	// check can levelup or buy
	cursor = 3;
	missionCursor = gameData.missionAccess;

	redraw();
}

void GameStateMeta::deactivate()
{
    fillTileMap( TILE_MAP0, 0 );
    fillTileMap( TILE_MAP1, 30 );
    fillTileMap( TILE_MAP2, 30 );
}

void GameStateMeta::tick()
{
    bool needRedraw = false;

	if ( !pending() )
	{
	    int lastCursor = cursor;
	    int lastMissionCursor = missionCursor;

	    if ( keyIsPressed( KEY_UP ) )
		{
		    soundPlayOnce( Snd_Touch );
		    cursor--;
		    if ( cursor < 0 )
                cursor = 3;
		};
		if ( keyIsPressed( KEY_DOWN ) )
		{
		    soundPlayOnce( Snd_Touch );
		    cursor++;
		    if ( cursor > 3 )
                cursor = 0;
		};

		// missions
        if ( keyIsPressed( KEY_L ) || keyIsPressed( KEY_LEFT ) )
        {
            soundPlayOnce( Snd_Touch );
            missionCursor--;
            if ( missionCursor < 0 )
                missionCursor = 0;
            cursor = 3;
        };
        if ( keyIsPressed( KEY_R ) || keyIsPressed( KEY_RIGHT ) )
        {
            soundPlayOnce( Snd_Touch );
            missionCursor++;
            if ( missionCursor > 4 )
                missionCursor = 4;
            if ( missionCursor > gameData.missionAccess )
                missionCursor = gameData.missionAccess;
            cursor = 3;
        };

		if ( keyIsPressed( KEY_A ) )
		{
		    if ( cursor == 0 )
            {
                if(gameData.buyLevel())
                {
                    needRedraw = true;
                    soundPlayOnce( Snd_LevelUp );
                }
                else
                {
                    soundPlayOnce( Snd_Fail );
                }
            }
            else if ( cursor == 1 )
            {
                if( gameData.buyShield() )
                {
                    needRedraw = true;
                    soundPlayOnce( Snd_Buy );
                }
                else
                {
                    soundPlayOnce( Snd_Fail ); // fail
                }
            }
            else if ( cursor == 2 )
            {
                if( gameData.buyArmor() )
                {
                    needRedraw = true;
                    soundPlayOnce( Snd_Buy ); // buy
                }
                else
                {
                    soundPlayOnce( Snd_Fail ); // fail
                }
            }
            else if ( cursor == 3 ) // play
            {
                soundPlayOnce( Snd_Touch );

                gameData.save();
                GameState::setNext( &gameStateGameplay, missionCursor, nullptr, 30 );
            }
		};

		if ( keyIsPressed( KEY_B ) )
        {
            soundPlayOnce( Snd_Touch );

            gameData.save();
            GameState::setNext( &gameStateMainmenu, 0, nullptr, 30 );
        }

		needRedraw = needRedraw || cursor != lastCursor || missionCursor != lastMissionCursor;
	};

	if(needRedraw)
    {
        redraw();
    }
}

void GameStateMeta::redraw()
{
    const char* txtInf = "%&";
    const char* txtMax = "(MAX)";

    fillTileMap( TILE_MAP0, 0 );

    // resources
    std::string s = int2str( gameData.playerResEssence );
    printString( 5, 2, s.c_str(), 0 );
    s = int2str( gameData.playerResLiquid );
    printString( 5, 4, s.c_str(), 0 );
    s = int2str( gameData.playerResOre );
    printString( 5, 6, s.c_str(), 0 );


    // params
    s = int2str( gameData.playerLevel );
    if ( gameData.playerLevel >= 50 )
        s += txtMax;
    printString( 15, 1, s.c_str(), 0 );

    s = int2str( gameData.playerShield / 60 )+"/"+int2str( gameData.playerShieldMax / 60 );
    printString( 15, 3, s.c_str(), 0 );

    s = int2str( unit2pix(gameData.playerArmor) )+"/"+int2str( unit2pix(gameData.playerArmorMax) );
    printString( 15, 5, s.c_str(), 0 );

    s = int2str( unit2pix(gameData.playerDamageMax) );
    printString( 15, 7, s.c_str(), 0 );

    // price
    s = gameData.playerLevel < 50 ? int2str( gameData.priceBuyLevel() ) : txtInf;
    printString( 24, 1, s.c_str(), 0 );
    s = int2str( gameData.priceBuyShield() );
    printString( 24, 3, s.c_str(), 0 );
    s = int2str( gameData.priceBuyArmor() );
    printString( 24, 5, s.c_str(), 0 );


    // cursor
    if ( cursor == 0 )
        redrawCursor( 23, 1 );
    else if ( cursor == 1 )
        redrawCursor( 23, 3 );
    else if ( cursor == 2 )
        redrawCursor( 23, 5 );
    else if ( cursor == 3 )
        redrawCursor( 23, 18 );

    // mission cursor & title
    redrawMissionPoint( 0, 5, 13 );
    redrawMissionPoint( 1, 10, 11 );
    redrawMissionPoint( 2, 14, 15 );
    redrawMissionPoint( 3, 19, 12 );
    redrawMissionPoint( 4, 24, 15 );
}

void GameStateMeta::redrawCursor(int x, int y)
{
    printTile( x - 1, y, 135 );
    printTile( x + 4, y, 136 );
}

void GameStateMeta::redrawMissionPoint(int num, int x, int y)
{
    const char* txtL = "L";
    const char* txtR = "R";
    const char* txtMission1 = "DEEP SPACE";
    const char* txtMission2 = "EVIL PLANET";
    const char* txtMission3 = "COMMUTATOR";
    const char* txtMission4 = "SPACEPORT";
    const char* txtMission5 = "LAST BATTLE";

    if ( num <= gameData.missionAccess )
    {
        std::string s = int2str( num + 1 );
        printString( x, y, s.c_str(), 0 );
    }

    // selected
    if ( num == missionCursor )
    {
        printTile( x - 1, y, 71 );
        printTile( x + 1, y, 103 );
        printTile( x, y - 1, 104 );
        printTile( x, y + 1, 72 );
        if ( num > 0 )
            printString( x - 2, y, txtL, 0 );
        if ( num < 4 )
            printString( x + 2, y, txtR, 0 );

        // title
        if ( num == 0 )
        {
            printString( 3, 18, txtMission1, 0 );
        }
        else if ( num == 1 )
        {
            printString( 3, 18, txtMission2, 0 );
        }
        else if ( num == 2 )
        {
            printString( 3, 18, txtMission3, 0 );
        }
        else if ( num == 3 )
        {
            printString( 3, 18, txtMission4, 0 );
        }
        else if ( num == 4 )
        {
            printString( 3, 18, txtMission5, 0 );
        }
    }
}

