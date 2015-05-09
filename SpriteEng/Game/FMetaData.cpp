#include "FMetaData.h"
#include "..\Core\FLog.h"



FObjectMetaData::FObjectMetaData( FObject * lpObjInst, const FString & sClassName, UI32 iSize ) : lpObjInst( lpObjInst ), sClassName( sClassName ), iClassSize( iSize )
{
	FObjectMetaManager::GetMetaManager()->RegisterMetaData( this );
}

FObjectMetaData::FObjectMetaData( FObject * lpObjInst, const FString & sSuperClass, const FString & sClassName, UI32 iSize ) :lpObjInst( lpObjInst ), sClassName( sClassName ), iClassSize( iSize )
{
	lpSuperClass = FObjectMetaManager::GetMetaManager()->FindMetaData( sSuperClass );
	if( !lpSuperClass )
		FLog::PutError( "Super class \"%s\" not found", sSuperClass.GetChar() );
	FObjectMetaManager::GetMetaManager()->RegisterMetaData( this );
}

FObjectMetaData::~FObjectMetaData()
{
}

UI32 FObjectMetaData::GetSize()const
{
	return iClassSize;
}

const FString & FObjectMetaData::GetName()const
{
	return sClassName;
}

FObject * FObjectMetaData::CreateObject( void * lpPlacement, FObjectList * lpList, const CHAR_ * lpFormat, va_list lpArgs )
{

}
