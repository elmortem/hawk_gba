#include "actor_cow.h"
#include "gamedata.h"
#include "mission.h"
#include "audio.h"
#include "res_sounds.h"

IMPLEMENT_READABLE( ActorCow, "cow" )

ActorCow::ActorCow()
{
    global = true;
    width = pix2unit( 16 );
    height = pix2unit( 16 );
    hitOffset = -pix2unit( 2 );
    group = GameGroupItems;

    health = 1;
    damage = 0;

    sndDeath = Snd_Cow;
}

void ActorCow::read( ActorReader &reader )
{
    ActorShip::read( reader );
}

void ActorCow::takeDamage( int damage, Actor *who )
{
    ActorShip::takeDamage( damage, who );
}

void ActorCow::tick()
{
    posY += currentMission.uSpeed() * 2 / 3;

    if ( global && actors.actorIsInCenter( this ) )
    {
        global = false;
    }
};

void ActorCow::draw()
{
    drawSprite( pLeft(), pTop(), 97, SpriteSize16x16, 1 );
};


