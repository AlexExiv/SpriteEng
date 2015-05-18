#ifndef __FTEXT_H__
#define __FTEXT_H__


#include "FPrimitive.h"
#include "..\Core\FColor.h"
#include "..\Math\FVector2D.h"
#include "..\Core\FString.h"



class FFont;

class FText : public FPrimitive
{
	/////свойства текста///
	FFont * lpFont;//шрифт
	FString sText;// собственно текст
	FColor4F cTextColor;//цвет текста
	FVector2F vPos;//позиция
	F32 fFontSize;//размер шрифта

	/////вспомогательные данные для отрисовки текста///
	UI32 iDataSize, iVertCount, iIndCount, iPrimCount;
	F32 fTextWidth, fTextHeight;

	void DrawCallback( UI32 iPrimCount, void * lpData, UI32 * lpIndeces );
	UI32 AddDataToSeq( void * lpData, UI32 * lpIndeces, UI32 iStartVert );
	UI32 GetDataSizePerInst()const;
	UI32 GetIndexCount()const;
	UI32 GetVertexCount()const;
	UI32 DrawType()const;

	//FObject * CreateObject( void * lpPlacement, const CHAR_ * lpTypes, va_list lpArgs ) ;

	MAKE_PRIM_FRIEND( FText );

public:
	FText( const FVector2F & vPos, const FColor4F & cColor, F32 fFontSize, const FString & sText, const FString & sFontName, UI32 iLayer, FScene * lpScene );
	~FText();

	void Draw( const FRect & rViewport );
	void Update( F32 fDTime );

	PRIM_ID GetID()const;

	void SetText( const FString & sText );
	void Move( const FVector2F & vDPos );
	void SetPos( const FVector2F & vPos );
	void SetColor( const FColor4F & cColor );
	void ChangeFont( const FString & sFontName );
	void SetFontSize( F32 fFontSize );

	FString GetText()const;
	FVector2F GetPos()const;
	FColor4F GetColor()const;
	F32 GetFontSize()const;
	F32 GetTextWidth()const;
	F32 GetTextHeight()const;
	F32 GetScaleTextWidth()const;
	F32 GetScaleTextHeight()const;
};

#endif