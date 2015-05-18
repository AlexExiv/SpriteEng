#ifndef __FIMAGE2D_H__
#define __FIMAGE2D_H__


#include "FPrimitive.h"

class FTexture;
class FShader;


class FImage2D : public FPrimitive
{
	FTexture * lpTexture;
	FShader * lpShader;

	FVector2F vPos;
	UI32 iWidth, iHeight;
	UI32 iTexBlock;

	void DrawCallback( UI32 iPrimCount, void * lpData, UI32 * lpIndeces );
	UI32 AddDataToSeq( void * lpData, UI32 * lpIndeces, UI32 iStartVert );
	UI32 GetDataSizePerInst()const;
	UI32 GetIndexCount()const;
	UI32 GetVertexCount()const;
	UI32 DrawType()const;

	//FObject * CreateObject( void * lpPlacement, const CHAR_ * lpTypes, va_list lpArgs ) ;

	MAKE_PRIM_FRIEND( FImage2D );

public:
	FImage2D( const FVector2F & vPos, FScene * lpScene, UI32 iLayer, const FString & sName );
	~FImage2D();

	UI32 GetWidth()const;
	UI32 GetHeight()const;
	FVector2F GetPos()const;
	PRIM_ID GetID()const;

	void Draw( const FRect & rViewport );
	void Update( F32 fDTime );

	void Move( const FVector2F & vDPos );
	void SetPos( const FVector2F & vPos );
};

#endif