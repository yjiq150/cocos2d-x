/*
 *  VVTextField.h
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 9..
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */

#ifndef __VVTextField_H__
#define __VVTextField_H__

#include "cocos2d.h"

class VVTextField;

class VVTextFieldDelegate 
{
public:
	// optional
	virtual void textFieldDidBeginEditing(VVTextField* pTextField){};
	virtual void textFieldDidEndEditing(VVTextField* pTextField){};
	virtual void textDidChange(VVTextField* pTextField, const char* pText){};
	/*
	 Managing Editing
	 – textFieldShouldBeginEditing:
	 – textFieldDidBeginEditing:
	 – textFieldShouldEndEditing:
	 – textFieldDidEndEditing:
	 Editing the Text Field’s Text
	 – textField:shouldChangeCharactersInRange:replacementString:
	 – textFieldShouldClear:
	 – textFieldShouldReturn:
	 
	 //- (void)searchBar:(UISearchBar *)searchBar textDidChange:(NSString *)searchText
	 */
};

// Wrapper class for CCTextFieldTTF
class VVTextField : public CCLayer,public CCIMEDelegate, public CCTextFieldDelegate
{
	
public:
	VVTextField();
	 ~VVTextField();
	virtual bool initWithPlaceHolder(const char *placeholder, const char *aFontName, float aFontSize, int limit, CCSize aFieldSize);
    void registerWithTouchDispatcher();
    
    void callbackRemoveNodeWhenDidAction(CCNode * pNode);

    // CCTextFieldDelegate
    virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * pSender);
    virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * pSender);
    virtual bool onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen);
    virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, int nLen);
    virtual bool onDraw(CCTextFieldTTF * pSender);

	CC_SYNTHESIZE(VVTextFieldDelegate*, m_pDelegate, Delegate);
	CC_SYNTHESIZE_READONLY(CCTextFieldTTF*, m_pTextField, TextField);
	
    CC_PROPERTY(ccColor3B, fontColor, FontColor);
    void setTextFieldSize(CCSize aSize);
    void setIsPasswordText(bool isEnabled);
    
	// 커서의 색상, 스프라이트, 오프셋정보 설정
	CC_SYNTHESIZE(CCPoint, cursorOffset, CursorOffset);
    void        setCursorColor(ccColor3B aColor);
    ccColor3B   getCursorColor();
    void        setCursorSprite(CCSprite* aCursorSprite);     // set Custom Sprite cursor
    
    // 텍스트 박스 필터 조건
	CC_SYNTHESIZE(bool, isAcceptingOnlyAlphabet, IsAcceptingOnlyAlphabet);
    CC_SYNTHESIZE(bool, isAcceptingOnlyAlphabetAndNumber, IsAcceptingOnlyAlphabetAndNumber);
    CC_SYNTHESIZE(bool, isAcceptingOnlyASCII, IsAcceptingOnlyASCII);
	const char* getText();
    
    // 이곳에 노드를 추가해놓으면 해당노드 안의 터치를 판단하여(getRect를 이용)
	// 텍스트 입력모드를 계속 유지한다
	void setFocusKeepingNode(CCNode* aNode);
	void removeFocusKeepingNode();
    
    virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
protected:
	string fontName;
	float fontSize;
    CCRepeatForever* cursorBlink;
    CCNode* cursor;
    
    bool m_bAction;
    int m_nCharLimit;       // the textfield max char limit
	CCNode* nodeToDefineTouchableArea; // 이 영역의 위치/컨텐츠 사이즈에 결정되는 영역에 터치 입력시 키보드 입력 시작
	CCNode* focusKeepingNode;
    
    string passwordActualText;

};

#endif