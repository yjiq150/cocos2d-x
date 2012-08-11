/*
 *  LCCrossMatchGame.cpp
 *  Collins_Dictionary_iphone
 *
 *  Created by 태호 이 on 11. 9. 2..
 *  Copyright 2011 Viscuit Creative. All rights reserved.
 *
 */

#include "LCCrossMatchGame.h"
#include "LCUtil.h"

USING_NS_CC;

enum {
	zBACKGROUND_SPRITE,
	zDOT_SPRITE,	
	zIMAGE_SPRITE,
	zTEXT_LABEL,
	zCANVAS_RENDER_TEXTURE
};

LCCrossMatchGame::LCCrossMatchGame():m_pLoadedSprites(NULL), m_pDotSprites(NULL), m_pFinishedDotSprites(NULL), m_pIndexArray(NULL), m_pImageArray(NULL), m_pTextArray(NULL)
{
	singleTouch = NULL;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	isTouchMoveCanBeCalled = false;
	isStartTouch = true;	
#endif
}
LCCrossMatchGame::~LCCrossMatchGame()
{
    if (m_pCanvas && (m_pCanvas->getRenderTextureName().length() > 0)) {
        CCTextureCache::sharedTextureCache()->unregisterRenderTexture(m_pCanvas);
    }
	CC_SAFE_RELEASE_NULL(m_pLoadedSprites);
	CC_SAFE_RELEASE_NULL(m_pDotSprites);
	CC_SAFE_RELEASE_NULL(m_pFinishedDotSprites);
	CC_SAFE_RELEASE_NULL(m_pIndexArray);
	CC_SAFE_RELEASE_NULL(m_pImageArray);
	CC_SAFE_RELEASE_NULL(m_pTextArray);
}

bool LCCrossMatchGame::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
		
	//initialize member variables
	m_pLoadedSprites = CCArray::array();
	m_pLoadedSprites->retain();
	
	m_pDotSprites = CCArray::array();
	m_pDotSprites->retain();
	
	m_pFinishedDotSprites = CCArray::array();
	m_pFinishedDotSprites->retain();
	
	m_pIndexArray = CCArray::array();
	m_pIndexArray->retain();
	
	m_pImageArray = CCArray::array();
	m_pImageArray->retain();
	
	m_pTextArray = CCArray::array();
	m_pTextArray->retain();
	
	m_pCanvas = CCRenderTexture::renderTextureWithWidthAndHeight(this->getContentSize().width, this->getContentSize().height);
	m_pCanvas->setPosition(ccp(this->getContentSize().width/2, this->getContentSize().height/2));
    m_pCanvas->setRenderTextureName("LCCrossMatchGame_cpp_m_pCanvas.png");
    CCTextureCache::sharedTextureCache()->registerRenderTexture(m_pCanvas);
	this->addChild(m_pCanvas, zCANVAS_RENDER_TEXTURE);
	
	m_pTouchedDotSprite = NULL;
	
	// Commented by YoungJae Kwon
	// 이부분을 runGame()으로 옮겼음
	// init할때 다 로딩해버리면 외부에서 셋팅값 변경 불가능하므로
	//this->loadIndex(m_pIndexArray);
//	m_iIndex = 0;
//	
//	this->loadSprites();
	
	this->setIsTouchEnabled(true);
    
    CCUserDefault::sharedUserDefault()->setBoolForKey("wasHalted", false);
	
	return true;
}

#pragma mark -
#pragma mark member functions

void LCCrossMatchGame::loadSprites()
{
	// check final game added by LEETAEHO on 111101
	// 두가지 체크 방법이 있음. 하나는 index의 끝 다른 하나는 배열이 비었을때, 일단은 인덱스로 체크
	if (m_iIndex >= m_pIndexArray->count()) {
		this->didFinishedGame();
		return;
	}
	
	// initialize
	for (int i=0; i<m_pLoadedSprites->count(); i++) 
	{
		CCNode* pNode = (CCNode*) m_pLoadedSprites->objectAtIndex(i);
		//pNode->removeFromParentAndCleanup(true);
		CCSequence* seq = CCSequence::actions(CCFadeOut::actionWithDuration(1.0f), 
											  CCCallFuncN::actionWithTarget(this, callfuncN_selector(LCCrossMatchGame::didSpriteAnimated)), 
											  NULL);
		pNode->runAction(seq);
	}
	m_pLoadedSprites->removeAllObjects();
	m_pCanvas->clear(0, 0, 0, 0);
	m_pDotSprites->removeAllObjects();
	m_pFinishedDotSprites->removeAllObjects();
	m_pImageArray->removeAllObjects();
	m_pTextArray->removeAllObjects();
	
	// load texts and images
	CCArray* textLabelArray = CCArray::array();
	CCArray* imageArray = CCArray::array();
	for (int i=0; i<this->getMaxNumberOfItems(); i++) 
	{
		// index 초과시 로딩 안함 added by LEETAEHO on 111101
		if (m_iIndex >= m_pIndexArray->count()) {
			CCLog("index was ended");
			break;
		}
		
		m_iIndex = (m_iIndex + m_pIndexArray->count()) % m_pIndexArray->count();
		
		// 이미지가 있는 단어만 가져오기
		CCSprite* imageSprite = this->getImageWithIndex(m_pIndexArray, m_iIndex);
		if (!imageSprite)
		{
			m_iIndex++;
			i--;
			continue;
		}
		imageSprite->setTag(m_iIndex);
		this->addChild(imageSprite, zIMAGE_SPRITE);
		imageArray->addObject(imageSprite);
		m_pLoadedSprites->addObject(imageSprite);
		
		CCLabelBMFont* textLabel = this->getTextLabelWithIndex(m_pIndexArray, m_iIndex);
		textLabel->setTag(m_iIndex);
		this->addChild(textLabel, zTEXT_LABEL);
		textLabelArray->addObject(textLabel);
		m_pLoadedSprites->addObject(textLabel);
		
		m_iIndex++;
	}
	
	// 배치
	this->arrangeComponents(textLabelArray, imageArray);
	
	//shuffle texts
	//this->shuffleNodes(textLabelArray);
	
	// add dots
	for (int i=0; i<imageArray->count(); i++) {
		CCSprite *imageSprite = (CCSprite *)imageArray->objectAtIndex(i);
		CCSprite *dotSprite = this->getDotSprite();
		dotSprite->setTag(imageSprite->getTag());
		dotSprite->setPosition(ccp(imageSprite->getPosition().x + this->getDotDistanceWithImage(), imageSprite->getPosition().y));
		this->addChild(dotSprite, zDOT_SPRITE);
		m_pDotSprites->addObject(dotSprite);
		m_pLoadedSprites->addObject(dotSprite);
	}
	
	for (int i=0; i<textLabelArray->count(); i++) {
		CCLabelAtlas *characterLabel = (CCLabelAtlas *)textLabelArray->objectAtIndex(i);
		CCSprite *dotSprite = this->getDotSprite();
		dotSprite->setTag(characterLabel->getTag());
		dotSprite->setPosition(ccp(characterLabel->getPosition().x + this->getDotDistanceWithTextLabel(), characterLabel->getPosition().y));
		this->addChild(dotSprite, zDOT_SPRITE);
		m_pDotSprites->addObject(dotSprite);
		m_pLoadedSprites->addObject(dotSprite);
	}
	
	// add arrays of images and texts
	m_pImageArray->addObjectsFromArray(imageArray);
	m_pTextArray->addObjectsFromArray(textLabelArray);
	
	
	// fade in animation
	for (int i=0; i < m_pLoadedSprites->count(); i++) 
	{
		CCSprite *loadedSprite = (CCSprite *)m_pLoadedSprites->objectAtIndex(i);
		loadedSprite->runAction(CCFadeIn::actionWithDuration(1.0f));
	} 
}


void LCCrossMatchGame::shuffleNodes(CCArray* nodeArray)
{
	CCArray *tempArray = CCArray::arrayWithArray(nodeArray);
	CCPoint *tempPosition = (CCPoint *)malloc(sizeof(CCPoint) * nodeArray->count());
	
	for (int i=0; i<nodeArray->count(); i++) {
		CCNode* node = (CCNode*)tempArray->randomObject();
		tempArray->removeObject(node);
		
		tempPosition[i] = node->getPosition();
	}
	
	for (int i=0; i<nodeArray->count(); i++) {
		CCNode* node = (CCNode*)nodeArray->objectAtIndex(i);
		node->setPosition(tempPosition[i]);
	}
	
	free(tempPosition);
}

void LCCrossMatchGame::drawLine(CCPoint startPoint, CCPoint endPoint)
{
	glLineWidth(5);
	glColor4f(1.0, 0.0, 0.0, 1.0);
	ccDrawLine(startPoint, endPoint);
}

CCSprite* LCCrossMatchGame::getBrushSprite()
{
	// brushSprite를 override 안했을 시에 dotsprite 사용
	return this->getDotSprite();
}

#pragma mark -
#pragma mark implement CCTouchDelegate
void LCCrossMatchGame::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	// 멀티터치 금지
	if (singleTouch != NULL) 
	{
		return;
	}
	
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	singleTouch = touch;
	
	prevTouchLocation = touch->locationInView( touch->view() );

	moveAmount = 0;
	
	m_pTouchedDotSprite = NULL;
	for (int i=0; i<m_pDotSprites->count(); i++) {
		CCSprite *dotSprite = (CCSprite *)m_pDotSprites->objectAtIndex(i);
		if (LCUtil::isTouchInside(dotSprite, pTouches)) {
			m_pTouchedDotSprite = dotSprite;
			break;
		}
	}
	
	// image touch 및 text label touch 구역 추가 on 11.10.17
	if (!m_pTouchedDotSprite) {
		for (int i=0; i<m_pImageArray->count(); i++) {
			CCSprite *imageSprite = (CCSprite *)m_pImageArray->objectAtIndex(i);
			if (LCUtil::isTouchInside(imageSprite, pTouches)) {
				// dotArray에 이미지 부분부터 추가되므로
				for (int i=0; i<m_pDotSprites->count(); i++) {
					CCSprite *dotSprite = (CCSprite *)m_pDotSprites->objectAtIndex(i);
					if (dotSprite->getTag() == imageSprite->getTag()) {
						m_pTouchedDotSprite = dotSprite;
						break;
					}
				}
			}
		}
	}
	
	if (!m_pTouchedDotSprite) {
		for (int i=0; i<m_pTextArray->count(); i++) {
			CCLabelTTF *textLabel = (CCLabelTTF*)m_pTextArray->objectAtIndex(i);
			if (LCUtil::isTouchInside(textLabel, pTouches)) {
				// dotArray에 이미지 부분부터 추가되므로 뒤에서부터 탐색
				for (int i=m_pDotSprites->count()-1; i>=0; i--) {
					CCSprite *dotSprite = (CCSprite *)m_pDotSprites->objectAtIndex(i);
					if (dotSprite->getTag() == textLabel->getTag()) {
						m_pTouchedDotSprite = dotSprite;
						break;
					}
				}
			}
		}
	}
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
void LCCrossMatchGame::touchMoveSuppressor(ccTime dt)
{
	isTouchMoveCanBeCalled = true;
	this->unschedule(schedule_selector(LCCrossMatchGame::touchMoveSuppressor));
}
#endif

void LCCrossMatchGame::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	
	if (m_pTouchedDotSprite) {
		CCTouch *touch = (CCTouch *) pTouches->anyObject();
		if (touch != singleTouch)
			return;
		
		CCPoint touchLocation = touch->locationInView( touch->view() );	
		CCPoint touchPointToGL = CCDirector::sharedDirector()->convertToGL(touchLocation);

		int distance = ccpDistance(prevTouchLocation,touchLocation);

				
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
		startTouchLocation = prevTouchLocation;
		moveAmount = distance;
		prevTouchLocation = touchLocation;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
		/*
		prevTouchLocation = touchLocation;
		this->schedule(schedule_selector(LCCrossMatchGame::touchMoveSuppressor));
		if (isStartTouch) 
		{
			startTouchLocation = touchLocation;
			isStartTouch = false;
		}

		if( !isTouchMoveCanBeCalled )
		{
			moveAmount += distance;
			return;
		}
		isTouchMoveCanBeCalled = false;
		isStartTouch = true;
		//CCLog("move:%d",moveAmount);
		 */
		
		//MOD: 예전에 안됐었는데 갑자기 잘되네..이상하다
		startTouchLocation = prevTouchLocation;
		moveAmount = distance;
		prevTouchLocation = touchLocation;
#endif
		
		
		if (moveAmount >= 1)
		{
			/*
			CCPoint start = touch->locationInView(touch->view());
			start = CCDirector::sharedDirector()->convertToGL(start);
			CCPoint end = touch->previousLocationInView(touch->view());
			end = CCDirector::sharedDirector()->convertToGL(end);
			*/
			CCPoint start = touchPointToGL;
			CCPoint end = CCDirector::sharedDirector()->convertToGL(startTouchLocation);
			m_pCanvas->begin();
			
			int d = (int)moveAmount;
			CCSprite* brushSprite = this->getBrushSprite();
			brushSprite->setPosition(touchPointToGL);
			
			for (int i = 0; i < d; i++)
			{
				float difx = end.x - start.x;
				float dify = end.y - start.y;
				float delta = (float)i / moveAmount;
				brushSprite->setPosition(ccp(start.x + (difx * delta)
									   , start.y + (dify * delta)
									   ) );
				// Call visit to draw the brush, don't call draw..
				brushSprite->visit();
			}
			
#if CC_ENABLE_CACHE_TEXTTURE_DATA
			m_pCanvas->end(false);
#else
            m_pCanvas->end();
#endif
			moveAmount = 0;
		}
		/* 손을 따라다니면서 직선을 잇는 부분
		m_pCanvas->clear(0, 0, 0, 0);
		m_pCanvas->begin();
		//draw finished lines
		for (int i=0; i<m_pFinishedDotSprites->count(); i+=2) {
			CCSprite *dotSprite1 = (CCSprite *)m_pFinishedDotSprites->objectAtIndex(i);
			CCSprite *dotSprite2 = (CCSprite *)m_pFinishedDotSprites->objectAtIndex(i+1);
			
			this->drawLine(dotSprite1->getPosition(), dotSprite2->getPosition());	
		}
		//draw touched point line
		
		this->drawLine(m_pTouchedDotSprite->getPosition(), touchPointToGL );
		m_pCanvas->end();
		 
		 */
	}
}
void LCCrossMatchGame::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	if (m_pTouchedDotSprite) {
		CCSprite *matchedDotSprite = NULL;
		bool isTouchedIncorrectly = false;
		
		// 다른 점을 터치 했는지 체크
		for (int i=0; i<m_pDotSprites->count(); i++) {
			CCSprite *dotSprite = (CCSprite *)m_pDotSprites->objectAtIndex(i);
			if (m_pTouchedDotSprite == dotSprite) {
				continue;
			}
			if (LCUtil::isTouchInside(dotSprite, pTouches)) {
				//move dots from dot array to finished dot array
				if (dotSprite->getTag() == m_pTouchedDotSprite->getTag()) {
					matchedDotSprite = dotSprite;
				}else {
					isTouchedIncorrectly = true;
				}
				
				break;
			}
		}
		
		// image touch 및 text label touch 구역 추가 on 11.10.17
		if (!matchedDotSprite && !isTouchedIncorrectly) {
			int index = m_pDotSprites->indexOfObject(m_pTouchedDotSprite);
			
			if (index < m_pDotSprites->count() / 2) {
				// 앞쪽에 있는 것은 image 부분에 있는 점 => text부분 터치 체크
				for (int i=0; i<m_pTextArray->count(); i++) {
					CCLabelTTF *textLabel = (CCLabelTTF*)m_pTextArray->objectAtIndex(i);
					if (LCUtil::isTouchInside(textLabel, pTouches)) {
						if (textLabel->getTag() == m_pTouchedDotSprite->getTag()) {
							// dotArray에 이미지 부분부터 추가되므로 뒤에서부터 탐색
							for (int i=m_pDotSprites->count()-1; i>=0; i--) {
								CCSprite *dotSprite = (CCSprite *)m_pDotSprites->objectAtIndex(i);
								if (dotSprite->getTag() == textLabel->getTag()) {
									matchedDotSprite = dotSprite;
									break;
								}
							}
						}else {
							isTouchedIncorrectly = true;
						}
						
						break;
					}
				}
			}
			else {
				// 뒤쪽에 있는 것은 text부분에 있는 점 => image 부분 터치 체크
				for (int i=0; i<m_pImageArray->count(); i++) {
					CCSprite *imageSprite = (CCSprite *)m_pImageArray->objectAtIndex(i);
					if (LCUtil::isTouchInside(imageSprite, pTouches)) {
						if (imageSprite->getTag() == m_pTouchedDotSprite->getTag()) {
							// dotArray에 이미지 부분부터 추가되므로
							for (int i=0; i<m_pDotSprites->count(); i++) {
								CCSprite *dotSprite = (CCSprite *)m_pDotSprites->objectAtIndex(i);
								if (dotSprite->getTag() == imageSprite->getTag()) {
									matchedDotSprite = dotSprite;
									break;
								}
							}
						}else {
							isTouchedIncorrectly = true;
						}
					}
				}
			}
		}
		
		// 올바르게 터치된 점이 있다면
		if (matchedDotSprite) {
			m_pFinishedDotSprites->addObject(m_pTouchedDotSprite);
			m_pFinishedDotSprites->addObject(matchedDotSprite);
			m_pDotSprites->removeObject(m_pTouchedDotSprite);
			m_pDotSprites->removeObject(matchedDotSprite);
			
			// 터치된 스프라이트 찾기
			CCSprite* matchedSprite = NULL;
			for (int i=0; i<m_pImageArray->count(); i++) {
				CCSprite* sprite = (CCSprite*)m_pImageArray->objectAtIndex(i);
				if (sprite->getTag() == matchedDotSprite->getTag()) {
					matchedSprite = sprite;
					break;
				}
			}
			
			this->didMatchWord();
			this->didMatchWord(matchedSprite, m_pIndexArray, matchedDotSprite->getTag());
			drawMatchedLines();
			
			if (m_pDotSprites->count() == 0) {
				//CCLog("finished");
				this->didFinishedStage();
				//load
				CCSequence* seq = CCSequence::actions(CCDelayTime::actionWithDuration(this->delayTimeOfChangeWord(m_pIndexArray, m_iIndex)),
													  CCCallFunc::actionWithTarget(this, callfunc_selector(LCCrossMatchGame::didDelayedChangeWord)) ,NULL);
				this->runAction(seq);
				//this->loadSprites();
			}
			
			m_pTouchedDotSprite = NULL;			
			singleTouch = NULL;
			return;
			
		}
		
		// 만일 잘못된 연결을 했을경우
		if (isTouchedIncorrectly) {
			this->didMissMatchWord();
			drawMatchedLines();
			
			m_pTouchedDotSprite = NULL;			
			singleTouch = NULL;
			return;
		}
		
		
		
	} 
	
	
	drawMatchedLines();
		
	// 점을 터치 한 것이 아니라 다른 것을 터치했을경우
	// 이미지 터치 체크
	CCTouch* touch = (CCTouch*)pTouches->anyObject();
	CCPoint touchLocation = touch->locationInView( touch->view() );
	touchLocation = CCDirector::sharedDirector()->convertToGL(touchLocation);
	
	for (int i=0; i<m_pImageArray->count(); i++) {
		CCSprite* imageSprite = (CCSprite*) m_pImageArray->objectAtIndex(i);
		
		// added by YoungJae Kwon
		// 태호씨 LCUtil의 isTouchInside depricate 시키세요~ 대신 CCRectContainsPoint랑 convetToNodeSpace 섞어서 쓰시길
		//			if (LCUtil::isTouchInside(imageSprite, pTouches)) {
		if( CCRect::CCRectContainsPoint(imageSprite->getRect(), imageSprite->getParent()->convertToNodeSpace(touchLocation)) )
		{				
			this->didWordImagedTouched(imageSprite, m_pIndexArray, imageSprite->getTag());
		}
	}
	
	m_pTouchedDotSprite = NULL;
	singleTouch = NULL;

}
void LCCrossMatchGame::drawMatchedLines()
{
	m_pCanvas->clear(0, 0, 0, 0);
	m_pCanvas->begin();
	
	//draw finished lines
	for (int i=0; i<m_pFinishedDotSprites->count(); i+=2) {
		CCSprite *dotSprite1 = (CCSprite *)m_pFinishedDotSprites->objectAtIndex(i);
		CCSprite *dotSprite2 = (CCSprite *)m_pFinishedDotSprites->objectAtIndex(i+1);
		
		this->drawLine(dotSprite1->getPosition(), dotSprite2->getPosition());	
	}
	m_pCanvas->end();
	
	
	
}
void LCCrossMatchGame::didSpriteAnimated(CCNode* node)
{
	node->removeFromParentAndCleanup(true);
}

void LCCrossMatchGame::didDelayedChangeWord(){this->loadSprites();}

void LCCrossMatchGame::didHaltedDevice(){
    CCLOG("LCCrossMatchGame::didHaltedDevice");
}

#pragma mark -
#pragma mark Event Handler
void LCCrossMatchGame::didMatchWord(){}
void LCCrossMatchGame::didMissMatchWord(){}
void LCCrossMatchGame::didFinishedStage(){}
void LCCrossMatchGame::didMatchWord(CCSprite* matchedSprite, CCArray* wordIdxArray, int index){}
void LCCrossMatchGame::didWordImagedTouched(CCSprite* touchedSprite, CCArray* wordIdxArray, int touchedIdx){}
ccTime LCCrossMatchGame::delayTimeOfChangeWord(CCArray* wordIdxArray, int index){return 0;}
void LCCrossMatchGame::didFinishedGame(){}
