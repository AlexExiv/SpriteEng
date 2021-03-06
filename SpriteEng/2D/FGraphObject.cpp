#include "FGraphObject.h"
#include "..\Core\FLog.h"
#include "FAnimation2D.h"
#include "FShader.h"
#include "..\Core\FException.h"
#include "FFont.h"
#include "FTexture.h"
#include "FView.h"


static FGraphObjectManager * lpGrManager = NULL;


FGraphObject::FGraphObject( const FString & sName, UI32 iObjType ) : sName( sName ), iRefCount( 1 ), iObjType( iObjType )
{
}

FGraphObject::~FGraphObject()
{
	iObjType = 0;
}

const FString & FGraphObject::GetName()const
{
	return sName;
}

void FGraphObject::AddRef()
{
	iRefCount++;
}

void FGraphObject::DecrRef()
{
	iRefCount--;
}

bool FGraphObject::IsRef()const
{
	return iRefCount != 0;
}

UI32 FGraphObject::GetType()const
{
	return iObjType;
}



FGraphObjectManager::FGraphObjectManager() : iNodeCount( OBJ_NODE_COUNT ), iCurNode( 0 ), lpFreeArr( NULL )
{
	for( UI32 i = 0;i < MAX_HASH;i++ )
		lpObjectHash[i] = NULL;
	lpNodeArr = (FNode *)FMalloc( sizeof( FNode )*iNodeCount );
}

FGraphObjectManager::~FGraphObjectManager()
{
	Clear();
	FFree( lpNodeArr );
}


FGraphObjectManager::FNode * FGraphObjectManager::AllocNode()
{
	FNode * lpNode = NULL;
	if( lpFreeArr )
	{
		lpNode = lpFreeArr;
		lpFreeArr = lpFreeArr->lpNext;
	}
	else
	{
		lpNode = lpNodeArr + iCurNode;
		iCurNode++;
		AssertFatal( iCurNode != iNodeCount, "to very few nodes for objects" );
	}
	lpNode->lpNext = NULL;
	lpNode->lpPrev = NULL;

	return lpNode;
}

void FGraphObjectManager::FreeNode( FGraphObjectManager::FNode * lpNode )
{
	lpNode->lpNext = lpFreeArr;
	lpFreeArr = lpNode;
}

FGraphObject * FGraphObjectManager::Find( const FString & sName )
{
	FNode * lpNode = lpObjectHash[sName.GetKey()%MAX_HASH];
	while( lpNode )
	{
		if( lpNode->lpObject->GetName() == sName )
			return lpNode->lpObject;
		lpNode = lpNode->lpNext;
	}

	return NULL;
}

FGraphObject * FGraphObjectManager::CreateObject( const FString & sName, UI32 iObjType )
{
	FGraphObject * lpObject = Find( sName );
	if( lpObject )
	{
		AssertFatal( iObjType == lpObject->GetType(), "same object name, have different object types" );
		lpObject->AddRef();
		return lpObject;
	}

	try
	{
		switch( iObjType )
		{
		case OBJECT_ANIMATION:
			lpObject = (FGraphObject *)FMalloc( sizeof( FAnimation2D ) );
			lpObject = new (lpObject ) FAnimation2D( sName );
			break;
		case OBJECT_SHADER:
			lpObject = (FGraphObject *)FMalloc( FView::GetMainView()->GetShaderObjSize() );
			lpObject = FView::GetMainView()->CreateShader( lpObject, sName );
			break;
		case OBJECT_FONT:
			lpObject = (FGraphObject *)FMalloc( sizeof( FFont ) );
			lpObject = new (lpObject ) FFont( sName );
			break;
		case OBJECT_TEXTURE:
			lpObject = (FGraphObject *)FMalloc( FView::GetMainView()->GetTextureObjSize() );
			lpObject = FView::GetMainView()->CreateTexture( lpObject, sName );

			break;
		default:
			AssertFatal( false, "Unknown object type" );
		}

		FNode * lpNode = AllocNode();
		lpNode->lpObject = lpObject;
		UI32 iKey = sName.GetKey()%MAX_HASH;
		lpNode->lpNext = lpObjectHash[iKey];
		if( lpObjectHash[iKey] )
			lpObjectHash[iKey]->lpPrev = lpNode;
		lpObjectHash[iKey] = lpNode;
	}
	catch( FException eExcp )
	{
		if( lpObject )
			FFree( lpObject );
		lpObject = NULL;

		FLog::PutError( FString::PrintString( "%s: \"%s\"", eExcp.GetMess().GetChar(), sName.GetChar() ) );
		if( eExcp.GetCode() == FException::EXCP_FATAL_ERROR )
			throw eExcp;
	}

	return lpObject;
}

FTexture * FGraphObjectManager::CreateTexture( const FString & sName, const FImageResource * lpImg )
{
	FTexture * lpTexture = (FTexture *)Find( sName );
	if( lpTexture )
	{
		AssertFatal( lpTexture->GetType() == FGraphObjectManager::OBJECT_TEXTURE, "same object name, have different object types" );
		lpTexture->AddRef();
		return lpTexture;
	}

	try
	{
		lpTexture = (FTexture *)FMalloc( FView::GetMainView()->GetTextureObjSize() );
		lpTexture = FView::GetMainView()->CreateTexture( lpTexture, sName, lpImg );

		FNode * lpNode = AllocNode();
		lpNode->lpObject = lpTexture;
		UI32 iKey = sName.GetKey()%MAX_HASH;
		lpNode->lpNext = lpObjectHash[iKey];
		if( lpObjectHash[iKey] )
			lpObjectHash[iKey]->lpPrev = lpNode;
		lpObjectHash[iKey] = lpNode;
	}
	catch( FException eExcp )
	{
		if( eExcp.GetCode() == FException::EXCP_FILE_NOT_FOUND )
			FLog::PutError( "Can't load object data" );
		FFree( lpTexture );
		return NULL;
	}
	return lpTexture;
}

void FGraphObjectManager::ReleaseObject( FGraphObject * lpObject )
{
	if( !lpObject )
		return;

	lpObject->DecrRef();
	if( !lpObject->IsRef() )
	{
		UI32 iKey = lpObject->GetName().GetKey() % MAX_HASH;
		FNode * lpNode = lpObjectHash[iKey];
		while( lpNode )
		{
			if( lpNode->lpObject == lpObject )
				break;
			lpNode = lpNode->lpNext;
		}
		AssertFatal( lpNode != NULL, "Release unknow object" );
		if( lpNode )
		{
			if( lpNode->lpPrev )
				lpNode->lpPrev->lpNext = lpNode->lpNext;
			if( lpNode->lpNext )
				lpNode->lpNext->lpPrev = lpNode->lpPrev;
			if( lpNode == lpObjectHash[iKey] )
				lpObjectHash[iKey] = lpObjectHash[iKey]->lpNext;
			FreeNode( lpNode );
		}
		delete lpObject;
	}
}

void FGraphObjectManager::Clear()
{
	for( UI32 i = 0;i < MAX_HASH;i++ )
	{
		FNode * lpNode = lpObjectHash[i];
		while( lpNode )
		{
			FNode * lpNext = lpNode->lpNext;
			delete lpNode->lpObject;
			FreeNode( lpNode );
			lpNode = lpNext;
		}
	}
}

void FGraphObjectManager::ChacheObject( const FString & sName, UI32 iObjType )
{
	FGraphObject * lpObject = CreateObject( sName, iObjType );
	if( !lpObject )
	{
		FLog::PutError( "Can't chache object, \"%s\" is not exist", sName.GetChar() );
		return;
	}

	dObjectChache.AddRecord( sName, lpObject );
}

void FGraphObjectManager::UnChacheObject( const FString & sName )
{
	FObjectRecord rRec = dObjectChache.FindRecord( sName );
	if( rRec == FObjectRecord( NULL ) )
	{
		FLog::PutError( "Chached object with name \"%\" is not found", sName.GetChar() );
		return;
	}

	ReleaseObject( rRec );
	dObjectChache.RemoveRecord( sName );
}

void FGraphObjectManager::ClearChache()
{
	FObjectChache::FStringList lStringList;
	dObjectChache.GetKeyList( lStringList );
	FObjectChache::FStringIterator iIt = lStringList.Begin();
	for(;iIt != lStringList.End();iIt++ )
	{
		FObjectRecord rRec = dObjectChache.FindRecord( *iIt );
		ReleaseObject( rRec );
		dObjectChache.RemoveRecord( *iIt );
	}
}


FGraphObjectManager * FGraphObjectManager::GetInstance()
{
	if( !lpGrManager )
		lpGrManager = new FGraphObjectManager();
	return lpGrManager;
}