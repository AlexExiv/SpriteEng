//
//  FWorld.h
//  Amazing Balls
//
//  Created by Верун on 10.06.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#ifndef Amazing_Balls_FWorld_h
#define Amazing_Balls_FWorld_h

#include "..\Core\FList.h"
#include "FGameDef.h"
#include "FWorld.h"
#include "..\Math\FVector2D.h"

class FScene;
class FGameObject;
class FArithGame;
//class FSoundManager;
class FFile;
class FDigit;
class FAttention;
class FMessage;


class FArithWorld : public FWorld
{

	typedef FList<FGameObject *, FFreePointer__<FGameObject *>> FObjectList;
    typedef FList<FGameObject *, FFreePointer__<FGameObject *>>::Iterator FObjectIterator;
    
    FArithGame * lpArithGame;
    FObjectList lObjectList;
    FAttention * lpAttention;
    FMessage * lp2XScoreMess;
    FRect rGameField;
    
    bool bNewGame, bPause;
    FDigit * lpFirst, * lpSecond;
    F32 fNewDigitTime;
    F32 fTimeBonus;
    UI32 iBonusDigitCount;
    F32 fGameTime;
    UI32 iDigitCount;
    UI32 iDelDigitCount, iPeriodCount, iSumDelDigitCount, iLastAvgSpeed;
    F32 fCurSpeedTime;
    F32 fBonusTime, fCurBonusTime;
    UI32 iBonusType;

public:
    enum
    {
        BONUS_NO_BONUS = 0,
        BONUS_2X_SCORE,
        BONUS_ADD_SCORE,
        BONUS_DESTROY
    };
    
private:
    void Clear();
    void NewDigit();
    void NextDigit();
    void CalculateSpeed( F32 fDTime );
    void DestroySum10();
    
public:
    FArithWorld( FArithGame * lpGame );
    ~FArithWorld();
    
    void Update( F32 fDTime );
    void Draw( const FRect & rFrustum );
    void Save( FFile * lpFile );
    UI32 Load( void * lpData );
    
	void ClickDown( const FVector2F & vPoint, UI32 iNum );
    void ClickUp( const FVector2F & vPoint, UI32 iNum );
    void ClickMove( const FVector2F & vPoint, UI32 iNum );
    void DoubleClick( const FVector2F & vPoint, UI32 iNum );
    
    //UI32 GetUpperBound()const;
    //F32 GetFieldWidth()const;
    //FRect GetGameField()const;
    //FScene * GetScene()const;
    //FSoundManager * GetSoundManager()const;
    
    void AddObject( FGameObject * lpObject );
    void RemoveObject( FGameObject * lpObject );
    void NewWorld();
    
    void AddScore( const FVector2F & vPos, I32 iScore );
    void AddBonusScore( const FVector2F & vPos, UI32 iBonusType );
    void AddParticle( const FVector2F & vPos );
};

#endif
