/*
 *  LCMenu.cpp
 *  CollinsDictionaryHD
 *
 *  Created by 태호 이 on 11/22/11.
 *  Copyright 2011 Viscuit Creative. All rights reserved.
 *
 */

#include "LCMenu.h"


LCMenu * LCMenu::menuWithItems(CCMenuItem* item, ...)
{
	va_list args;
	va_start(args,item);
	LCMenu *pRet = new LCMenu();
	if (pRet && pRet->initWithItems(item, args))
	{
		pRet->autorelease();
		va_end(args);
		return pRet;
	}
	va_end(args);
	CC_SAFE_DELETE(pRet)
	return NULL;
}

LCMenu * LCMenu::menuWithItems(int priority, CCMenuItem* item, ...)
{
	va_list args;
	va_start(args,item);
	LCMenu *pRet = new LCMenu();
	pRet->setTouchPriority(priority);
	if (pRet && pRet->initWithItems(item, args))
	{
		pRet->autorelease();
		va_end(args);
		return pRet;
	}
	va_end(args);
	CC_SAFE_DELETE(pRet)
	return NULL;
}

void LCMenu::ccTargetedTouchesMoved(CCSet* touches, CCEvent* event)
{
	CCTouch* touch = (CCTouch*) touches->anyObject();
	
	CC_UNUSED_PARAM(event);
	CCAssert(m_eState == kCCMenuStateTrackingTouch, "[Menu ccTouchMoved] -- invalid state");
	
	// 관대하게 터치체크 추가 on 111122 by LEETAEHO 
	
	CCMenuItem *currentItem;
	if (m_pSelectedItem) {
		currentItem = this->itemForTouchGenerous(touch);
	} else {
		currentItem = this->itemForTouch(touch);
	}
	
	if (currentItem != m_pSelectedItem) 
	{
		if (m_pSelectedItem)
		{
			m_pSelectedItem->unselected();
		}
		m_pSelectedItem = currentItem;
		if (m_pSelectedItem)
		{
			m_pSelectedItem->selected();
		}
	}
}

CCMenuItem* LCMenu::itemForTouchGenerous(cocos2d::CCTouch *touch)
{

	CCPoint touchLocation = touch->locationInView(touch->view());
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);

	CCNode* pChild = (CCNode*) m_pSelectedItem;
    
//    if ( IS_TOUCH_INSIDE(pChild, touchLocation) )
//        return (CCMenuItem*)pChild;
	if (pChild && pChild->getIsVisible() && ((CCMenuItem*)pChild)->getIsEnabled())
	{
		CCPoint local = pChild->convertToNodeSpace(touchLocation);
		CCRect r = ((CCMenuItem*)pChild)->rect();
		r.origin = CCPointZero;
		
		// 관대하게 터치체크 추가 on 111122 by LEETAEHO
		float degree = 20;
		r.origin.x -= degree;
		r.origin.y -= degree;
		r.size.width += degree * 2;
		r.size.height += degree * 2;
		
		if (CCRect::CCRectContainsPoint(r, local))
		{
			return (CCMenuItem*)pChild;
		}
	}
	
	/*
	CCPoint touchLocation = touch->locationInView(touch->view());
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
	
	if (m_pChildren && m_pChildren->count() > 0)
	{
		CCObject* pObject = NULL;
		CCARRAY_FOREACH(m_pChildren, pObject)
		{
			CCNode* pChild = (CCNode*) pObject;
			if (pChild && pChild->getIsVisible() && ((CCMenuItem*)pChild)->getIsEnabled())
			{
				CCPoint local = pChild->convertToNodeSpace(touchLocation);
				CCRect r = ((CCMenuItem*)pChild)->rect();
				r.origin = CCPointZero;
				
				// 관대하게 터치체크 추가 on 111122 by LEETAEHO
				float degree = 20;
				r.origin.x -= degree;
				r.origin.y -= degree;
				r.size.width += degree * 2;
				r.size.height += degree * 2;
				
				if (CCRect::CCRectContainsPoint(r, local))
				{
					return (CCMenuItem*)pChild;
				}
			}
		}
		
	}
	 */
	
	return NULL;
}

