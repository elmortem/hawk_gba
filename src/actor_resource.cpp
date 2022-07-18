#include "actor_resource.h"
#include "gamedata.h"
#include "mission.h"
#include "audio.h"
#include "res_sounds.h"

IMPLEMENT_READABLE( ActorResource, "resource" )

ActorResource::ActorResource()
{
    global = true;
    width = pix2unit( 8 );
    height = pix2unit( 8 );
    hitOffset = -pix2unit( 2 );
    group = GameGroupItems;

    health = 1;
    damage = 0;

    sndDeath = Snd_Bonus;
    explType = -1;
}

ActorResource::ActorResource( int centerX, int centerY, int subtype, int count )
{
    global = true;
    width = pix2unit( 8 );
    height = pix2unit( 8 );
    hitOffset = -pix2unit( 2 );
    group = GameGroupItems;

    health = 1;
    damage = 0;

    sndDeath = Snd_Bonus;
    explType = -1;

    posX = centerX - width / 2;
    posY = centerY - height / 2;
    this->subtype = subtype;
    this->count = count;
}

void ActorResource::read( ActorReader &reader )
{
    ActorShip::read( reader );

    subtype = reader.readInt("subtype", -1);
    count = 1 + rand() % reader.readInt("countMax", 0);
}

void ActorResource::takeDamage( int damage, Actor *who )
{
    int res = subtype;
    if ( res < 0 )
        res = rand() % 3;

    if( res == 0 )
        gameData.playerResEssence += count;
    else if ( res == 1 )
        gameData.playerResLiquid += count;
    else if ( res == 2 )
        gameData.playerResOre += count;

    ActorShip::takeDamage( damage, who );
}

void ActorResource::tick()
{
    posY += currentMission.uSpeed() * 2 / 3;

    if ( global && actors.actorIsInCenter( this ) )
    {
        global = false;
    }
};

void ActorResource::draw()
{
    drawSprite( pLeft(), pTop(), 77 + subtype, SpriteSize8x8, 1 );
};

