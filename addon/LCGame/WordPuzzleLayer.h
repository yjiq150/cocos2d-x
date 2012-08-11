/*
 *  WordPuzzleLayer.h
 *  Collins_Dictionary_iphone
 *
 *  Created by 태호 이 on 11. 9. 2..
 *  Copyright 2011 Viscuit Creative. All rights reserved.
 *
 */

#ifndef __WordPuzzle_LAYER_H__
#define __WordPuzzle_LAYER_H__


#include "cocos2d.h"
#include "SimpleAudioEngine.h"

#include "LCWordPuzzleGame.h"
#include "TimeBar.h"
#include "VVPopupLayer.h"
#include "WordPuzzleCommon.h"

USING_NS_CC;

typedef enum
{
	WordPuzzleModeFavorite,
	WordPuzzleModeRandom
} WordPuzzleMode;


class WordPuzzleLayer : public LCWordPuzzleGame, public TimeBarDelegate, public VVPopupLayerDelegate
{
protected:
	CCSprite *pScoreLabelSprite;
	CCLabelAtlas* m_pScoreLabel;
	int m_iScore;
	
	
	virtual void loadIndex(CCArray* wordIdxArray);
	
	// implement LCWordPuzzleGame
	virtual const char* getWordWithIndex(CCArray* wordIdxArray, int index);
	virtual CCSprite* getImageWithIndex(CCArray* wordIdxArray, int index);
	virtual CCSprite* getBoxImage();
	virtual LCWordPuzzleAlphabet* getAlphabetImage(char alphabet);
	virtual void arrangeComponents(const char* word, CCArray* boxArray, CCArray* alphabetArray);
	
	// Event Handler	
		
	virtual void didCorrectCharacterSelected(CCArray* wordIdxArray, int index);
	virtual void didCorrectCharacterAnimated(CCArray* wordIdxArray, int index);
	virtual void didIncorrectCharacterSelected(CCArray* wordIdxArray, int index);
    virtual void didIncorrectCharacterAnimated(CCArray* wordIdxArray, int index);
    virtual void didFinishedWord(CCArray* wordIdxArray, int index);
	ccTime delayTimeOfChangeWord(CCArray* wordIdxArray, int index);
	void didDelayedChangeWord();
	
	// added by YoungJae Kwon
	virtual void didWordImageTouched(CCSprite* touchedSprite);
	virtual void didIndexArrayReset();

	// 스코어보드 팝업 처리
	virtual void didPopupClose(VVPopupLayer* aPopup);

	// 단어 게임문제 다풀어서 끝난것 처리
	virtual void didGameFinished();
	
	// added by YoungJae Kwon
	CCSize winSize;
	CCSprite* sprImage;
	string soundPath;
	CCAnimate* aniAction;
	CCSequence* aniSeqAction;

	bool isActionOngoing;
	bool isAllowingNext;
	
	CCLabelTTF* progressCurLabel;
	CCLabelTTF* progressTotalLabel;
	
	// action callbacks
	virtual void onAniDone();
	void onPointLabelFadeOut(CCNode* node);
	void onTimeUpAniDone(CCNode* node);	
	
	void updateProgressLabel();
	void delayMenuTouch(ccTime dt);
	void loadNextWord();
	
	//타이머 핸들러
	virtual void didTimeUp(TimeBar* aTimeBar);
	virtual void didTimeWarningUp(TimeBar* aTimeBar);
	TimeBar* timer;
	
	
	//게임 설정변수들
	CC_SYNTHESIZE(int, maxNumberOfWords, MaxNumberOfWords);
	
	
	// 1,2인용에 따른 기본 위치 셋팅 변수들
	CC_SYNTHESIZE(CCPoint, imagePosition,ImagePosition);
	CC_SYNTHESIZE(CCPoint, imageAnchor,ImageAnchor);
	CC_SYNTHESIZE(float, imageOriginalScale,ImageOriginalScale);
	
	int blockY,alphaY, maxShift;
	CCPoint progressCurLabelPosition;
	CCPoint progressTotalLabelPosition;

	CCPoint warningLabelPosition;
	
	CCPoint pointLabelPosition;
	CCPoint comboLabelPosition;
	float labelScaleTo;
	int combo;
	int multiplier;
	
	WordPuzzleScoreReport report;
	
	
	
public:
	WordPuzzleLayer();
	virtual ~WordPuzzleLayer();
	virtual void onEnter();
	virtual void onExit();
	/**
	 @brief	Implement init code here.
	 @return true    Initialize success.	
	 @return false   Initialize failed.
	 */
	virtual bool init();
	WordPuzzleScoreReport getScoreReport();
	
	void next();
};

#endif // __WordPuzzle_LAYER_H__
