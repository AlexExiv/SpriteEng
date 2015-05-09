#ifndef __FGUI_NAVIGATOR_H__
#define __FGUI_NAVIGATOR_H__


#include "FGuiController.h"

class FGuiNavigator : public FGuiController
{
	friend class FGuiController;

protected:
	typedef FList<FGuiController *, FFreePointer__<FGuiController *>> FGuiControllerStackList;
	typedef FList<FGuiController *, FFreePointer__<FGuiController *>>::Iterator FGuiControllerStackIterator;

	FGuiControllerStackList lGuiCntrlStack;
	FGuiController * lpHeadCntrl;

public:
	FGuiNavigator();
	~FGuiNavigator();

	bool ClickDown( const FVector2F & vPos, UI32 iIndex );
	bool ClickUp( const FVector2F & vPos, UI32 iIndex );
	bool ClickMove( const FVector2F & vPos, UI32 iIndex );
	bool KeyDown();
	bool KeyUp();

	void PushController( FGuiController * lpController );
	void PopController();
	void Update( F32 fDTime );
};


#endif