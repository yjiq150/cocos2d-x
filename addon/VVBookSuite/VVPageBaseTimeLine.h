//
//  VVPageBaseTimeLine.h
//  CocosX_test
//
//  Created by YoungJae Kwon on 11. 5. 21..
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

#ifndef __VVPageBaseTimeLine_H__
#define __VVPageBaseTimeLine_H__

#include "cocos2d.h"
#include "VVPageBase.h"
#include "CCTouchArea.h"


using namespace cocos2d;
using namespace CocosDenshion;
// 토글애니 정보 저장을 위한 객체
class ToggleEntity : public CCObject
{
public:
	CC_SYNTHESIZE_RETAIN(CCNode*,frame1,Frame1);
	CC_SYNTHESIZE_RETAIN(CCNode*,frame2,Frame2);
	CC_SYNTHESIZE(int,count,Count); // 반복횟수
	ToggleEntity(CCNode *aFrame1, CCNode *aFrame2,int aCount)
	{
		frame1 = NULL;
		frame2 = NULL;
		setFrame1(aFrame1);
		setFrame2(aFrame2);
		count = aCount;
	}
	~ToggleEntity()
	{
		CC_SAFE_RELEASE(frame1);
		CC_SAFE_RELEASE(frame2);
	}
protected:
	
};

// 타임라인 위에 올라가는 액션+오브젝트 객체
class TimeLineAniEntity : public CCObject
{
public:
	CC_SYNTHESIZE_RETAIN(CCAction*,action,Action);
	CC_SYNTHESIZE_RETAIN (CCNode*,object,Object);
	CC_SYNTHESIZE(ccTime,time,Time); // 시작 시간
	
	TimeLineAniEntity( CCAction *aAction, CCNode *aObject, ccTime aTime)
	{
		action = NULL;
		object = NULL;
		setAction(aAction);
		setObject(aObject);
		time = aTime;
	}
	~TimeLineAniEntity()
	{
		CC_SAFE_RELEASE(action);
		CC_SAFE_RELEASE(object);
	}
protected:
	
};

// 타임라인 위에 올라가는 액션+오브젝트 객체
class TimeLineSoundEntity : public CCObject
{
public:
	CC_SYNTHESIZE(std::string,path,Path);
	CC_SYNTHESIZE(AudioPlayerType,type,Type);
	CC_SYNTHESIZE(ccTime,time,Time); // 시작 시간
	
	//MOD: 여기 & 기호 맞게한건가?
	TimeLineSoundEntity( const char* aPath, AudioPlayerType aType, ccTime aTime)
	{
		path = aPath;
		type = aType;
		time = aTime;
	}
	~TimeLineSoundEntity()
	{
	}
protected:
	
};


// 기본적인 페이지 레이어에 TimeLine에 따른 액션/사운드를 추가할 수 있게 만들어놓은 클래스
class VVPageBaseTimeLine : public VVPageBase
{
protected:
	CCActionManager *actionMgr;
		
	CCSize layerSize; // 스크롤 제한을 걸기위한 mainLayer의 사이즈

	CCMutableArray<TimeLineAniEntity*> * timeLineAni; // 타임라인에 해당하는 action/object Dictionary를 가진 배열				에니메이션
	CCMutableArray<TimeLineSoundEntity*> * timeLineSound; // 타임라인에 해당하는 action/object Dictionary를 가진 배열			사운드
	CCMutableArray<TimeLineAniEntity*> * timeLineAni_backup; // 타임라인에 해당하는 action/object Dictionary를 가진 배열		..??
	
	ccTime elapsedTime;						// 타임라인의 현재 위치
	ccTime endTime;							// 타임라인이 종료되는 위치
	bool isEndTimeSet;						// 타임라인이 끝났는지를 체크
	
	
	
	// 토글 애니메이션 관리
	CCMutableDictionary<string,ToggleEntity*> *toggleDic;
	CCMutableDictionary<string,ToggleEntity*> *toggleDicStop;	
	
	CCMutableArray<CCNode*> * afterStopActions;
	
public:
	virtual bool init();  
	virtual ~VVPageBaseTimeLine();
	virtual void onExit();

	
	//MOD: virtual로 할건지 말지 결정
//	virtual void ccTouchesBegan(CCSet* touches, CCEvent* event);
//	virtual void ccTouchesMoved(CCSet* touches, CCEvent* event);
//	virtual void ccTouchesEnded(CCSet* touches, CCEvent* event);
	
	void addAction(CCAction *action, CCNode *object, ccTime timing);
	void addSound(const char *path, AudioPlayerType type, ccTime time);
	
	void setEndTime(ccTime aEndTime);
	
	virtual void onPageStart(); // onEnter는 로딩순간 호출되버리므로 로딩애니메이션이 끝난후 동작을 위해 새롭게 정의
	virtual void playTimeLine(ccTime startTime);
	void pauseTimeLine();
	void resumeTimeLine();

	void addToggleAnimation(CCString *keyName, CCNode *frame1, CCNode *frame2, int count);
	void setToggleAnimationCount(CCString *keyName, int count);
	
protected:
	// 스케쥴러 함수( 타임라인, 토글 두가지 )
	virtual void timeLineController(ccTime dt);
	virtual void toggleController();	
	

	void toggleAnimationAfterStop();
	void resumeAfterStopActions();
	 
	
protected:
#ifdef COCOS2D_DEBUG	
	CCLabelTTF *timeLabel;
	void updateTimeLabel();
#endif
	
};

#endif // __VVPageBaseTimeLine_SCENE_H__
