#include "FWorld.h"
#include "..\2D\FScene.h"
#include "..\Core\FLog.h"


static FWorld * lpCurWorld = NULL;

IMPLEMENT_OBJ_DERIVERED( FWorld );

FWorld::FWorld( FScene * lpScene, FGame * lpGame ) : lpScene( lpScene ), fWorldWidth( lpScene->GetWidth() ), 
	fWorldHeight( lpScene->GetHeight() )
{
	FWorld::SetCurrentWorld( this );
}

FWorld::~FWorld()
{
}

void FWorld::Update( F32 fDTime )
{
}

//FObjectAllocator * FWorld::GetAllocator()const
//{
//	return lpObjectAllocator;
//}

void FWorld::SetCurrentWorld( FWorld * lpWorld )
{
	if( lpWorld )
		lpCurWorld = lpWorld;
}

FWorld * FWorld::GetCurrentWorld()
{
	AssertFatal( lpCurWorld, "No one world is initialized" );
	return lpCurWorld;
}

FScene * FWorld::GetScene()const
{
	return lpScene;
}

F32 FWorld::GetWorldWidth()const
{
	return fWorldWidth;
}

F32 FWorld::GetWorldHeight()const
{
	return fWorldHeight;
}

