#include "FAtlasResource.h"
#include "FLoopAllocator.h"
#include "FLog.h"
#include "FException.h"
#include "FResourceManager.h"
#include "FImageResource.h"
#include "FJPEGResource.h"
#include "FPNGResource.h"
#include <new.h>


const FString sAtlasExt( "adf" );

struct FAtlasType
{
	UI32 iAtlasType;
	UI32 iProgType;
};

const FAtlasType aAtlasTypeMap[] =
{
	{ ATLAS_TYPE_FLOAT, FAtlasResource::TYPE_FLOAT },
	{ ATLAS_TYPE_INT, FAtlasResource::TYPE_INT },
	{ ATLAS_TYPE_STR, FAtlasResource::TYPE_STR }
};

UI32 AtlasTypeRemap( UI32 iAtlasType )
{
	for( UI32 i = 0;i < ARRAY_SIZE( aAtlasTypeMap );i++ )
	{
		if( aAtlasTypeMap[i].iAtlasType == iAtlasType )
			return aAtlasTypeMap[i].iProgType;
	}

	AssertFatal( false, "Unknown type in atlas remap" );
	return 0;
}

FAtlasResource::FAtlasResource( void * lpData0, UI32 iDataLen, FResourceManager * lpCreator ) : FResource( sAtlasExt, lpCreator )
{
	FAtlasHeader * lpHeader = (FAtlasHeader *)lpData0;
	if( lpHeader->iFileType != ATLAS_FILE_TYPE )
		throw FException( FException::EXCP_UNK_FILE_FORMAT, "Not valid signature of atlas file format" );

	FFrameHeader * lpFramesHeader = (FFrameHeader *)((UI8 *)lpData0 + lpHeader->iFrameDataStart);
	FAnimationHeader * lpAnimsHeader = (FAnimationHeader *)((UI8 *)lpData0 + lpHeader->iAnimationDataStart);
	FAddDataHeader * lpAddsHeader = (FAddDataHeader *)((UI8 *)lpData0 + lpHeader->iAdditionalDataStart);
	CHAR_ * lpStrTable = (CHAR_ *)lpData0;

	iFrameCount = lpHeader->iFrameCount;
	iAnimCount = lpHeader->iAnimationCount;
	iAddCount = lpHeader->iAddDataCount;
	iAtlasWidth = lpHeader->iTexWidth;
	iAtlasHeight = lpHeader->iTexHeight;

	lpFrames = (FFrame *)ALLOC_LOOPT( sizeof( FFrame )*iFrameCount );
	lpAnims = (FAnimation *)ALLOC_LOOPT( sizeof( FAnimation )*iAnimCount );
	lpAdds = (FAdd *)ALLOC_LOOPT( sizeof( FAdd )*iAddCount );

	for( UI32 i = 0;i < iFrameCount;i++ )
	{
		lpFrames[i].iFrameInd = lpFramesHeader[i].iFrameNum;
		lpFrames[i].x = lpFramesHeader[i].x;
		lpFrames[i].y = lpFramesHeader[i].y;
		lpFrames[i].iWidth = lpFramesHeader[i].iWidth;
		lpFrames[i].iHeight = lpFramesHeader[i].iHeight;
		lpFrames[i].u0 = lpFramesHeader[i].u0;
		lpFrames[i].v0 = lpFramesHeader[i].v0;
		lpFrames[i].u1 = lpFramesHeader[i].u1;
		lpFrames[i].v1 = lpFramesHeader[i].v1;
		lpFrames[i].sFrameName = lpStrTable + lpFramesHeader[i].iNameOff;

	}

	for( UI32 i = 0;i < iAnimCount;i++ )
	{
		lpAnims[i].iStartFrame = lpAnimsHeader[i].iStartFrame;
		lpAnims[i].iEndFrame = lpAnimsHeader[i].iEndFrame;
		lpAnims[i].bLooped = lpAnimsHeader[i].iAnimFlags & ATLAS_ANIM_LOOPED;
		lpAnims[i].sAnimName = lpStrTable + lpAnimsHeader[i].iNameOff;
	}

	for( UI32 i = 0;i < iAddCount;i++ )
	{
		lpAdds[i].iType = AtlasTypeRemap( lpAddsHeader[i].iValType );
		lpAdds[i].sAddName = lpStrTable + lpAddsHeader[i].iNameOff;
		switch( lpAdds[i].iType )
		{
		case FAtlasResource::TYPE_INT:
			lpAdds[i].iInt = lpAddsHeader[i].iInt;
			break;
		case FAtlasResource::TYPE_FLOAT:
			lpAdds[i].fFloat = lpAddsHeader[i].fFloat;
			break;
		case FAtlasResource::TYPE_STR:
			lpAdds[i].lpStr = ::new (ALLOC_LOOPT( sizeof( FString ) )) FString( lpStrTable + lpAddsHeader[i].iStrOff );
			break;
		}
	}
	UI32 iResSize = iAtlasWidth*iAtlasHeight*sizeof( RGBA );
	if( (2*iResSize + iDataLen) > lpCreator->GetAllocSize() )
		throw FException( FException::EXCP_RESALLOC_INSUFFIENC, FString( "Insufficiently memory for open and decode sprite atlas"  ) );

	lpCreator->ResetAllocator();
	lpData = lpCreator->AllocForResource( iAtlasWidth*iAtlasHeight*sizeof( RGBA ) );

	FImageResource * lpRGBRes = NULL, * lpAlphaRes = NULL;
	void * lpRGBData = NULL, * lpAlphaData = NULL;
	try
	{
		if( lpHeader->iFlags & ATLAS_PACKED )
		{
			switch( lpHeader->iCompr )
			{
			case ATLAS_COMPR_RLE:
			case ATLAS_COMPR_NONE:
				lpRGBData = (UI8 *)lpData0 + lpHeader->iTexAtlasDataStart;
				break;
			case ATLAS_COMPR_JPG:
				lpRGBRes = new FJPEGResource( (UI8 *)lpData0 + lpHeader->iTexAtlasDataStart, lpHeader->iRGBDataLen, lpCreator );
				lpAlphaRes = new FJPEGResource( (UI8 *)lpData0 + lpHeader->iTexAtlasDataStart + lpHeader->iRGBDataLen, lpHeader->iAlphaDataLen, lpCreator );
				break;
			case ATLAS_COMPR_PNG:
				lpRGBRes = new FPNGResource( (UI8 *)lpData0 + lpHeader->iTexAtlasDataStart, lpHeader->iRGBDataLen, lpCreator );
				break;
			}

			lpRGBData = lpRGBRes->GetData();
			if( lpAlphaRes )
				lpAlphaData = lpAlphaRes->GetData();
		}
		else
		{
			FString sRGBName = lpStrTable + lpHeader->iAtlasNameOff;
			FString sAlphaName = lpStrTable + lpHeader->iAtlasAlphaNameOff;

			lpRGBRes = (FImageResource *)lpCreator->CreateResource( sRGBName );
			lpAlphaRes = (FImageResource *)lpCreator->CreateResource( sAlphaName );
			lpRGBData = lpRGBRes->GetData();
			lpAlphaData = lpAlphaRes->GetData();
		}

		if( lpHeader->iFlags & ATLAS_RGB )
		{
			RGBA * lpDst = (RGBA *)lpData;
			RGB * lpSrc = (RGB *)lpRGBData;
			for( UI32 i = 0;i < (iAtlasWidth*iAtlasHeight);i++, lpDst++, lpSrc++ )
			{
				lpDst->r = lpSrc->r;
				lpDst->g = lpSrc->g;
				lpDst->b = lpSrc->b;
			}
		}
		if( lpHeader->iFlags & ATLAS_ALPHA )
		{
			RGBA * lpDst = (RGBA *)lpData;
			UI8 * lpSrc = (UI8 *)lpAlphaData;
			for( UI32 i = 0;i < (iAtlasWidth*iAtlasHeight);i++, lpDst++, lpSrc++ )
				lpDst->a = *lpSrc;
		}
		if( lpHeader->iFlags & ATLAS_RGBA )
		{
			RGBA * lpDst = (RGBA *)lpData;
			RGBA * lpSrc = (RGBA *)lpAlphaData;
			for( UI32 i = 0;i < (iAtlasWidth*iAtlasHeight);i++, lpDst++, lpSrc++ )
				*lpDst = *lpSrc;
		}
	}
	catch( FException eExcp )
	{
		if( lpRGBRes )
			delete lpRGBRes;
		if( lpAlphaRes )
			delete lpAlphaRes;

		throw;
	}
	if( lpRGBRes )
		delete lpRGBRes;
	if( lpAlphaRes )
		delete lpAlphaRes;
}

FAtlasResource::FAtlasResource() : FResource( sAtlasExt, FString( "Atlas file" ) )
{
}

FAtlasResource::~FAtlasResource( )
{
	for( UI32 i = 0;i < iAddCount;i++ )
	{
		if( lpAdds[i].iType == FAtlasResource::TYPE_STR )
			lpAdds[i].lpStr->~FString();
	}
}

FResource * FAtlasResource::Make( void * lpData, UI32 iDataLen, FResourceManager * lpCreator )
{
	return new FAtlasResource( lpData, iDataLen, lpCreator );
}

void FAtlasResource::SaveResource( void ** lpData, UI32 & iImgSize )
{
}

UI32 FAtlasResource::GetAtlasWidth()const
{
	return iAtlasWidth;
}

UI32 FAtlasResource::GetAtlasHeight()const
{
	return iAtlasHeight;
}

UI32 FAtlasResource::GetFrameCount()const
{
	return iFrameCount;
}

UI32 FAtlasResource::GetAnimationCount()const
{
	return iAnimCount;
}

UI32 FAtlasResource::GetAddDataCount()const
{
	return iAddCount;
}

void FAtlasResource::GetFrameData( FFrame * lpHeader, UI32 iFrameInd )
{
	lpHeader->iFrameInd = lpFrames[iFrameInd].iFrameInd;
	lpHeader->iWidth = lpFrames[iFrameInd].iWidth;
	lpHeader->iHeight = lpFrames[iFrameInd].iHeight;
	lpHeader->u0 = lpFrames[iFrameInd].u0;
	lpHeader->v0 = lpFrames[iFrameInd].v0;
	lpHeader->u1 = lpFrames[iFrameInd].u1;
	lpHeader->v1 = lpFrames[iFrameInd].v1;
	lpHeader->x = lpFrames[iFrameInd].x;
	lpHeader->y = lpFrames[iFrameInd].y;
	lpHeader->sFrameName = lpFrames[iFrameInd].sFrameName;
}

void FAtlasResource::GetAnimationData( FAnimation * lpHeader, UI32 iAnimInd )
{
	lpHeader->iStartFrame = lpAnims[iAnimInd].iStartFrame;
	lpHeader->iEndFrame = lpAnims[iAnimInd].iEndFrame;
	lpHeader->bLooped = lpAnims[iAnimInd].bLooped;
	lpHeader->sAnimName = lpAnims[iAnimInd].sAnimName;
}

void FAtlasResource::GetAdditData( FAdd * lpHeader, UI32 iAddInd )
{
	lpHeader->sAddName = lpAdds[iAddInd].sAddName;
	lpHeader->iType = lpAdds[iAddInd].iType;

	switch( lpHeader->iType )
	{
	case FAtlasResource::TYPE_FLOAT:
		lpHeader->fFloat = lpAdds[iAddInd].fFloat;
		break;
	case FAtlasResource::TYPE_INT:
		lpHeader->iInt = lpAdds[iAddInd].iInt;
		break;
	case FAtlasResource::TYPE_STR:
		lpHeader->sAddName = lpAdds[iAddInd].sAddName;
		break;
	};
}
