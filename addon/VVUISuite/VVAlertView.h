/*
 *  VVAlertView.h
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 10..
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __VVAlertView_H__
#define __VVAlertView_H__

#include "cocos2d.h"
#include "VVPopupLayer.h"

using namespace std;


typedef struct
{
    CCSize  fieldSize;
    int     maxFieldLength;
    int     textFieldFontSize;
    std::string fieldBG_name;
    CCRect      field9patch_rect;
    
} VVAlertViewTextFieldParam;


class VVAlertView;

class VVAlertViewDelegate : public VVPopupLayerDelegate
{
public:
    // mocking up iOS protocol
    virtual void willDismissWithButtonIndex(VVAlertView * alertView, int buttonIndex){};
    virtual void didPresentAlertView(VVAlertView * alertView){};
};


class VVAlertView : public VVPopupLayer
{
public:
    VVAlertView();
    VVAlertView(const char* aAlertBG_name, const CCRect& aAlert9patch_rect, const char* aBtnBG_name, const CCRect& aBtn9patch_rect);
    
    // call before init() to setup basic parameter of VVAlertView
    // if not called, default value will be used
    virtual void preInit(const char* aFontName, int aTitleFontSize, int aMessageFontSize,
                         float aMaxWidth, float aBorderPadding, float aMessagePadding,
                         const CCSize& aBtnSize, float aBetweenBtnPadding );
    
	virtual bool init();
    // one button alertView
    virtual bool initWithTitle(const char* aTitle, const char* aMessage, VVAlertViewDelegate* aDelegate, const char* aCancelBtnTitle);
    // two button alertView
    virtual bool initWithTitle(const char* aTitle, const char* aMessage, VVAlertViewDelegate* aDelegate, const char* aCancelBtnTitle, const char* aOtherBtnTitle);
    // ToDo: genenral buttons alertView
    //bool initWithTitle(const char* aTitle, const char* aMessage, VVAlertViewDelegate* aDelegate, const char* aCancelBtnTitle, const char* aOtherBtnTitleArray);
    virtual bool initWithTitle(const char* aTitle, const char* aMessage, VVAlertViewDelegate* aDelegate, const char* aCancelBtnTitle, const char* aOtherBtnTitle, float centerPadding);
    
    virtual void show();
    virtual void showAt(CCPoint aPos);
    
protected:
    virtual void onBtnCancel(CCObject* sender);
    virtual void onBtnOk(CCObject* sender);
    
	bool ccTouchBegan(CCTouch* touch, CCEvent*event);
	bool ccTargetedTouchesBegan(CCSet* touches, CCEvent*event);
	void ccTargetedTouchesMoved(CCSet *touches, CCEvent *event);
	void ccTargetedTouchesEnded(CCSet *touches, CCEvent *event);
	void ccTargetedTouchesCancelled(CCSet *touches, CCEvent *event);
	
    virtual void willPopupClose();
    
    float totalHeight;
    float centerPadding;
    float centerOfMargin;
    
    // parameters
    string alertBG_name;
    string btnBG_name;
    
    CCRect alert9patch_rect;
    CCRect btn9patch_rect;
    
    string  fontName;
    int     titleFontSize;
    int     messageFontSize;
    float   maxWidth;
    float   borderPadding;
    float   messagePadding;
    CCSize  btnSize;
    float   betweenBtnPadding;
};

#endif
