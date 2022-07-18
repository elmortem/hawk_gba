#include "state_end.h"
#include "video.h"
#include "gamedata.h"
#include "res_graphics.h"
#include "state_mainmenu.h"
#include <string>

TileSet endTileSet
{
    gr_end_til, gr_end_til_size,
    gr_end_opt, gr_end_opt_size,
    gr_end_pal, gr_end_pal_size,

    gr_sprites_til, gr_sprites_til_size,
    gr_sprites_pal, gr_sprites_pal_size,

    0
};

GameMapLayerDesc gmlEndBg =
{
    TILE_MAP2,
    map_end_bg_width, map_end_bg_height,
    map_end_bg
};

GameMapLayerDesc gmlEndFg =
{
    TILE_MAP1,
    map_end_fg_width, map_end_fg_height,
    map_end_fg
};

GameStateEnd gameStateEnd;

void GameStateEnd::activate( int iParam, void *vParam )
{
	applyTileSet( &endTileSet );

	fillTileMap( TILE_MAP0, 0 );
	refreshGameMapLayer( &gmlEndBg, true );
	refreshGameMapLayer( &gmlEndFg, true );
}

void GameStateEnd::deactivate()
{
    fillTileMap( TILE_MAP0, 30 );
    fillTileMap( TILE_MAP1, 30 );
    fillTileMap( TILE_MAP2, 30 );
}

void GameStateEnd::tick()
{
	if ( !pending() )
	{
		if ( keyIsPressed( KEY_A ) || keyIsPressed( KEY_B ) || keyIsPressed( KEY_START ) || keyIsPressed( KEY_SELECT ) )
		{
		    GameState::setNext( &gameStateMainmenu, 0, nullptr, 10 );
            return;
		};
	};
}

