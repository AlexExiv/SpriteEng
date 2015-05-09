#include "FGuiImage.h"
#include "FGuiController.h"
#include "..\2D\FImage2D.h"



FGuiImage::FGuiImage( const FVector2F & vPos, const FString & sImageName, FGuiView * lpSuperView ) : FGuiSingleInaccesbleView( lpSuperView )
	, lpImage( NULL )
{
	lpImage = (FImage2D *)AllocObject( "FImage2D", "\\vector\\scene\\ui\\string", &vPos, GetController()->GetGuiScene(), 0, &sImageName );
	AddPrimitive( lpImage );
	CalcImageSize( vPos );
	CalcPrimitivePos( vPos );
}

FGuiImage::~FGuiImage()
{
	if( lpImage )
		Delete( lpImage );
}

void FGuiImage::CalcImageSize( const FVector2F & vPos )
{
	rFrame.fLeft = vPos.x;
	rFrame.fBottom = vPos.y;
	if( lpImage )
	{
		rFrame.fRight = vPos.x + lpImage->GetWidth();
		rFrame.fTop = vPos.y + lpImage->GetHeight();
	}
	else
	{
		rFrame.fRight = vPos.x;
		rFrame.fTop = vPos.y;
	}
}

void FGuiImage::SetImage( const FString & sName )
{
	if( lpImage )
	{
		RemovePrimitive( lpImage );
		Delete( lpImage );
	}

	FVector2F vPos( rFrame.fLeft, rFrame.fBottom );
	lpImage = (FImage2D *)AllocObject( "FImage2D", "\\vector\\scene\\ui\\string", &vPos, GetController()->GetGuiScene(), 0, &sName );
	AddPrimitive( lpImage );
	CalcImageSize( vPos );
	CalcPrimitivePos( vPos );
}
