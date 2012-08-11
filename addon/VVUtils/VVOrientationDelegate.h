//
//  VVOrientationDelegate.h
//  
//
//  Created by YoungJae Kwon on 12. 6. 17..
//  Copyright (c) 2012 Beyondi Soft All rights reserved.
//

#ifndef _VVOrientationDelegate_h
#define _VVOrientationDelegate_h

#include "cocos2d.h"
USING_NS_CC;


//
// Deprecated : CCScene에 그냥 구현해버렸음.
//

// this orientation represent GLView's orientation(not CCDirector's orientation!!)
typedef enum {
    VVOrientationPortrait,    
    VVOrientationLandscape
} VVOrientation;



class VVOrientationDelegate
{
public:
    virtual void onOrientationChange(VVOrientation orientation){}
};


#endif
