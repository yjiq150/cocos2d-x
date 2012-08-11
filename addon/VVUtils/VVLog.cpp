/*
 *  VVLog.cpp
 *  CollinsDic
 *
 *  Created by YoungJae Kwon on 11. 7. 10..
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */

#include "VVLog.h"

void VVLog(CCNode* node)
{
    CCLog("Node: x_y(%0.0f,%0.0f) w_h(%0.0f,%0.0f) anchor(%f,%f)",node->getPosition().x,node->getPosition().y,
          node->getContentSize().width,node->getContentSize().height,
          node->getAnchorPoint().x, node->getAnchorPoint().y
          );
}

void VVLog(const char* msg, CCNode* node)
{
    CCLog("%s ,Node: x_y(%0.0f,%0.0f) w_h(%0.0f,%0.0f) anchor(%f,%f)",msg, node->getPosition().x,node->getPosition().y,
          node->getContentSize().width,node->getContentSize().height,
          node->getAnchorPoint().x, node->getAnchorPoint().y
          );
}


void VVLog(CCRect rect)
{
    CCLog("Rect: x_y(%0.0f,%0.0f) w_h(%0.0f,%0.0f)",rect.origin.x, rect.origin.y,
          rect.size.width, rect.size.height);
}

void VVLog(const char* msg, CCRect rect)
{
    CCLog("%s, Rect: x_y(%0.0f,%0.0f) w_h(%0.0f,%0.0f)",msg, rect.origin.x, rect.origin.y,
          rect.size.width, rect.size.height);
}

void VVLog(CCSize sz)
{
    CCLog("Size:w_h(%0.0f,%0.0f)",
          sz.width, sz.height);
}

void VVLog(const char* msg, CCSize sz)
{
    CCLog("%s, Size:w_h(%0.0f,%0.0f)",msg,
          sz.width, sz.height);
}


void VVLog(CCPoint point)
{
    CCLog("Point: x_y(%0.0f,%0.0f)",point.x, point.y);
}

void VVLog(const char* msg, CCPoint point)
{
    CCLog("%s, Point: x_y(%0.0f,%0.0f)",msg, point.x, point.y);
}