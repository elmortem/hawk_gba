#ifndef AUDIO_H
#define AUDIO_H

#include "utils.h"

// **************************
// *    �������� �������    *
// **************************

// ������ ��������������� ���������� �����
// Off - ����, Once - ����������, Repeat - ���������� (����������)
enum SoundMode
{
	SoundModeOff, SoundModeOnce, SoundModeRepeat
};

// �������� ����� ��� ��������� ��������
struct SoundSource
{
	SoundMode mode;		// ����� ���������������
	int clip;
	int volume;		// ��������� fixed(:8), 0x0100 ��� 1.0f
	const u32 *start;	// ������ �����������
	const u32 *end;		// ����� ����������� (���������-��-���������)
	const u32 *cur;		// ������� � ��������������� ��������
	SoundSource *next;	// ��������� �������� ����� � ������
	bool reserved;		// ����� ����� �������������� ���������� �����
				// ��� ���������� �������������
};

struct SoundLibEntry
{
	const u32 *start;
	int len;
};

extern const SoundLibEntry soundLib[];
extern SoundSource sounds[];
// ������� ��������������� ��������� �������� � ������
extern const int sndFreqHz;
// ������� � ������� ����� ��������� ���������� ��� �����.
// ������ ������ ������ SndFreqHz � ���������� �� ��������� 100
extern const int sndIntrHz;
// ����������� ������ �������� ������
extern const int sndBankSize;

// ��������� �� ������ �������� ����� (������ ����������������� ������)
extern SoundSource *volatile sndFirstSource;
// ����� �������� ������������������ �����
extern volatile u8 sndCurBank;
// �������� ��������� ������ (master volume), fixed(8:8), 0x0100 ��� 1.0f
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
