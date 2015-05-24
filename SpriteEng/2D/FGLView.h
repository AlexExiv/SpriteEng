#ifndef __FGLVIEW_H__
#define __FGLVIEW_H__


#include "FView.h"
#include "libExt.h"


class FGLView : public FView
{
	enum
	{
		VIEW_MAX_STATE = 16
	};
	GLenum eState[FGLView::VIEW_MAX_STATE];
	UI32 iCurState;

	void ResetStates();

#ifdef WINDOWS_FAMILY
	FWndAdd * lpWndAdd;
#endif

public:
	FGLView( UI32 iWidth, UI32 iHeight, void * lpAddData );
	~FGLView();

	void LineWidth( F32 fWidth );
	void SetViewport( UI32 x, UI32 y, UI32 iWidth, UI32 iHeight );
	void ResetViewport() ;
	void SetPrimData( const FDrawDataCont * lpContainers, UI32 iContCount ) ;
	void SetPrimDataArr( const FDrawDataCont * lpContainers, UI32 iContCount, void * lpData ) ;
	void Draw( UI32 iPrimType, UI32 iPrimCount );
	void DrawIndexed( UI32 iPrimType, UI32 iPrimCount, const void * lpIndeces ) ;

	void BeginDraw();
	void EndDraw();
	void SwapBuffers();

	FTexture * CreateTexture( FGraphObject * lpPlacement, const FString & sName );
	FTexture * CreateTexture( FGraphObject * lpPlacement, const FString & sName, const FImageResource * lpImg );
	FTexture * CreateTexture( const FString & sName, UI32 iWidth, UI32 iHeight, UI32 iFormat );
	FShader * CreateShader( FGraphObject * lpPlacement, const FString & sShaderName );

	UI32 GetTextureObjSize()const;
	UI32 GetShaderObjSize()const;
};

#endif