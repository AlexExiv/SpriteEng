//
//  FDigit.h
//  Arithmetic Numbers
//
//  Created by Верун on 14.07.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#ifndef Arithmetic_Numbers_FDigit_h
#define Arithmetic_Numbers_FDigit_h

#include "FGameObject.h"


class FArithWorld;
class FObject2D;
class FText;

class FDigit : public FGameObject
{
public:
    enum
    {
        DIGIT_MOVE = OBJECT_DEAD + 1,
        DIGIT_STAY,
        DIGIT_PUSHED,
        DIGIT_DYING,
        DIGIT_DESTROY,
        DIGIT_BORN,
        DIGIT_MOVE_TO_POS
    };
    
protected:
    FObject2D * lpAnimation;
    FText * lpLabel;
    UI32 iNum;
    F32 fDestrTime;
    F32 fXLine;//положение цифры в линейной системе: все цифры представлены в один ряд, это из виртуальное положение
    
    FDigit * lpNext;//указатель на след цифру в линейке
    FDigit * lpMirror;
    
    void Init();
    void CenterLabel();//отцентровать цифру по ее заднему фону
    
    void BornDigit( F32 fDTime );
    void Move2Pos( F32 fDTime );
    void DeadTimeOut( F32 fDTime );//отчет задержки перед уничтожение цифры
    void DigitDying( F32 fDTime );
    
    virtual FDigit * CreateMirror( const FVector2F & vPos );//создает зеркало для отображения копии цифры на предыдущей строке
    F32 GetFieldWidth()const;//реальная ширина игрового поля
    
public:
    FDigit( FArithWorld * lpWorld );
	FDigit( const FVector2F & vPos, UI32 iNum, FArithWorld * lpWorld );
    ~FDigit();
    
    F32 GetWidth()const ;
    F32 GetHeight()const ;
    F32 GetLineX()const;
    UI32 GetNum()const;
    
    void Update( F32 fDTime );
    void StrideLeft( F32 fDTime );//сдвинуть цифру влево если там пусто, рекурсивная функция
    void SetNext( FDigit * lpNext );
    bool IsFirst()const;
    void Move(const FVector2F & vPos );
    void Destroy();
    bool IsAdjacent()const;
    virtual bool IsHidden()const;//если цифра скрытая
    bool IsDying()const;
    bool IsBorning()const;
    void Select( bool bSel );
    
    virtual UI32 Load( void * lpData );
    virtual void Save( FFile * lpFile );
};

#endif
