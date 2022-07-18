#include "actor_bullet.h"
#include "mission.h"

ActorBullet::ActorBullet( int x, int y, int vx, int vy, int subtype, int group, int damage )
{
    this->global = true;
    this->width = pix2unit( 8 );
	this->height = ( subtype == 0 || subtype == 16 || subtype == 2 || subtype == 18 ) ? pix2unit( 8 ) : pix2unit( 16 );
	this->posX = x - (width >> 1);
	this->posY = y - (height >> 1);
	this->hitOffset = ( subtype == 0 || subtype == 16 ) ? -pix2unit( 2 ) : pix2unit( 2 );
	this->vx = vx;
	this->vy = vy;
	this->subtype = subtype;
	this->group = group;
	this->damage = damage;
}

void ActorBullet::activate()
{
    this->global = false;
}

void ActorBullet::deactivate()
{
    markDeleted();
}

void ActorBullet::takeDamage( int damage, Actor *who )
{
    markDeleted();
}

void ActorBullet::tick()
{
    posX += vx;
    posY += vy;
};

void ActorBullet::draw()
{
    int spr = 65 + subtype;
    SpriteSize size = SpriteSize8x16;
    if ( subtype == 0 || subtype == 16 || subtype == 2 || subtype == 18 )
        size = SpriteSize8x8;
    drawSprite( pLeft(), pTop(), spr, size, 1 );
};

