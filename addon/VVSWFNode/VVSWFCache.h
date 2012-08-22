//
//  VVSWFCache.h
//  Cocos2dxTemplate
//
//  Created by YoungJae Kwon on 12. 4. 11..
//  Copyright (c) 2012년 Beyondi Inc. All rights reserved.
//

#ifndef __VVSWFCache_H__
#define __VVSWFCache_H__

#include "cocos2d.h"
#include <list>
class VVSWFNode;



class VVSWFCache : public CCObject
{
protected:
    //CCMutableDictionary<std::string, VVSWFNode*> * m_pSWFs;
    std::list<VVSWFNode*> m_pSWFs;

    //pthread_mutex_t				*m_pDictLock;
    
    //added by YoungJae Kwon
    // If maxTasksInQueue == 0 , no limit for size of queue
    //CC_SYNTHESIZE(int, maxTasksInQueue, MaxTasksInQueue);
    
private:
    // @todo void addImageWithAsyncObject(CCAsyncObject* async);
    //void addImageAsyncCallBack(ccTime dt);
    
public:
    
    VVSWFCache();
    virtual ~VVSWFCache();
    void addSWFNode(VVSWFNode* node);
    void removeSWFNode(VVSWFNode* node);    
    void reloadAll();
    
    static VVSWFCache * sharedSWFCache();

};

#endif
