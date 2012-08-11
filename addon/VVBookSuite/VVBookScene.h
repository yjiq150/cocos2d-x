//
//  VVBookScene.h
//  CocosX_test
//
//  Created by YoungJae Kwon on 11. 5. 21..
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

#ifndef __VVBOOKSCENE_SCENE_H__
#define __VVBOOKSCENE_SCENE_H__

#include "cocos2d.h"
#include "VVPageFactory.h"

class VVPageBase;

using namespace cocos2d;

typedef enum 
{
	VVBookSceneTransSlideIn,
	VVBookSceneTransSlideOut,
	VVBookSceneTransPageTurnNext,
	VVBookSceneTransPageTurnPrev,
	VVBookSceneTransPushToLeft,
	VVBookSceneTransPushToRight,
	VVBookSceneTransFadeInOut
} VVBookSceneTransitionType;


enum 
{
	kTag_PageBelow = 0,
    kTag_PageLayer = 1,
	kTag_PageAbove = 2,
    kTag_BottomMenuLayer = 3,
	kTag_SideMenuLayer = 4	
};


// VVBookScene을 그냥사용하지 말것
// 상속받아서 특정 scene을 만든 후 customize해서 사용할것
class VVBookScene : public CCScene
{
	
public:
	virtual bool init();  
	virtual ~VVBookScene();
	virtual void onExit();

	CC_SYNTHESIZE_READONLY(unsigned int, language, Language);
	CC_SYNTHESIZE_READONLY(unsigned int, pageIdx, PageIdx);
	
	unsigned int getPageListCount();
	string getPageFromListAt(unsigned int idx);
	void addPageOnList(const char* pageName);

	
	virtual void loadPageByIndex(unsigned int idx, VVBookSceneTransitionType type);
	void loadNextPage(VVBookSceneTransitionType type);
	
	bool loadPage(string& pageClassName);
	bool loadPage(string& pageClassName, VVBookSceneTransitionType type);
	// Added by LEETAEHO 11.07.27
	bool loadPage(VVPageBase* page);
	bool loadPage(VVPageBase* page, VVBookSceneTransitionType type);
    
    void pausePage();
    void playPage();
	
	void pageAniDone();
	
	// Page Life Cycle. Added by LEETAEHO 11.08.28
	virtual void pageWillAppear(VVPageBase* page);
	virtual void pageWillDisappear(VVPageBase* page);
	virtual void pageDidAppear(VVPageBase* page);
	virtual void pageDidDisappear(VVPageBase* page);
	
protected:
	// 상속받아서 사용할때 원하는 첫페이지 설정하기 위한 함수
	virtual void loadInitialPage(string& pageClassName);
	
	bool isActionOngoing;
	CCSize winSize;
	
	CCMutableArray<CCFiniteTimeAction*> *transActionArray;
	VVPageBase *curPage;
	VVPageBase *nextPage;
	//VVBookNavigator* nav;
	
	//페이지들의 리스트를 갖고있음
	vector<string> pageNameList;
	
};

#endif // __VVBOOKSCENE_SCENE_H__
