#ifndef __FLOOP_ALLOCATOR_H__
#define __FLOOP_ALLOCATOR_H__


#include "..\types.h"

//�������� ��������� �������� ����� ������ � ������ ���� �� ������� ������ ��� ��������� ������ ��� �������� ���� ��������� ������� ���������� �����
class FLoopAllocator final
{
	UI8 * lpData;
	UI32 iMaxSize, iCurAlloc;

	FLoopAllocator( UI32 iMaxSize );
	FLoopAllocator( const FLoopAllocator & ) {}
	FLoopAllocator & operator = ( const FLoopAllocator & ) {}


public:
	~FLoopAllocator();

	void * AllocBlock( UI32 iSize );//�������� ���� ������ � ������ ���� ��� ����� ������������ ������ ������������ ��������� �� NULL
	void * AllocBlockThrow( UI32 iSize );//�������� ���� ������ � ������ ���� ��� ����� ������������ ������ ������������ ���������� ���� FException

	static void Init();
	static void Destroy();
	static FLoopAllocator * GetInstance();
};

//������� ��� ��������� ��������� ������
#define ALLOC_LOOP( iSize ) FLoopAllocator::GetInstance()->AllocBlock( iSize )
#define ALLOC_LOOPT( iSize ) FLoopAllocator::GetInstance()->AllocBlockThrow( iSize )

#endif