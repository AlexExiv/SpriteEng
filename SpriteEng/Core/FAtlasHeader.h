#ifndef __FATLAS_HEADER_H__
#define __FATLAS_HEADER_H__


#include "..\types.h"

#define ATLAS_FILE_TYPE (((UI32)'a') | (((UI32)'s') << 8) | (((UI32)'f') << 16) | (((UI32)0) << 24))
#define SPRITE_ATLAS_VER 1

#define ATLAS_COMPR_NONE 1
#define ATLAS_COMPR_RLE 2
#define ATLAS_COMPR_JPG 3
#define ATLAS_COMPR_PNG 4

//�����
#define ATLAS_PACKED 0x00000001
#define ATLAS_RGB 0x00000002
#define ATLAS_ALPHA 0x00000004
#define ATLAS_RGBA 0x00000008

//����� ��������
#define ATLAS_ANIM_LOOPED 0x00000001 

#define ATLAS_TYPE_FLOAT 1
#define ATLAS_TYPE_INT 2
#define ATLAS_TYPE_STR 3


struct FAtlasHeader
{
	UI32 iFileType;//��� ����� ���������� ����� "asf"
	UI32 iVersion;//������� ������

	union
	{
		struct
		{
			UI32 iAtlasNameOff, iAtlasNameLen;//��� ����� ���������� ������ �������� ������� � RGB ��� RGBA �������, ���� ������ �� ����������
			UI32 iAtlasAlphaNameOff, iAtlasAlphaNameLen;//��� ����� ���������� ������ �������� ������� � Alpha �������, ���� ������ �� ����������
		};
		struct
		{
			UI32 iRGBDataLen;//����� RGB ��� RGBA ������ � �����, ���� ���� �������
			UI32 iAlphaDataLen;//�����  Alpha ������ � �����, ���� ���� �������
		};
	};

	UI32 iTexWidth, iTexHeight;//������ ������ ����������� ������
	UI32 iCompr;//������� ������ � ��� ���
	UI32 iFlags;//�����

	UI32 iFrameCount;//���������� ������
	UI32 iAnimationCount;//���������� ��������
	UI32 iAddDataCount;//���������� ���������� �������������� ������

	UI32 iFrameDataStart;//������ ������ ���������� ������
	UI32 iAnimationDataStart;//������ ������ ���������� ��������
	UI32 iAdditionalDataStart;//������ ������ ���������� �������������� ������
	UI32 iTexAtlasDataStart;//������ ������ ����������� ������ ����� ����� ������������ ������ �����
	UI32 iStringDataStart;//������ ��������� ������
};

struct FFrameHeader
{
	UI32 iFrameNum;//����� �����

	UI32 x, y;//���������� ����� ������ �����
	UI32 iWidth, iHeight;//������ � ������ �����

	F32 u0, v0, u1, v1;//���������� ���������� �����

	UI32 iNameOff, iNameLen;//������ ����� �����, ����� ������������ ������ ����� � ����� �����
};

struct FAnimationHeader
{
	UI32 iStartFrame, iEndFrame;//������ � ����� ��������
	UI32 iAnimFlags;//����� ��������, �������� ����������� ��������
	UI32 iNameOff, iNameLen;//������ ����� ��������, ����� ������������ ������ ����� � ����� �����
};


struct FAddDataHeader
{
	UI32 iNameOff, iNameLen;//������ ����� ������������� ����, ����� ������������ ������ ����� � ����� �����
	UI32 iValType;//��� �������� ����������� � ����
	union
	{
		I32 iInt;//�����
		F32 fFloat;//�����������
		struct//������ ������ � �����
		{
			UI32 iStrOff, iStrLen;
		};
	};
};

#endif