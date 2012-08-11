/*
 *  VVAlertView.cpp
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 10..
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "VVAlertView.h"
#include "VVActionSet.h"
#include "CCMenuItemDynamic.h"
#include "CCScale9Sprite.h"

USING_NS_CC;

#define BTN_INDEX_OK        1
#define BTN_INDEX_CANCEL    0

#define FONT_NAME "Arial"
#define FONT_SIZE_TITLE 30
#define FONT_SIZE_OTHER 30

#define MAX_WIDTH 350

#define BORDER_PADDING 20
#define MESSAGE_PADDING 20

#define BTN_SIZE CCSizeMake(120,70)
#define BETWEEN_BTN_PADDING 10

VVAlertView::VVAlertView()
{
    CCAssert(0, "Use other constructor");
}

VVAlertView::VVAlertView(const char* aAlertBG_name, const CCRect& aAlert9patch_rect, const char* aBtnBG_name, const CCRect& aBtn9patch_rect)
{
    totalHeight = 0;
    centerPadding = 0;
    
    alertBG_name = aAlertBG_name;
    alert9patch_rect = aAlert9patch_rect;
    
    btnBG_name = aBtnBG_name;
    btn9patch_rect = aBtn9patch_rect;
    
    // set Default value
    fontName = FONT_NAME;
    titleFontSize = FONT_SIZE_TITLE;
    messageFontSize = FONT_SIZE_OTHER;
    maxWidth = MAX_WIDTH;
    borderPadding = BORDER_PADDING;
    messagePadding = MESSAGE_PADDING;
    
    btnSize = BTN_SIZE;
    betweenBtnPadding = BETWEEN_BTN_PADDING;
}



void VVAlertView::preInit(const char* aFontName, int aTitleFontSize, int aMessageFontSize,
                          float aMaxWidth, float aBorderPadding, float aMessagePadding,
                          const CCSize& aBtnSize, float aBetweenBtnPadding )
{
    fontName = aFontName;
    titleFontSize = aTitleFontSize;
    messageFontSize = aMessageFontSize;
    maxWidth = aMaxWidth;
    borderPadding = aBorderPadding;
    messagePadding = aMessagePadding;

    btnSize = aBtnSize;
    betweenBtnPadding = aBorderPadding;
}


bool VVAlertView::initWithTitle(const char* aTitle, const char* aMessage, VVAlertViewDelegate* aDelegate, const char* aCancelBtnTitle)
{
    return initWithTitle( aTitle, aMessage, aDelegate, aCancelBtnTitle, NULL);
}

bool VVAlertView::initWithTitle(const char* aTitle, const char* aMessage, VVAlertViewDelegate* aDelegate, const char* aCancelBtnTitle, const char* aOtherBtnTitle, float aCenterPadding)
{
    centerPadding = aCenterPadding;
    return initWithTitle( aTitle, aMessage, aDelegate, aCancelBtnTitle, aOtherBtnTitle);
}

bool VVAlertView::initWithTitle(const char* aTitle, const char* aMessage, VVAlertViewDelegate* aDelegate, const char* aCancelBtnTitle, const char* aOtherBtnTitle)
{
    if ( !VVPopupLayer::init() )
		return false;
    
    delegate = aDelegate;
    
    overlayLayer->removeFromParentAndCleanup(true);
    
    CCLabelTTF *titleLabel = NULL;
    
    if( aTitle != NULL )
    {
        titleLabel = CCLabelTTF ::labelWithString(aTitle ,fontName.c_str(), titleFontSize);
    }
    
    CCScale9Sprite *bg = CCScale9Sprite::scale9SpriteWithFile(alertBG_name.c_str(), alert9patch_rect);
    
    
    CCLabelTTF *msgLabel = CCLabelTTF::labelWithString(aMessage, CCSizeMake(maxWidth-borderPadding*4, 0), CCTextAlignmentCenter, fontName.c_str(), messageFontSize);
    
    
    CCMenuItemSprite* btnCancel = CCMenuItemDynamic::dynamicItemWithTTFLabel(aCancelBtnTitle, fontName.c_str(), messageFontSize, btnSize, btnBG_name.c_str(), NULL, this,
                                                                             menu_selector(VVAlertView::onBtnCancel), 
                                                                             btn9patch_rect, 255);
    CCMenu *menu;
    if (aOtherBtnTitle == NULL) 
    {
        menu = CCMenu::menuWithItems(btnCancel, NULL);
    }
    else
    {
        CCMenuItemSprite* btnOk = CCMenuItemDynamic::dynamicItemWithTTFLabel(aOtherBtnTitle, fontName.c_str(), messageFontSize, btnSize, btnBG_name.c_str(), NULL, this,
                                                                                 menu_selector(VVAlertView::onBtnOk), 
                                                                                 btn9patch_rect, 255);

        menu = CCMenu::menuWithItems(btnCancel, btnOk, NULL);
        menu->alignItemsHorizontallyWithPadding(betweenBtnPadding);
    }
     
    
    menu->setTouchPriority(VVTouchPriorityMenuOnPopupLayer);
    
    addChild(bg);
    if (titleLabel) 
        addChild(titleLabel);            

    addChild(msgLabel);    
    addChild(menu);
    
    
    if (titleLabel)     
        totalHeight += titleLabel->getContentSize().height;    
    
    totalHeight += msgLabel->getContentSize().height;    
    totalHeight += btnCancel->getContentSize().height;
    totalHeight += centerPadding;
    
    totalHeight += messagePadding*2;
    totalHeight += borderPadding*2;
    
    float halfHeight = totalHeight/2;
    
    if (titleLabel) 
    {
        titleLabel  ->setPosition(ccp(0, halfHeight - borderPadding - titleLabel->getContentSize().height/2)); // -15 : top padding
        msgLabel    ->setPosition(ccp(0, halfHeight - borderPadding - titleLabel->getContentSize().height - messagePadding - msgLabel->getContentSize().height/2 ));
    }
    else
    {
        msgLabel    ->setPosition(ccp(0, halfHeight - borderPadding - msgLabel->getContentSize().height/2 ));
    }
    menu        ->setPosition(ccp(0, -halfHeight + borderPadding + btnCancel->getContentSize().height/2));
    
    bg->setContentSize(CCSizeMake(maxWidth, totalHeight));
    
    
    // will be used for placing text field in the middle
    centerOfMargin = (msgLabel->getPosition().y + menu->getPosition().y)/2;
    
    return true;
}

bool VVAlertView::init()
{																	   
	CCAssert(0, "use initWithTitle()");
}

void VVAlertView::showAt(CCPoint aPos)
{
    CCDirector::sharedDirector()->getRunningScene()->addChild(this, 99999);
    // show AlertView in the center of screen
    setPosition(aPos);
    
    if( delegate )
        ((VVAlertViewDelegate*)delegate)->didPresentAlertView(this);
}

void VVAlertView::show()
{
    showAt(ccp(winSize.width/2, winSize.height/2));
}

void VVAlertView::willPopupClose()
{
	//CCFiniteTimeAction* fadeOut = CCSequence::actions(CCScaleTo::actionWithDuration(0.3,0.1), CCCallFunc::actionWithTarget(this, callfunc_selector(VVPopupLayer::onFadeOutCompleted)),NULL);
	//this->runAction(fadeOut);
    onFadeOutCompleted();
}

void VVAlertView::onBtnCancel(CCObject* sender)
{
    willPopupClose();
    if( delegate )
        ((VVAlertViewDelegate*)delegate)->willDismissWithButtonIndex(this,BTN_INDEX_CANCEL);
}

void VVAlertView::onBtnOk(CCObject* sender)
{
    willPopupClose();
    if( delegate )
        ((VVAlertViewDelegate*)delegate)->willDismissWithButtonIndex(this,BTN_INDEX_OK);    
}

// CCLayer의 CCAssert구문 회피용 더미함수
bool VVAlertView::ccTouchBegan(CCTouch* touch, CCEvent*event)
{
	return false;
}

// targeted multitouch를 입력받아서 다른 노드로 터치 전파를 막은 후에
// 터치가 전달되길 원하는 tableView의 터치함수를 직접 실행한다.
bool VVAlertView::ccTargetedTouchesBegan(CCSet* touches, CCEvent*event)
{
	if( VVPopupLayer::ccTargetedTouchesBegan(touches,event) )
	{
        /*
		CCTouch* touch = (CCTouch*)touches->anyObject();
		CCRect rect = CCRectMake(tblView->getPosition().x,tblView->getPosition().y,tblView->getViewSize().width,tblView->getViewSize().height);
		if ( CCRect::CCRectContainsPoint(rect,this->convertTouchToNodeSpace(touch)) ) 
		{
			tblView->ccTouchesBegan(touches, event);

		}
         */
		return true;
		
	}
	else
		return false;
	
}

void VVAlertView::ccTargetedTouchesMoved(CCSet *touches, CCEvent *event)
{
//	tblView->ccTouchesMoved(touches, event);
}
void VVAlertView::ccTargetedTouchesEnded(CCSet *touches, CCEvent *event)
{
//	tblView->ccTouchesEnded(touches, event);
}
void VVAlertView::ccTargetedTouchesCancelled(CCSet *touches, CCEvent *event) 
{
//	tblView->ccTouchesCancelled(touches, event);
}
