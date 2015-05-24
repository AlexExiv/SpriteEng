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

void FView::Init( UI32 iAPI, UI32 iWidth, UI32 iHeight, void * lpAddData )
{
	switch( iAPI )
	{
	case F_API_OPENGL:
		lpView = new FGLView( iWidth, iHeight, lpAddData );
		break;
	case F_API_DIRECT3D:
		break;
	}
}

FView * FView::GetMainView()
{
	AssertFatal( lpView != NULL, "Try to access not initialized FView object" );
	return lpView;
}
