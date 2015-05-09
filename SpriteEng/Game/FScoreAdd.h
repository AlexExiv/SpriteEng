//
//  FScoreAdd.h
//  Amazing Balls
//
//  Created by Верун on 06.07.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#ifndef Amazing_Balls_FScoreAdd_h
#define Amazing_Balls_FScoreAdd_h

#include "FGameObject.h"

class FWorld;
class FText;
class FArithWorld;


class FScoreAdd : public FGameObject
{
    enum
    {
        SCORE_SHOW = OBJECT_DEAD + 1,
        SCORE_GO_UP,
        SCORE_HIDE
    };
    
    F32 fShowTime;
    FText * lpLabel;
    
public:
    FScoreAdd( FArithWorld * lpWorld );
	FScoreAdd( const FVector2F & vPos, FArithWorld * lpWorld, I32 iIncrScore );
    ~FScoreAdd();
    
    F32 GetWidth()const;
    F32 GetHeight()const;
    void Move( const FVector2F & vPos );
    
    void Update( F32 fDTime );
    bool IsLineIntersect( F32 x0, F32 y0, F32 x1, F32 y1 ) ;
    bool IsObjIntersect( FObject * lpObj );
    void Blow( I32 iCameFrom, F32 fTimeToBlow = 0.f ) ;
    
    UI32 Load( void * lpData );
    void Save( FFile * lpFile );
};

#endif
