#include "actor_bomb.h"
#include "actor_resource.h"
#include "mission.h"
#include "gamedata.h"

IMPLEMENT_READABLE( ActorBomb, "bomb" )

ActorBomb::ActorBomb()
{
    width = pix2unit( 32 );
    height = pix2unit( 32 );
    hitOffset = pix2unit( 6 );
    group = GameGroupEnemy;

    level = currentMission.getLevel();
    health = pix2unit( GameData::calcHealth( level * 5 ) );
    damage = pix2unit( GameData::calcDamage( level + 5 ) );
    explType = 1;
}

void ActorBomb::read( ActorReader &reader )
{
    ActorEnemy::read( reader );
}

void ActorBomb::tick()
{
    ActorEnemy::tick();

    posY += currentMission.uSpeed();

    for ( auto it: ActorsRange( hitLeft(), hitTop(), hitWidth(), hitHeight(), GameGroupCollideOfEnemy, this ) )
    {
        it->takeDamage( getDamage(), this );
        takeDamage( it->getDamage(), it );
    };
};

void ActorBomb::draw()
{
    if ( blink % 5 == 0 )
        drawSprite( pLeft(), pTop(), 2, SpriteSize32x32, 1 );
};

void ActorBomb::takeDamage( int damage, Actor *who )
{
    ActorEnemy::takeDamage( damage, who );

    if ( health <= 0 && rand() % 2 == 0 )
    {
        ActorResource* res = new ActorResource( uCenterX(), uCenterY(), -1, rand() % 3 + 1 );
        actors.addActor( res, this );
    }
}
