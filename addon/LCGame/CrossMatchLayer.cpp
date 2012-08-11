/*
 *  CrossMatchLayer.cpp
 *  HelloWorld
 *
 *  Created by 태호 이 on 11. 6. 16..
 *  Copyright 2011 Viscuit Creative. All rights reserved.
 *
 */

#include "CrossMatchLayer.h"
#include "DBController.h"
#include "LCUtil.h"
#include "SimpleAudioEngine.h"

#if (defined PD_IPHONE) || (defined ANDROID_PHONE)
#include "config.h"
#endif

#define TAG_COMBO 1234


using namespace CocosDenshion;
USING_NS_CC;


CrossMatchLayer::CrossMatchLayer()
{
	isActionOngoing = false;
	
	combo = 0;
	
	report.givenTime = 0;
	report.maxCombo = 0;
	report.score = 0;
	report.incorrectTouch = 0;
}

CrossMatchLayer::~CrossMatchLayer()
{
	map<int,CCAnimate*>::iterator iter;
	for( iter = aniActions.begin(); iter != aniActions.end(); ++iter)
	{
		CCAnimate* ani = (*iter).second;
		ani->release();
	}
	aniActions.clear();	
	
}


bool CrossMatchLayer::init(){
	if (!LCCrossMatchGame::init()) {
		return false;
	}
	
	m_iScore = 0;
	
	winSize = CCDirector::sharedDirector()->getWinSize();
#ifdef ANDROID_GALAXY_NOTE
	winSize.height -= 48;
#endif	
	return true;
}

void CrossMatchLayer::onEnter()
{
	CCLayer::onEnter();
	timer->setProgress(0);
	timer->setMaxTime(CM_TOTAL_GIVEN_TIME);
	timer->setWarningTime(timer->getMaxTime()*0.7);
	
	report.givenTime = timer->getMaxTime();
}


void CrossMatchLayer::onExit()
{
	CCLayer::onExit();
}

void CrossMatchLayer::runGame()
{
	timer->start();
	this->loadIndex(m_pIndexArray);
	m_iIndex = 0;
	
	this->loadSprites();
}

#pragma mark -
#pragma mark implement TimeBar Delegate
void CrossMatchLayer::didTimeUp(TimeBar* aTimeBar)
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
	CCCallFuncN* callfunc = CCCallFuncN::actionWithTarget(this, callfuncN_selector(CrossMatchLayer::onTimeUpAniDone));
	sprTimeUp->runAction(CCSequence::actions(fadeIn,CCDelayTime::actionWithDuration(0.8),fadeout,callfunc, NULL));
	
	//전부 터치 막고
	SimpleAudioEngine::sharedEngine()->playEffect("sound/time_warning.mp3");
	
}
void CrossMatchLayer::didTimeWarningUp(TimeBar* aTimeBar)
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
	CCCallFuncN* callfunc = CCCallFuncN::actionWithTarget(this, callfuncN_selector(CrossMatchLayer::onPointLabelFadeOut));
	sprWarn->runAction(CCSequence::actions(fadeIn,CCDelayTime::actionWithDuration(0.8),fadeout,callfunc, NULL));
	
	SimpleAudioEngine::sharedEngine()->playEffect("sound/time_warning.mp3");
}


#pragma mark -
#pragma mark implement LCCrossMatchGame
void CrossMatchLayer::loadIndex(CCArray* wordIdxArray)
{
	char buf[10];
	
	SqlQuery query( *DBController::sharedDB()->getDB() );
	
	// 이미지있는것 불러와서 후 리스트 셔플
	std::ostringstream strStream;
	strStream << "SELECT idx FROM words WHERE is_image_exist = 1 AND length(headword) < 8 AND lang='"<< languageCode.c_str() << "' ORDER BY RANDOM() LIMIT " << CM_MAX_NUMBER_OF_QUESTION;
	SqlResult r = query( strStream.str() );
		
	while(!r.empty()) 
	{
		sprintf(buf, "%d", r.fetchInteger());
		wordIdxArray->addObject(CCString::makeString(buf));
	}
}

CCLabelBMFont* CrossMatchLayer::getTextLabelWithIndex(CCArray* wordIdxArray, int index)
{
	CCString* idxString = (CCString*)wordIdxArray->objectAtIndex(index);
	
	// 태호씨 워드스키마를 DBController로 대체
	DBController* wordSchema = DBController::sharedDB();
	
	char buf[40];
	sprintf(buf, "WHERE words.idx=%s", idxString->m_sString.c_str());
#if (defined PD_IPHONE) || (defined ANDROID_PHONE)    
	wordSchema->queryWithWhereStatementDBWordsSchema(buf);
	if (wordSchema->fetchDBWordsSchema()) 
#else
        wordSchema->queryWithWhereStatement(buf);
	if (wordSchema->fetch()) 
#endif
	{
		CCLabelBMFont* characterLabel = CCLabelBMFont::labelWithString(wordSchema->r_s_headword.c_str(), wordFont.c_str());
		characterLabel->setColor(wordColor);
		characterLabel->setAnchorPoint(ccp(1,0.5));
		return characterLabel;
	}

	return NULL;
}
CCSprite* CrossMatchLayer::getImageWithIndex(CCArray* wordIdxArray, int index)
{
	CCString* idxString = (CCString*)wordIdxArray->objectAtIndex(index);

	// 태호씨 워드스키마를 DBController로 대체
	DBController* wordSchema = DBController::sharedDB();
	
	char buf[40];
	sprintf(buf, "WHERE words.idx=%s", idxString->m_sString.c_str());
    
#if (defined PD_IPHONE) || (defined ANDROID_PHONE)    
	wordSchema->queryWithWhereStatementDBWordsSchema(buf);
	if (wordSchema->fetchDBWordsSchema()) 
#else
        wordSchema->queryWithWhereStatement(buf);
	if (wordSchema->fetch()) 
#endif
	{	
		SqlQuery query( *DBController::sharedDB()->getDB() );
		
		std::ostringstream strStream;
		CCString *ccStr = (CCString*)wordIdxArray->objectAtIndex(index);
		strStream << "SELECT soundName,imageName FROM words WHERE words.idx=" <<  ccStr->toStdString();
		SqlResult r = query( strStream.str() );
		
		if (r.empty()) 
			return NULL;
		
		
#if (defined PD_IPHONE) || (defined ANDROID_PHONE)
		string soundPath = "sound/voice/" + r.fetch();
#else
		string soundPath = "voice/" + r.fetch();
#endif
		soundPaths.insert(map<int,string>::value_type(index, soundPath));
		
		string imageName = r.fetch();
		
		//프레임 애니메이션 생성 코드
		CCSprite* sprImage;
		CCAnimate* aniAction;
		CCAnimation *ani = CCAnimation::animation();
		char buf[200];
		int count = 0;
		
		// picturecard/ani/a/abacus_ca_img00.png 형식으로 넘어온다. 뒤에 6글자 잘라서 번호 다시 붙여사용
		// 이미지가 존재할 경우에만 화면에 추가
		if ( imageName.length() != 0 && imageName != " ")
		{
			// DB에는 파일명만저장, 경로 붙여줄것
#if (defined PD_IPHONE) || (defined ANDROID_PHONE)
			imageName = __CONFIG_IMAGE_PATH_ANIMATION_ + imageSizeStr + "/" + imageName.substr(0,1) +"/" + imageName;
#else
			imageName = "ani/" + imageSizeStr + "/" + imageName.substr(0,1) +"/" + imageName;
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
			
			
			aniActions.insert(map<int,CCAnimate*>::value_type(index, aniAction));
			
			sprImage->runAction(aniAction);		
		}
		else 
		{
			aniAction = NULL;
		}	
		
		return sprImage;
	
	}
	return NULL;
}


void CrossMatchLayer::arrangeComponents(CCArray* labelArray, CCArray* imageArray)
{
	int i=0;
	for (i=0; i<labelArray->count(); i++) {
		CCLabelBMFont* label = (CCLabelBMFont*) labelArray->objectAtIndex(i);
		label->setPosition(ccp(wordPosX, wordStartHeight + wordHeightInterval * i));
	}
	
	for (i=0; i<imageArray->count(); i++) {
		CCSprite* sprite = (CCSprite*) imageArray->objectAtIndex(i);
		// 스케일 조절
		sprite->setScale(imageOriginalScale);
		sprite->setAnchorPoint(imageAnchor);
		sprite->setPosition(ccp(imagePosX, wordStartHeight + wordHeightInterval * i));
	}

	this->shuffleNodes(labelArray);
}
void CrossMatchLayer::onPointLabelFadeOut(CCNode* node)
{
	node->removeFromParentAndCleanup(true);
}
void CrossMatchLayer::onAniDone()
{
	isActionOngoing = false;	
}
#pragma mark -
#pragma mark Event Handler
void CrossMatchLayer::didWordImagedTouched(CCSprite* touchedSprite, CCArray* wordIdxArray, int touchedIdx)
{
	if (isActionOngoing) 
		return;
	
	if (touchedSprite) 
	{
		touchedSprite->getParent()->reorderChild(touchedSprite, 9999);
		SimpleAudioEngine::sharedEngine()->playEffect("sound/menu_select.mp3");
		touchedSprite->stopAllActions();

		isActionOngoing = true;
		map< int, CCAnimate* >::iterator iter = aniActions.find( touchedIdx );
		if( iter != aniActions.end() )
		{
			touchedSprite->runAction((*iter).second);	
			
			// 메인이미지 애니메이션과 함께 커지는 액션
			CCEaseExponentialIn* scaleup = CCEaseExponentialIn::actionWithAction( CCScaleTo::actionWithDuration(0.25,imageAniScale) );
			CCEaseExponentialIn* scaledown = CCEaseExponentialIn::actionWithAction( CCScaleTo::actionWithDuration(0.25,imageOriginalScale) );
			CCCallFunc* callAniDone = CCCallFunc::actionWithTarget(this, callfunc_selector(CrossMatchLayer::onAniDone));
			CCAction* aniSeqAction = CCSequence::actions(scaleup,CCDelayTime::actionWithDuration(1),scaledown,callAniDone,NULL);
			touchedSprite->runAction(aniSeqAction);
		}
	}
}

void CrossMatchLayer::didMatchWord(CCSprite* matchedSprite, CCArray* wordIdxArray, int index)
{
	SimpleAudioEngine::sharedEngine()->playEffect("sound/main_card_popup.mp3");
	//해당 인덱스의 사운드 재생
	map< int, string >::iterator iter = soundPaths.find( index );
	if( iter != soundPaths.end() )
	{
		SimpleAudioEngine::sharedEngine()->playVoice( (*iter).second.c_str() );			
	}
	
	matchedSprite->stopAllActions();
	map< int, CCAnimate* >::iterator iterAct = aniActions.find( index );
	if( iterAct != aniActions.end() )
	{
		matchedSprite->runAction((*iterAct).second);	
		
		// 메인이미지 애니메이션과 함께 커지는 액션
		CCEaseExponentialIn* scaleup = CCEaseExponentialIn::actionWithAction( CCScaleTo::actionWithDuration(0.25,imageAniScale) );
		CCEaseExponentialIn* scaledown = CCEaseExponentialIn::actionWithAction( CCScaleTo::actionWithDuration(0.25,imageOriginalScale) );
		CCCallFunc* callAniDone = CCCallFunc::actionWithTarget(this, callfunc_selector(CrossMatchLayer::onAniDone));
		CCAction* aniSeqAction = CCSequence::actions(scaleup,CCDelayTime::actionWithDuration(1),scaledown,callAniDone,NULL);
		matchedSprite->runAction(aniSeqAction);

	}
	
	//
	// 콤보부분은 워드퍼즐과 완전히 동일
	//
	
	//맥스 콤보 업데이트
	combo++;
	if (report.maxCombo < combo) 
	{
		report.maxCombo = combo;
	}
	
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
	CCCallFuncN* callfunc = CCCallFuncN::actionWithTarget(this, callfuncN_selector(CrossMatchLayer::onPointLabelFadeOut));
	
	label->runAction(CCMoveBy::actionWithDuration(0.8, ccp(0,100)));
	label->runAction(CCEaseBounceInOut::actionWithAction(CCScaleTo::actionWithDuration(0.3,labelScaleTo)));
	label->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.7),fadeout,callfunc, NULL));
	
	
	if (combo > 2)
	{
		sprintf(buf, "%dCombo",combo);
		CCLabelBMFont* lblCombo = (CCLabelBMFont*)getChildByTag(TAG_COMBO);
		
		CCEaseExponentialOut * fadeout = CCEaseExponentialOut::actionWithAction( CCActionTween::actionWithDuration(1.5,"opacity",255,0) );
		CCCallFuncN* callfunc = CCCallFuncN::actionWithTarget(this, callfuncN_selector(CrossMatchLayer::onPointLabelFadeOut));
		
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
	
	// 스코어 업데이트
	m_iScore += CM_BASE_SCORE*multiplier;

	sprintf(buf, "%08d", m_iScore);
	m_pScoreLabel->setString(buf);
}
void CrossMatchLayer::didMissMatchWord()
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
	CCCallFuncN* callfunc = CCCallFuncN::actionWithTarget(this, callfuncN_selector(CrossMatchLayer::onPointLabelFadeOut));
	label->runAction(CCMoveBy::actionWithDuration(0.8, ccp(0,100)));
	label->runAction(scaleup);
	label->runAction(CCSequence::actions(CCDelayTime::actionWithDuration(0.7),fadeout,callfunc, NULL));
	
	
	m_iScore -= 100;
	if (m_iScore < 0) {
		m_iScore = 0;
	}
	char buf[10];
	sprintf(buf, "%08d", m_iScore);
	m_pScoreLabel->setString(buf);
}
ccTime CrossMatchLayer::delayTimeOfChangeWord(CCArray* wordIdxArray, int index)
{
	return 1.5;
}

void CrossMatchLayer::onTimeUpAniDone(CCNode* node)
{
	node->removeFromParentAndCleanup(true);
	
	// 스코어 저장
	// 기타 보너스 점수 계산?, 현재는 없음
	report.score = m_iScore;
	
	this->didGameFinished();
}

void CrossMatchLayer::didFinishedStage()
{
	SimpleAudioEngine::sharedEngine()->playEffect("sound/game_success.mp3");
	
	// 다음스테이지를 위해 현재 맵/초기화 및 릴리즈
	map<int,CCAnimate*>::iterator iter;
	for( iter = aniActions.begin(); iter != aniActions.end(); ++iter)
	{
		CCAnimate* ani = (*iter).second;
		ani->release();
	}
	aniActions.clear();	
	soundPaths.clear();
	
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();
}

void CrossMatchLayer::didGameFinished()
{
	CCAssert(0,"didGameFinished:: override me!!");
}
