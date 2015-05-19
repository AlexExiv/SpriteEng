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
#include "..\Base\FBaseTypes.h"


IMPLEMENT_OBJ_DERIVERED( FPrimitive );

FObject * CreateObject2D( void * lpPlacement, va_list lpArgs )
{
	PFVector2F lpPos = va_arg( lpArgs, PFVector2F );
	FScene * lpScene = va_arg( lpArgs, FScene * );
	PFUInteger lpLayer = va_arg( lpArgs, PFUInteger );
	FString * lpName = va_arg( lpArgs, FString * );

	return new (lpPlacement) FObject2D( *lpPos, lpScene, *lpLayer, *lpName );
}

FObject * CreateLine( void * lpPlacement, va_list lpArgs )
{
	PFVector2F lpStart = va_arg( lpArgs, PFVector2F );
	PFVector2F lpEnd = va_arg( lpArgs, PFVector2F );
	FFloat * lpLineWidth = va_arg( lpArgs, FFloat * );
	PFColor4F lpColor = va_arg( lpArgs, PFColor4F );
	FScene * lpScene = va_arg( lpArgs, FScene * );
	PFUInteger lpLayer = va_arg( lpArgs, PFUInteger );

	return new (lpPlacement) FLine( *lpStart, *lpEnd, *lpLineWidth, *lpColor, lpScene, *lpLayer );
}

FObject * CreateFillQuad( void * lpPlacement, va_list lpArgs )
{
	PFVector2F lpPos = va_arg( lpArgs, PFVector2F );
	PFVector2F lpDim = va_arg( lpArgs, PFVector2F );
	PFColor4F lpColor = va_arg( lpArgs, PFColor4F );
	PFUInteger lpLayer = va_arg( lpArgs, PFUInteger );
	FScene * lpScene = va_arg( lpArgs, FScene * );

	return new (lpPlacement) FFillQuad( *lpPos, *lpDim, *lpColor, *lpLayer, lpScene );
}

FObject * CreateImage2D( void * lpPlacement, va_list lpArgs ) 
{
	PFVector2F lpPos = va_arg( lpArgs, PFVector2F );
	FScene * lpScene = va_arg( lpArgs, FScene * );
	PFUInteger lpLayer = va_arg( lpArgs, PFUInteger );
	FString * lpName = va_arg( lpArgs, FString * );

	return new (lpPlacement) FImage2D( *lpPos, lpScene, *lpLayer, *lpName );
}

FObject * CreateText( void * lpPlacement, va_list lpArgs )
{
	PFVector2F lpPos = va_arg( lpArgs, PFVector2F );
	PFColor4F lpColor = va_arg( lpArgs, PFColor4F );
	FFloat * lpFontSize = va_arg( lpArgs, FFloat * );
	FString * lpText = va_arg( lpArgs, FString * );
	FString * lpFontName = va_arg( lpArgs, FString * );
	PFUInteger lpLayer = va_arg( lpArgs, PFUInteger );
	FScene * lpScene = va_arg( lpArgs, FScene * );

	return new (lpPlacement) FText( *lpPos, *lpColor, *lpFontSize, *lpText, *lpFontName, *lpLayer, lpScene );
}


void FPrimitive::InitMetaData()
{
	FScene::InitMeta();

	DEFINE_META_SUPER( FPrimitive, FObject );
	DEFINE_META_SUPER( FLine, FPrimitive );
	DEFINE_META_SUPER( FText, FPrimitive );
	DEFINE_META_SUPER( FFillQuad, FPrimitive );
	DEFINE_META_SUPER( FObject2D, FPrimitive );
	DEFINE_META_SUPER( FImage2D, FPrimitive );

	DEFINE_META_CONSTR6( FLine, CreateLine, FVector2F_, FVector2F_, FFloat, FColor4F_, FScene, FUInteger );
    DEFINE_META_CONSTR7( FText, CreateText, FVector2F_, FColor4F_, FFloat, FString, FString, FUInteger, FScene );
	DEFINE_META_CONSTR5( FFillQuad, CreateFillQuad, FVector2F_, FVector2F_, FColor4F_, FUInteger, FScene );
	DEFINE_META_CONSTR4( FObject2D, CreateObject2D, FVector2F_, FScene, FUInteger, FString );
	DEFINE_META_CONSTR4( FImage2D, CreateImage2D, FVector2F_, FScene, FUInteger, FString );
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
