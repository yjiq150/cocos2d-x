//
//  VVClippingNode.h
//  Cocos2dxTemplate
//
//  Created by YoungJae Kwon on 12. 5. 2..
//  Copyright (c) 2012년 Viscuit Creative Inc. All rights reserved.
//

#ifndef Cocos2dxTemplate_VVClippingNode_h
#define Cocos2dxTemplate_VVClippingNode_h

#include "cocos2d.h"


// 만들다 말았음

// 계획.
// 클리핑 텍스쳐 만들기
// 터치 포인트 경로를 읽어들인다, 경로대로 드로잉을 한다(기존 튜토리얼 이용).

// 텍스쳐 테두리 입히기
// 렌더텍스쳐에서 특정값은 특정 컬러로 치환하는 방식?


class VVClippingNode : public CCNode
{
public:
    VVClippingNode()
    {
        
        clippingArea[0] = ccp(0, 0);
        clippingArea[1] = ccp(1024,0);
        clippingArea[2] = ccp(1024,768);
        clippingArea[3] = ccp(0, 768);
    }
    void setClipppingArea(CCRect r)
    {
        clippingArea[0] = ccp(r.origin.x, r.origin.y);
        clippingArea[1] = ccp(r.origin.x+r.size.width, r.origin.y);
        clippingArea[2] = ccp(r.origin.x+r.size.width, r.origin.y+r.size.height);
        clippingArea[3] = ccp(r.origin.x,r.origin.y+r.size.height);        
    }
    
protected:
    virtual void beforeDraw();
    virtual void draw();
    virtual void afterDraw();
    
    CCPoint clippingArea[4];
};


#endif
