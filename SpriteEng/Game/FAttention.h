//
//  FAttention.h
//  Arithmetic Numbers
//
//  Created by Верун on 13.09.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#ifndef Arithmetic_Numbers_FAttention_h
#define Arithmetic_Numbers_FAttention_h

#include "FGameObject.h"

class FObject2D;
class FImage2D;
class FArithWorld;

class FAttention : public FGameObject
{
    enum
    {
        OBJECT_WAIT = OBJECT_DEAD + 1,
        OBJECT_PREPARE,
        OBJECT_COUNTER,
        OBJECT_HOLDON,
        OBJECT_EVENT_STARTED,
        OBJECT_ATTACK_SOON,
        OBJECT_WAIT_FOR_ATTACK
    };
    
    FObject2D * lpCounter;
    FImage2D * lpPrepare, * lpHoldOn, * lpAttackSoon;
    F32 fCurTime, fEventTime;
    UI32 iLastCount;
    F32 fNextAttackTime, fNextAttackTimeM;
    
    void NextLoop();
    void StartEvent();
    
public:
	FAttention( const FVector2F & vPos, FArithWorld * lpWorld );
    ~FAttention();
    
    F32 GetWidth()const;
    F32 GetHeight()const;
    
    void Update( F32 fDTime );
    
    UI32 Load( void * lpData );
    void Save( FFile * lpFile );
    
    bool IsEventStart();
    bool IsEventEnd();
    void ResetState();
    void SetNextAttackTime( F32 fNextAttackTime );
};

#endif
