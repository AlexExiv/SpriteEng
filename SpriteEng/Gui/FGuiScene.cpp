#include "FGuiScene.h"
#include "..\2D\FPrimitive.h"
#include "..\Core\FLog.h"



FGuiScene::FGuiScene( UI32 iSceneWidth, UI32 iSceneHeight, FDrawSeq * lpDrawSeq ) : FScene( iSceneWidth, iSceneHeight, lpDrawSeq ), 
	lPrimList( 128, 0 ), lAnimatedPrimList( 128, 0 )
{
}

FGuiScene::~FGuiScene()
{
}


void FGuiScene::BuildSeq( const FRect & rViewport )
{
	FPrimitiveIterator iIt = lPrimList.Begin();
	for(;iIt != lPrimList.End();iIt++ )
		iIt->Draw( rViewport );
}

void FGuiScene::Update( F32 fDTime )
{
	FPrimitiveIterator iIt = lAnimatedPrimList.Begin();
	for(;iIt != lAnimatedPrimList.End();iIt++ )
		iIt->Update( fDTime );
}


void FGuiScene::AddObject( FPrimitive * lpPrim )
{
	if( lpPrim->IsAnimated() )
		AddObjectToAnimList( lpPrim );
	lPrimList.PushBack( lpPrim );
}

void FGuiScene::AddToFrontObject( FPrimitive * lpPrim )
{
	if( lpPrim->IsAnimated() )
		AddObjectToAnimList( lpPrim );
	lPrimList.PushFront( lpPrim );
}

void FGuiScene::RemoveObject( FPrimitive * lpPrim )
{
	FPrimitiveIterator iIt = lPrimList.Begin();
	for(;iIt != lPrimList.End();iIt++ )
	{
		if( (*iIt) == lpPrim )
			break;
	}

	if( iIt == lPrimList.End() )
	{
		FLog::PutError( "Try to delete unknown primitive from FGuiScene" );
		return;
	}

	lPrimList.Erase( iIt );
	if( lpPrim->IsAnimated() )
		RemoveObjectFromAnimList( lpPrim );
}

void FGuiScene::UpToBeginObject( FPrimitive * lpPrim )
{
	RemoveObject( lpPrim );
	lPrimList.PushFront( lpPrim );
}

void FGuiScene::AddObjectToAnimList( FPrimitive * lpPrim )
{
	lAnimatedPrimList.PushBack( lpPrim );
}

void FGuiScene::RemoveObjectFromAnimList( FPrimitive * lpPrim )
{
	FPrimitiveIterator iIt = lAnimatedPrimList.Begin();
	for(;iIt != lAnimatedPrimList.End();iIt++ )
	{
		if( (*iIt) == lpPrim )
			break;
	}

	if( iIt == lAnimatedPrimList.End() )
	{
		FLog::PutError( "Try to delete unknown primitive from FGuiScene" );
		return;
	}

	lAnimatedPrimList.Erase( iIt );
}

