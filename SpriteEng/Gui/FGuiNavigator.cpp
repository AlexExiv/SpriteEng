#include "FGuiNavigator.h"
#include "..\Core\FLog.h"


IMPLEMENT_OBJ_DERIVERED( FGuiNavigator );

FGuiNavigator::FGuiNavigator() : FGuiController( (FGuiScene *)NULL ), lpHeadCntrl( NULL ), lGuiCntrlStack( 32, 0 )
{
}

FGuiNavigator::~FGuiNavigator()
{
}


bool FGuiNavigator::ClickDown( const FVector2F & vPos, UI32 iIndex )
{
	if( lpHeadCntrl )
		return lpHeadCntrl->ClickDown( vPos, iIndex );
	return false;
}

bool FGuiNavigator::ClickUp( const FVector2F & vPos, UI32 iIndex )
{
	if( lpHeadCntrl )
		return lpHeadCntrl->ClickUp( vPos, iIndex );
	return false;
}

bool FGuiNavigator::ClickMove( const FVector2F & vPos, UI32 iIndex )
{
	if( lpHeadCntrl )
		return lpHeadCntrl->ClickMove( vPos, iIndex );
	return false;
}

bool FGuiNavigator::KeyDown()
{
	return false;
}

bool FGuiNavigator::KeyUp()
{
	return false;
}


void FGuiNavigator::PushController( FGuiController * lpController )
{
	lGuiCntrlStack.PushFront( lpController );
	lpHeadCntrl = lpController;
	lpController->SetNavigator( this );
}

void FGuiNavigator::PopController()
{
	if( lGuiCntrlStack.GetCount() == 0 )
		return;

	FGuiControllerStackIterator iIt = lGuiCntrlStack.Begin();
	iIt->SetNavigator( NULL );
	lGuiCntrlStack.Erase( iIt );
	iIt = lGuiCntrlStack.Begin();
	if( iIt == lGuiCntrlStack.End() )
		lpHeadCntrl = NULL;
	else
		lpHeadCntrl = *iIt;
}

void FGuiNavigator::Update( F32 fDTime )
{
	if( lpHeadCntrl )
		lpHeadCntrl->Update( fDTime );
}
