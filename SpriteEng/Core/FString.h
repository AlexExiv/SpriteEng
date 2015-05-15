#ifndef __FSTRING_H__
#define __FSTRING_H__

#include "..\types.h"

class FString
{
	CHAR_ * lpString;
	UI32 iKey, iLen;

	void FreeString();

public:
	FString();
	FString( const CHAR_ * lpString );
	FString( CHAR_ cChar );
	FString( const FString & sString );
	FString( I32 iNum );
	FString( UI32 iNum );
	FString( F32 fNum );
	~FString();

	FString & operator = ( const FString & sString );
	FString & operator = ( I32 iNum );
	FString & operator = ( F32 fNum );

	bool operator == ( const FString & sString );
	bool operator != ( const FString & sString );

	bool operator == ( const FString & sString )const;
	bool operator != ( const FString & sString )const;

	FString & operator += ( const FString & sString );
	FString & operator += ( const CHAR_ * lpString );
	FString & operator -= ( const FString & sString );
	FString & operator += ( CHAR_ cChar );
	
	operator CHAR_ * ()
	{
		return lpString;
	}
	CHAR_ * GetChar() const
	{
		return lpString;
	}

	CHAR_ operator [] ( I32 iIndex )
	{
		return lpString[iIndex];
	}

	CHAR_ operator [] ( I32 iIndex )const
	{
		return lpString[iIndex];
	}

	operator I32 ();
	operator F32 ();

	UI32 Length()const;
	//void SetSize( UI32 iSize );

	I32 Find( const CHAR_ * lpSubStr );
	I32 Find( const FString & sSubStr )const;

	//void SetChar( I32 iPos, CHAR_ cChar )
	//{
	//	if( iPos < 0 || iPos >= Length() )
	//		return;

	//	lpString[iPos] = cChar;
	//	ComputeKey();
	//}

	FString GetExt( bool bPoint = false )const;//получить расширение файла
	FString GetName()const;//получить имя файла
	FString GetPath()const;//получить путь к файлу
	FString GetMirrored()const;//зеркало строки
	FString AppendToName( const CHAR_ * lpStr )const;//добавить к имени файла с конца

	void Mirrored();
	UI32 GetKey()const;
	void ScanString( const CHAR_ * lpFormat, ... );
	static FString PrintString( const CHAR_ * lpFormat, ... );

	static void Init();
	static void Destroy();
};

inline FString operator + ( const FString & sStr0, const FString & sStr1 )
{
	return FString( sStr0 ) += sStr1;
}

inline FString operator + ( const CHAR_ * lpStr0, const FString & sStr1 )
{
	return FString( lpStr0 ) += sStr1;
}

inline FString operator + ( const FString & sStr0, const CHAR_ * lpStr1 )
{
	return FString( sStr0 ) += lpStr1;
}

#endif