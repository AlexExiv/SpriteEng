//
//  FInterface.h
//  Amazing Balls
//
//  Created by Верун on 10.06.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#ifndef Amazing_Balls_FInterface_h
#define Amazing_Balls_FInterface_h

#include "..\Core\FString.h"
#include "..\Gui\FGuiController.h"

class FGuiImage;
class FGuiLabel;
class FGame;

class FInterface : public FGuiController
{
	enum
	{
		TIME_LEN = 6
	};

	CHAR_ cTime[TIME_LEN];
    F32 fShowTimeDC;
    UI32 iLastScore, iMaxScore;
    F32 fScoreTime;
    FGuiImage * lpUpper, * lpGameOver;
	FGuiLabel * lpScore, * lpTime;
    
    void SetStrScore( UI32 iScore );
    void SetTime0( UI32 iTime0 );
    void SetTime1( UI32 iTime1 );
    
public:
    FInterface( FGuiScene * lpScene );
    ~FInterface();
    
    void SetScore( UI32 iScore );
    void SetGameTime( F32 fGameTime );
    
    void Update( F32 fDTime );
    void ShowGameOver( bool bShow ); 

	DEFINE_OBJ_DERIVERED( FInterface );
};

DEFINE_OBJ_NAME( FInterface );

#endif
