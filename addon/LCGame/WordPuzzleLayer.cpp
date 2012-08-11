/*
 *  WordPuzzleLayer.cpp
 *  Collins_Dictionary_iphone
 *
 *  Created by 태호 이 on 11. 9. 2..
 *  Copyright 2011 Viscuit Creative. All rights reserved.
 *
 */

#include "VVStringHelper.h"
#include "WordPuzzleLayer.h"

#include "DBController.h"
#include "LCUtil.h"

#include "WordPuzzleCommon.h"

#if (defined PD_IPHONE) || (defined ANDROID_PHONE)
#include "config.h"
#endif

#define PROGRESS_FONT  "mplus-2p-heavy.ttf" 
#if (defined PD_IPHONE)
#define PROGRESS_SIZE 13
#elif  (defined ANDROID_PHONE)
#define PROGRESS_SIZE 26
#else
#define PROGRESS_SIZE 20
#endif

#define TOTAL_COL ccc3(0, 91, 62)
#define CURRENT_COL ccc3(2, 168, 92)

#define TAG_COMBO 1234

USING_NS_CC;

using namespace CocosDenshion;

WordPuzzleLayer::WordPuzzleLayer()
{
	sprImage = NULL;
	isActionOngoing = false;
	progressCurLabel = NULL;
	progressTotalLabel = NULL;
	aniAction = NULL;
	isAllowingNext = true;
	maxNumberOfWords = 0;
	combo = 0;
	
	report.clearTime = 0;
	report.givenTime = 0;
	report.maxCombo = 0;
	report.score = 0;
	report.incorrectTouch = 0;
	report.timeBonusMultiplier = 0;
	report.isWinner = false;
	report.isSkipExist = false;
	
}
WordPuzzleLayer::~WordPuzzleLayer()
{
	CC_SAFE_RELEASE(aniAction);
}

bool WordPuzzleLayer::init()
{
	if (!LCWordPuzzleGame::init()) {
		return false;
	}
	
	winSize = CCDirector::sharedDirector()->getWinSize();
#ifdef ANDROID_GALAXY_NOTE
	winSize.height -= 48;
#endif	
	m_iScore = 0;
		

	
	
	return true;
}

WordPuzzleScoreReport WordPuzzleLayer::getScoreReport()
{
	timer->end();
	report.clearTime = timer->getProgress();
	report.score = m_iScore;
	return report;
}
void WordPuzzleLayer::onEnter()
{
	CCLayer::onEnter();
	timer->setProgress(0);
	timer->setMaxTime(m_pIndexArray->count() * TIME_PER_WORD);
	timer->setWarningTime(timer->getMaxTime()*0.7);
	timer->start();
	
	report.givenTime = timer->getMaxTime();
}
void WordPuzzleLayer::onExit()
{
	// 꼭 onExit에서 호출해줘야된다.
	// 이녀석에 callbackFuction이 걸려있기때문에, 얘가 릴리즈 안되면 파괴자도 호출이 안되므로 
	// 이녀석을 파괴자에서 호출해봐야 무의미하다.
	CC_SAFE_RELEASE(aniSeqAction);
	CCLayer::onExit();
}
#pragma mark -
#pragma mark implement TimeBar Delegate
void WordPuzzleLayer::didTimeUp(TimeBar* aTimeBar)
{
#if (defined PD_IPHONE) || (defined ANDROID_PHONE)
	CCSprite* sprTimeUp = CCSprite::spriteWithFile(__CONFIG_IMAGE_PATH_GAME__("effect/game_timeup.png"));
#else
	CCSprite* sprTimeUp = CCSprite::spriteWithFile("game/effect/game_timeup.png");
#endif
	sprTimeUp->setPosition( warningLabelPosition );
	addChild(sprTimeUp,10000);
	
	CCEaseExponentialOut * fadeIn = CCEaseExponentialOut::actionWithAction( CCActionTween::actionWithDuration(0.5,"opacity",0,255) );
	CCEaseExponentialOut * fadeout = CCEaseExponentialOut::actionWithAction( CCActionTween::actionWithDuration(0.3,"opacity",255,0) );
	CCCallFuncN* callfunc = CCCallFuncN::actionWithTarget(this, callfuncN_selector(WordPuzzleLayer::onTimeUpAniDone));
	sprTimeUp->runAction(CCSequence::actions(fadeIn,CCDelayTime::actionWithDuration(0.8),fadeout,callfunc, NULL));

	//전부 터치 막고
	SimpleAudioEngine::sharedEngine()->playEffect("sound/time_warning.mp3");

}
void WordPuzzleLayer::didTimeWarningUp(TimeBar* aTimeBar)
{
#if (defined PD_IPHONE) || (defined ANDROID_PHONE)
	CCSprite* sprWarn = CCSprite::spriteWithFile(__CONFIG_IMAGE_PATH_GAME__("effect/game_warning.png"));
#else
	CCSprite* sprWarn = CCSprite::spriteWithFile("game/effect/game_warning.png");
#endif
	sprWarn->setPosition( warningLabelPosition );
	addChild(sprWarn,10000);
	
	CCEaseExponentialOut * fadeIn = CCEaseExponentialOut::actionWithAction( CCActionTween::actionWithDuration(1.5,"opacity",0,255) );
	CCEaseExponentialOut * fadeout = CCEaseExponentialOut::actionWithAction( CCActionTween::actionWithDuration(1.5,"opacity",255,0) );
	CCCallFuncN* callfunc = CCCallFuncN::actionWithTarget(this, callfuncN_selector(WordPuzzleLayer::onPointLabelFadeOut));
	sprWarn->runAction(CCSequence::actions(fadeIn,CCDelayTime::actionWithDuration(0.8),fadeout,callfunc, NULL));

	SimpleAudioEngine::sharedEngine()->playEffect("sound/time_warning.mp3");
}

#pragma mark -
#pragma mark implement LCWordPuzzleGame

void WordPuzzleLayer::loadIndex(CCArray* wordIdxArray)
{
	// 외부에서 단어 리스트를 입력할경우 아무동작 하지 않음
}

const char* WordPuzzleLayer::getWordWithIndex(CCArray* wordIdxArray, int index)
{
	CCString* idxString = (CCString*)wordIdxArray->objectAtIndex(index);
	// 태호씨 워드스키마를 DBController로 대체
	DBController* wordSchema = DBController::sharedDB();
	char buf[100];
	sprintf(buf, "WHERE words.idx=%s", idxString->m_sString.c_str());
#if (defined PD_IPHONE) || (defined ANDROID_PHONE)    
	wordSchema->queryWithWhereStatementDBWordsSchema(buf);
	if (wordSchema->fetchDBWordsSchema()) 
#else
    wordSchema->queryWithWhereStatement(buf);
	if (wordSchema->fetch()) 
#endif
	{
		// handle data from database record
		VVStringHelper::toLower(wordSchema->r_s_headword);
		return wordSchema->r_s_headword.c_str();
	}
	
	return NULL;
}

CCSprite* WordPuzzleLayer::getImageWithIndex(CCArray* wordIdxArray, int index)
{
	CCString* idxString = (CCString*)wordIdxArray->objectAtIndex(index);
	// 태호씨 워드스키마를 DBController로 대체
	DBController* wordSchema = DBController::sharedDB();
	char buf[100];
	sprintf(buf, "WHERE words.idx=%s", idxString->m_sString.c_str());
#if (defined PD_IPHONE) || (defined ANDROID_PHONE)    
	wordSchema->queryWithWhereStatementDBWordsSchema(buf);
	if (wordSchema->fetchDBWordsSchema()) 
#else
        wordSchema->queryWithWhereStatement(buf);
	if (wordSchema->fetch()) 
#endif
	{
		// handle data from database record
	
		SqlQuery query( *DBController::sharedDB()->getDB() );
		
		std::ostringstream strStream;
		CCString *ccStr = (CCString*)wordIdxArray->objectAtIndex(index);
		strStream << "SELECT soundName,imageName FROM words WHERE words.idx=" <<  ccStr->toStdString();
		SqlResult r = query( strStream.str() );
		
		if (r.empty()) 
			return NULL;
		
		
#if (defined PD_IPHONE) || (defined ANDROID_PHONE)
		soundPath = "sound/voice/" + r.fetch();
#else
		soundPath = "voice/" + r.fetch();
#endif
		
		
		string imageName = r.fetch();
		
		//프레임 애니메이션 생성 코드
		CCAnimation *ani = CCAnimation::animation();
		char buf[200];
		int count = 0;
		
		// picturecard/ani/a/abacus_ca_img00.png 형식으로 넘어온다. 뒤에 6글자 잘라서 번호 다시 붙여사용
		// 이미지가 존재할 경우에만 화면에 추가
		if ( imageName.length() != 0 && imageName != " ")
		{
			// DB에는 파일명만저장, 경로 붙여줄것
#if (defined PD_IPHONE) || (defined ANDROID_PHONE)
			imageName = __CONFIG_IMAGE_PATH_ANIMATION_ "350/" + imageName.substr(0,1) +"/" + imageName;
#else
			imageName = "ani/350/" + imageName.substr(0,1) +"/" + imageName;
#endif
			imageName = imageName.substr(0, imageName.length()-6);
		}
		
		while (1) 
		{
			sprintf(buf,"%s%02d.png",imageName.c_str(),count);
			// 첫프레임 대표스프라이트는 직접 add
			if (count == 0) 
			{
				sprImage = CCSprite::spriteWithFile(buf);
				if (sprImage) 
				{
					sprImage->setAnchorPoint(imageAnchor);	
					sprImage->setScale(imageOriginalScale);
					sprImage->setPosition(imagePosition);
				}
			}
			
			// 아이폰의 경우 bundle에 접근하면 되고
			// 안드로이드는 zip으로 패키징된 apk에 접근해야한다
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
			if( LCUtil::isFileExistAtBundle(buf) )
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)	
				if( CCFileUtils::isFileExistInAPK(buf) )
#endif 
				{
					CCSprite *sprFrame = CCSprite::spriteWithFile(buf);
					ani->addFrame(CCSpriteFrame::frameWithTexture(sprFrame->getTexture(),sprFrame->getTextureRect()));
					count++;
				}
				else
					break;
		}
		// 프레임이 한장 이상일경우에만 애니 실행
		if (count > 1) 
		{
			ani->setDelay(0.25);
			aniAction = CCAnimate::actionWithAnimation(ani, true);
			aniAction->retain();
		}
		else 
		{
			aniAction = NULL;
		}	
		
		return sprImage;
	}
	return NULL;
}

CCSprite* WordPuzzleLayer::getBoxImage()
{
#if (defined PD_IPHONE) || (defined ANDROID_PHONE)
	return CCSprite::spriteWithFile(__CONFIG_IMAGE_PATH_GAME__("d_spell_box.png"));
#else
	return CCSprite::spriteWithFile("game/d_spell_box.png");
#endif
	
}

LCWordPuzzleAlphabet* WordPuzzleLayer::getAlphabetImage(char alphabet)
{
	LCWordPuzzleAlphabet* alphabetSprite = new LCWordPuzzleAlphabet();
	char buf[100];

	//modified by YoungJae Kwon
#if (defined PD_IPHONE) || (defined ANDROID_PHONE)
	sprintf(buf, __CONFIG_IMAGE_PATH_GAME__("alphabet/%c.png"), alphabet);
#else
	sprintf(buf, "game/alphabet/%c.png", alphabet);
#endif
	alphabetSprite->initWithFile(buf);
	alphabetSprite->autorelease();
	return alphabetSprite;
}

void WordPuzzleLayer::arrangeComponents(const char* word, CCArray* boxArray, CCArray* alphabetArray)
{
	float widthOfBlock = this->getBoxImage()->getContentSize().width;

	//modified by YoungJae Kwon
#if (defined PD_IPHONE)
	float paddingOfCharacter = (585.00/2.0f - 56.00/2.0f) / 6.0f - widthOfBlock;
#elif (defined ANDROID_PHONE)
	float paddingOfCharacter = 111 - 48 - widthOfBlock;
#else
	float paddingOfCharacter = (105) - widthOfBlock;
#endif
	
	int numberOfCharacter = strlen(word);
	float firstX = this->getContentSize().width/2 - (paddingOfCharacter + widthOfBlock) / 2 * (numberOfCharacter - 1);
	

	
	for (int i=0; i<boxArray->count(); i++) {
		CCSprite* blockSprite = (CCSprite*) boxArray->objectAtIndex(i);
		blockSprite->setPosition(ccp(firstX + (widthOfBlock + paddingOfCharacter) * i, blockY));
	}
	
	for (int i=0; i<alphabetArray->count(); i++) {
		LCWordPuzzleAlphabet* alphabetSprite = (LCWordPuzzleAlphabet*) alphabetArray->objectAtIndex(i);
		alphabetSprite->setPosition(ccp(firstX + (widthOfBlock + paddingOfCharacter) * i, alphaY+(rand() % maxShift)) );
		alphabetSprite->setOriginalPosition(alphabetSprite->getPosition());
	}
	
	// suffle alphabets
	this->shuffleSprites(alphabetArray);
}


#pragma mark -
#pragma mark Event Handler


void WordPuzzleLayer::didCorrectCharacterSelected(CCArray* wordIdxArray, int index)
{
	//맥스 콤보 업데이트
	combo++;
	if (report.maxCombo < combo) 
	{
		report.maxCombo = combo;
	}
	
	SimpleAudioEngine::sharedEngine()->playEffect("sound/main_card_popup.mp3");
	char buf[20];
	
	CCSprite* sprMultiplier;
	
	if (combo < 5) 
	{
		sprMultiplier = NULL;
		multiplier = 1;
	}
	else if (combo < 10)
	{
		// x2
#if (defined PD_IPHONE) || (defined ANDROID_PHONE)
		sprMultiplier = CCSprite::spriteWithFile(__CONFIG_IMAGE_PATH_GAME__("effect/game_double_01.png"));
#else
		sprMultiplier = CCSprite::spriteWithFile("game/effect/game_double_01.png");
#endif
		multiplier = 2;
	}
	else if (combo < 20)
	{
		// x4
#if (defined PD_IPHONE) || (defined ANDROID_PHONE)
		sprMultiplier = CCSprite::spriteWithFile(__CONFIG_IMAGE_PATH_GAME__("effect/game_double_02.png"));
#else
		sprMultiplier = CCSprite::spriteWithFile("game/effect/game_double_02.png");
#endif
		
		multiplier = 4;
	}
	else if (combo < 30)
	{
		// x8
#if (defined PD_IPHONE) || (defined ANDROID_PHONE)
		sprMultiplier = CCSprite::spriteWithFile(__CONFIG_IMAGE_PATH_GAME__("effect/game_double_03.png"));
#else
		sprMultiplier = CCSprite::spriteWithFile("game/effect/game_double_03.png");
#endif
		multiplier = 8;
	}
	else if (combo < 50)
	{
		// x16
#if (defined PD_IPHONE) || (defined ANDROID_PHONE)
		sprMultiplier = CCSprite::spriteWithFile(__CONFIG_IMAGE_PATH_GAME__("effect/game_double_04.png"));	
#else
		sprMultiplier = CCSprite::spriteWithFile("game/effect/game_double_04.png");	 
#endif
		
		multiplier = 16;
	}
	else 
	{
#if (defined PD_IPHONE) || (defined ANDROID_PHONE)
		sprMultiplier = CCSprite::spriteWithFile(__CONFIG_IMAGE_PATH_GAME__("effect/game_double_05.png"));	 
#else
		sprMultiplier = CCSprite::spriteWithFile("game/effect/game_double_05.png");	 
#endif
		
		multiplier = 32;
	}


#if (defined PD_IPHONE) || (defined ANDROID_PHONE)
	CCSprite* label = CCSprite::spriteWithFile(__CONFIG_IMAGE_PATH_GAME__("effect/game_up.png"));
#else
	CCSprite* label = CCSprite::spriteWithFile("game/effect/game_up.png");
#endif
	
	label->setPosition(pointLabelPosition);
	this->addChild(label, 9999);
	
	if (sprMultiplier) 
	{
		sprMultiplier->setAnchorPoint(ccp(0,0.5));
#if (defined PD_IPHONE) || (defined ANDROID_PHONE)
		sprMultiplier->setPosition(ccp(label->getContentSize().width - 5,35));
#else
		sprMultiplier->setPosition(ccp(175,35));
#endif
		label->addChild(sprMultiplier);
	}


	
	label->setScale(0.2);
	
	CCEaseExponentialOut * fadeout = CCEaseExponentialOut::actionWithAction( CCActionTween::actionWithDuration(1.5,"opacity",255,0) );
	CCCallFuncN* callfunc = CCCallFuncN::actionWithTarget(this, callfuncN_selector(WordPuzzleLayer::onPointLabelFadeOut));
	
	label->runAction(CCMoveBy::actionWithDuration(0.8, ccp(0,100)));
	label->runAction(CCEaseBounceInOut::actionWithAction(CCScaleTo::actionWithDuration(0.3,labelScaleTo)));
	label->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.7),fadeout,callfunc, NULL));

	
	if (combo > 2)
	{
		sprintf(buf, "%dCombo",combo);
		CCLabelBMFont* lblCombo = (CCLabelBMFont*)getChildByTag(TAG_COMBO);
		
		CCEaseExponentialOut * fadeout = CCEaseExponentialOut::actionWithAction( CCActionTween::actionWithDuration(1.5,"opacity",255,0) );
		CCCallFuncN* callfunc = CCCallFuncN::actionWithTarget(this, callfuncN_selector(WordPuzzleLayer::onPointLabelFadeOut));
		
		if (lblCombo)
		{
			lblCombo->setString(buf);
			
			lblCombo->stopAllActions();
			lblCombo->setOpacity(255);
			lblCombo->setPosition(comboLabelPosition);

			lblCombo->setScale(0.2);			
			lblCombo->runAction(CCEaseBounceInOut::actionWithAction(CCScaleTo::actionWithDuration(0.3,labelScaleTo)));
			lblCombo->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.5),fadeout,callfunc, NULL));
		}
		else
		{
			lblCombo = CCLabelBMFont::labelWithString(buf, "font/combo.fnt");
			lblCombo->setPosition(comboLabelPosition);
			
			this->addChild(lblCombo, 9999, TAG_COMBO);
			
			lblCombo->setScale(0.2);			
			lblCombo->runAction(CCEaseBounceInOut::actionWithAction(CCScaleTo::actionWithDuration(0.3,labelScaleTo)));
			lblCombo->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(1.5),fadeout,callfunc, NULL));
		}
	}

		
	
}
void WordPuzzleLayer::didCorrectCharacterAnimated(CCArray* wordIdxArray, int index)
{
	m_iScore += BASE_SCORE*multiplier;
	char buf[10];
	sprintf(buf, "%06d", m_iScore);
	m_pScoreLabel->setString(buf);
}
void WordPuzzleLayer::didIncorrectCharacterSelected(CCArray* wordIdxArray, int index)
{
	SimpleAudioEngine::sharedEngine()->playEffect("sound/game_fail.mp3");
	
	report.incorrectTouch++;
	combo = 0;
	multiplier = 1;
	
#if (defined PD_IPHONE) || (defined ANDROID_PHONE)
	CCSprite* label = CCSprite::spriteWithFile(__CONFIG_IMAGE_PATH_GAME__("effect/game_down.png"));
#else
	CCSprite* label = CCSprite::spriteWithFile("game/effect/game_down.png");
#endif
	
	label->setPosition(pointLabelPosition);
	this->addChild(label, 9999);
	
	label->setScale(0.2);
	CCEaseBounceInOut * scaleup = CCEaseBounceInOut::actionWithAction(CCScaleTo::actionWithDuration(0.3,labelScaleTo));
	CCEaseExponentialOut * fadeout = CCEaseExponentialOut::actionWithAction( CCActionTween::actionWithDuration(1.5,"opacity",255,0) );
	CCCallFuncN* callfunc = CCCallFuncN::actionWithTarget(this, callfuncN_selector(WordPuzzleLayer::onPointLabelFadeOut));
	label->runAction(CCMoveBy::actionWithDuration(0.8, ccp(0,100)));
	label->runAction(scaleup);
	label->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.7),fadeout,callfunc, NULL));
	
	
}
void WordPuzzleLayer::didIncorrectCharacterAnimated(CCArray* wordIdxArray, int index)
{
	m_iScore -= 100;
	if (m_iScore < 0) {
		m_iScore = 0;
	}
	char buf[10];
	sprintf(buf, "%06d", m_iScore);
	m_pScoreLabel->setString(buf);
}
void WordPuzzleLayer::didFinishedWord(CCArray* wordIdxArray, int index)
{
	updateProgressLabel();
	
	sprImage->stopAllActions();
	if (aniAction) {
		sprImage->runAction(aniAction);
	}
	
		
	SimpleAudioEngine::sharedEngine()->playVoice(soundPath.c_str());
	
	// 단어맞춰서 자동으로 넘어갈때는 화살표로 못넘어가게 막기
	isAllowingNext = false;
	
}

ccTime WordPuzzleLayer::delayTimeOfChangeWord(CCArray* wordIdxArray, int index)
{
	return 1.5;
}



void WordPuzzleLayer::didWordImageTouched(CCSprite* touchedSprite)
{
	if (isActionOngoing) 
		return;

	if (touchedSprite) 
	{
		SimpleAudioEngine::sharedEngine()->playEffect("sound/menu_select.mp3");
		
		isActionOngoing = true;
		touchedSprite->stopAllActions();

		if (aniSeqAction) {
			touchedSprite->runAction(aniSeqAction);
		}
		if (aniAction) {
			touchedSprite->runAction(aniAction);
		}
		
	}
}
void WordPuzzleLayer::didIndexArrayReset()
{
	// 인덱스어레이가 바뀔때, 다음단어로 넘어갈때 호출할것
	updateProgressLabel();
}
void WordPuzzleLayer::updateProgressLabel()
{
	char buf[5];
	//Lazy Initialize
	sprintf(buf, "%02d/", m_iIndex+1);
	if ( !progressCurLabel )
	{
		progressCurLabel = CCLabelTTF::labelWithString(buf,CCSizeZero, CCTextAlignmentRight, PROGRESS_FONT,PROGRESS_SIZE);
		progressCurLabel->setPosition(progressCurLabelPosition);	
		progressCurLabel->setColor(CURRENT_COL);
		
		this->addChild(progressCurLabel,10);
	}
	else
	{
		progressCurLabel->setString(buf);
	}

	
	sprintf(buf, "%02d", m_pIndexArray->count());
	if (!progressTotalLabel) 
	{
		progressTotalLabel = CCLabelTTF::labelWithString(buf,CCSizeZero, CCTextAlignmentRight, PROGRESS_FONT,PROGRESS_SIZE);
		progressTotalLabel->setPosition(progressTotalLabelPosition);	
		progressTotalLabel->setColor(TOTAL_COL);
		
		this->addChild(progressTotalLabel,10);
	}
	else 
	{
		progressTotalLabel->setString(buf);
	}	
	
}

void WordPuzzleLayer::onAniDone()
{
	isActionOngoing = false;	
}



void WordPuzzleLayer::onTimeUpAniDone(CCNode* node)
{
	node->removeFromParentAndCleanup(true);
	
	this->didGameFinished();
}

void WordPuzzleLayer::onPointLabelFadeOut(CCNode* node)
{
	node->removeFromParentAndCleanup(true);
}


void WordPuzzleLayer::delayMenuTouch(ccTime dt)
{
	isAllowingNext = true;
	this->unschedule(schedule_selector(WordPuzzleLayer::delayMenuTouch));
}

void WordPuzzleLayer::didDelayedChangeWord()
{
	SimpleAudioEngine::sharedEngine()->playEffect("sound/game_success.mp3");
	
	loadNextWord();
	
	// 다음 단어로 넘어가는 작업이 끝나면 next버튼 활성화
	isAllowingNext = true;
}

void WordPuzzleLayer::next()
{
	// next버튼 연속클릭 방지
	if (isAllowingNext == false)
		return;
	
	isAllowingNext = false;
	this->schedule(schedule_selector(WordPuzzleLayer::delayMenuTouch), 0.5);

	// 넥스트 버튼 눌러서 넘어가면 콤보 초기화
	combo = 0;
	report.isSkipExist = true;
	loadNextWord();
}

void WordPuzzleLayer::didGameFinished()
{
	CCAssert(0,"didGameFinished:: override me!!");
}
void WordPuzzleLayer::loadNextWord()
{
	//MOD: 마구 클릭되는거 안막으면 메모리릭나는듯?
	// 메모리 릭이라기보단 autorelease가 빨리 안되서 그런거같은데.. 그래서 Delay넣어서 연속클릭못하게 막아서 autorelease 할 시간 벌었음
	if (sprImage && isActionOngoing) 
	{
		isActionOngoing = false;
		sprImage->stopAllActions();
	}
	
	
	
	CC_SAFE_RELEASE_NULL(aniAction);
	

	// 마지막단어가 끝난경우
	if ( m_iIndex + 1 == m_pIndexArray->count() )
	{
		
		report.clearTime = timer->getProgress();
		report.score = m_iScore;
		report.isWinner = true;
		this->didGameFinished();
	}
	else
	{
		this->loadSprites();
		updateProgressLabel();
	}
	
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
	
}

void WordPuzzleLayer::didPopupClose(VVPopupLayer* aPopup)
{
	//스코어보드 팝업 닫힘처리 할것 없음...
}