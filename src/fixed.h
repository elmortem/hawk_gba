#ifndef _FIXED_H
#define _FIXED_H

#include "gba_defs.h"

// ������� � �������������� ������������ ���������� ������� ����� �� 0 �� 255.
// �������������� ������� ��������� ����� ����� � ������ ���������� �� ����� 256.
// �������� ������� �������� � ������� fixed(4:12) ��� ��������.
extern const u16 sin_table[ 256 ];

// ����� ���� ang. ���� �������������� � ������� fixed[:8] � � ������� 
// ��������� �����, ��� � ������ ���������� (2*Pi ������) ���������� 256 ��������.
// ��������� ������������ � ������� fixed(:12). ����� ���� ������������ �� ����
// ������� ���������� �� �������, ������������� ���� ��������� �������������
// ����� ����� ��������� ���������� � ����������� ���� ��������� � ������.
inline int sinfx12( int ang )
{
	int res;
	int frac = ang & 255;
	ang >>= 8;
	if ( frac == 0 )
	{
		// ������ � ������� ������� ������ ����
		res = ((i16)(sin_table[ ang & 255 ]));
	}
	else
	{
		// ������������ ����� ��������
		int r1 = ((i16)(sin_table[ ang & 255 ]));
		int r2 = ((i16)(sin_table[ (ang + 1) & 255 ]));
		res = (r1 * (256 - frac) + r2 * (frac)) >> 8;
	};
	return res;
};

// ������� ���� ang.
inline int cosfx12( int ang )
{
	return sinfx12( ang + (64 << 8) );
};

#endif // _FIXED_H