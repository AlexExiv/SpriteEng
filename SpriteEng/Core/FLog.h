#ifndef __FLOG_H__
#define __FLOG_H__

#include "..\types.h"
#include <stdarg.h>


//������� ����� ������ ��� ���������, ��� ������ ��� ����������� �� ���� � ������������� ����������� � ��� ���� ��� ������
class FLogOutput
{
protected:
	UI32 iLogFlags;

public:
	enum
	{
		LOG_MESSAGE = 0x00000001,
		LOG_ERROR = 0x00000002,
		LOG_WARNING = 0x00000004
	};


	FLogOutput( UI32 iLogFlags );
	virtual ~FLogOutput();

	bool CheckFlag( UI32 iFlag )const;
	virtual void PutString( const CHAR_ * lpFormat, va_list lpArgs ) = 0;//������ ������� ������ ������ ���������� �� ����� ������ ��� ������� ����� ��������� ��������
};

namespace FLog
{
	void Init();
	void Destroy();

	void PutMessage( const CHAR_ * lpFormat, ... );//����� �������� ���������, ������� �� �����������
	void PutError( const CHAR_ * lpFormat, ... );//����� ��������� �� ������, � ������ ��������� ����������� ������� "Error: "
	void PutWarning( const CHAR_ * lpFormat, ... );//����� ��������� �� ������, � ������ ��������� ����������� ������� "Message: "
};

#include "FException.h"

#ifdef _DEBUG
#define AssertFatal( bCond, lpStr ) if( !(bCond) )\
{\
	FLog::PutError( "Debug %s at file %s, at line %i", lpStr, __FILE__, __LINE__ );\
	throw FException( FException::EXCP_FATAL_ERROR, FString( "Fatal error, application stopped" ) );\
}

#else
#define AssertFatal( bCond, lpStr )
#endif

#endif