#ifndef __FGLTEXTURE_H__
#define __FGLTEXTURE_H__


#include "FTexture.h"
#include "libExt.h"


class FGLTexture : public FTexture
{
	GLuint iTexID;

	void Init();

public:
	FGLTexture( const FString & sName );
	FGLTexture( const FString & sName, const FImageResource * lpImg );
	FGLTexture( const FString & sName, UI32 iWidth, UI32 iHeight, UI32 iFormat );
	~FGLTexture();

	void SetData( UI32 iFormat, void * lpData );
	void Bind( UI32 iLevel );
	void UnBind( UI32 iLevel );

};

#endif