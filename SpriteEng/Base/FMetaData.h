#ifndef __FMETA_DATA_H__
#define __FMETA_DATA_H__


#include "..\Core\FString.h"
#include "..\Core\FDictionary.h"
#include <stdarg.h>


class FObject;
class FObjectList;
class FWorld;
typedef FObject * (*FObjectConstructor)( void * lpPlacement, va_list lpArgs );

class FObjectMetaData
{
	struct FConstructor
	{
		FConstructor * lpNext;
		CHAR_ * lpTypes;
		FObjectConstructor fConstruct;
	};


	FString sClassName;
	UI32 iClassSize;
	FObjectMetaData * lpSuperClass;
	FConstructor * lpConstrList;

public:
	FObjectMetaData( const FString & sClassName, UI32 iSize );
	FObjectMetaData( const FString & sClassName, const FString & sSuperClass , UI32 iSize );
	~FObjectMetaData();

	void Init();
	UI32 GetSize()const;
	const FString & GetName()const;
	void RegisterConstructor( CHAR_ * lpTypes, FObjectConstructor fConstructor );
	bool IsKindOfClass( const FObjectMetaData & mMeta )const;
	bool IsInstanceOfClass( const FObjectMetaData & mMeta )const;
	FObject * CreateObject( void * lpPlacement, const CHAR_ * lpCallTypes, va_list lpArgs );
};


class FObjectMetaManager
{
	typedef FDictionary<FObjectMetaData *> FMetaMap;
	typedef FDictionary<FObjectMetaData *>::FRecord FMetaRecord;

	FMetaMap mMetaMap;
	UI32 iMaxObjSize;

	FObjectMetaManager( const FObjectMetaManager & );
	FObjectMetaManager & operator = ( const FObjectMetaManager & );

public:
	FObjectMetaManager();
	~FObjectMetaManager();

	static void Init();
	static void Deinit();
	static FObjectMetaManager * GetMetaManager();

	void RegisterMetaData( FObjectMetaData * lpMetaData );
	FObjectMetaData * FindMetaData( const FString & sName );
	UI32 GetMaxObjSize()const;
};


#define DEFINE_META( TYPE ) static FObjectMetaData mMeta##TYPE( #TYPE, sizeof( TYPE ) )
#define DEFINE_META_SUPER( TYPE, SUPER_TYPE ) static FObjectMetaData mMeta##TYPE( #TYPE, #SUPER_TYPE, sizeof( TYPE ) )
#define DEFINE_META_CONSTR( TYPE, ARG_LIST, FUNCTOR ) mMeta##TYPE.RegisterConstructor( ARG_LIST, FUNCTOR )

#define META( STR ) FObjectMetaManager::GetMetaManager()->FindMetaData( STR )

#endif