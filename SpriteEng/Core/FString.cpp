#include <string.h>
#include <stdlib.h>
#include "FString.h"
#include <stdarg.h>
#include "FStack.h"

class FStringAllocator;
class FStringTable;

FStringAllocator * lpStrAlloc = NULL;
FStringTable * lpStrTable = NULL;
const CHAR_ * lpOutOfMem = "Out Of Mem";

////Класс для выделения памяти под строковые данные в одной куче
class FStringAllocator final
{
	struct FNode
	{
		FNode * lpNext, * lpPrev;
		CHAR_ * lpStrData;
		UI32 iLen;
		UI32 iFlags;
	};

	enum
	{
		STR_ALLOC_GLOBAL = 0x00000001,
		STR_OUT_MEM = 0x00000002
	};

	FBYTE * lpAllocData;
	UI32 iFullSize, iAllocSize;
	UI32 iOutAllocType;
	FNode * lpFreeNodes[STR_NODE_COUNT];
	FNode * lpOutOfMemNode;

	FBYTE * AllockBlock( UI32 iSize );
	FNode * FindFree( UI32 iLen );

public:
	FStringAllocator( UI32 iFullSize, UI32 iOutAllocType );
	~FStringAllocator();

	CHAR_ * AllocString( UI32 iLen );
	void FreeString( CHAR_ * lpStr );

	static FStringAllocator * GetInstance();
	static void Destroy();

};


FStringAllocator::FStringAllocator( UI32 iFullSize, UI32 iOutAllocType ) : iFullSize( iFullSize ), iAllocSize( 0 ), lpAllocData( NULL ), iOutAllocType( iOutAllocType ), lpOutOfMemNode( NULL )
{
	lpAllocData = (FBYTE *)FMalloc( iFullSize );
	for( UI32 i = 0;i < STR_NODE_COUNT;i++ )
		lpFreeNodes[i] = NULL;

	if( iOutAllocType == STR_OUT_MEM_MESS )
	{
		UI32 iLen = strlen( lpOutOfMem );
		lpOutOfMemNode = (FNode *)FMalloc( sizeof( FNode ) + (iLen + 1)*sizeof( CHAR_ ) );
		memset( lpOutOfMemNode, 0, sizeof( FNode ) );
		lpOutOfMemNode->lpStrData = ((CHAR_ *)lpOutOfMemNode) + sizeof( FNode );
		strcpy( lpOutOfMemNode->lpStrData, lpOutOfMem );
		lpOutOfMemNode->iLen = iLen;
	}
}

FStringAllocator::~FStringAllocator()
{
	if( lpAllocData )
		FFree( lpAllocData );
	if( lpOutOfMemNode )
		FFree( lpOutOfMemNode );
}

CHAR_ * FStringAllocator::AllocString( UI32 iLen )
{
	FNode * lpStrNode = FindFree( iLen );//поиск свободной памяти в уже выделеных блоках
	if( !lpStrNode )//если подходящего блока не найдено выделим новый блок
	{
		FBYTE * lpStr = AllockBlock( (iLen + 1)*sizeof( CHAR_ ) + sizeof( FNode ) );
		if( !lpStr )
		{
			switch( iOutAllocType )
			{
			case STR_ALLOCATED_GLOBAL:
				lpStr = (FBYTE *)FMalloc( (iLen + 1)*sizeof( CHAR_ ) + sizeof( FNode ) );
				((FNode *)lpStr)->iFlags = FStringAllocator::STR_ALLOC_GLOBAL;
				break;
			case STR_OUT_MEM_MESS:
				lpStr = (FBYTE *)lpOutOfMemNode;
				return ((FNode *)lpStr)->lpStrData;
			};
		}
		else
			((FNode *)lpStr)->iFlags = 0;

		*((CHAR_ *)(lpStr + iLen*sizeof( CHAR_ ) + sizeof( FNode ))) = 0;
		lpStrNode = (FNode *)lpStr;
		lpStrNode->lpNext = NULL;
		lpStrNode->lpPrev = NULL;
		lpStrNode->iLen = iLen;
		lpStrNode->lpStrData = (CHAR_ *)(lpStr + sizeof( FNode ));
	}

	return lpStrNode->lpStrData;
}

FBYTE * FStringAllocator::AllockBlock( UI32 iSize )
{
	if( (iAllocSize + iSize) >= iFullSize )//если не хватает памяти
		return NULL;

	FBYTE * lpRet = lpAllocData + iAllocSize;
	iAllocSize += iSize;
	return lpRet;
}

///освобождение память из под строки, блок помечается как свободный и добавляется в общую таблицу свободных блок разделеных макс длиной для строки(на STR_NODE_COUNT штук) и отсортированые по возрастанию длины
void FStringAllocator::FreeString( CHAR_ * lpStr )
{
	FNode * lpStrNode = (FNode *)((FBYTE *)lpStr - sizeof( FNode ));
	if( lpStrNode->iFlags & FStringAllocator::STR_ALLOC_GLOBAL )
	{
		FFree( lpStrNode );
		return;
	}
	if( lpStrNode->iFlags & FStringAllocator::STR_OUT_MEM )
		return;

	UI32 iPos = lpStrNode->iLen/STR_NODE_STEP;//вычисляем позицию в множестве интервалов
	if( iPos >= STR_NODE_COUNT )//последняя позиция включает строки длиной более чем (STR_NODE_COUNT - 1)*STR_NODE_STEP
		iPos = STR_NODE_COUNT - 1;

	lpStrNode->lpNext = NULL;
	lpStrNode->lpPrev = NULL;

	//всатвляем блок в список свободных, сортируя по возрастанию длины
	if( lpFreeNodes[iPos] )//если в списке есть элементы
	{
		FNode * lpNode  = lpFreeNodes[iPos];
		while( lpNode->lpNext )//пока есть следущий элемент в списке
		{
			if( lpNode->iLen >= lpStrNode->iLen )
				break;
			lpNode = lpNode->lpNext;
		}
		if( lpFreeNodes[iPos] == lpNode )//если в списке только один элемент
		{
			if( lpNode->iLen < lpStrNode->iLen )
			{
				lpNode->lpNext = lpStrNode;
				lpStrNode->lpPrev = lpNode;
			}
			else
			{
				lpStrNode->lpNext = lpNode;
				lpNode->lpPrev = lpStrNode;
				lpFreeNodes[iPos] = lpStrNode;
			}
		}
		else
		{
			if( lpNode->lpNext || (lpStrNode->iLen <= lpNode->iLen) )//если след элемент есть или это конец списка и длина вставляемой строки меньше чем текущего элемента, то позиция найдена, вставляем перед элементом
			{
				lpNode->lpPrev->lpNext = lpStrNode;
				lpStrNode->lpPrev = lpNode->lpPrev;
				lpNode->lpPrev = lpStrNode;
				lpStrNode->lpNext = lpNode;
			}
			else//если след элемента нет и lpStrNode->iLen > lpNode->iLen
			{
				lpNode->lpNext = lpStrNode;
				lpStrNode->lpPrev = lpNode;
			}
		}
	}
	else//иначе это первый элемент в списке
	{
		lpFreeNodes[iPos] = lpStrNode;
	}
}

//поиск подходящего по длине блока
FStringAllocator::FNode * FStringAllocator::FindFree( UI32 iLen )
{
	UI32 iPos = iLen/STR_NODE_STEP;
	if( iPos >= STR_NODE_COUNT )
		iPos = STR_NODE_COUNT - 1;

	FNode * lpNode = lpFreeNodes[iPos];
	if( !lpNode )
		return NULL;

	while( lpNode )
	{
		if( (lpNode->iLen >= iLen) && ((lpNode->iLen - iLen) < STR_LEN_DEV ) )//если блок длиной больше необходимой длины, но не больше чем на STR_LEN_DEV символов
		{//удаляем блок из списка свободных
			if( lpNode == lpFreeNodes[iPos] )
			{
				lpFreeNodes[iPos] = lpNode->lpNext;
				lpFreeNodes[iPos]->lpPrev = NULL;
				lpNode->lpNext = NULL;
			}
			else
			{
				FNode * lpPrev = lpNode->lpPrev,
					* lpNext = lpNode->lpNext;

				lpPrev->lpNext = lpNext;
				if( lpNext )
					lpNext->lpPrev = lpPrev;

				lpNode->lpNext = NULL;
				lpNode->lpPrev = NULL;
			}
			break;
		}
		lpNode = lpNode->lpNext;
	}

	return lpNode;
}

FStringAllocator * FStringAllocator::GetInstance()
{
	if( !lpStrAlloc )
		lpStrAlloc = new FStringAllocator( STR_ALLOC_SIZE, STR_ALLOCATED_GLOBAL );
	return lpStrAlloc;
}

void FStringAllocator::Destroy()
{
	if( lpStrAlloc )
		delete lpStrAlloc;
}

//макросы для упрощения выделения и освобождения строк
#define ALLOC_STR( iLen ) FStringAllocator::GetInstance()->AllocString( iLen )
#define FREE_STR( lpStr ) FStringAllocator::GetInstance()->FreeString( lpStr )

#define MAX_STRING 32

//CRC32 таблица для вычисления хеш ключа для таблицы строк 
static UI32 iCRCTable[256];
static bool bCRCReady = false;

inline UI32 Reflect( UI32 iRef, UI32 iCount )
{
	UI32 iValue = 0;
	for( I32 i = 1;i < (iCount + 1);i++ )
	{
		if( iRef & 1 )
			iValue |= 1 << (iCount - i);
		iRef >>= 1;
	}

	return iValue;
}

void InitTable()
{
	UI32 iPoly = 0x04C11DB7;
	for( I32 i = 0;i < 256;i++ )
	{
		iCRCTable[i] = Reflect( i, 8 ) << 24;
		for( I32 j = 0;j < 8;j++ )
			iCRCTable[i] = (iCRCTable[i] << 1) ^ (iCRCTable[i] & (1 << 31) ? iPoly : 0);
		iCRCTable[i] = Reflect( iCRCTable[i], 32 );
	}
}

UI32 GetCRC( const void * lpMessage, UI32 iLen )
{
	if( !bCRCReady )
	{
		InitTable();
		bCRCReady = true;
	}

	UI32 iCRC = 0xFFFFFFFF;
	UI8 * lpChar = (UI8 *)lpMessage;
	while( iLen-- )
		iCRC = (iCRC >> 8) ^ iCRCTable[(iCRC & 0xFF) ^ *lpChar++];

	return iCRC ^ 0xFFFFFFFF;
}

class FStringTable final
{
	struct FNode
	{
		FNode * lpNext, * lpPrev;
		UI32 iRefCount;
		CHAR_ * lpStr;
	};

	FNode * lpHashTable[STR_MAX_HASH], * lpFreeNodes;
	FNode * lpNodeHeap;
	UI32 iAllocNodeCnt, iMaxNode;

	FNode * AllocNode();
	void FreeNode( FNode * lpNode, UI32 iPos );

public:
	FStringTable();
	~FStringTable();

	CHAR_ * AllocString( const CHAR_ * lpStr, UI32 iKey, UI32 iLen );
	void FreeString( CHAR_ * lpStr, UI32 iKey );

	static FStringTable * GetInstance();
	static void Destroy();
};


FStringTable::FStringTable() : iMaxNode( STR_NODE_HASH_COUNT ), iAllocNodeCnt( 0 ), lpNodeHeap( NULL ), lpFreeNodes( NULL )
{
	lpNodeHeap = (FNode *)FMalloc( iMaxNode*sizeof( FNode ) );
	for( UI32 i = 0;i < STR_MAX_HASH;i++ )
		lpHashTable[i] = NULL;
}

FStringTable::~FStringTable()
{
	if( lpNodeHeap )
		FFree( lpNodeHeap );
}

//выделение новой строки
CHAR_ * FStringTable::AllocString( const CHAR_ * lpStr, UI32 iKey, UI32 iLen )
{
	UI32 iPos = iKey % STR_MAX_HASH;
	FNode * lpNode = lpHashTable[iPos];
	if( lpNode )
	{
		while( lpNode )
		{
			if( (lpNode->lpStr == lpStr) || (strcmp( lpStr, lpNode->lpStr ) == 0) )
			{
				lpNode->iRefCount++;
				return lpNode->lpStr;
			}

			lpNode = lpNode->lpNext;
		}
	}

	FNode * lpNewNode = AllocNode();
	lpNewNode->iRefCount = 1;
	lpNewNode->lpPrev = NULL;
	lpNewNode->lpNext = lpHashTable[iPos];
	if( lpHashTable[iPos] )
		lpHashTable[iPos]->lpPrev = lpNewNode;
	lpHashTable[iPos] = lpNewNode;
	lpNewNode->lpStr = ALLOC_STR( iLen );
	strncpy( lpNewNode->lpStr, lpStr, iLen + 1 );

	return lpNewNode->lpStr;
}

//освобождение строки либо уменьшение количество ссылок на единицу
void FStringTable::FreeString( CHAR_ * lpStr, UI32 iKey )
{
	UI32 iPos = iKey % STR_MAX_HASH;
	FNode * lpNode = lpHashTable[iPos];
	if( lpNode )
	{
		while( lpNode )
		{
			if( lpNode->lpStr == lpStr )
			{
				lpNode->iRefCount--;
				if( lpNode->iRefCount == 0 )//если количество ссылок на строку равно 0 удаляем ее
					FreeNode( lpNode, iPos );

				break;
			}
			lpNode = lpNode->lpNext;
		}
	}
}

//выделение записи либо из списка свободных, либо из кучи
FStringTable::FNode * FStringTable::AllocNode()
{
	if( lpFreeNodes )//если есть свободные записи
	{
		FNode * lpRet = lpFreeNodes;
		lpFreeNodes = lpRet->lpNext;
		return lpRet;
	}
	//иначе выделяем новую из кучи
	if( (iAllocNodeCnt + 1) == iMaxNode )
		return NULL;

	return &lpNodeHeap[iAllocNodeCnt++];
}

//освобождение записи, памяти под строку и добавление записи в список свободных
void FStringTable::FreeNode( FStringTable::FNode * lpNode, UI32 iPos )
{
	FNode * lpNext = lpNode->lpNext, * lpPrev = lpNode->lpPrev;
	//удалаяем запись из хеш таблицы
	if( lpNode == lpHashTable[iPos] )
	{
		lpHashTable[iPos] = lpNext;
		if( lpNext )
			lpNext->lpPrev = NULL;
	}
	else
	{
		lpPrev->lpNext = lpNext;
		if( lpNext )
			lpNext->lpPrev = lpPrev;
	}

	FREE_STR( lpNode->lpStr );//освободим строку
	//добавим в список свободных записей
	if( lpFreeNodes )
		lpNode->lpNext = lpFreeNodes;
	else
		lpNode->lpNext = NULL;
	lpFreeNodes = lpNode;
}

FStringTable * FStringTable::GetInstance()
{
	if( !lpStrTable )
		lpStrTable = new FStringTable();
	return lpStrTable;
}

void FStringTable::Destroy()
{
	if( lpStrTable )
		delete lpStrTable;
}

//макросы для облегчения выделения строк
#define ALLOC_STRING( lpStr, iKey, iLen ) FStringTable::GetInstance()->AllocString( lpStr, iKey, iLen )
#define FREE_STRING( lpStr, iKey ) FStringTable::GetInstance()->FreeString( lpStr, iKey )





FString::FString() : lpString( NULL ), iLen( 0 ), iKey( 0 )
{
	
}

FString::FString( const CHAR_ * lpString0 ) : iLen( 0 ), iKey( 0 )
{
	if( !lpString0 )
		return;

	iLen = strlen( lpString0 );
	iKey = GetCRC( lpString0, iLen );

	lpString = ALLOC_STRING( lpString0, iKey, iLen );
}

FString::FString( CHAR_ cChar )
{
	iLen = 1;
	CHAR_ cBuffer[2] = { cChar, 0 };
	iKey = GetCRC( cBuffer, iLen );

	lpString = ALLOC_STRING( cBuffer, iKey, iLen );
}

FString::FString( const FString & sString ) : lpString( NULL ), iKey( sString.iKey ), iLen( sString.iLen )
{
	if( !sString.lpString )
		return;

	lpString = ALLOC_STRING( sString.lpString, iKey, iLen );
}

FString::FString( I32 iNum )
{
	CHAR_ cNum[MAX_STRING];

	itoa( iNum, cNum, 10 );
	iLen = strlen( cNum );
	iKey = GetCRC( cNum, iLen );

	lpString = ALLOC_STRING( cNum, iKey, iLen );
}

FString::FString( UI32 iNum )
{
	CHAR_ cNum[MAX_STRING];

	ultoa( iNum, cNum, 10 );
	iLen = strlen( cNum );
	iKey = GetCRC( cNum, iLen );

	lpString = ALLOC_STRING( cNum, iKey, iLen );
}

FString::FString( F32 fNum )
{
	CHAR_ cNum[MAX_STRING];

	gcvt( fNum, 10, cNum );
	iLen = strlen( cNum );
	iKey = GetCRC( cNum, iLen );

	lpString = ALLOC_STRING( cNum, iKey, iLen );
}

FString::~FString()
{
	FreeString();
}

void FString::FreeString()
{
	if( lpString )
		FREE_STRING( lpString, iKey );
	lpString = NULL;
	iLen = 0;
	iKey = 0;
}

FString & FString::operator = ( const FString & sString0 )
{
	FreeString();
	if( sString0.lpString == NULL )
		return *this;

	iKey = sString0.iKey;
	iLen = sString0.iLen;
	lpString = ALLOC_STRING( sString0.lpString, iKey, iLen );

	return *this;
}

FString & FString::operator = ( I32 iNum )
{
	FreeString();

	CHAR_ cNum[MAX_STRING];

	itoa( iNum, cNum, 10 );
	iLen = strlen( cNum );
	iKey = GetCRC( cNum, iLen );
	lpString = ALLOC_STRING( cNum, iKey, iLen );

	return *this;
}
FString & FString::operator = ( F32 fNum )
{
	FreeString();

	CHAR_ cNum[MAX_STRING];

	_gcvt( fNum, MAX_STRING, cNum );
	iLen = strlen( cNum );
	iKey = GetCRC( cNum, iLen );
	lpString = ALLOC_STRING( cNum, iKey, iLen );

	return *this;
}

bool FString::operator == ( const FString & sString )
{
	return lpString == sString.lpString;
}

bool FString::operator != ( const FString & sString )
{
	return lpString != sString.lpString;
}

bool FString::operator == ( const FString & sString )const
{
	return lpString == sString.lpString;
}

bool FString::operator != ( const FString & sString )const
{
	return lpString != sString.lpString;
}

FString & FString::operator += ( const FString & sString )
{
	if( (sString.iLen == 0) || (!sString.lpString) )
		return *this;

	UI32 iNewLen = iLen + sString.iLen;
	CHAR_ * lpStrNew = (CHAR_ *)PUSH_BLOCK( (iNewLen + 1)*sizeof( CHAR_ ) );
	if( lpString )
	{
		strncpy( lpStrNew, lpString, iLen );
		strncpy( lpStrNew + iLen, sString.lpString, sString.iLen + 1 );
		FreeString();
	}
	else
		strncpy( lpStrNew, sString.lpString, sString.iLen );

	iLen = iNewLen;
	iKey = GetCRC( lpStrNew, iLen );
	lpString = ALLOC_STRING( lpStrNew, iKey, iLen );

	POP_BLOCK;
	return *this;
}

FString & FString::operator += ( const CHAR_ * lpString0 )
{
	if( !lpString0 )
		return *this;

	UI32 iStrLen = strlen( lpString0 );
	if( iStrLen == 0 )
		return *this;

	UI32 iNewLen = iLen + iStrLen;
	CHAR_ * lpStrNew = (CHAR_ *)PUSH_BLOCK( (iNewLen + 1)*sizeof( CHAR_ ) );
	if( lpString )
	{
		strncpy( lpStrNew, lpString, iLen );
		strncpy( lpStrNew + iLen, lpString0, iStrLen + 1 );
		FreeString();
	}
	else
		strncpy( lpStrNew, lpString0, iStrLen );

	iLen = iNewLen;
	iKey = GetCRC( lpStrNew, iLen );
	lpString = ALLOC_STRING( lpStrNew, iKey, iLen );

	POP_BLOCK;
	return *this;
}

FString & FString::operator -= ( const FString & sString )
{
	if( (sString.iLen == 0) || (!lpString) || (!sString.lpString) || (sString.iLen > iLen) )
		return *this;

	CHAR_ * lpSub = strstr( lpString, sString.lpString );
	if( lpSub == NULL )
		return *this;

	I32 iNum = lpSub - lpString;
	UI32 iNewLen = iLen - sString.iLen;

	CHAR_ * lpStrNew = (CHAR_ *)PUSH_BLOCK( (iNewLen + 1)*sizeof( CHAR_ ) );

	strncpy( lpStrNew, lpString, iNum );
	strncpy( lpStrNew + iNum, lpString + iNum + sString.iLen, iNewLen - iNum + 1 );

	FreeString();
	iLen = iNewLen;
	iKey = GetCRC( lpStrNew, iLen );
	lpString = ALLOC_STRING( lpStrNew, iKey, iLen );

	POP_BLOCK;
	return *this;
}

FString & FString::operator += ( CHAR_ cChar )
{
	UI32 iNewLen = iLen + 1;
	CHAR_ * lpStrNew = (CHAR_ *)PUSH_BLOCK( (iNewLen + 1) * sizeof( CHAR_ ) );
	if( lpString )
	{
		strncpy( lpStrNew, lpString, iLen );
		lpStrNew[iNewLen - 1] = cChar;
		lpStrNew[iNewLen] = 0;
		FreeString();
	}
	else
	{
		lpString = lpStrNew;
		lpString[0] = cChar;
		lpString[1] = 0;
	}

	iLen = iNewLen;
	iKey = GetCRC( lpStrNew, iLen );
	lpString = ALLOC_STRING( lpStrNew, iKey, iLen );

	POP_BLOCK;
	return *this;
}

FString::operator I32 ()
{
	return atoi( lpString );
}

FString::operator F32 ()
{
	return (F32)atof( lpString );
}

I32 FString::Find( const CHAR_ * lpSubStr )
{
	CHAR_ * lpStr = strstr( lpString, lpSubStr );
	if( !lpStr )
		return -1;
	return lpStr - lpString - 1;
}

I32 FString::Find( const FString & sSubStr ) const
{
	CHAR_ * lpStr = strstr( lpString, sSubStr.GetChar() );
	return lpStr - lpString - 1;
}

UI32 FString::Length() const
{
	return iLen;
}

//void FString::SetSize( UI32 iSize )
//{
//	if( lpString )
//		FFree( lpString );
//	lpString = (CHAR_ *)FMalloc( iSize*sizeof( CHAR_ ) );
//	lpString[0] = 0;
//	iLen = iSize;
//}

inline void MirrorString( CHAR_ * lpStr, UI32 iLen )
{
	for( UI32 i = 0;i < iLen/2;i++ )
	{
		CHAR_ t = lpStr[iLen - i - 1];
		lpStr[iLen - i - 1] = lpStr[i];
		lpStr[i] = t;
	}
}

FString FString::GetExt( bool bPoint )const
{
	CHAR_ cBuffer[256];
	CHAR_ * lpStr = lpString;

	I32 iLen = Length() - 1,
		i = iLen;

	for(;i != -1; i-- )
	{
		if( lpStr[i] == '.' )
			break;

		cBuffer[iLen - i] = lpStr[i];
	}
	if( bPoint )
	{
		cBuffer[iLen - i] = '.';
		i--;
	}

	cBuffer[iLen - i] = 0;
	MirrorString( cBuffer, iLen - i );

	return FString( cBuffer );
}

FString FString::GetName()const
{
	I32 iStart = 0, iEnd = 0;
	I32 i = Length() - 1;

	for( ;i != -1;i-- )
	{
		if( lpString[i] == '.' )
		{
			iEnd = i;
			break;
		}
	}
	for(;i != -1;i-- )
	{
		if( lpString[i] == '\\' )
		{
			iStart = i + 1;
			break;
		}
	}

	CHAR_ cBuffer[256];
	for( i = 0;i < (iEnd - iStart);i++ )
		cBuffer[i] = lpString[i + iStart];
	cBuffer[i] = 0;

	return FString( cBuffer );
}

FString FString::GetPath()const
{
	CHAR_ * lpPath = (CHAR_ *)PUSH_BLOCK( (iLen + 1)*sizeof( CHAR_ ) );

	strncpy( lpPath, lpString, iLen + 1 );
	I32 i = iLen - 1;
	for(;i != -1;i-- )
	{
		if( lpString[i] == '\\' )
			break;
	}
	lpPath[i + 1] = 0;
	FString sPath( lpPath );

	POP_BLOCK;
	return sPath;
}

FString FString::GetMirrored()const
{
	CHAR_ * lpMirror = (CHAR_ *)PUSH_BLOCK( (iLen + 1)*sizeof( CHAR_ ) );
	strncpy( lpMirror, lpString, iLen + 1 );
	MirrorString( lpMirror, iLen );
	FString sMirror( lpMirror );
	POP_BLOCK;

	return sMirror;
}

void FString::Mirrored()
{
	CHAR_ * lpMirror = (CHAR_ *)PUSH_BLOCK( (iLen + 1)*sizeof( CHAR_ ) );
	strncpy( lpMirror, lpString, iLen + 1 );
	FreeString();

	MirrorString( lpMirror, iLen );
	iKey = GetCRC( lpMirror, iLen );
	lpString = ALLOC_STRING( lpMirror, iKey, iLen );

	POP_BLOCK;
}

UI32 FString::GetKey()const
{
	return iKey;
}

I32 SkipSpace( const CHAR_ * lpStr, I32 iStartInd )
{
	I32 iSkipLen = 0;
	while( lpStr[iStartInd] == ' ' || lpStr[iStartInd] != 0 )
	{
		iSkipLen++;
		iStartInd++;
	}

	return iSkipLen;
}

I32 ParseString( const CHAR_ * lpStr, FString * lpParsedStr, I32 iStartInd )
{
	I32 iLen;
	CHAR_ cBuffer[MAX_BUFFER];
	if( lpStr[iStartInd++] != '"' )
		return 0;
	while( lpStr[iStartInd] != '"' )
	{
		if( lpStr[iStartInd] == 0 )
			return iLen;

		cBuffer[iLen++] = lpStr[iStartInd++];
	}
	cBuffer[iLen] = 0;
	*lpParsedStr = cBuffer;

	return iLen;
}

I32 ParseInt( const CHAR_ * lpStr, I32 * lpInteger, I32 iStartInd )
{
	I32 iLen;
	CHAR_ cBuffer[MAX_BUFFER];

	while( (lpStr[iStartInd] != ' ') || (lpStr[iStartInd] != 0) )
		cBuffer[iLen++] = lpStr[iStartInd++];

	cBuffer[iLen] = 0;
	*lpInteger = atoi( cBuffer );

	return iLen;
}

I32 ParseFloat( const CHAR_ * lpStr, F32 * lpFloat, I32 iStartInd )
{
	I32 iLen;
	CHAR_ cBuffer[MAX_BUFFER];

	while( (lpStr[iStartInd] != ' ') || (lpStr[iStartInd] != 0) )
		cBuffer[iLen++] = lpStr[iStartInd++];

	cBuffer[iLen] = 0;
	*lpFloat = atof( cBuffer );

	return iLen;
}

void FString::ScanString( const CHAR_ * lpFormat, ... )
{
	va_list lpArgs = NULL;
	va_start( lpArgs, lpFormat );
	I32 iFCurSym = 0, iCurSym = 0;
	while( lpFormat[iFCurSym] != 0 )
	{
		if( lpFormat[iFCurSym++] == '%' )
		{
			void * lpArg = va_arg( lpArgs, void * );
			if( lpFormat[iFCurSym] == 0 )
				break;

			switch( lpFormat[iFCurSym++] )
			{
			case 'c':
				*(CHAR_ *)lpArg = lpString[iCurSym++];
				break;
			case 's':
				iCurSym += ParseString( lpString, (FString *)lpArg, iCurSym );
				break;
			case 'f':
				iCurSym += ParseFloat( lpString, (F32 *)lpArg, iCurSym );
				break;
			case 'i':
				iCurSym += ParseInt( lpString, (I32 *)lpArg, iCurSym );
				break;
			}

			iCurSym += SkipSpace( lpString, iCurSym );
		}
	}

	va_end( lpArgs );

}

void FString::Init()
{
}

void FString::Destroy()
{
	FStringTable::Destroy();
	FStringAllocator::Destroy();
}


FString FString::PrintString( const CHAR_ * lpFormat, ... )
{
	va_list lpArgs;
	va_start( lpArgs, lpFormat );
	CHAR_ cBuffer[2*MAX_BUFFER];
	vsprintf_s( cBuffer, lpFormat, lpArgs );
	va_end( lpArgs );

	return FString( cBuffer );
}