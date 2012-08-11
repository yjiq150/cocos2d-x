//
//  CCTouchArea.cpp
//  CocosX_test
//
//  Created by YoungJae Kwon on 11. 5. 21..
//  Copyright Viscuit Creative Inc 2011. All rights reserved.
//

#include "CCTouchArea.h"


USING_NS_CC;
CCTouchArea::CCTouchArea()
{
}

bool CCTouchArea::initWithColor(ccColor4B color)
{
	if ( !CCLayerColor::initWithColor(color) ) 
		return false;

	name = "";	
	
	return true;
}

CCTouchArea * CCTouchArea::layerWithColor(ccColor4B color)
{
	CCTouchArea * pLayer = new CCTouchArea();
	if(pLayer && pLayer->initWithColor(color))
	{
		pLayer->autorelease();
		return pLayer;
	}
	CC_SAFE_DELETE(pLayer);
	return NULL;
}
void CCTouchArea::setName(const char *c_str)
{
	name = c_str;
}
string CCTouchArea::getName()
{
	return name;
}

// Overide CCNode's getRect()
// 좌상단 기준 rect 리턴
CCRect CCTouchArea::getRect()
{
	return CCRectMake( m_tPosition.x, 
					  m_tPosition.y,
					  m_tContentSize.width, m_tContentSize.height);
}

void CCTouchArea::draw()
{		
	// Default GL states: GL_TEXTURE_2D, GL_VERTEX_ARRAY, GL_COLOR_ARRAY, GL_TEXTURE_COORD_ARRAY
	// Needed states: GL_VERTEX_ARRAY, GL_COLOR_ARRAY
	// Unneeded states: GL_TEXTURE_2D, GL_TEXTURE_COORD_ARRAY
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);

	//Added by YoungJae Kwon 터치 오브젝트 앵커값에따라 평행이동
	// 앵커포인트는 포인트계산, 이미지는 픽셀계산(레티나 디스플레이 고려해서 평행이동)
	glTranslatef(-m_tAnchorPoint.x * m_tContentSizeInPixels.width, -m_tAnchorPoint.y * m_tContentSizeInPixels.height, 0.0f);
	
	glVertexPointer(2, GL_FLOAT, 0, m_pSquareVertices);
	glColorPointer(4, GL_UNSIGNED_BYTE, 0, m_pSquareColors);
	
	bool newBlend = false;
	if( m_tBlendFunc.src != CC_BLEND_SRC || m_tBlendFunc.dst != CC_BLEND_DST ) {
		newBlend = true;
		glBlendFunc(m_tBlendFunc.src, m_tBlendFunc.dst);
	}
	else if( m_cOpacity != 255 ) {
		newBlend = true;
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	if( newBlend )
		glBlendFunc(CC_BLEND_SRC, CC_BLEND_DST);
	
	// restore default GL state
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
}

CCTouchArea::~CCTouchArea()
{
}