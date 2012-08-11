/*
 *  LCButtonTouchUtil.cpp
 *  Collins_PictureCard_iphone
 *
 *  Created by 태호 이 on 11. 8. 22..
 *  Copyright 2011 Viscuit Creative. All rights reserved.
 *
 */

#include "LCButtonTouchUtil.h"
#include "LCUtil.h"
#include "SimpleAudioEngine.h"

static LCButtonTouchUtil* m_psharedButtonTouchUtil = NULL;

LCButtonTouchUtil::LCButtonTouchUtil():m_pTouchArray(NULL)
{}

LCButtonTouchUtil::~LCButtonTouchUtil()
{
	CC_SAFE_RELEASE_NULL(m_pTouchArray);
}

#pragma mark -
#pragma mark Make Singleton Class
LCButtonTouchUtil* LCButtonTouchUtil::sharedButtonTouchUtil()
{
	if (!m_psharedButtonTouchUtil) {
		m_psharedButtonTouchUtil = new LCButtonTouchUtil();
		
		//initialize
		m_psharedButtonTouchUtil->m_pTouchArray = CCArray::array();
		m_psharedButtonTouchUtil->m_pTouchArray->retain();
	}
	return m_psharedButtonTouchUtil;
}

#pragma mark -
#pragma mark Action For SingleButton
void LCButtonTouchUtil::actionOnTouchBegan(CCSprite* sprite, CCSet* touches, const char* effectPath)
{
	// find LCTouchEvent containing this touch.
	bool isExist = false;
	CCTouch* pTouch = (CCTouch*) touches->anyObject();
	LCTouchEvent* pTouchEvent = NULL;
	
	for (int i=0; i < m_pTouchArray->count(); i++) {
		LCTouchEvent* touchEvent = (LCTouchEvent*) m_pTouchArray->objectAtIndex(i);
		if (pTouch == touchEvent->getTouch()) {
			// when touchEvent containing this touch.
			pTouchEvent = touchEvent;
			
			isExist = true;
			break;
		}
	}
	
	if (!isExist) {
		// no touchEvent containing this touch.
		LCTouchEvent* touchEvent = new LCTouchEvent();
		touchEvent->init(pTouch);
		m_pTouchArray->addObject(touchEvent);
		
		pTouchEvent = touchEvent;
	}
	
	// add sprites
	pTouchEvent->getNodeArray()->addObject(sprite);
	pTouchEvent->getEffectArray()->addObject(CCString::makeString(effectPath));
	
	// handle button event
	pTouchEvent->setTouchState(LCTouchStateBegan);
	if (LCUtil::isTouchInside(sprite, touches)) {
		//CCLog("%d", sprite);
		if (!pTouchEvent->getSelectedSprite()) {
			
			// first selected sprite
			pTouchEvent->setSelectedSprite(sprite);
			pTouchEvent->setOriginalColor(sprite->getColor());
			sprite->setColor(ccGRAY);
			
			// play effect sound
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(effectPath);
		} else {
			// concident sprites in same point
			if (sprite->getZOrder() >= pTouchEvent->getSelectedSprite()->getZOrder()) {
				pTouchEvent->getSelectedSprite()->setColor(pTouchEvent->getOriginalColor());
				pTouchEvent->setSelectedSprite(sprite);
				pTouchEvent->setOriginalColor(sprite->getColor());
				sprite->setColor(ccGRAY);
				
				// play effect sound
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(effectPath);
			}
		}
	}
	
}
void LCButtonTouchUtil::actionOnTouchMoved(CCSet* touches)
{
	// find LCTouchEvent containing this touch.
	CCTouch* pTouch = (CCTouch*) touches->anyObject();
	
	for (int i=0; i < m_pTouchArray->count(); i++) {
		LCTouchEvent* touchEvent = (LCTouchEvent*) m_pTouchArray->objectAtIndex(i);
		if (pTouch == touchEvent->getTouch()) {
			// when touchEvent containing this touch.
			touchEvent->setTouchState(LCTouchStateMoved);
			
			if (touchEvent->getSelectedSprite()) {
				if (!LCUtil::isTouchInside(touchEvent->getSelectedSprite(), touches)) {
					touchEvent->getSelectedSprite()->setColor(touchEvent->getOriginalColor());
					touchEvent->setSelectedSprite(NULL);
				}
			} else {
				for (int j=0; j < touchEvent->getNodeArray()->count(); j++) {
					CCSprite* pSprite = (CCSprite*) touchEvent->getNodeArray()->objectAtIndex(j);
					if (LCUtil::isTouchInside(pSprite, touches)) {
						// re touch button
						touchEvent->setSelectedSprite(pSprite);
						touchEvent->setOriginalColor(pSprite->getColor());
						pSprite->setColor(ccGRAY);
						
						// play effect sound
						CCString* effectPathStr = (CCString*) touchEvent->getEffectArray()->objectAtIndex(touchEvent->getNodeArray()->indexOfObject(pSprite));
						const char* effectPath = effectPathStr->m_sString.c_str();
						CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(effectPath);
					}
				}
			}
			
			break;
		}
	}
}
void LCButtonTouchUtil::actionOnTouchEnded(CCSet* touches)
{
	// find LCTouchEvent containing this touch and remove it.
	CCTouch* pTouch = (CCTouch*) touches->anyObject();
	
	for (int i=0; i < m_pTouchArray->count(); i++) {
		LCTouchEvent* touchEvent = (LCTouchEvent*) m_pTouchArray->objectAtIndex(i);
		if (pTouch == touchEvent->getTouch()) {
			if (touchEvent->getTouchState() != LCTouchStateEnded) {
				// when touchEvent containing this touch.
				touchEvent->setTouchState(LCTouchStateEnded);
				
				if (touchEvent->getSelectedSprite()) {
					touchEvent->getSelectedSprite()->setColor(touchEvent->getOriginalColor());
				}
				touchEvent->setSelectedSprite(NULL);
				
				m_pTouchArray->removeObject(touchEvent);
			}

			break;
		}
	}
}



/////////////////////////////////////////////////////////////////////////////////
// LCTouchEvent
/////////////////////////////////////////////////////////////////////////////////

CC_PROPERTY_SYNTHESIZE(LCTouchState, m_iTouchState, TouchState, LCTouchEvent);
CC_PROPERTY_SYNTHESIZE(CCTouch*, m_pTouch, Touch, LCTouchEvent);
CC_PROPERTY_SYNTHESIZE(CCArray*, m_pNodeArray, NodeArray, LCTouchEvent);
CC_PROPERTY_SYNTHESIZE(CCArray*, m_pEffectArray, EffectArray, LCTouchEvent);
CC_PROPERTY_SYNTHESIZE(CCSprite*, m_pSelectedSprite, SelectedSprite, LCTouchEvent);
CC_PROPERTY_SYNTHESIZE(ccColor3B, m_tOriginalColor, OriginalColor, LCTouchEvent);

#pragma mark -
#pragma mark LCTouchEvent

LCTouchEvent::LCTouchEvent():m_pNodeArray(NULL)
{}
LCTouchEvent::~LCTouchEvent()
{
	CC_SAFE_RELEASE_NULL(m_pNodeArray);
	CC_SAFE_RELEASE_NULL(m_pEffectArray);
}
bool LCTouchEvent::init(CCTouch* pTouch)
{
	m_pNodeArray = CCArray::array();
	m_pNodeArray->retain();
	
	m_pEffectArray = CCArray::array();
	m_pEffectArray->retain();
	
	m_pTouch = pTouch;
	m_iTouchState = LCTouchStateEnded;
	
	m_pSelectedSprite = NULL;
	m_tOriginalColor = ccWHITE;
	
	return true;
}