//
//  VVResourceManager.cpp
//  
//
//  Created by YoungJae Kwon on 12. 6. 17..
//  Copyright (c) 2012 Beyondi Soft All rights reserved.
//

#include "VVResourceManager.h"
#include <iostream>
#include "VVLog.h"
USING_NS_CC;

static VVResourceManager* s_shared;

VVResourceManager* VVResourceManager::shared()
{
	if (s_shared == NULL)
	{
        s_shared = new VVResourceManager();
	}
    
	return s_shared;
}

VVResourceManager::VVResourceManager()
{
    isDownloadResourceFromServer = false;
}
