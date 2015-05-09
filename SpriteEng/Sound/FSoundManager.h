//
//  FSoundManager.h
//  Amazing Balls
//
//  Created by Верун on 30.06.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#ifndef Amazing_Balls_FSoundManager_h
#define Amazing_Balls_FSoundManager_h

#include "FString.h"
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#include "FList.h"
#include <OpenAl/oalStaticBufferExtension.h>


class FSound;
class FSoundObject;

class FSoundManager
{
public:
    enum
    {
        DEL_NEVER = 0,
        DEL_BY_IMG_CFG,
        DEL_ALWAYS
    };
    
private:
    struct Node
    {
        Node * lpPrev, * lpNext;
        FSound * lpSound;
    };
    
    struct FSource
    {
        ALuint iSourceID;
        bool bBusy;
        FSoundObject * lpSoundObj;
    };
    
    FSource * lpSources;
    UI32 iMaxSources;
    
    Node ** lpSoundHash;
    UI32 iMaxHash;
    
    I32 iDelState;
    
    I32 GetKey( const FString & sFileName );
    Node * Find( const FString & sFileName );
    Node * AllocNode( I32 iKey );
    
    void CreateSources();
    void ReleaseSources();
    I32 GetFreeSource();
    
    ALCdevice * lpDevice;
    ALCcontext * lpContext;
    
    typedef FList<FSoundObject *> FSoundList;
    typedef FList<FSoundObject *>::Iterator FSoundIterator;
    
    FSoundList lSoundList;
    
public:
    FSoundManager( UI32 iMaxSoundHash, I32 iDelState );
    virtual ~FSoundManager();
    
    bool PlaySound( const FString & sFileName, bool bLoop = false );
    bool PlaySound( FSoundObject * lpSoundObj, bool bLoop = false );
    bool StopSound( FSoundObject * lpSoundObj );
    void PauseAll();
    void StopAll();
    void ResumeAll();
    
    void Update( F32 fDTime );
    
    FSound * CreateSound( const FString & sFileName, bool bDel = true );
    void ReleaseSound( FSound * lpSounmd );
    void Clear();
};

extern alBufferDataStaticProcPtr alBufferDataStatic;

#endif
