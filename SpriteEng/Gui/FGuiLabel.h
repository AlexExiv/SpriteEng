#ifndef __FGUI_LABEL_H__
#define __FGUI_LABEL_H__


#include "FGuiViewType.h"
#include "..\Core\FString.h"
#include "..\Math\FVector2D.h"
#include "..\Core\FColor.h"

class FText;

class FGuiLabel : public FGuiSingleInaccesbleView
{
protected:
	FText * lpText;

	void CalcLabelSize( const FVector2F & vPos );

public:
	FGuiLabel( const FVector2F & vPos, const FString & sText, const FString & sFont, const FColor4F & cColor, F32 fSize, FGuiView * lpSuperView );
	~FGuiLabel();

	void SetText( const FString & sText );
	void SetFont( const FString & sFont );
	void SetTextColor( const FColor4F & cColor );
	void SetFontSize( F32 fSize );

	FString GetText()const;

	DEFINE_OBJ_DERIVERED( FGuiLabel );
};

DEFINE_OBJ_NAME( FGuiLabel );

#endif