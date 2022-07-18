#ifndef VIDEO_H
#define VIDEO_H

#include "utils.h"
#include <string>
#include "tileset.h"

struct GameMapLayerDesc
{
    u16 *tileMap;
    int width, height;
	const u16 *tiles;
	bool loop;

    int backgroundX, backgroundY;
	int tileX, tileY;
	int lastTileX, lastTileY;
};

// **************************
// *    “¿…ÀŒ¬€≈  ¿–“€      *
// **************************

#define TILE_DATA   ((u16*) (VID_RAM_START + 0x0000) )
#define TILE_MAP0   ((u16*) BG_TILE_MAP_ADDR( 28 ) )
#define TILE_MAP1   ((u16*) BG_TILE_MAP_ADDR( 29 ) )
#define TILE_MAP2   ((u16*) BG_TILE_MAP_ADDR( 30 ) )

// ƒÎˇ Ú‡ÈÎÓ‚˚ı Í‡Ú 64x32 Ë 64x64
inline u16 &tile64w( u16 *base, int x, int y )
{
	if ( x > 31 )
	{
		x -= 32;
		base += 32 * 32;
	};
	if ( y > 31 )
	{
		y -= 32;
		base += 2 * 32 * 32;
	};
	return base[ x + y * 32 ];
};

// ƒÎˇ Ú‡ÈÎÓ‚˚ı Í‡Ú 32x32 Ë 32x64
inline u16 &tile32w( u16 *base, int x, int y )
{
	return base[ x + y * 32 ];
};

// **************************
// *    Œ“Œ¡–¿∆≈Õ»≈ œŒÀﬂ    *
// **************************

const int videoWidth = 240;
const int videoHeight = 160;
extern TileSet* currentTileSet;

void setDefaultVideoMode();
void applyTileSet( TileSet *tileSet );
void resetGameMapLayer( GameMapLayerDesc *layer );

void refreshGameMapLayer( GameMapLayerDesc* layer, bool forceRefill );

void clearTileMap( u16 *tileMap );
void fillTileMap( u16 *tileMap, int tile );
void fillTileMapTile( u16 *tileMap, int tile, int x, int y );
void fillTileMapArea( u16 *tileMap, const u16 *tiles, int tx0, int ty0, int tx1, int ty1, int width, int height );
void refreshTileMap( u16 *tileMap, const u16 *tiles, int tx, int ty, int otx, int oty, int width, int height, bool forceRefill = false );

inline u16 mapTile( const u16* tiles, int x, int y, int width )
{
    return currentTileSet->mapTilesOpt[ tiles[ x + y * width ] ];
}

// ¬˚‚Ó‰ ÚÂÍÒÚ‡

void printString( int x, int y, const char *str, int minlen = 0 );
void printTile( int x, int y, int tile );
void fatalError( std::string reason );

// **************************
// *        —œ–¿…“€         *
// **************************

enum SpriteSize
{
	SpriteSize8x8,
	SpriteSize8x16,
	SpriteSize16x8,
	SpriteSize16x16,
	SpriteSize32x32,
	SpriteSizeCount
};

extern const int spriteSizeAttr0[];
extern const int spriteSizeAttr1[];

struct SpriteChunk
{
	int sprite;
	int px, py;
	u8 spriteSize;
	u8 color;
};

struct SpriteDesc
{
	int spriteCount;
	int pWidth, pHeight;
	int px, py;
	const SpriteChunk *sprites;
};

extern void resetSprites( int base, int count );
extern void drawSprite( int x, int y, int tile, SpriteSize size, int priority );
extern void drawPlayerJumpSprite( int x, int y, int body, int legs, int phase,
									int priority );
extern void drawSprite24x16( int x, int y, int tile16, int tile8,
								bool flipHoriz, int priority );
extern void drawSpriteDesc( int x, int y, bool flipHoriz, bool flipVert,
							int color1, int color2, const SpriteDesc *desc,
							int priority );
extern void beginSpriteDraw();
extern void endSpriteDraw();
inline int tileFlipH( int tile ) { return tile ^ (1 << 10); };
inline int tileFlipV( int tile ) { return tile ^ (1 << 11); };


#endif
