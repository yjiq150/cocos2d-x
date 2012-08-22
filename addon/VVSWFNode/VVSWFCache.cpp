//
//  VVSWFCache.cpp
//  Cocos2dxTemplate
//
//  Created by YoungJae Kwon on 12. 4. 11..
//  Copyright (c) 2012년 Beyondi Inc. All rights reserved.
//

#include "VVSWFCache.h"
#include "VVSWFNode.h"
#include <algorithm>
using namespace cocos2d;
using namespace std;

// TextureCache - Alloc, Init & Dealloc
static VVSWFCache *g_sharedSWFCache;

VVSWFCache * VVSWFCache::sharedSWFCache()
{
    if (!g_sharedSWFCache)
        g_sharedSWFCache = new VVSWFCache();
    
    return g_sharedSWFCache;
}

VVSWFCache::VVSWFCache()
{
    CCAssert(g_sharedSWFCache == NULL, "Attempted to allocate a second instance of a singleton.");
    
    
    // thread variable init
//    pthread_mutex_init(&s_taskQueueLock, NULL);
//    pthread_mutex_init(&s_readyQueueLock, NULL);
    
//    CCScheduler::sharedScheduler()->scheduleSelector(schedule_selector(VVSWFCache::addImageAsyncCallBack), this, 0, false);
    
//    maxTasksInQueue = 0;
   
}


void VVSWFCache::addSWFNode(VVSWFNode* node)
{    
    // find if node exists in cahce list
    list<VVSWFNode*>::iterator it = std::find(m_pSWFs.begin(), m_pSWFs.end(), node);
    
    if( it != m_pSWFs.end())
    {
        CCAssert(0, "VVSWFNode already exists in cache list");
    }

    else
    {
        m_pSWFs.push_back(node);
    }

}
void VVSWFCache::removeSWFNode(VVSWFNode* node)
{    
    // find if node exists in cahce list
    list<VVSWFNode*>::iterator it = std::find(m_pSWFs.begin(), m_pSWFs.end(), node);
    
    if( it != m_pSWFs.end())
    {
        m_pSWFs.erase(it);
    }
    else
    {
        CCAssert(0, "VVSWFNode doesn't exists in cache list");
    }
    
}

void VVSWFCache::reloadAll()
{
    list<VVSWFNode*>::iterator it = m_pSWFs.begin();
    
    while ( it != m_pSWFs.end() )
    {
        VVSWFNode* node = *it;
        if (node == NULL) 
            CCAssert(0, "null VVSWFNode found");
        
        node->reload();
        
        it++;
    }
}


VVSWFCache::~VVSWFCache()
{

}

