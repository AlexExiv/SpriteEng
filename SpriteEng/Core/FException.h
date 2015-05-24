#ifndef __FEXCEPTION_H__
#define __FEXCEPTION_H__


#include "..\Core\FString.h"
#include <stdarg.h>

//class FException;
//typedef UI32 (* FExceptionHandler)( FException * lpExcp, const CHAR_ * lpFormat, va_list lpArgs ) ;

class FException
{
public:
	enum
	{
		EXCP_FILE_NOT_FOUND = 1,
		EXCP_UNK_FILE_FORMAT,
		EXCP_LOOP_ALLOC_NEEDMORE,
		EXCP_STACK_OVERFLOW,
		EXCP_DECOMPR_FAILURE,
		EXCP_UNK_IMAGE_FORMAT,
		EXCP_RESALLOC_INSUFFIENC,
		EXCP_OBJALLOC_OVERHEAD,
		EXCP_FATAL_ERROR
	};

private:
	//FExceptionHandler fHandlerFunc;
	UI32 iCode;
	FString sMessage;

public: 
	//FException( FExceptionHandler fHandlerFunc, UI32 iCode );
	FException( UI32 iCode, const FString & sMessage );
	~FException();

	UI32 GetCode()const;
	FString GetMess()const;
 //   UI32 Process( const CHAR_ * lpFormat, ... );
};

#endif