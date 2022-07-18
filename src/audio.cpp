#include "audio.h"
#include "string.h"

// **************************
// *    ЗВУКОВОЙ ДРАЙВЕР    *
// **************************

const int sndSourceCount = 8;
SoundSource sndSources[ sndSourceCount ];
// Частота воспроизведения звукового драйвера в герцах
const int sndFreqHz = 16384;
// Частота с которой будет вызваться прерывание для звука.
// Должно делить нацело SndFreqHz и желательно не превышать 100
const int sndIntrHz = 64;
// Необходимый размер звуковых банков
const int sndBankSize = sndFreqHz / sndIntrHz;

// Два звуковых банка, а так же 16-битный банк микшера
static i8 sndBank0[ sndBankSize ];
static i8 sndBank1[ sndBankSize ];
static i16 sndMixBank[ sndBankSize ];

// Указатель на первый источник звука (голова однонаправленного списка)
SoundSource *volatile sndFirstSource = nullptr;
// Номер текущего воспроизводящегося банка
volatile u8 sndCurBank = 0;
// Основная громкость звуков (master volume), fixed(8:8), 0x0100 как 1.0f
volatile i16 sndVolume = 0x0080;

// Процедура таймера для драйвера звука.
// Должна вызываться по таймеру, когда DMA уже считал текущий буфер вывода,
// чтобы переключить его на другой и заполнить текущий свежими данными.
void soundDriverTimer()
{
	// Меняем рабочий банк (через временную переменную)
	u8 curBank = 1 - sndCurBank;
	sndCurBank = curBank;
	// Переключаем DMA-канал на другой звуковой банк
	REG_DMA1CNT_H = 0; // выключим канал, чтобы обновить его параметры
	// Переключим источник данных
	REG_DMA1SAD = curBank ? (u32) sndBank1 : (u32) sndBank0;
	// Включим DMA-канал и тем самым применим новые параметры
	REG_DMA1CNT_H = DMA_SRC_INCR | DMA_REPEAT | DMA_START_SPECIAL | DMA_ENABLE;
	// Далее работаем с банком, который начнёт воспроизводится со следующего прерывания
	i8 *workBank = curBank ? sndBank0 : sndBank1;
	// Заполняем звуковой банк тишиной...
	memset( (void *) sndMixBank, 0, sizeof( sndMixBank ) );

	// Смешиваем источники звука в 16-битный банк микшера
	i16 *mixPtr;	// указатель в буфер микшера
	i8 *chPtr;	// указатель на 8-битные данные
	// Обходим список источников звука
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
			 		// При достижении конца данных источника...
			 		if ( workSource->mode == SoundModeOnce )
			 		{
			 			// ...у однократных заканчиваем воспроизведение
						workSource->mode = SoundModeOff;
						workSource->cur = workSource->start;
						break;
			 		}
			 		else
			 		{
			 			// ...у бесконечных возвращаемся в начало
			 			workSource->cur = workSource->start;
			 			chPtr = (signed char *) workSource->cur;
			 		};
				};
				// Так как volume это fixed(:8), то результат умножения надо
				// сдвинуть вправо на 8 бит, чтобы он остался целым числом
				*mixPtr++ += ((*chPtr++ * workSource->volume) >> 8);
				*mixPtr++ += ((*chPtr++ * workSource->volume) >> 8);
				*mixPtr++ += ((*chPtr++ * workSource->volume) >> 8);
				*mixPtr++ += ((*chPtr++ * workSource->volume) >> 8);

				workSource->cur++;
			};
		};
		workSource = workSource->next;	// переходим к следующему источнику
	};

	// Выводим намикшированное в 16-битном банке в банк вывода
	i16 vol = sndVolume; // закешируем volatile переменную
	mixPtr = sndMixBank;
	chPtr = workBank;
	for ( int i = 0; i < sndBankSize; i++ )
	{
		*chPtr++ = ((*mixPtr++ * vol) >> 8);
	};
};

// Инициализация звукового драйвера
void soundDriverInit()
{
	// Предварительно отключим всё касающееся звука
	// Выключаем реакцию на прерывание таймера 1
	REG_IE &= ~INT_TIMER1;
	// Выключаем звуковой чип
	REG_SOUNDCNT_X = 0;
	// Выключаем DMA1
	REG_DMA1CNT_H = 0;
	// Выключаем таймеры
	REG_TM0CNT = 0;
	REG_TM1CNT = 0;

	// Очищаем звуковые буфера
	memset( (void*) sndBank0, 0, sndBankSize );
	memset( (void*) sndBank1, 0, sndBankSize );

	// Сбрасываем первый источник звука и текущий банк
	sndFirstSource = nullptr;
	sndCurBank = 0;

	// Сбрасываем буфер воспроизводимых звуков
	for ( int i = 0; i < sndSourceCount; i++ )
	{
		sndSources[ i ].mode = SoundModeOff;
		sndSources[ i ].reserved = false;
		sndSources[ i ].next = &sndSources[ i + 1 ];
	};
	sndSources[ sndSourceCount - 1 ].next = nullptr;
	sndFirstSource = sndSources;

	// Канал A на 100% громкости и в оба стереоканала, сбрасываем
	// ему внутренний буфер и привязываем воспроизведение к первому таймеру
	REG_SOUNDCNT_H =	SOUND_A_VOL_100 |
				SOUND_A_LEFT | SOUND_A_RIGHT |
				SOUND_A_TIMER0 | SOUND_A_RESET;
	// Включаем звуковой чип
	REG_SOUNDCNT_X =	SOUND_ENABLE;

    	// Подготовимся к обработке прерываний по опустошению буферов
	REG_IE |= INT_TIMER1;			// Включим реакцию на INT_TIMER1

	// Настраиваем канал DMA1 на подпитывание звуковых данных в FIFO_A
	REG_DMA1DAD = (u32) &REG_FIFO_A;
	REG_DMA1SAD = (u32) sndBank0;
	REG_DMA1CNT_H = DMA_SRC_INCR | DMA_REPEAT |
			DMA_START_SPECIAL | DMA_ENABLE;

	// Настраиваем служебный таймер на прерывание для
	// переключения звуковых буферов
	REG_TM1D	= 65536 - sndBankSize;
	REG_TM1CNT	= TIMER_ENABLE | TIMER_OVERFLOW | TIMER_IRQ_ENABLE;

	// Запускаем таймер-драйвер звукового канала FIFO_A
	REG_TM0D	= 65536 - ((1 << 24) / sndFreqHz);
	REG_TM0CNT	= TIMER_ENABLE;

	// Восемь "затравочных" семплов для синхронизации
	// всех очередей (под вопросом)
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

