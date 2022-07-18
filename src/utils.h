#ifndef UTILS_H
#define UTILS_H

#include "gba_defs.h"
#include <algorithm>

// **************************
// *       СИСТЕМНЫЕ        *
// **************************

void DMA3Copy16( void *dst, const void *src, int byteCount );

// **************************
// *        ГЕЙМПАД         *
// **************************

// Для реализации разных реакций на нажатие кнопок (событие)
// и удерживание их (состояние) запомним предыдущее и текущее и состояния
extern int prevKeys, curKeys;

// Обновление предыдущего и текущего состояния кнопок.
// Эту функцию следует вызывать 1 раз за VSync или наподобие того.
inline void readKeys()
{
	prevKeys = curKeys;
	curKeys = ~REG_KEYS;
};

// Зажата ли (состояние) кнопка
inline int keyIsDown( int key )
{
	return curKeys & key;
};

// Была ли кнопка нажата с момента последнего вызова read_keys.
// То есть для активации каждого события кнопку надо зажимать и отжимать.
inline int keyIsPressed( int key )
{
	return (curKeys & key) && !(prevKeys & key);
};

// Была ли кнопка отжата с момента последнего вызова read_keys.
// То есть для активации каждого события кнопку надо зажимать и отжимать.
inline int keyIsReleased( int key )
{
	return !(curKeys & key) && (prevKeys & key);
};


// **************************
// *         РАЗНОЕ         *
// **************************

inline int unit2pix( int x ) { return x >> 8; };
inline int pix2unit( int x ) { return x << 8; };
inline int unit2cell( int x ) { return x >> 11; };
inline int cell2unit( int x ) { return x << 11; };
inline int pix2cell( int x ) { return x >> 3; };
inline int cell2pix( int x ) { return x << 3; };

template< class T >
inline volatile T &vlt( T &val )
{
	return val;
};

extern char *str2buf( const char *str, char *buf );
extern char *int2buf( int num, char *buf );
extern char *int2hex( unsigned int num, char *buf );
extern int buf2int( const char *buf );

class write2buf
{
private:
	char *buf;
public:
	write2buf( char *sbuf ): buf( sbuf ) {};
	write2buf &operator<<( const char *s ) { buf = str2buf( s, buf ); return *this; };
	//write2buf &operator<<( std::string s ) { buf = str2buf( s.c_str(), buf ); return *this; };
	write2buf &operator<<( int i ) { buf = int2buf( i, buf ); return *this; };
	char *get() const { return buf; };
	void set( char *s ) { buf = s; };
};

// ******************
// *    EasyInit    *
// ******************

class EasyInit
{
public:
	template< class T >
	EasyInit( const T &functor ) { functor(); };
};

// ******************
// *       Ref      *
// ******************

template< class T >
class Ref
{
protected:
	T *ref = nullptr;

	void grab() { if ( ref ) ref->refCount++; };
	void release() { if ( ref ) { ref->refCount--; if ( ref->refCount == 0 ) delete ref; }; };

public:

	Ref(): ref( nullptr ) {};
	Ref( T *ptr ): ref( ptr ) { grab(); };
	Ref( const Ref &ar ): ref( ar.ref ) { grab(); };
	Ref( Ref &&ar ): ref( ar.ref ) { ar.ref = nullptr; };
	~Ref() { release(); };
	Ref &operator=( const Ref &ar ) { if ( &ar != this ) { release(); ref = ar.ref; grab(); }; return *this; };
	Ref &operator=( T *ptr ) { if ( ptr != ref ) { release(); ref = ptr; grab(); }; return *this; };

	T *operator->() const { return ref; };
	T *operator*() const { return ref; };
	T *ptr() const { return ref; };
	bool operator!() const { return ref == nullptr; };
	explicit operator bool() const { return ref != nullptr; };
	bool operator==( const Ref &ar ) { return ref == ar.ref; };
	bool operator!=( const Ref &ar ) { return ref != ar.ref; };

	friend void swap( Ref &a, Ref &b )
	{
		using std::swap;
		std::swap( a.ref, b.ref );
	};
};

// ******************
// *      List      *
// ******************

template < class T >
class List
{
protected:
	int size = 0;
	int swapCount = 0;
	T first = nullptr, last = nullptr;

public:

	List() {};
	virtual ~List() { clear(); };

	int getSize() { return size; };
	T &getFirst() { return first; };
	T &getLast() { return last; };
	int getSwapCount() { return swapCount; };

	void add( T item )
	{
		if ( last )
			last->next = item;
		item->prev = last;
		item->next = nullptr;
		last = item;
		if ( !first )
			first = item;
		size++;
    };

	void insert( T item, T before )
	{
		if ( !before )
			add( item );
		else
		{
			if ( before->prev )
			{
				item->prev = before->prev;
				item->prev->next = item;
			}
			else
			{
				first = item;
				item->prev = nullptr;
			};
			before->prev = item;
			item->next = before;
			size++;
		};
	};

	T remove( T item )
	{
		T nxt = item->next;
		T prv = item->prev;
		item->prev = nullptr;
		item->next = nullptr;
		if ( first == item )
		{
			first = nxt;
		};
		if ( last == item )
		{
			last = prv;
		}
		if ( prv )
		{
			prv->next = nxt;
		}
		if ( nxt )
		{
			nxt->prev = prv;
		}
		size--;
		return nxt;
	};

	void clear()
	{
		T cur = first;
		while ( cur )
		{
			T next = cur->next;
			cur->prev = nullptr;
			cur->next = nullptr;
			cur = next;
		};
		first = nullptr;
		last = nullptr;
		size = 0;
		swapCount = 0;
	};

	void swapSiblings( T a, T b )
	{
		swapCount++;

		T x = a->prev;
		T y = b->next;
		b->next = a;
		a->prev = b;
		b->prev = x;
		if ( !x )
			first = b;
		else
			x->next = b;
		a->next = y;
		if ( !y )
			last = a;
		else
			y->prev = a;
	};

	template < class U >
	void bubbleSort( U funcLess )
	{
		if ( first )
		{
			T cur = first;
			T next = cur->next;
			while ( next )
			{
				if ( funcLess( next, cur ) )
				{
					do
					{
						swapSiblings( next->prev, next );
					}
					while ( next->prev && funcLess( next, next->prev ) );
				}
				else
				{
					cur = cur->next;
				};
				next = cur->next;
			};
		};
	}
};

#endif
