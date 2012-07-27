//
//  CCSymbol.h
//  CocosX_test
//
//  Created by YoungJae Kwon on 11. 5. 21..
//  Copyright Viscuit Creative Inc 2011. All rights reserved.
//

#ifndef __CCSYMBOL_H__
#define __CCSYMBOL_H__

#include "cocos2d.h"
#include "CCTouchDelegateProtocol.h"

using namespace cocos2d;


class CCSymbol : public CCNode,public CCTouchDelegate
{
	
public:
	CCSymbol();
	virtual ~CCSymbol();
	virtual bool init();  
	static CCSymbol *node();
	
	virtual void onEnter();
	virtual void onExit();
	virtual void draw();
	//심볼기준점으로부터 좌우로 터치영역 계산을 위해 min/max값 갖고있기
	CC_SYNTHESIZE_READONLY(float, minX, MinX);
	CC_SYNTHESIZE_READONLY(float, minY, MinY);
	CC_SYNTHESIZE_READONLY(float, maxX, MaxX);
	CC_SYNTHESIZE_READONLY(float, maxY, MaxY);
	
	
	
	// 액션 후에 원래 상태로 복귀하기 위한 오리지널 값 저장
	CC_SYNTHESIZE(CCPoint, oriPosition,OriPosition); 
	CC_SYNTHESIZE(float, oriRotation,OriRotation);
	CC_SYNTHESIZE(float, oriScaleX,OriScaleX);
	CC_SYNTHESIZE(float, oriScaleY,OriScaleY);
	
	float getOriScale();
	void setOriScale(float s);
	
	// original값들 저장 + 심볼사이즈 재계산
	void updateSymbol();
	
	// updateSymbol 호출 후에만 불려질 수 있다.
	virtual CCRect getRect();
	
	// 해당 심볼자체의 이름
	void setName(const char *c_str);
	const char* getName();

	// 심볼로 생성된 인스턴스
	void setInstanceName(const char *c_str);
	string getInstanceName();
	
	string getClassName();
	
	void setOpacity(GLubyte aOpacity);
	GLubyte getOpacity();
	
	//주의: CCTouchDelegate를 상속받을경우 destroy와 keep을 구현해야한다(cocos2dx에만 해당되는 메서드)
	virtual void destroy(void);
	virtual void keep(void);
	
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
	
protected:
	//심볼 내부 객체들이 차지하고있는 영역으로부터 심볼 컨텐츠 사이즈를 계산
	void calculateSymbolSize();
		
	string name;
	string instanceName;
	GLubyte opacity;
};

#endif // __CCSYMBOL_H__
