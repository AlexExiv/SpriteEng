//
//  FGame.h
//  Amazing Balls
//
//  Created by Верун on 10.06.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#ifndef Amazing_Balls_FGame_h
#define Amazing_Balls_FGame_h

#include "..\types.h"
#include "FGame.h"
#include "FGameDef.h"
#include "..\Core\FString.h"



class FArithWorld;
class FScene;
class FDrawSeq;
class FInterface;


class FArithGame : public FGame
{
	enum
	{
		MAX_GAME_OBJECT = 2048
	};

    struct FClick
    {
        F32 x, y;
        I32 iPush;
    };
    
    FArithWorld * lpWorld;
    FInterface * lpInterface;
	FDrawSeq * lpWorldSeq, * lpGuiSeq;
	FScene * lpWorldScene, * lpGuiScene;
 
    UI32 iScore;
    UI32 iGameTime;
    FRect rFrustum;
    FClick cClickSeq[MAX_CLICK];
    
public:
    FArithGame( );
    ~FArithGame();
    
    void Update( F32 fDTime );
    void Draw();
    void Save( const FString & sFileName );
    void Load( const FString & sFileName );
    
	void ClickDown( const FVector2F & vPos, UI32 iIndex );
	void ClickUp( const FVector2F & vPos, UI32 iIndex );
	void ClickMove( const FVector2F & vPos, UI32 iIndex );
      
    UI32 GetScore()const;
    UI32 GetGameTime()const;
	FScene * GetWorldScene()const;
	FScene * GetGuiScene()const;
    
    void AddScore( I32 iScore );
    void SetGameTime( F32 fGameTime );
    void ResetScore();
    void ShowGameOver( bool bShow );
    void NewGame();

	static void InitMeta();
};

#endif
