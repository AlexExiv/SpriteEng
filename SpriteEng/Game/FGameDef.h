//
//  FGameDef.h
//  Amazing Balls
//
//  Created by Верун on 18.06.14.
//  Copyright (c) 2014 Верун. All rights reserved.
//

#ifndef Amazing_Balls_FGameDef_h
#define Amazing_Balls_FGameDef_h

#include "..\Core\FString.h"

//////имена файлов данных//////////
extern const FString sDeffFont;
extern const FString sScoreFont;
extern const FString sInterfaceBack;
extern const FString sGameOver;
extern const FString sCellBack;
extern const FString sPartName;
extern const FString sEndMoveSound;
extern const FString sDestroySound;
extern const FString sTimeBinusSound;
extern const FString sNotRightSound;
extern const FString sAttentionPrepare;
extern const FString sAttentionCounter;
extern const FString sAttentionHoldon;
extern const FString sAttentionAttackSoon;



#define DOUBLE_COMMA_TIME 30.f
#define SHOW_SCORE_TIME 20.f
#define SCORE_TIME 20.f
#define HIDE_SCORE_TIME 20.f
#define NEW_DIGIT_TIME 110.f
#define TIME_BONUS_TIME 360.f
#define TIME_BONUS_DIGIT_COUNT 6
#define MAX_DIGIT_COUNT 99
#define MIN_DIGIT_COUNT 19


#define SHOW_BONUS_TIME 60.f
#define BONUS_TIME 70.f
#define HIDE_BONUS_TIME 35.f

#define ATTENTION_SHOW_SOON_TIME 90.f
#define ATTENTION_SHOW_PREPARE_TIME 90.f
#define ATTENTION_SHOW_COUNTER_TIME 200.f
#define ATTENTION_SHOW_HOLDON_TIME 90.f
#define ATTENTION_EVENT_MIN_TIME 400.f
#define ATTENTION_EVENT_TIME_DIVIATION 460
#define ATTENTION_ATTACK_WAIT_TIME 3600.f
#define ATTENTION_NEW_DIGIT_TIME 20.f
#define ATTENTION_SHOW_DELAY_TIME 45.f
#define ATTENTION_NEXT_ATTACK_MIN_TIME 1200.f
#define ATTENTION_NEXT_ATTACK_TIME_DIV 600

#define NUMBER_SIZE 32.f
#define NUMBER_IN_LINE 9

#define MAX_CLICK 5


extern const FString sDigitStayAnim;
extern const FString sDigitSelectAnim;
extern const FString sDigitDyingAnim;
extern const FString sHiddenDigitAnim;
extern const FString sParticleAnim;
extern const FString sCounterAnim;

#define HIDDEN_DIGIT_SHOW_TIME 300.f
#define HIDDEN_DIGIT_HIDDING_TIME 70.f
#define HIDDEN_DIGIT_WAIT_TIME 1800.f
#define HIDDEN_DIGIT_PROBABILITY 35

#define DIGIT_TIME_TO_DESTROY 15.f

#endif
