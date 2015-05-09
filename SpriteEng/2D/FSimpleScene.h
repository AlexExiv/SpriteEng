#ifndef __FSIMPLE_SCENE_H__
#define __FSIMPLE_SCENE_H__


#include "FScene.h"
#include "..\Core\FList.h"


class FSimpleScene : public FScene
{
	FPrimitiveList lPrimitiveList, lAnimatedPrimList;

public:
	FSimpleScene( UI32 iSceneWidth, UI32 iSceneHeight, FDrawSeq * lpDrawSeq );
	~FSimpleScene();

	void BuildSeq( const FRect & rViewport );
	void Update( F32 fDTime );

	void AddObject( FPrimitive * lpPrim );
	void AddToFrontObject( FPrimitive * lpPrim );
	void RemoveObject( FPrimitive * lpPrim );
	void AddObjectToAnimList( FPrimitive * lpPrim );
	void RemoveObjectFromAnimList( FPrimitive * lpPrim );
};

#endif