#ifndef __FGUI_IMAGE_H__
#define __FGUI_IMAGE_H__


#include "FGuiViewType.h"
#include "..\Math\FVector2D.h"


class FImage2D;

class FGuiImage : public FGuiSingleInaccesbleView
{
protected:
	FImage2D * lpImage;
	void CalcImageSize( const FVector2F & vPos );

public:
	FGuiImage( const FVector2F & vPos, const FString & sImageName, FGuiView * lpSuperView );
	~FGuiImage();

	void SetImage( const FString & sName );

	DEFINE_OBJ_DERIVERED( FGuiImage );
};

DEFINE_OBJ_NAME( FGuiImage );

#endif