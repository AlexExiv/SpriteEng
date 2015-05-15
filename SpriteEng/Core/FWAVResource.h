#ifndef __FWAVRESOURCE_H__
#define __FWAVRESOURCE_H__


#include "FResource.h"

class FWAVResource : public FResource
{
protected:
	UI32 iSampleRate, iNumChan, iBitsPerSample;
	F32 FWavLen;

	FResource * Make( void * lpData, UI32 iDataLen, FResourceManager * lpCreator );
	void SaveResource( void ** lpData, UI32 & iDataSize );

public:
	FWAVResource( void * lpData, UI32 iDataLen, FResourceManager * lpCreator );
	~FWAVResource();

	F32 GetLen()const;
	UI32 GetSampleRate()const;
	UI32 GetNumChan()const;
	UI32 GetBitsPerSample()const;
};

#endif