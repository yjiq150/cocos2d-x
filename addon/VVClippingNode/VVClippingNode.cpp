//
//  VVClippingNode.cpp
//  Cocos2dxTemplate
//
//  Created by YoungJae Kwon on 12. 5. 2..
//  Copyright (c) 2012년 Viscuit Creative Inc. All rights reserved.
//

#include <iostream>
#include "VVClippingNode.h"

void VVClippingNode::beforeDraw()
{
    // draw
//    CCPoint vertices[4] = {
//        ccp(0,0), ccp(200,70),
//        ccp(200,500), ccp(50,200)
//    };



    glClear(GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    //glClear(GL_STENCIL_BUFFER_BIT);
    
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS,1, 0xffffffff);
    glStencilOp(GL_REPLACE, GL_REPLACE, GL_REPLACE);
    
        ccFillPolyStencil(clippingArea, 4); // 잘라낼 모양(클리핑패스) 스텐실버퍼에 드로잉
    
    glStencilFunc(GL_EQUAL, 1, 0xffffffff);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    
    
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
    
    glPushMatrix();
}

void VVClippingNode::draw()
{
#ifdef CLIPPING_AREA_DEBUG_ON
    // glBlendFunc(GL_ONE, GL_ZERO); 
    // 스텐실 테스트를 이용해 드로잉하자.
    // ccFillPoly(vertices, 4); 
    CCNode::draw();
#endif    
}

void VVClippingNode::afterDraw()
{
    glPopMatrix();
    
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    
    //Debug: 잘리는영역 표기
    ccDrawPoly(clippingArea, 4,true);
}