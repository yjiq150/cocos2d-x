//
//  CCTouchArea.h
//  CocosX_test
//
//  Created by YoungJae Kwon on 11. 5. 21..
//  Copyright Viscuit Creative Inc 2011. All rights reserved.
//

#ifndef __CCTOUCHAREA_H__
#define __CCTOUCHAREA_H__

#include "cocos2d.h"

// 디버그 모드일때는 레이어 컬러가 회색빛이 되도록
#ifdef COCOS2D_DEBUG
#define TOUCH_LAYER_COLOR ccc4(100,100,100,170)
//	#define TOUCH_LAYER_COLOR ccc4(0,0,0,0)
#else
	#define TOUCH_LAYER_COLOR ccc4(0,0,0,0)
#endif

using namespace cocos2d;

class CCTouchArea : public CCLayerColor
{
public:
	CCTouchArea();
	virtual ~CCTouchArea();
	virtual bool initWithColor(ccColor4B color);
	static CCTouchArea * layerWithColor(ccColor4B color);
	
	void setName(const char *c_str);
	string getName();
	
	LAYER_NODE_FUNC(CCTouchArea);
	virtual CCRect getRect();
	virtual void draw();

protected:
	string name;
};

#endif // __CCTOUCHAREA_H__
