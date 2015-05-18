#include "FGLTexture.h"
#include "..\Core\FImageResource.h"
#include "..\Core\FResourceManager.h"
#include "..\Core\FException.h"



FGLTexture::FGLTexture( const FString & sName ) : FTexture( sName ), iTexID( 0 )
{
	Init();
	FImageResource * lpRes = (FImageResource *)FResourceManager::SharedManager()->CreateResource( sName );
	if( !lpRes )
		throw FException( FException::EXCP_FILE_NOT_FOUND, "Can't create texture object" );

	FImageResource * lpAlphaRes = (FImageResource *)FResourceManager::SharedManager()->CreateResource( sName.AppendToName( "_a" ) );
	if( lpAlphaRes )
		lpRes->AddAlphaData( lpAlphaRes );

	iWidth = lpRes->GetWidth();
	iHeight = lpRes->GetHeight();
	
	try
	{
		switch( lpRes->GetFormat() )
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
	catch( FException eExcp )
	{
		//delete lpRes;
		glBindTexture( GL_TEXTURE_2D, 0 );
		glDeleteTextures( 1, &iTexID );
		throw eExcp;
	}

	GLenum eFmt;
	switch( iFormat )
	{
	case FTexture::TEX_RGB:
		eFmt = GL_RGB;
		break;
	case FTexture::TEX_RGBA:
		eFmt = GL_RGBA;
		break;
	};

	glTexImage2D( GL_TEXTURE_2D, 0, eFmt, iWidth, iHeight, 0, eFmt, GL_UNSIGNED_BYTE, lpRes->GetData() );

	//delete lpRes;
	glBindTexture( GL_TEXTURE_2D, 0 );
}

FGLTexture::FGLTexture( const FString & sName, const FImageResource * lpImg ) : FTexture( sName ), iTexID( 0 )
{
	Init();

	iWidth = lpImg->GetWidth();
	iHeight = lpImg->GetHeight();
	
	try
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
	catch( FException eExcp )
	{
		glBindTexture( GL_TEXTURE_2D, 0 );
		glDeleteTextures( 1, &iTexID );
		throw eExcp;
	}

	GLenum eFmt;
	switch( iFormat )
	{
	case FTexture::TEX_RGB:
		eFmt = GL_RGB;
		break;
	case FTexture::TEX_RGBA:
		eFmt = GL_RGBA;
		break;
	};

	glTexImage2D( GL_TEXTURE_2D, 0, eFmt, iWidth, iHeight, 0, eFmt, GL_UNSIGNED_BYTE, lpImg->GetData() );
	glBindTexture( GL_TEXTURE_2D, 0 );
}

FGLTexture::FGLTexture( const FString & sName, UI32 iWidth, UI32 iHeight, UI32 iFormat ) : FTexture( sName, iWidth, iHeight, iFormat ), iTexID( 0 )
{
	Init();
	GLenum eFmt;

	switch( iFormat )
	{
	case FTexture::TEX_RGB:
		eFmt = GL_RGB8;
		break;
	case FTexture::TEX_RGBA:
		eFmt = GL_RGBA8;
		break;
	};
	//glTexImage2D( GL_TEXTURE_2D, 0, eFmt, iWidth, iHeight, 0, eFmt, GL_UNSIGNED_BYTE, NULL );
	glTexStorage2D( GL_TEXTURE_2D, 1, eFmt, iWidth, iHeight );
}

FGLTexture::~FGLTexture()
{
	glDeleteTextures( 1, &iTexID );
}

void FGLTexture::Init()
{
	glGenTextures( 1, &iTexID );
	glBindTexture( GL_TEXTURE_2D, iTexID );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
}

void FGLTexture::SetData( UI32 iFormat_, void * lpData )
{
	GLenum eFmt;
	switch( iFormat_ )
	{
	case FTexture::TEX_RGB:
		eFmt = GL_RGB;
		break;
	case FTexture::TEX_RGBA:
		eFmt = GL_RGBA;
		break;
	};
	glBindTexture( GL_TEXTURE_2D, iTexID );
	glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, iWidth, iHeight, eFmt, GL_UNSIGNED_BYTE, lpData );
	glBindTexture( GL_TEXTURE_2D, 0 );
}

void FGLTexture::Bind( UI32 iLevel )
{
	glActiveTexture( GL_TEXTURE0 + iLevel );
	glBindTexture( GL_TEXTURE_2D, iTexID );
}

void FGLTexture::UnBind( UI32 iLevel )
{
	glActiveTexture( GL_TEXTURE0 + iLevel );
	glBindTexture( GL_TEXTURE_2D, 0 );
}

