#ifndef __FFILLQUAD_H__
#define __FFILLQUAD_H__



#include "FPrimitive.h"
#include "..\Core\FColor.h"
#include "..\Math\FVector2D.h"

class FShader;

class FFillQuad : public FPrimitive
{
	FVector2F vPos, vDim;
	FColor4F cColor;
	FShader * lpShader;

	void DrawCallback( UI32 iPrimCount, void * lpData, UI32 * lpIndeces );
	UI32 AddDataToSeq( void * lpData, UI32 * lpIndeces, UI32 iStartVert );
	UI32 GetDataSizePerInst()const;
	UI32 GetIndexCount()const;
	UI32 GetVertexCount()const;
	UI32 DrawType()const;

	//FObject * CreateObject( void * lpPlacement, const CHAR_ * lpTypes, va_list lpArgs ) ;

	MAKE_PRIM_FRIEND( FFillQuad );

public:
	FFillQuad( const FVector2F & vPos, const FVector2F & vDimension, const FColor4F & cColor, UI32 iLayer, FScene * lpScene );
	~FFillQuad();

	void Draw( const FRect & rViewport );
	void Update( F32 fDTime );

	PRIM_ID GetID()const;
	void Move( const FVector2F & vDPos );
	void SetPos( const FVector2F & vPos );
	void SetDim( const FVector2F & vDim );
	void SetColor( const FColor4F & cColor );

	DEFINE_OBJ_DERIVERED( FFillQuad );
};

DEFINE_OBJ_NAME( FFillQuad );


#endif