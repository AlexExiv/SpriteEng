#ifndef __FOBJECT2D_H__
#define __FOBJECT2D_H__


#include "FPrimitive.h"
#include "..\Core\FString.h"
#include "..\Math\FVector2D.h"

class FAnimation2D;

class FObject2D : public FPrimitive
{
	FAnimation2D * lpAnimation;
	FVector2F vPos;
	UI32 iStartFrame, iEndFrame, iCurFrame;
	F32 fFrameTime, fCurTime;
	bool bLooped;

	void DrawCallback( UI32 iPrimCount, void * lpData, UI32 * lpIndeces );
	UI32 AddDataToSeq( void * lpData, UI32 * lpIndeces, UI32 iStartVert );
	UI32 GetDataSizePerInst()const;
	UI32 GetIndexCount()const;
	UI32 GetVertexCount()const;
	UI32 DrawType()const;

	//FObject * CreateObject( void * lpPlacement, const CHAR_ * lpTypes, va_list lpArgs ) ;

	MAKE_PRIM_FRIEND( FObject2D );

public:
	FObject2D( FScene * lpScene, UI32 iLayer, const FString & sName );
	FObject2D( const FVector2F & vPos, FScene * lpScene, UI32 iLayer, const FString & sName );
	~FObject2D();

	void Draw( const FRect & rViewport );
	void Update( F32 fDTime );

	F32 GetX()const;
	F32 GetY()const;
	UI32 GetWidth()const;
	UI32 GetHeight()const;
	F32 GetScaleWidth()const;
	F32 GetScaleHeight()const;
	UI32 GetCurFrame()const;
	F32 GetCurTime()const;
	bool IsAnimEnd()const;
	PRIM_ID GetID()const;

	void SetCurFrame( UI32 iFrame );
	void SetCurTime( F32 fTime );
	void SetDuration( F32 fDuration );
	void Move( const FVector2F & vDPos );
	void StartAnimation( const FString & sAnimName );//запуск анимации по имени, им€ хранитс€ в атлас файле!!!
};

#endif