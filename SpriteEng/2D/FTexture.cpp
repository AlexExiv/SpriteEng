#include "FTexture.h"
#include "..\Core\FImageResource.h"
#include "..\Core\FException.h"



FTexture::FTexture( const FString & sName ) : FGraphObject( sName, FGraphObjectManager::OBJECT_TEXTURE ), iWidth( 0 ), iHeight( 0 ), iFormat( 0 )
{

}

FTexture::FTexture( const FString & sName, FImageResource * lpImg ) : FGraphObject( sName, FGraphObjectManager::OBJECT_TEXTURE ),
	iWidth( lpImg->GetWidth() ), iHeight( lpImg->GetHeight() )
{
	switch( lpImg->GetFormat() )
	{
	case FImageResource::IMAGE_RGB:
		iFormat = TEX_RGB;
		break;
	case FImageResource::IMAGE_RGBA:
		iFormat = TEX_RGBA;
		break;
	default:
		throw FException( FException::EXCP_UNK_IMAGE_FORMAT, FString( "Unsupported texture format" ) );
	}
}

FTexture::FTexture( const FString & sName, UI32 iWidth, UI32 iHeight, UI32 iFormat ): FGraphObject( sName, FGraphObjectManager::OBJECT_TEXTURE ),
	iWidth( iWidth ), iHeight( iHeight ), iFormat( iFormat )
{

}

FTexture::~FTexture()
{

}


UI32 FTexture::GetWidth()const
{
	return iWidth;
}

UI32 FTexture::GetHeight()const
{
	return iHeight;
}

UI32 FTexture::GetFormat()const
{
	return iFormat;
}

