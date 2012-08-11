/*
 *  LCWordPuzzleGame.cpp
 *  Collins_Dictionary_iphone
 *
 *  Created by 태호 이 on 11. 9. 2..
 *  Copyright 2011 Viscuit Creative. All rights reserved.
 *
 */

#include "LCWordPuzzleGame.h"
#include "LCUtil.h"

USING_NS_CC;


// modified by YoungJae Kwon, 순서 변경, image를 제일 높게
enum {
	zBACKGROUND_SPRITE,
	zBACKGROUND_SPRITE2,
	zBLOCK_SPRITE,
	zCHARACTER_SPRITE,
	zIMAGE_SPRITE,
};


LCWordPuzzleGame::LCWordPuzzleGame():m_pLoadedSprites(NULL), m_pBlockSprites(NULL), m_pCharacterSprites(NULL), m_pFinishedCharacterSprites(NULL), m_pCurrentWord(NULL), m_pIndexArray(NULL)
{
	m_pCurrentWordImage = NULL;
}

LCWordPuzzleGame::~LCWordPuzzleGame()
{
	CC_SAFE_RELEASE_NULL(m_pLoadedSprites);
	CC_SAFE_RELEASE_NULL(m_pBlockSprites);
	CC_SAFE_RELEASE_NULL(m_pCharacterSprites);
	CC_SAFE_RELEASE_NULL(m_pFinishedCharacterSprites);
	CC_SAFE_RELEASE_NULL(m_pIndexArray);
	CC_SAFE_FREE(m_pCurrentWord);
}

bool LCWordPuzzleGame::init()
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
	
	m_pBlockSprites = CCArray::array();
	m_pBlockSprites->retain();
	
	m_pCharacterSprites = CCArray::array();
	m_pCharacterSprites->retain();
	
	m_pFinishedCharacterSprites = CCArray::array();
	m_pFinishedCharacterSprites->retain();
	
	m_pIndexArray = CCArray::array();
	m_pIndexArray->retain();
	
	m_pCurrentWord = (char*)malloc(sizeof(char) * 50);
	
	this->setIsTouchEnabled(true);
	
	this->loadIndex(m_pIndexArray);
	m_iIndex = 0;
	this->loadSprites();
	
	return true;
}

#pragma mark -
#pragma mark protected member functions

#pragma mark -
#pragma mark Load Datas

void LCWordPuzzleGame::didSpriteAnimated(CCNode* node)
{
	node->removeFromParentAndCleanup(true);
}

// loadSprites의경우 인덱스 0부터 로딩 시작 불가능하므로 초기에는 이 함수를 호출할것
void LCWordPuzzleGame::loadInitialSprites()
{	
	// added by YoungJae Kwon
	if (m_pIndexArray->count() == 0) 
	{
		// loadIndex 델리게이션을 구현안했을경우 스프라이트 로딩 안함
		return;
	}
	m_pCurrentWordImage = NULL;
		
	// initialize
	m_iNumberOfFinishedCharacter = 0;
	m_iIndexOfTargetCharacter = 0;
	m_bIsFinished = false;
	
	// load data with index
	m_iIndex = 0;
	strcpy(m_pCurrentWord, this->getWordWithIndex(m_pIndexArray, m_iIndex));		
	CCSprite* imageSprite = this->getImageWithIndex(m_pIndexArray, m_iIndex);

	// added by YOungJae Kwon
	m_pCurrentWordImage = imageSprite;
	this->addChild(imageSprite, zIMAGE_SPRITE);
	m_pLoadedSprites->addObject(imageSprite);
	
	for (int i=0; i < strlen(m_pCurrentWord); i++) 
	{
		CCSprite *blockSprite = this->getBoxImage();
		this->addChild(blockSprite, zBLOCK_SPRITE);
		m_pBlockSprites->addObject(blockSprite);
		m_pLoadedSprites->addObject(blockSprite);
		
		LCWordPuzzleAlphabet* alphabetSprite = this->getAlphabetImage(m_pCurrentWord[i]);
		alphabetSprite->setAlphabet(m_pCurrentWord[i]);
		this->addChild(alphabetSprite, zCHARACTER_SPRITE);
		m_pCharacterSprites->addObject(alphabetSprite);
		m_pLoadedSprites->addObject(alphabetSprite);
	}
	
	this->arrangeComponents(m_pCurrentWord, m_pBlockSprites, m_pCharacterSprites);
	
	// suffle alphabets
	//this->shuffleSprites(m_pCharacterSprites);
	
	// fade in animation
	for (int i=0; i < m_pLoadedSprites->count(); i++) 
	{
		CCSprite *loadedSprite = (CCSprite *)m_pLoadedSprites->objectAtIndex(i);
		loadedSprite->runAction(CCFadeIn::actionWithDuration(0.5f));
	}
}



void LCWordPuzzleGame::loadSprites()
{	
	// added by YoungJae Kwon
	if (m_pIndexArray->count() == 0) 
	{
		// loadIndex 델리게이션을 구현안했을경우 스프라이트 로딩 안함
		return;
	}
	m_pCurrentWordImage = NULL;
	
	//remove all sprites to init
	for (int i=0; i < m_pLoadedSprites->count(); i++) 
	{
		CCSprite *loadedSprite = (CCSprite *)m_pLoadedSprites->objectAtIndex(i);
		CCSequence* seq = CCSequence::actions(CCFadeOut::actionWithDuration(0.5f), 
											  CCCallFuncN::actionWithTarget(this, callfuncN_selector(LCWordPuzzleGame::didSpriteAnimated)), 
											  NULL);
		loadedSprite->runAction(seq);
		//loadedSprite->removeFromParentAndCleanup(true);
	}
	m_pLoadedSprites->removeAllObjects();
	m_pBlockSprites->removeAllObjects();
	m_pCharacterSprites->removeAllObjects();
	m_pFinishedCharacterSprites->removeAllObjects();
	
	// initialize
	m_iNumberOfFinishedCharacter = 0;
	m_iIndexOfTargetCharacter = 0;
	m_bIsFinished = false;
	
	// load data with index
	// 그림있는 단어만 갖고오기 체크부분
	while (1) 
	{
		m_iIndex = (m_iIndex + 1 + m_pIndexArray->count()) % m_pIndexArray->count();
		strcpy(m_pCurrentWord, this->getWordWithIndex(m_pIndexArray, m_iIndex));

		CCSprite* imageSprite = this->getImageWithIndex(m_pIndexArray, m_iIndex);
		if (!imageSprite)
		{
			m_iIndex++;
			continue;
		}
		// added by YOungJae Kwon
		m_pCurrentWordImage = imageSprite;
		this->addChild(imageSprite, zIMAGE_SPRITE);
		m_pLoadedSprites->addObject(imageSprite);
		break;
	}
	
	for (int i=0; i < strlen(m_pCurrentWord); i++) 
	{
		CCSprite *blockSprite = this->getBoxImage();
		this->addChild(blockSprite, zBLOCK_SPRITE);
		m_pBlockSprites->addObject(blockSprite);
		m_pLoadedSprites->addObject(blockSprite);
		
		LCWordPuzzleAlphabet* alphabetSprite = this->getAlphabetImage(m_pCurrentWord[i]);
		alphabetSprite->setAlphabet(m_pCurrentWord[i]);
		this->addChild(alphabetSprite, zCHARACTER_SPRITE);
		m_pCharacterSprites->addObject(alphabetSprite);
		m_pLoadedSprites->addObject(alphabetSprite);
	}
	
	this->arrangeComponents(m_pCurrentWord, m_pBlockSprites, m_pCharacterSprites);
	
	// suffle alphabets
	//this->shuffleSprites(m_pCharacterSprites);
	
	// fade in animation
	for (int i=0; i < m_pLoadedSprites->count(); i++) 
	{
		CCSprite *loadedSprite = (CCSprite *)m_pLoadedSprites->objectAtIndex(i);
		loadedSprite->runAction(CCFadeIn::actionWithDuration(0.5f));
	}
}

void LCWordPuzzleGame::shuffleSprites(CCArray* alphabetSpriteArray)
{
	CCArray *tempArray = CCArray::arrayWithArray(alphabetSpriteArray);
	CCPoint *tempPosition = (CCPoint *)malloc(sizeof(CCPoint) * alphabetSpriteArray->count());
	
	for (int i=0; i<alphabetSpriteArray->count(); i++) {
		LCWordPuzzleAlphabet* alphabetSprite = (LCWordPuzzleAlphabet*)tempArray->randomObject();
		tempArray->removeObject(alphabetSprite);
		
		tempPosition[i] = alphabetSprite->getPosition();
	}
	
	for (int i=0; i<alphabetSpriteArray->count(); i++) {
		LCWordPuzzleAlphabet* alphabetSprite = (LCWordPuzzleAlphabet*)m_pCharacterSprites->objectAtIndex(i);
		alphabetSprite->setPosition(tempPosition[i]);
		alphabetSprite->setOriginalPosition(tempPosition[i]);
	}
	
	free(tempPosition);
}

#pragma mark -
#pragma mark Event Handler

//////// deprecated in 11.10.07 by LEETAEHO
void LCWordPuzzleGame::didCorrectCharacterSelected(){}
void LCWordPuzzleGame::didCorrectCharacterAnimated(){}
void LCWordPuzzleGame::didIncorrectCharacterSelected(){}
void LCWordPuzzleGame::didIncorrectCharacterAnimated(){}
void LCWordPuzzleGame::didFinishedWord(){}

//////// modified in 11.10.07 by LEETAEHO
void LCWordPuzzleGame::didCorrectCharacterSelected(CCArray* wordIdxArray, int index){}
void LCWordPuzzleGame::didCorrectCharacterAnimated(CCArray* wordIdxArray, int index){}
void LCWordPuzzleGame::didIncorrectCharacterSelected(CCArray* wordIdxArray, int index){}
void LCWordPuzzleGame::didIncorrectCharacterMetBlock(CCArray* wordIdxArray, int index){}
void LCWordPuzzleGame::didIncorrectCharacterAnimated(CCArray* wordIdxArray, int index){}
void LCWordPuzzleGame::didFinishedWord(CCArray* wordIdxArray, int index){}
ccTime LCWordPuzzleGame::delayTimeOfChangeWord(CCArray* wordIdxArray, int index){return 0;}
void LCWordPuzzleGame::didDelayedChangeWord(){this->loadSprites();}

// added by YoungJae Kwon
void LCWordPuzzleGame::didWordImageTouched(CCSprite* touchedSprite){}
void LCWordPuzzleGame::didIndexArrayReset(){}

#pragma mark -
#pragma mark Animation Callback
void LCWordPuzzleGame::didIncorrectCharacterMetBlockAnimationFinished()
{
	this->didIncorrectCharacterMetBlock(m_pIndexArray, m_iIndex);
}
void LCWordPuzzleGame::didIncorrectCharacterAnimationFinished()
{
	this->didIncorrectCharacterAnimated(); // deprecated
	this->didIncorrectCharacterAnimated(m_pIndexArray, m_iIndex);
}
void LCWordPuzzleGame::didCorrectCharacterAnimationFinished()
{
	this->didCorrectCharacterAnimated(); // deprecated
	this->didCorrectCharacterAnimated(m_pIndexArray, m_iIndex);
	
	m_iNumberOfFinishedCharacter ++;
	if (m_iNumberOfFinishedCharacter >= strlen(m_pCurrentWord) && !m_bIsFinished) {
		// ignore if game finished
		m_bIsFinished = true;
		this->didFinishedWord(); // deprecated
		this->didFinishedWord(m_pIndexArray, m_iIndex);
					
		// load next word
		CCSequence* seq = CCSequence::actions(CCDelayTime::actionWithDuration(this->delayTimeOfChangeWord(m_pIndexArray, m_iIndex)),
											  CCCallFunc::actionWithTarget(this, callfunc_selector(LCWordPuzzleGame::didDelayedChangeWord)) ,NULL);
		this->runAction(seq);
		
		return;
	}
}

#pragma mark -
#pragma mark implement CCTouchDispatcher
void LCWordPuzzleGame::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	for (int i=0; i<m_pCharacterSprites->count(); i++) {
		LCWordPuzzleAlphabet* alphabetSprite = (LCWordPuzzleAlphabet*)m_pCharacterSprites->objectAtIndex(i);
		if (LCUtil::isTouchInside(alphabetSprite, pTouches)) {
			CCScaleTo *scaleUp = CCScaleTo::actionWithDuration(0.1f, 1.2f);
			CCScaleTo *scaleDown = CCScaleTo::actionWithDuration(0.1f, 1.0f);
			alphabetSprite->runAction(CCSequence::actions(scaleUp,scaleDown,NULL));
		}
	}
	for (int i=0; i<m_pFinishedCharacterSprites->count(); i++) {
		LCWordPuzzleAlphabet* alphabetSprite = (LCWordPuzzleAlphabet*)m_pFinishedCharacterSprites->objectAtIndex(i);
		if (LCUtil::isTouchInside(alphabetSprite, pTouches)) {
			CCScaleTo *scaleUp = CCScaleTo::actionWithDuration(0.1f, 1.2f);
			CCScaleTo *scaleDown = CCScaleTo::actionWithDuration(0.1f, 1.0f);
			alphabetSprite->runAction(CCSequence::actions(scaleUp,scaleDown,NULL));
		}
	}
}
void LCWordPuzzleGame::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
}
void LCWordPuzzleGame::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	if (m_iIndexOfTargetCharacter + 1 > strlen(m_pCurrentWord)) {
		// ignore if game finished
		return;
	}
	for (int i=0; i<m_pCharacterSprites->count(); i++) {
		LCWordPuzzleAlphabet* alphabetSprite = (LCWordPuzzleAlphabet*)m_pCharacterSprites->objectAtIndex(i);
		if (LCUtil::isTouchInside(alphabetSprite, pTouches)) {
			char touchedCharacter = alphabetSprite->getAlphabet();
			char targetCharacter = *(m_pCurrentWord + m_iIndexOfTargetCharacter);
			if (targetCharacter == touchedCharacter) {
				// if touched correct character
				this->didCorrectCharacterSelected(); // deprecated
				this->didCorrectCharacterSelected(m_pIndexArray, m_iIndex);
				
				m_pFinishedCharacterSprites->addObject(alphabetSprite);
				m_pCharacterSprites->removeObject(alphabetSprite);
				CCMoveTo *moveToBlock = CCMoveTo::actionWithDuration(0.5, ((CCLabelAtlas *)m_pBlockSprites->objectAtIndex(m_iIndexOfTargetCharacter))->getPosition());
				alphabetSprite->runAction(CCSequence::actions(moveToBlock, CCCallFunc::actionWithTarget(this, callfunc_selector(LCWordPuzzleGame::didCorrectCharacterAnimationFinished)), NULL));
				m_iIndexOfTargetCharacter++;
				break;
			} else {
				// if touched incorrect character
				this->didIncorrectCharacterSelected(); // deprecated
				this->didIncorrectCharacterSelected(m_pIndexArray, m_iIndex);
				CCMoveTo *moveToBlock = CCMoveTo::actionWithDuration(0.5, ((CCSprite *)m_pBlockSprites->objectAtIndex(m_iIndexOfTargetCharacter))->getPosition());
				CCMoveTo *moveToOrigin = CCMoveTo::actionWithDuration(0.5, alphabetSprite->getOriginalPosition());
				alphabetSprite->runAction(CCSequence::actions(moveToBlock, 
															  CCCallFunc::actionWithTarget(this, callfunc_selector(LCWordPuzzleGame::didIncorrectCharacterMetBlockAnimationFinished)),
															  moveToOrigin, 
															  CCCallFunc::actionWithTarget(this, callfunc_selector(LCWordPuzzleGame::didIncorrectCharacterAnimationFinished)), 
															  NULL));
			}
		}
	}
	
	//added by YOungJae Kwon
	// 워드이미지가 터치된걸 체크
	if (m_pCurrentWordImage == NULL)
		return;
	
	CCTouch* touch = (CCTouch*)pTouches->anyObject();	
	if ( CCRect::CCRectContainsPoint(m_pCurrentWordImage->getRect(),m_pCurrentWordImage->getParent()->convertTouchToNodeSpace(touch)) ) 
	{
		this->didWordImageTouched(m_pCurrentWordImage);
	}
	
}


CCArray* LCWordPuzzleGame::getIndexArray()
{
	return m_pIndexArray;
}

void LCWordPuzzleGame::setIndexArray(CCArray* idxList)
{
	// 기존 리스트 존재시 해제 후에 새 리스트 리테인
	CC_SAFE_RELEASE(m_pIndexArray);
	
	m_pIndexArray = idxList;
	idxList->retain();
	
	this->didIndexArrayReset();
	
}
//////////////////////////////////////////
// WorldPuzzleAlphabet
//////////////////////////////////////////

CC_PROPERTY_SYNTHESIZE(char, m_cAlphabet, Alphabet, LCWordPuzzleAlphabet);
CC_PROPERTY_SYNTHESIZE(CCPoint, m_tOriginalPosition, OriginalPosition, LCWordPuzzleAlphabet);

#pragma mark -
#pragma mark Constructor and Destructor
LCWordPuzzleAlphabet::LCWordPuzzleAlphabet()
{}

LCWordPuzzleAlphabet::~LCWordPuzzleAlphabet()
{
}