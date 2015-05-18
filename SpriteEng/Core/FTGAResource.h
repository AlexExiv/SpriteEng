#ifndef __FTGARESOURCE_H__
#define __FTGARESOURCE_H__

#include "FImageResource.h"

class FTGAResource : public FImageResource
{
	friend class FResourceManager;

	FResource * Make( void * lpPlacement, void * lpData, UI32 iDataLen, FResourceManager * lpCreator );
	UI32 GetSize()const;

	FTGAResource();
public:
	FTGAResource( void * lpData, UI32 iDataLen, FResourceManager * lpCreator );
	~FTGAResource();
};

#endif