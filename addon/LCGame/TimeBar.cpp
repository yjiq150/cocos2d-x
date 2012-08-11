/*
 *  TimeBar.cpp
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 10..
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "TimeBar.h"

USING_NS_CC;
TimeBar::TimeBar()
{
	progress = 0.0f;
	isTimeWarned = false;
	delegate = NULL;
	maxTime = 100;
	warningTime = 0;
	startColor = ccc3(255, 161, 68);
	endColor = ccc3(255, 0, 0);
}

bool TimeBar::initWithSize(CCSize aSize)
{
	size = aSize;

	return true;
}

bool TimeBar::isCompleted()
{
	return (progress >= maxTime);
}
// 상대 비율이아닌 절대값 프로그레스
float TimeBar::getProgress()
{
	return progress;
}

// 절대값
void TimeBar::setProgress(float aProgress)
{
	if (aProgress >= maxTime) 
		progress = maxTime;
	else
		progress = aProgress;
		
	progressWidth = progress/maxTime*size.width;
}

// 누적값
void TimeBar::addProgress(float changeAmount)
{
	setProgress(progress+changeAmount);
}

void TimeBar::start()
{
	this->schedule(schedule_selector(TimeBar::timeUpdate),0.5);
}

void TimeBar::end()
{
	this->unschedule(schedule_selector(TimeBar::timeUpdate));
}


void TimeBar::timeUpdate(ccTime dt)
{
	if (progress >= warningTime && !isTimeWarned) 
	{
		progress += dt;
		isTimeWarned = true;
		delegate->didTimeWarningUp(this);
		
	}
	else if (progress >= maxTime) 
	{
		progress = maxTime;
		delegate->didTimeUp(this);
		this->end();
	}
	else
		progress += dt;
	
	progressWidth = progress/maxTime*size.width;
}


void TimeBar::draw()
{
	// draw texture box
    CCPoint offsetPix = ccp(0,0);
    CCPoint vertTotal[4] = 
	{
        ccp(offsetPix.x,			offsetPix.y),			  ccp(offsetPix.x+size.width,	offsetPix.y),
        ccp(offsetPix.x+size.width, offsetPix.y+size.height), ccp(offsetPix.x,				offsetPix.y+size.height)
    };
	//검은 바
	glColor4f(51.0/255.0, 51.0/255.0, 51.0/255.0, 1.0);
    ccFillPoly(vertTotal, 4);

	
	CCPoint vertProgress[4] = 
	{
        ccp(offsetPix.x,			offsetPix.y),			  ccp(offsetPix.x+progressWidth,	offsetPix.y),
        ccp(offsetPix.x+progressWidth, offsetPix.y+size.height), ccp(offsetPix.x,				offsetPix.y+size.height)
    };
	
	//그위에 덮어서 주황색 바
	if (isTimeWarned)
	{
		// 시간경고후 바뀌는 색상
		float ratio = (progress-warningTime) / (maxTime-warningTime); // 진전시간 / x증분
		float r = ((endColor.r - startColor.r)*ratio + startColor.r) / 255.0; 
		float g = ((endColor.g - startColor.g)*ratio + startColor.g) / 255.0; 
		float b = ((endColor.b - startColor.b)*ratio + startColor.b) / 255.0;
		glColor4f(r, g, b, 1.0);
	}
	else
	{
		//  평소색상
		glColor4f(startColor.r/255.0, startColor.g/255.0, startColor.b/255.0, 1.0);
	}
	
    ccFillPoly(vertProgress, 4);
}

TimeBar::~TimeBar()
{
	this->end();
}