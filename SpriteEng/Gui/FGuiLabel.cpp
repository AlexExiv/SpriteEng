#include "FGuiLabel.h"
#include "..\2D\FText.h"
#include "FGuiController.h"


FGuiLabel::FGuiLabel( const FVector2F & vPos, const FString & sText, const FString & sFont, const FColor4F & cColor,
					 F32 fSize, FGuiView * lpSuperView ) : FGuiSingleInaccesbleView( lpSuperView ), lpText( NULL )
{
	lpText = (FText *)AllocObject( "FText", "\\vector\\color\\f\\string\\string\\ui\\scene", &vPos, &cColor, fSize, &sText, 
		&sFont, 0, GetController()->GetGuiScene() );
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
	lpText = (FText *)AllocObject( "FText", "\\vector\\color\\f\\string\\string\\ui\\scene", &vPos, &cColor, fSize, &sText, 
		&sFont, 0, GetController()->GetGuiScene() );

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

