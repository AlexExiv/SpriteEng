#ifndef __FLIST_H__
#define __FLIST_H__

#include "..\types.h"
#include "FLoopAllocator.h"

template <class T>
class  FFreePointer__
{
public:
	static void Free( T t )
	{
	}
};

template <class T>
class  FFreeObject__
{
public:
	static void Free( T t )
	{
		t.~T();
	}
};

template <class T, class FreeMethod = FFreeObject__<T>>
class FList
{

	struct Node
	{
		Node * lpPrev, * lpNext;
		T tData;
	};

	enum
	{
		ALLOC_BLOCK_COUNT_STEP = 16
	};
	Node * lpNodeList;
	UI32 iCount;
	UI32 iAllocType;
	UI32 iBlockSize, iBlockCount, iCurBlockObjCnt;
	FBYTE ** lpBlocks;
	Node * lpFreeBlocks;

	Node * AllocNode()
	{
		if( lpFreeBlocks )
		{
			Node * lpNode = lpFreeBlocks;
			lpFreeBlocks = lpFreeBlocks->lpNext;
			return lpNode;
		}
		if( iCurBlockObjCnt == iBlockSize )
		{
			iBlockCount++;
			iCurBlockObjCnt = 0;
			if( (iBlockCount % ALLOC_BLOCK_COUNT_STEP) == 0 )
			{
				FBYTE ** lpNewBlocks = (FBYTE **)FMalloc( sizeof( FBYTE * )*(iBlockCount + ALLOC_BLOCK_COUNT_STEP) );
				for( UI32 i = 0;i < iBlockCount;i++ )
					lpNewBlocks[i] = lpBlocks[i];
				FFree( lpBlocks );
				lpBlocks = lpNewBlocks;
			}
			lpBlocks[iBlockCount] = (FBYTE *)FMalloc( sizeof( Node )*iBlockSize );
		}
		Node * lpNode = (Node *)(lpBlocks[iBlockCount] + iCurBlockObjCnt*sizeof( Node ));
		iCurBlockObjCnt++;
		return lpNode;
	}

	void FreeNode( Node * lpNode )
	{
		FreeMethod::Free( lpNode->tData );
		lpNode->lpNext = lpFreeBlocks;
		lpFreeBlocks = lpNode;
	}

	void InitBlockAllocator( )
	{
		iBlockCount = 0;
		iCurBlockObjCnt = 0;
		lpBlocks = (FBYTE **)FMalloc( sizeof(FBYTE *)*ALLOC_BLOCK_COUNT_STEP );
		lpBlocks[iBlockCount] = (FBYTE *)FMalloc( sizeof( Node )*iBlockSize );
	}

	void FreeBlockAllocator()
	{
		for( UI32 i = 0;i < iBlockCount;i++ )
			FFree( lpBlocks[i] );
		FFree( lpBlocks );
	}

	void Init()
	{
		switch( iAllocType )
		{
		case FList::GLOBAL_ALLOCATOR:
			lpNodeList = new Node;
			break;
		case FList::LOOP_ALLOCATOR:
			lpNodeList = (Node *)ALLOC_LOOP( sizeof( Node ) );
			break;
		case FList::BLOCK_ALLOCATOR:
			lpNodeList = AllocNode();
			break;
		};
		lpNodeList->lpNext = lpNodeList;
		lpNodeList->lpPrev = lpNodeList;
	}
public:
	enum
	{
		GLOBAL_ALLOCATOR = 1,
		LOOP_ALLOCATOR,
		BLOCK_ALLOCATOR
	};

	class Iterator
	{
		friend class FList;

		Node * lpCurNode;

	public:
		Iterator() : lpCurNode(NULL){}
		Iterator( const Iterator & iIt ) : lpCurNode( iIt.lpCurNode ){}
		Iterator( Node * lpNode ) : lpCurNode( lpNode ) {}

		Iterator & operator = ( const Iterator & iIt )
		{
			lpCurNode = iIt.lpCurNode;
			return *this;
		}

		T & operator -> ()
		{
			return lpCurNode->tData;
		}

		T & operator -> ()const
		{
			return lpCurNode->tData;
		}

		operator T * ()
		{
			return &lpCurNode->tData;
		}

		operator T * ()const
		{
			return &lpCurNode->tData;
		}

		Iterator & operator ++ ()
		{
			if( lpCurNode )
				lpCurNode = lpCurNode->lpNext;
			return *this;
		}

		Iterator & operator -- ()
		{
			if( lpCurNode )
				lpCurNode = lpCurNode->lpPrev;
			return *this;
		}

		Iterator operator ++ (I32)
		{
			Iterator iIt = *this;

			if( lpCurNode )
				lpCurNode = lpCurNode->lpNext;
			return iIt;
		}

		Iterator operator -- (I32)
		{
			Iterator iIt = *this;

			if( lpCurNode )
				lpCurNode = lpCurNode->lpPrev;
			return iIt;
		}

		bool operator == ( const Iterator & iIt )const
		{
			return lpCurNode == iIt.lpCurNode;
		}

		bool operator != ( const Iterator & iIt )const
		{
			return lpCurNode != iIt.lpCurNode;
		}

		bool operator == ( const T & tData )const
		{
			return lpCurNode->tData == tData;
		}

		bool operator != ( const T & tData )const
		{
			return lpCurNode->tData != tData;
		}

		Node * GetNode()
		{
			return lpCurNode;
		}
	};


	FList() : lpNodeList( NULL ), iCount( 0 ), iAllocType( GLOBAL_ALLOCATOR ), lpFreeBlocks( NULL )
	{
		Init();
	}

	FList( UI32 iAllocType ) : lpNodeList( NULL ), iCount( 0 ), iAllocType( iAllocType ), lpFreeBlocks( NULL )
	{
		Init();
	}

	FList( UI32 iBlockSize, UI32 bReserved ) : lpNodeList( NULL ), iCount( 0 ), iAllocType( BLOCK_ALLOCATOR )
		, lpFreeBlocks( NULL ), iBlockCount( 0 ), iBlockSize( iBlockSize )
	{
		InitBlockAllocator( );
		Init();
	}
	FList( const FList<T, FreeMethod> & lList ) : lpNodeList( NULL ), iCount( 0 ), iAllocType( lList.iAllocType ), lpFreeBlocks( NULL )
	{
		if( iAllocType == FList::BLOCK_ALLOCATOR )
		{
			iBlockSize = lList.iBlockSize;
			InitBlockAllocator();
		}

		Init();

		for( Iterator iIt = lList.Begin();iIt != lList.End();iIt++ )
			PushBack( *iIt );
	}
	~FList()
	{
		Clear();
		switch( iAllocType )
		{
		case FList::GLOBAL_ALLOCATOR:
			delete lpNodeList;
			break;
		case FList::BLOCK_ALLOCATOR:
			FreeBlockAllocator();
			break;
		}
	}
	FList<T, FreeMethod> & operator = ( const FList<T, FreeMethod> & lList )
	{
		Clear();
		switch( iAllocType )
		{
		case FList::GLOBAL_ALLOCATOR:
			delete lpNodeList;
			break;
		//case FList::BLOCK_ALLOCATOR:
		//	FreeBlockAllocator();
		//	break;
		}
		iAllocType = lList.iAllocType;
		if( iAllocType == FList::BLOCK_ALLOCATOR )
		{
			iBlockSize = lList.iBlockSize;
			InitBlockAllocator();
		}
		Init();
		
		for( Iterator iIt = lList.Begin();iIt != lList.End();iIt++ )
			PushBack( *iIt );
		return *this;
	}

	void Clear()
	{
		Iterator iIt = Begin();
		while( iIt != End() )
			iIt = Erase( iIt );
		iCount = 0;
	}

	Iterator Begin()
	{
		return Iterator( lpNodeList->lpNext );
	}

	Iterator Begin()const
	{
		return Iterator( lpNodeList->lpNext );
	}

	Iterator End()
	{
		return Iterator( lpNodeList );
	}

	Iterator End()const
	{
		return Iterator( lpNodeList );
	}

	Iterator First()
	{
		return Begin();
	}

	Iterator First()const
	{
		return Begin();
	}

	Iterator Last()
	{
		return Iterator( lpNodeList->lpPrev );
	}

	Iterator Last()const
	{
		return Iterator( lpNodeList->lpPrev );
	}

	UI32 GetCount()const
	{
		return iCount;
	}

	bool IsEmpy()const
	{
		return iCount == 0;
	}

	void Insert( const T & tData, Iterator iIt )
	{
		Node * lpNode = iIt.lpCurNode;
		Node * lpNewNode = NULL;

		switch( iAllocType )
		{
		case FList::GLOBAL_ALLOCATOR:
			lpNewNode = new Node;
			break;
		case FList::LOOP_ALLOCATOR:
			lpNewNode = (Node *)ALLOC_LOOP( sizeof( Node ) );
			break;
		case FList::BLOCK_ALLOCATOR:
			lpNewNode = (Node *)AllocNode();
			break;
		default:
			FLog::PutError( "Unknown allocator type code: %i", iAllocType );
			return;
		};

		lpNewNode->tData = tData;
		Node * lpTemp = lpNode->lpNext;
		lpNode->lpNext = lpNewNode;
		lpNewNode->lpPrev = lpNode;
		lpNewNode->lpNext = lpTemp;
		lpTemp->lpPrev = lpNewNode;
		iCount++;
	}

	Iterator Erase( Iterator iIt )
	{
		Node * lpNode = iIt.GetNode();
		lpNode->lpPrev->lpNext = lpNode->lpNext;
		lpNode->lpNext->lpPrev = lpNode->lpPrev;
		Node * lpNext = lpNode->lpNext;

		switch( iAllocType )
		{
		case FList::GLOBAL_ALLOCATOR:
			delete lpNode;
			break;
		case FList::BLOCK_ALLOCATOR:
			FreeNode( lpNode );
			break;
		};
		iCount--;

		return Iterator( lpNext );
	}

	void PushBack( const T & tData )
	{
		Insert( tData, --End() );
	}

	void PushFront( const T & tData )
	{
		Insert( tData, --Begin() );
	}

	void PopBack()
	{
		Erase( --End() );
	}

	void PopFront()
	{
		Erase( Begin() );
	}
};


#endif