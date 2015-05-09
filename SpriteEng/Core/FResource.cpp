#include <stdlib.h>
#include "FResource.h"
#include "FFile.h"
#include "FBMPResource.h"



FResource::FResource( const FString & sExtStr, FResourceManager * lpCreator ) : lpData( NULL ), bRemoveData( true ), sExtStr( sExtStr ), lpCreator( lpCreator )
{
}

FResource::FResource( const FString & sExtStr, const FString & sResClass ) : sExtStr( sExtStr ), sResClass( sResClass )
{

}

FResource::~FResource()
{
	if( lpData && bRemoveData )
		FFree( lpData );
}

void * FResource::GetData()const
{
	return lpData;
}

bool FResource::IsExtEqual( const FString & sExtStr0 )
{
	return sExtStr == sExtStr0;
}
