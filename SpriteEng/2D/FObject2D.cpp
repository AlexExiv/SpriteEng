#include "FObject2D.h"
#include "FAnimation2D.h"
#include "FGraphObject.h"
#include "FScene.h"
#include "FView.h"
#include <new.h>


IMPLEMENT_OBJ_DERIVERED( FObject2D );

FObject2D::FObject2D( FScene * lpScene, UI32 iLayer, const FString & sName ) : FPrimitive( iLayer, lpScene ), iStartFrame( 0 ), iEndFrame( 0 ), iCurFrame( 0 ), fFrameTime( 0.f ), fCurTime( 0.f ), bLooped( false )
{
	SetAnimated( true );
	lpAnimation = (FAnimation2D *)FGraphObjectManager::GetInstance()->CreateObject( sName, FGraphObjectManager::OBJECT_ANIMATION );
}

FObject2D::FObject2D( const FVector2F & vPos, FScene * lpScene, UI32 iLayer, const FString & sName ) : FPrimitive( iLayer, lpScene ), vPos( vPos ), iStartFrame( 0 ), iEndFrame( 0 ), 
																									iCurFrame( 0 ), fFrameTime( 0.f ), fCurTime( 0.f ), bLooped( false )
{
	SetAnimated( true );
	lpAnimation = (FAnimation2D *)FGraphObjectManager::GetInstance()->CreateObject( sName, FGraphObjectManager::OBJECT_ANIMATION );
}

FObject2D::~FObject2D()
{
	if( lpAnimation )
		FGraphObjectManager::GetInstance()->ReleaseObject( lpAnimation );
}


void FObject2D::DrawCallback( UI32 iPrimCount, void * lpData, UI32 * lpIndeces )
{
	if( !lpAnimation )
		return;

	lpAnimation->Bind();
	FView::GetMainView()->SetPrimDataArr( lpAnimation->GetDrawDataCont(), lpAnimation->GetDrawContCount(), lpData );
	FView::GetMainView()->DrawIndexed( FView::PRIM_TRIANGLES, iPrimCount*6, lpIndeces );
	lpAnimation->UnBind();
}

UI32 FObject2D::AddDataToSeq( void * lpData, UI32 * lpIndeces, UI32 iStartVert )
{
	if( !lpAnimation )
		return 0;

	//FVector2F vPos_ = vPos + 0.5f*(1.f - fScale)*FVector2F( lpAnimation->GetWidth( iCurFrame ), lpAnimation->GetHeight( iCurFrame ) );
	lpAnimation->Draw( vPos, fScale, iCurFrame, fAlpha, lpData, lpIndeces, iStartVert );
	return 1;
}

UI32 FObject2D::GetDataSizePerInst()const
{
	if( !lpAnimation )
		return 0;

	return lpAnimation->GetAnimDataSize();
}

UI32 FObject2D::GetIndexCount()const
{
	return 6;
}

UI32 FObject2D::GetVertexCount()const
{
	return 4;
}

UI32 FObject2D::DrawType()const
{
	return FPrimitive::DRAW_INDEXED;
}

void FObject2D::Draw( const FRect & rViewport )
{
	lpScene->AddToDrawSeq( this );
}

void FObject2D::Update( F32 fDTime )
{
	fCurTime += fDTime;
	if( fCurTime >= fFrameTime )
	{
		iCurFrame++;
		if( iCurFrame > iEndFrame )
		{
			if( bLooped )
				iCurFrame = iStartFrame;
			else
				iCurFrame = iEndFrame;
		}
	}
}


F32 FObject2D::GetX()const
{
	return vPos.x;
}

F32 FObject2D::GetY()const
{
	return vPos.y;
}

UI32 FObject2D::GetWidth()const
{
	if( !lpAnimation )
		return 0;

	return lpAnimation->GetWidth( iCurFrame );
}

UI32 FObject2D::GetHeight()const
{
	if( !lpAnimation )
		return 0;

	return lpAnimation->GetHeight( iCurFrame );
}

F32 FObject2D::GetScaleWidth()const
{
	if( !lpAnimation )
		return 0;

	return F32( lpAnimation->GetWidth( iCurFrame ) )*fScale;
}

F32 FObject2D::GetScaleHeight()const
{
	if( !lpAnimation )
		return 0;

	return F32( lpAnimation->GetHeight( iCurFrame ) )*fScale;
}


UI32 FObject2D::GetCurFrame()const
{
	return iCurFrame;
}

F32 FObject2D::GetCurTime()const
{
	return fCurTime;
}

void FObject2D::SetCurTime( F32 fTime )
{
	fCurTime = fTime;
}

void FObject2D::SetDuration( F32 fDuration )
{
	fFrameTime = fDuration;
}


void FObject2D::SetCurFrame( UI32 iFrame )
{
	iCurFrame = iFrame;
}

bool FObject2D::IsAnimEnd()const
{
	return iCurFrame == iEndFrame;
}

PRIM_ID FObject2D::GetID()const
{
	return PRIM_ID( lpAnimation );
}

void FObject2D::Move( const FVector2F & vDPos )
{
	vPos += vDPos;
}

void FObject2D::SetPos( const FVector2F & vPos_ )
{
	vPos = vPos_;
}


void FObject2D::StartAnimation( const FString & sAnimName )
{
	if( !lpAnimation )
		return ;

	FAnimation2D::FAnimation aAnim;
	lpAnimation->GetAnimInf( sAnimName, &aAnim );
	iStartFrame = aAnim.iStartFrame;
	iEndFrame = aAnim.iEndFrame;
	bLooped = aAnim.bLooped;
}

//FObject * FObject2D::CreateObject( void * lpPlacement, const CHAR_ * lpTypes, va_list lpArgs )
//{
//	CHECK_OBJ_TYPES( lpTypes, lpContrTypes );
//
//	FVector2F * lpPos = va_arg( lpArgs, FVector2F * );
//	FScene * lpScene = va_arg( lpArgs, FScene * );
//	UI32 iLayer = va_arg( lpArgs, UI32 );
//	FString * lpName = va_arg( lpArgs, FString * );
//
//	return new (lpPlacement) FObject2D( *lpPos, lpScene, iLayer, *lpName );
//}