#include "FMetaData.h"
#include "..\Core\FLog.h"
#include "FObject.h"
#include <string.h>



static FObjectMetaManager * lpMetaManager = NULL;

FObjectMetaData::FObjectMetaData( const FString & sClassName, UI32 iSize ) :  
	sClassName( sClassName ), iClassSize( iSize ), lpConstrList( NULL )
{
	FObjectMetaManager::GetMetaManager()->RegisterMetaData( this );
}

FObjectMetaData::FObjectMetaData( const FString & sClassName, const FString & sSuperClass, UI32 iSize ) 
	: sClassName( sClassName ), iClassSize( iSize ), lpConstrList( NULL )
{
	lpSuperClass = FObjectMetaManager::GetMetaManager()->FindMetaData( sSuperClass );
	if( !lpSuperClass )
		FLog::PutError( "Super class \"%s\" not found", sSuperClass.GetChar() );
	FObjectMetaManager::GetMetaManager()->RegisterMetaData( this );
}

FObjectMetaData::~FObjectMetaData()
{
	while( lpConstrList )
	{
		FConstructor * lpNext = lpConstrList->lpNext;
		delete lpConstrList;
		lpConstrList = lpNext;
	}
}

UI32 FObjectMetaData::GetSize()const
{
	return iClassSize;
}

const FString & FObjectMetaData::GetName()const
{
	return sClassName;
}

void FObjectMetaData::RegisterConstructor( CHAR_ * lpTypes, FObjectConstructor fConstructor )
{
	FConstructor * lpConstr = new FConstructor;
	lpConstr->lpTypes = lpTypes;
	lpConstr->fConstruct = fConstructor;
	lpConstr->lpNext = lpConstrList;
	lpConstrList = lpConstr;
}

bool FObjectMetaData::IsKindOfClass( const FObjectMetaData & mMeta )const
{
	const FObjectMetaData * lpCurClass = this;
	while( lpCurClass )
	{
		if( lpCurClass->IsInstanceOfClass( mMeta ) )
			return true;
		lpCurClass = lpCurClass->lpSuperClass;
	}

	return false;
}

bool FObjectMetaData::IsInstanceOfClass( const FObjectMetaData & mMeta )const
{
	return sClassName == mMeta.sClassName;
}

FObject * FObjectMetaData::CreateObject( void * lpPlacement, const CHAR_ * lpCallTypes, va_list lpArgs )
{
	if( !lpConstrList )
	{
		FLog::PutError( "\"%s\" no one constructor are avaible", sClassName.GetChar() );
		return NULL;
	}

	FConstructor * lpConstr = lpConstrList;
	while( lpConstr )
	{
		if( strcmp( lpCallTypes, lpConstr->lpTypes ) == 0 )
			return lpConstr->fConstruct( lpPlacement, lpArgs );
		lpConstr = lpConstr->lpNext;
	}

	FLog::PutError( "\"%s\" no have constructor with that arguments list \"%s\"", sClassName.GetChar(), lpCallTypes );
	return NULL;
}

FObjectMetaManager::FObjectMetaManager() : iMaxObjSize( 0 )
{
}

FObjectMetaManager::~FObjectMetaManager()
{
}

void FObjectMetaManager::Init()
{
}

void FObjectMetaManager::Deinit()
{
}

FObjectMetaManager * FObjectMetaManager::GetMetaManager()
{
	if( !lpMetaManager )
		lpMetaManager = new FObjectMetaManager();
	return lpMetaManager;
}

void FObjectMetaManager::RegisterMetaData( FObjectMetaData * lpMetaData )
{
	FMetaRecord rRecord = mMetaMap.FindRecord( lpMetaData->GetName() );
	if( rRecord != FMetaRecord( NULL ) )
	{
		FLog::PutError( "Class \"%s\" is alreadey exist", lpMetaData->GetName().GetChar() );
		return;
	}

	iMaxObjSize = max( iMaxObjSize, lpMetaData->GetSize() );
	mMetaMap.AddRecord( lpMetaData->GetName(), lpMetaData );
}

FObjectMetaData * FObjectMetaManager::FindMetaData( const FString & sName )
{
	return mMetaMap.FindRecord( sName );
}

UI32 FObjectMetaManager::GetMaxObjSize()const
{
	return iMaxObjSize;
}

