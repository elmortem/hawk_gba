#include "actor_player.h"
#include "actor_bullet.h"
#include "mission.h"
#include "gamedata.h"
#include "state_mainmenu.h"
#include "audio.h"
#include "res_sounds.h"

IMPLEMENT_READABLE( ActorPlayer, "player" )

ActorPlayer::ActorPlayer()
{
    global = true;
    width = pix2unit( 16 );
    height = pix2unit( 16 );
    hitOffset = pix2unit( 4 );
    group = GameGroupPlayer;
	actors.player = this;

	level = gameData.playerLevel;
	health = gameData.playerArmor;
	shield = gameData.playerShield;
	damage = gameData.playerDamageMax;
	out = false;
	weaponTime = 30 - ( ( (level * 100) / 5 ) * 2 ) / 100;
	start = 50;
}

void ActorPlayer::read( ActorReader &reader )
{
    ActorShip::read( reader );
}

void ActorPlayer::deactivate()
{
    gameData.playerArmor = health;
	gameData.playerShield = shield;

	ActorShip::deactivate();
}

void ActorPlayer::tick()
{
    const int stepX = pix2unit( 3 ) / 2;
    const int stepY = pix2unit( 2 );

    ActorShip::tick();

    if ( out )
    {
        posY -= stepY / 3 * 4;
        return;
    }

    if ( keyIsDown( KEY_LEFT ) )
    {
        velX = -stepX;
    }
    else if ( keyIsDown( KEY_RIGHT ) )
    {
        velX = stepX;
    }
    else
    {
        velX = 0;
    }

    if ( keyIsDown( KEY_UP ) )
    {
        velY = -stepY;
    }
    else if ( keyIsDown( KEY_DOWN ) )
    {
        velY = stepY;
    }
    else
    {
        velY = 0;
    }

    if ( start > 0 )
    {
        start--;
    }
    else
    {
        //TODO if ( gameData.missionAccess >= 2 )
            powered = keyIsDown( KEY_R );
        //TODO if ( gameData.missionAccess >= 1 )
            shielded = keyIsDown( KEY_L );

        if ( powered && shield > 0 )
        {
            shield -= 1;
        }
        if ( shielded && shield > 0 )
        {
            shield -= 1;
        }
        if ( shield <= 0 )
        {
            shield = 0;
            powered = false;
            shielded = false;
        }


        //if ( shielded )
        //{
        //    health += pix2unit( 1 ) / 3;
        //}

        if ( weaponTimer > 0 )
            weaponTimer--;
        if ( weaponTimer <= 0 )
        {
            ActorBullet* bullet = new ActorBullet( uCenterX()-pix2unit( 7 ), uCenterY()-pix2unit( 1 ), 0, -pix2unit( 7 ) / 2, powered?1:0, GameGroupPlayerBullet, powered?damage*3:damage );
            actors.addActor( bullet, this );
            bullet = new ActorBullet( uCenterX()+pix2unit( 7 ), uCenterY()-pix2unit( 1 ), 0, -pix2unit( 7 ) / 2, powered?1:0, GameGroupPlayerBullet, powered?damage*3:damage );
            actors.addActor( bullet, this );

            soundPlayOnce( Snd_PlayerShoot, powered ? 0x0100 : 0x0080 );

            weaponTimer = weaponTime;
        }
    }

    posX += velX;
    if ( posX < 0 )
        posX = 0;
    if ( posX + width > pix2unit( videoWidth ) )
        posX = pix2unit( videoWidth ) - width;
    posY += velY;
    if ( posY < 0 )
        posY = 0;
    if ( posY + height > pix2unit( videoHeight ) )
        posY = pix2unit( videoHeight ) - height;

    for ( auto it: ActorsRange( hitLeft(), hitTop(), hitWidth(), hitHeight(),
								GameGroupCollideOfPlayer, this ) )
    {
        it->takeDamage( getDamage(), this );
        takeDamage( it->getDamage(), it );
    };
};

void ActorPlayer::draw()
{
    if ( blink % 5 == 0 )
        drawSprite( pLeft(), pTop() + start, 0, SpriteSize16x16, 1 );

    if ( shielded )
        drawSprite( pLeft(), pTop(), 110, SpriteSize16x16, 0 );

    /*char buf[ 64 ];
    write2buf w2b( buf );
    w2b << "Y=" << pTop() << "W=" << unit2pix( width ) << "H=" << unit2pix( height ) << "          ";
	printString( 0, 2, buf, 0 );*/
};

void ActorPlayer::takeDamage( int damage, Actor *who )
{
    if ( out )
        return;

    if ( shielded )
    {
        damage = damage / 5;
    }

    ActorShip::takeDamage( damage, who );
}

void ActorPlayer::goOut()
{
    out = true;
}
