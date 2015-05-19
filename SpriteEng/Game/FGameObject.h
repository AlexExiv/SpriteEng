//
//  FObject.h
//  Amazing Balls
//
//  Created by Верун on 10.06.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#ifndef Amazing_Balls_FObject_h
#define Amazing_Balls_FObject_h

#include "..\Base\FObject.h"
#include "..\Math\FVector2D.h"

class FWorld;
class FFile;


class FGameObject : public FObject
{
protected:
	FWorld * lpWorld;

    bool bSolid;
    UI32 iObjState,
        iObjType;
	FVector2F vPos;
    F32 fSpeed;
  
	FGameObject( UI32 iObjReserved );
    FGameObject( UI32 iObjType, FWorld * lpWorld );
	FGameObject( const FVector2F & vPos, UI32 iObjType, FWorld * lpWorld );

public:
    enum
    {
        OBJECT_DEAD = 0
    };
    enum
    {
        OBJECT_DIGIT = 0,
        OBJECT_LABEL,
        OBJECT_PARTICLE,
        OBJECT_BONUS,
        OBJECT_ATTENTION,
        OBJECT_HIDDEN_DIGIT,
        OBJECT_MESSAGE
    };
    
    ~FGameObject();
    
    bool IsSolid()const;
    bool IsDigit()const;
    UI32 GetObjState()const;
    UI32 GetObjType()const;
    virtual F32 GetWidth()const;
    virtual F32 GetHeight()const;
	FVector2F GetPos()const;
	virtual void Move( const FVector2F & vPos );
    
    void Update( F32 fDTime );
	bool ContainPoint( const FVector2F & pPoint );
    
    virtual UI32 Load( void * lpData );
    virtual void Save( FFile * lpFile );

	DEFINE_OBJ_DERIVERED( FGameObject );
};

DEFINE_OBJ_NAME( FGameObject );

#endif
