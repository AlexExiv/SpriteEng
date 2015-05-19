#include "FLine.h"
#include "FScene.h"
#include "FDrawSeq.h"
#include "FView.h"
#include "FShader.h"
#include <new.h>



#define LINE_DRAW_CONT_COUNT 3

const FString sLineWidth( "fLineWidth" );
const FString sLineShaderName( "bundle\\line.shr" );


struct FLineDrawData
{
	FVector2F vPoint;
	FColor4F cColor;
	F32 fLineWidth;
};

FDrawDataCont dDrawCont[LINE_DRAW_CONT_COUNT] =
{
	{ FView::VERTEX_DATA, 2, FView::DATA_FLOAT, sizeof( FLineDrawData ), FIELD_OFFSET_( FLineDrawData, vPoint ) },
	{ FView::COLOR_DATA, 4, FView::DATA_FLOAT, sizeof( FLineDrawData ), FIELD_OFFSET_( FLineDrawData, cColor ) },
	{ FView::ATTRIB_DATA, 1, FView::DATA_FLOAT, sizeof( FLineDrawData ), FIELD_OFFSET_( FLineDrawData, fLineWidth ) }
};


IMPLEMENT_OBJ_DERIVERED( FLine );

FLine::FLine( FScene * lpScene, UI32 iLayer ) : FPrimitive( iLayer, lpScene )
{
	
}

FLine::FLine( const FVector2F & vStart, const FVector2F & vEnd, F32 fLineWidth, const FColor4F & cColor,  FScene * lpScene, UI32 iLayer ) 
	: FPrimitive( iLayer, lpScene ), vStart( vStart ), vEnd( vEnd ), fLineWidth( fLineWidth ), cLineColor( cColor )
{
	lpShader = (FShader *)FGraphObjectManager::GetInstance()->CreateObject( sLineShaderName, FGraphObjectManager::OBJECT_SHADER );
	fAlpha = cColor.a;
}

FLine::~FLine()
{
	if( lpShader )
		FGraphObjectManager::GetInstance()->ReleaseObject( lpShader );
}

void FLine::Draw( const FRect & rViewport )
{
	lpScene->AddToDrawSeq( this );
}

void FLine::Update( F32 fDTime )
{
}

void FLine::SetLineWidth( F32 fLineWidth0 )
{
	fLineWidth = fLineWidth0;
}

void FLine::SetLineColor( const FColor4F & cColor0 )
{
	cLineColor = cColor0;
	cLineColor.a = fAlpha;
}

void FLine::Move( const FVector2F & vDPos )
{
	vStart += vDPos;
	vEnd += vDPos;
}

void FLine::SetPos( const FVector2F & vPos_ )
{
	FVector2F vOldPos = vStart;
	vStart = vPos_;
	vEnd += (vStart - vOldPos);
}

void FLine::SetPoints( const FVector2F & vStart0, const FVector2F & vEnd0 )
{
	vStart = vStart0;
	vEnd = vEnd0;
}

PRIM_ID FLine::GetID()const
{
	return PRIM_ID( FPrimitive::ID_LINE );
}

void FLine::DrawCallback( UI32 iPrimCount, void * lpData, UI32 * lpIndeces )
{
	FView * lpView = FView::GetMainView();
	if( lpShader )
	{
		lpShader->Bind();
		lpShader->SetFloatPointer( sLineWidth, sizeof( FLineDrawData ), (F32 *)((UI8 *)lpData + FIELD_OFFSET_( FLineDrawData, fLineWidth )) );
	}
	lpView->SetPrimDataArr( dDrawCont, LINE_DRAW_CONT_COUNT, lpData );
	lpView->Draw( FView::PRIM_LINES, iPrimCount*2 );
	if( lpShader )
		lpShader->UnBind();
}

UI32 FLine::AddDataToSeq( void * lpData, UI32 * lpIndeces, UI32 iStartVert )
{
	FLineDrawData * lpLineData = (FLineDrawData *)lpData;

	lpLineData->vPoint = vStart;
	lpLineData->cColor = cLineColor;
	lpLineData->fLineWidth = fLineWidth;
	lpLineData++;
	lpLineData->vPoint = vEnd;
	lpLineData->cColor = cLineColor;
	lpLineData->fLineWidth = fLineWidth;

	return 1;
}

UI32 FLine::GetDataSizePerInst()const
{
	return sizeof( FLineDrawData )*2;
}

UI32 FLine::DrawType()const
{
	return 0;
}

UI32 FLine::GetIndexCount()const
{
	return 0;
}

UI32 FLine::GetVertexCount()const
{
	return 2;
}

//FObject * FLine::CreateObject( void * lpPlacement, const CHAR_ * lpTypes, va_list lpArgs )
//{
//	CHECK_OBJ_TYPES( lpTypes, lpContrTypes );
//
//	FVector2F * lpStart = va_arg( lpArgs, FVector2F * );
//	FVector2F * lpEnd = va_arg( lpArgs, FVector2F * );
//	F32 fLineWidth = va_arg( lpArgs, F32 );
//	FColor4F * lpColor = va_arg( lpArgs, FColor4F * );
//	FScene * lpScene = va_arg( lpArgs, FScene * );
//	UI32 iLayer = va_arg( lpArgs, UI32 );
//
//	return new (lpPlacement) FLine( *lpStart, *lpEnd, fLineWidth, *lpColor, lpScene, iLayer );
//}