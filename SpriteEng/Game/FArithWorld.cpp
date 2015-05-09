//
//  FWorld.cpp
//  Amazing Balls
//
//  Created by Верун on 11.06.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#include "FArithWorld.h"
#include "FObject.h"
#include "../2D/FScene.h"
#include "FArithGame.h"
//#include "../Sound/FSoundManager.h"
#include "FScoreAdd.h"
#include "../Core/FFile.h"
#include "FDigit.h"
#include "FParticle.h"
#include "FBonus.h"
#include "FAttention.h"
#include "FHiddenDigit.h"
#include "FMessage.h"
#include "..\Core\FLog.h"
//#import <UIKit/UIKit.h>
#include <string.h>


#define CALC_SPEED_TIME 3600.f

#define END_BLOCK 99999
#define CUR_VER 1
#define WORLD_BLOCK_ADD 99998

#define BONUS_SCORE 20

struct FWorldBlock
{
    UI32 iVersion;
    bool bNewGame, bPause;
    F32 fNewDigitTime;
    F32 fGameTime;
};

struct FWorldBlockAdd
{
    UI32 iVersion;
    I32 iLastAvgSpeed;
    I32 iSumDelDigitCount;
    I32 iPeriodCount;
    I32 iDelDigitCount;
    F32 fCurSpeedTime;
    I32 iBonusType;
    F32 fBonusTime;
    F32 fBonusCurTime;
};

const I32 iNewTable[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9,
                          1, 1, 1, 2, 1, 3, 1, 4, 1,
                          5, 1, 6, 1, 7, 1, 8, 1, 9 };


const F32 fNextAttackTimes[] = { 100000.f, 7200.f, 5400.f, 4800.f, 3600.f, 2400.f, 1800.f };

struct FBonusBound
{
    UI32 iLowerBound, iUpperBound;
    UI32 iBonusType;
    F32 fBonusTime;
};


const FBonusBound bBoundBonuses[] =
{
    { 0, 25, FArithWorld::BONUS_2X_SCORE, 2440.f },
    { 26, 50, FArithWorld::BONUS_DESTROY, 0.f },
    { 51, 200, FArithWorld::BONUS_ADD_SCORE, 0.f }
};

#define MAX_PROB_BOUND 100


I32 GetBonusIndex( I32 iPropab )
{
    for( I32 i = 0;i < ARRAY_SIZE( bBoundBonuses );i++ )
    {
        if( (bBoundBonuses[i].iLowerBound <= iPropab) && (iPropab < bBoundBonuses[i].iUpperBound) )
            return i;
    }
    
    return -1;
}

FArithWorld::FArithWorld( FArithGame * lpGame ) : FWorld( lpGame->GetWorldScene(), lpGame ),
	  bNewGame( true ), bPause( false ), lpFirst( NULL ), lpSecond( NULL ), fNewDigitTime( 0.f ), fTimeBonus( 0.f ), iBonusDigitCount( 0 ), 
	  fGameTime( 0 ), iDigitCount( 0 ), fCurSpeedTime( 0.f ), iLastAvgSpeed( 0 ), iDelDigitCount( 0 ), iSumDelDigitCount( 0 ), iPeriodCount( 0 ),
	  lpArithGame( lpGame )
{
	lpAttention = (FAttention *)AllocObject( "FAttention", "\\vector\\world", &FVector2F( fWorldWidth/2.f, fWorldHeight/2.f ), this );
	lp2XScoreMess = (FMessage *)AllocObject( "FMessage", "\\world", this );
//    lObjectList.PushBack( lpAttention );
}

FArithWorld::~FArithWorld()
{
    Clear();
	if( lp2XScoreMess )
		Delete( lp2XScoreMess );
	if( lpAttention )
		Delete( lpAttention );    
}

void FArithWorld::Update( F32 fDTime )
{
    if( bPause )
        return;
    
    if( iDigitCount > MAX_DIGIT_COUNT )
    {
        bPause = true;
        NewWorld();
        lpArithGame->ShowGameOver( true );
    }
    
    if( iBonusType != BONUS_NO_BONUS )
    {
        fCurBonusTime += fDTime;
        if( fCurBonusTime > fBonusTime )
        {
            fBonusTime = 0.f;
            fCurBonusTime = 0.f;
            iBonusType = BONUS_NO_BONUS;
        }
    }
    
    fGameTime += fDTime;
    lpArithGame->SetGameTime( fGameTime );
    
    if( bNewGame )
    {
		FDigit * lpTmpDigit = (FDigit *)AllocObject( "FDigit", "\\world", this );
		FVector2F vPos( 0.f, fWorldHeight );
		F32  fHeight = lpTmpDigit->GetHeight();
        I32 iNum = 0;
        FDigit * lpNext = NULL;
        
        for( I32 i = 0;i < 3;i++ )
        {
            vPos.y -= fHeight;
            for( I32 j = 0;j < NUMBER_IN_LINE;j++, iNum++ )
            {
				FDigit * lpDigit = (FDigit *)AllocObject( "FDigit", "\\vector\\ui\\world", &vPos, iNewTable[iNum], this );
                lpDigit->SetNext( lpNext );
                lpNext = lpDigit;
                
                vPos.x += lpDigit->GetWidth();
                fHeight = lpDigit->GetHeight();
                
                lObjectList.PushBack( lpDigit );
                iDigitCount++;
            }
            vPos.x = 0.f;
            
        }
        bNewGame = false;
		Delete( lpTmpDigit );
    }
    
    fTimeBonus += fDTime;

    //lpSoundManager->Update( fDTime );
    
    FObjectList::Iterator iIt = lObjectList.Begin();
    for( ;iIt != lObjectList.End();iIt++ )
        iIt->Update( fDTime );
    
    iIt = lObjectList.Begin();
    for( ;iIt != lObjectList.End(); )
    {
        if( iIt->GetObjState() == FGameObject::OBJECT_DEAD )
        {
            FGameObject * lpObj = *iIt;
            if( lpObj->IsDigit() )
            {
                if( lpObj == lpFirst )
                    lpFirst = NULL;
                if( lpObj == lpSecond )
                    lpSecond = NULL;
                
                iDigitCount--;
                FObjectIterator iPrev = iIt, iNext = iIt;
                if( iIt != lObjectList.Begin() )
                {
                    do
                    {
                        iPrev--;
                    }while( !iPrev->IsDigit() && (iPrev != lObjectList.Begin()) );
                }
                

                do
                {
                    iNext++;
                    if( iNext == lObjectList.End() )
                        break;
                }while( !iNext->IsDigit() );

                
                if( iIt == lObjectList.Begin() )
                    ((FDigit *)*iNext)->SetNext( NULL );
                else
                {
                    FObjectIterator iLast = lObjectList.Last();
                    while( !iLast->IsDigit() )
                        iLast--;
                    
                    if( iIt != iLast )
                        ((FDigit *)*iNext)->SetNext( ((FDigit *)*iPrev) );
                }
                iBonusDigitCount++;
                if( iBonusDigitCount > TIME_BONUS_DIGIT_COUNT )
                {
                    if( fTimeBonus < TIME_BONUS_TIME )
                    {
                        AddScore( FVector2F( 100, 400 ), 10 );
						AddBonusScore( FVector2F( fWorldWidth/2.f, fWorldHeight/2.f ), FBonus::BONUS_TIME_NUM );
                    }
                    
                    fTimeBonus = 0.f;
                    iBonusDigitCount = 0;
                }
            }
            Delete( lpObj );
            iIt = lObjectList.Erase( iIt );
        }
        else
            iIt++;
    }
    
    iIt = lObjectList.Last();
    while( !iIt->IsDigit() )
        iIt--;
    
    ((FDigit *)*iIt)->StrideLeft( fDTime );
    
    lpAttention->Update( fDTime );
    lp2XScoreMess->Update( fDTime );
    
    fNewDigitTime += fDTime;
    F32 fMaxTime = NEW_DIGIT_TIME;
    if( lpAttention->IsEventStart() )
        fMaxTime = ATTENTION_NEW_DIGIT_TIME;
    
    if( fNewDigitTime >= fMaxTime )
    {
        fNewDigitTime = 0.f;
        NewDigit();
    }
    
    CalculateSpeed( fDTime );
}

void FArithWorld::CalculateSpeed( F32 fDTime )
{
    fCurSpeedTime += fDTime;
    if( fCurSpeedTime > CALC_SPEED_TIME )
    {
        iSumDelDigitCount += iDelDigitCount;
        iPeriodCount++;
        I32 iAvgSpeed = iSumDelDigitCount/iPeriodCount;
        
        if( iAvgSpeed > iLastAvgSpeed )
        {
            I32 iNextAttackTimeInd = 0;
            if( iAvgSpeed < 30 )
                iNextAttackTimeInd = 0;
            else if( iAvgSpeed < 35 )
                iNextAttackTimeInd = 1;
            else if( iAvgSpeed < 40 )
                iNextAttackTimeInd = 2;
            else if( iAvgSpeed < 45 )
                iNextAttackTimeInd = 3;
            else if( iAvgSpeed < 55 )
                iNextAttackTimeInd = 4;
            else if( iAvgSpeed < 65 )
                iNextAttackTimeInd = 5;
            else
                iNextAttackTimeInd = 6;
            
            lpAttention->SetNextAttackTime( fNextAttackTimes[iNextAttackTimeInd] );
			FLog::PutError( "Avg speed: %i", iAvgSpeed );
        }
        fCurSpeedTime = 0.f;
        iDelDigitCount = 0;
        iLastAvgSpeed = iAvgSpeed;
        
    }
}

//удаляет всевозможные цифры на поле, которые дают в сумме 10 либо равны и граничат друг с другом
void FArithWorld::DestroySum10()
{
    if( lpFirst )
    {
        lpFirst->Select( false );
        lpFirst = NULL;
    }
    if( lpSecond )
    {
        lpSecond->Select( false );
        lpSecond = NULL;
    }
    
    FDigit * lpDigites[NUMBER_IN_LINE*2];//указатели на обрабатываемые строки цифр
    FDigit ** lpWriteLine = &lpDigites[0], ** lpReadLine = &lpDigites[NUMBER_IN_LINE];
    memset( lpDigites, 0, sizeof( lpDigites ) );//заполняем указатели нулями
    UI32 iOldI = 0;//индекс обрабатываемой строки
    
    FObjectIterator iIt = lObjectList.Begin();
    for(; iIt != lObjectList.End();iIt++ )
    {
        if( !(*iIt)->IsDigit() )
            continue;
        
        FDigit * lpDigit = (FDigit *)(*iIt);
        if( lpDigit->IsDying() || lpDigit->IsHidden() )
            continue;
        
		UI32 i = (fWorldHeight - lpDigit->GetPos().y)/lpDigit->GetHeight() - 1;
        if( i > iOldI )//новая строка цифр
        {
            iOldI = i;
			//меняем строки указателей на чтение и запись местами
            lpReadLine = lpWriteLine;
            lpWriteLine = &lpDigites[NUMBER_IN_LINE*(i%2)];
            memset( (void *)lpWriteLine, 0, sizeof( FDigit * )*NUMBER_IN_LINE );//сбрасываем указатели на запись в ноль
        }
        
        UI32 j = lpDigit->GetPos().x/lpDigit->GetWidth();
        if( lpReadLine[j] )//если в строке на чтение записана цифра
        {
 			//проверяем на условие сокращения с соседней цифрой по вертикали
           if( (lpReadLine[j]->GetPos().x == lpDigit->GetPos().x) && (lpReadLine[j]->GetPos().y == (lpDigit->GetPos().y + lpDigit->GetHeight())) )
            {
                if( (lpReadLine[j]->GetNum() == lpDigit->GetNum()) || ((lpReadLine[j]->GetNum() + lpDigit->GetNum()) == 10) )
                {
                    UI32 iBonusMul = 1;
                    if( iBonusType == BONUS_2X_SCORE )
                        iBonusMul = 2;
                    
                    AddScore( lpDigit->GetPos(), lpDigit->GetNum()*iBonusMul );
                    AddScore( lpReadLine[j]->GetPos(), lpReadLine[j]->GetNum()*iBonusMul );
                    
                    AddParticle( lpDigit->GetPos() );
                    AddParticle( lpReadLine[j]->GetPos() );
                    
                    lpDigit->Destroy();
                    lpReadLine[j]->Destroy();
                }
            }
        }
        FObjectIterator iNext = iIt;
        iNext++;
        while( iNext != lObjectList.End() )
        {
            if( iNext->IsDigit() )
                break;
            iNext++;
        }
        
        if( iNext == lObjectList.End() )
            continue;
        
        FDigit * lpNextDigit = (FDigit *)(*iNext);
        if( lpNextDigit->IsHidden() || lpDigit->IsDying() )
            continue;
        
		//проверяем на условие сокращения с соседней цифрой по горизонтали
        if( (lpNextDigit->GetPos().y == lpDigit->GetPos().y) && (lpNextDigit->GetPos().x == (lpDigit->GetPos().x + lpDigit->GetWidth())) )
        {
            if( (lpNextDigit->GetNum() == lpDigit->GetNum()) || ((lpNextDigit->GetNum() + lpDigit->GetNum()) == 10) )
            {
                UI32 iBonusMul = 1;
                if( iBonusType == BONUS_2X_SCORE )
                    iBonusMul = 2;
                
                AddScore( lpDigit->GetPos(), lpDigit->GetNum()*iBonusMul );
                AddScore( lpNextDigit->GetPos(), lpReadLine[j]->GetNum()*iBonusMul );
                    
                AddParticle( lpDigit->GetPos() );
                AddParticle( lpNextDigit->GetPos() );
                
                lpDigit->Destroy();
                lpNextDigit->Destroy();
            }
        }

        if( !lpDigit->IsDying() )//если цифра не скоращена то дабавляем в список
            lpWriteLine[j] = lpDigit;
    }
}

void FArithWorld::NewDigit()
{
    UI32 iNum = 0;
    while( iNum == 0 )
        iNum = rand() % 10;
    
    FObjectIterator iLast = lObjectList.Last();
    while( !iLast->IsDigit() )
        iLast--;
    
    FVector2F vPos;
    if( iLast->GetPos().x >= (NUMBER_IN_LINE - 1)*iLast->GetWidth() )
        vPos.y = iLast->GetPos().y - iLast->GetHeight();
    else
    {
        vPos.x = iLast->GetPos().x + iLast->GetWidth();
        vPos.y = iLast->GetPos().y;
    }
    
    FDigit * lpDigit;
    if( (rand()%HIDDEN_DIGIT_PROBABILITY) == 0 )
		lpDigit = (FHiddenDigit *)AllocObject( "FHiddenDigit", "\\vector\\ui\\world", &vPos, iNum, this );
    else
		lpDigit = (FDigit *)AllocObject( "FDigit", "\\vector\\ui\\world", &vPos, iNum, this );
    
    lpDigit->SetNext( (FDigit *)*iLast );
    lObjectList.PushBack( lpDigit );
    iDigitCount++;
}

void FArithWorld::Save( FFile * lpFile )
{
    FWorldBlock oBlock;
    
    oBlock.iVersion = CUR_VER;
    oBlock.bNewGame = bNewGame;
    oBlock.bPause = bPause;
    oBlock.fNewDigitTime = fNewDigitTime;
    oBlock.fGameTime = fGameTime;
    
    lpFile->Write( &oBlock, sizeof( FWorldBlock ) );
    
    FWorldBlockAdd oBlockAdd = { CUR_VER, iLastAvgSpeed, iSumDelDigitCount, iPeriodCount, iDelDigitCount, fCurSpeedTime };
    I32 iBlock = WORLD_BLOCK_ADD;
    lpFile->Write( &iBlock, sizeof( I32 ) );
    lpFile->Write( &oBlockAdd, sizeof( FWorldBlockAdd ) );
    
    FObjectIterator iIt = lObjectList.Begin();
    for(;iIt != lObjectList.End();iIt++ )
    {

        I32 iObjType = iIt->GetObjType();
        lpFile->Write( &iObjType, sizeof( I32 ) );
        iIt->Save( lpFile );
    }
    
    I32 iObjType = lpAttention->GetObjType();
    lpFile->Write( &iObjType, sizeof( I32 ) );
    lpAttention->Save( lpFile );
    
    iObjType = END_BLOCK;
    lpFile->Write( &iObjType, sizeof( I32 ) );
}

UI32 FArithWorld::Load( void * lpData )
{
    I32 iSize = 0;
    FWorldBlock * lpBlock = (FWorldBlock *)lpData;
    FBYTE * lpByte = (FBYTE *)lpData + sizeof( FWorldBlock );
    
    bNewGame = lpBlock->bNewGame;
    bPause = lpBlock->bPause;
    fNewDigitTime = lpBlock->fNewDigitTime;
    fGameTime = lpBlock->fGameTime;

    I32 iObjType = *(I32 *)lpByte;
    if( iObjType == WORLD_BLOCK_ADD )
    {
        lpByte += sizeof( I32 );
        FWorldBlockAdd * lpAddBlock = (FWorldBlockAdd *)lpByte;
        
        iLastAvgSpeed = lpAddBlock->iLastAvgSpeed;
        iSumDelDigitCount = lpAddBlock->iSumDelDigitCount;
        iPeriodCount = lpAddBlock->iPeriodCount;
        iDelDigitCount = lpAddBlock->iDelDigitCount;
        fCurSpeedTime = lpAddBlock->fCurSpeedTime;
        
        lpByte += sizeof( FWorldBlockAdd );
        iObjType = *(I32 *)lpByte;
    }
    
    FDigit * lpNext = NULL;
    
    while( iObjType != END_BLOCK )
    {
        lpByte += sizeof( I32 );
        FGameObject * lpObject = NULL;

        switch( iObjType )
        {
            case FGameObject::OBJECT_DIGIT:
				lpObject = (FDigit *)AllocObject( "FDigit", "\\world", this );
                ((FDigit *)lpObject)->SetNext( lpNext );
                lpNext = (FDigit *)lpObject;
                iDigitCount++;
                break;
            case FGameObject::OBJECT_HIDDEN_DIGIT:
				lpObject = (FHiddenDigit *)AllocObject( "FHiddenDigit", "\\world", this );
                ((FDigit *)lpObject)->SetNext( lpNext );
                lpNext = (FDigit *)lpObject;
                iDigitCount++;
                break;
            case FGameObject::OBJECT_LABEL:
				lpObject = (FScoreAdd *)AllocObject( "FScoreAdd", "\\world", this );
                break;
            case FGameObject::OBJECT_PARTICLE:
				lpObject = (FParticle *)AllocObject( "FParticle", "\\world", this );
                break;
            case FGameObject::OBJECT_BONUS:
				lpObject = (FBonus *)AllocObject( "FBonus", "\\world", this );
                break;
            case FGameObject::OBJECT_ATTENTION:
                lpObject = lpAttention;
                break;
            default:
                ;
                
        }
        lpByte += lpObject->Load( lpByte );
        
        if( iObjType != FGameObject::OBJECT_ATTENTION )
            lObjectList.PushBack( lpObject );
        
        iObjType = *(I32 *)lpByte;
    }
    
    lpArithGame->SetGameTime( fGameTime );
    
    return iSize;
}

void FArithWorld::Clear()
{
    FObjectIterator iIt = lObjectList.Begin();
    for( ;iIt != lObjectList.End();iIt++ )
        delete *iIt;
    lObjectList.Clear();
}


void FArithWorld::NewWorld()
{
    Clear();
    bNewGame = true;
    fGameTime = 0.f;
    iDigitCount = 0;
    lpArithGame->ResetScore();
    lpAttention->ResetState();
}


void FArithWorld::ClickDown( const FVector2F & vPoint, UI32 iNum )
{
    FObjectIterator iIt = lObjectList.Begin();
    bool bContain = false;
    for( ;iIt != lObjectList.End();iIt++ )
    {
        if( iIt->IsDigit() )
        {
            FDigit * lpDigit = (FDigit *)*iIt;
            
            if( lpDigit->ContainPoint( vPoint ) )
            {
                if( lpDigit->IsDying() || lpDigit->IsBorning() )
                    break;
                
                if( lpDigit->IsHidden() )
                    if( lpDigit->GetObjState() != FHiddenDigit::HIDDEN_DIGIT_READY )
                        break;
                
                if( lpFirst )
                {
                    lpSecond = lpDigit;
                    lpSecond->Select( true );
                }
                else
                {
                    lpFirst = lpDigit;
                    lpFirst->Select( true );
                }
                bContain = true;
                break;
            }
        }
    }
    
    if( !bContain )
        return;
    
    if( lpSecond && (lpFirst != lpSecond) )
    {
        bool bAdjacent = false;
        if( lpFirst->GetPos().x == lpSecond->GetPos().x )
        {
            if( lpFirst->GetPos().y == (lpSecond->GetPos().y + lpSecond->GetHeight()) )
                bAdjacent = true;
            else if( (lpFirst->GetPos().y + lpFirst->GetHeight()) == lpSecond->GetPos().y )
                bAdjacent = true;
        }
        if( lpFirst->GetPos().y == lpSecond->GetPos().y )
        {
            if( lpFirst->GetPos().x == (lpSecond->GetPos().x + lpSecond->GetWidth()) )
                bAdjacent = true;
            else if( (lpFirst->GetPos().x + lpFirst->GetWidth()) == lpSecond->GetPos().x )
                bAdjacent = true;
        }
        
        if( bAdjacent )
        {
            if( (lpFirst->GetNum() == lpSecond->GetNum()) || ((lpFirst->GetNum() + lpSecond->GetNum()) == 10) )
            {
                iDelDigitCount += 2;
                
                I32 iBonusMul = 1;
                if( iBonusType == BONUS_2X_SCORE )
                    iBonusMul = 2;
                
                AddScore( lpFirst->GetPos(), lpFirst->GetNum()*iBonusMul );
                AddScore( lpSecond->GetPos(), lpSecond->GetNum()*iBonusMul );
                
                AddParticle( lpFirst->GetPos() );
                AddParticle( lpSecond->GetPos() );
                
                if( lpFirst->IsFirst() || lpSecond->IsFirst() )
                {
					AddScore( (lpFirst->GetPos() + lpSecond->GetPos())/2.f, 15 );
					AddBonusScore( FVector2F( fWorldWidth/2.f, fWorldHeight/2.f ), FBonus::BONUS_FIRST_NUM );
                }
                
                UI32 iHiddenCnt = 0;
                if( lpFirst->IsHidden() )
                    iHiddenCnt++;
                if( lpSecond->IsHidden() )
                    iHiddenCnt++;
                
                lpFirst->Destroy();
                lpSecond->Destroy();
                
                //lpSoundManager->PlaySound( DESTROY_DIGIT_SOUND );

                
                if( iHiddenCnt > 0 )
                {
                    UI32 iRandBonus = rand()%MAX_PROB_BOUND;
                    I32 iBonusInd = GetBonusIndex( iRandBonus );
					FLog::PutMessage( "Bonus chance: %i", iRandBonus );
					FVector2F vBonusPos( fWorldWidth/2.f, fWorldHeight/2.f );
                    switch( bBoundBonuses[iBonusInd].iBonusType )
                    {
                        case BONUS_2X_SCORE:
                            FLog::PutMessage( "2x score bonus" );
                            lp2XScoreMess->BeginTimer( bBoundBonuses[iBonusInd].fBonusTime );
                            iBonusType = BONUS_2X_SCORE;
                            fBonusTime += bBoundBonuses[iBonusInd].fBonusTime;
                            AddBonusScore( vBonusPos, FBonus::BONUS_2X_SCORE );
                            break;
                        case BONUS_ADD_SCORE:
                            FLog::PutMessage( "Add score" );
							AddScore( (lpFirst->GetPos() + lpSecond->GetPos())/2.f, 100 );
                            AddBonusScore( vBonusPos, FBonus::BONUS_ADD_SCORE );
                            break;
                        case BONUS_DESTROY:
                            FLog::PutMessage( "Bonus destroy sum 10 " );
                            DestroySum10();
                            AddBonusScore( vBonusPos, FBonus::BONUS_DESTROY );
                            break;
                    }
                }
                
            }
            else if( lpFirst->IsHidden() || lpSecond->IsHidden() )
            {
                lpFirst->Destroy();
                lpSecond->Destroy();
                //lpSoundManager->PlaySound( NOT_RIGHT_SOUND );
            }
            
        }
    }
    
    if( lpFirst && lpSecond )
    {
        lpFirst->Select( false );
        lpFirst = NULL;
        lpSecond->Select( false );
        lpSecond = NULL;
    }
}

void FArithWorld::ClickUp( const FVector2F & vPoint, UI32 iNum )
{
    if( bPause )
    {
        bPause = false;
        lpArithGame->ShowGameOver( false );
    }
}

void FArithWorld::ClickMove( const FVector2F & vPoint, UI32 iNum )
{
    
}

void FArithWorld::AddScore( const FVector2F & vPos, I32 iScore )
{
	lObjectList.PushBack( (FScoreAdd *)AllocObject( "FScoreAdd", "\\vector\\world\\i", &vPos, this, iScore ) );
    lpArithGame->AddScore( iScore );
}

void FArithWorld::AddBonusScore( const FVector2F & vPos, UI32 iBonusType )
{
	lObjectList.PushBack( (FBonus *)AllocObject( "FBonus", "\\vector\\ui\\world", &vPos, iBonusType, this ) );
    //lpSoundManager->PlaySound( TIME_BONUS_SOUND );
}

void FArithWorld::AddParticle( const FVector2F & vPos )
{
	lObjectList.PushBack( (FParticle *)AllocObject( "FParticle", "\\vector\\world", vPos, this ) );
}

