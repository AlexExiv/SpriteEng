//
//  FMessage.cpp
//  Arithmetic Numbers
//
//  Created by Верун on 05.01.15.
//  Copyright (c) 2015 Верун. All rights reserved.
//

#include "FMessage.h"
#include "../2D/FText.h"
#include "FGameDef.h"
#include "FArithWorld.h"
#include "..\Base\FBaseTypes.h"


#define MESSAGE_SHOW_TIME 60.f
#define MESSAGE_MOVE_TIME 60.f
#define MESSAGE_HIDE_TIME 60.f

#define MESSAGE_FONT_SIZE 64.f
#define BONUS_FONT_SIZE 54.f



inline F32 GetFract( F32 fFloat )
{
	return fFloat - F32( I32( fFloat ) );
}

IMPLEMENT_OBJ_DERIVERED( FMessage );

FMessage::FMessage( FArithWorld * lpWorld ) : FGameObject( FGameObject::OBJECT_MESSAGE, lpWorld ), fCurTime( 0.f ), fCounterTime( 0.f )
{
    iObjState = MESSAGE_WAIT;
	
	lpLabel = (FText *)AllocObject( MFText, RFVector2F( 0.f, 0.f ), RFColor4F( 1.f, 0.f, 0.f, 1.f ),
		RFFloat( 15.f ), RFString( " " ), &sDeffFont, RFUInteger( 2 ), lpWorld->GetScene(), NULL );

    lpLabel->Show( false );
}

FMessage::~FMessage()
{
	if( lpLabel )
		Delete( lpLabel );
}

void FMessage::BeginTimer( F32 fTime )
{
    fCounterTime += fTime;
    if( iObjState == MESSAGE_WAIT )
    {
        iObjState = MESSAGE_SHOWING;
        fCurTime = 0.f;
        lpLabel->SetFontSize( 0.f );
        lpLabel->Show( true );
    }
    lpLabel->SetText( FString( UI32( fCounterTime/60.f ) ) );
}

bool FMessage::IsEnd()const
{
    return iObjState == MESSAGE_WAIT;
}

F32 FMessage::GetWidth()const
{
    return lpLabel->GetTextWidth();
}

F32 FMessage::GetHeight()const
{
    return lpLabel->GetTextHeight();
}

void FMessage::Move( const FVector2F & vPos )
{
    FGameObject::Move( vPos );
    lpLabel->Move( vPos );
}

void FMessage::Update( F32 fDTime )
{
    if( iObjState == MESSAGE_WAIT )
        return;
    
    fCounterTime -= fDTime;
    if( fCounterTime < 0.f )
        fCounterTime = 0.f;
    lpLabel->SetText( FString( UI32( fCounterTime/60.f ) ) );
    
    fCurTime += fDTime;
    switch( iObjState )
    {
        case MESSAGE_SHOWING:
            lpLabel->SetFontSize( 2.f*fCurTime/MESSAGE_SHOW_TIME*MESSAGE_FONT_SIZE );
            if( fCurTime >= MESSAGE_SHOW_TIME )
                lpLabel->SetFontSize( 2.f*MESSAGE_FONT_SIZE );
            break;
        case MESSAGE_MOVE_TO_POS:
            lpLabel->SetFontSize( (2.f - fCurTime/MESSAGE_MOVE_TIME)*MESSAGE_FONT_SIZE );
            if( fCurTime >= MESSAGE_MOVE_TIME )
                lpLabel->SetFontSize( MESSAGE_FONT_SIZE );
            break;
        case MESSAGE_COUNTER:
            lpLabel->SetFontSize( (1.f - GetFract( fCounterTime/60.f )/3.f)*MESSAGE_FONT_SIZE );
            break;
        case MESSAGE_HIDDING:
            lpLabel->SetFontSize( (1.f - fCurTime/MESSAGE_SHOW_TIME)*MESSAGE_FONT_SIZE );
            if( fCurTime >= MESSAGE_HIDE_TIME )
                lpLabel->SetFontSize( 0.f );
            break;
    }

	F32 fXCenter = lpWorld->GetWorldWidth()/2.f - lpLabel->GetTextWidth()/2.f - 55.f,
		fYCenter = lpWorld->GetWorldHeight()/2.f - lpLabel->GetTextHeight()/2.f - lpLabel->GetFontSize() + 10.f,
		fXRight = lpWorld->GetWorldWidth() - 100.f - lpLabel->GetTextWidth()/2.f,
        fYRight = 50.f - lpLabel->GetTextHeight()/2.f;

    switch( iObjState )
    {
        case MESSAGE_SHOWING:
            Move( FVector2F( fXCenter, fYCenter ) );
            if( fCurTime >= MESSAGE_SHOW_TIME )
            {
                fCurTime = 0.f;
                iObjState = MESSAGE_MOVE_TO_POS;
                
            }
            break;
        case MESSAGE_MOVE_TO_POS:
            Move( FVector2F( fXCenter + fCurTime/MESSAGE_MOVE_TIME*( fXRight - fXCenter ), 
				fYCenter + fCurTime/MESSAGE_MOVE_TIME*(fYRight - fYCenter) ) );
            if( fCurTime >= MESSAGE_MOVE_TIME )
            {
                fCurTime = 0.f;
                iObjState = MESSAGE_COUNTER;
                Move( FVector2F( fXRight, fYRight ) );
            }
            break;
        case MESSAGE_COUNTER:
            Move( FVector2F( fXRight, fYRight ) );
            if( fCounterTime == 0.f )
            {
                iObjState = MESSAGE_HIDDING;
                fCurTime = 0.f;
            }
            break;
        case MESSAGE_HIDDING:
            Move( FVector2F( fXRight, fYRight ) );
            if( fCurTime >= MESSAGE_HIDE_TIME )
            {
                fCurTime = 0.f;
                iObjState = MESSAGE_WAIT;
                lpLabel->Show( false );
                
            }
            break;
    }
}

UI32 FMessage::Load( void * lpData )
{
    return 0;
}

void FMessage::Save( FFile * lpFile )
{
    
}