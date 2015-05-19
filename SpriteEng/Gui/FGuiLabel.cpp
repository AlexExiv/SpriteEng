#include "FGuiLabel.h"
#include "..\2D\FText.h"
#include "FGuiController.h"
#include "..\Base\FBaseTypes.h"



IMPLEMENT_OBJ_DERIVERED( FGuiLabel );

FGuiLabel::FGuiLabel( const FVector2F & vPos, const FString & sText, const FString & sFont, const FColor4F & cColor,
					 F32 fSize, FGuiView * lpSuperView ) : FGuiSingleInaccesbleView( lpSuperView ), lpText( NULL )
{
	lpText = (FText *)AllocObject( MFText, RFVector2F( vPos ), RFColor4F( cColor ), RFFloat( fSize ), &sText, 
		&sFont, RFUInteger( 0 ), GetController()->GetGuiScene(), NULL );
	AddPrimitive( lpText );
	CalcLabelSize( vPos );
	CalcPrimitivePos( vPos );
}

FGuiLabel::~FGuiLabel()
{
	if( lpText )
		Delete( lpText );
}

void FGuiLabel::CalcLabelSize( const FVector2F & vPos )
{
	rFrame.fLeft = vPos.x;
	rFrame.fBottom = vPos.y;
	if( lpText )
	{
		rFrame.fRight = vPos.x + lpText->GetTextWidth();
		rFrame.fTop = vPos.y + lpText->GetTextHeight();
	}
	else
	{
		rFrame.fRight = vPos.x;
		rFrame.fTop = vPos.y;
	}
}


void FGuiLabel::SetText( const FString & sText )
{
	lpText->SetText( sText );
	FVector2F vPos( rFrame.fLeft, rFrame.fBottom );
	CalcLabelSize( vPos );
}

void FGuiLabel::SetFont( const FString & sFont )
{
	FVector2F vPos( rFrame.fLeft, rFrame.fBottom );
	FColor4F cColor = lpText->GetColor();
	F32 fSize = lpText->GetFontSize();
	FString sText = lpText->GetText();

	if( lpText )
	{
		RemovePrimitive( lpText );
		Delete( lpText );
	}
	lpText = (FText *)AllocObject( MFText, RFVector2F( vPos ), RFColor4F( cColor ), RFFloat( fSize ), &sText, 
		&sFont, RFUInteger( 0 ), GetController()->GetGuiScene(), NULL );

	CalcLabelSize( vPos );
	CalcPrimitivePos( vPos );
}

void FGuiLabel::SetTextColor( const FColor4F & cColor )
{
	lpText->SetColor( cColor );
}

void FGuiLabel::SetFontSize( F32 fSize )
{
	lpText->SetFontSize( fSize );
	FVector2F vPos( rFrame.fLeft, rFrame.fBottom );
	CalcLabelSize( vPos );
}


FString FGuiLabel::GetText()const
{
	return lpText->GetText();
}

