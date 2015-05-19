#include "FGuiController.h"
#include "FGuiScene.h"
#include "FGuiNavigator.h"
#include "FGuiView.h"
#include "FGuiImage.h"
#include "FGuiLabel.h"
#include "FGuiNavigator.h"
#include "..\Core\FLog.h"
#include "..\Base\FMetaData.h"
#include <new.h>
#include "..\Base\FBaseTypes.h"



FObject * CreateGuiNavigator( void * lpPlacement, va_list lpArgs )
{
	return new (lpPlacement) FGuiNavigator();
}

FObject * CreateGuiView( void * lpPlacement, va_list lpArgs )
{
	PFRect lpRect = va_arg( lpArgs, PFRect );
	FGuiController * lpController = va_arg( lpArgs, FGuiController * );
	return new (lpPlacement) FGuiView( *lpRect, lpController );
}

FObject * CreateGuiLabel( void * lpPlacement, va_list lpArgs )
{
	PFVector2F lpPos = va_arg( lpArgs, PFVector2F );
	FString * lpText = va_arg( lpArgs, FString * );
	FString * lpFont = va_arg( lpArgs, FString * );
	PFColor4F lpColor = va_arg( lpArgs, PFColor4F );
	PFFloat lpFontSize = va_arg( lpArgs, PFFloat );
	FGuiView * lpView = va_arg( lpArgs, FGuiView * );
	return new (lpPlacement) FGuiLabel( *lpPos, *lpText, *lpFont, *lpColor, *lpFontSize, lpView );
}

FObject * CreateGuiImage( void * lpPlacement, va_list lpArgs )
{
	PFVector2F lpPos = va_arg( lpArgs, PFVector2F );
	FString * lpName = va_arg( lpArgs, FString * );
	FGuiView * lpView = va_arg( lpArgs, FGuiView * );
	return new (lpPlacement) FGuiImage( *lpPos, *lpName, lpView );
}


void FGuiController::InitMeta()
{
	DEFINE_META_SUPER( FGuiController, FObject );
	DEFINE_META_SUPER( FGuiNavigator, FGuiController );
	DEFINE_META_SUPER( FGuiView, FObject );
	DEFINE_META_SUPER( FGuiImage, FGuiView );
	DEFINE_META_SUPER( FGuiLabel, FGuiView );
	DEFINE_META_SUPER( FGuiScene, FScene );

	DEFINE_META_CONSTR0( FGuiNavigator, CreateGuiNavigator );
	DEFINE_META_CONSTR2( FGuiView, CreateGuiView, FRect_, FGuiController );
	DEFINE_META_CONSTR3( FGuiImage, CreateGuiImage, FVector2F_, FString, FGuiView );
	DEFINE_META_CONSTR6( FGuiLabel, CreateGuiLabel, FVector2F_, FString, FString, FColor4F_, FFloat, FGuiView );
}

IMPLEMENT_OBJ_DERIVERED( FGuiController );

FGuiController::FGuiController( FGuiScene * lpScene ) : lpGuiScene( lpScene ), lpNavigator( NULL ), lpMainView( NULL )
{
}

FGuiController::FGuiController( FGuiController * lpRootController ) : lpGuiScene( lpRootController->GetGuiScene() ), lpNavigator( NULL ), lpMainView( NULL )
{
}


FGuiController::~FGuiController()
{
	AssertFatal( lpNavigator == NULL, "Try to delete FGuiController, which is in FGuiNavigator stack" );
}


bool FGuiController::ClickDown( const FVector2F & vPos, UI32 iIndex )
{
	if( !lpMainView )
		return false;

	return lpMainView->ClickDown( vPos, iIndex );
}

bool FGuiController::ClickUp( const FVector2F & vPos, UI32 iIndex )
{
	if( !lpMainView )
		return false;

	return lpMainView->ClickUp( vPos, iIndex );
}

bool FGuiController::ClickMove( const FVector2F & vPos, UI32 iIndex )
{
	if( !lpMainView )
		return false;

	return lpMainView->ClickMove( vPos, iIndex );
}

bool FGuiController::KeyDown()
{
	return false;
}

bool FGuiController::KeyUp()
{
	return false;
}


FGuiScene * FGuiController::GetGuiScene()const
{
	return lpGuiScene;
}

FGuiNavigator * FGuiController::GetNavigator()const
{
	return lpNavigator;
}

FGuiView * FGuiController::GetView()const
{
	return lpMainView;
}

void FGuiController::Update( F32 fDTime )
{
	lpMainView->Update( fDTime );
}


void FGuiController::SetNavigator( FGuiNavigator * lpNavigator_ )
{
	lpNavigator = lpNavigator_;
}



void FGuiController::ControllerDidAppear()
{
	//lpMainView->UpToFrontView();
}

void FGuiController::ControllerDidDisappear()
{
}

