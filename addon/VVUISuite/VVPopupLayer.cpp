/*
 *  VVPopupLayer.cpp
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 10..
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */

#include "VVPopupLayer.h"
#include "VVActionSet.h"


USING_NS_CC;
using namespace CocosDenshion;

VVPopupLayer::VVPopupLayer()
{
	isTouchRectSet = false;
}
bool VVPopupLayer::init()
{
	if ( !CCSymbol::init() )
		return false;

	winSize = CCDirector::sharedDirector()->getWinSizeInPixels();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if( winSize.width == 1280 && (winSize.height == 800 || winSize.height == 720) )
        winSize.height = 752;
#endif		
	//화면 두배의 넓은면적 커버
	overlayLayer = CCLayerColor::layerWithColorWidthHeight(ccc4(0, 0, 0, 100),winSize.width*2,winSize.height*2);
	overlayLayer->setPosition(ccp(-winSize.width,-winSize.height));

	this->addChild(overlayLayer);
	
	delegate = NULL;
	
	
	return true;
}

void VVPopupLayer::setOverlayColor(ccColor3B col)
{
	overlayLayer->setColor(col);
}
ccColor3B VVPopupLayer::getOverlayColor()
{
	return overlayLayer->getColor();
}
GLubyte VVPopupLayer::getOverlayOpacity()
{
	return overlayLayer->getOpacity();
}
void VVPopupLayer::setOverlayOpacity(GLubyte opa)
{
	overlayLayer->setOpacity(opa);
}

void VVPopupLayer::onEnter()
{
	CCSymbol::onEnter();
	CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this,VVTouchPriorityPopupLayer,true);
	
	this->willPopupOpen();
}


void VVPopupLayer::showAt(CCPoint aPos)
{
    CCDirector::sharedDirector()->getRunningScene()->addChild(this, 99999);
    setPosition(aPos);
}

void VVPopupLayer::show()
{
    showAt(ccp(winSize.width/2, winSize.height/2));
}

void VVPopupLayer::willPopupOpen()
{
	this->setOpacity(0);
	runAction(CCActionTween::actionWithDuration(0.2,"opacity",0,255));
	runAction(VVActionSet::getAction(VVActionSet_BOUNCE));
}
void VVPopupLayer::willPopupClose()
{
	CCFiniteTimeAction* fadeOut = CCSequence::actions(CCActionTween::actionWithDuration(0.3,"opacity",255,0), CCCallFunc::actionWithTarget(this, callfunc_selector(VVPopupLayer::onFadeOutCompleted)),NULL);
	this->runAction(fadeOut);
}

void VVPopupLayer::close()
{
    willPopupClose();
}

// CCLayer의 CCAssert구문 회피용 더미함수
bool VVPopupLayer::ccTouchBegan(CCTouch* touch, CCEvent*event)
{
	return false;
}

// targeted multitouch를 입력받아서 다른 노드로 터치 전파를 막은 후에
// 터치가 전달되길 원하는 tableView의 터치함수를 직접 실행한다.
bool VVPopupLayer::ccTargetedTouchesBegan(CCSet* touches, CCEvent*event)
{
	CCTouch* touch = (CCTouch*)touches->anyObject();
	
	// 터치렉트가 설정된 경우에만 터치 범위체크 하고
	// 일반적인 경우에는 무조건 터치 먹음
	if (isTouchRectSet)
	{
		if ( CCRect::CCRectContainsPoint(touchRect,this->convertTouchToNodeSpace(touch)) ) 
		{
			return true;
		}
		else 
			return false;	
	}
	else 
		return true;

}
	

void VVPopupLayer::ccTargetedTouchesMoved(CCSet *touches, CCEvent *event)
{
}
void VVPopupLayer::ccTargetedTouchesEnded(CCSet *touches, CCEvent *event)
{
}
void VVPopupLayer::ccTargetedTouchesCancelled(CCSet *touches, CCEvent *event) 
{
}

void VVPopupLayer::onFadeOutCompleted()
{
	if (delegate) 
		delegate->didPopupClose(this);		

	removeFromParentAndCleanup(true);
}

void VVPopupLayer::onExit()
{
	// 터치디스패처도 현재 객체의 retain카운트를 올리기때문에 remove해줘야 파괴자 호출된다.
	CCTouchDispatcher::sharedDispatcher()->removeDelegate(this);
	CCSymbol::onExit();
}
VVPopupLayer::~VVPopupLayer()
{
}
