#include "video.h"
#include <algorithm>
#include <string.h>

// **************************
// *    ОТОБРАЖЕНИЕ ПОЛЯ    *
// **************************

TileSet* currentTileSet;

void setDefaultVideoMode()
{
	// Остановим обновление дисплея
	REG_DISPCNT |= MODE_DISPLAY_OFF;

	// Очистим видеопамять
	for ( int i = 0; i < VID_RAM_SIZE / 2; i++ )
		TILE_DATA[ i ] = 0;

	// Включаем MODE_0, 2 задних фона и спрайты
	REG_DISPCNT = MODE_0 | BG0_ENABLE | BG1_ENABLE | BG2_ENABLE | SPR_ENABLE;

	// Включаем задние фоны:
	REG_BG0CNT =	BG_PRIORITY( 0 ) | BG_TILE_DATA_AT_0000 | BG_COLOR_256 |
			BG_TILE_MAP_AT( 28 ) | BG_SIZE_256x256;

	REG_BG1CNT =	BG_PRIORITY( 2 ) | BG_TILE_DATA_AT_0000 | BG_COLOR_256 |
			BG_TILE_MAP_AT( 29 ) | BG_SIZE_256x256;

	REG_BG2CNT =	BG_PRIORITY( 3 ) | BG_TILE_DATA_AT_0000 | BG_COLOR_256 |
			BG_TILE_MAP_AT( 30 ) | BG_SIZE_256x256;

	// Обнулим прокрутки фонов
	REG_BG0HOFS = 0;
	REG_BG0VOFS = 0;

	REG_BG1HOFS = 0;
	REG_BG1VOFS = 0;

	REG_BG2HOFS = 0;
	REG_BG2VOFS = 0;

	// Отключим спрайты
	resetSprites( 0, 128 );

	// Восстановим обновление дисплея
	REG_DISPCNT &= ~MODE_DISPLAY_OFF;
};

void applyTileSet( TileSet *tileSet )
{
    currentTileSet = tileSet;

	//REG_DISPCNT |= MODE_DISPLAY_OFF;

	// Инициализируем палитру фонов
	memcpy( BGR_PALETTE, tileSet->mapPal,
		sizeof( tileSet->mapPal[ 0 ] ) * tileSet->mapPalSize );
	// Инициализируем палитру спрайтов
	memcpy( SPR_PALETTE, tileSet->sprPal,
		sizeof( tileSet->sprPal[ 0 ] ) * tileSet->sprPalSize );
	// Скопируем тайловые данные
	memcpy( TILE_DATA, tileSet->mapTileData,
		64 * tileSet->mapTileDataSize );
	// Скопируем тайлы спрайтов
	memcpy( SPR_TILES, tileSet->sprTileData,
		64 * tileSet->sprTileDataSize );

	// Инициализируем палитру фонов
	DMA3Copy16( BGR_PALETTE, tileSet->mapPal,
		sizeof( tileSet->mapPal[ 0 ] ) * tileSet->mapPalSize );
	// Инициализируем палитру спрайтов
	DMA3Copy16( SPR_PALETTE, tileSet->sprPal,
		sizeof( tileSet->sprPal[ 0 ] ) * tileSet->sprPalSize );
	// Скопируем тайловые данные
	DMA3Copy16( TILE_DATA, tileSet->mapTileData,
		64 * tileSet->mapTileDataSize );
	// Скопируем тайлы спрайтов
	DMA3Copy16( SPR_TILES, tileSet->sprTileData,
		64 * tileSet->sprTileDataSize );
}

void resetGameMapLayer( GameMapLayerDesc *layer )
{
    layer->backgroundX = 0;
    layer->backgroundY = 0;
    layer->tileX = 0;
    layer->tileY = 0;
    layer->lastTileX = -1;
    layer->lastTileY = -1;
}

// "Загнать" координаты viewport в такие рамки, чтобы не выйти за границы
// поля и вычислить curTx/Ty.
void refreshGameMapLayer( GameMapLayerDesc* layer, bool forceRefill )
{
    if ( !layer->loop )
    {
        layer->backgroundX = std::max( 0, layer->backgroundX );
        layer->backgroundY = std::max( 0, layer->backgroundY );
        layer->backgroundX = std::min( layer->width * 8 - videoWidth, layer->backgroundX );
        layer->backgroundY = std::min( layer->height * 8 - videoHeight, layer->backgroundY );
    }

    layer->tileX = layer->backgroundX >> 3;
    layer->tileY = layer->backgroundY >> 3;

    refreshTileMap( layer->tileMap, layer->tiles, layer->tileX, layer->tileY, layer->lastTileX, layer->lastTileY, layer->width, layer->height, forceRefill );

    // Обновим прокрутки фонов...
    if(layer->tileMap == TILE_MAP2)
    {
        REG_BG2HOFS = layer->backgroundX;
        REG_BG2VOFS = layer->backgroundY;
    }
    else if(layer->tileMap == TILE_MAP1)
    {
        REG_BG1HOFS = layer->backgroundX;
        REG_BG1VOFS = layer->backgroundY;
    }

	// Обновляем предыдущие координаты левой-верхней отображаемой клетки
	layer->lastTileX = layer->tileX;
	layer->lastTileY = layer->tileY;
};

void clearTileMap( u16 *tileMap )
{
	for ( int y = 0; y < 32; y++ )
	{
		for ( int x = 0; x < 32; x++ )
		{
			tile32w( tileMap, x, y ) = 0;
		};
	};
}

void fillTileMap( u16 *tileMap, int tile )
{
	// Заполним TILE_MAP2...
	u16 realTile = currentTileSet->mapTilesOpt[ tile ];
	for ( int y = 0; y < 32; y++ )
	{
		for ( int x = 0; x < 32; x++ )
		{
			tile32w( tileMap, x, y ) = realTile;
		};
	};
};

void fillTileMapTile( u16 *tileMap, int tile, int x, int y )
{
    tile32w( tileMap, x & 31, y & 31 ) = currentTileSet->mapTilesOpt[ tile ];
}

// Обновить содержимое тайловой карты актуальными данными из карты поля.
// Задаются координаты клеток прямоугольной области (tx0,ty0,tx1,ty1) поля,
// которая должна быть скопирована в соответствующие ей клетки тайловой карты.
// Работа с координатами тайловой карты ведется по модулю 32 (x & 31) чтобы
// замкнуть отображение в её ограниченный размер 32x32 и обновлять только
// необходимый минимум клеток при участии механизма прокрутки фона (см. ниже).
void fillTileMapArea( u16 *tileMap, const u16 *tiles, int tx0, int ty0, int tx1, int ty1, int width, int height )
{
	// Total refill screen rectangle by data
	for ( int y = ty0; y <= ty1; y++ )
	{
		for ( int x = tx0; x <= tx1; x++ )
		{
		    int tmx = x & 31;
		    if(tmx < 0)
                tmx = 32 + tmx;
            int tmy = y & 31;
		    if(tmy < 0)
                tmy = 32 + tmy;

            int tx = x % width;
		    if(tx < 0)
                tx = width + tx;
            int ty = y % height;
		    if(ty < 0)
                ty = height + ty;

			tile32w( tileMap, tmx, tmy ) = mapTile( tiles, tx, ty, width );
		};
	};
};

// Обновить с предыдущего кадра состояние экрана (viewport).
// Должны быть правильно рассчитаны curTx, curTy и vpx, xy не должны
// выходить за границы поля - т.е. предварительно надо вызвать validateViewport().
// Функция обновляет содержимое регистров прокрутки и, если в тайловой карте
// стали видны устаревшие полоски тайлов - обновляет их содержимое из карты поля.
// Важнейшую роль играет механизм отображения тайловых карт видеокартой GBA и то
// как происходит зацикливание при их прокрутке - работая с координатами клеток в
// тайловой карте "по модулю 32" мы ликвидируем необходимость обновлять всю
// площадь тайловой карты при плавной прокрутке.
// Тем не менее параметром forceRefill можно принудительно выполнить полное обновление.
void refreshTileMap( u16 *tileMap, const u16 *tiles, int tx, int ty, int otx, int oty, int width, int height, bool forceRefill )
{
	// Если координаты клетки, отображаемой в левом-верхнем углу экрана
	// "уплыли" со своего предыдущего значения - перерисовываем нужные
	// полоски клеток в тайловой карте.
	if ( (otx != tx) || (oty != ty) || forceRefill )
	{
		// Вычисляем разницу координат между предыдущим и текущим положением.
		int dtx = tx - otx;
		int dty = ty - oty;
		int dtxa = std::abs( dtx );
		int dtya = std::abs( dty );
		// Если "уплыли" по координатам слишком далеко или перерисовка полосками
		// будет нерациональна ввиду большой площади пересечения - включим
		// режим полной перерисовки.
		if ( (dtxa > 30) || (dtya > 20) || (dtxa * dtya > 30 * 20 / 4) )
		{
			forceRefill = true;
		};
		if ( forceRefill )
		{
			// Полная перерисовка
			fillTileMapArea( tileMap, tiles, tx, ty, tx + 30, ty + 20, width, height );
		}
		else
		{
			if ( dtx > 0 )
			{
				// Перекрашиваем полоску справа
				fillTileMapArea( tileMap, tiles, tx + 30 - dtx + 1, ty, tx + 30, ty + 20, width, height );
			}
			else if ( dtx < 0 )
			{
				// Перекрашиваем полоску слева
				fillTileMapArea( tileMap, tiles, tx, ty, tx - dtx - 1, ty + 20, width, height );
			};
			if ( dty > 0 )
			{
				// Перекрашиваем полоску снизу
				fillTileMapArea( tileMap, tiles, tx, ty + 20 - dty + 1, tx + 30, ty + 20, width, height );
			}
			else if ( dty < 0 )
			{
				// Перекрашиваем полоску сверху
				fillTileMapArea( tileMap, tiles, tx, ty, tx + 30, ty - dty - 1, width, height );
			};
		};

	};
};

// Вывод текста

void printString( int x, int y, const char *str, int minlen )
{
	char c;
	int startX = x;
	while ( (c = *str++) )
	{
		if ( (c >= 'a') && (c <= 'z') )
		{
			c -= ('a' - 'A');
		};
		if ( c != '\n' )
			tile32w( TILE_MAP0, x, y ) = c ? currentTileSet->mapTilesOpt[ currentTileSet->fontStartTile - ' ' + c ] : 0;
		if ( (++x >= 30) || (c == '\n') )
		{
			x = startX;
			y++;
		};
		minlen--;
	};
	while ( minlen-- > 0 )
	{
		tile32w( TILE_MAP0, x, y ) = 0;
		if ( ++x >= 30 )
		{
			x = startX;
			y++;
		};
	};
};

void printTile( int x, int y, int tile )
{
    tile32w( TILE_MAP0, x, y ) = currentTileSet->mapTilesOpt[ tile ];
}

void fatalError( std::string reason )
{
	while ( true )
	{
		VBlankIntrWait();

		printString( 0, 0, reason.c_str() );
	};
};

// **************************
// *        СПРАЙТЫ         *
// **************************

const int spriteSizeAttr0[ SpriteSizeCount ] =
{
	ATTR0_SHAPE_SQ,
	ATTR0_SHAPE_V,
	ATTR0_SHAPE_H,
	ATTR0_SHAPE_SQ,
	ATTR0_SHAPE_SQ
};
const int spriteSizeAttr1[ SpriteSizeCount ] =
{
	ATTR1_SQ_8x8,
	ATTR1_V_8x16,
	ATTR1_H_16x8,
	ATTR1_SQ_16x16,
	ATTR1_SQ_32x32
};
const int spriteMaxSize[ SpriteSizeCount ] = { 8, 16, 16, 16, 32 };
const int spriteSizeWidths[ SpriteSizeCount ] = { 8, 8, 16, 16, 32 };
const int spriteSizeHeights[ SpriteSizeCount ] = { 8, 16, 8, 16, 32 };

static int spriteCount = 0;
static int lastSpriteCount = 0;

void resetSprites( int base, int count )
{
	for ( int i = base; i < base + count; i++ )
	{
		SPR_BUFFER[ i ].attr0 = ATTR0_Y( -16 ) | ATTR0_DISABLED | ATTR0_SHAPE_SQ;
		SPR_BUFFER[ i ].attr1 = ATTR1_X( -16 ) | ATTR1_SQ_8x8;
		SPR_BUFFER[ i ].attr2 = ATTR2_PRIORITY( 3 );
	};
};

void drawSprite( int x, int y, int tile, SpriteSize size, int priority )
{
    if ( spriteCount >= 128 )
    {
        return;
    };

	int maxSize = spriteMaxSize[ size ];
	if ( (x + maxSize < 0) || (y + maxSize < 0) ||
		 (x > videoWidth - 1) || (y > videoHeight - 1) )
	{
		return;
	};
	int flip = 0;
	if ( tile & (1 << 10) )
	{
		flip |= ATTR1_H_FLIP;
	};
	if ( tile & (1 << 11) )
	{
		flip |= ATTR1_V_FLIP;
	};
	SPR_BUFFER[ spriteCount ].attr0 = ATTR0_Y( y ) | ATTR0_COLOR_256 | spriteSizeAttr0[ size ];
	SPR_BUFFER[ spriteCount ].attr1 = ATTR1_X( x ) | flip | spriteSizeAttr1[ size ];
	SPR_BUFFER[ spriteCount ].attr2 = ATTR2_TILE( 2 * (tile & 1023) ) | ATTR2_PRIORITY( priority );
	spriteCount++;
};

void beginSpriteDraw()
{
    lastSpriteCount = spriteCount;
	spriteCount = 0;
};

void endSpriteDraw()
{
	if ( spriteCount < lastSpriteCount )
		resetSprites( spriteCount, lastSpriteCount - spriteCount );
};

