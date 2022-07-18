#include <string.h>
#include <algorithm>
//#include <time.h>
#include <iomanip>
#include <iostream>
#include "video.h"
#include "audio.h"
#include "state_mainmenu.h"
#include "actor.h"
#include "gamedata.h"
#include "res_graphics.h"

static const char sram_label[] = { 'S', 'R', 'A', 'M', '_', 'V', 'n', 'n', 'n', 0, 0, 0 };

// Обработчик прерываний.
// Атрибут GCC __target__ ("arm") переключает компилятор в
// создание кода функции в режиме Arm даже внутри модуля
// в котором режим компиляции по умолчанию - Thumb.
void __attribute__ ((__target__ ("arm"))) interruptHandler(void)
{
	// Выключим прерывания
	REG_IME = 0;
	// Запомним входной флаг
	u16 enter_IF = REG_IF;

	// Проверяем, что прерывание равно INT_TIMER1
	if ( enter_IF & INT_TIMER1 )
	{
		// Вызываем процедуру обработки драйвера звука
		// (заодно полезно этим переключиться в режим Thumb).
		soundDriverTimer();
	};
	// Сбросим входной флаг (записью 1!)
	REG_IF = enter_IF;
	// Обновим флаг ожидания в BIOS для работы VBlankIntrWait()
	BIOS_WAIT_FLAG |= enter_IF;
	// Включим прерывания
	REG_IME = 1;
};

int main(void)
{
    memcpy( (void*) ROM_START, sram_label, 12 );

	// Инициализируем звук
	soundDriverInit();

	REG_INTERRUPT = &interruptHandler;	// Установим обработчик

	// Включим генерацию прерываний по VBlank для работы VBlankIntrWait()
	REG_DISPSTAT |= VBLANK_IRQ_ENABLE;
	// Включим реакцию на прерывания INT_VBLANK для работы VBlankIntrWait()
	REG_IE |= INT_VBLANK;

	setDefaultVideoMode();
    applyTileSet( &globalTileSet );

	GameState::setNext( &gameStateMainmenu );

	// Бесконечный цикл
	while ( true )
	{
		// Дождёмся выхода в VBLANK новым, энергосберегающим способом
		VBlankIntrWait();

		readKeys();	// Обновляем состояние кнопок

		GameState::doTick();

		//char buf[ 64 ];
		//write2buf w2b( buf );
		//w2b << "T=" << Actor::getActorsCount() << " L=" << actors.actorsCount() <<
		//	" A=" << actors.activeActorsCount() << " W=" << actors.getSwapCount() << "     ";
		//printString( 0, 0, buf, 30 );
	};

	return 0;
}
