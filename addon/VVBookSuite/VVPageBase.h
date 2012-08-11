//
//  VVPageBase.h
//  CocosX_test
//
//  Created by YoungJae Kwon on 11. 5. 21..
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

#ifndef __VVPageBase_SCENE_H__
#define __VVPageBase_SCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "CCSymbol.h"
#include "CCTouchArea.h"

// 여기 컴파일 에러 날경우 43라인의 class VVBookScene; 코멘트아웃 하고 아래 인클루드를 활성화
//#include "VVBookScene.h"

using namespace cocos2d;
using namespace CocosDenshion;


enum {
	kTag_ParallaxBG = 0,
    kTag_ParallaxSky = 1,
    kTag_ParallaxCloud = 2,
	kTag_ParallaxHouse = 3,
	kTag_ParallaxLiveObj = 4
};

enum {
	kTag_PageBGLayer = 0,
	kTag_PageCanvasLayer = 1,
	kTag_PageMainLayer = 2,
	kTag_PageParallax = 3,
	kTag_PageTextLayer = 5,
	kTag_PageLuckyBag = 8,
	kTag_PageLuckyBagPopup = 9,
	kTag_PageLuckyBagPopupClose = 10
};
 
class VVBookScene;

class VVPageBase : public CCLayer
{
	
public:
    VVPageBase();
	virtual ~VVPageBase();    
	virtual bool init();  
	virtual void onExit();

	
//	virtual void ccTouchesBegan(CCSet* touches, CCEvent* event);
//	virtual void ccTouchesMoved(CCSet* touches, CCEvent* event);
//	virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);
	

	virtual void onPageStart();
	
	void addSymbol(CCSymbol *symbol);
	CCSymbol* getSymbolWithName(string name);
	CCSymbol* getTouchedSymbol(CCTouch *touch);
	
	void addTouchArea(CCTouchArea *touchArea);
	CCTouchArea* getTouchAreaWithName(string name);
	
	CC_SYNTHESIZE_RETAIN(CCString*, bgm, BGM);
    CC_SYNTHESIZE(bool, isUsingFlashCoordinate, IsUsingFlashCoordinate);
	VVBookScene* getBookDelegate();
	void setBookDelegate(VVBookScene* delegate);
    
	void setBackGroundImage(const char* name);
	
    // interface for pause & play
    virtual void pause();
    virtual void play();
    
protected:
	VVBookScene* bookDelegate;
	CCSize winSize;

	SimpleAudioEngine *ap;
	CCNode *mainLayer;
	CCNode *bgLayer;
	CCFiniteTimeAction *bounceAction;
	CCFiniteTimeAction *rotateAction;
	
	//해당 페이지에 들어있는 심볼명들을 모두 저장
	CCMutableDictionary<std::string,CCSymbol*> *symbolDic;
	
	//해당 페이지에 들어있는 터치영역들을 모두 저장	
	CCMutableDictionary<std::string,CCTouchArea*> *touchDic;
	
	// 스티커북을 위한 변수: 사용자에의해 심볼이 터치된 상태인지 확인
	typedef map<std::string, bool> SymbolDic;
	SymbolDic isSymbolTouchBeganDic;	
	
	// 배경음악 값이 없을 경우 재생을 멈출것인지에 대한 플래그 by LEETAEHO on 11.10.14
};

#endif // __VVPageBase_SCENE_H__
