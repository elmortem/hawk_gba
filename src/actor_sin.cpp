#include "actor_sin.h"
#include "actor_bullet.h"
#include "actor_resource.h"
#include "mission.h"
#include "gamedata.h"
#include "audio.h"
#include "res_sounds.h"

IMPLEMENT_READABLE( ActorSin, "sin" )

ActorSin::ActorSin()
{
    width = pix2unit( 32 );
    height = pix2unit( 32 );
    hitOffset = pix2unit( 4 );
    group = GameGroupEnemy;

    level = currentMission.getLevel();
    health = pix2unit( GameData::calcHealth( level + 3 ) );
    damage = pix2unit( GameData::calcDamage( level - 1 ) );

    dir = rand()%2==0?-1:1;
    speed = pix2unit( 1 ) / 3;
    speedH = pix2unit( 1 ) / 2 + pix2unit( level ) / 30;
    weaponTime = 60 - level / 3;
    weaponTimer = 0;
}

void ActorSin::read( ActorReader &reader )
{
    ActorShip::read( reader );
    startX = posX;
    length = pix2unit( reader.readInt( "length", 30 ) );
}

void ActorSin::tick()
{
    ActorEnemy::tick();

    int vx = dir > 0 ? speedH : -speedH;
    int vy = speed;

    posX += vx;
    posY += vy;

    if ( std::abs( posX - startX ) > length )
        dir = -dir;

    if ( weaponTimer > 0 )
        weaponTimer--;
    if ( weaponTimer <= 0 )
    {
        ActorBullet* bullet = new ActorBullet( uCenterX(), uCenterY(), 0, speed + pix2unit( 1 ) / 2, 3, GameGroupEnemyBullet, damage );
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

void ActorSin::draw()
{
    if ( blink % 5 == 0 )
        drawSprite( pLeft(), pTop(), 10, SpriteSize16x16, 1 );
};

void ActorSin::takeDamage( int damage, Actor *who )
{
    ActorEnemy::takeDamage( damage, who );

    if ( health <= 0 && rand() % 2 == 0 )
    {
        ActorResource* res = new ActorResource( uCenterX(), uCenterY(), -1, rand() % 3 + 1 );
        actors.addActor( res, this );
    }
}


