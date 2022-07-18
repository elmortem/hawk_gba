#ifndef ACTORS_H
#define ACTORS_H

#include "utils.h"
#include <vector>
#include <algorithm>
#include <climits>
#include <string>
#include <map>
#include "video.h"

enum GameGroup
{
	GameGroupNone               = 1 << 0,
	GameGroupPlayer             = 1 << 1,
	GameGroupPlayerBullet       = 1 << 2,
	GameGroupEnemy              = 1 << 3,
	GameGroupEnemyBullet        = 1 << 4,
	GameGroupItems              = 1 << 5,

	GameGroupCollideOfPlayer    = GameGroupEnemy + GameGroupEnemyBullet + GameGroupItems,
	GameGroupCollideOfEnemy     = GameGroupPlayerBullet,

	GameGroupAll =			65535
};

enum Direction
{
	DirLeft = 0,
	DirRight = 1,
	DirUp = 2,
	DirDown = 3
};

class ActorReader
{
private:
	const char * const *data;
	const char *findValue( const std::string &name );

public:
	ActorReader( const char * const *newData ): data( newData ) {};
	~ActorReader() {};

	void setData( const char * const *newData ) { data = newData; };
	const char * const *getData() { return data; };

	bool hasName( const std::string &name )
		{ return findValue( name ) != nullptr; };
	int readInt( const std::string &name, int defVal = 0 );
	std::string readStr( const std::string &name );
};

class Actor
{
protected:
	static int actorsCount;

	int refCount = 0;
	int posX = 0, posY = 0;		// base x,y
	int width = pix2unit( 8 );	// width
	int height = pix2unit( 8 );	// height
	int hitOffset = pix2unit( 2 );
	int timer = 0;
	int group = 0;
	int subtype = 0;
	signed char delCount = 0;
	bool active = false;
	bool global = true;
	int hp = 0;	// hit points
	Ref< Actor > prev, next;
	Ref< Actor > spawner;
	int *spawnCount = nullptr;

public:
	static int getActorsCount() { return actorsCount; };
	int getRefCount() { return refCount; };

	Actor(): refCount( 0 ), delCount( 0 ) { actorsCount++; };
	virtual ~Actor() { releaseSpawner(); actorsCount--; };

	void setPos( int x, int y ) { posX = x; posY = y; };
	void setSizes( int w, int h ) { width = w; height = h; };

	int uLeft() { return ( posX ); };
	int uRight() { return ( posX + width ); };
	int uTop() { return ( posY ); };
	int uBottom() { return ( posY + height ); };
	int uCenterX() { return ( posX + (width >> 1) ); };
	int uCenterY() { return ( posY + (height >> 1) ); };
	int pLeft() { return unit2pix( uLeft() ); };
	int pRight() { return unit2pix( uRight() ); };
	int pTop() { return unit2pix( uTop() ); };
	int pBottom() { return unit2pix( uBottom() ); };
	int pCenterX() { return unit2pix( uCenterX() ); };
	int pCenterY() { return unit2pix( uCenterY() ); };
	virtual int hitLeft() { return uLeft() + hitOffset; };
	virtual int hitRight() { return uRight() - hitOffset; };
	virtual int hitTop() { return uTop() + hitOffset; };
	virtual int hitBottom() { return uBottom() - hitOffset; };
	virtual int hitWidth() { return width - hitOffset * 2; };
	virtual int hitHeight() { return height - hitOffset * 2; };

	virtual void read( ActorReader &reader ) = 0;
	//virtual void pretick() {};
	virtual void tick() {};
	virtual void draw() {};
	virtual void activate() {};
	virtual void deactivate();
	virtual int getDamage() { return 0; }
	virtual void takeDamage( int damage, Actor *who ) {};
	virtual bool needKeepFar() { return false; };

	bool isGlobal() { return global; };
	void setGlobal( bool val ) { global = val; };
	bool isActive() { return active; };
	void setActive( bool val ) { if ( active != val )
		{ active = val; if ( active ) activate(); else deactivate(); }; };
	bool isDeleted() { return delCount != 0; };
	void markDeleted() { delCount = 1; };
	void setSpawner( Actor *pSpawner, int *pSpawnCount );
	void releaseSpawner();

	template< class T > friend class Ref;
	friend class ActorsPool;
	friend class ActorsRange;
	friend class List< Ref< Actor > >;
};

// DECLARE_READABLE
#define DECLARE_READABLE  \
public: \
	static Actor *createInstance(); \
	void read( ActorReader &reader );

// IMPLEMENT_READABLE
#define IMPLEMENT_READABLE( className, name ) \
static EasyInit initOf##className{ [](){ \
	ActorsPool::registerCreator( name, className::createInstance ); } }; \
Actor *className::createInstance() \
{ \
	return new className; \
};

class ActorPlayer;
class ActorsRange;

class ActorsPool
{
public:
	typedef Actor *(*ActorCreator)();

protected:
	typedef std::map< std::string, ActorCreator > ActorsCreators;

	static ActorsCreators *creators;

	typedef std::vector< Ref<Actor> > ActorsVector;
	typedef List< Ref<Actor> > ActorsList;

	ActorsList		actorsList;
	ActorsVector	activeActors;

	int				manDeathSounds = 0;

public:
	Ref< ActorPlayer > player;
	Ref< Actor > baseActor;

	static void registerCreator( const std::string &name,
								 ActorCreator creator );
	static Actor *createActor( const std::string &className );

	void addActor( Actor *actor, Actor *before );
	void positionActor( Actor *actor );
	int actorsCount() { return actorsList.getSize(); };
	int activeActorsCount() { return activeActors.size(); };
	int getSwapCount() { return actorsList.getSwapCount(); };
	void sortActors() { actorsList.bubbleSort(
		[] ( const Ref<Actor> &a, const Ref<Actor> &b ) -> bool
		{ return a->uTop() < b->uTop(); } ); };

	Actor *first() { return actorsList.getFirst().ptr(); };
	Actor *last() { return actorsList.getLast().ptr(); };
	Actor *next( Actor *actor ) { return actor->next.ptr(); };
	Actor *prev( Actor *actor ) { return actor->prev.ptr(); };

	bool actorIsOnLeft( Actor *actor, int border = 0 )
	{
		return actor->pRight() < -border;
	};
	bool actorIsOnRight( Actor *actor, int border = 0 )
	{
		return actor->pLeft() > videoWidth + border;
	};
	bool actorIsOnTop( Actor *actor, int border = 0 )
	{
		return actor->pBottom() < -border;
	};
	bool actorIsOnBottom( Actor *actor, int border = 0 )
	{
		return actor->pTop() > videoHeight + border;
	};
	bool actorIsInCenter( Actor *actor, int hBorder = 0, int vBorder = 0 )
	{
		if ( actorIsOnLeft( actor, hBorder ) ||
            actorIsOnRight( actor, hBorder ) ||
            actorIsOnTop( actor, hBorder ) ||
            actorIsOnBottom( actor, hBorder ) )
            return false;
		return true;
	};

	void clear();
	void loadActors( const char * const *data );

	void tick();
	void draw();

	friend class ActorsRange;
};

extern ActorsPool actors;

class ActorsRange
{
private:
	int x, y, w, h;
	int groupMask;
	Actor *exclusion;

	bool actorHit( Actor *actor ) const
	{
		if ( (actor->isDeleted()) || (actor == exclusion)
			 || (!actor->isActive()) )
			return false;
		if ( groupMask && !(actor->group & groupMask) )
			return false;
		if ( (actor->hitRight() < x) || (actor->hitLeft() > x + w) )
			return false;
		if ( (actor->hitBottom() < y) || (actor->hitTop() > y + h) )
			return false;
		return true;
	};

	Actor *start();
	Actor *next( Actor *actor );

public:
	ActorsRange( int px, int py, int pw, int ph, int pGroupMask,
				 Actor *pExclusion )
	{
		x = px;
		y = py;
		w = pw;
		h = ph;
		groupMask = pGroupMask;
		exclusion = pExclusion;
	};
	ActorsRange( Actor *toucher, int pGroupMask )
	{
		x = toucher->uLeft();
		y = toucher->uBottom();
		w = toucher->width;
        h = toucher->height;
        groupMask = pGroupMask;
        exclusion = toucher;
	};

	friend class ActorsRangeIterator;
};

class ActorsRangeIterator
{
private:
	ActorsRange *range = nullptr;
	Actor *cur = nullptr;
public:
	ActorsRangeIterator() {};
	ActorsRangeIterator(  ActorsRange *pRange ):
		range( pRange ), cur( pRange->start() ) {};
	~ActorsRangeIterator() {};

	Actor *operator*() const { return cur; };
	void operator++() { cur = range->next( cur ); };

	bool operator!=( const ActorsRangeIterator &iter ) const
		{ return cur != iter.cur; };
};

inline ActorsRangeIterator begin( ActorsRange &range )
{
	return ActorsRangeIterator( &range );
}

inline ActorsRangeIterator end( ActorsRange &range )
{
	return ActorsRangeIterator();
}

#endif
