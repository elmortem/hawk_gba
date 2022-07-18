#include "actor_spawner.h"
#include "mission.h"

IMPLEMENT_READABLE( ActorSpawner, "spawner" )

int ActorSpawner::waitCount = 0;

void ActorSpawner::read( ActorReader &reader )
{
	readerData = reader.getData();

	posX = pix2unit( reader.readInt( "x", 100 ) );
	posY = pix2unit( reader.readInt( "y", 100 ) );
	width = pix2unit( reader.readInt( "width", 16 ) );
	height = pix2unit( reader.readInt( "height", 16 ) );
};

void ActorSpawner::tick()
{
    if ( waitCount <= 0 && currentMission.uLine() - pix2unit( 32 ) < posY )
    {
        ActorReader reader( readerData );
        std::string type = reader.readStr( "spawnType" );

        Actor *actor = ActorsPool::createActor( type );
        if ( actor )
        {
            actor->read( reader );
            actors.addActor( actor, this );
        }
        else
        {
            fatalError( std::string( "UNKNOWN ACTOR TYPE: " ) + type );
        };

        markDeleted();
        waitCount = 10;
    }
};
