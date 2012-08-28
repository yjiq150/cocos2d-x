//
//  VVMultiScreenManager.cpp
//  
//
//  Created by YoungJae Kwon on 12. 6. 17..
//  Copyright (c) 2012 Beyondi Soft All rights reserved.
//

#include "VVMultiScreenManager.h"
#include <iostream>
#include "VVLog.h"
USING_NS_CC;

static VVMultiScreenManager* s_shared;

VVMultiScreenManager* VVMultiScreenManager::shared()
{
	if (s_shared == NULL)
	{
        s_shared = new VVMultiScreenManager();
	}
    
	return s_shared;
}

VVMultiScreenManager::VVMultiScreenManager()
{
    scaleFactorX = 1.0f;
    scaleFactorY = 1.0f;

}

// 

void VVMultiScreenManager::calculateWinSize(CCSize aOriginalSize, CCSize aTargetSize)
{
    // 스케일 계산
    if( aOriginalSize.width != aTargetSize.width )
        scaleFactorX = aTargetSize.width / aOriginalSize.width;
    else
        scaleFactorX = 1;
    
    if( aOriginalSize.height != aTargetSize.height )
        scaleFactorY = aTargetSize.height / aOriginalSize.height;
    else
        scaleFactorY = 1;
    
    size = aOriginalSize;
    
    // 좌표 오프셋 계산
    offset.x = (aTargetSize.width - aOriginalSize.width)/2;
    offset.y = (aTargetSize.height - aOriginalSize.height)/2;
    
    // 오리엔테이션 설정
    if( size.width > size.height)
        orientation = VVOrientationLandscape;
    else
        orientation = VVOrientationPortrait;
#ifdef COCOS2D_DEBUG
    CCLog("----------Calculate MultiscreenManager---------");
    VVLog("original:",aOriginalSize);
    VVLog("target:",aTargetSize);
    CCLog("scale x:%f, y:%f",scaleFactorX,scaleFactorY);
    VVLog("offset:",offset);
    CCLog("-----------------------------------------------");
#endif
    
}

// 오리지널 구성된 화면의 윈도우 크기를 리턴
// 현재 실제 디바이스 사이즈는 CCDirector::getWinSize()를 이용할것.
CCSize VVMultiScreenManager::getWinSize()
{
    return size;
}

CCSize VVMultiScreenManager::getWinSizeInPixel()
{
    CCSize s = getWinSize();
    
	s.width *= CC_CONTENT_SCALE_FACTOR();
	s.height *= CC_CONTENT_SCALE_FACTOR();
    
	return s;
}

float VVMultiScreenManager::getScaleFactorX()
{
    return scaleFactorX;
}

float VVMultiScreenManager:: getScaleFactorY()
{
    return scaleFactorY;
}

float VVMultiScreenManager::getOffsetX()
{ 
    return offset.x;
}
float VVMultiScreenManager::getOffsetY()
{ 
    return offset.y;
}

CCPoint VVMultiScreenManager::getOffset()
{    
    return offset;
}

float VVMultiScreenManager::getConvertedCoordinate(float aPos)
{
    return aPos;
}

//deprecated: 단순히 updateOrientation하면 1232 800 -> 1280 752 변화를 잡아내지 못한다.
void VVMultiScreenManager::updateOrientation(bool isPortrait)
{
    if (isPortrait)
    {
        if( orientation == VVOrientationLandscape )
        {
            CCSize tmpSize = size;
            size.width = tmpSize.height;
            size.height = tmpSize.width;
            
            CCPoint tmpOffset = offset;
            offset.x = tmpOffset.y;
            offset.y = tmpOffset.x;
            
            float tmpScale = scaleFactorX;
            scaleFactorX = scaleFactorY;
            scaleFactorY = tmpScale;
        }
        orientation = VVOrientationPortrait;
    }
    else
    {
        if( orientation == VVOrientationPortrait )
        {
            CCSize tmpSize = size;
            size.width = tmpSize.height;
            size.height = tmpSize.width;

            CCPoint tmpOffset = offset;
            offset.x = tmpOffset.y;
            offset.y = tmpOffset.x;
            
            float tmpScale = scaleFactorX;
            scaleFactorX = scaleFactorY;
            scaleFactorY = tmpScale;
        }

        orientation = VVOrientationLandscape;
    }
}


/*
void VVMultiScreenManager::update()
{ 
    // CCDirector는 항상 Portrait 방향임을 가정한다.
    //    VVLog("winSize in point", CCDirector::sharedDirector()->getWinSize());
    //    VVLog("winSize in px",CCDirector::sharedDirector()->getWinSizeInPixels());
    //    VVLog("Device ScrfeenSize",CCDirector::sharedDirector()->getDeviceScreenSize());        
    //    VVLog("dsp in px",CCDirector::sharedDirector()->getDisplaySizeInPixels());
    
//    offset.x = -(1280 - CCDirector::sharedDirector()->getWinSizeInPixels().width)/2;
//    offset.y = 0;
    
    //ToDo, 레티나 디스플레이가 false일때?
    //CCDirector::sharedDirector()->isRetinaDisplay();
}

*/

