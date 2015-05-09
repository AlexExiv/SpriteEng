#ifndef __FTEXTURE_H__
#define __FTEXTURE_H__


#include "..\types.h"
#include "FGraphObject.h"

class FImageResource;

class FTexture : public FGraphObject
{
protected:
	UI32 iWidth, iHeight;
	UI32 iFormat;

public:
	enum
	{
		TEX_RGB = 1,
		TEX_RGBA
	};

	FTexture( const FString & sName );
	FTexture( const FString & sName, FImageResource * lpImg );
	FTexture( const FString & sName, UI32 iWidth, UI32 iHeight, UI32 iFormat );
	~FTexture();

	virtual void SetData( UI32 iFormat, void * lpData ) = 0;
	virtual void Bind( UI32 iLevel ) = 0;
	virtual void UnBind( UI32 iLevel ) = 0;

	UI32 GetWidth()const;
	UI32 GetHeight()const;
	UI32 GetFormat()const;
};

#endif