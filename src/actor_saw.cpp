#include "actor_saw.h"
#include "mission.h"
#include "gamedata.h"

IMPLEMENT_READABLE( ActorSaw, "saw" )

ActorSaw::ActorSaw()
{
    width = pix2unit( 16 );
    height = pix2unit( 16 );
    hitOffset = pix2unit( 4 );
    group = GameGroupEnemy;

    vx = 0;
    vy = 0;
    level = currentMission.getLevel();
    health = 1;
    damage = pix2unit( GameData::calcDamage( level ) );
    speed = pix2unit( 1 ) + pix2unit( level) / 30;
}

void ActorSaw::read( ActorReader &reader )
{
    ActorEnemy::read( reader );

    int dir = reader.readInt( "dir", 0 );
    if (dir == 0)
    {
        vy = speed;
    }
    else if (dir == 1)
    {
        vx = speed / 2;
        vy = speed;
    }
    else if (dir == -1)
    {
        vx = -speed / 2;
        vy = speed;
    }
}

void ActorSaw::tick()
{
    ActorEnemy::tick();

    posX += vx;
    posY += vy;
};

void ActorSaw::draw()
{
    drawSprite( pLeft(), pTop(), 32, SpriteSize16x16, 1 );
};

