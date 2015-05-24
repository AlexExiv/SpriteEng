#include "FResource.h"
#include "FResourceManager.h"
#include "FBMPResource.h"
#include "FTGAResource.h"
#include "FJPEGResource.h"
#include "FPNGResource.h"
#include "FAtlasResource.h"
#include "FGLSLResource.h"
#include "FFile.h"
#include "FLog.h"
#include "FStack.h"
#include "FException.h"


FResourceManager * FResourceManager::lpManager = NULL;

FResourceManager::FResourceManager( UI32 iResAllocSize ) : iResAllocSize( iResAllocSize ), iCurAlloc( 0 ), lpData( NULL ), iMaxResSize( 0 ),
	iAllocCount( 0 ), lpResData( NULL )
{
	RegisterResource( new FBMPResource() );
	RegisterResource( new FTGAResource() );
	RegisterResource( new FJPEGResource() );
	RegisterResource( new FPNGResource() );
	RegisterResource( new FAtlasResource() );
	RegisterResource( new FGLSLResource() );

	lpData = (FBYTE *)FMalloc( iResAllocSize );
	lpResData = (FBYTE *)FMalloc( iMaxResSize*RESOURCE_COUNT );
	AssertFatal( lpData, "Can't allocate memory for resources" );
}

void FResourceManager::RegisterResource( FResource * lpResource )
{
	FResourceIterator iIt = lResources.Begin();
	for(;iIt != lResources.End();iIt++ )
	{
		if( (*iIt)->IsExtEqual( lpResource->sExtStr ) )
			return;
	}
	iMaxResSize = max( iMaxResSize, lpResource->GetSize() );
	lResources.PushBack( lpResource );
}

void * FResourceManager::AllocRes()
{
	UI32 iAllocInd = iAllocCount%RESOURCE_COUNT;
	if( iAllocCount >= RESOURCE_COUNT )
		((FResource *)(lpResData + iAllocInd*iMaxResSize))->~FResource();

	void * lpRet = lpResData + iAllocInd*iMaxResSize;
	iAllocCount++;
	return lpRet;
}

void FResourceManager::DestructAllRes()
{
	for( UI32 i = 0;i < iAllocCount;i++ )
		((FResource *)(lpResData + i*iMaxResSize))->~FResource();
}

FResourceManager::~FResourceManager()
{
	DestructAllRes();

	if( lpData )
		FFree( lpData );
	if( lpResData )
		FFree( lpResData );

	FResourceIterator iIt = lResources.Begin();
	for(;iIt != lResources.End();iIt++ )
		delete *iIt;
}

FResourceManager * FResourceManager::SharedManager()
{
	if( !lpManager )
		lpManager = new FResourceManager( FResourceManager::RESOURCE_ALLOC_SIZE );
	return lpManager;
}

FResource * FResourceManager::CreateResource( const FString & sFileName )
{
	FFile * lpFile = FFile::OpenFile( sFileName, FFile::F_FILE_READ );
	if( !lpFile )
	{
		//FLog::PutError( "Can't find file with name: %s", sFileName.GetChar() );
		return NULL;
	}


	FResource * lpRes = NULL;
	FResourceIterator iIt = lResources.Begin();
	FString sFileExt = sFileName.GetExt();

	try
	{
		void * lpData_ = AllocForResource( lpFile->GetSize() );
		lpFile->GetData( lpData_ );
		for(;iIt != lResources.End();iIt++ )
		{
			if( iIt->IsExtEqual( sFileExt ) )
			{
				lpRes = iIt->Make( AllocRes(), lpData_, lpFile->GetSize(), this );
				break;
			}
		}

		if( iIt == lResources.End() )
			FLog::PutError( "Unknown file type with name: \"%s\"", sFileName.GetChar() );
	}
	catch( FException eExcp )
	{
		switch( eExcp.GetCode() )
		{
		case FException::EXCP_UNK_FILE_FORMAT:
		case FException::EXCP_UNK_IMAGE_FORMAT:
		case FException::EXCP_RESALLOC_INSUFFIENC:
			FLog::PutError( "%s, name of resource \"%s\"", eExcp.GetMess().GetChar(), sFileName.GetChar() );
			break;
		case FException::EXCP_FATAL_ERROR:
			throw eExcp;
			break;
		default:
			FLog::PutError( eExcp.GetMess() );
		};

		//if( lpRes )
		//	delete lpRes;
		//lpRes = NULL;
	}

	FFile::CloseFile( lpFile );

	return lpRes;
}

void FResourceManager::SaveResource( const FString & sFileName, FResource * lpRes )
{
	FString sFullName = sFileName + "." + lpRes->sExtStr;
	void * lpBuffer;
	UI32 iDataSize;
	lpRes->SaveResource( &lpBuffer, iDataSize );

	FFile * lpFile = FFile::OpenFile( sFullName, FFile::F_FILE_CREATE | FFile::F_FILE_WRITE );
	if( lpFile )
		lpFile->Write( lpBuffer, iDataSize );
	else
		FLog::PutError( "Can't create file \"%s\"", sFullName.GetChar() );

	FFile::CloseFile( lpFile );
	FFree( lpBuffer );
}

void FResourceManager::SaveResource( void ** lpBuffer, UI32 & iWritedLen, FResource * lpRes )
{
	AssertFatal( lpBuffer, "NULL pointer" );
	lpRes->SaveResource( lpBuffer, iWritedLen );
}

void * FResourceManager::AllocForResource( UI32 iSize )
{
	if( iSize > iResAllocSize )
		throw FException( FException::EXCP_RESALLOC_INSUFFIENC, FString::PrintString( "Insuffience memory for resource allocator, need: %i", iSize ) );

	if( iSize >= (iResAllocSize/4) )
		FLog::PutWarning( "Allocated resource size(%i byte) is more then 25\% from all resource heap size, loaded data might be overlapped", iSize );

	if( (iCurAlloc + iSize) >= iResAllocSize )
		iCurAlloc = 0;

	FBYTE * lpAlloc = lpData + iCurAlloc;
	iCurAlloc += iSize;

	return lpAlloc;
}

void FResourceManager::ResetAllocator()
{
	iCurAlloc = 0;
}

UI32 FResourceManager::GetAllocSize()const
{
	return iResAllocSize;
}

