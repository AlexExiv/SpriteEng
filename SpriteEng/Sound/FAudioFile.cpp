//
//  FAudioFile.cpp
//  Amazing Balls
//
//  Created by Верун on 28.06.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#include "FAudioFile.h"
#include "FCAFFile.h"


FAudioFile::FAudioFile() : iDataSize( 0 ), iBits( 0 ), iByteRate( 0 ), iNumChannels( 0 ), iSampleRate( 0 ), iSecLen( 0 )
{
    
}

FAudioFile::~FAudioFile()
{
    if( lpData )
        free( lpData );
}

UI32 FAudioFile::GetSampleRate()const
{
    return iSampleRate;
}

UI32 FAudioFile::GetNumChannels()const
{
    return iNumChannels;
}

UI32 FAudioFile::GetByteRate()const
{
    return iByteRate;
}

UI32 FAudioFile::GetBitsPerSample()const
{
    return iBits;
}

UI32 FAudioFile::GetDataSize()const
{
    return iDataSize;
}

UI32 FAudioFile::GetFileLenInSec()const
{
    return iSecLen;
}

UI32 FAudioFile::GetDataFormat()const
{
    return iDataFormat;
}

void * FAudioFile::GetData()
{
    return lpData;
}

FAudioFile * FAudioFile::CreateFile( const FString & sFileName )
{
    I32 iExtCode = GetExtCode( sFileName );
    switch( iExtCode )
    {
        case EXT_CAF:
            return new FCAFFile( sFileName );
    }
    
    return NULL;
}

UI32 FAudioFile::GetDataFormat( UI32 iNumChannels, UI32 iBits )
{
    if( !((iNumChannels == 1) || (iNumChannels == 2)) )
        return 0;
    
    if( !((iBits != 8) || (iBits != 16)) )
        return 0;
    
    UI32 iFormat = 0;
    
    if( iNumChannels == 1 )
    {
        if( iBits == 8 )
            iFormat = MONO_8BITS;
        else
            iFormat = MONO_16BITS;
    }
    else
    {
        if( iBits == 8 )
            iFormat = STEREO_8BITS;
        else
            iFormat = STEREO_16BITS;
    }
    
    return iFormat;
}

