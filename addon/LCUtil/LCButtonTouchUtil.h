/*
 *  LCButtonTouchUtil.h
 *  Collins_PictureCard_iphone
 *
 *  Created by 태호 이 on 11. 8. 22..
 *  Copyright 2011 Viscuit Creative. All rights reserved.
 *
 */

#ifndef __LC_BUTTON_TOUCH_UTIL_H__
#define __LC_BUTTON_TOUCH_UTIL_H__

#include "cocos2d.h"

USING_NS_CC;

/**
 @brief Event for buttons when touched.
 */
class LCButtonTouchUtil
{
protected:
	CCArray* m_pTouchArray;
public:
	LCButtonTouchUtil();
	virtual ~LCButtonTouchUtil();
	
	/**
	 @brief Make Singleton Class
	 @return LCButtonTouchUtil* the pointer of singleton object.
	 */
	static LCButtonTouchUtil* sharedButtonTouchUtil();
	
	// Action For SingleButton
	virtual void actionOnTouchBegan(CCSprite* sprite, CCSet* touches, const char* effectPath);
	virtual void actionOnTouchMoved(CCSet* touches);
	virtual void actionOnTouchEnded(CCSet* touches);
};


enum LCTouchState {
	LCTouchStateBegan,
	LCTouchStateMoved,
	LCTouchStateEnded,
	LCTouchStateCanceled
};

/**
 @brief storing pointers and states for button events
 
 버튼의 이벤트에 필요한 포인터들과 상태들을 저장한다.
 특정 터치에 관해 계산이 필요한 CCNode들의 포인터와 터치 상태를 기억한다.
 */
class LCTouchEvent: public CCObject
{
protected:
public:
	LCTouchEvent();
	virtual ~LCTouchEvent();
	
	/**
	 @brief initialize class
	 @param CCTouch* the CCTouch* related this class.
	 @return true success initializing.
	 @return false failed initializing.
	 */
	virtual bool init(CCTouch* pTouch);
	
	// getters and setters
	CC_PROPERTY(LCTouchState, m_iTouchState, TouchState);
	CC_PROPERTY(CCTouch*, m_pTouch, Touch);
	CC_PROPERTY(CCArray*, m_pNodeArray, NodeArray);
	CC_PROPERTY(CCArray*, m_pEffectArray, EffectArray);
	CC_PROPERTY(CCSprite*, m_pSelectedSprite, SelectedSprite);
	CC_PROPERTY(ccColor3B, m_tOriginalColor, OriginalColor);
};

#endif // __LC_BUTTON_TOUCH_UTIL_H__