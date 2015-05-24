#ifndef __FDICTIONARY_H__
#define __FDICTIONARY_H__



#include "FString.h"
#include "FLoopAllocator.h"
#include "FLog.h"
#include "FList.h"


template <class T>
class FDictionary
{
private:
	struct FNode
	{
		FNode * lpNext;
		T tRecord;
		FString sKey;
	};

public:
	typedef FList<FString> FStringList;
	typedef FList<FString>::Iterator FStringIterator;

	enum
	{
		LOOP_ALLOCATOR = 1,
		GLOBAL_ALLOCATOR
	};

	class FRecord
	{
		friend class FDictionary;
		FNode * lpNode;
	public:
		FRecord() : lpNode( NULL )
		{}
		FRecord( const FRecord & rRec ) : lpNode( rRec.lpNode )
		{}
		FRecord( FNode * lpNode ) : lpNode( lpNode )
		{}
		~FRecord(){}

		FRecord & operator = ( const FRecord & rRec )
		{
			lpNode = rRec.lpNode;
			return *this;
		}

		bool operator == ( const FRecord & rRec ) const
		{
			return lpNode == rRec.lpNode;
		}
		bool operator != ( const FRecord & rRec )const
		{
			return lpNode != rRec.lpNode;
		}
		bool operator == ( const T & lpRec )const
		{
			return lpNode->tRecord == lpRec;
		}
		bool operator != ( const T & lpRec )const
		{
			return lpNode->tRecord != lpRec;
		}

		T & operator -> ()const
		{
			return lpNode->tRecord;
		}
		T & operator -> ()
		{
			return lpNode->tRecord;
		}
		operator T ()
		{
			if( !lpNode )
				return NULL;
			return lpNode->tRecord;
		}
		operator T ()const
		{
			if( !lpNode )
				return NULL;
			return lpNode->tRecord;
		}
	};

private:
	FNode ** lpHashNodes;
	UI32 iHashSize;
	UI32 iAllocType;

	void Init()
	{
		switch( iAllocType )
		{
		case FDictionary::GLOBAL_ALLOCATOR:
			lpHashNodes = (FNode **)FMalloc( sizeof( FNode * )*iHashSize );
			break;
		case FDictionary::LOOP_ALLOCATOR:
			lpHashNodes = (FNode **)ALLOC_LOOP( sizeof( FNode * )*iHashSize );
			break;
		default:
			FLog::PutError( "Unknown allocator type code: %i", iAllocType );
		};
		for( UI32 i = 0;i < iHashSize;i++ )
			lpHashNodes[i] = NULL;
	}

public:
	FDictionary( ) : iHashSize( 256 ), iAllocType( FDictionary::GLOBAL_ALLOCATOR ), lpHashNodes( NULL )
	{
		Init();
	}

	FDictionary( UI32 iAllocType, UI32 iHashSize ) : iHashSize( iHashSize ), iAllocType( iAllocType ), lpHashNodes( NULL )
	{
		Init();
	}

	FDictionary( const FDictionary<T> & dDict ) : iHashSize( dDict.iHashSize ), lpHashNodes( NULL ), iAllocType( dDict.iAllocType )
	{
		Init();
		FStringList lStringList;
		dDict.GetKeyList( lStringList );

		FStringIterator iIt = lStringList.Begin();
		for(;iIt != lStringList.End();iIt++ )
		{
			FRecord rRec = dDict.FindRecord( *iIt );
			AddRecord( *iIt, *rRec );
		}
	}

	~FDictionary()
	{
		if( !lpHashNodes )
			return;
		Clear();
		switch( iAllocType )
		{
		case FDictionary::GLOBAL_ALLOCATOR:
			FFree( lpHashNodes );
			break;
		}
	}

	FDictionary<T> & operator = ( const FDictionary<T> & dDict )
	{
		Clear();
		switch( iAllocType )
		{
		case FDictionary::GLOBAL_ALLOCATOR:
			FFree( lpHashNodes );
			break;
		}

		iHashSize = dDict.iHashSize;
		iAllocType = dDict.iAllocType;
		Init();

		FStringIterator iIt = lStringList.Begin();
		for(;iIt != lStringList.End();iIt++ )
		{
			FRecord rRec = dDict.FindRecord( *iIt );
			AddRecord( *iIt, *rRec );
		}

		return *this;
	}

	void AddRecord( const FString & sKey, const T & tRecord )
	{
		if( !lpHashNodes )
			return;

		FRecord rRec = FindRecord( sKey );
		if( rRec != FRecord( NULL ) )
		{
			rRec.lpNode->tRecord = tRecord;
			return;
		}

		FNode * lpNode = NULL;
		switch( iAllocType )
		{
		case FDictionary::LOOP_ALLOCATOR:
			lpNode = (FNode *)ALLOC_LOOP( sizeof( FNode ) );
			break;
		case FDictionary::GLOBAL_ALLOCATOR:
			lpNode = new FNode;
			break;
		default:
			FLog::PutError( "Unknown allocator type code: %i", iAllocType );
			return;
		};
		lpNode->tRecord = tRecord;
		lpNode->lpNext = lpHashNodes[sKey.GetKey()%iHashSize];
		lpHashNodes[sKey.GetKey()%iHashSize] = lpNode;
		lpNode->sKey = sKey;
			
	}

	void RemoveRecord( const FString & sKey )
	{
		if( !lpHashNodes )
			return;

		UI32 iKey = sKey.GetKey()%iHashSize;
		FNode * lpNode = lpHashNodes[iKey], * lpPrevNode = NULL;

		while( lpNode )
		{
			if( lpNode->sKey == sKey )
				break;
			lpPrevNode = lpNode;
			lpNode = lpNode->lpNext;
		}
		if( !lpNode )
		{
			FLog::PutWarning( "Try to delete unknown record with name: \"%s\"", sKey.GetChar() );
			return;
		}
		if( lpPrevNode )
			lpPrevNode->lpNext = lpNode->lpNext;
		else
			lpHashNodes[iKey] = lpNode->lpNext;

		switch( iAllocType )
		{
		case FDictionary::GLOBAL_ALLOCATOR:
			delete lpNode;
			break;
		}
	}

	FRecord FindRecord( const FString & sKey )const
	{
		if( !lpHashNodes )
			return FRecord( NULL );

		UI32 iKey = sKey.GetKey()%iHashSize;
		FNode * lpNode = lpHashNodes[iKey];
		while( lpNode )
		{
			if( lpNode->sKey == sKey )
				break;
			lpNode = lpNode->lpNext;
		}

		return FRecord( lpNode );
	}

	void Clear()
	{
		if( !lpHashNodes )
			return;
		if( iAllocType == GLOBAL_ALLOCATOR )
		{
			for( UI32 i = 0;i < iHashSize;i++ )
			{
				FNode * lpNode = lpHashNodes[i];
				while( lpNode )
				{
					FNode * lpTmp = lpNode;
					lpNode = lpNode->lpNext;
					delete lpTmp;
				}
			}
		}
		for( UI32 i = 0;i < iHashSize;i++ )
			lpHashNodes[i] = NULL;
	}

	void GetKeyList( FStringList & lStringList )const
	{
		for( UI32 i = 0;i < iHashSize;i++ )
		{
			FNode * lpNode = lpHashNodes[i];
			while( lpNode )
			{
				lStringList.PushBack( lpNode->sKey );
				lpNode = lpNode->lpNext;
			}
		}
	}
};


#endif