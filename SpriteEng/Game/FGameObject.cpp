//
//  FObject.cpp
//  Amazing Balls
//
//  Created by Верун on 14.06.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#include "FGameObject.h"
#include "FWorld.h"
#include "..\Core\FFile.h"
#include "..\2D\FScene.h"



#define CUR_VER 1

struct FObjectBlock
{
    UI32 iVersion;
    bool bSolid;
    I32 iObjState;
    I32 iObjType;
    F32 x, y;
    F32 fSpeed;
};

FGameObject::FGameObject( UI32 iObjType, FWorld * lpWorld ) : iObjType( iObjType ),  bSolid( true ), fSpeed( 1.f ), iObjState( 1 ), lpWorld( lpWorld )
{
}

FGameObject::FGameObject( const FVector2F & vPos, UI32 iObjType, FWorld * lpWorld ) : vPos( vPos ), iObjType( iObjType ), 
	bSolid( true ), fSpeed( 1.f ), iObjState( 1 ), lpWorld( lpWorld )
{
    
}

FGameObject::~FGameObject()
{
    
}

bool FGameObject::IsSolid()const
{
    return bSolid;
}

bool FGameObject::IsDigit()const
{
    return (iObjType == OBJECT_HIDDEN_DIGIT) || (iObjType == OBJECT_DIGIT);
}

UI32 FGameObject::GetObjState()const
{
    return iObjState;
}

UI32 FGameObject::GetObjType()const
{
    return iObjType;
}

FVector2F FGameObject::GetPos()const
{
	return vPos;
}

void FGameObject::Move( const FVector2F & vPos_ )
{
	vPos = vPos_;
}

bool FGameObject::ContainPoint( const FVector2F & pPoint )
{
    if( (pPoint.x >= vPos.x) && (pPoint.y >= vPos.y) && (pPoint.x < (vPos.x + GetWidth())) && (pPoint.y < (vPos.y + GetHeight())) )
        return true;
    
    return false;
}

void FGameObject::Update( F32 fDTime )
{
}

UI32 FGameObject::Load( void * lpData )
{
    FObjectBlock * lpBlock = (FObjectBlock *)lpData;
    
    bSolid = lpBlock->bSolid;
    iObjState = lpBlock->iObjState;
    iObjType = lpBlock->iObjType;
    vPos.x = lpBlock->x;
    vPos.y = lpBlock->y;
    fSpeed = lpBlock->fSpeed;
    
    return sizeof( FObjectBlock );
}

void FGameObject::Save( FFile * lpFile )
{
    FObjectBlock oBlock = { CUR_VER, bSolid, iObjState, iObjType, vPos.x, vPos.y, fSpeed };
    lpFile->Write( &oBlock, sizeof( FObjectBlock ) );
}
