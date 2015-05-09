#ifndef __FGUI_VIEW_TYPE_H__
#define __FGUI_VIEW_TYPE_H__


#include "FGuiView.h"

class FGuiSingleView : public FGuiView
{
protected:
	FGuiSingleView( FGuiView * lpSuperView ) : FGuiView( lpSuperView )
	{
	}

	void AddView( FGuiView * lpView ) final{}
	void InsertView( FGuiView * lpView, UI32 iIndex ) final{}
	void RemoveView( FGuiView * lpView ) final{}
	void AddAnimatedView( FGuiView * lpView ) final{}
	void RemoveAnimatedView( FGuiView * lpView ) final{}

public:
	~FGuiSingleView(){}

};

class FGuiSingleInaccesbleView : public FGuiView
{
protected:
	FGuiSingleInaccesbleView( FGuiView * lpSuperView ) : FGuiView( lpSuperView )
	{
	}

	void AddView( FGuiView * lpView ) final{}
	void InsertView( FGuiView * lpView, UI32 iIndex ) final{}
	void RemoveView( FGuiView * lpView ) final{}
	void AddAnimatedView( FGuiView * lpView ) final{}
	void RemoveAnimatedView( FGuiView * lpView ) final{}

	bool ClickDown( const FVector2F & vPos, UI32 iIndex ) final{ return false; }
	bool ClickUp( const FVector2F & vPos, UI32 iIndex ) final{ return false; }
	bool ClickMove( const FVector2F & vPos, UI32 iIndex ) final{ return false; }
	bool KeyDown() final{ return false; }
	bool KeyUp() final{ return false; }

public:
	~FGuiSingleInaccesbleView(){}
};

#endif