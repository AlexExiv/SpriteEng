#ifndef __FJPEG_RESOURCE_H__
#define __FJPEG_RESOURCE_H__

#include "FImageResource.h"

class FJPEGResource : public FImageResource
{
	friend class FResourceManager;

	virtual FResource * Make( void * lpPlacement, void * lpData, UI32 iDataLen, FResourceManager * lpCreator );
	void SaveResource( void ** lpData, UI32 & iImgSize );
	UI32 GetSize()const;
	FJPEGResource();

	I32 iQuality;
public:
	FJPEGResource( void * lpData, UI32 iDataLen, FResourceManager * lpCreator );
	FJPEGResource( UI32 iWidth, UI32 iHeight, void * lpData, UI32 iFormat );
	~FJPEGResource();

	void SetQuality( I32 iQuality );
};

#endif