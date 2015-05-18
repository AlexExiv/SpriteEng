#ifndef __FPNG_RESOURCE_H__
#define __FPNG_RESOURCE_H__

#include "FImageResource.h"

class FPNGResource : public FImageResource
{
	friend class FResourceManager;

	virtual FResource * Make( void * lpPlacement, void * lpData, UI32 iDataLen, FResourceManager * lpCreator );
	void SaveResource( void ** lpData, UI32 & iImgSize );
	UI32 GetSize()const;
	FPNGResource();

public:
	FPNGResource( void * lpData, UI32 iDataLen, FResourceManager * lpCreator );
	FPNGResource( UI32 iWidth, UI32 iHeight, void * lpData, UI32 iFormat );
	~FPNGResource();

};

#endif