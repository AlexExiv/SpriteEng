#ifndef __FVIEW_H__
#define __FVIEW_H__


#include "..\Core\FString.h"
#include "..\Math\FVector2D.h"
#include "..\Core\FColor.h"

class FTexture;
class FShader;
class FImageResource;
class FGraphObject;

struct FDrawDataCont
{
	UI32 iAttribType;//вид данных: коорд вершин, цвет, текстурные коорд и т.д.
	UI32 iCompCount;//кол-во компонент на вид
	UI32 iDataType;// тип компоненты целое, с плав точкой и тд
	UI32 iStride;//сдвиг между видами данных в байтах
	union
	{
		UI32 iDataStart;//начало данных
		void * lpData;//либо указатель на данные
	};
};

#ifdef WINDOWS_FAMILY

#include <Windows.h>

struct FWndAdd
{
	HWND hWnd;
};
#endif

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

	enum
	{
		F_API_OPENGL = 1,
		F_API_DIRECT3D
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

	virtual void BeginDraw() = 0;
	virtual void EndDraw() = 0;
	virtual void SwapBuffers() = 0;

	virtual FTexture * CreateTexture( FGraphObject * lpPlacement, const FString & sName ) = 0;
	virtual FTexture * CreateTexture( FGraphObject * lpPlacement, const FString & sName, const FImageResource * lpImg ) = 0;
	virtual FTexture * CreateTexture( const FString & sName, UI32 iWidth, UI32 iHeight, UI32 iFormat ) = 0;
	virtual FShader * CreateShader( FGraphObject * lpPlacement, const FString & sShaderName ) = 0;

	UI32 GetWidth()const;
	UI32 GetHeight()const;
	virtual UI32 GetTextureObjSize()const = 0;
	virtual UI32 GetShaderObjSize()const = 0;

	static void Init( UI32 iApi, UI32 iWidth, UI32 iHeight, void * lpAddData );
	static FView * GetMainView();
};

#endif