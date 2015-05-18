#include "FPrimitive.h"
#include "FScene.h"
#include "..\Core\FLog.h"
#include "FLine.h"
#include "FText.h"
#include "FFillQuad.h"
#include "FObject2D.h"
#include "FImage2D.h"
#include "..\Base\FMetaData.h"
#include <new.h>



FObject * CreateObject2D( void * lpPlacement, va_list lpArgs )
{
	FVector2F * lpPos = va_arg( lpArgs, FVector2F * );
	FScene * lpScene = va_arg( lpArgs, FScene * );
	UI32 iLayer = va_arg( lpArgs, UI32 );
	FString * lpName = va_arg( lpArgs, FString * );

	return new (lpPlacement) FObject2D( *lpPos, lpScene, iLayer, *lpName );
}

FObject * CreateLine( void * lpPlacement, va_list lpArgs )
{
	FVector2F * lpStart = va_arg( lpArgs, FVector2F * );
	FVector2F * lpEnd = va_arg( lpArgs, FVector2F * );
	F32 fLineWidth = va_arg( lpArgs, F32 );
	FColor4F * lpColor = va_arg( lpArgs, FColor4F * );
	FScene * lpScene = va_arg( lpArgs, FScene * );
	UI32 iLayer = va_arg( lpArgs, UI32 );

	return new (lpPlacement) FLine( *lpStart, *lpEnd, fLineWidth, *lpColor, lpScene, iLayer );
}

FObject * CreateFillQuad( void * lpPlacement, va_list lpArgs )
{
	FVector2F * lpPos = va_arg( lpArgs, FVector2F * );
	FVector2F * lpDim = va_arg( lpArgs, FVector2F * );
	FColor4F * lpColor = va_arg( lpArgs, FColor4F * );
	UI32 iLayer = va_arg( lpArgs, UI32 );
	FScene * lpScene = va_arg( lpArgs, FScene * );

	return new (lpPlacement) FFillQuad( *lpPos, *lpDim, *lpColor, iLayer, lpScene );
}

FObject * CreateImage2D( void * lpPlacement, va_list lpArgs ) 
{
	FVector2F * lpPos = va_arg( lpArgs, FVector2F * );
	FScene * lpScene = va_arg( lpArgs, FScene * );
	UI32 iLayer = va_arg( lpArgs, UI32 );
	FString * lpName = va_arg( lpArgs, FString * );

	return new (lpPlacement) FImage2D( *lpPos, lpScene, iLayer, *lpName );
}

FObject * CreateText( void * lpPlacement, va_list lpArgs )
{
	FVector2F * lpPos = va_arg( lpArgs, FVector2F * );
	FColor4F * lpColor = va_arg( lpArgs, FColor4F * );
	F32 fFontSize = va_arg( lpArgs, F64 );
	FString * lpText = va_arg( lpArgs, FString * );
	FString * lpFontName = va_arg( lpArgs, FString * );
	UI32 iLayer = va_arg( lpArgs, UI32 );
	FScene * lpScene = va_arg( lpArgs, FScene * );

	return new (lpPlacement) FText( *lpPos, *lpColor, fFontSize, *lpText, *lpFontName, iLayer, lpScene );
}


void FPrimitive::InitMetaData()
{
	DEFINE_META( FPrimitive );
	DEFINE_META_SUPER( FLine, FPrimitive );
	DEFINE_META_SUPER( FText, FPrimitive );
	DEFINE_META_SUPER( FFillQuad, FPrimitive );
	DEFINE_META_SUPER( FObject2D, FPrimitive );
	DEFINE_META_SUPER( FImage2D, FPrimitive );

	DEFINE_META_CONSTR( FLine, "\\vector\\vector\\f\\color\\scene\\ui", CreateLine );
	DEFINE_META_CONSTR( FText, "\\vector\\color\\f\\string\\string\\ui\\scene", CreateText );
	DEFINE_META_CONSTR( FFillQuad, "\\vector\\vector\\color\\ui\\scene", CreateFillQuad );
	DEFINE_META_CONSTR( FObject2D, "\\vector\\scene\\ui\\string", CreateObject2D );
	DEFINE_META_CONSTR( FImage2D, "\\vector\\scene\\ui\\string", CreateImage2D );
}


FPrimitive::FPrimitive( UI32 iLayer, FScene * lpScene ) : iLayer( iLayer ), lpScene( lpScene ), fAlpha( 1.f ), bShow( true ), fScale( 1.f ),
	bAnimated( false )
{
	if( iLayer >= FScene::MAX_LAYER_COUNT )
	{
		FLog::PutWarning( "Need primitive layer is more than FScene::MAX_LAYER_COUNT, need %i", iLayer );
		iLayer = FScene::MAX_LAYER_COUNT - 1;
	}
	lpScene->AddObject( this );
}

FPrimitive::~FPrimitive()
{
	lpScene->RemoveObject( this );
}

void FPrimitive::Update( F32 fDTime )
{
}

UI32 FPrimitive::GetLayer()const
{
	return iLayer;
}

void FPrimitive::ChangeLayer( UI32 iLayer0 )
{
	lpScene->RemoveObject( this );
	if( iLayer >= FScene::MAX_LAYER_COUNT )
	{
		FLog::PutWarning( "Need primitive layer is more than FScene::MAX_LAYER_COUNT, need %i", iLayer );
		iLayer = FScene::MAX_LAYER_COUNT - 1;
	}
	else
		iLayer = iLayer0;
	lpScene->AddObject( this );
}

void FPrimitive::SetAlpha( F32 fAlpha0 )
{
	fAlpha = fAlpha0;
}

void FPrimitive::SetAnimated( bool bAnimated_ )
{
	if( bAnimated && !bAnimated_ )
		lpScene->RemoveObjectFromAnimList( this );
	else if( !bAnimated && bAnimated_ )
		lpScene->AddObjectToAnimList( this );

	bAnimated = bAnimated_;
}

void FPrimitive::Move( const FVector2F & vPos )
{
}

void FPrimitive::SetPos( const FVector2F & vPos )
{
}

void FPrimitive::Show( bool bShow_ )
{
	bShow = bShow_;
}

void FPrimitive::Scale( F32 fScale_ )
{
	fScale = fScale_;
}

void FPrimitive::Draw( const FRect & rViewport )
{
}

PRIM_ID FPrimitive::GetID()const
{
	return 0;
}

void FPrimitive::UpToFrontView()
{
	lpScene->RemoveObject( this );
	lpScene->AddObject( this );
}

void FPrimitive::DownToBackView()
{
	lpScene->RemoveObject( this );
	lpScene->AddToFrontObject( this );
}
