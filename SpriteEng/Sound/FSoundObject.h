//
//  FSoundObject.h
//  Amazing Balls
//
//  Created by Верун on 30.06.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#ifndef Amazing_Balls_FSoundObject_h
#define Amazing_Balls_FSoundObject_h

#include "FString.h"

class FSound;
class FSoundManager;

class FSoundObject
{
    FSound * lpSound;
    FSoundManager * lpSoundManager;
    I32 iSourceInd;
    
public:
    FSoundObject( const FString & sFileName, FSoundManager * lpSoundManager );
    virtual ~FSoundObject();
    
    UI32 GetSoundID()const;
    I32 GetSourceInd()const;
    
    void SetSourceInd( I32 iSourceInd );
    
    void Play( bool bLoop = false );
    void Pause();
    void Stop();
};

#endif
