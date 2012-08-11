/*
 *  VVTextField.cpp
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 9..
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */

#include "VVTextField.h"

VVTextField::VVTextField()
{
	cursor = NULL;
	cursorOffset = ccp(0,0);
	
    isAcceptingOnlyAlphabet = false;
    isAcceptingOnlyAlphabetAndNumber = false;
    isAcceptingOnlyASCII = false;
    
    nodeToDefineTouchableArea = NULL;
    focusKeepingNode = NULL;
}

void VVTextField::registerWithTouchDispatcher()
{
    CCTouchDispatcher::sharedDispatcher()->addTargetedDelegate(this, 0, false);
}

bool VVTextField::initWithPlaceHolder(const char *placeholder, const char *aFontName, float aFontSize, int limit, CCSize aFieldSize)
{
	if ( !CCLayer::init() )
		return false;
	
    setIsTouchEnabled(true);
    
	// 텍스트 필드의 폰트/사이즈 저장
	fontName = aFontName;
	fontSize = aFontSize;
	
	// 임의로 100넣어놓았음, (0,0)을 넣을경우 alignment동작 안함.
	m_pTextField = CCTextFieldTTF::textFieldWithPlaceHolder(placeholder,CCSizeMake(0,0),CCTextAlignmentLeft,fontName.c_str(),fontSize);
	m_pTextField->setAnchorPoint(ccp(0,0)); 	// 앵커 0,0 에 동작하도록 작성해놓았음
    addChild(m_pTextField);
	// CCTextFieldTTF의 delegation 구현
	m_pTextField->setDelegate(this);
	
	
	
	m_nCharLimit = limit;	
    cursorBlink = CCRepeatForever::actionWithAction(
							dynamic_cast<CCActionInterval*>(CCSequence::actions(CCFadeOut::actionWithDuration(0.4),CCFadeIn::actionWithDuration(0.4),NULL))
														   );
    cursorBlink->retain();
    m_bAction = false;
	m_pDelegate = NULL;

	nodeToDefineTouchableArea = CCNode::node();
	nodeToDefineTouchableArea->retain();
	nodeToDefineTouchableArea->setContentSize(aFieldSize);
	
    cursor =  CCLayerColor::layerWithColorWidthHeight(ccc4(255, 255, 255, 255), 5, 25);
    cursor->setOpacity(0);
    addChild(cursor);
	
	return true;
}
void VVTextField::setTextFieldSize(CCSize aSize)
{
    nodeToDefineTouchableArea->setContentSize(aSize);
}
void VVTextField::setFontColor(ccColor3B aColor)
{
	fontColor = aColor;
	m_pTextField->setColor(fontColor);
}

ccColor3B VVTextField::getFontColor()
{
	return m_pTextField->getColor();
}

void VVTextField::setCursorColor(ccColor3B aColor)
{
	((CCRGBAProtocol*)cursor)->setColor(aColor);
}

ccColor3B VVTextField::getCursorColor()
{
	return ((CCRGBAProtocol*)cursor)->getColor();
}

void VVTextField::setCursorSprite(CCSprite* aCursorSprite)
{
    if (cursor && cursor->getParent() != NULL) 
    {
        cursor->removeFromParentAndCleanup(true);
    }
	cursor = aCursorSprite;
	
	cursor->setAnchorPoint(ccp(0,0));
	cursor->setOpacity(0);
	addChild(aCursorSprite);
}

void VVTextField::setIsPasswordText(bool isEnabled)
{
    m_pTextField->setIsPasswordText(isEnabled);
}


const char* VVTextField::getText()
{
    return m_pTextField->getString();
}

VVTextField::~VVTextField()
{
	CC_SAFE_RELEASE(nodeToDefineTouchableArea);
	CC_SAFE_RELEASE(cursorBlink);
}


// CCTextFieldDelegate protocol
bool VVTextField::onTextFieldAttachWithIME(CCTextFieldTTF * pSender)
{
    if (! m_bAction)
    {
        cursor->runAction(cursorBlink);
        m_bAction = true;
		
		// modified by LEETAEHO on 11.07.14
		// 텍스트 수정을 시작했을때
		if (m_pDelegate) {
			m_pDelegate->textFieldDidBeginEditing(this);
		}
    }
    return false;
}

bool VVTextField::onTextFieldDetachWithIME(CCTextFieldTTF * pSender)
{
    if (m_bAction)
    {
        cursor->stopAction(cursorBlink);
        cursor->setOpacity(0);
		
        m_bAction = false;
		
		// modified by LEETAEHO on 11.07.14
		// 텍스트 수정을 종료했을때
		if (m_pDelegate) {
			m_pDelegate->textFieldDidEndEditing(this);
		}
    }
    return false;
}

bool VVTextField::onTextFieldInsertText(CCTextFieldTTF * pSender, const char * text, int nLen)
{
    // if insert enter, treat as default to detach with ime
    if ('\n' == *text)
    {
        return false;
    }
    
	// 영문 입력만 허용되어있는 경우
	if (isAcceptingOnlyAlphabet && ( !(97 <= *text && *text <= 122) || !(65 <= *text && *text <= 90)) )
	{
		return true;
	}
    if (isAcceptingOnlyAlphabetAndNumber && ( !(97 <= *text && *text <= 122) || !(65 <= *text && *text <= 90) || !(48 <= *text && *text <= 57)))
	{
		return true;
	}
    if( isAcceptingOnlyASCII && !(33 <= *text && *text <=126))
    {
        return true;
    }
    // if the textfield's char count more than m_nCharLimit, doesn't insert text anymore.
    if (pSender->getCharCount() >= m_nCharLimit)
    {
        return true;
    }
	
	// modified by LEETAEHO on 11.07.14
	// 텍스트가 바뀌었을때 delegate, 추가와 삭제 모두 있어야 함
	if (m_pDelegate) {
		std::string buf = pSender->getString();
		buf += text;
		m_pDelegate->textDidChange(this, buf.c_str());
	}
	
    if( m_pTextField->getIsPasswordText() )
        return false;
    
    // create a insert text sprite and do some action
    CCLabelTTF * label = CCLabelTTF::labelWithString(text, fontName.c_str(), fontSize);
	label->setColor(fontColor);
    this->addChild(label);
	
    // move the sprite from top to position
    CCPoint endPos = pSender->getPosition();
    if (pSender->getCharCount())
    {
        endPos.x += pSender->getContentSize().width / 2;
    }

	
    CCSize  inputTextSize = label->getContentSize();
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint beginPos(endPos.x, winSize.height - inputTextSize.height * 2); 
	
    ccTime duration = 0.5;
    label->setPosition(beginPos);
    label->setScale(8);
	
    CCAction * seq = CCSequence::actions(
										 CCSpawn::actions(
														  CCMoveTo::actionWithDuration(duration, endPos),
														  CCScaleTo::actionWithDuration(duration, 1),
														  CCFadeOut::actionWithDuration(duration),
														  0),
										 CCCallFuncN::actionWithTarget(this, callfuncN_selector(VVTextField::callbackRemoveNodeWhenDidAction)),
										 0);
    label->runAction(seq);
	
    return false;
}

bool VVTextField::onTextFieldDeleteBackward(CCTextFieldTTF * pSender, const char * delText, int nLen)
{
	// modified by LEETAEHO on 11.07.14
	// 텍스트가 바뀌었을때 delegate, 추가와 삭제 모두 있어야 함
	if (m_pDelegate) {
		char *buf = (char *)malloc(sizeof(char) * (strlen(pSender->getString()) + 1));
		strcpy(buf, pSender->getString());
		buf[strlen(buf) - 1] = NULL;
		m_pDelegate->textDidChange(this, buf);
		free(buf);
	}
	
    if( m_pTextField->getIsPasswordText() )
        return false;
    
    // create a delete text sprite and do some action
    CCLabelTTF * label = CCLabelTTF::labelWithString(delText, fontName.c_str(), fontSize);
	label->setColor(fontColor);
    this->addChild(label);
	
    // move the sprite to fly out
    CCPoint beginPos = pSender->getPosition();
    CCSize textfieldSize = pSender->getContentSize();
    CCSize labelSize = label->getContentSize();
    beginPos.x += (textfieldSize.width - labelSize.width) / 2.0f;
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();	
	CCPoint endPos(- winSize.width / 4.0f, winSize.height * (0.5 + (float)rand() / (2.0f * RAND_MAX)));
	
    ccTime duration = 1;
    ccTime rotateDuration = 0.2f;
    int repeatTime = 5; 
    label->setPosition(beginPos);
	
    CCAction * seq = CCSequence::actions(
										 CCSpawn::actions(
														  CCMoveTo::actionWithDuration(duration, endPos),
														  CCRepeat::actionWithAction(
																					 CCRotateBy::actionWithDuration(rotateDuration, (rand()%2) ? 360 : -360),
																					 repeatTime),
														  CCFadeOut::actionWithDuration(duration),
														  0),
										 CCCallFuncN::actionWithTarget(this, callfuncN_selector(VVTextField::callbackRemoveNodeWhenDidAction)),
										 0);
    label->runAction(seq);
    return false;
}


// 따로 그리고싶은것이 있을경우 여기서 직접 그린다
// return true 할 경우 기본 CCTextFieldTTF의 드로잉을 하지 않음
bool VVTextField::onDraw(CCTextFieldTTF * pSender)
{
	// 커서위치를 입력상태에 따라서 실시간으로 업데이트 해준다
	if (cursor) 
	{
		if ( strlen(m_pTextField->getString()) == 0 ) 
		{
			cursor->setPosition(cursorOffset);
		}
		else 
		{
			cursor->setPosition(ccp(m_pTextField->getContentSize().width+cursorOffset.x, cursorOffset.y));
		}
	}
    return false;
}

void VVTextField::callbackRemoveNodeWhenDidAction(CCNode * pNode)
{
    this->removeChild(pNode, true);
}


void VVTextField::setFocusKeepingNode(CCNode* aNode)
{
	focusKeepingNode = aNode;
}
void VVTextField::removeFocusKeepingNode()
{
	focusKeepingNode = NULL;
}

bool VVTextField::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
	// 현재 텍스트 필드노드(trackNode)나 인풋포커스를 유지해도 되는 노드안에 터치인지 확인
    CCPoint point = convertTouchToNodeSpace(pTouch);
	bool isTouchingTrackNode = CCRect::CCRectContainsPoint(nodeToDefineTouchableArea->getRect(), point);
	bool isTouchingFocuskeepingNode = false;
	
	// setting이 되어있을경우만 체크
	if( focusKeepingNode != NULL)
	{
		point = focusKeepingNode->getParent()->convertTouchToNodeSpace(pTouch);
		isTouchingFocuskeepingNode = CCRect::CCRectContainsPoint(focusKeepingNode->getRect(), point);
	}
	
	if ( isTouchingTrackNode || isTouchingFocuskeepingNode  )
    {
        m_pTextField->attachWithIME();
        return true;
    }
    else
    {
        m_pTextField->detachWithIME();
        return false;
    }
}

/* 
void KeyboardNotificationLayer::keyboardWillShow(CCIMEKeyboardNotificationInfo& info)
{
    //CCLog("TextInputTest:keyboardWillShowAt(origin:%f,%f, size:%f,%f)", info.end.origin.x, info.end.origin.y, info.end.size.width, info.end.size.height);
	
    if (! trackNode)
        return;
	
    CCRect rectTracked = trackNode->getRect();
    //CCLog("TextInputTest:trackingNodeAt(origin:%f,%f, size:%f,%f)", rectTracked.origin.x, rectTracked.origin.y, rectTracked.size.width, rectTracked.size.height);
	
    // if the keyboard area doesn't intersect with the tracking node area, nothing need to do.
    if (! CCRect::CCRectIntersectsRect(rectTracked, info.end))
        return;
	
	키보드 올라올때 위치조정하는 코드
     // assume keyboard at the bottom of screen, calculate the vertical adjustment.
     float adjustVert = CCRect::CCRectGetMaxY(info.end) - CCRect::CCRectGetMinY(rectTracked);
     CCLog("TextInputTest:needAdjustVerticalPosition(%f)", adjustVert);
     
     // move all the children node of KeyboardNotificationLayer
     CCArray * children = getChildren();
     CCNode * node = 0;
     int count = children->count();
     CCPoint pos;
     for (int i = 0; i < count; ++i)
     {
     node = (CCNode*)children->objectAtIndex(i);
     pos = node->getPosition();
     pos.y += adjustVert;
     node->setPosition(pos);
     }
	 
}
*/