#ifndef __FANIMATION2D_H__
#define __FANIMATION2D_H__


#include "FGraphObject.h"
#include "FView.h"
#include "..\Core\FArray.h"
#include "..\Core\FDictionary.h"
#include "..\Math\FVector2D.h"


class FTexture;
class FShader;

class FAnimation2D : public FGraphObject
{
public:
	struct FAnimation
	{
		FString sAnimName;
		bool bLooped;
		UI32 iStartFrame, iEndFrame;
	};

protected:
	struct FFrame
	{
		F32 u0, v0;
		F32 u1, v1;

		F32 fWidth, fHeight;
	};

	typedef FDictionary<FAnimation *> FAnimationDict;
	typedef FDictionary<FAnimation *>::FRecord FAnimationRecord;
	typedef FArray<FFrame *> FFrameArray;
	typedef FArray<FFrame *>::Iterator FFrameIterator;

	FAnimationDict dAnimatioDict;
	FFrameArray aFrameArr;

	FTexture * lpTexture;
	FShader * lpShader;
	UI32 iAtlasBlock;

	void DeleteAll();

public:
	FAnimation2D( const FString & sName );
	~FAnimation2D();

	F32 GetWidth( UI32 iFrame )const;
	F32 GetHeight( UI32 iFrame )const;
	void GetAnimInf( const FString & sAnimName, FAnimation * lpInfo );

	void Draw( const FVector2F & vPos, F32 fScale, UI32 iFrameIndex, F32 fAlpha, void * lpData, UI32 * lpIndeces, UI32 iStartVert );

	UI32 GetDrawContCount()const;
	const FDrawDataCont * GetDrawDataCont()const;
	UI32 GetAnimDataSize()const;

	void Bind();
	void UnBind();
};

#endif