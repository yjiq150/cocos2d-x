//
//  VVPageBase.cpp
//  CocosX_test
//
//  Created by YoungJae Kwon on 11. 5. 21..
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

#include "VVPageBase.h"


USING_NS_CC;
using namespace CocosDenshion;

VVPageBase::VVPageBase()
{
    // 기존 플래쉬 좌표계사용하는 앱들을 위해서 기본값 true
    isUsingFlashCoordinate = true;
}
bool VVPageBase::init()
{
	if ( !CCLayer::init() )
	{
		return false;
	}
	
	// 기본 셋팅
	ap = SimpleAudioEngine::sharedEngine();
	winSize = CCDirector::sharedDirector()->getWinSizeInPixels();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    if( winSize.width == 1280 && (winSize.height == 800 || winSize.height == 720) )
        winSize.height = 752;
#endif		
	this->setIsTouchEnabled(true);

	// 메인 노드 	
	mainLayer = CCNode::node();
	this->addChild(mainLayer, kTag_PageMainLayer);
		
	// 배경 노드 
	bgLayer = CCNode::node();
	this->addChild(bgLayer, kTag_PageBGLayer);
	
    if( isUsingFlashCoordinate )
    {
        mainLayer->setPosition(ccp(0,winSize.height));					
        bgLayer->setPosition(ccp(0,winSize.height));
    }

    
	// 바운스 액션 셋팅
	CCScaleTo *zoomOut = CCScaleTo::actionWithDuration(0.1, 1.2);
	CCScaleTo *zoomIn = CCScaleTo::actionWithDuration(0.1, 1.0);
	
	bounceAction = CCSequence::actions(zoomOut,zoomIn,NULL);
	bounceAction->retain();
	
	CCRotateBy *rotateR = CCRotateBy::actionWithDuration(0.1, 5);
	CCRotateBy *rotateL = CCRotateBy::actionWithDuration(0.1, -5);
	rotateAction = CCSequence::actions(rotateR,rotateL,NULL);
	rotateAction->retain();
	
	symbolDic = new CCMutableDictionary<std::string, CCSymbol*>();
	touchDic = new CCMutableDictionary<std::string, CCTouchArea*>();
 
	//CC_SYNTH_RETAIN 초기화
	bgm = NULL;
	bookDelegate = NULL;
	
	return true;
}

VVBookScene* VVPageBase::getBookDelegate()
{
	return bookDelegate;
}

void VVPageBase::setBookDelegate(VVBookScene* delegate)
{
	bookDelegate = delegate;
}

// Symbol의 name 속성을 key값으로 사용해서 딕셔너리에 대입
void VVPageBase::addSymbol(CCSymbol *symbol)
{
	string symName = symbol->getName();
	if (symName == "") 
		CCAssert(0,"Symbol cannot be added without name");
		
		
	if ( symbolDic->objectForKey(symName) != NULL )
		CCAssert(0,"Same symbol name already exist");
		
	// 심볼 추가
	symbolDic->setObject(symbol, symName);

	// 해당 심볼에대한 터치가 시작되었는지를 저장(초기값 = NO)
	isSymbolTouchBeganDic.insert(pair<std::string, bool>(symName,false));

}

// name에 해당하는 심볼 리턴
CCSymbol* VVPageBase::getSymbolWithName(string name)
{
	return symbolDic->objectForKey(name);
}

// Touch의 name 속성을 key값으로 사용해서 딕셔너리에 대입
void VVPageBase::addTouchArea(CCTouchArea *touchArea)
{
	string touchAreaName = touchArea->getName();
	if (touchAreaName == "") 
		CCAssert(0,"TouchArea cannot be added without name");
		
		
	if ( touchDic->objectForKey(touchAreaName) != NULL )
		CCAssert(0,"Same TouchArea name already exist");
		
	// 터치영역 추가
	touchDic->setObject(touchArea, touchAreaName);

}

// name에 해당하는 터치영역 리턴
CCTouchArea* VVPageBase::getTouchAreaWithName(string name)
{
	return touchDic->objectForKey(name);
}


// 좌상단 앵커포인트를 기준으로 함
//MOD: 한 터치포인트에 2개이상의 심볼이 있다면 심볼을 Array로만들어서 리턴해준다
CCSymbol* VVPageBase::getTouchedSymbol(CCTouch* touch)
{
	CCPoint location = touch->locationInView();
	CCPoint convertedLocation = CCDirector::sharedDirector()->convertToGL(location);
	

	if (symbolDic->count() > 0)
	{
		symbolDic->begin();
		while (1)
		{
			CCSymbol* symbol = symbolDic->next();
			if (symbol == NULL) 
				break;
			
			if( convertedLocation.x < mainLayer->getPosition().x +symbol->getPosition().x + symbol->getMaxX() && // X축 upper bound
			   convertedLocation.x > mainLayer->getPosition().x +symbol->getPosition().x + symbol->getMinX() && // X축 lower bound
			   
			   convertedLocation.y < mainLayer->getPosition().y +symbol->getPosition().y + symbol->getMaxY() && // Y축 upper bound
			   convertedLocation.y > mainLayer->getPosition().y +symbol->getPosition().y + symbol->getMinY() )  // Y축 lower bound
			{
				// 터치된 심볼목표 찾았다!
				//CCLog("Touched:%s",symbol->getName());
				return symbol;
			}
		}
	}
	
	// 발견된 심볼이 없으면 nil 리턴
	return NULL;
}	
void VVPageBase::pause()
{
    
}
void VVPageBase::play()
{
    
}
													

void VVPageBase::onPageStart()
{
	// 셋팅된 bgm을 재생
	if (bgm)
		SimpleAudioEngine::sharedEngine()->playBackgroundMusic(bgm->m_sString.c_str(), true);
}
void VVPageBase::setBackGroundImage(const char* name)
{
	CCSprite *sprite = CCSprite::spriteWithFile(name);
	sprite->setAnchorPoint(ccp(0,1));
	bgLayer->addChild(sprite);
}


void VVPageBase::onExit()
{
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	CCLayer::onExit();
}

VVPageBase::~VVPageBase()
{
	CC_SAFE_RELEASE_NULL(bounceAction);
	CC_SAFE_RELEASE_NULL(rotateAction);
	CC_SAFE_RELEASE_NULL(bgm);
	
	CC_SAFE_RELEASE_NULL(touchDic);
	CC_SAFE_RELEASE_NULL(symbolDic);
	CCLog("pagebase destroy");
}
