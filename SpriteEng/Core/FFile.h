#ifndef __FFILE_H__
#define __FFILE_H__

#include "FString.h"


class FFile
{
public:
	enum
	{
		F_FILE_OPEN_ERROR = 0x00000001,
		F_FILE_READ = 0x00000002,
		F_FILE_CREATE = 0x00000004,
		F_FILE_WRITE = 0x00000008,
		F_FILE_SHARE_READ = 0x00000010,
		F_FILE_SHARE_WRITE = 0x00000020
	};

	enum
	{
		F_FILE_BEGIN = 1,
		F_FILE_END,
		F_FILE_CURRENT
	};

protected:
	UI32 iFlags;
	FString sFileName;

	virtual I32 Open();
	virtual void Close();

public:
	FFile( const FString & sName, UI32 iOpenFlags );
	virtual ~FFile();

	bool OpenSucc()const;
	virtual UI32 Read( void * lpData, UI32 iBytes2Read ) = 0;
	virtual UI32 Write( const void * lpData, UI32 iBytes2Write ) = 0;
	
	virtual void * GetData( void * lpBuffer ) = 0;
	virtual UI64 GetSize()const = 0;
	virtual UI64 SetPointer( UI64 iOffset ) = 0;
	virtual UI64 SetPointer( UI64 iOffset, UI32 iMoveMethod ) = 0;

	const FString & GetName()const;

	static FFile * OpenFile( const FString & sName, UI32 iOpenFlags );
	static void CloseFile( FFile * lpFile );
	static void RemoveFile( const FString & sName );
};

#endif