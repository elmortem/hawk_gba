#ifndef _GBA_DEFS_H
#define _GBA_DEFS_H

typedef signed char i8;
typedef unsigned char u8;
typedef signed short i16;
typedef unsigned short u16;
typedef signed int i32;
typedef unsigned int u32;

// ======================
// =    ����� ������    =
// ======================

#define EW_RAM_START		0x02000000
#define EW_RAM_SIZE		262144
#define IW_RAM_START		0x03000000
#define IW_RAM_SIZE		32768
#define IO_RAM_START		0x04000000
#define IO_RAM_SIZE		1024
#define BGR_PAL_RAM_START	0x05000000
#define BGR_PAL_RAM_SIZE	512
#define SPR_PAL_RAM_START	0x05000200
#define SPR_PAL_RAM_SIZE	512
#define VID_RAM_START		0x06000000
#define VID_RAM_SIZE		98304
#define OAM_RAM_START		0x07000000
#define OAM_RAM_SIZE		1024
#define ROM_START		0x08000000
#define SRAM_START		0x0E000000
#define SRAM_SIZE		32768

#define SRAM_BUFFER		((volatile u8 *) SRAM_START)

// ======================
// =       �����        =
// ======================

// ������ RGB ���������� ��������� ������ 15-������� �����
#define RGB(r,g,b)  ((r)+((g)<<5)+((b)<<10)) 

// *** ������� �������� �����������
#define REG_DISPCNT	(*((volatile u32 *) 0x4000000))
// ����� ��������:
// ���� 0-2: ����� �����������
// ����� 0: 240x160 - �������� ����������. �������� 4 ������������� ������ ����� (BG0-BG3).
#define MODE_0		0x0
// ����� 1: 240x160 - �������� ����������. �������� 2 ������������� ������ ����� (BG0-BG1)
// � ���� � �������� ��������������� (BG2).
#define MODE_1		0x1
// ����� 2: 240x160 - �������� ����������. �������� 2 ������ ����� � �������� 
// ��������������� (BG2-BG3).
#define MODE_2		0x2
// ����� 3: 240x160x15. ���������� � ������ 0x06000000, ������ ������� �������� 2 ����� 
// 15-������� RGB-�����. ��-�� ������� �������� ����������� �� ����� ������ ��������.
#define MODE_3		0x3
// ����� 4: 240x160x8. ���������� � ������ 0x06000000, ����� ������ �������� �� ������ 
// 0x0600A000. ������������ ����� ��������������� �������������� ����� MODE_PAGE_2, ��� 
// ��������� ����������� �����������. ������������ ������� ������ ������ � 16-������ 
// ������� �� 256 ����� �� ������ BGR_PALETTE.
#define MODE_4		0x4
// ����� 5: 160x128x16. ���������� � ������ 0x06000000, ��-�� ����������� ���������� 
// ���� ����� ������ �������� �� ������ 0x0600A000.
#define MODE_5		0x5
// ��� 4: ��� ���������� ������� ����������� ����������� �� ������ ��������.
#define MODE_PAGE_2		0x0010
// ��� 5: ���� 1, �� ������������ ��������� ������ � OAM �� ����� ���������� HBlank �����
// ���������� ���������� ������������ ������������ �� ����� ������ ��������.
#define MODE_HBLANK_IS_FREE	0x0020
// ��� 6: ����� ���������� ������������� �������� (������ ��� 8x8 ��������).
//        � ����� �������� ������� ���� (������) ��������� � ���� ������-��������, ��������� 
//        ����������� � ���� ��������� �������:
//        1 - �������� �����, ����� ���������� ������ ��� ������������ ����� � ������� 
//            �������������� �������
//        0 - ������ ������ �������� ��������� ������ � ������� ������ � 32 ����� � 
//            ������� ����������� �� �������� ������� � ���� ������� ����������-�����������.
#define MODE_LINEAR_SPRITES	0x0040
// ��� 7: ��������� ������� - ������������ ����� ���������� ��������� ������ ������, ��
// ����� ������, �� ��������� � �����������, ��� ��������� �������� � ��� �� ������ ��������.
#define MODE_DISPLAY_OFF	0x0080
// ���� 8-11: �������� ������ BG0-BG3. 
// � ���������� ������������ ����� ��������� �������� BG2.
#define BG0_ENABLE		0x0100
#define BG1_ENABLE		0x0200
#define BG2_ENABLE		0x0400
#define BG3_ENABLE		0x0800
// ��� 12: �������� ����������� ��������.
#define SPR_ENABLE		0x1000
// ���� 13-15: �������� ����.
#define WIN0_ENABLE		0x2000
#define WIN1_ENABLE		0x4000
#define SPRWIN_ENABLE		0x8000

// *** ������� ������� ��������� �� ����� ������ ��������
// �������� �� 0 �� 159 �� ����� ����������� ����� ������� � ����� ��� ������������� 68 ��� 
// �� ���������� ����� ���������� ������. 
#define REG_VCOUNT	(*((volatile u16 *) 0x4000006))

// *** �������� ���������� ��������� ������
#define REG_BG0CNT	(*((volatile u16 *) 0x4000008))
#define REG_BG1CNT	(*((volatile u16 *) 0x400000A))
#define REG_BG2CNT	(*((volatile u16 *) 0x400000C))
#define REG_BG3CNT	(*((volatile u16 *) 0x400000E))
// ������� �����:
// ��������� (�� 0 �� 3)
#define BG_PRIORITY(x)		(x)
// ����� ������ ������ ������ (������� �������� TILE DATA)
#define	BG_TILE_DATA_AT_0000	0x00
#define	BG_TILE_DATA_AT_4000	0x04
#define	BG_TILE_DATA_AT_8000	0x08
#define	BG_TILE_DATA_AT_C000	0x0C
// ����� �������
#define BG_MOSAIC		0x40
// ������� � ������ 8-������ (����), ����� �������� ����� 
// ������ ������ ����� �� 16 ������, � ������� � ������ 4-������.
#define BG_COLOR_256		0x80
// ���������� ����� ������ ����� ������ (�� 0 �� 31) - �� ������������ � �������.
#define BG_TILE_MAP_AT(x)	((x)<<8)
// �������� ����� ������ ����� ������, ���������� ����������� ������
// ����������� ��� �� 2048 ����, ������� � ������ VID_RAM.
#define BG_TILE_MAP_ADDR(x)	(0x6000000 + (x) * 0x800)
// ����� ���������� ��� ������������ ����
#define BG_ROT_OVERLAP		0x2000
// ������� ��������� ����� (TILE MAP)
#define	BG_SIZE_256x256		0x0000
#define	BG_SIZE_512x256		0x4000
#define	BG_SIZE_256x512		0x8000
#define	BG_SIZE_512x512		0xC000
// ������� ����������� ����� (TILE MAP)
#define	BG_ROT_SIZE_128x128	0x0000
#define	BG_ROT_SIZE_256x256	0x4000
#define	BG_ROT_SIZE_512x512	0x8000
#define	BG_ROT_SIZE_1024x1024	0xC000

// *** �������� ��������������� � ������������� �������� ��������� �����
// ��� �� �������� ��� ����������� ����� - ��� ��������� ���������� ���� ��������.
// HOFS - ��� ��������������, � VOFS - ������������ ���������� ������� ����������������
// ����, ������� ����� ��������� � ����� �����-������� ���� ������.
#define REG_BG0HOFS	(*((volatile u16 *) 0x4000010))
#define REG_BG0VOFS	(*((volatile u16 *) 0x4000012))
#define REG_BG1HOFS	(*((volatile u16 *) 0x4000014))
#define REG_BG1VOFS	(*((volatile u16 *) 0x4000016))
#define REG_BG2HOFS	(*((volatile u16 *) 0x4000018))
#define REG_BG2VOFS	(*((volatile u16 *) 0x400001A))
#define REG_BG3HOFS	(*((volatile u16 *) 0x400001C))
#define REG_BG3VOFS	(*((volatile u16 *) 0x400001E))

// *** �������� ��������������� � ������������� �������� ����������� �����,
// � ��� �� ������������� ������ ������� �������������� ��� ���.
// �������� ������ ��� BG2 � BG3 � ������ ��������/���������������.
// ����������� ������� ������� BG2 (PA, PB, PC, PD):
#define REG_BG2PA	(*((volatile i16 *) 0x4000020))
#define REG_BG2PB	(*((volatile i16 *) 0x4000022))
#define REG_BG2PC	(*((volatile i16 *) 0x4000024))
#define REG_BG2PD	(*((volatile i16 *) 0x4000026))
// ���������� BG2 (PX, PY):
#define REG_BG2X	(*((volatile u32 *) 0x4000028))
#define REG_BG2Y	(*((volatile u32 *) 0x400002C))
// ������������ ������� ������� BG3 (PA, PB, PC, PD):
#define REG_BG3PA	(*((volatile i16 *) 0x4000030))
#define REG_BG3PB	(*((volatile i16 *) 0x4000032))
#define REG_BG3PC	(*((volatile i16 *) 0x4000034))
#define REG_BG3PD	(*((volatile i16 *) 0x4000036))
// ���������� BG3 (PX, PY):
#define REG_BG3X	(*((volatile u32 *) 0x4000038))
#define REG_BG3Y	(*((volatile u32 *) 0x400003C))

// *** ������� ������� �������������
#define REG_DISPSTAT		(*((volatile u16 *) 0x4000004))
// �����:
// ��� 0: 1, ���� ���������� VBlank (�� ����� ��������� ����� REG_VCOUNT)
#define STAT_VBLANK		0x0001
// ��� 1: 1, ���� ���������� HBlank
#define STAT_HBLANK		0x0002
// ��� 2: 1, ���� ��������� ���������� �� VCOUNT
#define STAT_VCOUNT		0x0004
// ��� 3: ���������� ���������� ���������� � ������ VBlank
#define VBLANK_IRQ_ENABLE	0x0008
// ��� 4: ���������� ���������� ���������� � ������ HBlank
#define HBLANK_IRQ_ENABLE	0x0010
// ��� 5: ���������� ���������� ���������� �� VCOUNT
#define VCOUNT_IRQ_ENABLE	0x0020
// ���� 8-15: �������� VCOUNT ��� ������� ���������� ��������� ����� ����������
#define VCOUNT_IRQ_VALUE( x )	(((u16)x) << 8)

// *** ������� � ������ �����������

// ����� �������, ������������ �������� ��������� (256 ����� �� 2 ����� RGB)
#define BGR_PALETTE ((u16 *) BGR_PAL_RAM_START)
// ����� �������, ������������ ��������� (256 ����� �� 2 ����� RGB)
#define SPR_PALETTE ((u16 *) SPR_PAL_RAM_START)
// �������� ������ �����������
#define VID_BUFFER ((u16 *) VID_RAM_START)

// *** �������

// ������ ���������� ������
#define SPR_TILES ((u16 *) (VID_RAM_START + 0x10000))
// ��������� �������� �������
struct OAM_Entry
{
	u16 attr0, attr1, attr2, attr3;
};
// ������ �������� �������� (�� 0 �� 127)
#define SPR_BUFFER ((OAM_Entry *) (OAM_RAM_START))
// ������� �� ������� �������� ������������ ������
// �������� �������� � ���� �������������������
// �������� � ����������� �� 0 �� 31.
#define SPR_ROT_A( x ) (SPR_BUFFER[(x) * 4 + 0].attr3)
#define SPR_ROT_B( x ) (SPR_BUFFER[(x) * 4 + 1].attr3)
#define SPR_ROT_C( x ) (SPR_BUFFER[(x) * 4 + 2].attr3)
#define SPR_ROT_D( x ) (SPR_BUFFER[(x) * 4 + 3].attr3)
// ������� ����� �������� 0:
// ���� 0-7: ���������� Y �������
#define ATTR0_Y( y )		((y) & 255)
// ��� 8: �������� �������/���������������
#define ATTR0_ROTATED		0x0100
// ��� 9 (1): �������� �������� (������ ��� ROTATED=1)
#define ATTR0_DOUBLED		0x0200
// ��� 9 (2): ��������� ����������� ������� (������ ��� ROTATED=0)
#define ATTR0_DISABLED		0x0200
// ���� 10-11: ����� ������� (���� �� ���): �������, ����������, ����� ����.
#define ATTR0_MODE_NORMAL	0x0000
#define ATTR0_MODE_TRANSPARENT	0x0400
#define ATTR0_MODE_WINDOW	0x0800
// ��� 12: �������� �������
#define ATTR0_MOSAIC		0x1000
// ��� 13: 256-������� ����� (����� 16-�������)
#define ATTR0_COLOR_256		0x2000
// ��� 14-15: ����� ������� (���� �� ���). �������� ������� ����� �������, 
// ������� � ���������� ���������� � ��������� ATTR1.
// (SQ - ����������, H - ��������������, V - ������������)
#define ATTR0_SHAPE_SQ		0x0000
#define ATTR0_SHAPE_H		0x4000
#define ATTR0_SHAPE_V		0x8000

// ������� ����� �������� 1:
// ���� 0-8: ���������� X �������
#define ATTR1_X( x )		((x) & 511)
// ���� 9-13 (������ ��� ROTATED=1): ������ ������� �������� (�� 0 �� 31)
#define ATTR1_ROT_INDEX( x )	(((x) & 31) << 9)
// ��� 12 (������ ��� ROTATED=0): ����������� �������������
#define ATTR1_H_FLIP		0x1000
// ��� 13 (������ ��� ROTATED=0): ����������� �����������
#define ATTR1_V_FLIP		0x2000
// ���� 14-15 (������ ��� ATTR0_SHAPE_SQ): ������ ������� � �������� (���� �������)
#define ATTR1_SQ_8x8		0x0000
#define ATTR1_SQ_16x16		0x4000
#define ATTR1_SQ_32x32		0x8000
#define ATTR1_SQ_64x64		0xC000
// ���� 14-15 (������ ��� ATTR0_SHAPE_H): ������ ������� � �������� (���� �������)
#define ATTR1_H_16x8		0x0000
#define ATTR1_H_32x8		0x4000
#define ATTR1_H_32x16		0x8000
#define ATTR1_H_64x32		0xC000
// ���� 14-15 (������ ��� ATTR0_SHAPE_V): ������ ������� � �������� (���� �������)
#define ATTR1_V_8x16		0x0000
#define ATTR1_V_8x32		0x4000
#define ATTR1_V_16x32		0x8000
#define ATTR1_V_32x64		0xC000

// ������� ����� �������� 2:
// ���� 0-9: ������ ����� (� ������� �� ������ ����� ������ ����� �
// ������� ������ ��������� ���� 32*������ �� ������ �������� ������
// ��������, �.�. ��� 256-������� ������� ��������� ������ �������.
#define ATTR2_TILE( x )		((x) & 1023)
// ���� 10-11: ��������� (��� �� ��� � ����� �� 0 �� 3).
#define ATTR2_PRIORITY( x )	(((x) & 3) << 10)
// ���� 12-15 (������ ��� 16-�������): ������� 4 ���� ��������� �������
#define ATTR2_PALETTE( x )	(((x) & 15) << 12)

// *** �������� ���������� ����-��������

// *** ������� ���������� ������� �������
#define REG_MOSAIC		(*((volatile u16 *) 0x400004C))
// (���������� ���� �� �������...)

// *** ������� ���������� �����-�������������, ���������� � �������������
#define REG_BLENDCNT		(*((volatile u16 *) 0x4000050))
// �����:
// ���� 0-5 - ����� ��� �������� ���������� ��� ����������:
// BG0-BG3 - ����, SPR - �������, BACK - ������ ���.
#define BLEND_SRC_BG0		0x01
#define BLEND_SRC_BG1		0x02
#define BLEND_SRC_BG2		0x04
#define BLEND_SRC_BG3		0x08
#define BLEND_SRC_SPR		0x10
#define BLEND_SRC_BACK		0x20
// ���� 6-7 ����� ����������: OFF - ����, BLEND - ����������������, 
// LIGHT - ������������, DARK - ����������.
#define BLEND_MODE_OFF		0x0000
#define BLEND_MODE_ALPHA	0x0040
#define BLEND_MODE_LIGHT	0x0080
#define BLEND_MODE_DARK		0x00C0
// ���� 8-13 - ����� ��� �������� ����� ��� ����������:
// BG0-BG3 - ����, SPR - �������, BACK - ������ ���.
#define BLEND_DEST_BG0		0x0100
#define BLEND_DEST_BG1		0x0200
#define BLEND_DEST_BG2		0x0400
#define BLEND_DEST_BG3		0x0800
#define BLEND_DEST_SPR		0x1000
#define BLEND_DEST_BACK		0x2000

// *** ������� ���������� �������������� �����-������������
#define REG_BLENDALPHA		(*((volatile u16 *) 0x4000052))
#define BLEND_SRC_A( x )	(x)
#define BLEND_DEST_A( x )	((x) << 8)
// *** ������� ���������� ��������� ���������/������������
#define REG_BLENDCOLY		(*((volatile u16 *) 0x4000054))

// ======================
// =       ������       =
// ======================

// ������� ������� ������� ������ (0 - ������, 1 - ������!)
#define REG_KEYS	(*((volatile u16 *) 0x04000130))
// ������� ����� ������ � ��������
#define KEY_A		1
#define KEY_B		2
#define KEY_SELECT	4
#define KEY_START	8
#define KEY_RIGHT	16
#define KEY_LEFT	32
#define KEY_UP		64
#define KEY_DOWN	128
#define KEY_R		256
#define KEY_L		512

// ������� ���������� ��������
#define REG_KEYCNT	(*((volatile u16 *) 0x04000132))

// ======================
// =      �������       =
// ======================

// �������� ������ (D) � ���������� (CNT) ���������������� ���������.
// ������ (D) �� ������ ���������� ������� �������� �������, � �� ������
// ��������� ��������� �������� ����� ��� ������������.
#define REG_TM0D	(*((volatile u16 *) 0x4000100))
#define REG_TM0CNT	(*((volatile u16 *) 0x4000102))
#define REG_TM1D	(*((volatile u16 *) 0x4000104))
#define REG_TM1CNT	(*((volatile u16 *) 0x4000106))
#define REG_TM2D	(*((volatile u16 *) 0x4000108))
#define REG_TM2CNT	(*((volatile u16 *) 0x400010A))
#define REG_TM3D	(*((volatile u16 *) 0x400010C))
#define REG_TM3CNT	(*((volatile u16 *) 0x400010E))

// ����� ��������� ���������� ��������� (CNT)
#define TIMER_FREQUENCY_SYSTEM  0x0
#define TIMER_FREQUENCY_64      0x1
#define TIMER_FREQUENCY_256     0x2
#define TIMER_FREQUENCY_1024    0x3
#define TIMER_OVERFLOW          0x4
#define TIMER_IRQ_ENABLE        0x40
#define TIMER_ENABLE            0x80

// ======================
// =     ����������     =
// ======================

// *** ������� ������ ��������� ����������. ���� 0-�� ��� ����� 0, �� �����
// ���������� �����������. ����������� ���������� ������ ������� ���������
// ���������� ������ ��������� � �������� �� ������� �� ������.
#define REG_IME			(*((volatile u16 *) 0x4000208))
// *** ������� ���������/���������� ������� �� �� ���������� ���� ���������� 
// (��. ����� ����� ���������� ����)
// �������� ��������, ��� ����� ��������� ����� � REG_IE ����� ��� ������ 
// �������� ��������� ���������� � ��������������� ����������� ����� 
// ����������-��������� (�������� � REG_DISPSTAT ��� �������������).
#define REG_IE			(*((volatile u16 *) 0x4000200))
// *** ������� �������� ������������ ����������. �������� �� ��, ��� �����
// ���������� ����� �� ����� ���������� ��� � REG_IE, �� ������ ���� ������� ����
// ��������� �������� ������ ��� ������� � ���������� ����������, ��� �����
// �� ����� ���� ������������ ��������. ���������� ���������� ������
// �������� ���� ������� 1 (�������!) � ��������������� ��� �� ������.
#define REG_IF			(*((volatile u16 *) 0x4000202))
// ����� ��������� ��������� ����������
typedef void (*intr_handler_type)(void);
#define REG_INTERRUPT		(*((volatile intr_handler_type *) 0x3007FFC))
// ����� ����� ���������� � ��������� REG_IE � REG_IF
#define INT_VBLANK 0x0001
#define INT_HBLANK 0x0002
#define INT_VCOUNT 0x0004
#define INT_TIMER0 0x0008
#define INT_TIMER1 0x0010
#define INT_TIMER2 0x0020
#define INT_TIMER3 0x0040
#define INT_COM    0x0080
#define INT_DMA0   0x0100
#define INT_DMA1   0x0200
#define INT_DMA2   0x0400
#define INT_DMA3   0x0800
#define INT_BUTTON 0x1000
#define INT_CART   0x2000

// ======================
// =        DMA         =
// ======================

// *** ����� ��������� (source)
// DMA0 �� ����� ���������� ���� ROM_START (��������� �����)
#define REG_DMA0SAD		(*((volatile u32 *) 0x40000b0))
#define REG_DMA1SAD		(*((volatile u32 *) 0x40000bc))
#define REG_DMA2SAD		(*((volatile u32 *) 0x40000c8))
#define REG_DMA3SAD		(*((volatile u32 *) 0x40000d4))

// *** ����� �������� (destination)
// DMA3 ����� ���������� ���� ROM_START (��������� �� �����!)
#define REG_DMA0DAD		(*((volatile u32 *) 0x40000b4))
#define REG_DMA1DAD		(*((volatile u32 *) 0x40000c0))
#define REG_DMA2DAD		(*((volatile u32 *) 0x40000cc))
#define REG_DMA3DAD		(*((volatile u32 *) 0x40000d8))

// *** ����� ��������� (16 ��� 32-������) � �������� (count)
// ������� DMA3 16-������, ��������� 14-������. �������� 0 
// ������������� "���������" (65536 ��� DMA3 � 16384 ��� ���������).
#define REG_DMA0CNT_L		(*((volatile u16 *) 0x40000b8))
#define REG_DMA1CNT_L		(*((volatile u16 *) 0x40000c4))
#define REG_DMA2CNT_L		(*((volatile u16 *) 0x40000d0))
#define REG_DMA3CNT_L		(*((volatile u16 *) 0x40000dc))

// *** ������� ���������� ������� (��. ����� ����)
#define REG_DMA0CNT_H		(*((volatile u16 *) 0x40000ba))
#define REG_DMA1CNT_H		(*((volatile u16 *) 0x40000c6))
#define REG_DMA2CNT_H		(*((volatile u16 *) 0x40000d2))
#define REG_DMA3CNT_H		(*((volatile u16 *) 0x40000de))

// *** ����� ��������� ����������:
// ���� 5-6: ����� ���� ������ ��������:
// INCR - ����������, DECR - ����������, FIXED - ������������� �����,
// INCR_RELOAD - ����� ����� ��� ������ ����������� (REPEAT) -
//   DMA-���������� ��� ������� ����� ������������� ����������
//   ����� �������� ���������� ����� ��������.
#define DMA_DEST_INCR		0x0000
#define DMA_DEST_DECR		0x0020
#define DMA_DEST_FIXED		0x0040
#define DMA_DEST_INCR_RELOAD	0x0060
// ���� 7-8: ����� ���� ������ ���������
// ��� ����� ����������� DEST_INCR_RELOAD!
#define DMA_SRC_INCR		0x0000
#define DMA_SRC_DECR		0x0080
#define DMA_SRC_FIXED		0x0100
// ��� 9: ����� �����������
#define DMA_REPEAT		0x0200
// ��� 10: ������ ������������ ���������
#define DMA_16BIT		0x0000
#define DMA_32BIT		0x0400
// ��� 11: (������ ��� DMA3) GamePack DRQ (?)
#define DMA_GAMEPACK_DRQ	0x0800
// ���� 12-13: ������ ������� DMA-��������:
// IMMEDIATE - ����� ����� �������� ���� ��������� (DMA_ENABLE) �� 0 � 1
// VBLANK    - ��� ����� � VBlank
// HBLANK    - ��� ����� � HBlank
// SPECIAL   - ��� DMA1 � DMA2 - "������������" �������� FIFO-�������,
//             ��� DMA3 - �����������
#define DMA_START_IMMEDIATE	0x0000
#define DMA_START_VBLANK	0x1000
#define DMA_START_HBLANK	0x2000
#define DMA_START_SPECIAL	0x3000
// ��� 14: ������������ ���������� ��� ���������� �����������
#define DMA_IRQ_ENABLE		0x4000
// ��� 15: ��� ��������� DMA-������
#define DMA_ENABLE		0x8000

// ======================
// =        ����        =
// ======================

// *** FIFO-������ ��� ���� ������� (A � B) DirectSound .
// 32-������ �������� � ������ FIFO_A � FIFO_B ��������� ������ ������
// (����� �� ������), ��� ���� ������� ������������� ������� �����.
// FIFO-������ ����� ������ 8 32-������ �������� ��� 32 ������ (�����).
#define REG_FIFO_A_L		(*((volatile u16 *) 0x40000a0))
#define REG_FIFO_A_H		(*((volatile u16 *) 0x40000a2))
#define REG_FIFO_A		(*((volatile u32 *) 0x40000a0))
#define REG_FIFO_B_L		(*((volatile u16 *) 0x40000a4))
#define REG_FIFO_B_H		(*((volatile u16 *) 0x40000a6))
#define REG_FIFO_B		(*((volatile u32 *) 0x40000a4))

// *** ������� ������ ���������� PSG (�������������)
// ���� ���� �� ������������ � ��� ����� � �� ���������.
#define REG_SOUNDCNT_L		(*((volatile u16 *) 0x4000080))

// *** ������� ���������� DirectSound � ����� ���������� PSG.
#define REG_SOUNDCNT_H		(*((volatile u16 *) 0x4000082))
// ���� 0-1: ��������� �������� ������������ (PSG).
#define SOUND_PSG_VOL_25	0x0000
#define SOUND_PSG_VOL_50	0x0001
#define SOUND_PSG_VOL_100	0x0002
// ��� 2: ��������� ������ A (50% ��� 100%)
#define SOUND_A_VOL_50		0x0000
#define SOUND_A_VOL_100		0x0004
// ��� 3: ��������� ������ B (50% ��� 100%)
#define SOUND_B_VOL_50		0x0000
#define SOUND_B_VOL_100		0x0008
// ��� 8: ����� ������ A � ������ �������
#define SOUND_A_RIGHT		0x0100
// ��� 9: ����� ������ A � ����� �������
#define SOUND_A_LEFT		0x0200
// ��� 10: ����� �������������� ������� ������ A (0-�� ��� 1-��)
#define SOUND_A_TIMER0		0x0000
#define SOUND_A_TIMER1		0x0400
// ��� 11: ��� ��������� � 1 ������� FIFO-�����
#define SOUND_A_RESET		0x0800
// ��� 12: ����� ������ B � ������ �������
#define SOUND_B_RIGHT		0x1000
// ��� 13: ����� ������ B � ����� �������
#define SOUND_B_LEFT		0x2000
// ��� 14: ����� �������������� ������� ������ B (0-�� ��� 1-��)
#define SOUND_B_TIMER0		0x0000
#define SOUND_B_TIMER1		0x4000
// ��� 15: ��� ��������� � 1 ������� FIFO-�����
#define SOUND_B_RESET		0x8000

// *** ������� ������� PSG � ��������� ��������� ����.
#define REG_SOUNDCNT_X		(*((volatile u16 *) 0x4000084))
// ���� 0-3: ����� 1 ���� ��������������� ���������� (PSG)
// ���������� ���� � ������������ � 0 ����� ��� ���������������.
// ���� 7: ���������� ���� ���������/���������� ��������� ����.
#define SOUND_ENABLE		0x80

// ======================
// =       ������       =
// ======================

// ������ ������ ��������� �� BIOS (��������� �����) ��� ����������
#if defined ( __thumb__ )
#define	SystemCall(N) __asm("SWI "#N"\n" ::: "r0","r1","r2","r3")
#else
#define	SystemCall(N) __asm("SWI "#N"<<16\n" ::: "r0","r1","r2","r3")
#endif

// ������� ��������� BIOS, ������� ������������� ���������
// �� ����������� ���������� �� VBlank. ���������� VBlank ������
// � ����������� � ���������������, � ��������� ���������
// ���������� ������� �� ������ ��� ���������� REG_IF �������� �� �������� OR
// �� �� ����� ��������, ��� ��� � ���� ���������� �� ���� BIOS:
// BIOS_WAIT_FLAG |= value;
// ������������� ���� ������� �������� ����������� ������� �����������.
inline void VBlankIntrWait()	{ SystemCall( 5 ); };

// ���� BIOS ��� ����������� ������ VBlankIntrWait() (� ��., ��. ����)
#define BIOS_WAIT_FLAG		(*((volatile u16 *) 0x3007FF8))

// "������ ������" ���������� ���������� ��������� � ������ ��������
// ���������� (���� ��� ���������� � ���������) �� ������� � �� ������ 
// ������������� � ���������� ������ ����� ������� (�� ���� �������
// �� ������������ �������� � ��������� �����������). 
// ��� ��������� ������������ �� ������ ������ ��������� � �������� 
// �������������� ����������, ��������� ����� ���� ������� �� ��� ������ 
// �� ���������� �������� ��������. 
// ���� ������ ��������� ������ �� ������ �����������, �� �� ����������!
// ������� ������ ��� ���������� �������� ������������, �� ��� ���
// GBA �������� ���������������� ��������, �� � �� ��� �� �����
// �������� ��������.
#define MEMORY_BARRIER		__asm volatile("": : :"memory")

#endif
