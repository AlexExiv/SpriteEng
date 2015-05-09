#include "FGraphObject.h"
#include "..\Core\FLog.h"
#include "FAnimation2D.h"
#include "FShader.h"
#include "FView.h"
#include "..\Core\FException.h"
#include "FFont.h"
#include "FTexture.h"



static FGraphObjectManager * lpGrManager = NULL;


FGraphObject::FGraphObject( const FString & sName, UI32 iObjType ) : sName( sName ), iRefCount( 1 ), iObjType( iObjType )
{
}

FGraphObject::~FGraphObject()
{
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
	}

	return NULL;
}

FGraphObject * FGraphObjectManager::CreateObject( const FString & sName, UI32 iObjType )
{
	FGraphObject * lpObject = Find( sName );
	if( lpObject )
	{
		AssertFatal( iObjType == lpObject->GetType(), "same object name, have different object types" );
		return lpObject;
	}

	try
	{
		switch( iObjType )
		{
		case OBJECT_ANIMATION:
			lpObject = new FAnimation2D( sName );
			break;
		case OBJECT_SHADER:
			lpObject = FView::GetMainView()->CreateShader( sName );
			break;
		case OBJECT_FONT:
			lpObject = new FFont( sName );
			break;
		case OBJECT_TEXTURE:
			lpObject = FView::GetMainView()->CreateTexture( sName );

			break;
		default:
			AssertFatal( false, "Unknown object type" );
		}

		FNode * lpNode = AllocNode();
		lpNode->lpObject = lpObject;
		UI32 iKey = sName.GetKey()%MAX_HASH;
		lpNode->lpNext = lpObjectHash[iKey];
		lpObjectHash[iKey]->lpPrev = lpNode;
		lpObjectHash[iKey] = lpNode;
	}
	catch( FException eExcp )
	{
		if( lpObject )
			delete lpObject;
		FLog::PutError( FString::PrintString( "%s: \"%s\"", eExcp.GetMessage().GetChar(), sName.GetChar() ) );
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
		return lpTexture;
	}

	try
	{
		lpTexture = FView::GetMainView()->CreateTexture( sName, lpImg );
	}
	catch( FException eExcp )
	{
		if( eExcp.GetCode() == FException::EXCP_FILE_NOT_FOUND )
			FLog::PutError( "Can't load object data" );
		delete lpTexture;
		return NULL;
	}
	return lpTexture;
}

void FGraphObjectManager::ReleaseObject( FGraphObject * lpObject )
{
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
		AssertFatal( lpNode == NULL, "Release unknow object" );
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

FGraphObjectManager * FGraphObjectManager::GetInstance()
{
	if( !lpGrManager )
		lpGrManager = new FGraphObjectManager();
	return lpGrManager;
}