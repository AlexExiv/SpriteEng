#ifndef __FGLSHADER_H__
#define __FGLSHADER_H__

#include "FShader.h"

#ifndef MOBILE_DEVICE


class FGLShader : public FShader
{
	enum
	{
		MAX_ATTRIB_ = 16
	};
	ShaderID iProgramID, iVertexID, iFragmentID;
	UI32 iAttribs[MAX_ATTRIB_], iCurAttrib;

	void PushAttrib( UI32 iIndex );
	void PopAll();

public:
	FGLShader( const FString & sName );
	~FGLShader();

	void Bind();
	void UnBind() ;

	void SetTexture( const FString & sTexName, UI32 iBlock );
	void SetFloat( const FString & sVarName, F32 fFloat );
	void SetFloatPointer( const FString & sVarName, UI32 iStride, const F32 * lpPointer );
	void SetInt( const FString & sVarName, I32 iInt );
	void SetColor( const FString & sVarName, const FColor4F & cColor );
	void SetVector2F( const FString & sVarName, const FVector2F & vVec );
};

#endif

#endif