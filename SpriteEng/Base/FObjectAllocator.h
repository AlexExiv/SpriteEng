#ifndef __FOBJECT_ALLOCATOR_H__
#define __FOBJECT_ALLOCATOR_H__


#include "..\Core\FString.h"
#include "FObject.h"


//класс для выделения памяти под объекты, память выделяется блоками одинакого размера, равными максимальному размеру объекта в мета менеджере
class FObjectAllocator : public FObject
{
	friend class FObject;
public:
	enum
	{
		OBJECT_BLOCK_COUNT = 32
	};
protected:
	struct FNode
	{
		FNode * lpNext;
		void * lpObjData;
	};

	FBYTE * lpData[OBJECT_BLOCK_COUNT];
	UI32 iObjReserved, iLastBlock, iCurAlloc, iRealCount;
	FNode * lpFreeList;//список свободных эелементов
	FObject * lpOwner;

	FObjectAllocator( UI32 iReservedSize, FObject * lpOwner );
	FObjectAllocator( const FObjectAllocator & ) ;
	FObjectAllocator & operator = ( const FObjectAllocator & ) ;
	~FObjectAllocator();

public:

	////
	FObject * AllocObject( const FString & sObjName, FObject * lpFirst, va_list lpArgs );
	FObject * AllocObjectT( const FString & sObjName, FObject * lpFirst, va_list lpArgs );//выделение памяти под объек генерирует исключение
	////
	FObject * AllocObject( const FString & sObjName, FObject * lpFirst, ... );
	FObject * AllocObjectT( const FString & sObjName, FObject * lpFirst, ... );//выделение памяти под объек генерирует исключение
	void DeleteObject( FObject * lpObject );
};

#endif