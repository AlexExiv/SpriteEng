#include "FFillQuad.h"
#include "FView.h"
#include "FScene.h"
#include "FShader.h"
#include <new.h>



const FString sFillQuadShader( "fill.shr" );

#define FILL_QUAD_VERT_COUNT 4
#define FILL_QUAD_IND_COUNT 6
#define FILL_QUAD_CONT_COUNT 2

struct FFillQuadData
{
	FVector2F vPos;
	FColor4F cColor;
};

FDrawDataCont dFillQuadCont[FILL_QUAD_CONT_COUNT] =
{
	{ FView::VERTEX_DATA, 2, FView::DATA_FLOAT, sizeof( FFillQuadData ), FIELD_OFFSET_( FFillQuadData, vPos ) },
	{ FView::COLOR_DATA, 4, FView::DATA_FLOAT, sizeof( FFillQuadData ), FIELD_OFFSET_( FFillQuadData, cColor ) }
};

FFillQuad::FFillQuad( const FVector2F & vPos, const FVector2F & vDimension, const FColor4F & cColor, UI32 iLayer, FScene * lpScene ) : FPrimitive( iLayer, lpScene ), vPos( vPos ), vDim( vDimension ), cColor( cColor )
{
	lpShader = (FShader *)FGraphObjectManager::GetInstance()->CreateObject( sFillQuadShader, FGraphObjectManager::OBJECT_SHADER );
	fAlpha = cColor.a;
}

FFillQuad::~FFillQuad()
{
	if( lpShader )
		FGraphObjectManager::GetInstance()->ReleaseObject( lpShader );
}

void FFillQuad::DrawCallback( UI32 iPrimCount, void * lpData, UI32 * lpIndeces )
{
	if( lpShader )
		lpShader->Bind();
	FView::GetMainView()->SetPrimDataArr( dFillQuadCont, FILL_QUAD_CONT_COUNT, lpData );
	FView::GetMainView()->DrawIndexed( FView::PRIM_TRIANGLES, iPrimCount, lpIndeces );
	if( lpShader )
		lpShader->UnBind();
}

UI32 FFillQuad::AddDataToSeq( void * lpData, UI32 * lpIndeces, UI32 iStartVert )
{
	//3--2
	//|\ |
	//| \|
	//0--1
	//

	lpIndeces[0] = iStartVert + 0;
	lpIndeces[1] = iStartVert + 1;
	lpIndeces[2] = iStartVert + 3;
	lpIndeces[3] = iStartVert + 1;
	lpIndeces[4] = iStartVert + 2;
	lpIndeces[5] = iStartVert + 3;

	FFillQuadData * lpFill = (FFillQuadData *)lpData;
	lpFill->vPos;
	lpFill->cColor = cColor;
	lpFill++;

	lpFill->vPos = FVector2F( vPos.x + vDim.x, vPos.y );
	lpFill->cColor = cColor;
	lpFill++;

	lpFill->vPos = FVector2F( vPos.x + vDim.x, vPos.y + vDim.y );
	lpFill->cColor = cColor;
	lpFill++;

	lpFill->vPos = FVector2F( vPos.x, vPos.y + vDim.y );
	lpFill->cColor = cColor;

	return 1;
}

UI32 FFillQuad::GetDataSizePerInst()const
{
	return FILL_QUAD_VERT_COUNT*sizeof( FFillQuadData );
}

UI32 FFillQuad::GetIndexCount()const
{
	return FILL_QUAD_IND_COUNT;
}

UI32 FFillQuad::GetVertexCount()const
{
	return FILL_QUAD_VERT_COUNT;
}

UI32 FFillQuad::DrawType()const
{
	return FPrimitive::DRAW_INDEXED;
}

void FFillQuad::Draw( const FRect & rViewport ) 
{
	lpScene->AddToDrawSeq( this );
}

void FFillQuad::Update( F32 fDTime )
{

}

PRIM_ID FFillQuad::GetID()const
{
	return FPrimitive::ID_FILL_QUAD;
}

void FFillQuad::Move( const FVector2F & vDPos )
{
	vPos += vDPos;
}

void FFillQuad::SetDim( const FVector2F & vDim0 )
{
	vDim = vDim0;
}

void FFillQuad::SetColor( const FColor4F & cColor0 )
{
	cColor = cColor0;
	cColor.a = fAlpha;
}

//FObject * FFillQuad::CreateObject( void * lpPlacement, const CHAR_ * lpTypes, va_list lpArgs )
//{
//	CHECK_OBJ_TYPES( lpTypes, lpContrTypes );
//
//	FVector2F * lpPos = va_arg( lpArgs, FVector2F * );
//	FVector2F * lpDim = va_arg( lpArgs, FVector2F * );
//	FColor4F * lpColor = va_arg( lpArgs, FColor4F * );
//	UI32 iLayer = va_arg( lpArgs, UI32 );
//	FScene * lpScene = va_arg( lpArgs, FScene * );
//
//	return new (lpPlacement) FFillQuad( *lpPos, *lpDim, *lpColor, iLayer, lpScene );
//}