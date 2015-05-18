#ifndef __FLINE_H__
#define __FLINE_H__


#include "FPrimitive.h"
#include "..\Core\FColor.h"
#include "..\Math\FVector2D.h"


class FShader;

class FLine : public FPrimitive
{
protected:
	FVector2F vStart, vEnd;
	FColor4F cLineColor;
	F32 fLineWidth;
	FShader * lpShader;

	void DrawCallback( UI32 iPrimCount, void * lpData, UI32 * lpIndeces );
	UI32 AddDataToSeq( void * lpData, UI32 * lpIndeces, UI32 iStartVert );
	UI32 GetDataSizePerInst()const;
	UI32 DrawType()const;
	UI32 GetIndexCount()const;
	UI32 GetVertexCount()const;

	//FObject * CreateObject( void * lpPlacement, const CHAR_ * lpTypes, va_list lpArgs ) ;

	MAKE_PRIM_FRIEND( FLine );

public:
	FLine( FScene * lpScene, UI32 iLayer );
	FLine( const FVector2F & vStart, const FVector2F & vEnd, F32 fLineWidth, const FColor4F & cColor,  FScene * lpScene, UI32 iLayer );
	~FLine();

	void Draw( const FRect & rViewport );
	void Update( F32 fDTime );

	void SetLineWidth( F32 fLineWidth );
	void SetLineColor( const FColor4F & cColor );
	void Move( const FVector2F & vVec );
	void SetPos( const FVector2F & vPos );
	void SetPoints( const FVector2F & vStart, const FVector2F & vEnd );
	PRIM_ID GetID()const;
};

#endif