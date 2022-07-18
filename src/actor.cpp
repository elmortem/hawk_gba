#include "actor.h"
#include "actor_player.h"
#include <algorithm>

static const int MaxActorWidth = 64;

// *** ActorReader

const char *ActorReader::findValue( const std::string &name )
{
	const char * const * cur = data;
	while ( *cur )
	{
		if ( name == *cur )
		{
			return cur[ 1 ];
		}
		else
		{
			cur += 2;
		};
	};
	return nullptr;
};

int ActorReader::readInt( const std::string &name, int defVal )
{
	int retVal = defVal;
	const char *val = findValue( name );
	if ( val )
		retVal = buf2int( val );
	return retVal;
};

std::string ActorReader::readStr( const std::string &name )
{
	const char *val = findValue( name );
	if ( !val )
		val = "";
	return std::string( val );
};


// *** Actor

int Actor::actorsCount = 0;

void Actor::setSpawner( Actor *pSpawner, int *pSpawnCount )
{
	spawner = pSpawner;
	spawnCount = pSpawnCount;
	if ( spawnCount )
		(*spawnCount)++;
}
void Actor::releaseSpawner()
{
	if ( spawnCount )
		(*spawnCount)--;
	spawnCount = nullptr;
	spawner = nullptr;
}

void Actor::deactivate()
{
	if ( spawner )
	{
		// Если нас породил спавнер - удаляемся при выходе за экран
		releaseSpawner();
		markDeleted();
	}
};

// *** ActorsPool

ActorsPool actors;

ActorsPool::ActorsCreators *ActorsPool::creators = nullptr;

void ActorsPool::registerCreator( const std::string &name,
								  ActorCreator creator )
{
	if ( !creators )
		creators = new ActorsCreators;
	creators->insert( make_pair( name, creator ) );
};

/* static */ Actor *ActorsPool::createActor( const std::string &className )
{
	Actor *retVal = nullptr;
	ActorsCreators::iterator iter = creators->find( className );
	if ( iter != creators->end() )
		retVal = (iter->second)();
	return retVal;
};

void ActorsPool::positionActor( Actor *actor )
{
	while ( actor->next && (actor->next->uTop() < actor->uTop()) )
	{
		actorsList.swapSiblings( actor, actor->next );
	};
	while ( actor->prev && (actor->prev->uTop() > actor->uTop()) )
	{
		actorsList.swapSiblings( actor->prev, actor );
	}
};

void ActorsPool::addActor( Actor *actor, Actor *before )
{
	actorsList.insert( actor, before );
	positionActor( actor );
	if ( actor->isGlobal() )
	{
		actor->setActive( true );
		activeActors.push_back( actor );
	}
};

void ActorsPool::clear()
{
	for ( auto &it: activeActors )
	{
		if ( it->isActive() )
			it->setActive( false );
	};
	activeActors.clear();
	actorsList.clear();
	baseActor = nullptr;
	player = nullptr;
};

void ActorsPool::loadActors( const char * const *data )
{
	char buf[ 30 * 20 ];
	write2buf w2b( buf );
	Actor *actor = nullptr;
	while ( *data )
	{
		ActorReader reader( data );
		std::string type = reader.readStr( "type" );

		actor = createActor( type );
		if ( actor )
		{
			actor->read( reader );
			addActor( actor, nullptr );
		}
		else
		{
			fatalError( std::string( "UNKNOWN ACTOR TYPE: " ) + type );
		};

		// skip to next
		while ( *data )
			data += 2;
		data++;
	};

	sortActors();
};

void ActorsPool::tick()
{
	manDeathSounds = 0;

	// Перепозиционировать базового актора
	if ( !baseActor )
		baseActor = actorsList.getFirst();
	if ( actorIsOnTop( baseActor.ptr() ) )
	{
		while ( baseActor && actorIsOnTop( baseActor.ptr() ) )
			baseActor = baseActor->next;
		if ( baseActor )
			baseActor = baseActor->prev;
	}
	else
	{
		while ( baseActor && !actorIsOnLeft( baseActor.ptr() ) )
			baseActor = baseActor->prev;
	};
	if ( !baseActor )
		baseActor = actorsList.getFirst();

	// Просканировать попадающих в экран акторов на предмет изменения состояния
	for ( Actor *actor = baseActor.ptr(); actor; actor = next( actor ) )
	{
		int hBorder = 0;
        int vBorder = 0;
        if ( actor->needKeepFar() )
        {
            hBorder = videoWidth;
            vBorder = videoHeight;
        }
		if ( actorIsInCenter( actor, hBorder, vBorder ) )
		{
			if ( !actor->isDeleted() && !actor->isActive() )
			{
				activeActors.push_back( actor );
				actor->setActive( true );
			};
		};
	}

	// Просканироать активных акторов на предмет изменений состояния
	unsigned int i = 0;
	while ( i < activeActors.size() )
	{
		auto &it = activeActors[ i ];
		if ( it->isDeleted() )
		{
			if ( it->isActive() )
				it->setActive( false );
			if ( ++(it->delCount) > 3 )
			{
				if ( baseActor == it )
				{
					baseActor = baseActor->prev;
				}
				actorsList.remove( it );
				activeActors.erase( activeActors.begin() + i );
				continue;
			};
		}
		else if ( !it->isGlobal() )
		{
			int hBorder = 0;
			int vBorder = 0;
			if ( it->needKeepFar() )
			{
				hBorder = videoWidth;
				vBorder = videoHeight;
			}
			if ( !actorIsInCenter( it.ptr(), hBorder, vBorder ) )
			{
				it->setActive( false );
				if ( !it->isDeleted() ) // если не пометился на удаление
				{
					activeActors.erase( activeActors.begin() + i );
					continue; // пропускаем инкремент i
				};
			};
		}
		i++;
	}

	// Подготовить активных акторов к перемещению
	/*for ( unsigned int i = 0; i < activeActors.size(); i++)
	{
		Actor *actor = activeActors[ i ].ptr();
		int oldUTop = actor->uTop();
		if ( !actor->isDeleted() )
		{
			actor->pretick();
			if ( oldUTop != actor->uTop() )
				positionActor( actor );
		}
	}*/
	// Переместить активных акторов
	for ( unsigned int i = 0; i < activeActors.size(); i++)
	{
		Actor *actor = activeActors[ i ].ptr();
		int oldUTop = actor->uTop();
		if ( !actor->isDeleted() )
		{
			actor->tick();
			if ( oldUTop != actor->uTop() )
				positionActor( actor );
		}
	}
};

void ActorsPool::draw()
{
	for ( auto &it: activeActors )
	{
		if ( !it->isDeleted() )
			it->draw();
	}
};

// *** ActorsRange

Actor *ActorsRange::start()
{
	Actor *actor;
	if ( exclusion )
	{
		actor = actors.prev( exclusion );
		while ( actor && (actor->uTop() + pix2unit( MaxActorWidth ) >= y) )
			actor = actors.prev( actor );
		if ( !actor )
			actor = actors.first();
		for ( ; actor; actor = actors.next( actor ) )
		{
			if ( actorHit( actor ) )
				break;
		};
	}
	else
	{
		for ( actor = actors.first(); actor; actor = actors.next( actor ) )
		{
			if ( actor->uLeft() + pix2unit( MaxActorWidth ) < x )
				continue; // быстрый пропуск
			if ( actorHit( actor ) )
				break;
		};
	}

	return actor;
};

Actor *ActorsRange::next( Actor *actor )
{
	if ( actor )
	{
		while ( ( actor = actors.next( actor ) ) )
		{
			if ( actor->uLeft() > x + w )
			{
				actor = nullptr;
				break;
			};
			if ( actorHit( actor ) )
			{
				break;
			};
		};
	}
	return actor;
};
