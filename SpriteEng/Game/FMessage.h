//
//  FMessage.h
//  Arithmetic Numbers
//
//  Created by Верун on 05.01.15.
//  Copyright (c) 2015 Верун. All rights reserved.
//

#ifndef __Arithmetic_Numbers__FMessage__
#define __Arithmetic_Numbers__FMessage__

#include "FGameObject.h"

class FText;
class FArithWorld;

class FMessage : public FGameObject
{
    FText * lpLabel;
    F32 fCounterTime, fCurTime;
    
    
    enum
    {
        MESSAGE_SHOWING = 10,
        MESSAGE_MOVE_TO_POS,
        MESSAGE_COUNTER,
        MESSAGE_HIDDING,
        MESSAGE_WAIT
    };
    
public:
    FMessage( FArithWorld * lpWorld );
    ~FMessage();
    
    void BeginTimer( F32 fTime );
    bool IsEnd()const;
    
    F32 GetWidth()const;
    F32 GetHeight()const ;
    
	void Move( const FVector2F & vPos );
    void Update( F32 fDTime );
    
    virtual UI32 Load( void * lpData );
    virtual void Save( FFile * lpFile );
};

#endif /* defined(__Arithmetic_Numbers__FMessage__) */
