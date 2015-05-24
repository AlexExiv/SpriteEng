//
//  FBonus.cpp
//  Arithmetic Numbers
//
//  Created by Верун on 26.08.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#include "FBonus.h"
#include "..\2D\FImage2D.h"
#include "..\Core\FString.h"
#include "FArithWorld.h"
#include "..\2D\FScene.h"
#include "..\Core\FFile.h"
#include "..\Base\FBaseTypes.h"
#include "..\2D\FGraphObject.h"



#define CUR_VER 1

struct FBonusBlock
{
    UI32 iVersion;
    
    I32 iBonusTime;
    F32 fShowTime;
};

const FString sBonusNames[] = { "bundle\\firstbonus.bmp", "bundle\\timebonus.bmp", "bundle\\bonus2X.bmp", "bundle\\bonus_score.bmp", "bundle\\bonus_pairs.bmp" };
static bool bIsFirstBonus = true;

IMPLEMENT_OBJ_DERIVERED( FBonus );

FBonus::FBonus( FArithWorld * lpWorld ) : FGameObject( FGameObject::OBJECT_BONUS, lpWorld ), lpImage( NULL ), fShowTime( 0.f ), iBonusType( -1 )
{
 	if( bIsFirstBonus )
	{
		for( UI32 i = 0;i < ARRAY_SIZE( sBonusNames );i++ )
			FGraphObjectManager::GetInstance()->ChacheObject( sBonusNames[i], FGraphObjectManager::OBJECT_IMAGE );
		bIsFirstBonus = false;
	}

    fSpeed = 3.f;
    bSolid = false;
    iObjState = BONUS_SHOW;
}


FBonus::FBonus( const FVector2F & vPos, UI32 iBonusType, FArithWorld * lpWorld ) : FGameObject( vPos, FGameObject::OBJECT_BONUS, lpWorld ), lpImage( NULL ), fShowTime( 0.f ), iBonusType( iBonusType )
{
	if( bIsFirstBonus )
	{
		for( UI32 i = 0;i < ARRAY_SIZE( sBonusNames );i++ )
			FGraphObjectManager::GetInstance()->ChacheObject( sBonusNames[i], FGraphObjectManager::OBJECT_IMAGE );
		bIsFirstBonus = false;
	}

    fSpeed = 3.f;
	lpImage = (FImage2D *)AllocObject( MFImage2D, RFVector2F( vPos ), lpWorld->GetScene(), RFUInteger( 1 ), &sBonusNames[iBonusType] );
    bSolid = false;
    iObjState = BONUS_SHOW;
    Move( FVector2F( vPos.x - lpImage->GetWidth()/2.f, vPos.y - lpImage->GetHeight()/2.f ) );
    lpImage->Move( vPos );
}

FBonus::~FBonus()
{
    if( lpImage )
		Delete( lpImage );
    lpImage = NULL;
}

F32 FBonus::GetWidth()const
{
	if( !lpImage )
		return 0;
    return lpImage->GetWidth();
}

F32 FBonus::GetHeight()const
{
	if( !lpImage )
		return 0;
    return lpImage->GetHeight();
}

void FBonus::Update( F32 fDTime )
{
	if( !lpImage )
		return;

    fShowTime += fDTime;
    
    switch( iObjState )
    {
        case BONUS_SHOW:
            lpImage->SetAlpha( F32( 255 )*fShowTime/SHOW_BONUS_TIME );
            if( fShowTime > SHOW_BONUS_TIME )
            {
                fShowTime = 0.f;
                iObjState = BONUS_GO_UP;
                lpImage->SetAlpha( 255 );
            }
            
            break;
        case BONUS_GO_UP:
            vPos.y += fDTime*fSpeed;
    
            if( fShowTime > BONUS_TIME )
            {
                fShowTime = 0.f;
                iObjState = BONUS_HIDE;
            }
            break;
        case BONUS_HIDE:
            lpImage->SetAlpha( F32( 255 )*(1.f - fShowTime/HIDE_BONUS_TIME) );
            if( fShowTime > HIDE_BONUS_TIME )
            {
                fShowTime = 0.f;
                iObjState = FGameObject::OBJECT_DEAD;
            }
            break;
    }
    if( lpImage )
        lpImage->Move( vPos );
    
}

UI32 FBonus::Load( void * lpData )
{
    I32 iSize = FGameObject::Load( lpData );
    
    FBonusBlock * lpBlock = (FBonusBlock *)( (FBYTE *)lpData + iSize );
    iBonusType = lpBlock->iBonusTime;
    fShowTime = lpBlock->fShowTime;
    
	lpImage = (FImage2D *)AllocObject( MFImage2D, &FVector2F_( vPos ), lpWorld->GetScene(), &FUInteger( 1 ), &sBonusNames[iBonusType] );
    
    return sizeof( FBonusBlock ) + iSize;
}

void FBonus::Save( FFile * lpFile )
{
    FGameObject::Save( lpFile );
    FBonusBlock oBlock = { CUR_VER, iBonusType, fShowTime };
    lpFile->Write( &oBlock, sizeof( oBlock ) );
}
