#include "actor_enemy.h"

void ActorEnemy::tick()
{
    ActorShip::tick();

    if ( global && actors.actorIsInCenter( this ) )
    {
        global = false;
    }
}
