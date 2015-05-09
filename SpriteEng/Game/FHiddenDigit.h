//
//  FHiddenDigit.h
//  Arithmetic Numbers
//
//  Created by Верун on 29.09.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#ifndef __Arithmetic_Numbers__FHiddenDigit__
#define __Arithmetic_Numbers__FHiddenDigit__

#include "FDigit.h"

class FHiddenDigit : public FDigit
{
public:
    enum
    {
        HIDDEN_DIGIT_SHOW = 100,
        HIDDEN_DIGIT_HIDDING,
        HIDDEN_DIGIT_WAIT,
        HIDDEN_DIGIT_READY
    };
    
private:
    F32 fCurTime;
    
    void HideDigit();
    void WaitDigit();
    void ReadyDigit();
    FDigit * CreateMirror( const FVector2F & vPos );
    
public:
    FHiddenDigit( FArithWorld * lpWorld );
    //FHiddenDigit( const FVector2F & vPos, FArithWorld * lpWorld );
    FHiddenDigit( const FVector2F & vPos, UI32 iNum, FArithWorld * lpWorld );
    ~FHiddenDigit();
    
    void Update( F32 fDTime );
    bool IsHidden()const;
    
    UI32 Load( void * lpData );
    void Save( FFile * lpFile );
};

#endif /* defined(__Arithmetic_Numbers__FHiddenDigit__) */
