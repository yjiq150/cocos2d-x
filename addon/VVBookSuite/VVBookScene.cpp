//
//  VVBookScene.cpp
//  CocosX_test
//
//  Created by YoungJae Kwon on 11. 5. 21..
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

#include "VVBookScene.h"
#include "VVPageBase.h"

USING_NS_CC;


bool VVBookScene::init()
{
	if ( !CCScene::init() )
		return false;

	pageIdx = 0;
	
	winSize = CCDirector::sharedDirector()->getWinSizeInPixels();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if( winSize.width == 1280 && (winSize.height == 800 || winSize.height == 720) )
        winSize.height = 752;
#endif		
	isActionOngoing = false;
	curPage = NULL;


	CCDelayTime * delay = CCDelayTime::actionWithDuration(0.35);
	CCDelayTime * delay2 = CCDelayTime::actionWithDuration(0.1);
	CCMoveBy *moveOut = CCMoveBy::actionWithDuration(0.35f, ccp(-winSize.width,0));
	CCMoveBy *moveIn = (CCMoveBy *)moveOut->reverse();
	

	CCPageTurn3D *turnNext = CCPageTurn3D::actionWithSize(ccg(64,64), 0.7);
	CCFiniteTimeAction *slideOut = CCSequence::actions(delay,moveOut,NULL);
	CCFiniteTimeAction *slideIn = CCSequence::actions(delay,moveIn,NULL);
	
	CCFiniteTimeAction *pageTurnNext = CCSequence::actions(delay,turnNext,NULL);
	CCFiniteTimeAction *pageTurnPrev = CCSequence::actions(delay,turnNext,NULL);	

	CCFiniteTimeAction *pushToLeft = CCSequence::actions(delay2,moveOut,NULL);
	CCFiniteTimeAction *pushToRight = CCSequence::actions(delay2,moveIn,NULL);
		
	transActionArray = CCMutableArray<CCFiniteTimeAction*>::arrayWithObjects(slideIn,slideOut,pageTurnNext,pageTurnPrev,pushToLeft,pushToRight,NULL);
	transActionArray->retain();
	
	return true;
}

unsigned int VVBookScene::getPageListCount()
{
	return pageNameList.size();
}

string VVBookScene::getPageFromListAt(unsigned int idx)
{
	if (pageNameList.size() <= idx) 
		CCAssert(0,"page idx out of bounds");

	return pageNameList[idx];

}
void VVBookScene::addPageOnList(const char* pageName)
{
	string pageClassName = pageName;
	pageNameList.push_back(pageClassName);
}

void VVBookScene::loadInitialPage(string& pageClassName)
{
	VVPageFactory *factory = new VVPageFactory(pageClassName);
	curPage = factory->createPage(); // 오토릴리즈객체 반환
	addChild(curPage, kTag_PageLayer, kTag_PageLayer);
	curPage->setBookDelegate(this); // 현재페이지 클래스에서 bookScene에 접근가능하도록
	curPage->onPageStart();
}

void VVBookScene::loadPageByIndex(unsigned int idx, VVBookSceneTransitionType type)
{
	if(pageNameList.size() == 0)
		CCAssert(0,"cannot call loadPageAt without pageList");
	
	pageIdx = (idx ) % pageNameList.size();
	
	VVPageFactory *factory = new VVPageFactory(pageNameList[pageIdx]);
	this->loadPage(factory->createPage(), type);
}

bool VVBookScene::loadPage(string& pageClassName)
{
	// 기본 액션 지정
	VVPageFactory *factory = new VVPageFactory(pageClassName);
	return this->loadPage(factory->createPage());
}
bool VVBookScene::loadPage(string& pageClassName, VVBookSceneTransitionType type)
{
	VVPageFactory *factory = new VVPageFactory(pageClassName);
	return this->loadPage(factory->createPage(), type);
}
/*
 * Added by LEETAEHO 11.07.27
 * loadPage를 VVPageFactory 없이 가능하도록 추가한 함수
 */
bool VVBookScene::loadPage(VVPageBase* page)
{
	// 기본 액션 지정
	VVBookSceneTransitionType type = VVBookSceneTransSlideIn;
	return this->loadPage(page,type);
}

/*
 * Added by LEETAEHO 11.07.27
 * loadPage를 VVPageFactory 없이 가능하도록 추가한 함수
 */
bool VVBookScene::loadPage(VVPageBase* page, VVBookSceneTransitionType type)
{
	if (isActionOngoing) {
		return false;
	}
	else 
	{
		if (!curPage) {
			curPage = page;
			this->pageWillAppear(curPage);
			addChild(curPage, kTag_PageLayer, kTag_PageLayer);
			curPage->setBookDelegate(this); // 현재페이지 클래스에서 bookScene에 접근가능하도록
			curPage->onPageStart();
		} else {
			isActionOngoing = true;
			
			nextPage = page;
			this->pageWillAppear(nextPage);
			addChild(nextPage, kTag_PageBelow);
			if (type == VVBookSceneTransPushToLeft) 
			{
				CCScaleTo *zoomOut = CCScaleTo::actionWithDuration(0.35, 0.2);
				CCScaleTo *zoomIn = CCScaleTo::actionWithDuration(0.35,1);
				CCFiniteTimeAction *action = transActionArray->getObjectAtIndex(type);
				nextPage->setScale(0.5);
				nextPage->setPosition(ccp(winSize.width,0));
				nextPage->runAction( CCSpawn::actions(zoomIn,action->copy(),NULL) );			
				curPage->runAction(CCSequence::actions(CCSpawn::actions(zoomOut,action,NULL),CCCallFunc::actionWithTarget(this, callfunc_selector(VVBookScene::pageAniDone)),NULL));
			}
			else if (type == VVBookSceneTransPushToRight ) 
			{
				CCScaleTo *zoomOut = CCScaleTo::actionWithDuration(0.35, 0.2);
				CCScaleTo *zoomIn = CCScaleTo::actionWithDuration(0.35,1);
				CCFiniteTimeAction *action = transActionArray->getObjectAtIndex(type);
				nextPage->setScale(0.5);
				nextPage->setPosition(ccp(-winSize.width,0));
				nextPage->runAction( CCSpawn::actions(zoomIn,action->copy(),NULL) );
				curPage->setAnchorPoint(ccp(1,0));
				curPage->runAction( CCSequence::actions(CCSpawn::actions(zoomOut,action,NULL),CCCallFunc::actionWithTarget(this, callfunc_selector(VVBookScene::pageAniDone)),NULL));
				
			}
			else if (type == VVBookSceneTransFadeInOut)
			{
				nextPage->runAction( CCEaseExponentialOut::actionWithAction( CCActionTween::actionWithDuration(0.5,"opacity",0,255) ) );
				curPage->runAction( CCSequence::actions(CCEaseExponentialOut::actionWithAction( CCActionTween::actionWithDuration(0.5,"opacity",255,0) ),CCCallFunc::actionWithTarget(this, callfunc_selector(VVBookScene::pageAniDone)),NULL));
			}
			else
			{
				
				curPage->runAction(CCSequence::actions(transActionArray->getObjectAtIndex(type),CCCallFunc::actionWithTarget(this, callfunc_selector(VVBookScene::pageAniDone)),NULL));        		
			}
		}
	}
	
	return true;
}

void VVBookScene::pageAniDone()
{
	removeChild(curPage,true);
	curPage = nextPage;
	nextPage = NULL;
	
	reorderChild(curPage,kTag_PageLayer);
	isActionOngoing = false;
	curPage->setBookDelegate(this);
	curPage->onPageStart();
}

void VVBookScene::loadNextPage(VVBookSceneTransitionType type)
{
	if(pageNameList.size() == 0)
		CCAssert(0,"cannot call loadNextPage without pageList");
	
	pageIdx = (pageIdx + 1 ) % pageNameList.size();

	VVPageFactory *factory = new VVPageFactory(pageNameList[pageIdx]);
	this->loadPage(factory->createPage(), type);
}
void VVBookScene::pausePage()
{
    if( curPage )
        curPage->pause();
}
void VVBookScene::playPage()
{
    if (curPage) 
        curPage->play();

}

void VVBookScene::onExit()
{
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	CCScene::onExit();
}

VVBookScene::~VVBookScene()
{
	CC_SAFE_RELEASE_NULL(transActionArray);
}

#pragma mark -
#pragma mark Page Life Cycle
void VVBookScene::pageWillAppear(VVPageBase* page){}
void VVBookScene::pageWillDisappear(VVPageBase* page){}
void VVBookScene::pageDidAppear(VVPageBase* page){}
void VVBookScene::pageDidDisappear(VVPageBase* page){}