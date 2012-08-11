/*
 *  VVAlertViewForLogin.cpp
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 10..
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "VVAlertViewForLogin.h"
#include "VVActionSet.h"
#include "CCScale9Sprite.h"

USING_NS_CC;


#define BTN_INDEX_OK        1
#define BTN_INDEX_CANCEL    0


#define MAX_FIELD_LENGTH 16
#define CENTER_PADDING 100
#define FIELD_SIZE CCSizeMake(240,30);
#define TEXTFIELD_FONT_SIZE 30

VVAlertViewForLogin::VVAlertViewForLogin()
{
    CCAssert(0, "Use other constructor with parameter");
}

VVAlertViewForLogin::VVAlertViewForLogin(const char* aAlertBG_name, const CCRect& aAlert9patch_rect, const char* aBtnBG_name, const CCRect& aBtn9patch_rect)
:VVAlertView(aAlertBG_name,aAlert9patch_rect,aBtnBG_name,aBtn9patch_rect)
{
    //MOD:상위 생성자 호출되는지 확인
    txtUser = NULL;
    txtPassword = NULL;
    
    centerPadding = CENTER_PADDING;

    paramUser.maxFieldLength = MAX_FIELD_LENGTH;
    paramUser.textFieldFontSize = TEXTFIELD_FONT_SIZE;
    paramUser.fieldSize = FIELD_SIZE;
    paramUser.fieldBG_name = "";
    paramUser.field9patch_rect = CCRectMake(0, 0, 0, 0);

    paramPassword.maxFieldLength = MAX_FIELD_LENGTH;
    paramPassword.textFieldFontSize = TEXTFIELD_FONT_SIZE;
    paramPassword.fieldSize = FIELD_SIZE;
    paramPassword.fieldBG_name = "";
    paramPassword.field9patch_rect = CCRectMake(0, 0, 0, 0);

}



void VVAlertViewForLogin::preInit(const char* aFontName, int aTitleFontSize, int aMessageFontSize,
                                  float aMaxWidth, float aBorderPadding, float aMessagePadding,
                                  const CCSize& aBtnSize, float aBetweenBtnPadding,
                                  float aCenterPadding,  const VVAlertViewTextFieldParam& aParamUser,
                                  const VVAlertViewTextFieldParam& aParamPassword)
{
    VVAlertView::preInit(aFontName, aTitleFontSize, aMessageFontSize, aMaxWidth, aBorderPadding, aMessagePadding, aBtnSize, aBetweenBtnPadding);
    
    centerPadding = aCenterPadding;

    paramUser = aParamUser;
    paramPassword = aParamPassword;
}

bool VVAlertViewForLogin::initWithTitle(const char* aTitle, const char* aMessage, VVAlertViewDelegate* aDelegate, const char* aCancelBtnTitle, const char* aOtherBtnTitle, 
                                         const char* aPlaceHolderUser, const char* aPlaceHolderPassword, VVTextFieldDelegate* aTextDelegate)
{
    if( !VVAlertView::initWithTitle(aTitle, aMessage, aDelegate, aCancelBtnTitle,aOtherBtnTitle,centerPadding) )
        return false;
    
    float txtBoxPaddingLeft = 10;
    float txtBoxPaddingTop = 5;
    CCScale9Sprite *txtbg = CCScale9Sprite::scale9SpriteWithFile(paramUser.fieldBG_name.c_str(), paramUser.field9patch_rect);
    if( txtbg )
    {
        txtbg->setContentSize(CCSizeMake(paramUser.fieldSize.width+txtBoxPaddingLeft*2, paramUser.fieldSize.height+txtBoxPaddingTop*2));
        txtbg->setAnchorPoint(ccp(0.5,0));
        txtbg->setPosition(ccp(0, centerOfMargin+paramUser.fieldSize.height*4/5-txtBoxPaddingTop));
        addChild(txtbg);
    }
    
    txtbg = NULL;
    txtbg = CCScale9Sprite::scale9SpriteWithFile(paramPassword.fieldBG_name.c_str(), paramPassword.field9patch_rect);
    if( txtbg )
    {
        txtbg->setContentSize(CCSizeMake(paramUser.fieldSize.width+txtBoxPaddingLeft*2, paramUser.fieldSize.height+txtBoxPaddingTop*2));
        txtbg->setAnchorPoint(ccp(0.5,0));
        txtbg->setPosition(ccp(0, centerOfMargin-paramPassword.fieldSize.height*4/5-txtBoxPaddingTop));
        addChild(txtbg);
    }

    
    txtUser = new VVTextField();
    txtUser->initWithPlaceHolder(aPlaceHolderUser, fontName.c_str(), paramUser.textFieldFontSize, paramUser.maxFieldLength, paramUser.fieldSize);
    txtUser->setPosition(ccp(-paramUser.fieldSize.width/2, centerOfMargin+paramUser.fieldSize.height*4/5));
    txtUser->setDelegate(aTextDelegate);
    txtUser->setTag(VVAlertViewForLoginTypeUser);
    txtUser->setIsAcceptingOnlyASCII(true);
    
    addChild(txtUser);
    txtUser->release();
    

    txtPassword = new VVTextField();
    txtPassword->initWithPlaceHolder(aPlaceHolderPassword, fontName.c_str(), paramPassword.textFieldFontSize, paramPassword.maxFieldLength, paramPassword.fieldSize);
    txtPassword->setPosition(ccp(-paramPassword.fieldSize.width/2, centerOfMargin-paramPassword.fieldSize.height*4/5));
    txtPassword->setDelegate(aTextDelegate);
    txtPassword->setTag(VVAlertViewForLoginTypePassword);
    txtPassword->setIsPasswordText(true);
    txtPassword->setIsAcceptingOnlyASCII(true);
    
    addChild(txtPassword);
    txtPassword->release();
    
    
    return true;
}

string VVAlertViewForLogin::getTextUser()
{
    return txtUser->getText();
}

string VVAlertViewForLogin::getTextPassword()
{
    return txtPassword->getText();
}

void VVAlertViewForLogin::setTextFieldRect(CCRect rectUser, CCRect rectPass)
{
    txtUser->setPosition(rectUser.origin);
    txtPassword->setPosition(rectPass.origin);    
    
    txtUser->setTextFieldSize(rectUser.size);
    txtPassword->setTextFieldSize(rectPass.size);
}

void VVAlertViewForLogin::setFocus(VVAlertViewForLoginType aType)
{
    if( aType == VVAlertViewForLoginTypeUser )
        txtUser->getTextField()->attachWithIME();
    else
        txtPassword->getTextField()->attachWithIME();
    
}
void VVAlertViewForLogin::onBtnCancel(CCObject* sender)
{
    txtPassword->getTextField()->detachWithIME();
    txtUser->getTextField()->detachWithIME();    
    
    willPopupClose();
    if( delegate )
        ((VVAlertViewDelegate*)delegate)->willDismissWithButtonIndex(this,BTN_INDEX_CANCEL);
}

void VVAlertViewForLogin::onBtnOk(CCObject* sender)
{
    txtPassword->getTextField()->detachWithIME();
    txtUser->getTextField()->detachWithIME();        
    willPopupClose();
    if( delegate )
        ((VVAlertViewDelegate*)delegate)->willDismissWithButtonIndex(this,BTN_INDEX_OK);    
}

// CCLayer의 CCAssert구문 회피용 더미함수
bool VVAlertViewForLogin::ccTouchBegan(CCTouch* touch, CCEvent*event)
{
	return false;
}

// targeted multitouch를 입력받아서 다른 노드로 터치 전파를 막은 후에
// 터치가 전달되길 원하는 tableView의 터치함수를 직접 실행한다.
bool VVAlertViewForLogin::ccTargetedTouchesBegan(CCSet* touches, CCEvent*event)
{
	if( VVAlertView::ccTargetedTouchesBegan(touches,event) )
	{
        txtUser->ccTouchBegan((CCTouch*)touches->anyObject(), event);
        txtPassword->ccTouchBegan((CCTouch*)touches->anyObject(), event);
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

void VVAlertViewForLogin::ccTargetedTouchesMoved(CCSet *touches, CCEvent *event)
{
//	tblView->ccTouchesMoved(touches, event);
}
void VVAlertViewForLogin::ccTargetedTouchesEnded(CCSet *touches, CCEvent *event)
{
//	tblView->ccTouchesEnded(touches, event);
}
void VVAlertViewForLogin::ccTargetedTouchesCancelled(CCSet *touches, CCEvent *event) 
{
//	tblView->ccTouchesCancelled(touches, event);
}
