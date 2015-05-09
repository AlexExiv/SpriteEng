#include "FView.h"
#include "..\Core\FLog.h"
#include "FGLView.h"


static FView * lpView = NULL;

FView::FView( UI32 iWidth, UI32 iHeight ) : iWidth( iWidth ), iHeight( iHeight )
{
}

UI32 FView::GetWidth()const
{
	return iWidth;
}

UI32 FView::GetHeight()const
{
	return iHeight;
}

#ifdef WINDOWS_FAMILY && !defined( MOBILE_DEVICE )
void FView::Init( UI32 iWidth, UI32 iHeight )
{
	lpView = new FGLView( iWidth, iHeight );
}
#endif

FView * FView::GetMainView()
{
	AssertFatal( lpView != NULL, "Try to access not initialized FView object" );
	return lpView;
}
