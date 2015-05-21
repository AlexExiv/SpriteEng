#include "FWinFile.h"

#ifdef WINDOWS_FAMILY

#include <fileapi.h>
#include "FLog.h"



FFile * FFile::OpenFile( const FString & sName, UI32 iOpenFlags )
{
	FFile * lpFile = new FWinFile( sName, iOpenFlags );
	if( lpFile->OpenSucc() )
		return lpFile;
	delete lpFile;
	return NULL;
}

void FFile::CloseFile( FFile * lpFile )
{
	delete lpFile;
}

void FFile::RemoveFile( const FString & sName )
{
	DeleteFile( sName.GetChar() );
}

I32 FWinFile::Open()
{
	DWORD dwAccessFlags = 0, dwShareFlags = 0, dwCreateFlags = OPEN_EXISTING;
	if( iFlags & F_FILE_READ )
		dwAccessFlags |= GENERIC_READ;
	if( iFlags & F_FILE_WRITE )
		dwAccessFlags |= GENERIC_WRITE;
	if( iFlags & F_FILE_CREATE )
		dwCreateFlags = CREATE_ALWAYS;
	if( iFlags & F_FILE_SHARE_READ )
		dwShareFlags |= FILE_SHARE_READ;
	if( iFlags & F_FILE_SHARE_WRITE )
		dwShareFlags |= FILE_SHARE_WRITE;

	hFile = CreateFile( sFileName.GetChar(), dwAccessFlags, dwShareFlags, NULL, dwCreateFlags, FILE_ATTRIBUTE_NORMAL, NULL );
	if( hFile == INVALID_HANDLE_VALUE )
	{
		return -1;
	}
	return 1;
}

void FWinFile::Close()
{
	if( hFile != INVALID_HANDLE_VALUE )
	{
		CloseHandle( hFile );
		hFile = INVALID_HANDLE_VALUE;
	}
}

FWinFile::FWinFile( const FString & sName, UI32 iOpenFlags ) : FFile( sName, iOpenFlags ), hFile( INVALID_HANDLE_VALUE )
{
	if( Open() == -1 )
		iFlags |= F_FILE_OPEN_ERROR;
}

FWinFile::~FWinFile()
{
	Close();
}

UI32 FWinFile::Read( void * lpData, UI32 iBytes2Read )
{
	AssertFatal( hFile != INVALID_HANDLE_VALUE, "Handle value is NULL can't read file" );

	DWORD dwRead = 0;
	ReadFile( hFile, lpData, iBytes2Read, &dwRead, NULL );

	return dwRead;
}

UI32 FWinFile::Write( const void * lpData, UI32 iBytes2Write )
{
	AssertFatal( hFile != INVALID_HANDLE_VALUE, "Handle value is NULL can't write file" );

	DWORD dwWrite = 0;
	WriteFile( hFile, lpData, iBytes2Write, &dwWrite, NULL );

	return dwWrite;
}


void * FWinFile::GetData( void * lpBuffer )
{
	SetPointer( 0 );
	Read( lpBuffer, GetSize() );

	return lpBuffer;
}

UI64 FWinFile::GetSize()const
{
	LARGE_INTEGER sLarge;
	FILE_STANDARD_INFO sInfo = { 0 };
	GetFileInformationByHandleEx( hFile, FileStandardInfo, (void *)&sInfo, sizeof( sInfo ) );
	return sInfo.EndOfFile.QuadPart;
}

UI64 FWinFile::SetPointer( UI64 iPoint )
{
	LARGE_INTEGER sMove, sMoved;
	sMove.QuadPart = iPoint;
	SetFilePointerEx( hFile, sMove, &sMoved, FILE_BEGIN );

	return sMoved.QuadPart;
}

UI64 FWinFile::SetPointer( UI64 iPoint, UI32 iMoveMethod )
{
	LARGE_INTEGER sMove, sMoved;
	sMove.QuadPart = iPoint;
	DWORD dwMoveMethod = 0;
	switch( iMoveMethod )
	{
	case F_FILE_BEGIN:
		dwMoveMethod = FILE_BEGIN;
		break;
	case F_FILE_END:
		dwMoveMethod = FILE_END;
		break;
	case F_FILE_CURRENT:
		dwMoveMethod = FILE_CURRENT;
		break;
	default:
		AssertFatal( false, "Unknown case move method" );
	};
	SetFilePointerEx( hFile, sMove, &sMoved, FILE_BEGIN );

	return sMoved.QuadPart;
}


#endif