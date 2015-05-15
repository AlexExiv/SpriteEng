#include <windows.h>
#include "FBMPResource.h"
#include "FFile.h"
#include "FLoopAllocator.h"
#include "FException.h"
#include "FResourceManager.h"



#define BMP_FILE_TYPE 0x4D42
const FString sBMPExt( "bmp" );

FBMPResource::FBMPResource() : FImageResource( sBMPExt )
{
}

FBMPResource::FBMPResource( void * lpData0, UI32 iDataLen, FResourceManager * lpCreator ) : FImageResource( lpData0, sBMPExt, lpCreator )
{
	BITMAPFILEHEADER * lpFileBmp = (BITMAPFILEHEADER *)lpData0;
	BITMAPINFOHEADER * lpBmp = (BITMAPINFOHEADER *)((FBYTE *)lpData0 + sizeof(BITMAPFILEHEADER));

	if( lpFileBmp->bfType != BMP_FILE_TYPE )
		throw FException( FException::EXCP_UNK_FILE_FORMAT, FString( "Unknown file format, first 2 bytes must be 0x4D42 or \"BM\"" ) );

	UI8 * lpBmpData = (UI8 *)lpData0 + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	iWidth = lpBmp->biWidth;
	iHeight = lpBmp->biHeight;
	iFormat = IMAGE_RGB;
	iBpp = 3;

	lpData = lpCreator->AllocForResource( iWidth * iHeight * sizeof( RGB ) );
	RGB * lpDst = (RGB *)lpData;

	for( UI32 i = 0;i < iHeight;i++ )
	{
		RGB * lpSrc = (RGB *)(lpBmpData + (lpBmp->biHeight - i - 1)*DWORD_ALIG( lpBmp->biWidth ));
		for( UI32 j = 0;j < iWidth;j++ )
		{
			lpDst->r = lpSrc->b;
			lpDst->g = lpSrc->g;
			lpDst->b = lpSrc->r;

			lpDst++;
			lpSrc++;
		}
	}
}

FBMPResource::FBMPResource( UI32 iWidth, UI32 iHeight, void * lpData, UI32 iFormat ) : FImageResource( iWidth, iHeight, lpData, iFormat, FString( "bmp" ) )
{
}

FBMPResource::~FBMPResource()
{
}


char * MakeDwordAligImage( UI32 iWidth, UI32 iHeight, void * lpData, UI32 iFormat )
{
	char * lpAligImage = (char *) ALLOC_LOOP( DWORD_ALIG(iWidth) * iHeight );
	if( iFormat == FImageResource::IMAGE_RGBA )
	{
		RGBA * lpSrc = (RGBA *)lpData;
		for( int i = 0;i < iHeight;i++ )
		{
			RGB * lpDst = (RGB *)(lpAligImage + (iHeight - i - 1)*DWORD_ALIG(iWidth));
			for( int j = 0;j < iWidth;j++ )
			{
				I32 iData = *(I32 *)lpSrc;
				lpDst->b = lpSrc->r;
				lpDst->g = lpSrc->g;
				lpDst->r = lpSrc->b;
				lpSrc++;
				lpDst++;
			}
		}
	}
	else if( iFormat == FImageResource::IMAGE_RGB )
	{
		RGB * lpSrc = (RGB *)lpData;
		for( int i = 0;i < iHeight;i++ )
		{
			RGB * lpDst = (RGB *)(lpAligImage + (iHeight - i - 1)*DWORD_ALIG(iWidth));
			for( int j = 0;j < iWidth;j++ )
			{
				I32 iData = *(I32 *)lpSrc;
				lpDst->b = lpSrc->r;
				lpDst->g = lpSrc->g;
				lpDst->r = lpSrc->b;
				lpSrc++;
				lpDst++;
			}
		}
	}
	else
	{
	}

	return lpAligImage;
}

void FBMPResource::SaveResource( void ** lpBuffer, UI32 & iImgSize )
{
	iImgSize = sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ) + DWORD_ALIG(iWidth) * iHeight;
	*lpBuffer = ALLOC_LOOP( iImgSize );

	BITMAPFILEHEADER * lpFileBmp = (BITMAPFILEHEADER *)(*lpBuffer);
	BITMAPINFOHEADER * lpBmp = (BITMAPINFOHEADER *)(((UI8 *)(*lpBuffer)) + sizeof( BITMAPFILEHEADER ));

	lpBmp->biSize = sizeof( BITMAPINFOHEADER );
	lpBmp->biWidth = iWidth;
	lpBmp->biHeight = iHeight;
	lpBmp->biPlanes = 1;
	lpBmp->biBitCount = 24;
	lpBmp->biCompression = BI_RGB;
	lpBmp->biSizeImage = DWORD_ALIG(iWidth) * iHeight; 
	lpBmp->biClrImportant = 0;
	lpBmp->biClrUsed = 0;

	lpFileBmp->bfType = 0x4d42;
	lpFileBmp->bfSize = sizeof( BITMAPFILEHEADER ) + lpBmp->biSize + lpBmp->biSizeImage;
	lpFileBmp->bfReserved1 = 0;
	lpFileBmp->bfReserved2 = 0;
	lpFileBmp->bfOffBits = sizeof( BITMAPFILEHEADER ) + lpBmp->biSize;

	CHAR_ * lpAligData = MakeDwordAligImage( iWidth, iHeight, lpData, iFormat );
	memcpy( ((UI8 *)(*lpBuffer)) + sizeof( BITMAPFILEHEADER ) + sizeof( BITMAPINFOHEADER ), lpAligData, lpBmp->biSizeImage );
}

FResource * FBMPResource::Make( void *lpData, UI32 iDataLen, FResourceManager * lpCreator )
{
	return new FBMPResource( lpData, iDataLen, lpCreator );
}