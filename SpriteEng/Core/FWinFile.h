#ifndef __FOS_FILE_H__
#define __FOS_FILE_H__

#include "FFile.h"

#ifdef WINDOWS_FAMILY

#include <windows.h>

class FWinFile : public FFile
{
	HANDLE hFile;
protected:
	virtual I32 Open();
	virtual void Close();

public:
	FWinFile( const FString & sName, UI32 iOpenFlags );
	~FWinFile();

	UI32 Read( void * lpData, UI32 iBytes2Read );
	UI32 Write( const void * lpData, UI32 iBytes2Write );
	
	void * GetData( void * lpBuffer );
	UI64 GetSize()const;
	UI64 SetPointer( UI64 iPoint );
	UI64 SetPointer( UI64 iOffset, UI32 iMoveMethod );
};

#endif

#endif