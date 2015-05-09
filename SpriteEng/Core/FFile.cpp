#include "FFile.h"


I32 FFile::Open()
{
	return -1;
}

void FFile::Close()
{
}

FFile::FFile( const FString & sName, UI32 iOpenFlags ): sFileName( sName ), iFlags( iOpenFlags )
{
	//if( this->Open() == -1 )
	//	iFlags |= FILE_OPEN_ERROR;
} 

FFile::~FFile()
{
	Close();
}

const FString & FFile::GetName()const
{
	return sFileName;
}

bool FFile::OpenSucc()const
{
	return !(iFlags & F_FILE_OPEN_ERROR);
}
