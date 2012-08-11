/*
 *  CrossMatchLayer.h
 *  HelloWorld
 *
 *  Created by 태호 이 on 11. 6. 16..
 *  Copyright 2011 Viscuit Creative. All rights reserved.
 *
 */

#ifndef __CrossMatch_LAYER_H__
#define __CrossMatch_LAYER_H__

#include "LCCrossMatchGame.h"
#include "cocos2d.h"
#include "TimeBar.h"
#include "CrossMatchCommon.h"

USING_NS_CC;


class CrossMatchLayer : public LCCrossMatchGame, public TimeBarDelegate
{
protected:
	
	CCLabelBMFont* m_pScoreLabel;
	int m_iScore;

	// implement LCCrossMatchGame
	virtual void loadIndex(CCArray* wordIdxArray);
	virtual CCLabelBMFont* getTextLabelWithIndex(CCArray* wordIdxArray, int index);
	virtual CCSprite* getImageWithIndex(CCArray* wordIdxArray, int index);	

	virtual void arrangeComponents(CCArray* labelArray, CCArray* imageArray);
	
	// Event Handler
	virtual void didMatchWord(CCSprite* matchedSprite, CCArray* wordIdxArray, int index);
	virtual ccTime delayTimeOfChangeWord(CCArray* wordIdxArray, int index);
	
	virtual void didMissMatchWord();
	virtual void didFinishedStage();
	virtual void didWordImagedTouched(CCSprite* touchedSprite, CCArray* wordIdxArray, int touchedIdx);
	
	
	virtual void didGameFinished();

	//타이머 핸들러
	virtual void didTimeUp(TimeBar* aTimeBar);
	virtual void didTimeWarningUp(TimeBar* aTimeBar);
	TimeBar* timer;
	
	CCSize winSize;
	void onPointLabelFadeOut(CCNode* node);
	void onTimeUpAniDone(CCNode* node);
	void onAniDone();
	bool isActionOngoing;

	CCSequence* aniSeqAction;
	
	map<int,string> soundPaths;
	map<int,CCAnimate*> aniActions;

	CrossMatchScoreReport report;
	
	//////////////////////////
	// 상속해서 사용할 설정 변수들
	//////////////////////////

	CC_SYNTHESIZE(CCPoint, imageAnchor,ImageAnchor);
	CC_SYNTHESIZE(float, imageOriginalScale,ImageOriginalScale);
	float imageAniScale;
	
	// DB가 달라서 랭귀지 코드 불러오는법 달라서 만든 변수, 추후 DB통합 후 제거
	string languageCode;
	// 폰용은 150, 패드용은 350 설정
	string imageSizeStr;
	ccColor3B wordColor;
	string wordFont;
	
	
	int wordHeightInterval;
	int wordStartHeight;
	int wordPosX;
	int imagePosX;
	
	
	CCPoint warningLabelPosition;
	
	CCPoint pointLabelPosition;
	CCPoint comboLabelPosition;
	float labelScaleTo;
	int combo;
	int multiplier;
	
public:
	CrossMatchLayer();
	virtual ~CrossMatchLayer();
	virtual void onEnter();
	virtual void onExit();
	/**
	 @brief	Implement init code here.
	 @return true    Initialize success.	
	 @return false   Initialize failed.
	 */
	bool init();
	
	// 게임 초기화 완료 후 실행시점에 호출
	virtual void runGame();
};

#endif // __CrossMatch_LAYER_H__
