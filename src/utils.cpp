#include "utils.h"
#include "string.h"

// **************************
// *       СИСТЕМНЫЕ        *
// **************************

void blt_c( const unsigned char * __restrict src, unsigned char * __restrict dst, size_t cnt )
{
	unsigned char *end = dst + cnt;
	while ( dst < end )
	{
		if ( *src )
			*dst = *src;
		src++;
		dst++;
	};
};

void DMA3Copy16( void *dst, const void *src, int byteCount )
{
	u16 *p_dst = (u16*) dst;
	const u16 *p_src = (const u16*) src;
	int cnt = byteCount /= 2;
	while ( cnt > 0 )
	{
		int part = cnt;
		if ( part > 256 )
			part = 256;
		REG_DMA3DAD = (u32) p_dst;
		REG_DMA3SAD = (u32) p_src;
		REG_DMA3CNT_L = part;
		REG_DMA3CNT_H = DMA_SRC_INCR | DMA_DEST_INCR | DMA_16BIT |
				DMA_START_IMMEDIATE | DMA_ENABLE;

		p_dst += part;
		p_src += part;
		cnt -= part;
	};
};

// **************************
// *        ГЕЙМПАД         *
// **************************

// Для реализации разных реакций на нажатие кнопок (событие)
// и удерживание их (состояние) запомним предыдущее и текущее и состояния
int prevKeys, curKeys = 0;

// **************************
// *         РАЗНОЕ         *
// **************************

char *str2buf( const char *str, char *buf )
{
	while ( ( *buf++ = *str++ ) );
	return --buf;
};

char *int2buf( int num, char *buf )
{
	char tmp[ 16 ];
	bool neg = num < 0;
	int tmpSize = 0;
	do
	{
		int rem = num % 10;
		num = num / 10;
		tmp[ tmpSize++ ] = '0' + (rem >= 0 ? rem : -rem);
	} while ( num );
	if ( neg )
	{
		tmp[ tmpSize++ ] = '-';
	};
	while ( tmpSize > 0 )
	{
		*buf++ = tmp[ --tmpSize ];
	};
	*buf = 0;
	return buf;
};

char *int2hex( unsigned int num, char *buf )
{
	char tmp[ 16 ];
	int tmpSize = 0;
	do
	{
		int rem = num % 16;
		num = num / 16;
		if ( rem > 9 )
			rem += 'A' - 10;
		else
			rem += '0';
		tmp[ tmpSize++ ] = rem;
	} while ( num );
	while ( tmpSize > 0 )
	{
		*buf++ = tmp[ --tmpSize ];
	};
	*buf = 0;
	return buf;
};

int buf2int( const char *buf )
{
	int retVal = 0;
	bool negative = false;
	while ( true )
	{
		char c = *buf++;
		if ( c == '-' )
			negative = !negative;
		else if ( (c < '0') || (c > '9') )
			break;
		else
			retVal = 10 * retVal + (c - '0');
	};
	return negative ? -retVal : retVal;
};

