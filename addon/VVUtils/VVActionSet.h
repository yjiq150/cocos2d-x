/*
 *  VVActionSet.h
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 10..
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */

#ifndef __VVActionSet_H__
#define __VVActionSet_H__

#include "cocos2d.h"

#define ACT_DELAY(dur) CCDelayTime::actionWithDuration(dur)
#define ACT_SCALE(dur,from,to) CCActionTween::actionWithDuration(dur,"scale",from,to)
#define ACT_FADE_IN(dur) CCActionTween::actionWithDuration(dur,"opacity",0,255)
#define ACT_FADE_OUT(dur) CCActionTween::actionWithDuration(dur,"opacity",255,0)

typedef enum
{
	VVActionSet_SCALEUPDOWN_REPEAT,
	VVActionSet_SHAKE,
	VVActionSet_BOUNCE
} VVActionSetIndex;

class VVActionSet
{
public:
	// 해당 터치포인트가 타겟 안에서 눌러졌는지
	static bool isTouchInside(CCNode *target, CCPoint touchLocation);
	
	// 자동으로 터치시 크기 늘려주고 떨어질때 크기 작게해주는 함수 셋
	static bool scaleUpOnTouchBegan(CCNode *target, CCPoint touchLocation);
	static void scaleDownOnMoveOut(CCNode *target, CCPoint touchLocation);
	static bool scaleDownOnTouchEnded(CCNode *target, CCPoint touchLocation);	
	
	// 미리 정의된 액션 리턴
	static CCAction* getAction(VVActionSetIndex index);
protected:

};

#endif
