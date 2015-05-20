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

//void FObjectMetaData::RegisterConstructor( CHAR_ * lpTypes, FObjectConstructor fConstructor )
//{
//	FConstructor * lpConstr = new FConstructor;
//	lpConstr->lpTypes = lpTypes;
//	lpConstr->fConstruct = fConstructor;
//	lpConstr->lpNext = lpConstrList;
//	lpConstrList = lpConstr;
//}

FObjectMetaData::FConstructor * FObjectMetaData::FindConstructorByObj( FObject * lpFirst, va_list lpArgs )
{
	FConstructor * lpConstr = lpConstrList;
	while( lpConstr )
	{
		UI32 i = 0;
		va_list lpArgs_ = lpArgs;
		FObject * lpObj = lpFirst;
		for(;i < lpConstr->iArgCount;i++ )
		{
			if( !lpObj )
				break;

			if( lpConstr->sArguments[i] != lpObj->GetObjName() )
			{
				FObjectMetaData * lpSuperMeta = META( lpConstr->sArguments[i] ), 
					* lpMeta = META( lpObj->GetObjName() );
				if( !lpMeta->IsKindOfClass( *lpSuperMeta ) )
					break;
			}
			lpObj = va_arg( lpArgs_, FObject * );
		}
		if( i == lpConstr->iArgCount )
			break;

		lpConstr = lpConstr->lpNext;
	}

	return lpConstr;
}

FObjectMetaData::FConstructor * FObjectMetaData::FindConstructorByStr(  va_list lpArgs )
{
	FConstructor * lpConstr = lpConstrList;
	while( lpConstr )
	{
		UI32 i = 0;
		va_list lpArgs_ = lpArgs;
		for(;i < lpConstr->iArgCount;i++ )
		{
			FString * lpArgName = va_arg( lpArgs_, FString * );
			if( lpConstr->sArguments[i] != *lpArgName )
				break;
		}
		if( i == lpConstr->iArgCount )
			break;

		lpConstr = lpConstr->lpNext;
	}

	return lpConstr;
}

void FObjectMetaData::RegisterConstructor( FObjectConstructor fConstructor, ... )
{
	va_list lpArgs;
	va_start( lpArgs, fConstructor );

	FConstructor * lpConstr = FindConstructorByStr( lpArgs );
	va_end( lpArgs );
	va_start( lpArgs, fConstructor );
#ifdef _DEBUG
	FString sArguments( "" );
	bool bIsFirst = true;

	while( *lpArgs != NULL )
	{
		FString * lpArgName = va_arg( lpArgs, FString * );
		if( bIsFirst )
		{
			sArguments += *lpArgName;
			bIsFirst = false;
		}
		else
			sArguments += (FString( ", " ) + *lpArgName);
	}
	va_end( lpArgs );
	if( lpConstr )
	{
		FLog::PutError( "Class %s constructor redefinition ( %s )", sClassName.GetChar(), sArguments.GetChar() );
		return;
	}

	va_start( lpArgs, fConstructor );
#else
	if( lpConstr )
	{
		FString sArguments;
		bool bIsFirst = true;

		while( *lpArgs != NULL )
		{
			FString * lpArgName = va_arg( lpArgs, FString * );
			if( bIsFirst )
			{
				sArguments += *lpArgName;
				bIsFirst = false;
			}
			else
				sArguments += (FString( " ," ) + *lpArgName);
		}
		va_end( lpArgs );

		FLog::PutError( "Class %s constructor redefinition ( %s )", sClassName.GetChar(), sArguments.GetChar() );
		return;
	}
#endif

	lpConstr = new FConstructor;

	UI32 iArgCount = 0;
	while( *lpArgs != NULL )
	{
		FString * lpArgName = va_arg( lpArgs, FString * );
		lpConstr->sArguments[iArgCount++] = *lpArgName;
	}

	va_end( lpArgs );

	lpConstr->fConstruct = fConstructor;
	lpConstr->iArgCount = iArgCount;
	lpConstr->lpNext = lpConstrList;
	lpConstrList = lpConstr;

	FLog::PutMessage( "Register constructor %s( %s )", sClassName.GetChar(), sArguments.GetChar() );
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

FObject * FObjectMetaData::CreateObject( void * lpPlacement, FObject * lpFirst, va_list lpArgs )
{
	if( !lpConstrList )
	{
		FLog::PutError( "\"%s\" no one constructor are avaible", sClassName.GetChar() );
		return NULL;
	}

	FConstructor * lpConstr = FindConstructorByObj( lpFirst, lpArgs );
	if( lpConstr )
	{
		va_parg( lpArgs, FObject * );
		return lpConstr->fConstruct( lpPlacement, lpArgs );
	}

	FString sArguments;
	bool bIsFirst = true;
	FObject * lpObj = lpFirst;

	while( lpObj != NULL )
	{
		if( bIsFirst )
		{
			sArguments += lpObj->GetObjName();
			bIsFirst = false;
		}
		else
			sArguments += (FString( " ," ) + lpObj->GetObjName());

		lpObj = va_arg( lpArgs, FObject * );
	}

	FLog::PutError( "Class %s no have constructor with that arg list ( %s )", sClassName.GetChar(), sArguments.GetChar() );
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

