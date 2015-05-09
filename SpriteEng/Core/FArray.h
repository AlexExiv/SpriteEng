#ifndef __FARRAY_H__
#define __FARRAY_H__

#include <vector>

#include "..\types.h"

#define DEF_CHUNK_SIZE 32

template <class T>
class FArray
{
	T * lpData;
	UI32 iCount, iArrSize;
	UI32 iChunkSize;
public:
	class Iterator
	{
		T * lpData;
	public:
		Iterator() : lpData( NULL ){}
		Iterator( T * lpData ) : lpData( lpData ) {}
		Iterator( const Iterator & iIt ) : lpData( iIt.lpData ) {}
		
		Iterator & operator = ( const Iterator & iIt )
		{
			lpData = iIt.lpData;
			return *this;
		}

		Iterator & operator = ( const T * lpData0 )
		{
			lpData = lpData0;
			return *this;
		}

		bool operator == ( const Iterator & iIt )
		{
			return (lpData == iIt.lpData);
		}

		bool operator != ( const Iterator & iIt )
		{
			return (lpData != iIt.lpData);
		}

		operator T * ()
		{
			return lpData;
		}

		operator T * ()const
		{
			return lpData;
		}

		T operator -> ()
		{
			return (*lpData);
		}

		Iterator & operator ++ ()
		{
			lpData = (T *)((I8 *)lpData + sizeof(T));
			return *this;
		}

		Iterator & operator -- ()
		{
			lpData = (T *)((I8 *)lpData - sizeof(T));
			return *this;
		}
	};

	FArray() : lpData( NULL ), iCount(0), iArrSize(0), iChunkSize(DEF_CHUNK_SIZE) {}
	FArray( UI32 iDeffSize ) : lpData( NULL ), iArrSize( iDeffSize ), iCount( 0 ), iChunkSize( DEF_CHUNK_SIZE )
	{
		lpData = (T *)FMalloc( iArrSize*sizeof( T ) );
	}
	FArray( const FArray & aArr )
	{
		iCount = aArr.iCount;
		iArrSize = aArr.iArrSize;
		iChunkSize = aArr.iChunkSize;
		lpData = (T *)FMalloc( iArrSize * sizeof(T) );
		memcpy( lpData, aArr.lpData, iCount * sizeof(T) );
	}
	~FArray()
	{
		Clear();
	}

	FArray & operator = ( const FArray & aArr )
	{
		Clear();

		iCount = aArr.iCount;
		iArrSize = aArr.iArrSize;
		iChunkSize = aArr.iChunkSize;
		lpData = (T *)FMalloc( iArrSize * sizeof(T) );
		memcpy( lpData, aArr.lpData, iCount * sizeof(T) );

		return *this;
	}

	void Clear()
	{
		if( lpData )
			FFree( lpData );
		lpData = NULL;
		iArrSize = 0;
		iCount = 0;
	}

	Iterator Begin()const
	{
		return Iterator( lpData );
	}

	Iterator End()const
	{
		return Iterator( (T *)((I8 *)lpData + iCount*sizeof(T)) );
	}

	UI32 GetCount()const
	{
		return iCount;
	}

	void SetChunkSize( UI32 iChunk )
	{
		iChunkSize = iChunk;
	}

	Iterator operator[] ( UI32 iIndex )
	{
		return (T *)((I8 *)lpData + iIndex*sizeof(T));
	}

	Iterator operator[] ( UI32 iIndex )const
	{
		return (T *)((I8 *)lpData + iIndex*sizeof(T));
	}

	//T * operator[] ( I32 iIndex )
	//{
	//	return &(lpData + iIndex);
	//}

	void Realloc( UI32 iSize )
	{
		if( iSize < iArrSize )
			return;

		T * lpNewData = (T *)FMalloc( iSize * sizeof(T) );
		if( lpData )
		{	
			memcpy( lpNewData, lpData, iCount * sizeof(T) );
			FFree( lpData );
			
		}
		lpData = lpNewData;
		iArrSize = iSize;
	}

	Iterator Add( const T & oObject )
	{
		if( iCount == iArrSize )
			Realloc( iArrSize + iChunkSize );

		memcpy( End(), (void *)&oObject, sizeof( T ) );
		iCount++;

		return End() - 1;
	}

	Iterator Add( const T & oObject, I32 iIndex )
	{
		if( iCount == iArrSize )
			Realloc( iArrSize + iChunkSize );

		I32 iHalfSize = (iCount - iIndex) * sizeof( T );
		T * lpHalfData = (T *)FMalloc( iHalfSize );
		memcpy( lpHalfData, lpData + iIndex, iHalfSize );
		memcpy( lpData + iIndex, oObject, sizeof( T ) );
		memcpy( lpData + iIndex + 1, lpHalfData, iHalfSize );
		FFree( lpHalfData );
		iCount++;

		return lpData + iIndex;
	}

	Iterator Add( const T & oObject, Iterator iIt )
	{
		return Add( oObject, iIt - lpData - 1 );
	}

	Iterator Erase( UI32 iIndex )
	{
		if( (!lpData) || (iIndex >= iCount) )
			return NULL;
		memcpy( lpData + iIndex, lpData + iIndex + 1, (iCount - iIndex) * sizeof(T) );
		return lpData + iIndex;
	}

	Iterator Erase( Iterator iIt )
	{
		return Erase( iIt - lpData - 1 );
	}

};

#endif