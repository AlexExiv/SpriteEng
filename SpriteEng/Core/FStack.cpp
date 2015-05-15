#include "FStack.h"
#include "FLog.h"
#include "FException.h"


static FStack * lpInstStack = NULL;


FStack::FStack( UI32 iStackSize ) : iStackSize( iStackSize ), lpData( NULL ), iAllocatedSize( 0 ), iLastElement( 0 ) 
{
	lpData = (UI8 *)FMalloc( iStackSize );
}

FStack::~FStack()
{
	if( lpData )
		FFree( lpData );
}

void * FStack::PushBlock( UI32 iSize )
{
	if( (iLastElement == MAX_STACK_ELEMENTS) || ((iSize + iAllocatedSize) >= iStackSize) )
	{
		FLog::PutError( "Stack overflow need: %i", iSize + iAllocatedSize );
		return NULL;
	}

	void * lpRet = (void *)(lpData + iAllocatedSize);
	iElementSizes[iLastElement++] = iSize;
	iAllocatedSize += iSize;

	return lpRet;
}

void * FStack::PushBlockThrow( UI32 iSize )
{
	if( (iLastElement == MAX_STACK_ELEMENTS) || ((iSize + iAllocatedSize) >= iStackSize) )
		throw FException( FException::EXCP_STACK_OVERFLOW, FString::PrintString( "Stack overflow need: %i", iSize + iAllocatedSize ) );

	void * lpRet = (void *)(lpData + iAllocatedSize);
	iElementSizes[iLastElement++] = iSize;
	iAllocatedSize += iSize;

	return lpRet;
}

void FStack::PopBlock()
{
	if( iLastElement == 0 )
		return;
	
	iAllocatedSize -= iElementSizes[--iLastElement];
}

void FStack::CheckStack()
{
	if( iAllocatedSize != 0 )
		FLog::PutError( "Stack is not empty, may be leaks, allocated size: %i", iAllocatedSize );
}

void FStack::Init()
{
	lpInstStack = new FStack( MAX_STACK_SIZE );
}

void FStack::Deinit()
{
	if( lpInstStack )
		delete lpInstStack;
	lpInstStack = NULL;
}

FStack * FStack::GetInstanceStack()
{
	if( !lpInstStack )
		lpInstStack = new FStack( MAX_STACK_SIZE );
	return lpInstStack;
}
