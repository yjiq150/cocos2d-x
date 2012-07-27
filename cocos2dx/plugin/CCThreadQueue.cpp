/****************************************************************************
Copyright (c) 2010-2011 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/



#include <cctype>
#include "CCThreadQueue.h"
#include "ccMacros.h"
#include "platform/platform.h"
#include "support/ccUtils.h"

namespace   cocos2d 
{
/////////////////////////////////////	
// CCThreadQueueTask Implementation
/////////////////////////////////////	
CCThreadQueueTask::CCThreadQueueTask()
{
}
CCThreadQueueTask::~CCThreadQueueTask()
{
	if(m_pTarget)
		m_pTarget->selectorProtocolRelease();
}
void CCThreadQueueTask::execute()
{
	if(m_pTarget)
		(m_pTarget->*m_pfnTask)();
}

	
	
/////////////////////////////////////	
// CCThreadQueue Implementation
/////////////////////////////////////
	
static CCThreadQueue *g_sharedThreadQueue;

CCThreadQueue * CCThreadQueue::sharedThreadQueue()
{
	if (!g_sharedThreadQueue)
		g_sharedThreadQueue = new CCThreadQueue();
	
	return g_sharedThreadQueue;
}

CCThreadQueue::CCThreadQueue()
{
	CCAssert(g_sharedThreadQueue == NULL, "Attempted to allocate a second instance of a singleton.");
	
	
	queueLock = new CCLock();
	queue = new CCMutableArray<CCThreadQueueTask*>();
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )

#endif
	
}

CCThreadQueue::~CCThreadQueue()
{
	CC_SAFE_DELETE(queueLock);
	CC_SAFE_RELEASE(queue);
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )

#endif
}

// this is called from MainThread by Draw() or mainloop()
void CCThreadQueue::runTasks()
{
	queueLock->lock();
	
	unsigned int count = queue->count();
//	if (count > 0) {
//			CCLog("queueCnt:%d",count);
//	}

	for( unsigned int i = 0; i < count; i++)
	{
		CCThreadQueueTask *task = (CCThreadQueueTask*)queue->getObjectAtIndex(i);
		task->execute();
	}
	queue->removeAllObjects();

	queueLock->unlock();
}
	
void CCThreadQueue::addFunction(SelectorProtocol *target, SEL_CallFunc selector)
{
	CCThreadQueueTask *task = new CCThreadQueueTask();
	task->m_pfnTask = selector;
	task->m_pTarget = target;
	
	//this retain will be released at destructor of CCThreadQueueTask
	target->selectorProtocolRetain();
	
	queueLock->lock();
	queue->addObject(task);
	queueLock->unlock();	
	
	task->release();
}

}//namespace   cocos2d 

