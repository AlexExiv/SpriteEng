#include "FObjectAllocator.h"
#include "FMetaData.h"
#include "..\Core\FException.h"
#include <new.h>



FObjectAllocator::FObjectAllocator( UI32 iReservedSize, FObject * lpOwner ) : iObjReserved( iReservedSize ), iCurAlloc( 0 ), 
	iRealCount( 0 ), lpFreeList( NULL ), iLastBlock( 0 ), lpOwner( lpOwner )
{
	for( UI32 i = 0;i < FObjectAllocator::OBJECT_BLOCK_COUNT;i++ )
		lpData[i] = NULL;
	lpData[0] = (FBYTE *)FMalloc( FObjectMetaManager::GetMetaManager()->GetMaxObjSize()*iObjReserved );
}

FObjectAllocator::~FObjectAllocator()
{
	AssertFatal( iRealCount == 0, "Real allocated object in FObjectAllocator is not zero, may be leaks?" );
	UI32 i = 0;
	while( lpData[i] )
	{
		FFree( lpData[i] );
		lpData[i] = NULL;
	}
}

FObject * FObjectAllocator::AllocObject( const FString & sObjName, const CHAR_ * lpCallTypes, va_list lpArgs )
{
	void * lpObjData = NULL;
	if( lpFreeList )
	{
		lpObjData = lpFreeList->lpObjData;
		lpFreeList = lpFreeList->lpNext;
	}
	else
	{
		if( (iCurAlloc + 1) == iObjReserved )
		{
			iLastBlock++;
			if( iLastBlock == FObjectAllocator::OBJECT_BLOCK_COUNT )
			{
				FLog::PutError( "Maximum allocated objects in FObjectAllocator is insufficient" );
				return NULL;
			}
			iCurAlloc = 0;
			lpData[iLastBlock] = (FBYTE *)FMalloc( FObjectMetaManager::GetMetaManager()->GetMaxObjSize()*iObjReserved );
		}

		FNode * lpNode = (FNode *)(lpData[iLastBlock] + iCurAlloc*(sizeof( FNode ) + FObjectMetaManager::GetMetaManager()->GetMaxObjSize()));
		lpNode->lpObjData = (void *)(lpData[iLastBlock] + iCurAlloc*(sizeof( FNode ) + FObjectMetaManager::GetMetaManager()->GetMaxObjSize()) + sizeof( FNode ));
		lpObjData = lpNode->lpObjData;
		iCurAlloc++;
	}
	iRealCount++;
	FObjectMetaData * lpMeta = META( sObjName );

	FObject * lpTmpObj = new (lpObjData ) FObject();
	lpTmpObj->SetAllocator( this );
	FObject * lpObject = lpMeta->CreateObject( lpObjData, lpCallTypes, lpArgs );
	if( !lpObject )
	{
		FLog::PutError( "Can't create \"%s\" object", sObjName.GetChar() );
		return NULL;
	}
	return lpObject;
}

FObject * FObjectAllocator::AllocObjectT( const FString & sObjName, const CHAR_ * lpCallTypes, va_list lpArgs )
{
	void * lpObjData = NULL;
	if( lpFreeList )
	{
		lpObjData = lpFreeList->lpObjData;
		lpFreeList = lpFreeList->lpNext;
	}
	else
	{
		if( (iCurAlloc + 1) == iObjReserved )
		{
			iCurAlloc = 0;
			iLastBlock++;
			if( iLastBlock == FObjectAllocator::OBJECT_BLOCK_COUNT )
				throw FException( FException::EXCP_OBJALLOC_OVERHEAD, "Maximum allocated objects in FObjectAllocator is insufficient" );
			lpData[iLastBlock] = (FBYTE *)FMalloc( FObjectMetaManager::GetMetaManager()->GetMaxObjSize()*iObjReserved );
		}

		FNode * lpNode = (FNode *)(lpData[iLastBlock] + iCurAlloc*(sizeof( FNode ) + FObjectMetaManager::GetMetaManager()->GetMaxObjSize()));
		lpNode->lpObjData = (void *)(lpData[iLastBlock] + iCurAlloc*(sizeof( FNode ) + FObjectMetaManager::GetMetaManager()->GetMaxObjSize()) + sizeof( FNode ));
		lpObjData = lpNode->lpObjData;
		iCurAlloc++;
	}
	iRealCount++;
	FObjectMetaData * lpMeta = META( sObjName );

	FObject * lpObject = lpMeta->CreateObject( lpObjData, lpCallTypes, lpArgs );
	if( !lpObject )
		throw FException( FException::EXCP_FATAL_ERROR, FString::PrintString( "Can't create \"%s\" object", sObjName.GetChar() ) );
	lpObject->SetAllocator( this );
	return lpObject;
}

FObject * FObjectAllocator::AllocObject( const FString & sObjName, const CHAR_ * lpCallTypes, ... )
{
	void * lpObjData = NULL;
	if( lpFreeList )
	{
		lpObjData = lpFreeList->lpObjData;
		lpFreeList = lpFreeList->lpNext;
	}
	else
	{
		if( (iCurAlloc + 1) == iObjReserved )
		{
			iLastBlock++;
			if( iLastBlock == FObjectAllocator::OBJECT_BLOCK_COUNT )
			{
				FLog::PutError( "Maximum allocated objects in FObjectAllocator is insufficient" );
				return NULL;
			}
			iCurAlloc = 0;
			lpData[iLastBlock] = (FBYTE *)FMalloc( FObjectMetaManager::GetMetaManager()->GetMaxObjSize()*iObjReserved );
		}

		FNode * lpNode = (FNode *)(lpData[iLastBlock] + iCurAlloc*(sizeof( FNode ) + FObjectMetaManager::GetMetaManager()->GetMaxObjSize()));
		lpNode->lpObjData = (void *)(lpData[iLastBlock] + iCurAlloc*(sizeof( FNode ) + FObjectMetaManager::GetMetaManager()->GetMaxObjSize()) + sizeof( FNode ));
		lpObjData = lpNode->lpObjData;
		iCurAlloc++;
	}
	iRealCount++;
	FObjectMetaData * lpMeta = META( sObjName );
	va_list lpArgs;
	va_start( lpArgs, lpCallTypes );

	FObject * lpObject = lpMeta->CreateObject( lpObjData, lpCallTypes, lpArgs );
	va_end( lpArgs );

	if( !lpObject )
	{
		FLog::PutError( "Can't create \"%s\" object", sObjName.GetChar() );
		return NULL;
	}
	lpObject->SetAllocator( this );
	return lpObject;
}

FObject * FObjectAllocator::AllocObjectT( const FString & sObjName, const CHAR_ * lpCallTypes, ... )
{
	void * lpObjData = NULL;
	if( lpFreeList )
	{
		lpObjData = lpFreeList->lpObjData;
		lpFreeList = lpFreeList->lpNext;
	}
	else
	{
		if( (iCurAlloc + 1) == iObjReserved )
		{
			iCurAlloc = 0;
			iLastBlock++;
			if( iLastBlock == FObjectAllocator::OBJECT_BLOCK_COUNT )
				throw FException( FException::EXCP_OBJALLOC_OVERHEAD, "Maximum allocated objects in FObjectAllocator is insufficient" );
			lpData[iLastBlock] = (FBYTE *)FMalloc( FObjectMetaManager::GetMetaManager()->GetMaxObjSize()*iObjReserved );
		}

		FNode * lpNode = (FNode *)(lpData[iLastBlock] + iCurAlloc*(sizeof( FNode ) + FObjectMetaManager::GetMetaManager()->GetMaxObjSize()));
		lpNode->lpObjData = (void *)(lpData[iLastBlock] + iCurAlloc*(sizeof( FNode ) + FObjectMetaManager::GetMetaManager()->GetMaxObjSize()) + sizeof( FNode ));
		lpObjData = lpNode->lpObjData;
		iCurAlloc++;
	}
	iRealCount++;
	FObjectMetaData * lpMeta = META( sObjName );
	va_list lpArgs;
	va_start( lpArgs, lpCallTypes );

	FObject * lpObject = lpMeta->CreateObject( lpObjData, lpCallTypes, lpArgs );
	va_end( lpArgs );
	if( !lpObject )
		throw FException( FException::EXCP_FATAL_ERROR, FString::PrintString( "Can't create \"%s\" object", sObjName.GetChar() ) );
	lpObject->SetAllocator( this );
	return lpObject;
}

void FObjectAllocator::DeleteObject( FObject * lpObject )
{
	lpObject->~FObject();
	FNode * lpNode = (FNode *)(((FBYTE *)lpObject) - sizeof( FNode ));
	lpNode->lpNext = lpFreeList;
	lpFreeList = lpNode;
	iRealCount--;
}
