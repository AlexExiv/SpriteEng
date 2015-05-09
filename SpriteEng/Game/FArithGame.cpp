//
//  FGame.cpp
//  Amazing Balls
//
//  Created by Верун on 11.06.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#include "FArithGame.h"
#include "FArithWorld.h"
#include "../2D/FView.h"
#include "FInterface.h"
#include "../Core/FFile.h"
#include "FGameDef.h"
#include "..\2D\FGroupSeq.h"
#include "..\2D\FSerialSeq.h"
#include "..\2D\FSimpleScene.h"
#include "..\Gui\FGuiScene.h"
#include "..\Gui\FGuiView.h"
#include "..\Core\FLog.h"
#include "..\Core\FStack.h"
#include <time.h>



#define CUR_VER 1

struct FGameBlock
{
    UI32 iVersion;
    I32 iScore;
};



const FString sDeffFont( "bundle\\digit2" );
const FString sScoreFont( "bundle\\comic" );
const FString sInterfaceBack( "bundle\\scoresback.bmp" );
const FString sGameOver( "bundle\\gmn.bmp" );
const FString sCellBack( "bundle\\cellback.bmp" );
const FString sPartName( "bundle\\stars.bmp" );
const FString sEndMoveSound( "bundle\\bryak.caf" );
const FString sDestroySound( "bundle\\zvon.caf" );
const FString sTimeBinusSound( "bundle\\magic_mirror.caf" );
const FString sNotRightSound( "bundle\\not_right.caf" );
const FString sAttentionPrepare( "bundle\\prepareatack.bmp" );
const FString sAttentionCounter( "bundle\\counter.bmp" );
const FString sAttentionHoldon( "bundle\\holdon.bmp" );
const FString sAttentionAttackSoon( "bundle\\beready.bmp" );


const FString sDigitStayAnim( "DIGIT_STAY" );
const FString sDigitSelectAnim( "DIGIT_SELECT" );
const FString sDigitDyingAnim( "DIGIT_DYING" );
const FString sHiddenDigitAnim( "HIDDEN_DIGIT" );
const FString sParticleAnim( "BLOW" );
const FString sCounterAnim( "COUNTER" );


FArithGame::FArithGame( ) : FGame( MAX_GAME_OBJECT ), lpWorldSeq( NULL ), lpGuiSeq( NULL ), lpGuiScene( NULL ), lpWorldScene( NULL ), lpWorld( NULL ), lpInterface ( NULL ),
	iScore( 0 ), iGameTime( 0 )
{
	lpGuiSeq = new FSerialSeq( 512 );
	lpGuiScene = new FGuiScene( FView::GetMainView()->GetWidth(), FView::GetMainView()->GetHeight(), lpGuiSeq );
	lpInterface = (FInterface *)AllocObject( "FInterface", "\\scene", lpGuiScene );

	lpWorldSeq = new FGroupSeq( 512 );
	lpWorldScene = new FSimpleScene( FView::GetMainView()->GetWidth(), FView::GetMainView()->GetHeight() 
		- lpInterface->GetView()->GetSize().fHeight, lpWorldSeq );
	lpWorld = (FArithWorld *)AllocObject( "FArithWorld", "\\game", this );
    for( I32 i = 0;i < MAX_CLICK;i++ )
    {
        cClickSeq[i].x = 0.f;
        cClickSeq[i].y = 0.f;
        cClickSeq[i].iPush = 0;
    }
    
    srand( ((unsigned)time( NULL ))%1000 );
    
    Load( "save.sav" );
}    


FArithGame::~FArithGame()
{
	if( lpWorld )
		Delete( lpWorld );
	if( lpInterface )
		Delete( lpInterface );
	if( lpGuiScene )
		delete lpGuiScene;
	if( lpWorldScene )
		delete lpWorldScene;
	if( lpGuiSeq )
		delete lpGuiSeq;
	if( lpWorldSeq )
		delete lpWorldSeq;
}

void FArithGame::Update( F32 fDTime )
{
    lpInterface->Update( fDTime );
    lpWorld->Update( fDTime );
	lpWorldScene->Update( fDTime );
	lpGuiScene->Update( fDTime );
}

void FArithGame::Draw()
{
	FRect rFrustum( 0.f, 0.f, lpWorld->GetWorldWidth(), lpWorld->GetWorldHeight() );

	lpWorldSeq->BeginBuildSeq();
	lpWorldScene->BuildSeq( rFrustum );
	lpWorldSeq->EndBuildSeq();
	lpWorldSeq->DrawSeq();

	lpGuiSeq->BeginBuildSeq();
	lpGuiScene->BuildSeq( rFrustum );
	lpGuiSeq->EndBuildSeq();
	lpGuiSeq->DrawSeq();
}

void FArithGame::Save( const FString & sFileName )
{
	FFile * lpFile = FFile::OpenFile( sFileName, FFile::F_FILE_CREATE | FFile::F_FILE_WRITE );
    
    FGameBlock oBlock = { CUR_VER, iScore };
    lpFile->Write( &oBlock, sizeof( FGameBlock ) );
    
    lpWorld->Save( lpFile );
    
	FFile::CloseFile( lpFile );
}

void FArithGame::Load( const FString & sFileName )
{
	FFile * lpFile = FFile::OpenFile( sFileName, FFile::F_FILE_READ );
	if( !lpFile )
	{
		FLog::PutError( "Can't create save file" );
		return;
	}

	void * lpData = PUSH_BLOCK( lpFile->GetSize() );
    lpFile->Read( lpData, lpFile->GetSize() );
    
    FGameBlock * lpBlock = (FGameBlock *)lpData;
    iScore = lpBlock->iScore;
    lpInterface->SetScore( iScore );
    
    lpWorld->Load( (FBYTE *)lpData + sizeof( FGameBlock ) );
	FFile::CloseFile( lpFile );
	POP_BLOCK;
}

void FArithGame::ClickDown( const FVector2F & vPos, UI32 iIndex )
{
	if( iIndex > MAX_CLICK )
	{
		FLog::PutError( "Dispacth click index is more then MAX_CLICK constant" );
		return;
	}
    cClickSeq[iIndex].x = vPos.x;
    cClickSeq[iIndex].y = vPos.y;
    cClickSeq[iIndex].iPush = 1;
    
    lpWorld->ClickDown( vPos, iIndex );
}

void FArithGame::ClickUp( const FVector2F & vPos, UI32 iIndex )
{
  	if( iIndex > MAX_CLICK )
	{
		FLog::PutError( "Dispacth click index is more then MAX_CLICK constant" );
		return;
	}

	cClickSeq[iIndex].iPush = 0;
    lpWorld->ClickUp( vPos, iIndex );
}

void FArithGame::ClickMove( const FVector2F & vPos, UI32 iIndex )
{
	if( iIndex > MAX_CLICK )
	{
		FLog::PutError( "Dispacth click index is more then MAX_CLICK constant" );
		return;
	}

    lpWorld->ClickMove( vPos, iIndex );
}

UI32 FArithGame::GetScore()const
{
    return iScore;
}

UI32 FArithGame::GetGameTime()const
{
    return iGameTime;
}

FScene * FArithGame::GetWorldScene()const
{
	return lpWorldScene;
}

FScene * FArithGame::GetGuiScene()const
{
	return lpGuiScene;
}


void FArithGame::AddScore( I32 iDScore )
{
    iScore += iDScore;
    lpInterface->SetScore( iScore );
}

void FArithGame::ResetScore()
{
    iScore = 0;
    lpInterface->SetScore( 0 );
    //[lpCntrl ReportArchiviment];
}

void FArithGame::SetGameTime( F32 fGameTime )
{
    iGameTime = fGameTime;
    lpInterface->SetGameTime( fGameTime );
}

void FArithGame::ShowGameOver( bool bShow )
{
    lpInterface->ShowGameOver( bShow );
}

void FArithGame::NewGame()
{
    lpWorld->NewWorld();
}

