#include "FGuiView.h"
#include "..\2D\FPrimitive.h"
#include "..\Core\FLog.h"


FGuiView::FGuiView( FGuiView * lpSuperView_ ) : iFlags( GUI_VIEW_ENABLE | GUI_VIEW_VISIBLE ), lGuiViewList( 32, 0 ), 
	lGuiAnimList( 16, 0 ), lpController( lpSuperView_->GetController() ), lpSuperView( NULL )
{
	lpSuperView_->AddView( this );
}

FGuiView::FGuiView( const FRect & rFrame, FGuiController * lpController ) : rFrame( rFrame ), iFlags( GUI_VIEW_ENABLE | GUI_VIEW_VISIBLE ), lGuiViewList( 32, 0 ), 
	lGuiAnimList( 16, 0 ), lpSuperView( NULL ), lpController( lpController )
{
}

FGuiView::~FGuiView()
{
	if( lpSuperView )
		lpSuperView->RemoveView( this );
}


FVector2F FGuiView::GetPosFromSuperView( const FVector2F & vPos )const
{
	FVector2F vStartPos( rFrame.fLeft, rFrame.fBottom );
	if( lpSuperView )
		vStartPos = lpSuperView->GetPosFromSuperView( vStartPos );
	return vStartPos + vPos;
}

void FGuiView::SetSuperView( FGuiView * lpSuperView_ )
{
	lpSuperView = lpSuperView_;
}

void FGuiView::CalcPrimitivePos( const FVector2F & vOldPos )
{
	FGuiViewIterator iIt = lGuiViewList.Begin();
	for(;iIt != lGuiViewList.End();iIt++ )
		iIt->CalcPrimitivePos( vOldPos );

	FVector2F vDPos = FVector2F( rFrame.fLeft, rFrame.fBottom ) - vOldPos;
	FPrimitiveIterator iItPrim = lPrimitiveList.Begin();
	for(;iItPrim != lPrimitiveList.End();iItPrim++)
		iItPrim->Move( vDPos );
}

void FGuiView::AddPrimitive( FPrimitive * lpPrim )
{
	if( lpPrim )
		lPrimitiveList.PushBack( lpPrim );
}

void FGuiView::RemovePrimitive( FPrimitive * lpPrim )
{
	FPrimitiveIterator iIt = lPrimitiveList.Begin();
	for(;iIt != lPrimitiveList.End();iIt++)
	{
		if( iIt == lpPrim )
			break;
	}

	if( iIt == lPrimitiveList.End() )
	{
		FLog::PutError( "Try to remove unknown view object from FGuiView" );
		return;
	}
	lPrimitiveList.Erase( iIt );
}

FVector2F FGuiView::GetPosFromSuperView()const
{
	FVector2F vStartPos( rFrame.fLeft, rFrame.fBottom );
	if( lpSuperView )
		vStartPos = lpSuperView->GetPosFromSuperView( vStartPos );
	return vStartPos;
}

FVector2F FGuiView::GetPos()const
{
	return FVector2F( rFrame.fLeft, rFrame.fBottom );
}

FVector2F FGuiView::GetSize()const
{
	return FVector2F( rFrame.fRight - rFrame.fLeft, rFrame.fTop - rFrame.fBottom );
}

FGuiView * FGuiView::GetSuperView()const
{
	return lpSuperView;
}

FGuiController * FGuiView::GetController()const
{
	return lpController;
}


void FGuiView::SetFrame( const FRect & rFrame_ )
{
	FVector2F vOldPos( rFrame.fLeft, rFrame.fBottom );
	rFrame = rFrame_;
	CalcPrimitivePos( vOldPos );
}

void FGuiView::SetPos( const FVector2F & vPos )
{
	FVector2F vOldPos( rFrame.fLeft, rFrame.fBottom );
	rFrame.fLeft = vPos.x;
	rFrame.fBottom = vPos.y;
	CalcPrimitivePos( vOldPos );
}

void FGuiView::SetSize( const FVector2F & vSize )
{
	rFrame.fRight = rFrame.fLeft + vSize.x;
	rFrame.fTop = rFrame.fBottom + vSize.y;
}

void FGuiView::Visible( bool bVisible )
{
	if( bVisible )
		iFlags |= GUI_VIEW_VISIBLE;
	else
		iFlags &= (~GUI_VIEW_VISIBLE);

	FGuiViewIterator iIt = lGuiViewList.Begin();
	for(;iIt != lGuiViewList.End();iIt++)
		iIt->Visible( bVisible );

	FPrimitiveIterator iItPrim = lPrimitiveList.Begin();
	for(;iItPrim != lPrimitiveList.End();iItPrim++)
		iItPrim->Show( bVisible );
}

void FGuiView::Enable( bool bEnable )
{
	if( bEnable )
		iFlags |= GUI_VIEW_ENABLE;
	else
		iFlags &= (~GUI_VIEW_ENABLE);
}


void FGuiView::AddView( FGuiView * lpView )
{
	if( !lpView )
		return;

	if( lpView->lpSuperView )
	{
		FLog::PutError( "Try to add subview who is already has super view" );
		return;
	}

	FGuiViewIterator iIt = lGuiViewList.Begin();
	for(;iIt != lGuiViewList.End();iIt++)
	{
		if( iIt == lpView )
		{
			FLog::PutError( "Added view is already exist in view list" );
			return;
		}
	}
	lGuiViewList.PushBack( lpView );
	lpView->SetSuperView( this );
}

void FGuiView::InsertView( FGuiView * lpView, UI32 iIndex )
{
	if( !lpView )
		return;

	if( lpView->lpSuperView )
	{
		FLog::PutError( "Try to add subview who is already has super view" );
		return;
	}

	FGuiViewIterator iIt = lGuiViewList.Begin();
	for(;iIt != lGuiViewList.End();iIt++)
	{
		if( iIt == lpView )
		{
			FLog::PutError( "Added view is already exist in view list" );
			return;
		}
	}

	if( iIndex >= lGuiViewList.GetCount() )
		return;
	iIt = lGuiViewList.Begin();
	for(UI32 i = 0;(i < iIndex)&&(iIt != lGuiViewList.End());iIt++, i++);
	lGuiViewList.Insert( lpView, iIt );
	if( lpView->IsAnimated() )
		AddAnimatedView( lpView );
	lpView->SetSuperView( this );
}

void FGuiView::RemoveView( FGuiView * lpView )
{
	FGuiViewIterator iIt = lGuiViewList.Begin();
	for(;iIt != lGuiViewList.End();iIt++)
	{
		if( iIt == lpView )
			break;
	}

	if( iIt == lGuiViewList.End() )
	{
		FLog::PutError( "Try to remove unknown view object from FGuiView" );
		return;
	}
	lGuiViewList.Erase( iIt );
	if( lpView->IsAnimated() )
		RemoveAnimatedView( lpView );
	lpView->SetSuperView( NULL );
}

void FGuiView::AddAnimatedView( FGuiView * lpView )
{
	if( !lpView )
		return;

	FGuiViewIterator iIt = lGuiAnimList.Begin();
	for(;iIt != lGuiAnimList.End();iIt++)
	{
		if( iIt == lpView )
		{
			FLog::PutError( "Added view is already exist in animated view list" );
			return;
		}
	}
	lGuiAnimList.PushBack( lpView );
}

void FGuiView::RemoveAnimatedView( FGuiView * lpView )
{
	FGuiViewIterator iIt = lGuiAnimList.Begin();
	for(;iIt != lGuiAnimList.End();iIt++)
	{
		if( *iIt == lpView )
			break;
	}

	if( iIt == lGuiAnimList.End() )
	{
		FLog::PutError( "Try to remove unknown animated view object from FGuiView" );
		return;
	}
	lGuiAnimList.Erase( iIt );
}

void FGuiView::UpToFrontView()
{
	FPrimitiveIterator iPrimIt = lPrimitiveList.Begin();
	for(;iPrimIt != lPrimitiveList.End();iPrimIt++ )
		iPrimIt->UpToFrontView();

	FGuiViewIterator iIt = lGuiViewList.Begin();
	for(;iIt != lGuiViewList.Begin();iIt++)
		iIt->UpToFrontView();
}



void FGuiView::Update( F32 fDTime )
{
	FGuiViewIterator iIt = lGuiAnimList.Begin();
	for(;iIt != lGuiAnimList.End();iIt++)
		iIt->Update( fDTime );
}


bool FGuiView::ClickDown( const FVector2F & vPos, UI32 iIndex )
{
	FVector2F vLocalPos( vPos.x - rFrame.fLeft, vPos.y - rFrame.fBottom );//переводим в локальную систему координат
	FGuiViewIterator iIt = lGuiViewList.Begin();
	for(;iIt != lGuiViewList.End();iIt++)
	{
		if( iIt->ClickDown( vLocalPos, iIndex ) )
			return true;
	}
	return false;
}

bool FGuiView::ClickUp( const FVector2F & vPos, UI32 iIndex )
{
	FVector2F vLocalPos( vPos.x - rFrame.fLeft, vPos.y - rFrame.fBottom );//переводим в локальную систему координат
	FGuiViewIterator iIt = lGuiViewList.Begin();
	for(;iIt != lGuiViewList.End();iIt++)
	{
		if( iIt->ClickUp( vLocalPos, iIndex ) )
			return true;
	}
	return false;
}

bool FGuiView::ClickMove( const FVector2F & vPos, UI32 iIndex )
{
	FVector2F vLocalPos( vPos.x - rFrame.fLeft, vPos.y - rFrame.fBottom );//переводим в локальную систему координат
	FGuiViewIterator iIt = lGuiViewList.Begin();
	for(;iIt != lGuiViewList.End();iIt++)
	{
		if( iIt->ClickMove( vLocalPos, iIndex ) )
			return true;
	}
	return false;
}

bool FGuiView::KeyDown()
{
	return false;
}

bool FGuiView::KeyUp()
{
	return false;
}

