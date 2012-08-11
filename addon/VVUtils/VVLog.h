/*
 *  VVLog.h
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 10..
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */

#ifndef __VVLog_H__
#define __VVLog_H__

// logging macros
#include "cocos2d.h"

    void VVLog(CCNode* node);
    void VVLog(const char* msg, CCNode* node);


    void VVLog(CCRect rect);
    void VVLog(const char* msg, CCRect rect);

    void VVLog(CCSize sz);
    void VVLog(const char* msg, CCSize sz);

    void VVLog(CCPoint point);
    void VVLog(const char* msg, CCPoint point);


#endif
