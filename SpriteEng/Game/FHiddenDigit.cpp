//
//  FHiddenDigit.cpp
//  Arithmetic Numbers
//
//  Created by Верун on 29.09.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#include "FHiddenDigit.h"
#include "FArithWorld.h"
#include "..\Core\FFile.h"
#include "../2D/FText.h"
#include "../2D/FObject2D.h"

#define CUR_VER 1

struct FHiddenDigitBlock
{
    UI32 iVersion;
    F32 fCurTime;
};

FHiddenDigit::FHiddenDigit( FArithWorld * lpWorld ) : FDigit( lpWorld ), fCurTime( 0.f )
{
    iObjState = DIGIT_BORN;
	lpAnimation->StartAnimation( sHiddenDigitAnim );
    iObjType = OBJECT_HIDDEN_DIGIT;
}

//FHiddenDigit::FHiddenDigit( const FVector2F & vPos, FArithWorld * lpWorld ) : FDigit( vPos, 10, lpWorld ), fCurTime( 0.f )
//{
//    lpLabel->SetText( "X" );
//}

FHiddenDigit::FHiddenDigit( const FVector2F & vPos, UI32 iNum, FArithWorld * lpWorld ) : FDigit( vPos, iNum, lpWorld ), fCurTime( 0.f )
{
	if( iNum == 10 )
	{
	   lpLabel->SetText( "X" );
	}
	else
	{
		iObjState = DIGIT_BORN;
		lpAnimation->StartAnimation( sHiddenDigitAnim );
		iObjType = OBJECT_HIDDEN_DIGIT;
	}
}

FHiddenDigit::~FHiddenDigit()
{
    fCurTime = 0.f;
}

void FHiddenDigit::HideDigit()
{
    iObjState = HIDDEN_DIGIT_HIDDING;
}

void FHiddenDigit::WaitDigit()
{
    lpLabel->SetAlpha( 255 );
    lpLabel->SetText( "X" );
    iObjState = HIDDEN_DIGIT_WAIT;
}

void FHiddenDigit::ReadyDigit()
{
    iObjState = HIDDEN_DIGIT_READY;
	lpAnimation->StartAnimation( sDigitStayAnim );
}

FDigit * FHiddenDigit::CreateMirror( const FVector2F & vMirrorPos )
{
	FHiddenDigit * lpDigit = (FHiddenDigit *)AllocObject( "FHiddenDigit", "\\vector\\ui\\world", &vMirrorPos, 10, lpWorld );

    lpDigit->iObjState = iObjState;
    lpDigit->lpAnimation->Scale( 1.f );
    lpDigit->lpLabel->Scale( 1.f );
    lpDigit->fCurTime = fCurTime;
    lpDigit->Move( vMirrorPos );
    
    return lpDigit;
}

void FHiddenDigit::Update( F32 fDTime )
{
    if( lpMirror )
        lpMirror->Update( fDTime );
    
    FDigit::Update( fDTime );
    if( iObjState == DIGIT_STAY )
        iObjState = HIDDEN_DIGIT_SHOW;
    
    fCurTime += fDTime;
    switch( iObjState )
    {
        case HIDDEN_DIGIT_SHOW:
            if( fCurTime > HIDDEN_DIGIT_SHOW_TIME )
            {
                HideDigit();
                fCurTime = 0.f;
            }
            break;
        case HIDDEN_DIGIT_HIDDING:
            lpLabel->SetAlpha( 255.f - 255.f * fCurTime/HIDDEN_DIGIT_HIDDING_TIME );
            if( fCurTime > HIDDEN_DIGIT_HIDDING_TIME )
            {
                WaitDigit();
                fCurTime = 0.f;
            }
            break;
        case HIDDEN_DIGIT_WAIT:
            if( fCurTime > HIDDEN_DIGIT_WAIT_TIME )
            {
                ReadyDigit();
                fCurTime = 0.f;
            }
            break;
        case HIDDEN_DIGIT_READY:
            break;
    }
}

bool FHiddenDigit::IsHidden()const
{
    return true;
}

UI32 FHiddenDigit::Load( void * lpData )
{
    I32 iSize = FDigit::Load( lpData );
    FHiddenDigitBlock * lpBlock = (FHiddenDigitBlock *)((FBYTE *)lpData + iSize);
    fCurTime = lpBlock->fCurTime;
    
    switch( iObjState )
    {
        case HIDDEN_DIGIT_SHOW:
            break;
        case HIDDEN_DIGIT_HIDDING:
            lpLabel->SetAlpha( 255.f - 255.f * fCurTime/HIDDEN_DIGIT_HIDDING_TIME );
            break;
        case HIDDEN_DIGIT_WAIT:
            lpLabel->SetText( "X" );
            break;
        case HIDDEN_DIGIT_READY:
            lpLabel->SetText( "X" );
			lpAnimation->StartAnimation( sDigitStayAnim );
            break;
    }
    
    return sizeof( FHiddenDigitBlock ) + iSize;
}

void FHiddenDigit::Save( FFile * lpFile )
{
    FDigit::Save( lpFile );
    
    FHiddenDigitBlock oBlock = { CUR_VER, fCurTime };
    lpFile->Write( &oBlock, sizeof( oBlock ) );
}