#include "state_dialog.h"
#include "video.h"
#include "audio.h"
#include "gamedata.h"
#include "res_graphics.h"
#include "res_sounds.h"
#include "state_mainmenu.h"
#include "state_gameplay.h"
#include "state_meta.h"
#include "state_end.h"
#include <string>

TileSet dialogTileSet
{
    gr_dialog_til, gr_dialog_til_size,
    gr_dialog_opt, gr_dialog_opt_size,
    gr_dialog_pal, gr_dialog_pal_size,

    gr_sprites_til, gr_sprites_til_size,
    gr_sprites_pal, gr_sprites_pal_size,

    0
};

GameMapLayerDesc gmlDialogBg =
{
    TILE_MAP2,
    map_dialog_bg_width, map_dialog_bg_height,
    map_dialog_bg
};

GameMapLayerDesc gmlDialogCh1 =
{
    TILE_MAP1,
    map_dialog_chr1_width, map_dialog_chr1_height,
    map_dialog_chr1
};

GameMapLayerDesc gmlDialogCh2 =
{
    TILE_MAP1,
    map_dialog_chr2_width, map_dialog_chr2_height,
    map_dialog_chr2
};

GameMapLayerDesc gmlDialogCh3 =
{
    TILE_MAP1,
    map_dialog_chr3_width, map_dialog_chr3_height,
    map_dialog_chr3
};

DialogDesc dialogs[6] =
{
    { // 0
        { // phrases
            { 0, "Sorry,I'm a little late\nfor the celebration."},
            { 1, "It looks like the enemies\ngot ahead of us." },
            { 0, "Let's clip their wings\na little." },
            { 0, "Trust me!" },
            { 1, "Are we sure we should\ngo there?" },
            { 0, "Yes..." },
            { 1, "Good luck for us!" }
        },
        7
    },
    // Deep Space
    { // 1
        { // phrases
            { 0, "Giant Evil planet..." },
            { 0, "I read a book about\nsomething like that." },
            { 0, "Cool,right?" },
            { 1, "Alarm!We are under massive\nattack!" },
            { 0, "Looks like we celebrated\nway too early." },
            { 1, "Enough jokes!" },
            { 1, "We have been searching\nfor agent." },
            { 0, "Come on!" }
        },
        8
    },
    // Evil Planet
    { // 2
        { // phrases
            { 2, "Please follow my\ncoordinates." },
            { 2, "I will provide you support\nin this mission." },
            { 0, "And who the hell are you?\nAgent?" },
            { 2, "I am a friend. The man\nof intelligence." },
            { 1, "So what is our current\nobjective?" },
            { 2, "We have long noticed that\nour enemy is constructing" },
            { 2, "battle commutator." },
            { 2, "It should be somewhere\nin this area." },
            { 0, "Not for long." }
        },
        9
    },
    // Commutator
    { // 3
        { // phrases
            { 0, "Commutator is destroyed." },
            { 1, "Great job!" },
            { 0, "So what are our chances\nto not die alone" },
            { 0, "on this Evil Planet?" },
            { 1, "Looks like we celebrated\nway too early.Again." },
            { 2, "Can you find the spaceport?" },
            { 0, "But why?" },
            { 2, "Space are closed.This may\nbe our only chance." },
            { 0, "Let's investigate it." }
        },
        9
    },
    // Spaceport
    { // 4
        { // phrases
            { 1, "Come in!Do you copy?We have\nseized the Spaceport!" },
            { 0, "Shine!Lets get out of here!" },
            { 2, "Not so fast,kitty!" },
            { 2, "You need to fight a way to\nyour army!" },
            { 1, "When was it different?" },
            { 0, "Do it!We hurry to the\nrescue!" }
        },
        6
    },
    // Last Battle
    { // 5
        { // phrases
            { 0, "Ah,homeland." },
            { 0, "As always,beautiful at\nthis time of year." },
            { 1, "Is battle finally over?" },
            { 0, "Yes.For now." },
            { 1, "But war is not over yet?" },
            { 0, "Exactly! See you in a next\nbattles!" }
        },
        6
    }
};


GameStateDialog gameStateDialog;

void GameStateDialog::activate( int iParam, void *vParam )
{
    dialog = iParam;
    phrase = 0;

	applyTileSet( &dialogTileSet );

	refreshGameMapLayer( &gmlDialogBg, true );

	redraw();

	playMusic( Snd_Lobby, 0x30 );
}

void GameStateDialog::deactivate()
{
    fillTileMap( TILE_MAP0, 30 );
    fillTileMap( TILE_MAP1, 30 );
    fillTileMap( TILE_MAP2, 30 );
}

void GameStateDialog::tick()
{
    bool needRedraw = false;

	if ( !pending() )
	{
	    int lastPhrase = phrase;

		if ( keyIsPressed( KEY_A ) )
		{
		    soundPlayOnce( Snd_Dialog );

		    phrase++;
		    if ( phrase >= dialogs[ dialog ].count )
            {
                if ( dialog == 0 )
                    GameState::setNext( &gameStateGameplay, 0, nullptr, 10 );
                else if ( dialog == 5 )
                    GameState::setNext( &gameStateEnd, 0, nullptr, 10 );
                else
                    GameState::setNext( &gameStateMeta, 0, nullptr, 10 );

                return;
            }
		};

		needRedraw = lastPhrase != phrase;
	};

	if(needRedraw)
    {
        redraw();
    }
}

void GameStateDialog::redraw()
{
    fillTileMap( TILE_MAP0, 0 );

    // character
    int character = dialogs[ dialog ].phrases[ phrase ].character;
    if ( character == 0 )
    {
        refreshGameMapLayer( &gmlDialogCh1, true );
    }
    else if ( character == 1 )
    {
        refreshGameMapLayer( &gmlDialogCh2, true );
    }
    else if ( character == 2 )
    {
        refreshGameMapLayer( &gmlDialogCh3, true );
    }

    printString( 2, 17, dialogs[ dialog ].phrases[ phrase ].message, 0 );
}


