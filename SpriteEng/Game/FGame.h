#ifndef __FGAME_H__
#define __FGAME_H__


#include "..\Math\FVector2D.h"
#include "..\Base\FObject.h"

class FWorld;

class FGame : public FObject
{
//protected:
//	FWorld * lpWorld;

public:
	FGame( UI32 iObjReserved );
	~FGame();

	virtual void ClickDown( const FVector2F & vPos, UI32 iIndex );
	virtual void ClickUp( const FVector2F & vPos, UI32 iIndex );
	virtual void ClickMove( const FVector2F & vPos, UI32 iIndex );
	virtual void KeyDown();
	virtual void KeyUp();

	void Update( F32 fDTime );
	virtual void Draw();

	static void InitMeta();
};

#endif