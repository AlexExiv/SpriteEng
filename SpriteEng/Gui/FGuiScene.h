#ifndef __FGUI_SCENE_H__
#define __FGUI_SCENE_H__



#include "..\2D\FScene.h"
#include "..\Core\FList.h"


class FGuiScene : public FScene
{
protected:
	FPrimitiveList lPrimList, lAnimatedPrimList;

public:
	FGuiScene( UI32 iSceneWidth, UI32 iSceneHeight, FDrawSeq * lpDrawSeq );
	~FGuiScene();

	void BuildSeq( const FRect & rViewport );
	void Update( F32 fDTime );

	void AddObject( FPrimitive * lpPrim );
	void AddToFrontObject( FPrimitive * lpPrim );
	void RemoveObject( FPrimitive * lpPrim );
	void UpToBeginObject( FPrimitive * lpPrim );
	void AddObjectToAnimList( FPrimitive * lpPrim );
	void RemoveObjectFromAnimList( FPrimitive * lpPrim );
};


#endif