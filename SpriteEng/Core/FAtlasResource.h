#ifndef __FATLASRESOURCE_H__
#define __FATLASRESOURCE_H__


#include "FResource.h"
#include "..\Core\FAtlasHeader.h"



class FAtlasResource : public FResource
{
	friend class FResourceManager;
public:
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
		UI32 iStartFrame, iEndFrame;
		bool bLooped;
		FString sAnimName;
	};

	struct FAdd
	{
		FString sAddName;
		UI32 iType;
		union
		{
			I32 iInt;
			F32 fFloat;
			FString * lpStr;
		};
	};

	enum
	{
		TYPE_FLOAT = 1,
		TYPE_INT = 2,
		TYPE_STR
	};

protected:
	FFrame * lpFrames;
	FAnimation * lpAnims;
	FAdd * lpAdds;

	UI32 iAtlasWidth, iAtlasHeight;
	UI32 iFrameCount, iAnimCount, iAddCount;

	friend class FResourceManager;

	FAtlasResource( void * lpData, UI32 iDataLen, FResourceManager * lpCreator );
	FAtlasResource();

	virtual FResource * Make( void * lpData, UI32 iDataLen, FResourceManager * lpCreator );
	void SaveResource( void ** lpData, UI32 & iImgSize );

public:
	~FAtlasResource( );

	UI32 GetAtlasWidth()const;
	UI32 GetAtlasHeight()const;
	UI32 GetFrameCount()const;
	UI32 GetAnimationCount()const;
	UI32 GetAddDataCount()const;
	void GetFrameData( FFrame * lpHeader, UI32 iFrameInd );
	void GetAnimationData( FAnimation * lpHeader, UI32 iAnimInd );
	void GetAdditData( FAdd * lpHeader, UI32 iAddInd );
};


#endif