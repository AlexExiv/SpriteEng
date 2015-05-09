//
//  FSoundManager.cpp
//  Amazing Balls
//
//  Created by Верун on 30.06.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#include "FSoundManager.h"
#include "FSound.h"
#include "FSoundObject.h"
#import <UIKit/UIKit.h>
alBufferDataStaticProcPtr alBufferDataStatic = NULL;

bool LogOpenALError( const CHAR_ * lpFile, I32 iLine )
{
    ALenum aError;
    bool bErr = false;
    while( (aError = alGetError()) != AL_NO_ERROR )
    {
        NSLog( @"OpenAL: File - %s, Line - %ld, Code: %d", lpFile, iLine, aError );
        bErr = true;
    }
    
    return bErr;
}

FSoundManager::FSoundManager( UI32 iMaxSoundHash, I32 iDelState  ) : iMaxHash( iMaxSoundHash ), iMaxSources( 0 ), lpSources( NULL ), lpSoundHash( NULL ), lpDevice( NULL ), lpContext( NULL ), iDelState( iDelState )
{
    const ALCchar * lpDevices = alcGetString( NULL, ALC_DEVICE_SPECIFIER );
    NSLog( @"Dev: %s", lpDevices );
    
    lpDevice = alcOpenDevice( lpDevices );
    if( lpDevice )
    {
        lpContext = alcCreateContext( lpDevice, NULL );
        alcMakeContextCurrent( lpContext );
    }
    
    const ALCchar * lpName = "alBufferDataStatic";
    alBufferDataStatic = (alBufferDataStaticProcPtr)alcGetProcAddress( lpDevice, lpName );
    
    lpSoundHash = (Node **)malloc( sizeof( Node * )*iMaxHash );
    for( I32 i = 0;i < iMaxHash;i++ )
        lpSoundHash[i] = NULL;
    
    CreateSources();
}

FSoundManager::~FSoundManager()
{
    StopAll();
    Clear();
    ReleaseSources();
    free( lpSources );
    lpSources = NULL;
    free( lpSoundHash );
    lpSoundHash = NULL;
    
    alcDestroyContext( lpContext );
    alcCloseDevice( lpDevice );
}

bool FSoundManager::PlaySound( const FString & sFileName, bool bLoop )
{
    FSoundObject * lpSound = new FSoundObject( sFileName, this );
    if( PlaySound( lpSound, bLoop ) )
    {
        lSoundList.PushBack( lpSound );
    }
    else
    {
        delete lpSound;
        return false;
    }
    
    return true;
}

bool FSoundManager::PlaySound( FSoundObject * lpSoundObj, bool bLoop )
{
    I32 i = GetFreeSource();
    if( i == -1 )
        return false;
    
    alSourcei( lpSources[i].iSourceID, AL_BUFFER, lpSoundObj->GetSoundID() );
    LogOpenAL();
    alSourcef( lpSources[i].iSourceID, AL_PITCH, 1.f );
    LogOpenAL();
    alSourcef( lpSources[i].iSourceID, AL_GAIN, 1.f );
    LogOpenAL();
    
    if( bLoop )
        alSourcei( lpSources[i].iSourceID, AL_LOOPING, AL_TRUE );
    else
        alSourcei( lpSources[i].iSourceID, AL_LOOPING, AL_FALSE );
    LogOpenAL();
    
    alSourcei( lpSources[i].iSourceID, AL_SOURCE_RELATIVE, AL_TRUE );
    LogOpenAL();
    alSourcef( lpSources[i].iSourceID, AL_ROLLOFF_FACTOR, 0.f );
    LogOpenAL();
    alSourcePlay( lpSources[i].iSourceID );
    LogOpenAL();
    
    lpSources[i].bBusy = true;
    lpSources[i].lpSoundObj = lpSoundObj;
    lpSoundObj->SetSourceInd( i );
    
    return true;
}

bool FSoundManager::StopSound( FSoundObject * lpSoundObj )
{
    if( !lpSoundObj )
        return false;
    
    I32 i = lpSoundObj->GetSourceInd();
    if( i < 0 || i > iMaxSources )
        return false;
    alSourceStop( lpSources[i].iSourceID );
    if( LogOpenAL() )
        NSLog( @"SourceID %d Index %ld", lpSources[i].iSourceID, i );
    
    lpSources[i].bBusy = false;
    lpSoundObj->SetSourceInd( -1 );
    lpSources[i].lpSoundObj = NULL;
    alSourcei( lpSources[i].iSourceID, AL_BUFFER, 0 );
    LogOpenAL();
    
    return true;
}

void FSoundManager::Update( F32 fDTime )
{
    FSoundIterator iIt = lSoundList.Begin();
    for( ;iIt != lSoundList.End(); )
    {
        ALint iSourceState;
        I32 iInd = iIt->GetSourceInd();
        alGetSourcei( lpSources[iInd].iSourceID, AL_SOURCE_STATE, &iSourceState );
        
        if( LogOpenAL() )
            NSLog( @"SourceID %d Index %ld", lpSources[iInd].iSourceID, iInd );
        
        if( (iSourceState == AL_PLAYING) || (iSourceState == AL_PAUSED) )
        {
            iIt++;
            continue;
        }
        
        delete *iIt;
        iIt = lSoundList.Erase( iIt );
    }
    
/*    for( I32 i = 0;i < iMaxSources;i++ )
    {
        ALint iSourceState;
        if( !lpSources[i].bBusy )
            continue;
        
        alGetSourcei( lpSources[i].iSourceID, AL_SOURCE_STATE, &iSourceState );
        LogOpenAL();
        if( (iSourceState == AL_PLAYING) || (iSourceState == AL_PAUSED) )
            continue;
        
        StopSound( lpSources[i].lpSoundObj );
        LogOpenAL();
    }*/
}

void FSoundManager::PauseAll()
{
    for( I32 i = 0;i < iMaxSources;i++ )
    {
        if( !lpSources[i].bBusy )
            continue;
        
        alSourcePause( lpSources[i].iSourceID );
        LogOpenAL();
    }
}

void FSoundManager::StopAll()
{
    for( I32 i = 0;i < iMaxSources;i++ )
    {
        if( !lpSources[i].bBusy )
            continue;
        
        StopSound( lpSources[i].lpSoundObj );
        LogOpenAL();
    }
}

void FSoundManager::ResumeAll()
{
    for( I32 i = 0;i < iMaxSources;i++ )
    {
        if( !lpSources[i].bBusy )
            continue;
        
        alSourcePlay( lpSources[i].iSourceID );
        LogOpenAL();
    }
}


FSound * FSoundManager::CreateSound( const FString & sFileName, bool bDel )
{
    Node * lpNode = Find( sFileName );
    if( lpNode )
    {
        lpNode->lpSound->AddRef();
        return lpNode->lpSound;
    }
    
    I32 iKey = GetKey( sFileName.GetChar() );
    Node * lpNewNode = AllocNode( iKey );
    FSound * lpSound = new FSound( sFileName, bDel );
    lpNewNode->lpSound = lpSound;
    
    return lpSound;
}

void FSoundManager::ReleaseSound( FSound * lpSound )
{
    lpSound->DecrRef();
    if( iDelState == DEL_NEVER )
        return;
    
    if( (!lpSound->IsRef() && lpSound->IsDel()) || (iDelState == DEL_ALWAYS) )
    {
        Node * lpNode = Find( lpSound->GetName() );
        if( !lpNode )
            return;
        
        I32 iKey = GetKey( lpSound->GetName() );
        delete lpNode->lpSound;
        
        if( lpNode == lpSoundHash[iKey] )
        {
            delete lpSoundHash[iKey];
            lpSoundHash[iKey] = NULL;
        }
        else
        {
            if( lpNode->lpNext )
                lpNode->lpNext->lpPrev = lpNode->lpPrev;
            if( lpNode->lpPrev )
                lpNode->lpPrev->lpNext = lpNode->lpNext;
            
            delete lpNode;
        }
    }
}

void FSoundManager::Clear()
{
    for( I32 i = 0;i < iMaxHash;i++ )
    {
        Node * lpNode = lpSoundHash[i];
        while( lpNode )
        {
            Node * lpTmp = lpNode->lpPrev;
            delete lpNode->lpSound;
            delete lpNode;
            lpNode = lpTmp;
        }
    }
}


void FSoundManager::CreateSources()
{
/*    ALCint iSize;
    alcGetIntegerv( lpDevice, ALC_ATTRIBUTES_SIZE, sizeof( iSize ), &iSize );
    ALCint * lpAttribs = (ALCint *)malloc( iSize );
    
    ALCint iMaxStereo = 0, iMaxMono = 0;
    alcGetIntegerv( lpDevice, ALC_STEREO_SOURCES, sizeof( ALCint ), &iMaxStereo );
    alcGetIntegerv( lpDevice, ALC_MONO_SOURCES, sizeof( ALCint ), &iMaxMono );
 
    for( I32 i = 0;i < iSize/(sizeof( ALCint ));i += 2 )
    {
        if( lpAttribs[i] == ALC_STEREO_SOURCES )
            iMaxStereo = lpAttribs[i + 1];
        if( lpAttribs[i] == ALC_MONO_SOURCES )
            iMaxMono = lpAttribs[i + 1];
    }
*/
    iMaxSources = 16;
    lpSources = (FSource *)malloc( sizeof( FSource )*iMaxSources );
    for( I32 i = 0;i < iMaxSources;i++ )
    {
        alGenSources( 1, &lpSources[i].iSourceID );
        LogOpenAL();
        lpSources[i].bBusy = false;
        lpSources[i].lpSoundObj = NULL;
    }
}

void FSoundManager::ReleaseSources()
{
    for( I32 i = 0;i < iMaxSources;i++ )
    {
        if( lpSources[i].bBusy )
             StopSound( lpSources[i].lpSoundObj );

        
        alDeleteSources( 1, &lpSources[i].iSourceID );
        LogOpenAL();
    }
}

I32 FSoundManager::GetFreeSource()
{
    for( I32 i = 0;i < iMaxSources;i++ )
        if( !lpSources[i].bBusy )
            return i;
    return -1;
}

I32 FSoundManager::GetKey( const FString & sFileName )
{
    I32 iSum = 0;
    for( I32 i = 0;i < sFileName.Length();i++ )
        iSum += sFileName[i];
    
    return iSum % iMaxHash;
}

FSoundManager::Node * FSoundManager::AllocNode( I32 iKey )
{
    Node * lpEnd = lpSoundHash[iKey];
    while( lpEnd )
        lpEnd = lpEnd->lpNext;
    
    Node * lpNewNode = new Node;
    lpNewNode->lpPrev = lpEnd;
    if( lpEnd )
        lpEnd->lpNext = lpNewNode;
    else
        lpSoundHash[iKey] = lpNewNode;
    lpNewNode->lpNext = NULL;
    
    return lpNewNode;
}

FSoundManager::Node * FSoundManager::Find( const FString & sFileName )
{
    I32 iKey = GetKey( sFileName.GetChar() );
    Node * lpNode = lpSoundHash[iKey];
    while( lpNode )
    {
        if( lpNode->lpSound->GetName() == sFileName )
            return lpNode;
        lpNode = lpNode->lpNext;
    }
    
    return NULL;
}