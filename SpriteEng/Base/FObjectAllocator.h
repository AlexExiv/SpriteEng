#ifndef __FOBJECT_ALLOCATOR_H__
#define __FOBJECT_ALLOCATOR_H__


#include "..\Core\FString.h"
#include "FObject.h"


//����� ��� ��������� ������ ��� �������, ������ ���������� ������� ��������� �������, ������� ������������� ������� ������� � ���� ���������
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
	FNode * lpFreeList;//������ ��������� ����������
	FObject * lpOwner;

	FObjectAllocator( UI32 iReservedSize, FObject * lpOwner );
	FObjectAllocator( const FObjectAllocator & ) ;
	FObjectAllocator & operator = ( const FObjectAllocator & ) ;
	~FObjectAllocator();

public:

	////
	FObject * AllocObject( const FString & sObjName, const CHAR_ * lpCallTypes, va_list lpArgs );
	FObject * AllocObjectT( const FString & sObjName, const CHAR_ * lpCallTypes, va_list lpArgs );//��������� ������ ��� ����� ���������� ����������
	////
	FObject * AllocObject( const FString & sObjName, const CHAR_ * lpCallTypes, ... );
	FObject * AllocObjectT( const FString & sObjName, const CHAR_ * lpCallTypes, ... );//��������� ������ ��� ����� ���������� ����������
	void DeleteObject( FObject * lpObject );
};

#endif