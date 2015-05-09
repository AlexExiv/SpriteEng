#ifndef __FRESOURCE_H__
#define __FRESOURCE_H__

#include "FString.h"

class FResourceManager;

class FResource
{
	FString sExtStr, sResClass;
protected:
	friend class FResourceManager;

	FResourceManager * lpCreator;

	void * lpData;
	bool bRemoveData;

	virtual FResource * Make( void * lpData, UI32 iDataLen, FResourceManager * lpCreator ) = 0;
	virtual void SaveResource( void ** lpData, UI32 & iDataSize ) = 0;

	bool IsExtEqual( const FString & sExtStr );
	FResource( const FString & sExtStr, FResourceManager * lpCreator );
	FResource( const FString & sExtStr, const FString & sResClass );

public:
	virtual ~FResource();

	void * GetData()const;
};

#endif