/*
 *  WordPuzzleCommon.h
 *  CollinsDictionaryHD
 *
 *  Created by YoungJae Kwon on 11. 11. 3..
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __WordPuzzleCommon_H__
#define __WordPuzzleCommon_H__

#define SCORE_COL ccc3(255, 210, 39)

#define BASE_SCORE 200


#define TIME_PER_WORD 10 // 단어당 풀이시간 몇초?
#define MAX_NUMBER_OF_QUESTION 15 // 게임 한판당 단어 문제 수

#define MAX_LENGTH_OF_WORD 8 // MOD: 귀찮아서 적용안된다..단어 길이 제한



typedef struct 
{
	int score;
	int maxCombo;
	int incorrectTouch; // 잘못 터치한 횟수
	ccTime givenTime; // 원래 총 주어진 시간
	ccTime clearTime; // 스테이지 클리어한 시간
	int timeBonusMultiplier;
	bool isWinner;
	bool isSkipExist;
	
} WordPuzzleScoreReport;

#endif