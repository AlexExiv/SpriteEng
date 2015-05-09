#ifndef __FMETA_DATA_H__
#define __FMETA_DATA_H__


#include "..\Core\FString.h"
#include "..\Core\FDictionary.h"
#include <stdarg.h>


class FObject;
class FObjectList;
class FWorld;

class FObjectMetaData
{
	FString sClassName;
	UI32 iClassSize;
	FObjectMetaData * lpSuperClass;
	FObject * lpObjInst;

public:
	FObjectMetaData( FObject * lpObjInst, const FString & sClassName, UI32 iSize );
	FObjectMetaData( FObject * lpObjInst, const FString & sSuperClass, const FString & sClassName, UI32 iSize );
	~FObjectMetaData();

	void Init();
	UI32 GetSize()const;
	const FString & GetName()const;
	FObject * CreateObject( void * lpPlacement, FObjectList * lpList, const CHAR_ * lpFormat, va_list lpArgs );
	FObject * CreateObject( void * lpPlacement, FWorld * lpWorld, const CHAR_ * lpFormat, ... );
};


class FObjectMetaManager
{
	typedef FDictionary<FObjectMetaData *> FMetaMap;
	FMetaMap mMetaMap;
	UI32 iMaxObjSize;

public:
	FObjectMetaManager();
	~FObjectMetaManager();

	static FObjectMetaManager * GetMetaManager();

	void RegisterMetaData( FObjectMetaData * lpMetaData );
	FObjectMetaData * FindMetaData( const FString & sName );
	UI32 GetMaxObjSize()const;
};


#define DEFINE_META( TYPE ) static TYPE oObjectInst##TYPE;\
							static FObjectMetaData mMeta##TYPE( &oObjectInst##TYPE, #TYPE, sizeof( TYPE ) )

#define META( STR ) FObjectMetaManager::GetMetaManager()->FindMetaData( STR )

#endif