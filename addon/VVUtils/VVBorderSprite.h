/*
 *  VVBorderSprite.h
 *  CollinsPictureCardHD
 *
 *  Created by YoungJae Kwon on 11. 11. 1..
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */

#ifndef __VVBorderSprite_H__
#define __VVBorderSprite_H__

#include "cocos2d.h"


class VVBorderSprite
{
public:

	// 스프라이트 외부 border로 이용될 수 있도록 해당스프라이트보다 borderSize만큼 큰 allWhite Texture를 리턴
	static CCTexture2D* createBorderTextureWithSprite(CCSprite* spr, float borderSize);

		
protected:
};

#endif