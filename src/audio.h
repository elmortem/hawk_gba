#ifndef AUDIO_H
#define AUDIO_H

#include "utils.h"

// **************************
// *    ЗВУКОВОЙ ДРАЙВЕР    *
// **************************

// Режимы воспроизведения источников звука
// Off - выкл, Once - однократно, Repeat - зацикленно (бесконечно)
enum SoundMode
{
	SoundModeOff, SoundModeOnce, SoundModeRepeat
};

// Источник звука для звукового драйвера
struct SoundSource
{
	SoundMode mode;		// Режим воспроизведения
	int clip;
	int volume;		// Громкость fixed(:8), 0x0100 как 1.0f
	const u32 *start;	// Начало аудиоданных
	const u32 *end;		// Конец аудиоданных (следующий-за-последним)
	const u32 *cur;		// Текущий к воспроизведению фрагмент
	SoundSource *next;	// Следующий источник звука в списке
	bool reserved;		// Канал звука зарезервирован клиентским кодом
				// для повторного использования
};

struct SoundLibEntry
{
	const u32 *start;
	int len;
};

extern const SoundLibEntry soundLib[];
extern SoundSource sounds[];
// Частота воспроизведения звукового драйвера в герцах
extern const int sndFreqHz;
// Частота с которой будет вызваться прерывание для звука.
// Должно делить нацело SndFreqHz и желательно не превышать 100
extern const int sndIntrHz;
// Необходимый размер звуковых банков
extern const int sndBankSize;

// Указатель на первый источник звука (голова однонаправленного списка)
extern SoundSource *volatile sndFirstSource;
// Номер текущего воспроизводящегося банка
extern volatile u8 sndCurBank;
// Основная громкость звуков (master volume), fixed(8:8), 0x0100 как 1.0f
extern volatile i16 sndVolume;

extern void soundDriverTimer();
extern void soundDriverInit();
extern void soundPlayOnce( int sndNum, int volume = 0x0100 );
extern int soundPlay( int sndNum, SoundMode mode, int volume = 0x0100 );
extern void soundSetMode( int sndNum, SoundMode mode );
extern void soundSetVolume( int sndNum, int volume );
extern void soundSetPosition( int sndNum, int pos );
extern void soundStop( int slot );
extern bool soundIsPlaying( int slot, int clip );


#endif
