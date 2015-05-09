//
//  FSoundObject.cpp
//  Amazing Balls
//
//  Created by Верун on 30.06.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#include "FSoundObject.h"
#include "FSoundManager.h"
#include "FSound.h"



FSoundObject::FSoundObject( const FString & sFileName, FSoundManager * lpSoundManager ) : lpSoundManager( lpSoundManager ), iSourceInd( -1 )
{
    lpSound = lpSoundManager->CreateSound( sFileName, false );
}

FSoundObject:: ~FSoundObject()
{
    Stop();
    lpSoundManager->ReleaseSound( lpSound );
}

UI32 FSoundObject::GetSoundID()const
{
    return lpSound->GetSoundID();
}

I32 FSoundObject::GetSourceInd()const
{
    return iSourceInd;
}

void FSoundObject::SetSourceInd( I32 iSourceInd0 )
{
    iSourceInd = iSourceInd0;
}

void FSoundObject::Play( bool bLoop  )
{
    lpSoundManager->PlaySound( this, bLoop );
}

void FSoundObject::Pause()
{
    //lpSoundManager->PauseSound( this );
}

void FSoundObject::Stop()
{
    lpSoundManager->StopSound( this );
}
