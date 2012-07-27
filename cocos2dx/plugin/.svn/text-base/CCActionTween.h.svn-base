/*
 *  CCActionTween.h
 *  Cocos2d_X_BasicTest
 *
 *  Created by Jaeyun, Oh on 11. 6. 21..
 *  Copyright 2011 비스킷크리에이티브. All rights reserved.
 *
 */

#ifndef __CCActionTween_H__
#define __CCActionTween_H__

#include "cocos2d.h"
#include <iostream>

using namespace std;

using namespace cocos2d;

class CCActionTween : public CCActionInterval
{
private:
	enum TweenProperties 
	{
		kCond_ScaleX,
		kCond_ScaleY,
		kCond_Scale,
		kCond_Opacity
	};
	string key_;
	char condition;
	float from_,to_;
	float delta_;
public:
	~CCActionTween();
	virtual void startWithTarget(CCNode* target);
	virtual void update(ccTime dt);
	virtual cocos2d::CCActionInterval* reverse();
public:
	bool initWithDuration(ccTime duration,const char* key ,float from,float to);
	static CCActionTween* actionWithDuration(ccTime duration,const char* key ,float from,float to);
};

#endif //  __CCActionTween_H__