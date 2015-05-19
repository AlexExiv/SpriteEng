#ifndef __FGUI_CONTROLLER_H__
#define __FGUI_CONTROLLER_H__


#include "..\Base\FObject.h"
#include "..\Core\FList.h"
#include "..\Math\FVector2D.h"

class FGuiView;
class FGuiScene;
class FGuiNavigator;

class FGuiController : public FObject
{
	friend class FGuiNavigator;

	FGuiNavigator * lpNavigator;
	FGuiScene * lpGuiScene;
	void SetNavigator( FGuiNavigator * lpNavigator );

protected:
	typedef FList<FGuiView *, FFreePointer__<FGuiView *>> FGuiViewList;
	typedef FList<FGuiView *, FFreePointer__<FGuiView *>>::Iterator FGuiViewIterator;

	FGuiView * lpMainView;

	virtual void ControllerDidAppear();
	virtual void ControllerDidDisappear();

	FGuiController( FGuiScene * lpScene );
	FGuiController( FGuiController * lpRootController );
	FGuiController( const FGuiController & ) ;
	FGuiController & operator = ( const FGuiController & );

public:
	~FGuiController();

	virtual bool ClickDown( const FVector2F & vPos, UI32 iIndex );
	virtual bool ClickUp( const FVector2F & vPos, UI32 iIndex );
	virtual bool ClickMove( const FVector2F & vPos, UI32 iIndex );
	virtual bool KeyDown();
	virtual bool KeyUp();

	FGuiScene * GetGuiScene()const;
	FGuiNavigator * GetNavigator()const;
	FGuiView * GetView()const;

	void Update( F32 fDTime );

	static void InitMeta();

	DEFINE_OBJ_DERIVERED( FGuiController );
};

DEFINE_OBJ_NAME( FGuiController );

#endif