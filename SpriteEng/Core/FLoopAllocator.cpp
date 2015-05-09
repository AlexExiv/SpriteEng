#include "FLoopAllocator.h"
#include "FLog.h"
#include "FException.h"
#include "FString.h"



static FLoopAllocator * lpLoopAlloc = NULL;


FLoopAllocator::FLoopAllocator( UI32 iMaxSize ) : iMaxSize( iMaxSize ), iCurAlloc( 0 ), lpData( NULL )
{
	lpData = (UI8 *)FMalloc( iMaxSize );
}

FLoopAllocator::~FLoopAllocator()
{
	if( lpData )
		FFree( lpData );
	lpData = NULL;
}

void * FLoopAllocator::AllocBlock( UI32 iSize )
{
	AssertFatal( lpData, "NULL pointer in loop allocator" );
	if( iSize >= iMaxSize )
	{
		FLog::PutError( "Very big memory block need to alloc: %i", iSize );
		return NULL;
	}
	if( (iCurAlloc + iSize) >= iMaxSize )
		iCurAlloc = 0;

	UI8 * lpRet = lpData + iCurAlloc;
	iCurAlloc += iSize;

	return lpRet;
}

void * FLoopAllocator::AllocBlockThrow( UI32 iSize )
{
	AssertFatal( lpData, "NULL pointer in loop allocator" );
	if( iSize >= iMaxSize )
		throw FException( FException::EXCP_LOOP_ALLOC_NEEDMORE, FString::PrintString( "Very big memory block need to alloc: %i", iSize ) );

	if( (iCurAlloc + iSize) >= iMaxSize )
		iCurAlloc = 0;

	UI8 * lpRet = lpData + iCurAlloc;
	iCurAlloc += iSize;

	return lpRet;
}

void FLoopAllocator::Init()
{
	lpLoopAlloc = new FLoopAllocator( MAX_LOOPALLOC_SIZE );
}

void FLoopAllocator::Destroy()
{
	if( lpLoopAlloc )
		delete lpLoopAlloc;
}

FLoopAllocator * FLoopAllocator::GetInstance()
{
	if( lpLoopAlloc )
		FLoopAllocator::Init();
	return lpLoopAlloc;
}