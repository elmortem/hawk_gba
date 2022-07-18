#include "actor_min.h"
#include "mission.h"
#include "gamedata.h"

IMPLEMENT_READABLE( ActorMin, "min" )

ActorMin::ActorMin()
{
    width = pix2unit( 8 );
    height = pix2unit( 8 );
    hitOffset = pix2unit( 1 );
    group = GameGroupEnemy;

    level = currentMission.getLevel();
    health = pix2unit( GameData::calcHealth( level * 3 ) );
    damage = pix2unit( GameData::calcDamage( level ) );
}

void ActorMin::read( ActorReader &reader )
{
    ActorEnemy::read( reader );
}

void ActorMin::tick()
{
    ActorEnemy::tick();

    posY += currentMission.uSpeed();

    for ( auto it: ActorsRange( hitLeft(), hitTop(), hitWidth(), hitHeight(), GameGroupCollideOfEnemy, this ) )
    {
        it->takeDamage( getDamage(), this );
        takeDamage( it->getDamage(), it );
    };
};

void ActorMin::draw()
{
    if ( blink % 5 == 0 )
        drawSprite( pLeft(), pTop(), 12, SpriteSize8x8, 1 );
};

