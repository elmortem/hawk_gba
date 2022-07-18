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

// ���������� ����������.
// ������� GCC __target__ ("arm") ����������� ���������� �
// �������� ���� ������� � ������ Arm ���� ������ ������
// � ������� ����� ���������� �� ��������� - Thumb.
void __attribute__ ((__target__ ("arm"))) interruptHandler(void)
{
	// �������� ����������
	REG_IME = 0;
	// �������� ������� ����
	u16 enter_IF = REG_IF;

	// ���������, ��� ���������� ����� INT_TIMER1
	if ( enter_IF & INT_TIMER1 )
	{
		// �������� ��������� ��������� �������� �����
		// (������ ������� ���� ������������� � ����� Thumb).
		soundDriverTimer();
	};
	// ������� ������� ���� (������� 1!)
	REG_IF = enter_IF;
	// ������� ���� �������� � BIOS ��� ������ VBlankIntrWait()
	BIOS_WAIT_FLAG |= enter_IF;
	// ������� ����������
	REG_IME = 1;
};

int main(void)
{
    memcpy( (void*) ROM_START, sram_label, 12 );

	// �������������� ����
	soundDriverInit();

	REG_INTERRUPT = &interruptHandler;	// ��������� ����������

	// ������� ��������� ���������� �� VBlank ��� ������ VBlankIntrWait()
	REG_DISPSTAT |= VBLANK_IRQ_ENABLE;
	// ������� ������� �� ���������� INT_VBLANK ��� ������ VBlankIntrWait()
	REG_IE |= INT_VBLANK;

	setDefaultVideoMode();
    applyTileSet( &globalTileSet );

	GameState::setNext( &gameStateMainmenu );

	// ����������� ����
	while ( true )
	{
		// ������� ������ � VBLANK �����, ����������������� ��������
		VBlankIntrWait();

		readKeys();	// ��������� ��������� ������

		GameState::doTick();

		//char buf[ 64 ];
		//write2buf w2b( buf );
		//w2b << "T=" << Actor::getActorsCount() << " L=" << actors.actorsCount() <<
		//	" A=" << actors.activeActorsCount() << " W=" << actors.getSwapCount() << "     ";
		//printString( 0, 0, buf, 30 );
	};

	return 0;
}
