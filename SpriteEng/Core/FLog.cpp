#include "FLog.h"
#include "FList.h"
#include "FStack.h"
#include <string.h>

#define MAX_MESSAGE 512

#ifdef WINDOWS_FAMILY

#include <Windows.h>

//стандартная отладочная консоль Visual C++
class FDebugConsole : public FLogOutput
{
public:
	FDebugConsole();
	~FDebugConsole();

	void PutString( const CHAR_ * lpFormat, va_list lpArgs );
};

FDebugConsole::FDebugConsole() : FLogOutput( FLogOutput::LOG_ERROR | FLogOutput::LOG_WARNING | FLogOutput::LOG_MESSAGE )
{
}

FDebugConsole::~FDebugConsole()
{
}

void FDebugConsole::PutString( const CHAR_ * lpFormat, va_list lpArgs )
{
	CHAR_ cBuffer[MAX_MESSAGE];

	vsprintf_s( cBuffer, lpFormat, lpArgs );
	OutputDebugString( cBuffer );
}

#endif


namespace FLog
{
	void RegLogOutput( FLogOutput * lpLog );//добавление лог вывода в список
	void DeRegLogOutput( FLogOutput * lpLog );//удаление лог вывода из списока
};


FLogOutput::FLogOutput( UI32 iLogFlags ) : iLogFlags( iLogFlags )
{
	FLog::RegLogOutput( this );
}

FLogOutput::~FLogOutput()
{
	FLog::DeRegLogOutput( this );
}

bool FLogOutput::CheckFlag( UI32 iFlag )const
{
	return iLogFlags & iFlag;
}



namespace FLog
{
	const CHAR_ * lpErrorL = "Error: ",
		* lpWarningL = "Warning: ";
#ifdef WINDOWS_FAMILY
	FDebugConsole * lpDebugConsole = NULL;
#endif

	UI32 iErrLen = strlen( lpErrorL ),
		iWarnLen = strlen( lpWarningL );
	UI32 iEndLen = strlen( lpEndL );

	typedef FList<FLogOutput *, FFreePointer__<FLogOutput *>> FLogList;
	typedef FList<FLogOutput *, FFreePointer__<FLogOutput *>>::Iterator FLogIterator;

	FLogList lLogList;

	void Init()
	{
#ifdef WINDOWS_FAMILY
		lpDebugConsole = new FDebugConsole();
#endif
	}

	void Destroy()
	{
#ifdef WINDOWS_FAMILY
		delete lpDebugConsole;
#endif
	}

	void RegLogOutput( FLogOutput * lpLog )
	{
		lLogList.PushBack( lpLog );
	}

	void DeRegLogOutput( FLogOutput * lpLog )
	{
		FLogIterator iIt = lLogList.Begin();
		for(;iIt != lLogList.End();iIt++ )
		{
			if( (*iIt) == lpLog )
			{
				lLogList.Erase( iIt );
				break;
			}
		}
	}

	inline void PutMessageInAll( const CHAR_ * lpFormat, va_list lpArgs, UI32 iMessType )
	{
		FLogIterator iIt = lLogList.Begin();
		for(;iIt != lLogList.End();iIt++ )
		{
			if( (*iIt)->CheckFlag( iMessType ) )
				(*iIt)->PutString( lpFormat, lpArgs );
		}
	}

	void PutMessage( const CHAR_ * lpFormat, ... )
	{
		va_list lpArgs = NULL;
		va_start( lpArgs, lpFormat );

		CHAR_ cBuffer[MAX_MESSAGE];
		UI32 iLen = strlen( lpFormat );
		if( (iLen + iEndLen + 1) >= MAX_MESSAGE )
			iLen -= (iLen + iEndLen - MAX_MESSAGE + 2);
		strncpy_s( cBuffer, lpFormat, iLen );
		strncpy( &cBuffer[iLen], lpEndL, iEndLen + 1 );

		PutMessageInAll( cBuffer, lpArgs, FLogOutput::LOG_MESSAGE );
		POP_BLOCK;
	}

	void PutError( const CHAR_ * lpFormat, ... )
	{
		va_list lpArgs = NULL;
		va_start( lpArgs, lpFormat );

		CHAR_ cBuffer[MAX_MESSAGE];
		UI32 iLen = strlen( lpFormat );
		if( (iLen + iEndLen + 1) >= MAX_MESSAGE )
			iLen -= (iLen + iEndLen - MAX_MESSAGE + 2);
		strncpy( cBuffer, lpErrorL, iErrLen );
		strncpy( &cBuffer[iErrLen], lpFormat, iLen );
		strncpy( &cBuffer[iErrLen + iLen], lpEndL, iEndLen + 1 );

		PutMessageInAll( cBuffer, lpArgs, FLogOutput::LOG_ERROR );
		va_end( lpArgs );
	}

	void PutWarning( const CHAR_ * lpFormat, ... )
	{
		va_list lpArgs = NULL;
		va_start( lpArgs, lpFormat );

		CHAR_ cBuffer[MAX_MESSAGE];
		UI32 iLen = strlen( lpFormat );
		if( (iLen + iEndLen + 1) >= MAX_MESSAGE )
			iLen -= (iLen + iEndLen - MAX_MESSAGE + 2);
		strncpy( cBuffer, lpWarningL, iWarnLen );
		strncpy( &cBuffer[iWarnLen], lpFormat, iLen );
		strncpy( &cBuffer[iWarnLen + iLen], lpEndL, iEndLen + 1 );

		PutMessageInAll( cBuffer, lpArgs, FLogOutput::LOG_WARNING );
		va_end( lpArgs );
		POP_BLOCK;
	}
};