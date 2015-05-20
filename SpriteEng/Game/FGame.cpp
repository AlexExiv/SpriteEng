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
#include "..\Base\FBaseTypes.h"
#include "FArithGame.h"



IMPLEMENT_OBJ_DERIVERED( FGame );

FObject * ConstructDigit1( void * lpPlacement, va_list lpArgs )
{
	FArithWorld * lpWorld = va_arg( lpArgs, FArithWorld * );
	return new (lpPlacement) FDigit( lpWorld );
}

FObject * ConstructDigit3( void * lpPlacement, va_list lpArgs )
{
	PFVector2F lpPos = va_arg( lpArgs, PFVector2F );
	PFUInteger lpNum = va_arg( lpArgs, PFUInteger );
	FArithWorld * lpWorld = va_arg( lpArgs, FArithWorld * );
	return new (lpPlacement) FDigit( *lpPos, *lpNum, lpWorld );
}

FObject * ConstructHiddenDigit1( void * lpPlacement, va_list lpArgs )
{
	FArithWorld * lpWorld = va_arg( lpArgs, FArithWorld * );
	return new (lpPlacement) FHiddenDigit( lpWorld );
}

FObject * ConstructHiddenDigit3( void * lpPlacement, va_list lpArgs )
{
	PFVector2F lpPos = va_arg( lpArgs, PFVector2F );
	PFUInteger lpNum = va_arg( lpArgs, PFUInteger );
	FArithWorld * lpWorld = va_arg( lpArgs, FArithWorld * );
	return new (lpPlacement) FHiddenDigit( *lpPos, *lpNum, lpWorld );
}

FObject * ConstructAttention( void * lpPlacement, va_list lpArgs )
{
	PFVector2F lpPos = va_arg( lpArgs, PFVector2F );
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
	PFVector2F lpPos = va_arg( lpArgs, PFVector2F );
	PFUInteger lpBonusType = va_arg( lpArgs, PFUInteger );
	FArithWorld * lpWorld = va_arg( lpArgs, FArithWorld * );
	return new (lpPlacement) FBonus( *lpPos, *lpBonusType, lpWorld );
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
	PFVector2F lpPos = va_arg( lpArgs, PFVector2F );
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
	PFVector2F lpPos = va_arg( lpArgs, PFVector2F );
	FArithWorld * lpWorld = va_arg( lpArgs, FArithWorld * );
	PFInteger lpNum = va_arg( lpArgs, PFInteger );
	return new (lpPlacement) FScoreAdd( *lpPos, lpWorld, *lpNum );
}

void FGame::InitMeta()
{
	FObject::InitMeta();
	FPrimitive::InitMetaData();
	FGuiController::InitMeta();

	DEFINE_META_SUPER( FGameObject, FObject );
	DEFINE_META_SUPER( FDigit, FGameObject );
	DEFINE_META_SUPER( FHiddenDigit, FGameObject );
	DEFINE_META_SUPER( FAttention, FGameObject );
	DEFINE_META_SUPER( FBonus, FGameObject );
	DEFINE_META_SUPER( FMessage, FGameObject );
	DEFINE_META_SUPER( FParticle, FGameObject );
	DEFINE_META_SUPER( FInterface, FGuiController );
	DEFINE_META_SUPER( FScoreAdd, FGameObject );
	DEFINE_META_SUPER( FWorld, FGameObject );
	DEFINE_META_SUPER( FGame, FGameObject );
	DEFINE_META_SUPER( FArithWorld, FGameObject );
	DEFINE_META_SUPER( FArithGame, FGameObject );

	DEFINE_META_CONSTR1( FDigit, ConstructDigit1, FArithWorld );
	DEFINE_META_CONSTR3( FDigit, ConstructDigit3, FVector2F_, FUInteger, FArithWorld );
	DEFINE_META_CONSTR1( FHiddenDigit, ConstructHiddenDigit1, FArithWorld );
	DEFINE_META_CONSTR3( FHiddenDigit, ConstructHiddenDigit3, FVector2F_, FUInteger, FArithWorld );
	DEFINE_META_CONSTR2( FAttention, ConstructAttention, FVector2F_, FArithWorld );
	DEFINE_META_CONSTR1( FBonus, ConstructBonus1, FArithWorld );
	DEFINE_META_CONSTR3( FBonus, ConstructBonus3, FVector2F_, FUInteger, FArithWorld );
	DEFINE_META_CONSTR1( FMessage, ConstructMessage, FArithWorld );
	DEFINE_META_CONSTR1( FParticle, ConstructParticle1, FArithWorld );
	DEFINE_META_CONSTR2( FParticle, ConstructParticle2, FVector2F_, FArithWorld );
	DEFINE_META_CONSTR1( FInterface, ConstructInterface, FGuiScene );
	DEFINE_META_CONSTR1( FScoreAdd, ConstructScoreAdd1, FArithWorld );
	DEFINE_META_CONSTR3( FScoreAdd, ConstructScoreAdd3, FVector2F_, FArithWorld, FInteger );
	DEFINE_META_CONSTR1( FArithWorld, ConstructArithWorld, FArithGame );
}

FGame::FGame( UI32 iObjReserved ) : FGameObject( iObjReserved )
{

}

FGame::~FGame()
{
}


void FGame::ClickDown( const FVector2F & vPos, UI32 iIndex, UI32 iFlags )
{
}

void FGame::ClickUp( const FVector2F & vPos, UI32 iIndex, UI32 iFlags )
{
}

void FGame::ClickMove( const FVector2F & vPos, UI32 iIndex, UI32 iFlags )
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


