//
//  FSound.h
//  Amazing Balls
//
//  Created by Верун on 30.06.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#ifndef Amazing_Balls_FSound_h
#define Amazing_Balls_FSound_h

#include "FString.h"
#include <OpenAL/al.h>


class FSound
{
    ALuint iSoundID;
    UI32 iSecLen;
    FString sFileName;
    bool bDel;
    I32 iRefCount;
    void * lpData;
    
public:
    FSound( const FString & sFileName, bool bDel = true );
    virtual ~FSound();
    
    UI32 GetLen()const;
    const FString & GetName()const;
    ALuint GetSoundID()const;
    
    void AddRef();
    void DecrRef();
    bool IsRef()const;
    bool IsDel()const;
    
};

#endif
