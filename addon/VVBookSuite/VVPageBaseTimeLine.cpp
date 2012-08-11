//
//  VVPageBaseTimeLine.cpp
//  CocosX_test
//
//  Created by YoungJae Kwon on 11. 5. 21..
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

#include "VVPageBaseTimeLine.h"


USING_NS_CC;


bool VVPageBaseTimeLine::init()
{
	if ( !VVPageBase::init() )
		return false;
	
	actionMgr = CCActionManager::sharedManager();
	elapsedTime = 0;

	
	//타임라인 변수 초기화
	timeLineAni = new CCMutableArray<TimeLineAniEntity*>();
	timeLineSound = new CCMutableArray<TimeLineSoundEntity*>();
	
	
	//토글 애니메이션 초기화
	toggleDic = new CCMutableDictionary<string,ToggleEntity*>();
	toggleDicStop = new CCMutableDictionary<string,ToggleEntity*>();
	
	// 정지후 액션 설정
	afterStopActions = new CCMutableArray<CCNode*>();
	
	isEndTimeSet = false;
#ifdef COCOS2D_DEBUG	
	// 디버그용 타임 카운터 초기화
	char buf[10];
	sprintf(buf, "%.0f",elapsedTime);
	timeLabel = CCLabelTTF::labelWithString(buf, "", 30);
	timeLabel->setPosition( ccp( winSize.width /2 , winSize.height-30 ) );
	addChild(timeLabel,999);
	schedule(schedule_selector(VVPageBaseTimeLine::updateTimeLabel), 1);
#endif
	
	return true;
}
#ifdef COCOS2D_DEBUG
void VVPageBaseTimeLine::updateTimeLabel()
{
	char buf[10];
	sprintf(buf, "%.0f",elapsedTime);
	timeLabel->setString(buf);
}
#endif


void VVPageBaseTimeLine::setEndTime(ccTime aEndTime)
{
	endTime = aEndTime;
	isEndTimeSet = true;
}

void VVPageBaseTimeLine::addToggleAnimation(CCString* aKeyName, CCNode* aFrame1, CCNode* aFrame2, int aCount)
{
	aFrame1->setIsVisible(true); aFrame2->setIsVisible(false);

	ToggleEntity *toggle = new ToggleEntity(aFrame1,aFrame2,aCount);
	toggleDic->setObject(toggle,aKeyName->m_sString);
	
	toggle->release();
}
void VVPageBaseTimeLine::setToggleAnimationCount(CCString * aKeyName, int aCount)
{
	ToggleEntity *toggleAni = toggleDic->objectForKey(aKeyName->m_sString);
	toggleAni->setCount(aCount);
	toggleDicStop->setObject(toggleAni,aKeyName->m_sString);
}

// 애니메이션 끝나고 터치시 움직이는 녀석들
void VVPageBaseTimeLine::toggleAnimationAfterStop()
{
	
	std::vector<std::string> keys = toggleDicStop->allKeys();
	std::vector<std::string>::iterator it;
	for (it = keys.begin(); it != keys.end(); ++it)
	{
		ToggleEntity *toggleAni = toggleDicStop->objectForKey(*it);
		int count = toggleAni->getCount();
		// 카운트가 0이 아닐때만 토글
		if( count != 0 )
		{
			// 토글을 위해 visible값 교환
			CCNode *frame1 = toggleAni->getFrame1();
			CCNode *frame2 = toggleAni->getFrame2();
			bool temp = frame1->getIsVisible();
			frame1->setIsVisible( frame2->getIsVisible() );
			frame2->setIsVisible( temp );

			if ( count != -1 )
			{
				// 카운트 업데이트
				count--;
				toggleAni->setCount(count);
			}
		}
	}
}

void VVPageBaseTimeLine::toggleController()
{
	std::vector<std::string> keys = toggleDic->allKeys();
	std::vector<std::string>::iterator it;
	for (it = keys.begin(); it != keys.end(); ++it)
	{
		ToggleEntity *toggleAni = toggleDic->objectForKey(*it);
		// 토글을 위해 visible값 교환
		CCNode *frame1 = toggleAni->getFrame1();
		CCNode *frame2 = toggleAni->getFrame2();
		bool temp = frame1->getIsVisible();
		frame1->setIsVisible( frame2->getIsVisible() );
		frame2->setIsVisible( temp );
	}
}

//동작이 정지된 후에도 계속 움직일 녀석을
void VVPageBaseTimeLine::resumeAfterStopActions()
{

	CCMutableArray<CCNode*>::CCMutableArrayIterator it;
	for (it = afterStopActions->begin(); it != afterStopActions->end(); ++it)
	{
		CCNode * target = *it;
		target->setIsVisible(true);
		actionMgr->resumeTarget(target);
	}
}

void VVPageBaseTimeLine::playTimeLine(ccTime startTime)
{
	elapsedTime = startTime;

	timeLineAni_backup = CCMutableArray<TimeLineAniEntity*>::arrayWithArray(timeLineAni);
	timeLineAni_backup->retain();
	schedule(schedule_selector(VVPageBaseTimeLine::timeLineController));
}

void VVPageBaseTimeLine::timeLineController(ccTime dt)
{
	elapsedTime += dt;

	if (isEndTimeSet && elapsedTime > endTime) 
	{
		pauseTimeLine();
		resumeAfterStopActions();
		//sprPause.visible = false;
		//sprPlay.visible = false;
		//isControlBtnEnabled = false;
	}
	
	if( timeLineAni->count() > 0 )
	{
		int count = timeLineAni->count();
		for( unsigned int i = 0; i < count; i++)
		{
			TimeLineAniEntity *entity = timeLineAni->getObjectAtIndex(i);
			if( entity->getTime() < elapsedTime )
			{
				CCNode *targetSymbol = entity->getObject();
				targetSymbol->runAction( entity->getAction() );
				timeLineAni->removeObjectAtIndex(i);
				i--; //하나 지워서 오브젝트들이 당겨졌으므로 i값은 그대로 돌려놓음
				count--;
			}
		}
	}
	if( timeLineSound->count() > 0 )
	{
		int count = timeLineSound->count();
		for( int i = 0; i < count; i++)
		{
			TimeLineSoundEntity *entity = timeLineSound->getObjectAtIndex(i);
			if( entity->getTime() < elapsedTime )
			{
				
				//안드로이드 버그 주의: 이렇게하면 안드로이드에서 stlport로 컴파일시 쓰레기 문자열이 들어간다
				//const char* path = entity->getPath().c_str();
				//CCLog(path);
				
				//효과음/목소리 재생
				if (entity->getType() == kAudio_Effect) 
					ap->playEffect( entity->getPath().c_str() );
				else if (entity->getType() == kAudio_Voice) 
					ap->playVoice( entity->getPath().c_str() );	
				else 
					ap->playBackgroundMusic(entity->getPath().c_str() );
				
				timeLineSound->removeObjectAtIndex(i);
				i--; //하나 지워서 오브젝트들이 당겨졌으므로 i값은 그대로 돌려놓음
				count--;
			}
		}
	}
}

 
void VVPageBaseTimeLine::addAction(CCAction* aAction, CCNode* aObject, ccTime time)
{
	TimeLineAniEntity* entity = new TimeLineAniEntity(aAction, aObject, time);
	//최적화 하려면 sort해서 넣을것.
	timeLineAni->addObject(entity);
	entity->release();
}
void VVPageBaseTimeLine::addSound(const char *aPath, AudioPlayerType aType, ccTime aTime)
{	
	TimeLineSoundEntity* entity = new TimeLineSoundEntity(aPath, aType, aTime);
	//최적화 하려면 sort해서 넣을것.
	timeLineSound->addObject(entity);
	entity->release();
}

void VVPageBaseTimeLine::onPageStart()
{
	VVPageBase::onPageStart();
	playTimeLine(0);
}

void VVPageBaseTimeLine::pauseTimeLine()
{
	if (timeLineAni_backup) 
	{
		unschedule(schedule_selector(VVPageBaseTimeLine::timeLineController));
		unschedule(schedule_selector(VVPageBaseTimeLine::toggleController));

		for( unsigned int i = 0; i < timeLineAni_backup->count(); i++)
		{
			TimeLineAniEntity *entity = timeLineAni_backup->getObjectAtIndex(i);
			{
				actionMgr->pauseTarget( entity->getObject() );
				ap->pauseVoice();
			}
		}
	}
}
void VVPageBaseTimeLine::resumeTimeLine()
{
	if (timeLineAni_backup) 
	{
		schedule(schedule_selector(VVPageBaseTimeLine::timeLineController));
		schedule(schedule_selector(VVPageBaseTimeLine::toggleController),1);

		for( unsigned int i = 0; i < timeLineAni_backup->count(); i++)
		{
			TimeLineAniEntity *entity = timeLineAni_backup->getObjectAtIndex(i);
			{
				actionMgr->pauseTarget( entity->getObject() );
				ap->resumeVoice();
			}
		}
	}
}



void VVPageBaseTimeLine::onExit()
{
	VVPageBase::onExit();
}

VVPageBaseTimeLine::~VVPageBaseTimeLine()
{
	CC_SAFE_RELEASE_NULL(timeLineSound);
	CC_SAFE_RELEASE_NULL(timeLineAni);
	CC_SAFE_RELEASE_NULL(timeLineAni_backup);
	CC_SAFE_RELEASE_NULL(toggleDic);
	CC_SAFE_RELEASE_NULL(toggleDicStop);
	CCLog("pagebasetimeline destroy");
}