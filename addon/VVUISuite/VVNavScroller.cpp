/*
 *  VVNavScroller.cpp
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 10..
 *  Copyright 2011 Viscuit Creative Inc. All rights reserved.
 *
 */

#include "VVNavScroller.h"
#include "VVActionSet.h"
#include "CCTextureCache.h"

USING_NS_CC;
CCAsyncObjectForVVNavScroller::CCAsyncObjectForVVNavScroller()
{
	//CCLog("CCAsyncObjectForVVNavScroller created");
}
CCAsyncObjectForVVNavScroller::~CCAsyncObjectForVVNavScroller()
{
	//CCLog("CCAsyncObjectForVVNavScroller destroyd");
}


VVNavScroller::VVNavScroller()
{
	arrowMinus = NULL;
	arrowPlus = NULL;
	navDelegate = NULL;
    selected = -1;
}

bool VVNavScroller::initWithViewSizeAndDir(CCSize size, VVScrollViewDirection dir)
{
	if ( !VVScrollView::initWithViewSize(size) )
		return false;
	
	winSize = CCDirector::sharedDirector()->getWinSizeInPixels();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if( winSize.width == 1280 && (winSize.height == 800 || winSize.height == 720) )
        winSize.height = 752;
#endif		
	
	setDirection(dir);
	setViewSize(size);	
	setDelegate(this);
	
	isMoved = false;
	isTouchOnNavScroller = false;
	
	numberOfNavIcons = 0;
	
	// 수직 스크롤 모드시 좌표계때문에 wrapper 필요
	wrapper = CCSymbol::node();
	wrapper->setAnchorPoint(ccp(0,0));
	addChild(wrapper);
	
	return true;
}
void VVNavScroller::loadIcons(vector<string>* navName)
{
	// override me!
	// 서브클래싱한 특정 스크롤러에서 의무적으로 오버라이드 해서 아이콘을 불러올것
	CCAssert(0,"override Me, VVNavScroller::loadIcons");
}
void VVNavScroller::reloadIcons(vector<string>* navName)
{
	//MOD: 삭제하기 전에 적절한 애니메이션 걸고 삭제
	wrapper->removeAllChildrenWithCleanup(true);
	loadIcons(navName);
	

}
void VVNavScroller::loadIcons(int navIdx)
{
	// override me!
	// 서브클래싱한 특정 스크롤러에서 의무적으로 오버라이드 해서 아이콘을 불러올것
	CCAssert(0,"override Me, VVNavScroller::loadIcons");
}

void VVNavScroller::reloadIcons(int navIdx)
{
	//MOD: 삭제하기 전에 적절한 애니메이션 걸고 삭제
	wrapper->removeAllChildrenWithCleanup(true);
	loadIcons(navIdx);
	
	
}
void VVNavScroller::imageLoadFinished(CCObject *aObject)
{
	CCAsyncObjectForVVNavScroller *async = (CCAsyncObjectForVVNavScroller*)aObject;

	//CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
	CCTexture2D* texture = CCTextureCache::sharedTextureCache()->textureForKey( async->getTextureName() );
	if (texture != NULL)
	{
		CCSprite *sprIcon = CCSprite::spriteWithTexture(texture);
		sprIcon->setPosition( async->sprPosition );
		wrapper->addChild(sprIcon,async->iconIdx,async->iconIdx); // idx값으로 태그
		CCLog("Texture AsyncLoad Success:%s",async->getTextureName());
	}
	else
	{
		CCLog("Texture AsyncLoad Fail:%s",async->getTextureName());
	}

}

void VVNavScroller::scrollViewDidScroll(VVScrollView *aView)
{	
	if (arrowMinus && arrowPlus)
	{	
		if( direction == VVScrollViewDirectionHorizontal)
		{
			// 스크롤 방향 남아있나없나 표시
			if( aView->getContentOffset().x >= 0 )
				arrowMinus->setIsVisible(false);
			else
				arrowMinus->setIsVisible(true);
			
			if ( aView->getContentOffset().x <= -(aView->getContentSize().width - aView->getViewSize().width) )
				arrowPlus->setIsVisible(false);
			else 
				arrowPlus->setIsVisible(true);
		}
		else if ( direction == VVScrollViewDirectionVertical ) 
		{
			
			// 스크롤 방향 남아있나없나 표시
			if( aView->getContentOffset().y >= 0 )
				arrowMinus->setIsVisible(false);
			else
				arrowMinus->setIsVisible(true);
			
			if ( aView->getContentOffset().y <= -(aView->getContentSize().height - aView->getViewSize().height) )
				arrowPlus->setIsVisible(false);
			else 
				arrowPlus->setIsVisible(true);
		}

	}
	
}

void VVNavScroller::setSelected(unsigned int idx)
{
	// 이부분은 상속받아서 커스텀할것!
	/*
	// 예전에 선택되어있던 스프라이트 존재시
	CCSprite * lastSpr = (CCSprite*)container_->getChildByTag(selected);
	if (lastSpr)
	{
		lastSpr->removeChildByTag(selected, true);
	}	
	 
	CCSprite * spr = (CCSprite*)container_->getChildByTag(idx);
	if (spr) 
	{
		//현재 선택된 부분에 테두리 추가
		int borderSize = 10;
		CCSprite* sprBorder = new CCSprite();
		sprBorder->initWithTexture(VVBorderSprite::createBorderTextureWithSprite(spr,borderSize));
		sprBorder->setColor(ccc3(255,187,3));
		sprBorder->setAnchorPoint(ccp(0,0));
		sprBorder->setPosition(ccp(-borderSize/2,-borderSize/2));
		// 인텍스를 태그로해서 추가
		spr->addChild(sprBorder,-10,idx);
		
		sprBorder->release();
	}
	 */
	
	// 선택된 인덱스 업데이트
	selected = idx;

}

unsigned int VVNavScroller::getSelected()
{
	return selected;
}

void VVNavScroller::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
	// 오버라이드 했을경우 상위클래스 호출
	VVScrollView::ccTouchesBegan(touches,event);

	// validTouch가 아닐경우(뷰사이즈 바깥의 터치) 아이콘에대한 터치 받지 않음
	if (!m_bIsVisible || !isEnabled || !isValidTouch) 
		return;

	CCTouch *touch = (CCTouch*)touches->anyObject();
	CCPoint touchLocation = touch->locationInView(  );
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
	
	isMoved = false;
	touchedIcon = NULL;
	for(int i=0; i < numberOfNavIcons; i++)
	{
		CCSprite * sprIcon = (CCSprite*)wrapper->getChildByTag(i);
 		if (sprIcon == NULL)
			continue;
		
		// 터치된 아이콘이 확인되면 터치된 아이콘 변수 업데이트
		if( VVActionSet::scaleUpOnTouchBegan(sprIcon,touchLocation) )
		{
			isTouchOnNavScroller = true;
			touchedIcon = sprIcon;
			break;
		}
	}
}
	

void VVNavScroller::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
	// 오버라이드 했을경우 상위클래스 호출
	VVScrollView::ccTouchesMoved(touches,event);
		
	if (!m_bIsVisible || !isEnabled || !isValidTouch)
		return;
	
	CCTouch *touch = (CCTouch*)touches->anyObject();
	CCPoint touchLocation = touch->locationInView(  );

	
	// 안드로이드용 isMove 체크 코드
	// 안드로이드의 경우 움직이지 않고 터치를 떼도 moved와 ended가 떼는 순간 동시에 호출된다.
	// 따라서 이전 터치포인트에서 달라지지 않은 moved는 moved로 생각하지 않는다!
	CCPoint prevLocation = touch->previousLocationInView(  );
	if (!isMoved && touchLocation.x == prevLocation.x && touchLocation.y == prevLocation.y) 
	{
		isMoved = false;
	}
	else 
		isMoved = true;
	
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
	
	
	// 터치된 아이콘이 있을때 moveOut된경우
	if(touchedIcon != NULL && isTouchOnNavScroller)
		VVActionSet::scaleDownOnMoveOut(touchedIcon, touchLocation);
	
}

void VVNavScroller::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
	// 오버라이드 했을경우 상위클래스 호출
	VVScrollView::ccTouchesEnded(touches,event);
	
	
	
	CCTouch *touch = (CCTouch*)touches->anyObject();
	CCPoint touchLocation = touch->locationInView(  );
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
	
	//제자리 터치인 상태로 터치가 끝났을경우에만 해당 아이콘의 동작 수행
	if( touchedIcon != NULL && isTouchOnNavScroller && VVActionSet::scaleDownOnTouchEnded(touchedIcon, touchLocation) && !isMoved )
	{
		
		// 터치된 아이콘 인덱스 값과 함께 델리게이션 실행
		int idx = touchedIcon->getTag();
		navDelegate->onNavIconTouched(this,idx);
		
	}
	isTouchOnNavScroller = false;
	touchedIcon = NULL;
}
