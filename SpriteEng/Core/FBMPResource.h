#ifndef __FBMP_RESOURCE_H__
#define __FBMP_RESOURCE_H__

#include "FImageResource.h"

class FBMPResource : public FImageResource
{
	friend class FResourceManager;

	virtual FResource * Make( void * lpPlacement, void * lpData, UI32 iDataLen, FResourceManager * lpCreator );
	void SaveResource( void ** lpData, UI32 & iImgSize );
	UI32 GetSize()const;
	FBMPResource();

public:
	FBMPResource( void * lpData, UI32 iDataLen, FResourceManager * lpCreator );
	FBMPResource( UI32 iWidth, UI32 iHeight, void * lpData, UI32 iFormat );
	~FBMPResource();

};

#endif