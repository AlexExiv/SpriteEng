#ifndef __FGUI_OBJECT_H__
#define __FGUI_OBJECT_H__


#include "..\Base\FObject.h"

class FGuiObject : public FObject
{
protected:
	FGuiObject * lpParent;
	UI32 iFlags;

	FObject * CreateObject( void * lpPlacement, const CHAR_ * lpTypes, va_list lpArgs );
	FGuiObject( FGuiObject * lpParent, UI32 iFlags );
	void SetParent( FGuiObject * lpParent );

public:
	~FGuiObject();

	void SetFlags( UI32 iFlags );

	FGuiObject * GetParent()const;
	bool IsVisible()const;
	bool IsActive()const;

	void Update( F32 fDTime );

	static void InitMeta();
};

#endif