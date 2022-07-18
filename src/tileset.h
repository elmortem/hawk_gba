#ifndef TILESET_H
#define TILESET_H

#include "utils.h"

struct TileSet
{
	// map tiles
	const u8 *mapTileData;
	int mapTileDataSize;
	const u16 *mapTilesOpt;
	int mapTilesOptSize;
	const u16 *mapPal;
	int mapPalSize;

	// sprite tiles
	const u8 *sprTileData;
	int sprTileDataSize;
	const u16 *sprPal;
	int sprPalSize;

	// font
	int fontStartTile;
};

#endif // TILESET_H
