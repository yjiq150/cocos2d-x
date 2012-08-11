/*
 *  VVAlertViewForLogin.h
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 10..
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __VVAlertViewForLogin_H__
#define __VVAlertViewForLogin_H__

#include "cocos2d.h"
#include "VVAlertView.h"

#include "VVTextField.h"
enum VVAlertViewForLoginType
{
    VVAlertViewForLoginTypeUser,
    VVAlertViewForLoginTypePassword
};

class VVAlertViewForLogin : public VVAlertView
{
public:
    VVAlertViewForLogin();
    VVAlertViewForLogin(const char* aAlertBG_name, const CCRect& aAlert9patch_rect, const char* aBtnBG_name, const CCRect& aBtn9patch_rect);
    
    virtual void preInit(const char* aFontName, int aTitleFontSize, int aMessageFontSize,
                         float aMaxWidth, float aBorderPadding, float aMessagePadding,
                         const CCSize& aBtnSize, float aBetweenBtnPadding,
                         float aCenterPadding,  const VVAlertViewTextFieldParam& aParamUser, const VVAlertViewTextFieldParam& aParamPassword);
    // two button alertView
    bool initWithTitle(const char* aTitle, const char* aMessage, VVAlertViewDelegate* aDelegate, const char* aCancelBtnTitle, const char* aOtherBtnTitle,
                       const char* aPlaceHolderUser, const char* aPlaceHolderPassword, VVTextFieldDelegate* aTextDelegate);

    void setTextFieldRect(CCRect rectUser, CCRect rectPass);
    string getTextUser();
    string getTextPassword();
    
    void setFocus(VVAlertViewForLoginType aType);
    
protected:
    VVTextField* txtUser;
    VVTextField* txtPassword;
    
    
    void onBtnCancel(CCObject* sender);
    void onBtnOk(CCObject* sender);
    
	bool ccTouchBegan(CCTouch* touch, CCEvent*event);
	bool ccTargetedTouchesBegan(CCSet* touches, CCEvent*event);
	void ccTargetedTouchesMoved(CCSet *touches, CCEvent *event);
	void ccTargetedTouchesEnded(CCSet *touches, CCEvent *event);
	void ccTargetedTouchesCancelled(CCSet *touches, CCEvent *event);

protected:
    float   centerPadding;
    VVAlertViewTextFieldParam paramUser;
    VVAlertViewTextFieldParam paramPassword;
};

#endif
