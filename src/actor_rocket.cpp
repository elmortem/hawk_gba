#include "actor_rocket.h"
#include "mission.h"
#include "gamedata.h"
#include "audio.h"
#include "res_sounds.h"

IMPLEMENT_READABLE( ActorRocket, "rocket" )

ActorRocket::ActorRocket()
{
    width = pix2unit( 8 );
    height = pix2unit( 16 );
    hitOffset = 0;
    group = GameGroupEnemy;

    level = currentMission.getLevel();
    health = 1;
    damage = pix2unit( GameData::calcDamage( level ) );
    phase = 0;
    speed = pix2unit( 1 ) + rand() % pix2unit( level / 30 );
}

void ActorRocket::read( ActorReader &reader )
{
    ActorEnemy::read( reader );
}

void ActorRocket::tick()
{
    ActorEnemy::tick();

    if ( phase < 240 )
    {
        phase++;

        if ( phase == 240 )
        {
            soundPlayOnce( Snd_ShootPower );
        }
    }
    else
    {
        posY += speed;
    }
};

void ActorRocket::draw()
{
    if ( phase < 240 )
    {
        if ( ((tailBlink++ >> 3) & 1) )
            drawSprite( pLeft(), 5, 128, SpriteSize8x8, 1 );
    }
    else
    {
        drawSprite( pLeft(), pTop(), 96, SpriteSize8x16, 1 );
        if ( tailBlink++ % 5 == 0 )
            drawSprite( pLeft(), pTop()-16, 64, SpriteSize8x16, 1 );
    }
};

