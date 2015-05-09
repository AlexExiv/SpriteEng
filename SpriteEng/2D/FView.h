#ifndef __FVIEW_H__
#define __FVIEW_H__


#include "..\Core\FString.h"
#include "..\Math\FVector2D.h"
#include "..\Core\FColor.h"

class FTexture;
class FShader;
class FImageResource;

struct FDrawDataCont
{
	UI32 iAttribType;//��� ������: ����� ������, ����, ���������� ����� � �.�.
	UI32 iCompCount;//���-�� ��������� �� ���
	UI32 iDataType;// ��� ���������� �����, � ���� ������ � ��
	UI32 iStride;//����� ����� ������ ������ � ������
	union
	{
		UI32 iDataStart;//������ ������
		void * lpData;//���� ��������� �� ������
	};
};


class FView 
{
public:
	enum
	{
		PRIM_POINTS = 0,
		PRIM_LINES,
		PRIM_LINES_STRIP,
		PRIM_TRIANGLES,
		PRIM_TRIANGLES_STRIP,
		PRIM_TRIANGLES_FAN
	};

	enum
	{
		DATA_FLOAT = 0,
		DATA_INT32,
		DATA_UINT32
	};

	enum
	{
		VERTEX_DATA = 0,
		COLOR_DATA,
		TEXCOORD_DATA,
		ATTRIB_DATA
	};

protected:
	UI32 iWidth, iHeight;

public:
	FView( UI32 iWidth, UI32 iHeight );
	virtual ~FView(){}

	virtual void LineWidth( F32 fWidth ) = 0;
	virtual void SetViewport( UI32 x, UI32 y, UI32 iWidth, UI32 iHeight ) = 0;
	virtual void ResetViewport() = 0;
	virtual void SetPrimData( const FDrawDataCont * lpContainers, UI32 iContCount ) = 0;
	virtual void SetPrimDataArr( const FDrawDataCont * lpContainers, UI32 iContCount, void * lpData ) = 0;
	virtual void Draw( UI32 iPrimType, UI32 iPrimCount ) = 0;
	virtual void DrawIndexed( UI32 iPrimType, UI32 iPrimCount, const void * lpIndeces ) = 0;

	virtual FTexture * CreateTexture( const FString & sName ) = 0;
	virtual FTexture * CreateTexture( const FString & sName, const FImageResource * lpImg ) = 0;
	virtual FTexture * CreateTexture( const FString & sName, UI32 iWidth, UI32 iHeight, UI32 iFormat ) = 0;
	virtual FShader * CreateShader( const FString & sShaderName ) = 0;

	UI32 GetWidth()const;
	UI32 GetHeight()const;

	static void Init( UI32 iWidth, UI32 iHeight );
	static FView * GetMainView();
};

#endif