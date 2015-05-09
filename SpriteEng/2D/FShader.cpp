#include "FShader.h"

FShader::FShader( const FString & sName ) : FGraphObject( sName, FGraphObjectManager::OBJECT_SHADER )
{
}

FShader::~FShader()
{
	FTextureDict::FStringList lStringList;
	dTextureDict.GetKeyList( lStringList );

	FTextureDict::FStringIterator iIt = lStringList.Begin();
	for(;iIt != lStringList.End();iIt++ )
	{
		FTextureRecord rRec = dTextureDict.FindRecord( *iIt );
		delete rRec;
	}
}

UI32 FShader::GetTextureBlock( const FString & sTexName )const
{
	FTextureRecord rRec = dTextureDict.FindRecord( sTexName );
	if( rRec == NULL )
	{
		FLog::PutError( "Unknow texture name \"%s\" in shader file \"%s\"", sTexName.GetChar(), GetName().GetChar() );
		return 0;
	}
	return rRec->iBlock;
}
