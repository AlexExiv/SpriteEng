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
public:
	enum
	{
		MAX_CONSTRUCTOR_ARG = 16
	};

private:
	struct FConstructor
	{
		FConstructor * lpNext;
		FString sArguments[MAX_CONSTRUCTOR_ARG];
		UI32 iArgCount;
		FObjectConstructor fConstruct;
	};


	FString sClassName;
	UI32 iClassSize;
	FObjectMetaData * lpSuperClass;
	FConstructor * lpConstrList;

	FConstructor * FindConstructorByStr( va_list lpArgs );
	FConstructor * FindConstructorByObj( FObject * lpFirst, va_list lpArgs );

public:
	FObjectMetaData( const FString & sClassName, UI32 iSize );
	FObjectMetaData( const FString & sClassName, const FString & sSuperClass , UI32 iSize );
	~FObjectMetaData();

	void Init();
	UI32 GetSize()const;
	const FString & GetName()const;
	//void RegisterConstructor( CHAR_ * lpTypes, FObjectConstructor fConstructor );
	void RegisterConstructor( FObjectConstructor fConstructor, ... );
	bool IsKindOfClass( const FObjectMetaData & mMeta )const;
	bool IsInstanceOfClass( const FObjectMetaData & mMeta )const;
	FObject * CreateObject( void * lpPlacement, FObject * lpFirst, va_list lpArgs );
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
//#define DEFINE_META_CONSTR( TYPE, ARG_LIST, FUNCTOR ) mMeta##TYPE.RegisterConstructor( ARG_LIST, FUNCTOR )

#define DEFINE_META_CONSTR0( TYPE, FUNCTOR ) mMeta##TYPE.RegisterConstructor( FUNCTOR, NULL )
#define DEFINE_META_CONSTR1( TYPE, FUNCTOR, ARG0 ) mMeta##TYPE.RegisterConstructor( FUNCTOR, &GET_OBJNAME( ARG0 ), NULL )
#define DEFINE_META_CONSTR2( TYPE, FUNCTOR, ARG0, ARG1 ) mMeta##TYPE.RegisterConstructor( FUNCTOR, &GET_OBJNAME( ARG0 ), &GET_OBJNAME( ARG1 ), NULL )
#define DEFINE_META_CONSTR3( TYPE, FUNCTOR, ARG0, ARG1, ARG2 )\
	mMeta##TYPE.RegisterConstructor( FUNCTOR, &GET_OBJNAME( ARG0 ), &GET_OBJNAME( ARG1 ), &GET_OBJNAME( ARG2 ), NULL )
#define DEFINE_META_CONSTR4( TYPE, FUNCTOR, ARG0, ARG1, ARG2, ARG3 )\
	mMeta##TYPE.RegisterConstructor( FUNCTOR, &GET_OBJNAME( ARG0 ), &GET_OBJNAME( ARG1 ), &GET_OBJNAME( ARG2 ), &GET_OBJNAME( ARG3 ), NULL )
#define DEFINE_META_CONSTR5( TYPE, FUNCTOR, ARG0, ARG1, ARG2, ARG3, ARG4 )\
	mMeta##TYPE.RegisterConstructor( FUNCTOR, &GET_OBJNAME( ARG0 ), &GET_OBJNAME( ARG1 ), &GET_OBJNAME( ARG2 ), &GET_OBJNAME( ARG3 ), &GET_OBJNAME( ARG4), NULL )
#define DEFINE_META_CONSTR6( TYPE, FUNCTOR, ARG0, ARG1, ARG2, ARG3, ARG4, ARG5 )\
	mMeta##TYPE.RegisterConstructor( FUNCTOR, &GET_OBJNAME( ARG0 ), &GET_OBJNAME( ARG1 ), &GET_OBJNAME( ARG2 ), &GET_OBJNAME( ARG3 ), &GET_OBJNAME( ARG4 ), &GET_OBJNAME( ARG5 ), NULL )
#define DEFINE_META_CONSTR7( TYPE, FUNCTOR, ARG0, ARG1, ARG2, ARG3, ARG4, ARG5, ARG6 )\
	mMeta##TYPE.RegisterConstructor( FUNCTOR, &GET_OBJNAME( ARG0 ), &GET_OBJNAME( ARG1 ), &GET_OBJNAME( ARG2 ), &GET_OBJNAME( ARG3 ), &GET_OBJNAME( ARG4 ), &GET_OBJNAME( ARG5 ), &GET_OBJNAME( ARG6 ), NULL )



#define META( STR ) FObjectMetaManager::GetMetaManager()->FindMetaData( STR )

#endif