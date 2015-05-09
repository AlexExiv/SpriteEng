#ifndef __FSHADER_H__
#define __FSHADER_H__


#include "FGraphObject.h"
#include "..\Math\FVector2D.h"
#include "..\Core\FColor.h"
#include "..\Core\FDictionary.h"




class FShader : public FGraphObject
{
protected:
	struct FTextureBlock
	{
		UI32 iBlock;
	};
	typedef FDictionary<FTextureBlock *> FTextureDict;
	typedef FDictionary<FTextureBlock *>::FRecord FTextureRecord;

	FTextureDict dTextureDict;

public:
	FShader( const FString & sName );
	~FShader();

	virtual void Bind() = 0;
	virtual void UnBind() = 0;

	virtual void SetTexture( const FString & sTexName, UI32 iBlock ) = 0;
	virtual void SetFloat( const FString & sVarName, F32 fFloat ) = 0;
	virtual void SetFloatPointer( const FString & sVarName, UI32 iStride, const F32 * lpPointer ) = 0;
	virtual void SetInt( const FString & sVarName, I32 iInt ) = 0;
	virtual void SetColor( const FString & sVarName, const FColor4F & cColor ) = 0;
	virtual void SetVector2F( const FString & sVarName, const FVector2F & vVec ) = 0;
	UI32 GetTextureBlock( const FString & sTexName )const;
};

#endif