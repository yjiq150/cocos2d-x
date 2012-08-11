/*
 *  LCWordCloudLayer.h
 *  Collins_Dictionary_iphone
 *
 *  Created by 태호 이 on 11. 8. 11..
 *  Copyright 2011 Viscuit Creative. All rights reserved.
 *
 */

#ifndef __LC_WordCloud_LAYER_H__
#define __LC_WordCloud_LAYER_H__

#include "cocos2d.h"


typedef struct  {
	float x;
	float y;
	float z;
} LCVector3;

USING_NS_CC;

class LCWordCloudLayerDelegate;

class LCWordCloudLayer : public CCLayer
{
protected:
	// rotation animation
	float m_fTotalAngle;
	float m_fExponentialConstant;
	CCArray* m_pPositionArray;
	LCVector3 m_tAxisVector;
	
	virtual void updateRotation(ccTime dt);
	
	// mathematic calculate functions
	float convertDegreeToRadian(float degree);
	int findNumberOfSeperation(int number);
	CCArray* makeInitialPositionArray(int n);
	
	// touch control
	bool m_bIsMoved;
	
	// manage nodes
	virtual void resetNodesPosition();
	virtual void syncronizeNodesPosition();
	
	// added by YoungJAe Kwon
	string fontName;
	int fontSize;
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )	
	// 안드로이드용 터치체크 코드
	CCPoint initialTouchLocation;
#endif
	
public:
	LCWordCloudLayer();
	virtual ~LCWordCloudLayer();
	
	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();
	
	// initialize with nodes
	virtual bool init(CCArray* nodeArray);
	
	// add objects
	virtual void addNode(CCNode* pNode);
	virtual void addNodes(CCArray* pArray);
	virtual void addText(const char* pText);
	virtual void addTexts(CCArray* pArray);
	//virtual void addTexts(int count, ...);
	
	// remove objects
	virtual void removeAll();
	
	// implement CCTouchDelegate
	virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
	virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);	
	
	// delegate
	CC_SYNTHESIZE(LCWordCloudLayerDelegate*, m_pLCWordCloudLayerDelegate, LCWordCloudLayerDelegate);
	
	// nodeArray
	CC_PROPERTY_READONLY(CCArray*, m_pNodeArray, NodeArray);
	
	// setting values
	CC_PROPERTY(float, m_fMaxOpacity, MaxOpacity);
	CC_PROPERTY(float, m_fMinOpacity, MinOpacity);
	CC_PROPERTY(float, m_fRadius, Radius);
	CC_PROPERTY(float, m_fMinScale, MinScale);
	CC_PROPERTY(float, m_fMaxSpeed, MaxSpeed);
	CC_PROPERTY(float, m_fMouseSensitivity, MouseSensitivity);
	CC_PROPERTY(float, m_fCurrentSpeed, CurrentSpeed);
	
	
	// added by YoungJAe Kwon
	void setFont(const char* fontName, int fontSize);	

//#define _MAX_OPACITY_ 255	// 0 ~ 255
//#define _MIN_OPACITY_ 50	// 0 ~ 255
//#define _GLOBE_RADIUS_ 100	
//#define _MIN_SCALE_ 0.3		// 0 ~ 1
//#define _MAX_SPEED_ 1.0f
//#define _MOUSE_SENSITIVITY_ 1.0f
};

class LCWordCloudLayerDelegate
{
public:
	virtual void wordCloudDidSelectedWord(LCWordCloudLayer* wordCloudLayer, CCNode *pNode);
};

class LCVector3Container : public CCObject {
protected:
	LCVector3 m_tVector;
public:
	
	// constructor and destructor
	LCVector3Container();
	virtual ~LCVector3Container();
	static LCVector3Container* makeVector3Container(float x, float y, float z);
	
	// mathematic calculate vector functions
	LCVector3Container* convertToUnitVector();
	float length();
	LCVector3Container* scalarMult(float k);
	LCVector3Container* rotationWithNormalVector(LCVector3 u, float theta);
	
	// getter and setter
	virtual LCVector3 getVector();
	virtual void setVector(LCVector3 tVector);
};

#endif // __LC_WordCloud_LAYER_H__
