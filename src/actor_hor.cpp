#include "actor_hor.h"
#include "actor_bullet.h"
#include "actor_resource.h"
#include "mission.h"
#include "gamedata.h"
#include "audio.h"
#include "res_sounds.h"

IMPLEMENT_READABLE( ActorHor, "hor" )

ActorHor::ActorHor()
{
    width = pix2unit( 16 );
    height = pix2unit( 16 );
    hitOffset = pix2unit( 2 );
    group = GameGroupEnemy;

    level = currentMission.getLevel();
    health = pix2unit( GameData::calcHealth( level + 15 ) );
    damage = pix2unit( GameData::calcDamage( level - 5 ) );

    speed = pix2unit( 1 ) / 2 + rand() % pix2unit( 1 );
    weaponTime = 50 - level / 2;
    weaponTimer = 0;
}

void ActorHor::read( ActorReader &reader )
{
    ActorEnemy::read( reader );

    dir = reader.readInt( "dir", 1 );
    if ( dir == 0 )
        dir = rand()%2==0?-1:1;

    if ( dir > 0 && pRight() > 0 )
        posX = -pix2unit( 16 );
    if ( dir < 0 && pLeft() < videoWidth )
        posX = pix2unit( videoWidth );

    hor = reader.readInt( "hor", 0 );

    if ( hor != 0 )
    {
        posY = pix2unit( rand() % ( videoHeight / 2 ) );
    }
}

void ActorHor::tick()
{
    ActorEnemy::tick();

    if ( hor == 0 )
        posY += currentMission.uSpeed();

    if ( pBottom() < 0 )
        return;

    posX += speed * dir;

    if ( weaponTimer > 0 )
        weaponTimer--;
    if ( weaponTimer <= 0 )
    {
        ActorBullet* bullet = new ActorBullet( uCenterX(), uCenterY(), 0, speed, 2, GameGroupEnemyBullet, damage );
        actors.addActor( bullet, this );

        soundPlayOnce( Snd_Shoot );

        weaponTimer = weaponTime;
    }

    for ( auto it: ActorsRange( hitLeft(), hitTop(), hitWidth(), hitHeight(), GameGroupCollideOfEnemy, this ) )
    {
        it->takeDamage( getDamage(), this );
        takeDamage( it->getDamage(), it );
    };
};

void ActorHor::draw()
{
    if ( blink % 5 == 0 )
        drawSprite( pLeft(), pTop(), 74 + ( dir > 0 ? 0 : 32 ), SpriteSize16x16, 1 );
};

void ActorHor::takeDamage( int damage, Actor *who )
{
    ActorEnemy::takeDamage( damage, who );

    if ( health <= 0 && rand() % 3 == 0 )
    {
        ActorResource* res = new ActorResource( uCenterX(), uCenterY(), -1, rand() % 3 + 1 );
        actors.addActor( res, this );
    }
};


