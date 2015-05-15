#include "FImageResource.h"
#include "FResourceManager.h"
#include "FLog.h"


FImageResource::FImageResource( const FString & sExtStr ) : FResource( sExtStr, "Image" )
{

}

FImageResource::FImageResource( void * lpData, const FString & sExtStr, FResourceManager * lpCreator ) : FResource( sExtStr, lpCreator ), iFormat( IMAGE_UNKNOWN )
{
	bRemoveData = false;
}

FImageResource::FImageResource( UI32 iWidth, UI32 iHeight, void * lpData, UI32 iFormat, const FString & sExtStr  ): iWidth( iWidth ), 
	iHeight( iHeight ), iFormat( iFormat ), FResource( sExtStr, NULL )
{
	bRemoveData = false;
	switch( iFormat )
	{
	case IMAGE_RGB:
		iBpp = 3;
		break;
	case IMAGE_RGBX:
	case IMAGE_RGBA:
		iBpp = 4;
		break;
	case IMAGE_A:
	case IMAGE_L:
		iBpp = 1;
		break;
	case IMAGE_DXT1:
		iBpp = 3;
		break;
	case IMAGE_DXT2:
	case IMAGE_DXT3:
	case IMAGE_DXT4:
	case IMAGE_DXT5:
		iBpp = 4;
		break;
	};
}

FImageResource::~FImageResource()
{
}

UI32 FImageResource::GetWidth()const
{
	return iWidth;
}

UI32 FImageResource::GetHeight()const
{
	return iHeight;
}

UI32 FImageResource::GetFormat()const
{
	return iFormat;
}

UI32 FImageResource::GetBpp()const
{
	return iBpp;
}

void * FImageResource::GetDataLevel( I32 iLevel )const
{
	return lpData;
}

UI32 FImageResource::GetLevelSize( I32 iLevel )const
{
	return 0;
}

bool FImageResource::IsCompressed()const
{
	return (iFormat == IMAGE_DXT1)||(iFormat == IMAGE_DXT2)||(iFormat == IMAGE_DXT3)||(iFormat == IMAGE_DXT4)||(iFormat == IMAGE_DXT5);
}


void FImageResource::AddAlphaData( void * lpAlphaData, UI32 iFormat_ )
{
	if( !lpAlphaData )
		return;

	if( IsCompressed() )
	{
		FLog::PutError( "Can't add alpha data from DXT compressed format" );
		return;
	}

	if( iFormat_ == IMAGE_DXT1 || iFormat_ == IMAGE_DXT2 || iFormat_ == IMAGE_DXT3 
		|| iFormat_ == IMAGE_DXT4 || iFormat_ == IMAGE_DXT5 || iFormat_ == IMAGE_RGBX )
	{
		FLog::PutError( "Can't add alpha data to DXT compressed format" );
		return;
	}


	RGBA * lpNewData;
	if( (iFormat == IMAGE_RGBA) || (iFormat == IMAGE_RGBX) )
		lpNewData = (RGBA *)lpData;
	else if( (iFormat == IMAGE_A) || (iFormat == IMAGE_L) )
	{
		UI8 * lpDst = (UI8 *)lpData;
		UI8 * lpSrc1;
		RGB * lpSrc3;
		RGBA * lpSrc4;
		UI32 i = 0;

		switch( iFormat_ )
		{
		case IMAGE_A:
		case IMAGE_L:
			lpSrc1 = (UI8 *)lpAlphaData;
			for(;i < iWidth*iHeight;i++, lpSrc1++, lpDst++ )
				*lpDst = *lpSrc1;
			break;
		case IMAGE_RGB:
			lpSrc3 = (RGB *)lpAlphaData;
			for(;i < iWidth*iHeight;i++, lpSrc3++, lpDst++ )
				*lpDst = lpSrc3->r;
			break;
		case IMAGE_RGBA:
			lpSrc4 = (RGBA *)lpAlphaData;
			for(;i < iWidth*iHeight;i++, lpSrc4++, lpDst++ )
				*lpDst = lpSrc4->a;
			break;
		default:
			FLog::PutError( "Can't add alpha data: unknown source file format" );
		};

		return;
	}
	else
	{
		lpNewData = (RGBA *)lpCreator->AllocForResource( iWidth*iHeight*sizeof( RGBA ) );
		RGB * lpSrc = (RGB *)lpData;
		RGBA * lpDst = lpNewData;
		for( UI32 i = 0;i < iWidth*iHeight;i++, lpSrc++, lpDst++ )
		{
			lpDst->r = lpSrc->r;
			lpDst->g = lpSrc->g;
			lpDst->b = lpSrc->b;
		}
	}

	RGBA * lpDst = (RGBA *)lpNewData;
	UI8 * lpSrc1;
	RGB * lpSrc3;
	RGBA * lpSrc4;
	UI32 i = 0;

	switch( iFormat_ )
	{
	case IMAGE_A:
	case IMAGE_L:
		lpSrc1 = (UI8 *)lpAlphaData;
		for(;i < iWidth*iHeight;i++, lpSrc1++, lpDst++ )
			lpDst->a = *lpSrc1;
		break;
	case IMAGE_RGB:
		lpSrc3 = (RGB *)lpAlphaData;
		for(;i < iWidth*iHeight;i++, lpSrc3++, lpDst++ )
			lpDst->a = lpSrc3->r;
		break;
	case IMAGE_RGBA:
		lpSrc4 = (RGBA *)lpAlphaData;
		for(;i < iWidth*iHeight;i++, lpSrc4++, lpDst++ )
			lpDst->a = lpSrc4->a;
		break;
	default:
		FLog::PutError( "Can't add alpha data: unknown source file format" );
		return;
	};
	lpData = lpNewData;
}

void FImageResource::AddAlphaData( FImageResource * lpImage )
{
	if( !lpImage )
		return;

	if( IsCompressed() )
	{
		FLog::PutError( "Can't add alpha data from DXT compressed format" );
		return;
	}

	if( lpImage->IsCompressed() )
	{
		FLog::PutError( "Can't add alpha data to DXT compressed format" );
		return;
	}

	if( (lpImage->GetWidth() != iWidth) || (lpImage->GetHeight() != iHeight) )
	{
		FLog::PutError( "Can't add alpha data different width and height" );
		return;
	}

	RGBA * lpNewData;
	if( (iFormat == IMAGE_RGBA) || (iFormat == IMAGE_RGBX) )
		lpNewData = (RGBA *)lpData;
	else if( (iFormat == IMAGE_A) || (iFormat == IMAGE_L) )
	{
		UI8 * lpDst = (UI8 *)lpData;
		UI8 * lpSrc1;
		RGB * lpSrc3;
		RGBA * lpSrc4;
		UI32 i = 0;

		switch( lpImage->GetFormat() )
		{
		case IMAGE_A:
		case IMAGE_L:
			lpSrc1 = (UI8 *)lpImage->GetData();
			for(;i < iWidth*iHeight;i++, lpSrc1++, lpDst++ )
				*lpDst = *lpSrc1;
			break;
		case IMAGE_RGB:
			lpSrc3 = (RGB *)lpImage->GetData();
			for(;i < iWidth*iHeight;i++, lpSrc3++, lpDst++ )
				*lpDst = lpSrc3->r;
			break;
		case IMAGE_RGBA:
			lpSrc4 = (RGBA *)lpImage->GetData();
			for(;i < iWidth*iHeight;i++, lpSrc4++, lpDst++ )
				*lpDst = lpSrc4->a;
			break;
		default:
			FLog::PutError( "Can't add alpha data: unknown source file format" );
		};

		return;
	}
	else
	{
		lpNewData = (RGBA *)lpCreator->AllocForResource( iWidth*iHeight*sizeof( RGBA ) );
		RGB * lpSrc = (RGB *)lpData;
		RGBA * lpDst = lpNewData;
		for( UI32 i = 0;i < iWidth*iHeight;i++, lpSrc++, lpDst++ )
		{
			lpDst->r = lpSrc->r;
			lpDst->g = lpSrc->g;
			lpDst->b = lpSrc->b;
		}
	}

	RGBA * lpDst = (RGBA *)lpNewData;
	UI8 * lpSrc1;
	RGB * lpSrc3;
	RGBA * lpSrc4;
	UI32 i = 0;

	switch( lpImage->GetFormat() )
	{
	case IMAGE_A:
	case IMAGE_L:
		lpSrc1 = (UI8 *)lpImage->GetData();
		for(;i < iWidth*iHeight;i++, lpSrc1++, lpDst++ )
			lpDst->a = *lpSrc1;
		break;
	case IMAGE_RGB:
		lpSrc3 = (RGB *)lpImage->GetData();
		for(;i < iWidth*iHeight;i++, lpSrc3++, lpDst++ )
			lpDst->a = lpSrc3->r;
		break;
	case IMAGE_RGBA:
		lpSrc4 = (RGBA *)lpImage->GetData();
		for(;i < iWidth*iHeight;i++, lpSrc4++, lpDst++ )
			lpDst->a = lpSrc4->a;
		break;
	default:
		FLog::PutError( "Can't add alpha data: unknown source file format" );
		return;
	};
	lpData = lpNewData;
}

FResource * FImageResource::Make( void * lpData, UI32 iDataLen, FResourceManager * lpCreator )
{
	return NULL;
}