#ifndef __FTGARESOURCE_H__
#define __FTGARESOURCE_H__

#include "FImageResource.h"

class FTGAResource : public FImageResource
{
	friend class FResourceManager;

	virtual FResource * Make( void * lpData, UI32 iDataLen, FResourceManager * lpCreator );

	FTGAResource();
public:
	FTGAResource( void * lpData, UI32 iDataLen, FResourceManager * lpCreator );
	~FTGAResource();
};

#endif