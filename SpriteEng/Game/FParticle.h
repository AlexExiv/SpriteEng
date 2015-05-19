//
//  FParticle.h
//  Arithmetic Numbers
//
//  Created by Верун on 24.08.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#ifndef Arithmetic_Numbers_FParticle_h
#define Arithmetic_Numbers_FParticle_h

#include "FGameObject.h"

class FObject2D;
class FArithWorld;

class FParticle : public FGameObject
{
    FObject2D * lpAnimation;
    void Init();
    
public:
    FParticle( FArithWorld * lpWorld );
	FParticle( const FVector2F & vPos, FArithWorld * lpWorld );
    ~FParticle();
    
    F32 GetWidth()const ;
    F32 GetHeight()const ;
    void Update( F32 fDTime );
    
    UI32 Load( void * lpData );
    void Save( FFile * lpFile );

	DEFINE_OBJ_DERIVERED( FParticle );
};

DEFINE_OBJ_NAME( FParticle );

#endif
