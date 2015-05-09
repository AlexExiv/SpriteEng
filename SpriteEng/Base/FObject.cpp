#include "FObject.h"
#include "FObjectAllocator.h"
#include <string.h>
#include "..\Core\FLog.h"



FObject::FObject( ) : iRefCount( 1 )/*, lpAllocator( NULL )*/
{
}

FObject::FObject( UI32 iObjReserved ) : iRefCount( 1 ), lpAllocator( NULL )
{
	lpAllocator = new FObjectAllocator( iObjReserved, this );
}

FObject::~FObject()
{
	if( lpAllocator->lpOwner == this )
		delete lpAllocator;
}

void FObject::Update( F32 fDTime )
{
}

void FObject::AddRef()
{
	iRefCount++;
}

void FObject::Release()
{
	iRefCount--;
	if( iRefCount == 0 )
		lpAllocator->DeleteObject( this );
}

void FObject::SetAllocator( FObjectAllocator * lpAllocator_ )
{
	lpAllocator = lpAllocator_;
}

FObject * FObject::CreateObject( void * lpPlacement, const CHAR_ * lpTypes, va_list lpArgs ) 
{
	return NULL;
}

FObject * FObject::AllocObject( const FString & sObjName, const CHAR_ * lpCallTypes, ... )
{
	va_list lpArgs;
	va_start( lpArgs, lpCallTypes );
	FObject * lpObject = lpAllocator->AllocObject( sObjName, lpCallTypes, lpArgs );
	va_end( lpArgs );

	return lpObject;
}

FObject * FObject::AllocObjectT( const FString & sObjName, const CHAR_ * lpCallTypes, ... )
{
	va_list lpArgs;
	va_start( lpArgs, lpCallTypes );
	FObject * lpObject = lpAllocator->AllocObjectT( sObjName, lpCallTypes, lpArgs );
	va_end( lpArgs );

	return lpObject;
}

void FObject::Delete( FObject * lpObject )
{
	lpAllocator->DeleteObject( lpObject );
}

bool FObject::CheckCallTypes( const CHAR_ * lpCallTypes, const CHAR_ * lpObjTypes )const
{
	if( strcmp( lpCallTypes, lpObjTypes ) )
	{
		FLog::PutError( "CreateObject call arguments is not satisfy object constructor parameters list: your \"%s\", need \"%s\"", lpCallTypes, lpObjTypes );
		return false;
	}
	return true;
}