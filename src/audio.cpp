#include "audio.h"
#include "string.h"

// **************************
// *    �������� �������    *
// **************************

const int sndSourceCount = 8;
SoundSource sndSources[ sndSourceCount ];
// ������� ��������������� ��������� �������� � ������
const int sndFreqHz = 16384;
// ������� � ������� ����� ��������� ���������� ��� �����.
// ������ ������ ������ SndFreqHz � ���������� �� ��������� 100
const int sndIntrHz = 64;
// ����������� ������ �������� ������
const int sndBankSize = sndFreqHz / sndIntrHz;

// ��� �������� �����, � ��� �� 16-������ ���� �������
static i8 sndBank0[ sndBankSize ];
static i8 sndBank1[ sndBankSize ];
static i16 sndMixBank[ sndBankSize ];

// ��������� �� ������ �������� ����� (������ ����������������� ������)
SoundSource *volatile sndFirstSource = nullptr;
// ����� �������� ������������������ �����
volatile u8 sndCurBank = 0;
// �������� ��������� ������ (master volume), fixed(8:8), 0x0100 ��� 1.0f
volatile i16 sndVolume = 0x0080;

// ��������� ������� ��� �������� �����.
// ������ ���������� �� �������, ����� DMA ��� ������ ������� ����� ������,
// ����� ����������� ��� �� ������ � ��������� ������� ������� �������.
void soundDriverTimer()
{
	// ������ ������� ���� (����� ��������� ����������)
	u8 curBank = 1 - sndCurBank;
	sndCurBank = curBank;
	// ����������� DMA-����� �� ������ �������� ����
	REG_DMA1CNT_H = 0; // �������� �����, ����� �������� ��� ���������
	// ���������� �������� ������
	REG_DMA1SAD = curBank ? (u32) sndBank1 : (u32) sndBank0;
	// ������� DMA-����� � ��� ����� �������� ����� ���������
	REG_DMA1CNT_H = DMA_SRC_INCR | DMA_REPEAT | DMA_START_SPECIAL | DMA_ENABLE;
	// ����� �������� � ������, ������� ����� ��������������� �� ���������� ����������
	i8 *workBank = curBank ? sndBank0 : sndBank1;
	// ��������� �������� ���� �������...
	memset( (void *) sndMixBank, 0, sizeof( sndMixBank ) );

	// ��������� ��������� ����� � 16-������ ���� �������
	i16 *mixPtr;	// ��������� � ����� �������
	i8 *chPtr;	// ��������� �� 8-������ ������
	// ������� ������ ���������� �����
	SoundSource *workSource = sndFirstSource;
	while ( workSource )
	{
		if ( workSource->mode != SoundModeOff )
		{
			mixPtr = sndMixBank;
			chPtr = (signed char *) workSource->cur;
			for ( int i = 0; i < sndBankSize / 4; i++ )
			{
				if ( workSource->cur >= workSource->end )
			 	{
			 		// ��� ���������� ����� ������ ���������...
			 		if ( workSource->mode == SoundModeOnce )
			 		{
			 			// ...� ����������� ����������� ���������������
						workSource->mode = SoundModeOff;
						workSource->cur = workSource->start;
						break;
			 		}
			 		else
			 		{
			 			// ...� ����������� ������������ � ������
			 			workSource->cur = workSource->start;
			 			chPtr = (signed char *) workSource->cur;
			 		};
				};
				// ��� ��� volume ��� fixed(:8), �� ��������� ��������� ����
				// �������� ������ �� 8 ���, ����� �� ������� ����� ������
				*mixPtr++ += ((*chPtr++ * workSource->volume) >> 8);
				*mixPtr++ += ((*chPtr++ * workSource->volume) >> 8);
				*mixPtr++ += ((*chPtr++ * workSource->volume) >> 8);
				*mixPtr++ += ((*chPtr++ * workSource->volume) >> 8);

				workSource->cur++;
			};
		};
		workSource = workSource->next;	// ��������� � ���������� ���������
	};

	// ������� ��������������� � 16-������ ����� � ���� ������
	i16 vol = sndVolume; // ���������� volatile ����������
	mixPtr = sndMixBank;
	chPtr = workBank;
	for ( int i = 0; i < sndBankSize; i++ )
	{
		*chPtr++ = ((*mixPtr++ * vol) >> 8);
	};
};

// ������������� ��������� ��������
void soundDriverInit()
{
	// �������������� �������� �� ���������� �����
	// ��������� ������� �� ���������� ������� 1
	REG_IE &= ~INT_TIMER1;
	// ��������� �������� ���
	REG_SOUNDCNT_X = 0;
	// ��������� DMA1
	REG_DMA1CNT_H = 0;
	// ��������� �������
	REG_TM0CNT = 0;
	REG_TM1CNT = 0;

	// ������� �������� ������
	memset( (void*) sndBank0, 0, sndBankSize );
	memset( (void*) sndBank1, 0, sndBankSize );

	// ���������� ������ �������� ����� � ������� ����
	sndFirstSource = nullptr;
	sndCurBank = 0;

	// ���������� ����� ��������������� ������
	for ( int i = 0; i < sndSourceCount; i++ )
	{
		sndSources[ i ].mode = SoundModeOff;
		sndSources[ i ].reserved = false;
		sndSources[ i ].next = &sndSources[ i + 1 ];
	};
	sndSources[ sndSourceCount - 1 ].next = nullptr;
	sndFirstSource = sndSources;

	// ����� A �� 100% ��������� � � ��� ������������, ����������
	// ��� ���������� ����� � ����������� ��������������� � ������� �������
	REG_SOUNDCNT_H =	SOUND_A_VOL_100 |
				SOUND_A_LEFT | SOUND_A_RIGHT |
				SOUND_A_TIMER0 | SOUND_A_RESET;
	// �������� �������� ���
	REG_SOUNDCNT_X =	SOUND_ENABLE;

    	// ������������ � ��������� ���������� �� ����������� �������
	REG_IE |= INT_TIMER1;			// ������� ������� �� INT_TIMER1

	// ����������� ����� DMA1 �� ������������ �������� ������ � FIFO_A
	REG_DMA1DAD = (u32) &REG_FIFO_A;
	REG_DMA1SAD = (u32) sndBank0;
	REG_DMA1CNT_H = DMA_SRC_INCR | DMA_REPEAT |
			DMA_START_SPECIAL | DMA_ENABLE;

	// ����������� ��������� ������ �� ���������� ���
	// ������������ �������� �������
	REG_TM1D	= 65536 - sndBankSize;
	REG_TM1CNT	= TIMER_ENABLE | TIMER_OVERFLOW | TIMER_IRQ_ENABLE;

	// ��������� ������-������� ��������� ������ FIFO_A
	REG_TM0D	= 65536 - ((1 << 24) / sndFreqHz);
	REG_TM0CNT	= TIMER_ENABLE;

	// ������ "�����������" ������� ��� �������������
	// ���� �������� (��� ��������)
	REG_FIFO_A = 0;
	REG_FIFO_A = 0;
};

static int soundFindFreeSlot()
{
	for ( int i = 0; i < sndSourceCount; i++ )
	{
		if ( !sndSources[ i ].reserved && (sndSources[ i ].mode == SoundModeOff) )
		{
			return i;
		};
	};
	return -1;
};

void soundPlayOnce( int clip, int volume )
{
	REG_IME = 0;
	MEMORY_BARRIER;
	int slot = soundFindFreeSlot();
	if ( slot >= 0 )
	{
		sndSources[ slot ].mode = SoundModeOnce;
		sndSources[ slot ].clip = clip;
		sndSources[ slot ].reserved = false;
		sndSources[ slot ].volume = volume;
		sndSources[ slot ].start = soundLib[ clip ].start;
		sndSources[ slot ].end = soundLib[ clip ].start + soundLib[ clip ].len / 4;
		sndSources[ slot ].cur = sndSources[ slot ].start;
	};
	REG_IME = 1;
	MEMORY_BARRIER;
};

int soundPlay( int clip, SoundMode mode, int volume )
{
	REG_IME = 0;
	MEMORY_BARRIER;
	int slot = soundFindFreeSlot();
	if ( slot >= 0 )
	{
		sndSources[ slot ].mode = mode;
		sndSources[ slot ].clip = clip;
		sndSources[ slot ].reserved = true;
		sndSources[ slot ].volume = volume;
		sndSources[ slot ].start = soundLib[ clip ].start;
		sndSources[ slot ].end = soundLib[ clip ].start + soundLib[ clip ].len / 4;
		sndSources[ slot ].cur = sndSources[ slot ].start;
	};
	REG_IME = 1;
	MEMORY_BARRIER;
	return slot;
};

void soundSetMode( int slot, SoundMode mode )
{
	REG_IME = 0;
	MEMORY_BARRIER;

	sndSources[ slot ].mode = mode;

	REG_IME = 1;
	MEMORY_BARRIER;
};

void soundSetVolume( int slot, int volume )
{
	REG_IME = 0;
	MEMORY_BARRIER;

	sndSources[ slot ].volume = volume;

	REG_IME = 1;
	MEMORY_BARRIER;
};

void soundSetPosition( int slot, int pos )
{
	REG_IME = 0;
	MEMORY_BARRIER;

	sndSources[ slot ].cur = sndSources[ slot ].start + pos;

	REG_IME = 1;
	MEMORY_BARRIER;
};

void soundStop( int slot )
{
	REG_IME = 0;
	MEMORY_BARRIER;
	sndSources[ slot ].mode = SoundModeOff;
	sndSources[ slot ].reserved = false;
	REG_IME = 1;
	MEMORY_BARRIER;
};

bool soundIsPlaying( int slot, int clip )
{
    if ( slot < 0 )
        return false;

    REG_IME = 0;
	MEMORY_BARRIER;
	return sndSources[ slot ].clip == clip && sndSources[ slot ].mode != SoundModeOff;
	REG_IME = 1;
	MEMORY_BARRIER;
}

