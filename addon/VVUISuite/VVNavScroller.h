/*
 *  VVNavScroller.h
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 10..
 *  Copyright 2011 Viscuit Creative Inc. All rights reserved.
 *
 */

#ifndef __VVNavScroller_H__
#define __VVNavScroller_H__

#include "cocos2d.h"
#include "VVScrollView.h"
#include "CCSymbol.h"
class CCAsyncObjectForVVNavScroller : public CCAsyncObject
{
public:
	CCAsyncObjectForVVNavScroller();
	virtual ~CCAsyncObjectForVVNavScroller();
	CCPoint sprPosition;
	int iconIdx;
};

class VVNavScroller;

class VVNavScrollerDelegate
{
public:
	virtual void onNavIconTouched(VVNavScroller* scroller, unsigned int touchedIconIdx){return;};
};

class VVNavScroller : public VVScrollView, public VVScrollViewDelegate
{
public:
	VVNavScroller();
	bool initWithViewSizeAndDir(CCSize size, VVScrollViewDirection dir);

	virtual void reloadIcons(vector<string>* navName);
	virtual void loadIcons(vector<string>* navName);

	virtual void reloadIcons(int navIdx);
	virtual void loadIcons(int navIdx);

	// callbacks
	void imageLoadFinished(CCObject *aObject);
	
	// delegations
	void ccTouchesBegan(CCSet* touches, CCEvent* event);
	void ccTouchesMoved(CCSet* touches, CCEvent* event);
	void ccTouchesEnded(CCSet* touches, CCEvent* event);
	void scrollViewDidScroll(VVScrollView *aView);
	
	
	CC_SYNTHESIZE(CCSprite*, arrowPlus,PlusArrow)
	CC_SYNTHESIZE(CCSprite*, arrowMinus,MinusArrow);
	CC_SYNTHESIZE(VVNavScrollerDelegate*,navDelegate,NavDelegate);

	CC_PROPERTY(unsigned int, selected,Selected); 
	
protected:
	// 이상한곳에서 자꾸 VVNavScroller로 터치넘어오는 버그 체크하기위해 변수 하나 더둠
	bool isTouchOnNavScroller;
	CCSize winSize;
	CCSprite* touchedIcon;
	bool isMoved;
	
	// 네비게이션 스크롤러에 추가될 아이콘의 숫자
	int numberOfNavIcons;	
	
	CCSymbol* wrapper;
};

#endif
