#ifndef __FLOOP_ALLOCATOR_H__
#define __FLOOP_ALLOCATOR_H__


#include "..\types.h"

// руговой аллокатор выдел€ет блоки пам€ти в случае если не хватает пам€ти дл€ выделени€ пам€ти дл€ текущего блок аллокатор стирает предыдущие блоки
class FLoopAllocator final
{
	UI8 * lpData;
	UI32 iMaxSize, iCurAlloc;

	FLoopAllocator( UI32 iMaxSize );
	FLoopAllocator( const FLoopAllocator & ) {}
	FLoopAllocator & operator = ( const FLoopAllocator & ) {}


public:
	~FLoopAllocator();

	void * AllocBlock( UI32 iSize );//выдел€ет блок пам€ти в случае если дл€ блока недостаточно пам€ти возвращаетс€ указатель на NULL
	void * AllocBlockThrow( UI32 iSize );//выдел€ет блок пам€ти в случае если дл€ блока недостаточно пам€ти генерируетс€ исключение типа FException

	static void Init();
	static void Destroy();
	static FLoopAllocator * GetInstance();
};

//макросы дл€ упрощени€ выделени€ пам€ти
#define ALLOC_LOOP( iSize ) FLoopAllocator::GetInstance()->AllocBlock( iSize )
#define ALLOC_LOOPT( iSize ) FLoopAllocator::GetInstance()->AllocBlockThrow( iSize )

#endif