#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "FFile.h"
#include "FStack.h"
#include "FAtlasHeader.h"
#include "FList.h"


const FString sFileName( "cellback.adf" );

struct FFrame
{
	UI32 iFrameInd;
	UI32 x, y;
	UI32 iWidth, iHeight;
	F32 u0, v0, u1, v1;
	FString sFrameName;
};

struct FAnimation
{
	UI32 iFrameStart, iFrameEnd;
	FString sAnimName;
};

typedef FList<FFrame *> FFrameList;
typedef FList<FAnimation *> FAnimationList;



void main()
{
	FStack::Init();

	FFile * lpFile = FFile::MakeFile( sFileName, FFile::FILE_READ );
	UI32 iSize = lpFile->GetSize();
	UI8 * lpData = (UI8 *)FStack::GetInstanceStack()->PushBlock( iSize );
	lpFile->Read( lpData, iSize );

	FAtlasHeader * lpHeader = (FAtlasHeader *)lpData;
	FFrameHeader * lpFrames = (FFrameHeader *)(lpData + lpHeader->iFrameDataStart);
	FAnimationHeader * lpAnims = (FAnimationHeader *)(lpData + lpHeader->iAnimationDataStart);
	CHAR_ * lpStrTable = (CHAR_ *)lpData;

	FFrameList lFrameList;
	FAnimationList lAnimList;

	for( UI32 i = 0;i < lpHeader->iFrameCount;i++ )
	{
		FFrame * lpFrame = new FFrame;

		lpFrame->iFrameInd = lpFrames[i].iFrameNum;
		lpFrame->x = lpFrames[i].x;
		lpFrame->y = lpFrames[i].y;
		lpFrame->iWidth = lpFrames[i].iWidth;
		lpFrame->iHeight = lpFrames[i].iHeight;
		lpFrame->u0 = lpFrames[i].u0;
		lpFrame->v0 = lpFrames[i].v0;
		lpFrame->u1 = lpFrames[i].u1;
		lpFrame->v1 = lpFrames[i].v1;
		lpFrame->sFrameName = lpStrTable + lpFrames[i].iNameOff;

		lFrameList.PushBack( lpFrame );
	}

	for( UI32 i = 0;i < lpHeader->iAnimationCount;i++ )
	{
		FAnimation * lpAnim = new FAnimation;

		lpAnim->iFrameStart = lpAnims[i].iStartFrame;
		lpAnim->iFrameEnd = lpAnims[i].iEndFrame;
		lpAnim->sAnimName = lpStrTable + lpAnims[i].iNameOff;

		lAnimList.PushBack( lpAnim );
	}


	FFile::ReleaseFile( lpFile );

	FStack::Deinit();
}