/*
 *  VVPopupLayer.h
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 10..
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */

#ifndef __VVPopupLayer_H__
#define __VVPopupLayer_H__

#include "cocos2d.h"
#include "CCSymbol.h"
#include "SimpleAudioEngine.h"

class VVPopupLayer;

typedef enum {
	VVTouchPriorityMenu = -128, // Cocos2dx Default value
	VVTouchPriorityPopupLayer = - 129, // 메뉴보다 먼저 터치를 체크해야 하므로 priority하나 높음
	VVTouchPriorityMenuOnPopupLayer = -130 // 팝업레이어 위에붙은 메뉴의 터치 동작위함
} VVTouchPriority;

class VVPopupLayerDelegate
{
public:
	virtual void didPopupClose(VVPopupLayer* aPopup){ return; }
};

class VVPopupLayer : public CCSymbol
{
public:
	VVPopupLayer();
	virtual ~VVPopupLayer();	
	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	
	// Overide for custom actions
	virtual void willPopupOpen();
	virtual void willPopupClose();
		
    // open & close popup
    virtual void show();
    virtual void showAt(CCPoint aPos);
	virtual void close();
    
	bool ccTouchBegan(CCTouch* touch, CCEvent*event);	
	bool ccTargetedTouchesBegan(CCSet* touches, CCEvent*event);
	void ccTargetedTouchesMoved(CCSet *touches, CCEvent *event);
	void ccTargetedTouchesEnded(CCSet *touches, CCEvent *event);
	void ccTargetedTouchesCancelled(CCSet *touches, CCEvent *event); 
	
protected:
	CC_SYNTHESIZE(bool, isTouchRectSet,IsTouchRectSet);
	CC_SYNTHESIZE(CCRect,touchRect, TouchRect);
	CCSize winSize;
    
	CCLayerColor* overlayLayer;
	CC_PROPERTY(ccColor3B, overlayColor,OverlayColor);
	CC_PROPERTY(GLubyte, overlayOpacity,OverlayOpacity);
	
	CC_SYNTHESIZE(VVPopupLayerDelegate *, delegate, Delegate);
	virtual void onFadeOutCompleted();
};

#endif
