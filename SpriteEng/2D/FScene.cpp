#include "FScene.h"
#include "FDrawSeq.h"
#include "..\Core\FLog.h"



FScene::FScene( UI32 iSceneWidth, UI32 iSceneHeight, FDrawSeq * lpDrawSeq ) : iSceneWidth( iSceneWidth ), iSceneHeight( iSceneHeight ),
	lpDrawSeq( lpDrawSeq )
{
}

FScene::~FScene()
{
}

void FScene::AddToDrawSeq( FPrimitive * lpPrim )
{
	AssertFatal( lpDrawSeq, "Can't build drawed sequence, because FDrawSeq is not set" );
	lpDrawSeq->AddPrimitive( lpPrim );
}

void FScene::SetDrawSeq( FDrawSeq * lpDrawSeq_ )
{
	lpDrawSeq = lpDrawSeq_;
}

UI32 FScene::GetWidth()const
{
	return iSceneWidth;
}

UI32 FScene::GetHeight()const
{
	return iSceneHeight;
}
