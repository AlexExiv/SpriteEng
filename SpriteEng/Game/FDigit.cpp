//
//  FDigit.cpp
//  Arithmetic Numbers
//
//  Created by Верун on 14.07.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#include "FDigit.h"
#include "../2D/FObject2D.h"
#include "../2D/FText.h"
#include "FGameDef.h"
#include "FArithWorld.h"
//#include "../Sound/FSoundManager.h"
#include "../Core/FFile.h"
#include "..\Core\FLog.h"
#include "..\Base\FBaseTypes.h"


#define CUR_VER 2

#define DIGIT_BORN_TIME 35.f
#define DIGIT_MOVE_TIME 75.f
//#pragma  pack( push, 1 )

struct FDigitBlock1
{
    UI32 iVersion;
    
    F32 fDestrTime;
    I32 iNum;
    bool bMirror;
    bool bReserved[3];
};

struct FDigitBlock2
{
    UI32 iVersion;
    
    F32 fDestrTime;
    I32 iNum;
    I32 iCurFrame;
    F32 fAnimTime;
    bool bMirror;
    bool bReserved[3];
};


typedef FDigitBlock2 FDigitBlock;

//#pragma pack( pop )

IMPLEMENT_OBJ_DERIVERED( FDigit );

FDigit::FDigit( FArithWorld * lpWorld ) : FGameObject( FGameObject::OBJECT_DIGIT, lpWorld ), lpAnimation( NULL ), lpLabel( NULL ), iNum( 0 ), lpNext( NULL ), lpMirror( NULL )
{
    
    Init();
}

FDigit::FDigit( const FVector2F & vPos, UI32 iNum, FArithWorld * lpWorld ) : FGameObject( vPos, FGameObject::OBJECT_DIGIT, lpWorld ), 
	lpAnimation( NULL ), lpLabel( NULL ), iNum( iNum ), lpNext( NULL ), lpMirror( NULL ), fDestrTime( 0.f )
{
    Init();
	fXLine = ((lpWorld->GetWorldHeight() - vPos.y)/GetHeight() - 1.f)*GetFieldWidth() + vPos.x;
}

FDigit::~FDigit()
{
    if( lpAnimation )
		Delete( lpAnimation );
    if( lpLabel )
		Delete( lpLabel );
    if( lpMirror )
		Delete( lpMirror );
    
    lpAnimation = NULL;
    lpLabel = NULL;
    lpMirror = NULL;
}

void FDigit::Init()
{
	lpAnimation = (FObject2D *)AllocObject( MFObject2D, RFVector2F( vPos ), lpWorld->GetScene(), RFUInteger( 0 ), &sCellBack, NULL );
	lpAnimation->StartAnimation( sDigitStayAnim );
    if( iNum != 0)
	{
		lpLabel = (FText *)AllocObject( MFText, RFVector2F( vPos ), RFColor4F( 0.f, 0.f, 0.f, 1.f ), 
			RFFloat( NUMBER_SIZE ), &FString( iNum ), &sDeffFont, RFUInteger( 1 ), lpWorld->GetScene(), NULL );
	}
    
    lpAnimation->Scale( 0.f );
    if( lpLabel )
        lpLabel->Scale( 0.f );
	lpAnimation->Move( FVector2F( (lpWorld->GetWorldWidth() - lpAnimation->GetScaleWidth())/2.f, 20.f ) );
    CenterLabel();
    fSpeed = 2.f;
    
    Select( false );
    iObjState = DIGIT_BORN;
}

void FDigit::CenterLabel()
{
    if( !lpLabel )
        return;
    
    F32 fLabX = lpAnimation->GetX() + (lpAnimation->GetScaleWidth() - lpLabel->GetScaleTextWidth())/2.f;
    F32 fLabY = lpAnimation->GetY() + (lpAnimation->GetScaleHeight() - lpLabel->GetScaleTextHeight())/2.f;
    
    lpLabel->SetPos( FVector2F( fLabX, fLabY ) );
}

FDigit * FDigit::CreateMirror( const FVector2F & vMirrorPos )
{
	FDigit * lpDigit = (FDigit *)AllocObject( MFDigit, &FVector2F_( vMirrorPos ), FUInteger( iNum ), lpWorld, NULL );
    lpDigit->iObjState = iObjState;
    lpDigit->lpAnimation->Scale( 1.f );
    lpDigit->lpLabel->Scale( 1.f );
    
    return lpDigit;
}

F32 FDigit::GetFieldWidth()const
{
    return NUMBER_IN_LINE*GetWidth();
}

F32 FDigit::GetWidth()const
{
    return lpAnimation->GetWidth();
}

F32 FDigit::GetHeight()const
{
    return lpAnimation->GetHeight();
}

F32 FDigit::GetLineX()const
{
    return fXLine;
}

UI32 FDigit::GetNum()const
{
    return iNum;
}

void FDigit::BornDigit( F32 fDTime )
{
    fDestrTime += fDTime;
    F32 fScale = 3.f*fDestrTime/DIGIT_BORN_TIME;
    lpAnimation->Scale( fScale );
    lpLabel->Scale( fScale );
    
    if( fDestrTime > DIGIT_BORN_TIME )
    {
        iObjState = DIGIT_MOVE_TO_POS;
        fDestrTime = 0.f;
        lpAnimation->Scale( 3.f );
        lpLabel->Scale( 3.f );
    }
	lpAnimation->SetPos( FVector2F( (lpWorld->GetWorldWidth() - lpAnimation->GetScaleWidth())/2.f, 20.f ) );
    CenterLabel();
}

void FDigit::Move2Pos( F32 fDTime )
{
    fDestrTime += fDTime;
    F32 xn = (lpWorld->GetWorldWidth() - lpAnimation->GetScaleWidth())/2.f + (fDestrTime/DIGIT_MOVE_TIME)*(vPos.x - (lpWorld->GetWorldWidth() - lpAnimation->GetScaleWidth())/2.f),
    yn = 20.f + (fDestrTime/DIGIT_MOVE_TIME)*(vPos.y - 20.f);
    lpAnimation->SetPos( FVector2F( xn, yn ) );
    F32 fScale = 4.f - 3.f*fDestrTime/DIGIT_MOVE_TIME;
    lpAnimation->Scale( fScale );
    lpLabel->Scale( fScale );
    
    if( fDestrTime > DIGIT_MOVE_TIME )
    {
        iObjState = DIGIT_STAY;
        fDestrTime = 0.f;
        lpAnimation->Scale( 1.f );
        lpLabel->Scale( 1.f );
        lpAnimation->SetPos( vPos );
    }
    
    CenterLabel();
}

void FDigit::DeadTimeOut( F32 fDTime )
{
    fDestrTime += fDTime;
    if( fDestrTime > DIGIT_TIME_TO_DESTROY )
    {
        iObjState = DIGIT_DYING;
		lpAnimation->StartAnimation( sDigitDyingAnim );
        lpAnimation->SetDuration( 2 );
        fDestrTime = 0.f;
    }
}

void FDigit::DigitDying( F32 fDTime )
{
    fDestrTime += fDTime;
    if( lpAnimation->IsAnimEnd() )
        iObjState = OBJECT_DEAD;
}

void FDigit::Update( F32 fDTime )
{
    switch( iObjState )
    {
        case DIGIT_BORN:
            BornDigit( fDTime );
            break;
        case DIGIT_MOVE_TO_POS:
            Move2Pos( fDTime );
            break;
        case DIGIT_DESTROY:
            DeadTimeOut( fDTime );
            break;
        case DIGIT_DYING:
            DigitDying( fDTime );
            break;
    }
}

//сдвиг цифры влево, сдвиг осуществляется рекурсивно, т.е. с конца линейки по очереди до первой цифры что не имеет общей границы с соседом
void FDigit::StrideLeft( F32 fDTime )
{
    if( !lpNext )//если нет след запись, то делаем вычисления и разворачиваем рекурсию
    {
        if( fXLine == 0.f )//если начало поля то прекращаем сдвиг
            return;
        
        fXLine -= fDTime * fSpeed;
        if( fXLine < 0.f )//если в результате сдвига ушли дальше начала
        {
            //lpWorld->GetSoundManager()->PlaySound( END_MOVE_SOUND );
            fXLine = 0.f;
        }
    }
    else
    {
        if( (lpNext->GetLineX() + lpNext->GetWidth()) == fXLine )//если с соседом есть общая граница
        {
            lpNext->StrideLeft( fDTime );//рекурсивный вызов
            fXLine = lpNext->GetLineX() + lpNext->GetWidth();//вычисляем новое положение в виртуальной линии
        }
        else//иначе осуществляем сдвиг и разворачиваем рекурсию
        {
            fXLine -= fDTime*fSpeed;
            if( fXLine < (lpNext->GetLineX() + lpNext->GetWidth()) )
            {
                fXLine = lpNext->GetLineX() + lpNext->GetWidth();
                //lpWorld->GetSoundManager()->PlaySound( END_MOVE_SOUND );
            }
        }
    }
    
    UI32 iFieldW = GetFieldWidth();
	//вычисляем реальные координаты из виртуальных
    vPos.x = F32(I32( fXLine ) % iFieldW) + (fXLine - F32( I32(fXLine) ));
	vPos.y = lpWorld->GetWorldHeight() - (I32( fXLine )/iFieldW + 1)*GetHeight();
	//FLog::PutMessage( "FDigit pos: %f, %f", vPos.x, vPos.y );

	//если х + ширина цифры больше ширины игрового поля, то значит цифра у нас в стадии переноса на новую строку и не видна полность, необходимо зеркало
    if( ((vPos.x + GetWidth()) > iFieldW) && !IsBorning() )
    {
        if( !lpMirror )
			lpMirror = CreateMirror( FVector2F( -iFieldW + vPos.x, lpWorld->GetWorldHeight() - (I32( fXLine )/iFieldW + 2)*GetHeight() ) );
        else
			lpMirror->Move( FVector2F( -iFieldW + vPos.x, lpWorld->GetWorldHeight() - (I32( fXLine )/iFieldW + 2)*GetHeight() ) );
    }
    
    if( lpMirror )
    {
        if( (vPos.x + GetWidth()) <= iFieldW )
        {
            Delete( lpMirror );
            lpMirror = NULL;
        }
    }
    
    if( !IsBorning() )
		Move( vPos );
}

void FDigit::SetNext( FDigit *lpNext0 )
{
    lpNext = lpNext0;
}

bool FDigit::IsFirst()const
{
    return (lpNext == NULL);
}

void FDigit::Move( const FVector2F & vPos_ )
{
    FGameObject::Move( vPos_ );
    lpAnimation->SetPos( vPos_ );
    CenterLabel();
}

void FDigit::Destroy()
{
    iObjState = DIGIT_DESTROY;
    fDestrTime = 0.f;
}

bool FDigit::IsAdjacent()const
{
	if( lpNext->GetPos().y == vPos.y )
		return ((lpNext->GetPos().x + lpNext->GetWidth()) == vPos.x);
        
	return ((vPos.x + lpWorld->GetWorldWidth()) == (lpNext->GetPos().x + lpNext->GetWidth()));

}

bool FDigit::IsHidden()const
{
    return  false;
}

bool FDigit::IsDying()const
{
    return (iObjState == OBJECT_DEAD) || (iObjState == DIGIT_DESTROY) || (iObjState == DIGIT_DYING);
}

bool FDigit::IsBorning()const
{
    return (iObjState == DIGIT_BORN) || (iObjState == DIGIT_MOVE_TO_POS);
}

void FDigit::Select( bool bSel )
{
    if( IsDying() )
        return;
    
    if( bSel )
    {
        iObjState = DIGIT_PUSHED;
		lpAnimation->StartAnimation( sDigitSelectAnim );
    }
    else
    {
        iObjState = DIGIT_STAY;
		lpAnimation->StartAnimation( sDigitStayAnim );
    }
}

UI32 FDigit::Load( void * lpData )
{
    I32 iSize = FGameObject::Load( lpData );
    FDigitBlock * lpBlock = (FDigitBlock *)( (FBYTE *)lpData + iSize );
    I32 iBlockSize = 0;
    
    if( lpBlock->iVersion == CUR_VER )
    {
        iBlockSize = sizeof( FDigitBlock );
        fDestrTime = lpBlock->fDestrTime;
        iNum = lpBlock->iNum;
    
        if( lpBlock->bMirror )
        {
            lpMirror = CreateMirror( FVector2F( 0.f, 0.f ) );
            iSize += lpMirror->Load( (FBYTE *)lpData + iSize + sizeof( FDigitBlock ) );
        }
    
        if( iObjState == DIGIT_DYING )
        {
            lpAnimation->SetCurFrame( lpBlock->iCurFrame );
            lpAnimation->SetCurTime( lpBlock->fAnimTime );
        }
    }
    else
    {
        iBlockSize = sizeof( FDigitBlock1 );
        FDigitBlock1 * lpBlock1 = (FDigitBlock1 *)( (FBYTE *)lpData + iSize );
        
        fDestrTime = lpBlock1->fDestrTime;
        iNum = lpBlock1->iNum;
        
        if( lpBlock1->bMirror )
        {
            lpMirror = CreateMirror( FVector2F( 0.f, 0.f ) );
            iSize += lpMirror->Load( (FBYTE *)lpData + iSize + sizeof( FDigitBlock ) );
        }
    }

    lpLabel = (FText *)AllocObject( MFText, &FVector2F_( vPos ), &FColor4F_( 0.f, 0.f, 0.f, 1.f ), 
			&FFloat( NUMBER_SIZE ), &FString( iNum ), &sDeffFont, &FUInteger( 1 ), lpWorld->GetScene(), NULL );
    
	fXLine = ((lpWorld->GetWorldHeight() - vPos.y)/GetHeight() - 1.f)*GetFieldWidth() + vPos.x;
    lpAnimation->SetPos( vPos );
    lpAnimation->Scale( 1.f );
    lpLabel->Scale( 1.f );
    lpLabel->SetFontSize( NUMBER_SIZE );
    CenterLabel();
    if( iObjState == DIGIT_PUSHED )
        iObjState = DIGIT_STAY;
    
    fSpeed = 2.f;
    
    return iSize + iBlockSize;
}

void FDigit::Save( FFile * lpFile )
{
    FGameObject::Save( lpFile );
    
    FDigitBlock oBlock = { CUR_VER, fDestrTime, iNum, lpAnimation->GetCurFrame(), lpAnimation->GetCurTime(), lpMirror ? true : false };
    lpFile->Write( &oBlock, sizeof( oBlock ) );
    
    if( lpMirror )
        lpMirror->Save( lpFile );
}