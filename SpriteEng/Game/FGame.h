#ifndef __FGAME_H__
#define __FGAME_H__


#include "..\Math\FVector2D.h"
#include "FGameObject.h"

class FWorld;

class FGame : public FGameObject
{
//protected:
//	FWorld * lpWorld;

public:
	enum
	{
		GAME_INVERT_Y = 0x00000001
	};

	FGame( UI32 iObjReserved );
	~FGame();

	virtual void ClickDown( const FVector2F & vPos, UI32 iIndex, UI32 iFlags );
	virtual void ClickUp( const FVector2F & vPos, UI32 iIndex, UI32 iFlags );
	virtual void ClickMove( const FVector2F & vPos, UI32 iIndex, UI32 iFlags );
	virtual void KeyDown();
	virtual void KeyUp();

	void Update( F32 fDTime );
	virtual void Draw();

	static void InitMeta();

	DEFINE_OBJ_DERIVERED( FGame );
};

DEFINE_OBJ_NAME( FGame );

#endif