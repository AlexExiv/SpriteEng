//
//  FParticle.cpp
//  Arithmetic Numbers
//
//  Created by Верун on 24.08.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#include "FParticle.h"
#include "FArithWorld.h"
#include "..\2D\FObject2D.h"
#include "../Core/FFile.h"
#include "..\Base\FBaseTypes.h"



#define CUR_VER 1

struct FParticleBlock
{
    UI32 iVersion;
    
    I32 iCurFrame;
    F32 fFrameTime;
};

IMPLEMENT_OBJ_DERIVERED( FParticle );

FParticle::FParticle( FArithWorld * lpWorld ) : FGameObject( FGameObject::OBJECT_PARTICLE, lpWorld )
{
    bSolid = false;
    Init();
}

FParticle::FParticle( const FVector2F & vPos, FArithWorld * lpWorld ) : FGameObject( vPos, FGameObject::OBJECT_PARTICLE, lpWorld )
{
    bSolid = false;
    Init();
}

FParticle::~FParticle()
{
    if( lpAnimation )
		Delete( lpAnimation );
    lpAnimation = NULL;
}

void FParticle::Init()
{
	lpAnimation = (FObject2D *)AllocObject( MFObject2D, RFVector2F( vPos ), lpWorld->GetScene(), RFUInteger( 1 ), &sPartName, NULL );
	lpAnimation->StartAnimation( sParticleAnim );
    lpAnimation->SetDuration( 18.f );
}

F32 FParticle::GetWidth()const
{
    return lpAnimation->GetWidth();
}

F32 FParticle::GetHeight()const
{
    return lpAnimation->GetHeight();
}

void FParticle::Update( F32 fDTime )
{
    if( lpAnimation->IsAnimEnd() )
        iObjState = OBJECT_DEAD;
}

UI32 FParticle::Load( void * lpData )
{
    UI32 iSize = FGameObject::Load( lpData );
    
    FParticleBlock * lpBlock = (FParticleBlock *)( (FBYTE *)lpData + iSize );
    lpAnimation->SetCurFrame( lpBlock->iCurFrame );
    lpAnimation->SetCurTime( lpBlock->fFrameTime );
    lpAnimation->Move( vPos );
    
    return iSize + sizeof( FParticleBlock );
}

void FParticle::Save( FFile * lpFile )
{
    FGameObject::Save( lpFile );
    
    FParticleBlock oBlock = { CUR_VER, lpAnimation->GetCurFrame(), lpAnimation->GetCurTime() };
    lpFile->Write( &oBlock, sizeof( oBlock ) );
}