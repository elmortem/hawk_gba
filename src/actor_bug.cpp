#include "actor_bug.h"
#include "actor_bullet.h"
#include "mission.h"
#include "gamedata.h"
#include "audio.h"
#include "res_sounds.h"

IMPLEMENT_READABLE( ActorBug, "bug" )

ActorBug::ActorBug()
{
    width = pix2unit( 32 );
    height = pix2unit( 32 );
    hitOffset = pix2unit( 8 );
    group = GameGroupEnemy;

    level = currentMission.getLevel();
    health = pix2unit( GameData::calcHealth( level ) );
    damage = pix2unit( GameData::calcDamage( level + 5 ) );
    speed = pix2unit( 2 ) / 5 + pix2unit( level ) / 40;

    explType = 1;
}

void ActorBug::read( ActorReader &reader )
{
    ActorEnemy::read( reader );
}

void ActorBug::tick()
{
    ActorEnemy::tick();

    posY += speed;

    for ( auto it: ActorsRange( hitLeft(), hitTop(), hitWidth(), hitHeight(), GameGroupCollideOfEnemy, this ) )
    {
        it->takeDamage( getDamage(), this );
        takeDamage( it->getDamage(), it );
    };
};

void ActorBug::draw()
{
    if ( blink % 5 == 0 )
        drawSprite( pLeft(), pTop(), 6, SpriteSize32x32, 1 );
};

void ActorBug::takeDamage( int damage, Actor *who )
{
    ActorEnemy::takeDamage( damage, who );

    if ( health <= 0 )
    {
        ActorBullet* bullet = new ActorBullet( uCenterX(), uCenterY(), 0, pix2unit( 2 ), 18, GameGroupEnemyBullet, damage );
        actors.addActor( bullet, this );
        bullet = new ActorBullet( uCenterX(), uCenterY(), pix2unit( 4 ) / 3, pix2unit( 4 ) / 3, 18, GameGroupEnemyBullet, damage );
        actors.addActor( bullet, this );
        bullet = new ActorBullet( uCenterX(), uCenterY(), -pix2unit( 4 ) / 3, pix2unit( 4 ) / 3, 18, GameGroupEnemyBullet, damage );
        actors.addActor( bullet, this );

        //bullet = new ActorBullet( uCenterX(), uCenterY(), pix2unit( 4 ) / 3, 0, 18, GameGroupEnemyBullet, damage );
        //actors.addActor( bullet, this );
        //bullet = new ActorBullet( uCenterX(), uCenterY(), -pix2unit( 4 ) / 3, 0, 18, GameGroupEnemyBullet, damage );
        //actors.addActor( bullet, this );

        //bullet = new ActorBullet( uCenterX(), uCenterY(), 0, -pix2unit( 2 ), 18, GameGroupEnemyBullet, damage );
        //actors.addActor( bullet, this );
        //bullet = new ActorBullet( uCenterX(), uCenterY(), pix2unit( 2 ) / 3, -pix2unit( 2 ) / 3, 18, GameGroupEnemyBullet, damage );
        //actors.addActor( bullet, this );
        //bullet = new ActorBullet( uCenterX(), uCenterY(), -pix2unit( 2 ) / 3, -pix2unit( 2 ) / 3, 18, GameGroupEnemyBullet, damage );
        //actors.addActor( bullet, this );
    }
}



