#include "actor_ship.h"
#include "actor_expl.h"
#include "mission.h"
#include "audio.h"
#include "res_sounds.h"

ActorShip::ActorShip()
{
     blink = 0;
}

void ActorShip::read( ActorReader &reader )
{
    int hw = pix2unit( reader.readInt( "width", 100 ) ) / 2;
    int hh = pix2unit( reader.readInt( "height", 100 ) ) / 2;
    posX = pix2unit( reader.readInt( "x", 100 ) ) + hw - width / 2;
    posY = pix2unit( reader.readInt( "y", 100 ) ) - currentMission.uLine() + hh - height / 2;
}

void ActorShip::deactivate()
{
    markDeleted();
}

void ActorShip::takeDamage( int damage, Actor *who )
{
    blink = damage > 0 ? 15 : 5;

    health -= damage;
    if ( health <= 0 )
    {
        health = 0;
        if ( explType >= 0 )
        {
            ActorExpl* expl = new ActorExpl( uCenterX(), uCenterY(), explType );
            actors.addActor( expl, this );
        }
        soundPlayOnce( sndDeath );
        markDeleted();
    }
}

void ActorShip::tick()
{
    if ( blink > 0 )
        blink--;
}
