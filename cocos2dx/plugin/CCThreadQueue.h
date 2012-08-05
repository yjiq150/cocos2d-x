
#ifndef __CCThreadQueue_H__
#define __CCThreadQueue_H__

#include <string>
#include "CCObject.h"
#include "CCMutableDictionary.h"
#include "CCTexture2D.h"
/*

namespace   cocos2d {

class CCLock;	
	
class CCThreadQueueTask : public CCObject
{
public:
	SEL_CallFunc m_pfnTask;
	CCObject* m_pTarget;
	
	CCThreadQueueTask();
	virtual ~CCThreadQueueTask();
	
	// 해당 객체의 콜백함수를 실행
	// execute는 항상 메인스레드에서 실행되어야한다.
	// 서브스레드에서 실행시 일단 readyToRunOnMainThread에 넣어주고 메인스레드가 때가되면 접근하도록 설계됨
	virtual void execute();
	
};

	
class CC_DLL CCThreadQueue : public CCObject
{
public:

	CCThreadQueue();
	virtual ~CCThreadQueue();

	static CCThreadQueue * sharedThreadQueue();
	
	void addFunction(CCObject *target, SEL_CallFunc selector);

	//MOD: if additional arguments required, implement!
//	void addFunction(SelectorProtocol *target, SEL_CallFuncO selector);
//	void addFunction(SelectorProtocol *target, SEL_CallFuncN selector);
//	void addFunction(SelectorProtocol *target, SEL_CallFuncND selector);	

	// will be called by main thread
	void runTasks();
	CCLock* queueLock;
	
protected:
	CCMutableArray<CCThreadQueueTask*> *queue;
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )

protected:
	// to access CCAsyncObject by char key
//	CCMutableDictionary<string,CCAsyncObject*> *asyncDic;
#endif
};

}//namespace   cocos2d 

 */

#endif //__CCThreadQueue_H__

