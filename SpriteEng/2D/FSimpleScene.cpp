#include "FSimpleScene.h"
#include "FPrimitive.h"
#include "..\Core\FLog.h"



FSimpleScene::FSimpleScene( UI32 iSceneWidth, UI32 iSceneHeight, FDrawSeq * lpDrawSeq ) : FScene( iSceneWidth, iSceneHeight, lpDrawSeq )
{
}

FSimpleScene::~FSimpleScene()
{
}


void FSimpleScene::BuildSeq( const FRect & rViewport )
{
	FPrimitiveIterator iIt = lPrimitiveList.Begin();
	for(;iIt != lPrimitiveList.End();iIt++ )
		iIt->Draw( rViewport );
}

void FSimpleScene::Update( F32 fDTime )
{
	FPrimitiveIterator iIt = lAnimatedPrimList.Begin();
	for(;iIt != lAnimatedPrimList.End();iIt++ )
		iIt->Update( fDTime );
}


void FSimpleScene::AddObject( FPrimitive * lpPrim )
{
	lPrimitiveList.PushBack( lpPrim );
	if( lpPrim->IsAnimated() )
		AddObjectToAnimList( lpPrim );
}

void FSimpleScene::AddToFrontObject( FPrimitive * lpPrim )
{
	if( lpPrim->IsAnimated() )
		AddObjectToAnimList( lpPrim );
	lPrimitiveList.PushFront( lpPrim );
}

void FSimpleScene::RemoveObject( FPrimitive * lpPrim )
{
	FPrimitiveIterator iIt = lPrimitiveList.Begin();
	for(;iIt != lPrimitiveList.End();iIt++ )
	{
		if( *iIt == lpPrim )
			break;
	}
	if( iIt == lPrimitiveList.End() )
	{
		FLog::PutError( "Try to remove not existing primitive in list, FSimpleScene" );
		return;
	}

	lPrimitiveList.Erase( iIt );
	if( lpPrim->IsAnimated() )
		RemoveObjectFromAnimList( lpPrim );
}

void FSimpleScene::AddObjectToAnimList( FPrimitive * lpPrim )
{
	lAnimatedPrimList.PushBack( lpPrim );
}

void FSimpleScene::RemoveObjectFromAnimList( FPrimitive * lpPrim )
{
	FPrimitiveIterator iIt = lAnimatedPrimList.Begin();
	for(;iIt != lAnimatedPrimList.End();iIt++ )
	{
		if( *iIt == lpPrim )
			break;
	}
	if( iIt == lAnimatedPrimList.End() )
	{
		FLog::PutError( "Try to remove not existing primitive in animated list, FSimpleScene" );
		return;
	}

	lAnimatedPrimList.Erase( iIt );
}
