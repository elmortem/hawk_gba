#include "state_mainmenu.h"
#include "video.h"
#include "audio.h"
#include "gamedata.h"
#include "res_graphics.h"
#include "res_sounds.h"
#include "state_meta.h"
#include "state_dialog.h"
#include "state_gameplay.h"
#include "state_end.h"

TileSet mainmenuTileSet
{
    gr_mainmenu_til, gr_mainmenu_til_size,
    gr_mainmenu_opt, gr_mainmenu_opt_size,
    gr_mainmenu_pal, gr_mainmenu_pal_size,

    gr_sprites_til, gr_sprites_til_size,
    gr_sprites_pal, gr_sprites_pal_size,

    0
};

GameMapLayerDesc gmlMainmenuBg =
{
    TILE_MAP2,
    map_mainmenu_bg_width, map_mainmenu_bg_height,
    map_mainmenu_bg,
    true
};

GameMapLayerDesc gmlMainmenuFg =
{
    TILE_MAP1,
    map_mainmenu_fg_width, map_mainmenu_fg_height,
    map_mainmenu_fg,
    true
};

GameStateMainmenu gameStateMainmenu;

void GameStateMainmenu::activate( int iParam, void *vParam )
{
    cursor = 0;

	applyTileSet( &mainmenuTileSet );
	resetGameMapLayer( &gmlMainmenuBg );
	resetGameMapLayer( &gmlMainmenuFg );

	fillTileMap( TILE_MAP0, 0 );

	refreshGameMapLayer( &gmlMainmenuBg, true );
	refreshGameMapLayer( &gmlMainmenuFg, true );

	playMusic( Snd_Lobby, 0x30 );
}

void GameStateMainmenu::deactivate()
{
    fillTileMap( TILE_MAP0, 30 );
    fillTileMap( TILE_MAP1, 30 );
    fillTileMap( TILE_MAP2, 30 );
    resetSprites(0, 128);
}

void GameStateMainmenu::tick()
{
    int scrollSpeed = pix2unit( 2 ) / 3;
    const char* txtNewGame = "NEW GAME";
    const char* txtContinue = "CONTINUE";
    const char* txtAuthors = "AUTHORS";

	if ( !pending() )
	{
	    if ( keyIsPressed( KEY_UP ) )
		{
		    soundPlayOnce( Snd_Touch );
		    cursor--;
		    if ( cursor < 0 )
                cursor = 2;
		};
		if ( keyIsPressed( KEY_DOWN ) )
		{
		    soundPlayOnce( Snd_Touch );
		    cursor++;
		    if ( cursor > 2 )
                cursor = 0;
		};

		if ( keyIsPressed( KEY_A ) )
		{
		    auto snd = Snd_Click;

		    if ( cursor == 0 )
            {
                gameData.reset();
                //GameState::setNext( &gameStateDialog, 0, nullptr, 10 );
                GameState::setNext( &gameStateMeta, 0, nullptr, 10 );
            }
            else if ( cursor == 1 )
            {
                if ( gameData.hasSave() )
                {
                    gameData.load();
                    GameState::setNext( &gameStateMeta, 0, nullptr, 10 );
                }
                else
                {
                    snd = Snd_Fail;
                }
            }
            else if ( cursor == 2 )
            {
                GameState::setNext( &gameStateEnd, 0, nullptr, 10 );
            }

            soundPlayOnce( snd );
		};
	};

	scrollBg -= scrollSpeed;

	gmlMainmenuBg.backgroundY = unit2pix( scrollBg );
	refreshGameMapLayer( &gmlMainmenuBg, false );
	refreshGameMapLayer( &gmlMainmenuFg, false );

	int mX = 11;
	int mY = 15;
	printString( mX, mY + 0, txtNewGame, 0 );
	printString( mX, mY + 1, txtContinue, 0 );
	printString( mX, mY + 2, txtAuthors, 0 );

	for( int i = 0; i < 3; ++i )
    {
        printTile( mX - 1, mY + i, cursor == i ? 67 : 0 );
    }
}
