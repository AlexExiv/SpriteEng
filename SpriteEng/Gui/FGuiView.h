#ifndef __FGUI_VIEW_H__
#define __FGUI_VIEW_H__



#include "..\Base\FObject.h"
#include "..\Math\FVector2D.h"
#include "..\Core\FList.h"


class FPrimitive;
class FGuiController;

class FGuiView : public FObject
{
public:
	enum
	{
		GUI_VIEW_VISIBLE = 0x00000001l,
		GUI_VIEW_ENABLE = 0x00000002l,
		GUI_VIEW_ANIMATED = 0x00000004l
	};

protected:
	typedef FList<FGuiView *, FFreePointer__<FGuiView *>> FGuiViewList;
	typedef FList<FGuiView *, FFreePointer__<FGuiView *>>::Iterator FGuiViewIterator;
	typedef FList<FPrimitive *, FFreePointer__<FPrimitive *>> FPrimitiveList;
	typedef FList<FPrimitive *, FFreePointer__<FPrimitive *>>::Iterator FPrimitiveIterator;

private:
	FPrimitiveList lPrimitiveList;
	FGuiController * lpController;


protected:
	FRect rFrame;
	UI32 iFlags;
	FGuiViewList lGuiViewList, lGuiAnimList;
	FGuiView * lpSuperView;

	FVector2F GetPosFromSuperView( const FVector2F & vPos )const;
	void SetSuperView( FGuiView * lpSuperView );

	void CalcPrimitivePos( const FVector2F & vOldPos );
	void AddPrimitive( FPrimitive * lpPrim );
	void RemovePrimitive( FPrimitive * lpPrim );

	FGuiView( FGuiView * lpSuperView );
	FGuiView( const FGuiView & ) ;
	FGuiView & operator = ( const FGuiView & );

public:
	FGuiView( const FRect & rFrame, FGuiController * lpController );
	~FGuiView();

	FVector2F GetPosFromSuperView()const;
	FVector2F GetPos()const;
	FVector2F GetSize()const;
	FRect GetFrame()const;
	FGuiView * GetSuperView()const;
	FGuiController * GetController()const;
	inline bool IsVisible()const
	{
		return iFlags & GUI_VIEW_VISIBLE;
	}
	inline bool IsEnable()const
	{
		return iFlags & GUI_VIEW_ENABLE;
	}
	inline bool IsAnimated()const
	{
		return iFlags & GUI_VIEW_ANIMATED;
	}

	void SetFrame( const FRect & rFrame );
	void SetPos( const FVector2F & vPos );
	void SetSize( const FVector2F & vSize );
	void Visible( bool bVisible );
	void Enable( bool bEnable );

	virtual void AddView( FGuiView * lpView );
	virtual void InsertView( FGuiView * lpView, UI32 iIndex );
	virtual void RemoveView( FGuiView * lpView );
	virtual void AddAnimatedView( FGuiView * lpView );
	virtual void RemoveAnimatedView( FGuiView * lpView );
	void UpToFrontView();

	void Update( F32 fDTime );

	virtual bool ClickDown( const FVector2F & vPos, UI32 iIndex );
	virtual bool ClickUp( const FVector2F & vPos, UI32 iIndex );
	virtual bool ClickMove( const FVector2F & vPos, UI32 iIndex );
	virtual bool KeyDown();
	virtual bool KeyUp();

	DEFINE_OBJ_DERIVERED( FGuiView );
};

DEFINE_OBJ_NAME( FGuiView );

#endif