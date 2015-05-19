#include "FGuiImage.h"
#include "FGuiController.h"
#include "..\2D\FImage2D.h"
#include "..\Core\FString.h"
#include "..\Base\FBaseTypes.h"


IMPLEMENT_OBJ_DERIVERED( FGuiImage );

FGuiImage::FGuiImage( const FVector2F & vPos, const FString & sImageName, FGuiView * lpSuperView ) : FGuiSingleInaccesbleView( lpSuperView )
	, lpImage( NULL )
{
	lpImage = (FImage2D *)AllocObject( MFImage2D, RFVector2F( vPos ), GetController()->GetGuiScene(), RFUInteger( 0 ), &sImageName, NULL );
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
	lpImage = (FImage2D *)AllocObject( MFImage2D, RFVector2F( vPos ), GetController()->GetGuiScene(), RFUInteger( 0 ), &sName, NULL );
	AddPrimitive( lpImage );
	CalcImageSize( vPos );
	CalcPrimitivePos( vPos );
}
