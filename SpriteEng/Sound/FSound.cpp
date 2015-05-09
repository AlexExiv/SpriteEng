//
//  FSound.cpp
//  Amazing Balls
//
//  Created by Верун on 30.06.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#include "FSound.h"
#include "FAudioFile.h"
#include "FSoundManager.h"


FSound::FSound( const FString & sFileName, bool bDel ) : iRefCount( 0 ), bDel( bDel ), sFileName( sFileName )
{
    alGenBuffers( 1, &iSoundID );
    LogOpenAL();
    
    FAudioFile * lpFile = FAudioFile::CreateFile( sFileName );
    iSecLen = lpFile->GetFileLenInSec();
    ALenum aFormat = 0;
    switch( lpFile->GetDataFormat() )
    {
        case MONO_8BITS:
            aFormat = AL_FORMAT_MONO8;
            break;
        case STEREO_8BITS:
            aFormat = AL_FORMAT_STEREO8;
            break;
        case MONO_16BITS:
            aFormat = AL_FORMAT_MONO16;
            break;
        case STEREO_16BITS:
            aFormat = AL_FORMAT_STEREO16;
            break;
    }

    lpData = malloc( lpFile->GetDataSize() );
    memcpy( lpData, lpFile->GetData(), lpFile->GetDataSize() );
    
    alBufferData( iSoundID, AL_FORMAT_STEREO16, lpFile->GetData(), lpFile->GetDataSize(), lpFile->GetSampleRate() );

    LogOpenAL();
    
    delete lpFile;
}

FSound::~FSound()
{
    alDeleteBuffers( 1, &iSoundID );
    LogOpenAL();
    free( lpData );
}

UI32 FSound::GetLen()const
{
    return iSecLen;
}

const FString & FSound::GetName()const
{
    return sFileName;
}

ALuint FSound::GetSoundID()const
{
    return iSoundID;
}

void FSound::AddRef()
{
    iRefCount++;
}

void FSound::DecrRef()
{
    iRefCount--;
}

bool FSound::IsRef()const
{
    return iRefCount != 0;
}

bool FSound::IsDel()const
{
    return bDel;
}
