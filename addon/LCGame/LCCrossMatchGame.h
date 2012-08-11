/*
 *  LCCrossMatchGame.h
 *  Collins_Dictionary_iphone
 *
 *  Created by 태호 이 on 11. 9. 2..
 *  Copyright 2011 Viscuit Creative. All rights reserved.
 *
 */

#ifndef __LC_CrossMatch_Game_H__
#define __LC_CrossMatch_Game_H__

#include "cocos2d.h"

USING_NS_CC;

/**
 @brief	Cross & Match Game Layer
 
 단어와 그림을 선으로 연결해서 매칭하는 게임.
 
 */
class LCCrossMatchGame : public CCLayer
{
protected:
	CCArray *m_pImageArray;
	CCArray *m_pTextArray;
	CCArray *m_pLoadedSprites; 
	CCArray *m_pDotSprites;
	CCArray *m_pFinishedDotSprites;
	CCSprite *m_pTouchedDotSprite;

	CCTouch* singleTouch;
	
	/**
	 @brief	Canvas for Drawing Lines.
	 */
	CCRenderTexture* m_pCanvas;
	
	/**
	 @brief	Contains wordIdx of word that can show(length limit).
	 */
	CCArray* m_pIndexArray;
	int m_iIndex;
	
	/**
	 @brief A Process, Load Sprite and Arrange it.
	 
	 필요한 배열 및 상태값 초기화, 이미지 로딩 및 배치 함수 호출에 관련된 일을 수행한다.
	 */
	void loadSprites();
	
	/**
	 @brief shuffle alphabet sprites randomly.
	 @param CCArray* the array contains CCNode*
	 
	 노드들을 입력받아서 서로의 위치를 임의로 섞어준다.
	 */
	void shuffleNodes(CCArray* nodeArray);
	
	/**
	 @brief load word indexes and add indexArray.
	 @param CCArray* wordIdxArray 게임에서 사용될 단어들의 wordIdx를 저장할 수 있는 변수.
	 
	 게임에서 사용될 단어들의 wordIdx들을 순서대로 indexArray에 집어 넣는다.
	 */
	virtual void loadIndex(CCArray* wordIdxArray) = 0;
	
	/**
	 @brief return word using array and index.
	 @param CCArray* wordIdxArray 게임에서 사용될 단어들의 wordIdx를 저장할 수 있는 변수.
	 @param int index 사용될 단어의 index.
	 @return CCLabelTTF* 현재 차례의 단어 headword 라벨.
	 
	 indexArray와 현재 차례인 index에 해당하는 단어라벨을 찾아 return한다.
	 */
	virtual CCLabelBMFont* getTextLabelWithIndex(CCArray* wordIdxArray, int index) = 0;
	
	/**
	 @brief return image using array and index.
	 @param CCArray* wordIdxArray 게임에서 사용될 단어들의 wordIdx를 저장할 수 있는 변수.
	 @param int index 사용될 단어의 index.
	 @return CCSprite* 현재 차례의 단어의 이미지
	 
	 indexArray와 현재 차례인 index에 해당하는 이미지를 찾아서 return한다.
	 */
	virtual CCSprite* getImageWithIndex(CCArray* wordIdxArray, int index) = 0;
	
	/**
	 @brief return dot image.
	 @return CCSprite* dot image.
	 
	 게임의 점 이미지를 돌려준다.
	 */
	virtual CCSprite* getDotSprite() = 0;
	
	/**
	 @brief return brush image.
	 @return CCSprite* brush image.
	 
	 게임의 브러쉬 이미지를 돌려준다.
	 */
	virtual CCSprite* getBrushSprite();
	
	/**
	 @brief return distance between dot and textLabel.
	 @return float distance between dot and textLabel.
	 
	 textLabel 기준의 점의 위치. 양수는 오른쪽 음수는 왼쪽에 배치된다.
	 */
	virtual float getDotDistanceWithTextLabel() = 0;
	
	/**
	 @brief return distance between dot and image.
	 @return distance between dot and image.
	 
	 image 기준의 점의 위치. 양수는 오른쪽 음수는 왼쪽에 배치된다.
	 */
	virtual float getDotDistanceWithImage() = 0;
	
	/**
	 @brief return number of items.
	 @return number of items.
	 
	 한번에 로딩할 단어의 갯수를 돌려준다.
	 */
	virtual int getMaxNumberOfItems() = 0;
	
	/**
	 @brief arrange components
	 @param CCArray* labelArray the array of CCLabelTTF* textLabel.
	 @param CCArray* imageArray the array of CCSprite* image.
	 
	 textLabel과 image들을 배치한다.
	 */
	virtual void arrangeComponents(CCArray* labelArray, CCArray* imageArray) = 0;
	
	/**
	 @brief drawLine from startPoint to endPoint
	 @param CCPoint startPoint the point of start of line.
	 @param CCPoint endPoint the point of end of line.
	 
	 시작점에서부터 끝점으로 선을 긋는다.
	 선에대한 세팅도 가능하기 때문에, 선 설정을 바꾸기 위해서 이 함수를 오버로딩한다.
	 */
	virtual void drawLine(CCPoint startPoint, CCPoint endPoint);
	
	// Event Handler
	virtual void didMatchWord();
	virtual void didMissMatchWord();
	virtual void didFinishedStage();
	
	// Event Handler (현재 사용되고 있는 단어를 사용하기 위한 기능 추가)
	virtual void didMatchWord(CCSprite* matchedSprite, CCArray* wordIdxArray, int index);
	virtual void didWordImagedTouched(CCSprite* touchedSprite, CCArray* wordIdxArray, int touchedIdx);
	virtual ccTime delayTimeOfChangeWord(CCArray* wordIdxArray, int index);
	virtual void didFinishedGame();
	
	// delay 액션 핸들을 위한 함수
	void didDelayedChangeWord();
	
	// animation이 끝날때 해제하는 부분 (현재는 FadeOut 애니메이션)
	void didSpriteAnimated(CCNode* node);
	
	void drawMatchedLines();
	
	
	CCPoint prevTouchLocation;
	
	// 안드로이드용 터치체크 코드
	int moveAmount;
		CCPoint startTouchLocation;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )	
	bool isTouchMoveCanBeCalled;
	void touchMoveSuppressor(ccTime dt);
	bool isStartTouch; // 시작 터치를 체크해서 startTouchLoc값을 저장해둔다
#endif

    virtual void didHaltedDevice();
	
public:
	LCCrossMatchGame();
	virtual ~LCCrossMatchGame();
	
	/**
	 @brief	Implement init code here.
	 @return true    Initialize success.	
	 @return false   Initialize failed.
	 */
	virtual bool init();
	
	// implement CCTouchDelegate
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
 	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
 	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
};

#endif // __LC_CrossMatch_Game_H__
