/*
 *  CCActionTween.cpp
 *  Cocos2d_X_BasicTest
 *
 *  Created by Jaeyun, Oh on 11. 6. 21..
 *  Copyright 2011 비스킷크리에이티브. All rights reserved.
 *
 */

#include "CCActionTween.h"

CCActionTween* CCActionTween::actionWithDuration(ccTime duration,const char* key,float from,float to)
{
	CCActionTween* p_ActionTween = new CCActionTween();
	if(!p_ActionTween->initWithDuration(duration,key,from,to))
		p_ActionTween = NULL;
	p_ActionTween->autorelease();
	
	return p_ActionTween;
}

bool CCActionTween::initWithDuration(ccTime duration,const char* key,float from,float to)
{
	if(CCActionInterval::initWithDuration(duration))
	{
		key_ = key;
		to_ = to;
		from_ = from;
		
		if(key_ == "scaleX")
			condition = kCond_ScaleX;
		else if(key_ == "scaleY")
			condition = kCond_ScaleY;
		else if(key_ == "scale")
			condition = kCond_Scale;
		else if(key_ == "opacity")
			condition = kCond_Opacity;
	
		
		return true;
	}
	
	return false;
}

void CCActionTween::startWithTarget(CCNode* target)
{
	CCActionInterval::startWithTarget(target);
	
	delta_ = to_ - from_;
}

void CCActionTween::update(ccTime dt)
{
	if(condition == kCond_ScaleX)
		m_pTarget->setScaleX(to_ - delta_ * (1 - dt));
	else if(condition == kCond_ScaleY)
		m_pTarget->setScaleY(to_ - delta_ * (1 - dt));
	else if(condition == kCond_Scale)
		m_pTarget->setScale(to_ - delta_ * (1 - dt));
	else if(condition == kCond_Opacity)
		m_pTarget->setOpacity(to_ - delta_ * (1 - dt));

}

CCActionInterval* CCActionTween::reverse()
{
	return CCActionTween::actionWithDuration(m_fDuration,key_.c_str(),to_,from_);
}

CCActionTween::~CCActionTween()
{
}