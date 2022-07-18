#include "mission.h"
#include "state_mainmenu.h"
#include "actor.h"

Mission currentMission;

void Mission::activate( MissionDesc* mission )
{
    this->mission = mission;

    //applyTileSet( &globalTileSet );

    line = pix2unit( mission->height * 8 - videoHeight );
    speed = pix2unit( 1 ) / 2;
    level = mission->levelStart + rand() % mission->levelRandom;

    resetGameMapLayer( &mission->background );
    resetGameMapLayer( &mission->foreground );

    actors.loadActors( mission->actors );

    mission->background.backgroundY = currentMission.uLine();
    mission->foreground.backgroundY = unit2pix( pix2unit( mission->background.backgroundY ) * mission->foregroundMult );
	refreshGameMapLayer( &mission->background, true );
	refreshGameMapLayer( &mission->foreground, true );
}

void Mission::deactivate()
{
    actors.clear();
    resetSprites(0, 128);
}

void Mission::tick()
{
    if ( line > 0 )
    {
        line -= speed;

        mission->background.backgroundY = pLine();
        mission->foreground.backgroundY = unit2pix( pix2unit( mission->background.backgroundY ) * mission->foregroundMult );
    }

    actors.tick();

    /*char buf[ 64 ];
    write2buf w2b( buf );
    w2b << "LINE=" << pLine() << " LVL=" << level << "          ";
	printString( 0, 1, buf, 0 );*/
}

void Mission::draw()
{
    refreshGameMapLayer( &mission->background, false );
    refreshGameMapLayer( &mission->foreground, false );

    actors.draw();
}
