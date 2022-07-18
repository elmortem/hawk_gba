#include "actor_expl.h"
#include "mission.h"

ActorExpl::ActorExpl( int x, int y, int type)
{
    width = 0;
    height = 0;
    posX = x;
	posY = y;
    hitOffset = 0;
    group = GameGroupNone;

    if ( type == 0 )
        timer = 16;
    else
        timer = 40;
}

void ActorExpl::read( ActorReader &reader )
{

}

void ActorExpl::tick()
{
    posY += currentMission.uSpeed();
    timer--;
    if ( timer <= 0 )
    {
        markDeleted();
    }
};

void ActorExpl::draw()
{
    if ( blink++ % 2 == 0 )
    {
        int rX = rand() % 8 - 4;
        int rY = rand() % 8 - 4;
        if ( timer > 16 && timer <= 28 )
            drawSprite( unit2pix( posX ) - 16 + rX, unit2pix( posY ) - 16 + rY, 101, SpriteSize32x32, 1 );
        else
            drawSprite( unit2pix( posX ) - 8 + rX, unit2pix( posY ) - 8 + rY, 92, SpriteSize16x16, 1 );
    }
};


