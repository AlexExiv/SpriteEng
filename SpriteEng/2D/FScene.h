#ifndef __FSCENE_H__
#define __FSCENE_H__


#include "..\Core\FList.h"
#include "..\Base\FObject.h"


class FPrimitive;
class FDrawSeq;

class FScene : public FObject
{
public:
	enum
	{
		MAX_LAYER_COUNT = 25
	};

protected:
	typedef FList<FPrimitive *, FFreePointer__<FPrimitive *>> FPrimitiveList;
	typedef FList<FPrimitive *, FFreePointer__<FPrimitive *>>::Iterator FPrimitiveIterator;

	UI32 iSceneWidth, iSceneHeight;
	FDrawSeq * lpDrawSeq;

public:
	FScene( UI32 iSceneWidth, UI32 iSceneHeight, FDrawSeq * lpDrawSeq );
	virtual ~FScene();

	void SetDrawSeq( FDrawSeq * lpDrawSeq );

	UI32 GetWidth()const;
	UI32 GetHeight()const;

	virtual void AddToDrawSeq( FPrimitive * lpPrim );
	virtual void BuildSeq( const FRect & rViewport ) = 0;
	virtual void Update( F32 fDTime ) = 0;

	virtual void AddObject( FPrimitive * lpPrim ) = 0;
	virtual void AddToFrontObject( FPrimitive * lpPrim ) = 0;
	virtual void RemoveObject( FPrimitive * lpPrim ) = 0;
	virtual void AddObjectToAnimList( FPrimitive * lpPrim ) = 0;
	virtual void RemoveObjectFromAnimList( FPrimitive * lpPrim ) = 0;

	DEFINE_OBJ_DERIVERED( FScene );

	static void InitMeta();
};

DEFINE_OBJ_NAME( FScene );

#endif