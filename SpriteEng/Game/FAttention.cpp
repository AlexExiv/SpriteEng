//
//  FAttention.cpp
//  Arithmetic Numbers
//
//  Created by Верун on 13.09.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#include "FAttention.h"
#include "..\2D\FImage2D.h"
#include "..\2D\FObject2D.h"
#include "FGameDef.h"
#include "FArithWorld.h"
#include "..\Core\FString.h"
#include "..\Core\FFile.h"
#include "..\Core\FLog.h"



#define CUR_VER 1

struct FAttentionBlock
{
    UI32 iVersion;
    
    F32 fCurTime; //текущее время события
    F32 fEventTime; //общее время события
    I32 iLastCount; //последняя цифра отчета
    F32 fNextAttackTime; //реальное время до следующей атаки
    F32 fNextAttackTimeM; //минимальное время до следующей атаки цифр
};

#define ATTENTION_PREPARE "bundle\\prepareatack.bmp"
#define ATTENTION_COUNTER "bundle\\counter.bmp"
#define ATTENTION_HOLDON "bundle\\holdon.bmp"
#define ATTENTION_SOONATTACK "bundle\\beready.bmp"


FAttention::FAttention( const FVector2F & vPos, FArithWorld * lpWorld ) : FGameObject( vPos, OBJECT_ATTENTION, lpWorld ), fCurTime( 0.f ), fEventTime( 0.f )
                    , lpPrepare( NULL ), lpCounter( NULL ), lpHoldOn( NULL ), fNextAttackTimeM( ATTENTION_NEXT_ATTACK_MIN_TIME )
{
	FVector2F vPos_( 0.f, 0.f );
	lpAttackSoon = (FImage2D *)AllocObject( "FImage2D", "\\vector\\scene\\ui\\string", &vPos_, lpWorld->GetScene(), 1, &sAttentionAttackSoon );
 	lpPrepare = (FImage2D *)AllocObject( "FImage2D", "\\vector\\scene\\ui\\string", &vPos_, lpWorld->GetScene(), 1, &sAttentionPrepare );
	lpHoldOn = (FImage2D *)AllocObject( "FImage2D", "\\vector\\scene\\ui\\string", &vPos_, lpWorld->GetScene(), 1, &sAttentionHoldon );
	lpCounter = (FObject2D *)AllocObject( "FObject2D", "\\vector\\scene\\ui\\string", &vPos_, lpWorld->GetScene(), 1, &sAttentionCounter );
   
    lpCounter->SetDuration( ATTENTION_SHOW_COUNTER_TIME/3.f );
    
    ResetState();
}

FAttention::~FAttention()
{
    if( lpPrepare )
		Delete( lpPrepare );
    if( lpCounter )
		Delete( lpCounter );
    if( lpHoldOn )
		Delete( lpHoldOn );
    if( lpAttackSoon )
		Delete( lpAttackSoon );
    
    lpPrepare = NULL;
    lpCounter = NULL;
    lpHoldOn = NULL;
    lpAttackSoon = NULL;
}

void FAttention::NextLoop()
{
    iObjState = OBJECT_WAIT;
    fCurTime = 0.f;
    fNextAttackTime = fNextAttackTimeM + rand()%ATTENTION_NEXT_ATTACK_TIME_DIV;
	FLog::PutError( "Next attack time:%f", fNextAttackTime );
}

F32 FAttention::GetWidth()const
{
    switch( iObjState )
    {
        case OBJECT_PREPARE:
            return lpPrepare->GetWidth();
        case OBJECT_COUNTER:
            return lpCounter->GetWidth();
        case OBJECT_HOLDON:
            return lpHoldOn->GetWidth();
    }
    
    return 0.f;
}

F32 FAttention::GetHeight()const
{
    switch( iObjState )
    {
        case OBJECT_PREPARE:
            return lpPrepare->GetHeight();
        case OBJECT_COUNTER:
            return lpCounter->GetHeight();
        case OBJECT_HOLDON:
            return lpHoldOn->GetHeight();
    }
    
    return 0.f;
}

void FAttention::Update( F32 fDTime )
{
    fCurTime += fDTime;
    F32 fScale = 1.f;
    
    switch( iObjState )
    {
        case OBJECT_WAIT:
            if( fCurTime > fNextAttackTime )
                StartEvent();
            break;
        case OBJECT_ATTACK_SOON:
            fScale = (fCurTime + ATTENTION_SHOW_DELAY_TIME)/ATTENTION_SHOW_SOON_TIME;
            if( fScale > 1.f )
                fScale = 1.f;
            
            lpAttackSoon->Scale( fScale );
			lpAttackSoon->Move( FVector2F( vPos.x - lpAttackSoon->GetWidth()/2.f, vPos.y - lpAttackSoon->GetHeight()/2.f ) );
            
            if( fCurTime > ATTENTION_SHOW_SOON_TIME )
            {
                iObjState = OBJECT_WAIT_FOR_ATTACK;
                lpAttackSoon->Show( false );
                fCurTime = 0.f;
            }
            break;
        case OBJECT_WAIT_FOR_ATTACK:
            if( fCurTime > ATTENTION_ATTACK_WAIT_TIME )
            {
                iObjState = OBJECT_PREPARE;
                lpPrepare->Show( true );
                lpPrepare->Scale( 0.f );
                fCurTime = 0.f;
            }
            break;
        case OBJECT_PREPARE:
            fScale = (fCurTime + ATTENTION_SHOW_DELAY_TIME)/ATTENTION_SHOW_PREPARE_TIME;
            if( fScale > 1.f )
                fScale = 1.f;
            
            lpPrepare->Scale( fScale );
            lpPrepare->Move( FVector2F( vPos.x - lpPrepare->GetWidth()/2.f, vPos.y - lpPrepare->GetHeight()/2.f ) );
            
            if( fCurTime > ATTENTION_SHOW_PREPARE_TIME )
            {
                iObjState = OBJECT_COUNTER;
                lpPrepare->Show( false );
                lpCounter->Show( true );
                lpCounter->Scale( 0.f );
				lpCounter->StartAnimation( sCounterAnim );
                iLastCount = 0;
                fCurTime = 0.f;
            }
            break;
        case OBJECT_COUNTER:
            if( iLastCount != lpCounter->GetCurFrame() )
            {
                iLastCount = lpCounter->GetCurFrame();
            }
            fScale = ((fCurTime + ATTENTION_SHOW_DELAY_TIME)/F32(iLastCount + 1))/(ATTENTION_SHOW_COUNTER_TIME/3.f);
            if( fScale > 1.f )
                fScale = 1.f;
            
            lpCounter->Scale( fScale );
			lpCounter->Move( FVector2F( vPos.x - lpCounter->GetWidth()/2.f, vPos.y - lpCounter->GetHeight()/2.f ) );
            
            if( fCurTime > ATTENTION_SHOW_COUNTER_TIME )
            {
                iObjState = OBJECT_HOLDON;
                lpCounter->Show( false );
                lpHoldOn->Show( true );
                lpHoldOn->Scale( 0.f );
                fCurTime = 0.f;
            }
            break;
        case OBJECT_HOLDON:
            fScale = (fCurTime + ATTENTION_SHOW_DELAY_TIME)/ATTENTION_SHOW_PREPARE_TIME;
            if( fScale > 1.f )
                fScale = 1.f;

            lpHoldOn->Scale( fScale );
			lpHoldOn->Move( FVector2F( vPos.x - lpHoldOn->GetWidth()/2.f, vPos.y - lpHoldOn->GetHeight()/2.f ) );
            
            if( fCurTime > ATTENTION_SHOW_HOLDON_TIME )
            {
                iObjState = OBJECT_EVENT_STARTED;
                lpHoldOn->Show( false );
                fCurTime = 0.f;
            }
            break;
        case OBJECT_EVENT_STARTED:
            if( fCurTime > fEventTime )
                NextLoop();
            
            break;
    }
}

void FAttention::StartEvent()
{
    fCurTime = 0.f;
    fEventTime = ATTENTION_EVENT_MIN_TIME + rand()%ATTENTION_EVENT_TIME_DIVIATION;
    iObjState = OBJECT_ATTACK_SOON;
    lpAttackSoon->Show( true );
    lpAttackSoon->Scale( 0.f );
}

bool FAttention::IsEventStart()
{
    return iObjState == OBJECT_EVENT_STARTED;
}

bool FAttention::IsEventEnd()
{
    return iObjState == OBJECT_WAIT;
}

void FAttention::ResetState()
{
    lpAttackSoon->Show( false );
    lpPrepare->Show( false );
    lpCounter->Show( false );
    lpHoldOn->Show( false );
    
    NextLoop();
}

void FAttention::SetNextAttackTime( F32 fNextAttackTimeM0 )
{
    fNextAttackTimeM = fNextAttackTimeM0;
    if( iObjState == OBJECT_WAIT )
        ResetState();
}

UI32 FAttention::Load( void * lpData )
{
    UI32 iSize = FGameObject::Load( lpData );
    
    FAttentionBlock * lpBlock = (FAttentionBlock *)( (FBYTE *)lpData + iSize );
    fCurTime = lpBlock->fCurTime;
    fEventTime = lpBlock->fEventTime;
    iLastCount = lpBlock->iLastCount;
    fNextAttackTime = lpBlock->fNextAttackTime;
    fNextAttackTimeM = lpBlock->fNextAttackTimeM;
    
    switch( iObjState )
    {
        case OBJECT_ATTACK_SOON:
            lpAttackSoon->Show( true );
            lpAttackSoon->Scale( fCurTime/ATTENTION_SHOW_SOON_TIME);
            lpAttackSoon->Move( FVector2F( vPos.x - lpPrepare->GetWidth()/2.f, vPos.y - lpPrepare->GetHeight()/2.f ) );
            break;
            
        case OBJECT_PREPARE:
            lpPrepare->Show( true );
            lpPrepare->Scale( fCurTime/ATTENTION_SHOW_PREPARE_TIME );
            lpPrepare->Move( FVector2F( vPos.x - lpPrepare->GetWidth()/2.f, vPos.y - lpPrepare->GetHeight()/2.f ) );
            break;
            
        case OBJECT_COUNTER:
            lpCounter->Show( true );
            lpCounter->Scale( (fCurTime/F32(iLastCount + 1))/(ATTENTION_SHOW_COUNTER_TIME/3.f) );
            lpCounter->Move( FVector2F( vPos.x - lpCounter->GetWidth()/2.f, vPos.y - lpCounter->GetHeight()/2.f ) );
            lpCounter->SetCurFrame( iLastCount );
            break;
            
        case OBJECT_HOLDON:
            lpHoldOn->Show( true );
            lpHoldOn->Scale( fCurTime/ATTENTION_SHOW_HOLDON_TIME );
            lpHoldOn->Move( FVector2F( vPos.x - lpHoldOn->GetWidth()/2.f, vPos.y - lpHoldOn->GetHeight()/2.f ) );
            break;
    }
    
    return sizeof( FAttentionBlock ) + iSize;
}

void FAttention::Save( FFile * lpFile )
{
    FGameObject::Save( lpFile );
    FAttentionBlock oBlock = { CUR_VER, fCurTime, fEventTime, iLastCount, fNextAttackTime, fNextAttackTimeM };
    lpFile->Write( &oBlock, sizeof( oBlock ) );
}


