/*
 *  CrossMatchCommon.h
 *  CollinsDictionaryHD
 *
 *  Created by YoungJae Kwon on 11. 11. 3..
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __CrossMatchCommon_H__
#define __CrossMatchCommon_H__

#define CM_SCORE_COL ccc3(255, 154, 56)

#define CM_BASE_SCORE 200

#define CM_TOTAL_GIVEN_TIME 180 // 한게임당 3분
#define CM_MAX_NUMBER_OF_QUESTION 360 // 게임 한판당 문제수, 120문제 로딩해서(시간내에 다 맞출 가능성 없도록 만든다)

typedef struct 
{
	int score;
	int maxCombo;
	int incorrectTouch; // 잘못 터치한 횟수
	ccTime givenTime; // 원래 총 주어진 시간
	
} CrossMatchScoreReport;

#endif