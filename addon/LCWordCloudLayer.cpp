/*
 *  LCWordCloudLayer.cpp
 *  Collins_Dictionary_iphone
 *
 *  Created by 태호 이 on 11. 8. 11..
 *  Copyright 2011 Viscuit Creative. All rights reserved.
 *
 */

#include "LCWordCloudLayer.h"
#include "LCUtil.h"

USING_NS_CC;

#define _MAX_OPACITY_ 255	// 0 ~ 255
#define _MIN_OPACITY_ 50	// 0 ~ 255
#define _GLOBE_RADIUS_ 100	
#define _MIN_SCALE_ 0.3f	// 0 ~ 1
#define _MAX_SPEED_ 1.0f
#define _MOUSE_SENSITIVITY_ 1.0f

#pragma mark -
#pragma mark constructor and destructor
LCWordCloudLayer::LCWordCloudLayer():m_pNodeArray(NULL), m_pPositionArray(NULL)
{
	// added by YoungJAe Kwon
	fontName = "Thonburi";
	fontSize = 30;
}

LCWordCloudLayer::~LCWordCloudLayer()
{
	CC_SAFE_RELEASE_NULL(m_pNodeArray);
	CC_SAFE_RELEASE_NULL(m_pPositionArray);
}

#pragma mark -
#pragma mark initialize
bool LCWordCloudLayer::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}
	
	m_pNodeArray = CCArray::array();
	m_pNodeArray->retain();
	
	m_pPositionArray = CCArray::array();
	m_pPositionArray->retain();
	
	this->setMaxOpacity(_MAX_OPACITY_);
	this->setMinOpacity(_MIN_OPACITY_);
	this->setRadius(_GLOBE_RADIUS_);
	this->setMinScale(_MIN_SCALE_);
	this->setMaxSpeed(_MAX_SPEED_);
	this->setMouseSensitivity(_MOUSE_SENSITIVITY_);
	
	LCVector3Container* container = LCVector3Container::makeVector3Container(1, 1, 0);
	container->rotationWithNormalVector((LCVector3){0, 0, 1}, convertDegreeToRadian(-90));
	m_tAxisVector = container->getVector();
	
	this->schedule(schedule_selector(LCWordCloudLayer::updateRotation));
	
	this->setIsTouchEnabled(true);
	
	return true;
}

/*
 * initialize with nodes
 * input : CCNode Array to show in cloud
 */
bool LCWordCloudLayer::init(CCArray* nodeArray)
{
	if ( !this->init()) {
		return false;
	}
	
	this->addNodes(nodeArray);
	
	return true;
}


#pragma mark -
#pragma mark Add and Remove Nodes
void LCWordCloudLayer::addNode(CCNode* pNode)
{
	this->m_pNodeArray->addObject(pNode);
	this->resetNodesPosition();
}

void LCWordCloudLayer::addNodes(CCArray* pArray)
{
	this->m_pNodeArray->addObjectsFromArray(pArray);
	this->resetNodesPosition();
}
void LCWordCloudLayer::addText(const char* pText)
{
	CCAssert(fontName.length(),"setFont should be called first!");
	
	CCLabelTTF* label = CCLabelTTF::labelWithString(pText, fontName.c_str(), fontSize);
	this->m_pNodeArray->addObject(label);
	this->resetNodesPosition();
	
	// color setting
	label->setColor(ccc3(rand()%150 + 105, rand()%150 + 105, rand()%150 + 105));
}

void LCWordCloudLayer::addTexts(CCArray* pArray)
{
	CCAssert(fontName.length(),"setFont should be called first!");
	
	for (int i=0; i<pArray->count(); i++) {
		CCString* str = (CCString*) pArray->objectAtIndex(i);
		CCLabelTTF* label = CCLabelTTF::labelWithString(str->m_sString.c_str(), fontName.c_str(), fontSize);
		this->m_pNodeArray->addObject(label);
		
		// color setting
		label->setColor(ccc3(rand()%150 + 105, rand()%150 + 105, rand()%150 + 105));
	}
	this->resetNodesPosition();
}
//virtual void addTexts(int count, ...);

// remove objects
void LCWordCloudLayer::removeAll()
{
	for (int i=0; i<m_pNodeArray->count(); i++) {
		CCNode* node = (CCNode*) m_pNodeArray->objectAtIndex(i);
		if (node->getParent()) {
			node->removeFromParentAndCleanup(true);
		}
	}
	this->m_pNodeArray->removeAllObjects();
	this->resetNodesPosition();
}

#pragma mark -
#pragma mark implement CCTouchDelegate
void LCWordCloudLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )	
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )	
	CCTouch *touch = (CCTouch*)pTouches->anyObject();
	initialTouchLocation = touch->locationInView( touch->view() );
#endif
	
	m_bIsMoved = false;
}

void LCWordCloudLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch *) pTouches->anyObject();
	CCPoint touchLocation = touch->locationInView( touch->view() );	
	CCPoint prevLocation = touch->previousLocationInView( touch->view() );	
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS )
	m_bIsMoved = true;
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )	
	if (touchLocation.x == initialTouchLocation.x && touchLocation.y == initialTouchLocation.y) 
		m_bIsMoved = false;
	else 
		m_bIsMoved = true;
#endif

	
	touchLocation = CCDirector::sharedDirector()->convertToGL( touchLocation );
	prevLocation = CCDirector::sharedDirector()->convertToGL( prevLocation );
	
	CCPoint diff = ccpSub(touchLocation,prevLocation);
	
	LCVector3Container* container = LCVector3Container::makeVector3Container(diff.x, diff.y, 0);
	container->rotationWithNormalVector((LCVector3){0, 0, 1}, convertDegreeToRadian(-90));
	container->scalarMult(m_fMouseSensitivity * 0.015);
	m_tAxisVector.x += container->getVector().x;
	m_tAxisVector.y += container->getVector().y;
	
	LCVector3Container* velocityContainer = LCVector3Container::makeVector3Container(m_tAxisVector.x, m_tAxisVector.y, m_tAxisVector.z);
	if (velocityContainer->length() > m_fMaxSpeed*2.0f) {
		m_tAxisVector = velocityContainer->convertToUnitVector()->scalarMult(m_fMaxSpeed*2.0f)->getVector();
	}
}

void LCWordCloudLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	if (!m_bIsMoved) {
		// tap action : select something
		CCNode *selectedNode = NULL;
		for (int i=0; i<m_pNodeArray->count(); i++) {
			CCNode *pNode = (CCNode *)m_pNodeArray->objectAtIndex(i);
			if (LCUtil::isTouchInside(pNode, pTouches)) {
				if (selectedNode) {
					if (selectedNode->getZOrder() < pNode->getZOrder()) {
						selectedNode = pNode;
					}
				} else {
					selectedNode = pNode;
				}
				
			}
		}
		
		if (selectedNode) {
			if (m_pLCWordCloudLayerDelegate) {
				m_pLCWordCloudLayerDelegate->wordCloudDidSelectedWord(this, selectedNode);
			} else {
				CCLog("You can control selected node event with WordCloudLayerDelegate::didWordSelected");
			}
		}
	}
}

#pragma mark -
#pragma mark Setters and Getters
CC_PROPERTY_SYNTHESIZE_READONLY(CCArray*, m_pNodeArray, NodeArray, LCWordCloudLayer);
CC_PROPERTY_SYNTHESIZE(float, m_fMaxOpacity, MaxOpacity, LCWordCloudLayer);
CC_PROPERTY_SYNTHESIZE(float, m_fMinOpacity, MinOpacity, LCWordCloudLayer);
CC_PROPERTY_SYNTHESIZE_READONLY(float, m_fRadius, Radius, LCWordCloudLayer);
CC_PROPERTY_SYNTHESIZE_READONLY(float, m_fMinScale, MinScale, LCWordCloudLayer);
CC_PROPERTY_SYNTHESIZE(float, m_fMaxSpeed, MaxSpeed, LCWordCloudLayer);
CC_PROPERTY_SYNTHESIZE(float, m_fMouseSensitivity, MouseSensitivity, LCWordCloudLayer);

void LCWordCloudLayer::setRadius(float value){this->m_fRadius = value; m_fExponentialConstant = powf(10, log10(m_fMinScale)/m_fRadius/2);}
void LCWordCloudLayer::setMinScale(float value){this->m_fMinScale = value; m_fExponentialConstant = powf(10, log10(m_fMinScale)/m_fRadius/2);}
void LCWordCloudLayer::setCurrentSpeed(float value)
{
	LCVector3Container *velocityVectorContainer = LCVector3Container::makeVector3Container(m_tAxisVector.x, m_tAxisVector.y, m_tAxisVector.z);
	if (value > m_fMaxSpeed) {
		value = m_fMaxSpeed;
	}
	velocityVectorContainer->convertToUnitVector()->scalarMult(value);
	m_tAxisVector = velocityVectorContainer->getVector();
}
float LCWordCloudLayer::getCurrentSpeed()
{
	LCVector3Container *velocityVectorContainer = LCVector3Container::makeVector3Container(m_tAxisVector.x, m_tAxisVector.y, m_tAxisVector.z);
	return velocityVectorContainer->length();
}


#pragma mark -
#pragma mark protected methods

#pragma mark -
#pragma mark rotation animation
void LCWordCloudLayer::updateRotation(ccTime dt)
{
	LCVector3Container *velocityVectorContainer = LCVector3Container::makeVector3Container(m_tAxisVector.x, m_tAxisVector.y, m_tAxisVector.z);
	float velocity = velocityVectorContainer->length();
	for (int i=0; i<m_pPositionArray->count(); i++) {
		LCVector3Container *container = (LCVector3Container *) m_pPositionArray->objectAtIndex(i);
		container->rotationWithNormalVector(m_tAxisVector, dt * velocity);
	}
	this->syncronizeNodesPosition();
}

#pragma mark -
#pragma mark mathematic calculate functions
float LCWordCloudLayer::convertDegreeToRadian(float degree)
{
	return degree/180.0f*M_PI;
}

int LCWordCloudLayer::findNumberOfSeperation(int number)
{
	for (int n=1; n<30; n++) {
		float result = 0;
		for (int k=0; k<=n; k++) {
			result += 2 * n * sinf(M_PI * k / n);
		}
		CCLog("%d : %f", n, result);
		if (result > number) {
			return n;
		}
	}
	return 30;
}

CCArray* LCWordCloudLayer::makeInitialPositionArray(int n)
{
	srand(time(NULL));
	CCLog("n : %d", n);
	CCArray* initialPositionArray = CCArray::array();
	int count = 0;
	for (int i=0; i<n; i++) {
		float z = m_fRadius * cosf(M_PI / n * i);
		float randomAngle = (float)rand()/(float)RAND_MAX * M_PI;
		float numberOfDot = 2 * n * sinf(M_PI * i / n);
		for (int j=0; j < numberOfDot - 1; j++) {
			float theta = 2 * M_PI / numberOfDot * j + randomAngle;
			float x = m_fRadius * sinf(i * M_PI / n) * cosf(theta);
			float y = m_fRadius * sinf(i * M_PI / n) * sinf(theta);
			
			initialPositionArray->addObject(LCVector3Container::makeVector3Container(x, y, z));
			count++;
		}
	}
	CCLog("count  : %d", count);
	return initialPositionArray;
}

#pragma mark -
#pragma mark manage nodes
// manage nodes
void LCWordCloudLayer::resetNodesPosition()
{
	m_pPositionArray->removeAllObjects();
	int numberOfSeperation = findNumberOfSeperation(m_pNodeArray->count());
	CCArray *initPositionArray = makeInitialPositionArray(numberOfSeperation);
	if (initPositionArray->count() < m_pNodeArray->count()) {
		initPositionArray = makeInitialPositionArray(numberOfSeperation + 1);
	}
	for (int i=0; i<m_pNodeArray->count(); i++) {
		LCVector3Container* container = (LCVector3Container*) initPositionArray->randomObject();
		if (container != NULL) 
		{
			initPositionArray->removeObject(container);
			m_pPositionArray->addObject(container);
			
		}
			
		CCNode* node = (CCNode*) m_pNodeArray->objectAtIndex(i);
		if (!node->getParent()) {
			this->addChild(node);
		}
	}
	this->syncronizeNodesPosition();
}
void LCWordCloudLayer::syncronizeNodesPosition()
{
	// syncronize node position with vector
	for (int i=0; i<m_pPositionArray->count(); i++) {
		LCVector3Container *container = (LCVector3Container *) m_pPositionArray->objectAtIndex(i);
		container->convertToUnitVector()->scalarMult(m_fRadius);
		
		CCNode *node = (CCNode *)m_pNodeArray->objectAtIndex(i);
	//	CCLog("%f %f %f", container->getVector().x, container->getVector().y ,container->getVector().z);
		node->setPosition(ccp(container->getVector().x, container->getVector().y));
		node->setScale(powf(m_fExponentialConstant, container->getVector().z + m_fRadius));
		
		//commented by YoungJae Kwon 이 계산결과를 GLUByte로 환산하면 ndk에서 에러난다, I don't know why.
		// 따라서 int형에 넣어서 대입.
		int test = -container->getVector().z / m_fRadius * ((m_fMaxOpacity - m_fMinOpacity) / 2) + m_fMaxOpacity - ((m_fMaxOpacity - m_fMinOpacity) / 2);
		node->setOpacity(test);
		this->reorderChild(node, container->getVector().z * -1000);
	}
}

void LCWordCloudLayer::setFont(const char* aFontName, int aFontSize)
{
	fontName = aFontName;
	fontSize = aFontSize;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark LCWordCloudLayerDelegate Class
#pragma mark -
void LCWordCloudLayerDelegate::wordCloudDidSelectedWord(LCWordCloudLayer* wordCloudLayer, CCNode *pNode)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
#pragma mark LCVector3Container Class
#pragma mark -
#pragma mark Constructor and Destructor
LCVector3Container::LCVector3Container()
{}
LCVector3Container::~LCVector3Container()
{}
LCVector3Container* LCVector3Container::makeVector3Container(float x, float y, float z)
{
	LCVector3Container* node = new LCVector3Container();
	if (node) {
		node->setVector((LCVector3){x, y, z});
		node->autorelease();
	} else {
		delete node;
		node = NULL;
	}
	return node;
}

#pragma mark -
#pragma mark Mathematic calculate vector functions
LCVector3Container* LCVector3Container::convertToUnitVector()
{
	// unitVector : length of UnitVector is 1
	if (this->length() != 0) {
		this->scalarMult(1 / this->length());
	}
	return this;
}
float LCVector3Container::length()
{
	// return length of Vector : root(x^2 + y^2 + z^2)
	return sqrt(m_tVector.x * m_tVector.x + m_tVector.y * m_tVector.y + m_tVector.z * m_tVector.z);
}
LCVector3Container* LCVector3Container::scalarMult(float k)
{
	// Vector3 * k
	m_tVector.x *= k;
	m_tVector.y *= k;
	m_tVector.z *= k;
	return this;
}
LCVector3Container* LCVector3Container::rotationWithNormalVector(LCVector3 u, float theta)
{
	LCVector3Container* normalVector = LCVector3Container::makeVector3Container(u.x, u.y, u.z);
	normalVector->convertToUnitVector();
	
	LCVector3 v = m_tVector;
	
	m_tVector.x = (u.x * u.x * (1 - cosf(theta)) + cosf(theta)) * v.x + ((u.x * u.y * (1 - cosf(theta)) - u.z * sinf(theta)) * v.y) + ((u.x * u.z * (1 - cosf(theta)) + u.y * sinf(theta)) * v.z);
	m_tVector.y = ((u.x * u.y * (1 - cosf(theta)) + u.z * sinf(theta)) * v.x) + ((u.y * u.y * (1 - cosf(theta)) + cosf(theta)) * v.y) + ((u.y * u.z * (1 - cosf(theta)) - u.x * sinf(theta)) * v.z);																															  
	m_tVector.z = ((u.x * u.z * (1 - cosf(theta)) - u.y * sinf(theta)) * v.x) + ((u.y * u.z * (1 - cosf(theta)) + u.x * sinf(theta)) * v.y) + ((u.z * u.z * (1 - cosf(theta)) + cosf(theta)) * v.z);
	
	return this;
}

#pragma mark -
#pragma mark Getter and Setter
LCVector3 LCVector3Container::getVector(){return this->m_tVector;}
void LCVector3Container::setVector(LCVector3 tVector){this->m_tVector = tVector;}
