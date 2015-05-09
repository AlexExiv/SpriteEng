#include "FView.h"
#include "FImage2D.h"
#include "..\Math\FVector2D.h"
#include "..\Core\FColor.h"
#include "FShader.h"
#include "FTexture.h"
#include "FScene.h"
#include <stdarg.h>
#include <new.h>


#define IMAGE_DATA_CONT_COUNT 3

struct FImageVertexData
{
	FVector2F vPoint;
	FVector2F vTexCoord;
	FColor4F cColor;
};


const FString sImageShaderName( "image2d.shr" );
const FString sImageTex( "tImage" );


const FDrawDataCont dImageDataCont[IMAGE_DATA_CONT_COUNT] =
{
	{ FView::VERTEX_DATA, 2, FView::DATA_FLOAT, sizeof( FImageVertexData ), FIELD_OFFSET_( FImageVertexData, vPoint ) },
	{ FView::TEXCOORD_DATA, 2, FView::DATA_FLOAT, sizeof( FImageVertexData ), FIELD_OFFSET_( FImageVertexData, vTexCoord ) },
	{ FView::COLOR_DATA, 4, FView::DATA_FLOAT, sizeof( FImageVertexData ), FIELD_OFFSET_( FImageVertexData, cColor ) }
};



FImage2D::FImage2D( const FVector2F & vPos, FScene * lpScene, UI32 iLayer, const FString & sName ) : FPrimitive( iLayer, lpScene ), vPos( vPos ), lpShader( NULL ), lpTexture( NULL ), iTexBlock( 0 )
{
	lpTexture = (FTexture *)FGraphObjectManager::GetInstance()->CreateObject( sName, FGraphObjectManager::OBJECT_TEXTURE );
	lpShader = (FShader *)FGraphObjectManager::GetInstance()->CreateObject( sName, FGraphObjectManager::OBJECT_SHADER );
	if( lpTexture )
	{
		iWidth = lpTexture->GetWidth();
		iHeight = lpTexture->GetHeight();
	}
	if( lpShader )
		iTexBlock = lpShader->GetTextureBlock( sImageTex );
}

FImage2D::~FImage2D()
{
	if( lpTexture )
		FGraphObjectManager::GetInstance()->ReleaseObject( lpTexture );
	if( lpShader )
		FGraphObjectManager::GetInstance()->ReleaseObject( lpShader );
}

UI32 FImage2D::GetWidth()const
{
	return iWidth;
}

UI32 FImage2D::GetHeight()const
{
	return iHeight;
}

FVector2F FImage2D::GetPos()const
{
	return vPos;
}

void FImage2D::Move( const FVector2F & vDPos )
{
	vPos += vDPos;
}

PRIM_ID FImage2D::GetID()const
{
	return PRIM_ID( lpTexture );
}

void FImage2D::Draw( const FRect & rViewport )
{
	lpScene->AddToDrawSeq( this );
}

void FImage2D::Update( F32 fDTime )
{
}

UI32 FImage2D::AddDataToSeq( void * lpData, UI32 * lpIndeces, UI32 iStartVert )
{
	if( !lpTexture )
		return 0;

	FColor4F cColor( 0.5f, 0.6f, 0.1f, fAlpha );

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

	FImageVertexData * lpVertData = (FImageVertexData *)lpData;
	//#0
	lpVertData->cColor = cColor;
	lpVertData->vPoint = vPos;
	lpVertData->vTexCoord = FVector2F( 0.f, 1.f );
	lpVertData++;
	//#1
	lpVertData->cColor = cColor;
	lpVertData->vPoint = FVector2F( vPos.x + iWidth, vPos.y );
	lpVertData->vTexCoord = FVector2F( 1.f, 1.f );
	lpVertData++;
	//#2
	lpVertData->cColor = cColor;
	lpVertData->vPoint = FVector2F( vPos.x + iWidth, vPos.y + iHeight );
	lpVertData->vTexCoord = FVector2F( 1.f, 0.f );
	lpVertData++;
	//#3
	lpVertData->cColor = cColor;
	lpVertData->vPoint = FVector2F( vPos.x, vPos.y + iHeight );
	lpVertData->vTexCoord = FVector2F( 0.f, 0.f );

	return 1;
}

void FImage2D::DrawCallback( UI32 iPrimCount, void * lpData, UI32 * lpIndeces )
{
	if( !lpTexture )
		return ;

	if( lpShader )
		lpShader->Bind();
	if( lpTexture )
		lpTexture->Bind( iTexBlock );

	FView::GetMainView()->SetPrimDataArr( dImageDataCont, IMAGE_DATA_CONT_COUNT, lpData );
	FView::GetMainView()->DrawIndexed( FView::PRIM_TRIANGLES, iPrimCount*6, lpIndeces );

	if( lpShader )
		lpTexture->UnBind( iTexBlock );
	if( lpTexture )
		lpShader->UnBind();
}

UI32 FImage2D::GetDataSizePerInst()const
{
	return 4*sizeof( FImageVertexData );
}

UI32 FImage2D::GetIndexCount()const
{
	return 6;
}

UI32 FImage2D::GetVertexCount()const
{
	return 4;
}

UI32 FImage2D::DrawType()const
{
	return FPrimitive::DRAW_INDEXED;
}

//FObject * FImage2D::CreateObject( void * lpPlacement, const CHAR_ * lpTypes, va_list lpArgs ) 
//{
//	CHECK_OBJ_TYPES( lpTypes, lpContrTypes );
//
//	FVector2F * lpPos = va_arg( lpArgs, FVector2F * );
//	FScene * lpScene = va_arg( lpArgs, FScene * );
//	UI32 iLayer = va_arg( lpArgs, UI32 );
//	FString * lpName = va_arg( lpArgs, FString * );
//
//	return new (lpPlacement) FImage2D( *lpPos, lpScene, iLayer, *lpName );
//}

