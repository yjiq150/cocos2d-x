/*
 *  TimeBar.h
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 10..
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __TimeBar_H__
#define __TimeBar_H__

#include "cocos2d.h"

class TimeBar;

class TimeBarDelegate 
{
public:
	virtual void didTimeUp(TimeBar* aTimeBar){return;} // 시간 만료 표시
	virtual void didTimeWarningUp(TimeBar* aTimeBar){return;} // 시간 만료전에 경고시점 알리기
};

class TimeBar : public CCNode
{
public:
	TimeBar();
	~TimeBar();
	virtual bool initWithSize(CCSize aSize);

	void addProgress(float changeAmount);
	CC_PROPERTY(float, progress,Progress);

	bool isCompleted();

	void start();
	void end();
	void timeUpdate(ccTime dt);
	
protected:
	CC_SYNTHESIZE(ccColor3B, startColor,StartColor);
	CC_SYNTHESIZE(ccColor3B, endColor,endColor);
	
	CC_SYNTHESIZE(TimeBarDelegate*, delegate,Delegate);
	CC_SYNTHESIZE(float, maxTime, MaxTime);
	CC_SYNTHESIZE(float, warningTime, WarningTime); // 경고가 날아가는 시간 - 경고시간을 최대값보다 크게잡으면 Disable시킨 효과
	CC_SYNTHESIZE(bool, isTimeWarned, IsTimeWarned); // 이전에 경고를 날렸었는지 설정
	
	void draw();
	CCSize size;
	float progressWidth;
	float dataMax;
	
};

#endif
