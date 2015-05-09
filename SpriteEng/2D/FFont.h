#ifndef __FFONT_H__
#define __FFONT_H__


#include "FGraphObject.h"
#include "..\Core\FColor.h"
#include "FView.h"


class FTexture;
class FShader;

class FFont : public FGraphObject
{
	struct FChar
	{
		F32 u0, v0;
		F32 u1, v1;
		F32 fWidth, fHeight;
		CHAR_ cChar;
	};

	FTexture * lpTexture;
	FShader * lpShader;
	UI32 iAlphaBlock;

	FChar cAlphabit[MAX_ALPHABIT];

	UI32 LoadInfo( UI32 iBlockSize, UI8 * lpData );
	UI32 LoadCommon( UI32 iBlockSize, UI8 * lpData );
	UI32 LoadPages( UI32 iBlockSize, UI8 * lpData, FString & sFileName );
	UI32 LoadChars( UI32 iBlockSize, UI8 * lpData, F32 fWidth, F32 fHeight );
	UI32 LoadKerning( UI32 iBlockSize, UI8 * lpData );


public:
	FFont( const FString & sName );
	~FFont();

	F32 GetCharHeight( CHAR_ cChar, F32 fFontSize )const;
	F32 GetCharWidth( CHAR_ cChar, F32 fFontSize )const;
	void GetTextRect( const CHAR_ * lpText, FRect * lpRect, F32 fFontSize );

	void DrawInLine( const FVector2F & vPos, const CHAR_ * lpString, F32 fFontSize, const FColor4F & cColor, void * lpOutput, UI32 * lpIndeces, UI32 iStartVert );
	void DrawInRect( const FRect & rOutputRect, const CHAR_ * lpText, F32 fFontSize, const FColor4F & cColor, void * lpOutput, UI32 * lpIndeces, UI32 iStartVert );
	void DrawInRectStart( const FVector2F & vPos, const FRect & rOutputRect, const CHAR_ * lpText, F32 fFontSize, const FColor4F & cColor, void * lpOutput, UI32 * lpIndeces, UI32 iStartVert );

	UI32 GetDrawContCount()const;
	const FDrawDataCont * GetDrawDataCont()const;
	UI32 GetCharDataSize()const;
	UI32 GetCharIndCount()const;
	UI32 GetCharVertCount()const;
	UI32 GetTextPrimCount( const CHAR_ * lpText )const;

	void Bind();
	void UnBind();
};

#endif