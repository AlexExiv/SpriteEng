#include "FGame.h"
#include "..\Base\FMetaData.h"
#include "FDigit.h"
#include "FHiddenDigit.h"
#include "FAttention.h"
#include "FBonus.h"
#include "FMessage.h"
#include "FParticle.h"
#include "FInterface.h"
#include "FScoreAdd.h"
#include "FArithWorld.h"
#include "..\Gui\FGuiScene.h"
#include "..\2D\FPrimitive.h"
#include <new.h>



FObject * ConstructDigit1( void * lpPlacement, va_list lpArgs )
{
	FArithWorld * lpWorld = va_arg( lpArgs, FArithWorld * );
	return new (lpPlacement) FDigit( lpWorld );
}

FObject * ConstructDigit3( void * lpPlacement, va_list lpArgs )
{
	FVector2F * lpPos = va_arg( lpArgs, FVector2F * );
	UI32 iNum = va_arg( lpArgs, UI32 );
	FArithWorld * lpWorld = va_arg( lpArgs, FArithWorld * );
	return new (lpPlacement) FDigit( *lpPos, iNum, lpWorld );
}

FObject * ConstructHiddenDigit1( void * lpPlacement, va_list lpArgs )
{
	FArithWorld * lpWorld = va_arg( lpArgs, FArithWorld * );
	return new (lpPlacement) FHiddenDigit( lpWorld );
}

FObject * ConstructHiddenDigit3( void * lpPlacement, va_list lpArgs )
{
	FVector2F * lpPos = va_arg( lpArgs, FVector2F * );
	UI32 iNum = va_arg( lpArgs, UI32 );
	FArithWorld * lpWorld = va_arg( lpArgs, FArithWorld * );
	return new (lpPlacement) FHiddenDigit( *lpPos, iNum, lpWorld );
}

FObject * ConstructAttention( void * lpPlacement, va_list lpArgs )
{
	FVector2F * lpPos = va_arg( lpArgs, FVector2F * );
	FArithWorld * lpWorld = va_arg( lpArgs, FArithWorld * );
	return new (lpPlacement) FAttention( *lpPos, lpWorld );
}

FObject * ConstructBonus1( void * lpPlacement, va_list lpArgs )
{
	FArithWorld * lpWorld = va_arg( lpArgs, FArithWorld * );
	return new (lpPlacement) FBonus( lpWorld );
}

FObject * ConstructBonus3( void * lpPlacement, va_list lpArgs )
{
	FVector2F * lpPos = va_arg( lpArgs, FVector2F * );
	UI32 iBonusType = va_arg( lpArgs, UI32 );
	FArithWorld * lpWorld = va_arg( lpArgs, FArithWorld * );
	return new (lpPlacement) FBonus( *lpPos, iBonusType, lpWorld );
}

FObject * ConstructInterface( void * lpPlacement, va_list lpArgs )
{
	FGuiScene * lpScene = va_arg( lpArgs, FGuiScene * );
	return new (lpPlacement) FInterface( lpScene );
}

FObject * ConstructArithWorld( void * lpPlacement, va_list lpArgs )
{
	FArithGame * lpGame = va_arg( lpArgs, FArithGame * );
	return new (lpPlacement) FArithWorld( lpGame );
}

FObject * ConstructMessage( void * lpPlacement, va_list lpArgs )
{
	FArithWorld * lpWorld = va_arg( lpArgs, FArithWorld * );
	return new (lpPlacement) FMessage( lpWorld );
}

FObject * ConstructParticle1( void * lpPlacement, va_list lpArgs )
{
	FArithWorld * lpWorld = va_arg( lpArgs, FArithWorld * );
	return new (lpPlacement) FParticle( lpWorld );
}

FObject * ConstructParticle2( void * lpPlacement, va_list lpArgs )
{
	FVector2F * lpPos = va_arg( lpArgs, FVector2F * );
	FArithWorld * lpWorld = va_arg( lpArgs, FArithWorld * );
	return new (lpPlacement) FParticle( *lpPos, lpWorld );
}

FObject * ConstructScoreAdd1( void * lpPlacement, va_list lpArgs )
{
	FArithWorld * lpWorld = va_arg( lpArgs, FArithWorld * );
	return new (lpPlacement) FScoreAdd( lpWorld );
}

FObject * ConstructScoreAdd3( void * lpPlacement, va_list lpArgs )
{
	FVector2F * lpPos = va_arg( lpArgs, FVector2F * );
	FArithWorld * lpWorld = va_arg( lpArgs, FArithWorld * );
	I32 iNum = va_arg( lpArgs, I32 );
	return new (lpPlacement) FScoreAdd( *lpPos, lpWorld, iNum );
}

void FGame::InitMeta()
{
	FPrimitive::InitMetaData();
	FGuiController::InitMeta();

	DEFINE_META( FGameObject );
	DEFINE_META_SUPER( FDigit, FGameObject );
	DEFINE_META_SUPER( FHiddenDigit, FGameObject );
	DEFINE_META_SUPER( FAttention, FGameObject );
	DEFINE_META_SUPER( FBonus, FGameObject );
	DEFINE_META_SUPER( FMessage, FGameObject );
	DEFINE_META_SUPER( FParticle, FGameObject );
	DEFINE_META_SUPER( FInterface, FGameObject );
	DEFINE_META_SUPER( FScoreAdd, FGameObject );
	DEFINE_META_SUPER( FArithWorld, FGameObject );

	DEFINE_META_CONSTR( FDigit, "\\world", ConstructDigit1 );
	DEFINE_META_CONSTR( FDigit, "\\vector\\ui\\world", ConstructDigit3 );
	DEFINE_META_CONSTR( FHiddenDigit, "\\world", ConstructHiddenDigit1 );
	DEFINE_META_CONSTR( FHiddenDigit, "\\vector\\ui\\world", ConstructHiddenDigit3 );
	DEFINE_META_CONSTR( FAttention, "\\vector\\world", ConstructAttention );
	DEFINE_META_CONSTR( FBonus, "\\world", ConstructBonus1 );
	DEFINE_META_CONSTR( FBonus, "\\vector\\ui\\world", ConstructBonus3 );
	DEFINE_META_CONSTR( FMessage, "\\world", ConstructMessage );
	DEFINE_META_CONSTR( FParticle, "\\world", ConstructParticle1 );
	DEFINE_META_CONSTR( FParticle, "\\vector\\world", ConstructParticle2 );
	DEFINE_META_CONSTR( FInterface, "\\scene", ConstructInterface );
	DEFINE_META_CONSTR( FScoreAdd, "\\world", ConstructScoreAdd1 );
	DEFINE_META_CONSTR( FScoreAdd, "\\vector\\world\\i", ConstructScoreAdd3 );
	DEFINE_META_CONSTR( FArithWorld, "\\game", ConstructArithWorld );
}

FGame::FGame( UI32 iObjReserved ) : FObject( iObjReserved )
{

}

FGame::~FGame()
{
}


void FGame::ClickDown( const FVector2F & vPos, UI32 iIndex )
{
}

void FGame::ClickUp( const FVector2F & vPos, UI32 iIndex )
{
}

void FGame::ClickMove( const FVector2F & vPos, UI32 iIndex )
{
}

void FGame::KeyDown()
{
}

void FGame::KeyUp()
{
}


void FGame::Update( F32 fDTime )
{
}

void FGame::Draw()
{
}


