//
//  VVResourceManager.h
//  
//
//  Created by YoungJae Kwon on 12. 6. 17..
//  Copyright (c) 2012 Beyondi Soft All rights reserved.
//

#ifndef _VVResourceManager_h
#define _VVResourceManager_h

#include "cocos2d.h"

USING_NS_CC;

class VVResourceManager
{
public:
    static VVResourceManager* shared();
    
    VVResourceManager();
    
    CC_SYNTHESIZE(bool, isDownloadResourceFromServer, IsDownloadResourceFromServer);

protected:

};


#endif
