//
//  VVMultiScreenManager.h
//  
//
//  Created by YoungJae Kwon on 12. 6. 17..
//  Copyright (c) 2012 Beyondi Soft All rights reserved.
//

#ifndef _VVMultiScreenManager_h
#define _VVMultiScreenManager_h

#include "cocos2d.h"

typedef enum {
    VVOrientationPortrait,
    VVOrientationLandscape
} VVOrientation;

USING_NS_CC;

//#define VVPOS(_arg_) VVMultiScreenManager::shared()->getConvertedCoordinate(_arg_)


// this orientation represent GLView's orientation(not CCDirector's orientation!!)

class VVMultiScreenManager
{
public:
    static VVMultiScreenManager* shared();
    
    VVMultiScreenManager();
    
    // calculateWinSize를 1. 앱실행시  2. Orientation change시 호출하여 업데이트 할것.
    // original : 원래 앱이 디자인된 스크린 사이즈
    // target : 원래 앱을 강제 스케일링 할 현재 디바이스 스크린 사이즈
    void calculateWinSize(CCSize aOrigialSize, CCSize aTargetSize);
    CCSize getWinSize();
    CCSize getWinSizeInPixel();
    
    float getScaleFactorX();
    float getScaleFactorY();
    float getOffsetX();
    float getOffsetY();    
    

    
    CCPoint getOffset();

    float getConvertedCoordinate(float aPos);

    //deprecated: 단순히 updateOrientation하면 1232 800 -> 1280 752 변화를 잡아내지 못한다.
    void updateOrientation(bool isPortrait);


protected:
    virtual float convert(float original){ return original; }
    
    CCSize size;
    CCPoint offset;
    float scaleFactorX;
    float scaleFactorY;
    
    CC_SYNTHESIZE(VVOrientation, orientation, Orientation);

};


#endif
