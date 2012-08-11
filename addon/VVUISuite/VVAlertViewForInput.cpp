/*
 *  VVAlertViewForInput.cpp
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 10..
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "VVAlertViewForInput.h"
#include "VVActionSet.h"
#include "CCScale9Sprite.h"

USING_NS_CC;


#define BTN_INDEX_OK        1
#define BTN_INDEX_CANCEL    0


#define MAX_FIELD_LENGTH 16
#define CENTER_PADDING 100
#define FIELD_SIZE CCSizeMake(240,30);
#define TEXTFIELD_FONT_SIZE 30


VVAlertViewForInput::VVAlertViewForInput()
{
    CCAssert(0, "Use other constructor");
}

VVAlertViewForInput::VVAlertViewForInput(const char* aAlertBG_name, const CCRect& aAlert9patch_rect, const char* aBtnBG_name, const CCRect& aBtn9patch_rect)
:VVAlertView(aAlertBG_name,aAlert9patch_rect,aBtnBG_name,aBtn9patch_rect)
{
    txtField = NULL;

    centerPadding = CENTER_PADDING;
    param.maxFieldLength = MAX_FIELD_LENGTH;
    param.textFieldFontSize = TEXTFIELD_FONT_SIZE;
    param.fieldSize = FIELD_SIZE;
    param.fieldBG_name = "";
    param.field9patch_rect = CCRectMake(0, 0, 0, 0);
}



void VVAlertViewForInput::preInit(const char* aFontName, int aTitleFontSize, int aMessageFontSize,
                                  float aMaxWidth, float aBorderPadding, float aMessagePadding,
                                  const CCSize& aBtnSize, float aBetweenBtnPadding,
                                  float aCenterPadding,  const VVAlertViewTextFieldParam& aParam)
{
    VVAlertView::preInit(aFontName, aTitleFontSize, aMessageFontSize, aMaxWidth, aBorderPadding, aMessagePadding, aBtnSize, aBetweenBtnPadding);
    
    centerPadding = aCenterPadding;    
    param = aParam;
}


bool VVAlertViewForInput::initWithTitle(const char* aTitle, const char* aMessage, VVAlertViewDelegate* aDelegate, const char* aCancelBtnTitle, const char* aOtherBtnTitle, 
                                         const char* aPlaceHolder, VVTextFieldDelegate* aTextDelegate)
{
    if( !VVAlertView::initWithTitle(aTitle, aMessage, aDelegate, aCancelBtnTitle,aOtherBtnTitle, centerPadding) )
        return false;
    
    float txtBoxPaddingLeft = 10;
    float txtBoxPaddingTop = 5;
    CCScale9Sprite *txtbg = CCScale9Sprite::scale9SpriteWithFile(param.fieldBG_name.c_str(), param.field9patch_rect);
    if( txtbg )
    {
        txtbg->setContentSize(CCSizeMake(param.fieldSize.width+txtBoxPaddingLeft*2, param.fieldSize.height+txtBoxPaddingTop*2));
        txtbg->setAnchorPoint(ccp(0.5,0));
        txtbg->setPosition(ccp(0, centerOfMargin-txtBoxPaddingTop));
        addChild(txtbg);
    }
    
    txtField = new VVTextField();
    txtField->initWithPlaceHolder(aPlaceHolder, fontName.c_str(), param.textFieldFontSize, param.maxFieldLength, param.fieldSize);
    txtField->setPosition(ccp(-param.fieldSize.width/2,centerOfMargin));
    txtField->setDelegate(aTextDelegate);
    txtField->setIsAcceptingOnlyASCII(true);
    
    addChild(txtField);
    txtField->release();
    
    return true;
}
string VVAlertViewForInput::getText()
{
    return txtField->getText();
}

void VVAlertViewForInput::setTextFieldRect(CCRect rect)
{
    txtField->setPosition(rect.origin);
    txtField->setTextFieldSize(rect.size);
}

void VVAlertViewForInput::onBtnCancel(CCObject* sender)
{
    txtField->getTextField()->detachWithIME();

    willPopupClose();
    if( delegate )
        ((VVAlertViewDelegate*)delegate)->willDismissWithButtonIndex(this,BTN_INDEX_CANCEL);
}

void VVAlertViewForInput::onBtnOk(CCObject* sender)
{
    txtField->getTextField()->detachWithIME();
    
    willPopupClose();
    if( delegate )
        ((VVAlertViewDelegate*)delegate)->willDismissWithButtonIndex(this,BTN_INDEX_OK);    
}

// CCLayer의 CCAssert구문 회피용 더미함수
bool VVAlertViewForInput::ccTouchBegan(CCTouch* touch, CCEvent*event)
{
	return false;
}

// targeted multitouch를 입력받아서 다른 노드로 터치 전파를 막은 후에
// 터치가 전달되길 원하는 tableView의 터치함수를 직접 실행한다.
bool VVAlertViewForInput::ccTargetedTouchesBegan(CCSet* touches, CCEvent*event)
{
	if( VVAlertView::ccTargetedTouchesBegan(touches,event) )
	{
        txtField->ccTouchBegan((CCTouch*)touches->anyObject(), event);
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

void VVAlertViewForInput::ccTargetedTouchesMoved(CCSet *touches, CCEvent *event)
{
//	tblView->ccTouchesMoved(touches, event);
}
void VVAlertViewForInput::ccTargetedTouchesEnded(CCSet *touches, CCEvent *event)
{
//	tblView->ccTouchesEnded(touches, event);
}
void VVAlertViewForInput::ccTargetedTouchesCancelled(CCSet *touches, CCEvent *event) 
{
//	tblView->ccTouchesCancelled(touches, event);
}
