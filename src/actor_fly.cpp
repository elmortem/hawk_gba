#include "actor_fly.h"
#include "actor_bullet.h"
#include "actor_resource.h"
#include "mission.h"
#include "gamedata.h"
#include "audio.h"
#include "res_sounds.h"

IMPLEMENT_READABLE( ActorFly, "fly" )

ActorFly::ActorFly()
{
    width = pix2unit( 16 );
    height = pix2unit( 16 );
    hitOffset = pix2unit( 4 );
    group = GameGroupEnemy;

    level = currentMission.getLevel();
    health = pix2unit( GameData::calcHealth( level + 3 ) + 15 );
    damage = pix2unit( GameData::calcDamage( level ) + 5 );
    speed = pix2unit( 1 ) / 3 + pix2unit( level ) / 20;

    dir = rand()%2==0?-1:1;
    weaponTime = 40 - level / 2;
    weaponTimer = 0;
}

void ActorFly::read( ActorReader &reader )
{
    ActorEnemy::read( reader );
}

void ActorFly::tick()
{
    ActorEnemy::tick();

    posY += speed;

    if ( weaponTimer > 0 )
        weaponTimer--;
    if ( weaponTimer <= 0 )
    {
        ActorBullet* bullet = new ActorBullet( uCenterX(), uCenterY(), speed * dir, speed / 2, 18, GameGroupEnemyBullet, damage );
        actors.addActor( bullet, this );
        bullet = new ActorBullet( uCenterX(), uCenterY(), -speed * dir, speed * 2, 18, GameGroupEnemyBullet, damage );
        actors.addActor( bullet, this );

        soundPlayOnce( Snd_Shoot );

        weaponTimer = weaponTime;
        dir = -dir;
    }

    for ( auto it: ActorsRange( hitLeft(), hitTop(), hitWidth(), hitHeight(), GameGroupCollideOfEnemy, this ) )
    {
        it->takeDamage( getDamage(), this );
        takeDamage( it->getDamage(), it );
    };
};

void ActorFly::draw()
{
    if ( blink % 5 == 0 )
        drawSprite( pLeft(), pTop(), 42, SpriteSize16x16, 1 );
};

void ActorFly::takeDamage( int damage, Actor *who )
{
    ActorEnemy::takeDamage( damage, who );

    if ( health <= 0 && rand() % 3 == 0 )
    {
        ActorResource* res = new ActorResource( uCenterX(), uCenterY(), -1, rand() % 5 + 1 );
        actors.addActor( res, this );
    }
};
