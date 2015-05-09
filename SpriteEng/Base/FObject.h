#ifndef __FOBJECT_H__
#define __FOBJECT_H__

#include "..\Core\FString.h"

class FObject;

class FObjectAllocator;

class FObject
{
	friend class FObjectAllocator;

	enum
	{

	};

	UI32 iRefCount;
	FObjectAllocator * lpAllocator;
	UI32 iObjFlags;

	void SetAllocator( FObjectAllocator * lpAllocator );
	
protected:
	FObject * AllocObject( const FString & sObjName, const CHAR_ * lpCallTypes, ... );
	FObject * AllocObjectT( const FString & sObjName, const CHAR_ * lpCallTypes, ... );//выделение памяти под объек генерирует исключение
	void Delete( FObject * lpObject );

	bool CheckCallTypes( const CHAR_ * lpCallTypes, const CHAR_ * lpObjTypes )const;
	FObject();
	FObject( UI32 iObjReserved );

public:
	virtual ~FObject();

	virtual void Update( F32 fDTime );

	void AddRef();
	void Release();

	virtual FObject * CreateObject( void * lpPlacement, const CHAR_ * lpTypes, va_list lpArgs ) ;
};


#ifdef _DEBUG
#define CHECK_OBJ_TYPES( lpCallTypes, lpObjTypes ) if( !FObject::CheckCallTypes( lpCallTypes, lpObjTypes ) ) return NULL;
#else
#define CHECK_OBJ_TYPES( lpCallTypes, lpObjTypes )
#endif


#endif