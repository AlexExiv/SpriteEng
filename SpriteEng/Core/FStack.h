#ifndef __FSTACK_H__
#define __FSTACK_H__

#include "..\types.h"



class FStack final
{
	UI8 * lpData;
	UI32 iElementSizes[MAX_STACK_ELEMENTS];
	UI32 iStackSize, iAllocatedSize, iLastElement;

	FStack( UI32 iStackSize );
	FStack( const FStack & ) {}
	FStack & operator = ( const FStack & ) {}

public:
	~FStack();

	static void Init();
	static void Deinit();
	static FStack * GetInstanceStack();

	void * PushBlock( UI32 iSize );//выделяет блок памяти в случае переполнения стека возвращается указатель на NULL
	void * PushBlockThrow( UI32 iSize );//выделяет блок памяти в случае переполнения стека генерируется исключение типа FException
	void PopBlock();//освободить последний выделеный блок в стеке
	void CheckStack();
};


#define PUSH_BLOCK( iSize ) FStack::GetInstanceStack()->PushBlock( iSize )
#define PUSH_BLOCKT( iSize ) FStack::GetInstanceStack()->PushBlockThrow( iSize )
#define POP_BLOCK FStack::GetInstanceStack()->PopBlock()
#define CHECK_STACK FStack::GetInstanceStack()->CheckStack()

#endif