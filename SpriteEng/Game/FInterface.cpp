//
//  FInterface.cpp
//  Amazing Balls
//
//  Created by Верун on 15.06.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#include "FInterface.h"
#include "..\Gui\FGuiImage.h"
#include "..\Gui\FGuiLabel.h"
#include "..\Gui\FGuiScene.h"
#include "FGameDef.h"



#define INTERFACE "inter.png"
#define PROG_X 40
#define PROG_Y 35
#define PROG_W 	120
#define PROG_H 25

#define ZERO_NUM 8
#define SCORES_INCR_TIME 25.f


FInterface::FInterface( FGuiScene * lpGuiScene ) : FGuiController( lpGuiScene ), fScoreTime( 60.f ), iLastScore( 0 ),
	 iMaxScore( 0 ), lpGameOver( NULL ), fShowTimeDC( 0.f )
{
	FRect rFrame;
	cTime[TIME_LEN - 1] = 0;
	lpMainView = (FGuiView *)AllocObject( "FGuiView", "\\rect\\guicontroller", &rFrame, this );
	lpUpper = (FGuiImage *)AllocObject( "FGuiImage", "\\vector\\string\\guiview", &FVector2F( 0.f, 0.f ), &sInterfaceBack, lpMainView );
	lpGameOver = (FGuiImage *)AllocObject( "FGuiImage", "\\vector\\string\\guiview", &FVector2F( 0.f, 0.f ), &sGameOver, lpMainView );
	lpScore = (FGuiLabel *)AllocObject( "FGuiLabel", "\\vector\\string\\string\\color\\f\\guiview", &FVector2F( 0.f, 0.f ), &FString( "00000000" ),
		&sDeffFont, &FColor4F( 1.f, 0.f, 0.f, 1.f ), 12.f, lpMainView );
	lpTime = (FGuiLabel *)AllocObject( "FGuiLabel", "\\vector\\string\\string\\color\\f\\guiview", &FVector2F( 0.f, 0.f ), &FString( "0" ),
		&sDeffFont, &FColor4F( 0.f, 0.f , 0.f, 1.f ), 12.f, lpMainView );

	rFrame.fLeft = 0.f;
	rFrame.fBottom = lpGuiScene->GetHeight() - lpUpper->GetSize().fHeight;
	rFrame.fRight = lpUpper->GetSize().fWidth;
	rFrame.fTop = lpGuiScene->GetHeight();
	lpMainView->SetFrame( rFrame );

	FVector2F vScorePos( 205.f, (lpUpper->GetSize().fHeight - lpScore->GetSize().fHeight)/2.f );
	FVector2F vTimePos( 30.f, (lpUpper->GetSize().fHeight - lpScore->GetSize().fHeight)/2.f );
	FVector2F vGOPos( (lpMainView->GetSize().fWidth - lpGameOver->GetSize().fWidth)/2.f,
		(lpMainView->GetSize().fHeight - lpGameOver->GetSize().fHeight)/2.f );

	lpScore->SetPos( vScorePos );
	lpTime->SetPos( vTimePos );
	lpGameOver->SetPos( vGOPos );
	lpGameOver->Visible( false );
}

FInterface::~FInterface()
{
	if( lpScore )
		Delete( lpScore );
	if( lpTime )
		Delete( lpTime );
	if( lpUpper )
		Delete( lpUpper );
	if( lpGameOver )
		Delete( lpGameOver );
    
	lpScore = NULL;
	lpTime = NULL;
	lpUpper = NULL;
    lpGameOver = NULL;
}

void FInterface::SetStrScore( UI32 iScore )
{
	CHAR_ cScore[ZERO_NUM  + 1] = "00000000";
    if( iScore < 0 )
        return;
    
    I32 iPos = 7;
    while( iScore != 0 )
    {
        UI32 iNum = iScore % 10;
        iScore /= 10;
        cScore[iPos] = iNum + 48;
        iPos--;
		if( iPos < 0 )
			break;
    }

	lpScore->SetText( cScore );
}

void FInterface::SetTime0( UI32 iTime0 )
{
    cTime[1] = iTime0 % 10 + 48;
    cTime[0] = (iTime0 / 10) % 10 + 48;
}

void FInterface::SetTime1( UI32 iTime1 )
{
    cTime[4] = iTime1 % 10 + 48;
    cTime[3] = (iTime1 / 10) % 10 + 48;
}

void FInterface::SetGameTime( F32 fGameTime )
{
    if( fGameTime / 60.f < 60.f )
    {
        SetTime0( 0 );
        SetTime1( UI32( fGameTime / 60.f ) );
    }
    else
    {
        F32 fMin = fGameTime/(60.f * 60.f);
        if( fMin < 60.f )
        {
            SetTime0( UI32( fMin ) );
            SetTime1( UI32( ( fMin - F32( UI32( fMin ) ) )*60.f ) );
        }
        else
        {
            F32 fHour = fGameTime/(60.f * 60.f * 60.f);
            if( fHour < 24.f )
            {
                SetTime0( UI32( fHour ) );
                SetTime1( UI32( ( fHour - F32( UI32( fHour ) ) )*60.f ) );
            }
            else
            {
                F32 fDay = fGameTime/(24.f * 60.f * 60.f * 60.f);
                SetTime0( UI32( fDay ) );
                SetTime1( UI32( ( fDay - F32( UI32( fDay ) ) )*60.f ) );
            }
        }
    }
}

void FInterface::SetScore( UI32 iScore )
{
    iLastScore = iMaxScore;
    iMaxScore = iScore;
    fScoreTime = 0.f;
}

void FInterface::Update( F32 fDTime )
{
    fShowTimeDC += fDTime;
    if( fShowTimeDC > DOUBLE_COMMA_TIME )
    {
        fShowTimeDC = 0.f;
		if( cTime[3] == ' ' )
			cTime[3] = ':';
		else
			cTime[3] = ' ';
		lpScore->SetText( cTime );
    }
    
    if( fScoreTime >= SCORES_INCR_TIME )
        return;
    
    fScoreTime += fDTime;
    if( fScoreTime > SCORES_INCR_TIME )
        fScoreTime = SCORES_INCR_TIME;
    
    UI32 iCurScore = iLastScore + F32(iMaxScore - iLastScore) * fScoreTime/SCORES_INCR_TIME;
    SetStrScore( iCurScore );
}

void FInterface::ShowGameOver( bool bShow )
{
	lpGameOver->Visible( bShow );
}

