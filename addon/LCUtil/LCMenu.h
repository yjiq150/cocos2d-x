/*
 *  LCMenu.h
 *  CollinsDictionaryHD
 *
 *  Created by 태호 이 on 11/22/11.
 *  Copyright 2011 Viscuit Creative. All rights reserved.
 *
 */


#ifndef __LC_MENU_H__
#define __LC_MENU_H__

#include "cocos2d.h"
#include "LCMenuItem.h"

USING_NS_CC;

class LCMenu : public CCMenu {
public:
	
	static LCMenu* menuWithItems(CCMenuItem* item, ...);
	static LCMenu* menuWithItems(int priority, CCMenuItem* item, ...);
	
	virtual void ccTargetedTouchesMoved(CCSet* touches, CCEvent* event);
	
protected:
	CCMenuItem* itemForTouchGenerous(CCTouch * touch);
};

#endif // __LC_MENU_H__