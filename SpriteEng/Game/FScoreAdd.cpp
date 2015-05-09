//
//  FScoreAdd.cpp
//  Amazing Balls
//
//  Created by Верун on 06.07.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#include "FScoreAdd.h"
#include "FGameDef.h"
#include "..\2D\FText.h"
#include "FArithWorld.h"
#include "..\Core\FFile.h"
#include "..\2D\FScene.h"


#define CUR_VER 1

#pragma pack( push,  1 )

struct FScoreBlock
{
    UI32 iVersion;
    F32 fShowTime;
    FColor4F cTextColor;
    CHAR_ cLabelStr;
};

#pragma pack( pop )


FScoreAdd::FScoreAdd( FArithWorld * lpWorld ) : FGameObject( OBJECT_LABEL, lpWorld )
{
    
}

FScoreAdd::FScoreAdd( const FVector2F & vPos, FArithWorld * lpWorld, I32 iIncrScore ) : 
	FGameObject( vPos, FGameObject::OBJECT_LABEL, lpWorld ), fShowTime( 0.f ), lpLabel( NULL )
{
    fSpeed = 3.f;
    iObjState = SCORE_SHOW;
    bSolid = false;
    FString sScore;
    
    if( iIncrScore == 0 )
        return;
    
    if( iIncrScore > 0 )
        sScore = FString( "+" ) + FString( iIncrScore );
    else
        sScore = FString( "-" ) + FString( iIncrScore );

	FColor4F cColor;
    if( iIncrScore > 0 )
        cColor = FColor4F( 0.f, 1.f, 0.f, 1.f );
    else
        cColor = FColor4F( 1.f, 0.f, 0.f, 1.f );
    
	lpLabel = (FText *)AllocObject( "FText", "\\vector\\color\\f\\string\\string\\ui\\scene", &vPos, &cColor, 15, &sScore, 
		&sScoreFont, 1, lpWorld->GetScene() );
    lpLabel->SetAlpha( 0.f );
    
}


FScoreAdd::~FScoreAdd()
{
    if( lpLabel )
        Delete( lpLabel );
    lpLabel = NULL;
}

F32 FScoreAdd::GetWidth()const
{
    return 1.f;
}

F32 FScoreAdd::GetHeight()const
{
    return 1.f;
}

void FScoreAdd::Move( const FVector2F & vPos )
{
    
}

void FScoreAdd::Update( F32 fDTime )
{
    fShowTime += fDTime;
    
    switch( iObjState )
    {
        case SCORE_SHOW:
            lpLabel->SetAlpha( F32( 255 )*fShowTime/SHOW_SCORE_TIME );
            if( fShowTime > SHOW_SCORE_TIME )
            {
                fShowTime = 0.f;
                iObjState = SCORE_GO_UP;
                lpLabel->SetAlpha( 255 );
            }

            break;
        case SCORE_GO_UP:
            if( fShowTime > SCORE_TIME )
            {
                fShowTime = 0.f;
                iObjState = SCORE_HIDE;
            }
            break;
        case SCORE_HIDE:
            lpLabel->SetAlpha( F32( 255 )*(1.f - fShowTime/SHOW_SCORE_TIME) );
            if( fShowTime > HIDE_SCORE_TIME )
            {
                fShowTime = 0.f;
                iObjState = FGameObject::OBJECT_DEAD;
            }
            break;
    }
        
    vPos.y += fDTime*fSpeed;
    lpLabel->Move( vPos );
}

bool FScoreAdd::IsLineIntersect( F32 x0, F32 y0, F32 x1, F32 y1 )
{
    return true;
}

bool FScoreAdd::IsObjIntersect( FObject * lpObj )
{
    return true;
}

void FScoreAdd::Blow( I32 iCameFrom, F32 fTimeToBlow )
{
    
}

UI32 FScoreAdd::Load( void * lpData )
{
    UI32 iSize = FGameObject::Load( lpData );
    FScoreBlock * lpBlock = (FScoreBlock *)((FBYTE *)lpData + iSize);
    fShowTime = lpBlock->fShowTime;

    FString sScore = &lpBlock->cLabelStr;
	lpLabel = (FText *)AllocObject( "FText", "\\vector\\color\\f\\string\\string\\ui\\scene", &vPos, &lpBlock->cTextColor, 15, &sScore, 
		&sScoreFont, 1, lpWorld->GetScene() );
    lpLabel->SetAlpha( 0 );
    
    return iSize + sScore.Length() + sizeof( FScoreBlock );
}

void FScoreAdd::Save( FFile * lpFile )
{
    FGameObject::Save( lpFile );
	FScoreBlock oBlock = { CUR_VER, fShowTime, lpLabel->GetColor() };
    lpFile->Write( &oBlock, sizeof( FScoreBlock ) - sizeof( CHAR_ ) );
    lpFile->Write( lpLabel->GetText().GetChar(), lpLabel->GetText().Length() + 1 );
}

