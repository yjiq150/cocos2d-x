/*
 *  VVActionSet.cpp
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 10..
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */

#include "VVActionSet.h"

USING_NS_CC;

#define ON_TOUCH_SCALE 1.1

// Deprecated : use IS_TOUCH_INSIDE 매크로
bool VVActionSet::isTouchInside(CCNode *target, CCPoint touchLocation)
{
	return CCRect::CCRectContainsPoint(target->getRect(), target->getParent()->convertToNodeSpace(touchLocation));
}

bool VVActionSet::scaleUpOnTouchBegan(CCNode *target, CCPoint touchLocation)
{
		// 좌표계를 convert할때 비교하고싶은 노드의 부모좌표계로 컨버젼 할것!
	if (target == NULL) 
		return false;
		
	if( CCRect::CCRectContainsPoint(target->getRect(), target->getParent()->convertToNodeSpace(touchLocation)) )
	{
		target->setScale(ON_TOUCH_SCALE);
		return true;
	}
	else 
		return false;
	

}
void VVActionSet::scaleDownOnMoveOut(CCNode *target, CCPoint touchLocation)
{
	if (target == NULL) 
		return;
	
	if( !CCRect::CCRectContainsPoint(target->getRect(), target->getParent()->convertToNodeSpace(touchLocation)) )
		target->setScale(1);
}
bool VVActionSet::scaleDownOnTouchEnded(CCNode *target, CCPoint touchLocation)
{
	if (target == NULL) 
		return false;
	
	if ( CCRect::CCRectContainsPoint(target->getRect(), target->getParent()->convertToNodeSpace(touchLocation)) && target->getScale() > 1) 
	{
		target->setScale(1);
		return true;
	}
	else 
		return false;
	

}

CCAction* VVActionSet::getAction(VVActionSetIndex index)
{
	CCAction* returnAction;
	
	switch (index) 
	{
		case VVActionSet_SCALEUPDOWN_REPEAT:
			returnAction = CCRepeatForever::actionWithAction((CCActionInterval*)
															 CCSequence::actions(CCScaleTo::actionWithDuration(0.2f, 1.2f), 
																				 CCScaleTo::actionWithDuration(0.2f, 1.0f), 
																				 NULL)
															 );
			break;
		case VVActionSet_SHAKE: 
			returnAction = CCSequence::actions(CCScaleTo::actionWithDuration(0.1f, 1.2f),
											   CCRotateTo::actionWithDuration(0.3f, -30), 
											   CCRotateTo::actionWithDuration(0.6f, 30), 
											   CCRotateTo::actionWithDuration(0.3f, 0), 
											   CCScaleTo::actionWithDuration(0.1f, 1.0f), NULL); 
			break;
		case VVActionSet_BOUNCE:
			returnAction = CCSequence::actions(CCScaleTo::actionWithDuration(0.1, 1.2),
											   CCScaleTo::actionWithDuration(0.1, 1.0),
											   NULL);
			break;
		default:
			break;
	}
	
	return returnAction;
}