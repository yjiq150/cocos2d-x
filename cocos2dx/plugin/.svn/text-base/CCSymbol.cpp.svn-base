//
//  CCSymbol.cpp
//  CocosX_test
//
//  Created by YoungJae Kwon on 11. 5. 21..
//  Copyright Viscuit Creative Inc 2011. All rights reserved.
//

#include "CCSymbol.h"


USING_NS_CC;
CCSymbol::CCSymbol()
{
	m_eTouchDelegateType = ccTouchDelegateAllBit;
	// LEETAEHO 0.5 to 0.0
	//m_tAnchorPoint = ccp(0.5f, 0.5f);
	m_tAnchorPoint = ccp(0.0f, 0.0f);
	m_bIsRelativeAnchorPoint = false;
}

// on "init" you need to initialize your instance
bool CCSymbol::init()
{
	//부모클래스인 CCNode는 init이 없음.
	
	// 스프라이트와 같이 상대 앵커 포인트 사용
	this->setIsRelativeAnchorPoint(true);
	
	// 기본 투명도는 255 Max
	opacity = 255;
	
	// 심볼명 기본값 nil
	// name이 없을경우 PageBasicLayer의 심볼딕셔너리에 삽입 안됨
	name = "";
	
	minX = minY = maxX = maxY = 0;
	
	return true;
}

CCSymbol * CCSymbol::node()
{
	CCSymbol * pRet = new CCSymbol();
	pRet->init();
	pRet->autorelease();
	return pRet;
}
void CCSymbol::setName(const char *c_str)
{
	name = c_str;
}
const char* CCSymbol::getName()
{
	return name.c_str();
}

void CCSymbol::setInstanceName(const char *c_str)
{
	instanceName = c_str;
}
string CCSymbol::getInstanceName()
{
	return instanceName;
}


string CCSymbol::getClassName()
{
	string className("CCSymbol");
	return className;
}


void CCSymbol::setOpacity(GLubyte aOpacity)
{
	opacity = aOpacity;
	//각 그리기 개체의 draw()를 수정하는 방식으로 변경했음
	/*
	CCArray *childSprites = this->getChildren();

	// 차일드 노드가 없을경우 체크
	if (childSprites == NULL) 
		return;
	
	// 하위노드의 알파값들을 상위 알파비율에 맞게 적용하기 위해 0~255 -> 0~1.0 으로 변경
	float alpha = (float)opacity / 255.0f;
	
	for (int i = 0; i < childSprites->count(); i++)
	{
		CCNode *node = (CCNode*)childSprites->objectAtIndex(i);
		
		// 원래 개념 수식 : alpha * (node->getOpacity() / 255.0f) * 255;
		GLubyte newOpacity = (GLubyte)(alpha * (float)oriOpacityOfChildren[i]);
		node->setOpacity( newOpacity );
		CCLog("old = %d",node->getOpacity());
		CCLog("new = %d",newOpacity);
		//node->setOpacity( opacity );
	}
	 */
}

unsigned char CCSymbol::getOpacity()
{
	return opacity;
}

float CCSymbol::getOriScale()
{
	CCAssert( oriScaleX == oriScaleY, "Original scale X & Y are different! Don't know which one to return");
	return oriScaleX;
}

void CCSymbol::setOriScale(float s)
{
	oriScaleX = oriScaleY = s;
}

// 심볼 값(초기 position, rotaion 등)을 초기화 하고 스프라이트 등의 자식노드가 모두 추가된 이후에 실행한다.
void CCSymbol::updateSymbol()
{
	// original 값들 저장 (액션을 걸더라도 다시 원상 복귀 위해서 초기값 저장)

	oriPosition = m_tPosition;
	
	oriRotation = m_fRotation;
	oriScaleX = m_fScaleX;
	oriScaleY = m_fScaleY;
	
	CCSymbol::calculateSymbolSize();
}


// 심볼내의 노드들을 기초로해서 심볼 가로세로 크기를 구한다
// CCSymbol의 하위노드로는 CCSprite와 CCSymbol만 있다고 가정한 계산법
//MOD: 추후 Shapte를 지원하도록 코드 작성하기.
//MOD: 레티나에서 작동 안할 수 있음. child.contentsize의 scale factor 확인해줄것

void CCSymbol::calculateSymbolSize()
{
	// 초기값 리셋
	// calculateSymbolSize를 두번째로 호출해도 정상적인 계산이 되도록
	//MOD:두번이상 호출시 뭔가 잘 안된다... 원인분석할것
	minX = minY = maxX = maxY = 0;	
	bool isFirstChild = true;
	
	// 심볼내부에 요소가 없을경우 사이즈는 그대로 0
	if( this->getChildren()->count() == 0)
		return;
	
	CCObject *object;
	CCNode *child;
	CCARRAY_FOREACH(this->getChildren(), object)
	{
		child = (CCNode*)object;
		string str("CCSymbol");
		string te = child->getClassName();
		if (child->getClassName() == str) 
			((CCSymbol*)child)->calculateSymbolSize();

		if (isFirstChild) 
		{
			isFirstChild = false;
			// anchor포인트 고려
			minX = child->getPosition().x - child->getContentSize().width*(  child->getAnchorPoint().x);
			maxX = child->getPosition().x + child->getContentSize().width*(1-child->getAnchorPoint().x);
			minY = child->getPosition().y - child->getContentSize().height*(    child->getAnchorPoint().y);
			maxY = child->getPosition().y + child->getContentSize().height*(1 - child->getAnchorPoint().y);;
			
//			minX = child->getPosition().x;
//			maxX = child->getPosition().x + child->getContentSize().width;
//			minY = child->getPosition().y - child->getContentSize().height;
//			maxY = child->getPosition().y ;
			continue;
		}
		
		float value;
		
		value = child->getPosition().x - child->getContentSize().width*(  child->getAnchorPoint().x);
		if( value < minX )
			minX = value;
		
		value = child->getPosition().x + child->getContentSize().width*(1-child->getAnchorPoint().x);
		if (value > maxX) 
			maxX = value;
		
		
		
		// y좌표계는 (-)방향임에 주의
		value = child->getPosition().y - child->getContentSize().height*(    child->getAnchorPoint().y);
		if( value < minY )
			minY = value;
		
		value = child->getPosition().y + child->getContentSize().height*(1 - child->getAnchorPoint().y);
		if (value > maxY) 
			maxY = value;
	}
	
	//MOD: 앵커가 0,1일경우는 좌표가 그만큼 내려가서 draw된다.. 왜그렇지?
	// 심볼자체에 앵커 큰의미 두지 말것. 기본값 (0,0)을 계속 사용하면 된다.
	//self.anchorPoint = ccp(0,0);

	float width = maxX-minX;
	float height = maxY-minY;
	this->setContentSize( CCSize(width, height) );
	
	// setAnchorPoint사용시 다른 값들이 없데이트 되므로 사용하면 안되고
	//	this->setAnchorPoint(ccp(-minX/(width), -minY/height)); 사용금지!!
	// 심볼자체의 앵커 값 자체만 살짝 변경해준다( 이값을 셋팅해 놓아야 하위노드에 CCSymbol이 있을때 그 심볼의 크기를 적절히 계산해준다)
	m_tAnchorPoint = ccp(-minX/(width), -minY/height);

	
}

// CCNode의 getRect를 CCSymbol에 맞게 오버라이딩
CCRect CCSymbol::getRect()
{	
//   CC_CONTENT_SCALE_FACTOR()
	return CCRectMake( m_tPosition.x+minX,  m_tPosition.y+minY,
					  m_tContentSize.width, m_tContentSize.height);
}
void CCSymbol::draw()
{
	/*
	// 심볼 디버그용
	CCSize s = m_tContentSize;
    CCPoint vertices[4] = {
        ccp(minX,+minY), ccp(s.width+minX,+minY),
        ccp(s.width+minX,s.height+minY), ccp(+minX,s.height+minY)
    };
	glColor4f(255, 0, 0, 255);
    ccDrawPoly(vertices, 4, true);
	 */
	
}
void CCSymbol::destroy(void)
{
	this->release();
}

void CCSymbol::keep(void)
{
	this->retain();
}

void CCSymbol::onEnter()
{
	CCNode::onEnter();
}
void CCSymbol::onExit()
{
	CCNode::onExit();
}

void CCSymbol::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	if (isScriptHandlerExist(CCTOUCHBEGAN))
	{
		excuteScriptTouchesHandler(CCTOUCHBEGAN, pTouches);
	}
}

void CCSymbol::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	if (isScriptHandlerExist(CCTOUCHENDED))
	{
		excuteScriptTouchesHandler(CCTOUCHENDED, pTouches);
	}
}

void CCSymbol::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	if (isScriptHandlerExist(CCTOUCHMOVED))
	{
		excuteScriptTouchesHandler(CCTOUCHMOVED, pTouches);
	}
}

void CCSymbol::ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent)
{
	if (isScriptHandlerExist(CCTOUCHCANCELLED))
	{
		excuteScriptTouchesHandler(CCTOUCHCANCELLED, pTouches);
	}
}

CCSymbol::~CCSymbol()
{
	//CCLog("symbol dest");
}