#include "FException.h"

FException::FException(  UI32 iCode, const FString & sMessage ) : iCode( iCode ), sMessage( sMessage )
{
}

FException::~FException()
{
}

UI32 FException::GetCode()const
{
	return iCode;
}

FString FException::GetMess()const
{
	return sMessage;
}