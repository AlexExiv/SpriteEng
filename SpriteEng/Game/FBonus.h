//
//  FBonus.h
//  Arithmetic Numbers
//
//  Created by Верун on 26.08.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#ifndef Arithmetic_Numbers_FBonus_h
#define Arithmetic_Numbers_FBonus_h

#include "FGameObject.h"

class FImage2D;
class FArithWorld;


class FBonus : public FGameObject
{
public:
    enum
    {
        BONUS_FIRST_NUM = 0,
        BONUS_TIME_NUM,
        BONUS_2X_SCORE,
        BONUS_ADD_SCORE,
        BONUS_DESTROY
    };
    
    enum
    {
        BONUS_SHOW = OBJECT_DEAD + 1,
        BONUS_GO_UP,
        BONUS_HIDE
    };
    
private:
    FImage2D * lpImage;
    F32 fShowTime;
    UI32 iBonusType;
    
public:
    FBonus( FArithWorld * lpWorld );
	FBonus( const FVector2F & vPos, UI32 iBonusType, FArithWorld * lpWorld );
    ~FBonus();
    
    F32 GetWidth()const;
    F32 GetHeight()const;
    
    void Update( F32 fDTime );
    
    UI32 Load( void * lpData );
    void Save( FFile * lpFile );
};

#endif
