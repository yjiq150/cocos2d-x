/*
 *  LCUtil.cpp
 *  HelloWorld
 *
 *  Created by 태호 이 on 11. 6. 17..
 *  Copyright 2011 Viscuit Creative. All rights reserved.
 *
 */

#include "LCUtil.h"
#include "LCButtonTouchUtil.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

CCAction* LCUtil::actionForButtonTouched(int index)
{
	CCAction* returnAction;
	switch (index) {
		case LCUTIL_ACTION_SCALE_DOWN_UP: returnAction = CCSequence::actions(CCScaleTo::actionWithDuration(0.1f, 0.8f), CCScaleTo::actionWithDuration(0.1f, 1.0f), NULL); break;
		case LCUTIL_ACTION_SCALE_UP_DOWN: returnAction = CCSequence::actions(CCScaleTo::actionWithDuration(0.1f, 1.2f), CCScaleTo::actionWithDuration(0.1f, 1.0f), NULL); break;
		case LCUTIL_ACTION_SCALE_AND_SHAKE: 
			returnAction = CCSequence::actions(CCScaleTo::actionWithDuration(0.1f, 1.2f),
											   CCRotateTo::actionWithDuration(0.3f, -30), 
											   CCRotateTo::actionWithDuration(0.6f, 30), 
											   CCRotateTo::actionWithDuration(0.3f, 0), 
											   CCScaleTo::actionWithDuration(0.1f, 1.0f), NULL); 
			break;
		default:
			break;
	}
	
	return returnAction;
}

#pragma mark -
#pragma mark Converting

CCSet* LCUtil::convertTouchToTouches(CCTouch* pTouch)
{
	CCSet* pSet = new CCSet();
	if (pSet) {
		pSet->addObject(pTouch);
		pSet->autorelease();
	}
	return pSet;
}

#pragma mark -
#pragma mark Determine is touched inside of node

bool LCUtil::isTouchInside(CCNode* node, CCTouch* touch)
{	
	return isTouchInside(node, touch, true);
}

bool LCUtil::isTouchInside(CCNode* node, CCSet* touches)
{
	return isTouchInside(node, touches, true);
}

bool LCUtil::isTouchInside(CCNode* node, CCTouch* touch, bool isVisibleSensitive)
{
	if (!node) {
		return false;
	}
	if (!isVisibleSensitive || node->getIsVisible()) {
		if (isVisibleSensitive) {
			CCNode *pParent = node->getParent();
			while (pParent) {
				if (!pParent->getIsVisible()) {
					return false;
				}
				pParent = pParent->getParent();
			}
		}
		CCPoint touchPointToGL = CCDirector::sharedDirector()->convertToGL(touch->locationInView(touch->view()));
//        return IS_TOUCH_INSIDE(node, touchPointToGL);
		CCSize convertedSize = CCSizeMake(node->getContentSize().width * node->getScaleX(), node->getContentSize().height * node->getScaleY());

		
        CCPoint convertedPoint = ccp(node->getPosition().x - convertedSize.width * node->getAnchorPoint().x, node->getPosition().y - convertedSize.height * node->getAnchorPoint().y);
		
		// middle point
		convertedPoint = ccpAdd(convertedPoint, ccp(convertedSize.width * 0.5f, convertedSize.height * 0.5f));
		
		CCNode *pParent = node->getParent();
		float totalAngle = node->getRotation();
		while (pParent) {
			CCSize parentConvertedSize = CCSizeMake(pParent->getContentSize().width * pParent->getScaleX(), pParent->getContentSize().height * pParent->getScaleY());
			CCPoint parentConvertedPoint = ccp(pParent->getPosition().x - parentConvertedSize.width * pParent->getAnchorPoint().x, pParent->getPosition().y - parentConvertedSize.height * pParent->getAnchorPoint().y);
			
			
			float rotation = pParent->getRotation();
			totalAngle += rotation;
//			CCLog("rotation %f", rotation);
//			CCLog("anchor %f %f", pParent->getAnchorPoint().x, pParent->getAnchorPoint().y);
//			CCLog("prev %f %f", convertedPoint.x, convertedPoint.y);			
			convertedPoint = ccp(convertedPoint.x - parentConvertedSize.width * pParent->getAnchorPoint().x, convertedPoint.y - parentConvertedSize.height * pParent->getAnchorPoint().y);
			convertedPoint = ccp(convertedPoint.x * cosf(rotation/180.0f*M_PI) + convertedPoint.y * sinf(rotation/180.0f*M_PI), convertedPoint.x * -sinf(rotation/180.0f*M_PI) + convertedPoint.y * cosf(rotation/180.0f*M_PI));
			convertedPoint = ccp(convertedPoint.x + parentConvertedSize.width * pParent->getAnchorPoint().x, convertedPoint.y + parentConvertedSize.height * pParent->getAnchorPoint().y);
//			CCLog("after %f %f", convertedPoint.x, convertedPoint.y);
			
			convertedPoint = ccpAdd(convertedPoint, parentConvertedPoint);
			pParent = pParent->getParent();
		}
		
		if (touchPointToGL.x < convertedPoint.x - convertedSize.width / 2 || touchPointToGL.y < convertedPoint.y - convertedSize.height / 2 || 
			touchPointToGL.x > convertedPoint.x + convertedSize.width / 2 || touchPointToGL.y > convertedPoint.y + convertedSize.height / 2) {
			return false;
		} else {
			return true;
		}
        
		
/*
		if (touchPointToGL.x < convertedPoint.x || touchPointToGL.y < convertedPoint.y || 
			touchPointToGL.x > convertedPoint.x + convertedSize.width || touchPointToGL.y > convertedPoint.y + convertedSize.height) {
			return false;
		} else {
			return true;
		}
 */
	}
	return false;
}
bool LCUtil::isTouchInside(CCNode* node, CCSet* touches, bool isVisibleSensitive)
{
	if (!touches || touches->count() == 0)
    {
        return false;
    }
    
    CCSetIterator it;
	
    for( it = touches->begin(); it != touches->end(); it++)
    {
        if (*it)
        {
			bool bIsTouchInside = isTouchInside(node, (CCTouch *)*it, isVisibleSensitive);
			if (bIsTouchInside) {
				return bIsTouchInside;
			}
        }
    }
	
    return false;
}

#pragma mark -
#pragma mark actions for SwitchButton
void LCUtil::actionOnTouchBegan(CCSprite* normalSprite, CCSprite* touchedSprite, CCSet* touches)
{
	if (LCUtil::isTouchInside(normalSprite, touches)) {
		normalSprite->setIsVisible(false);
		touchedSprite->setIsVisible(true);
//		touchedSprite->stopAllActions();
//		touchedSprite->setScale(1.0f);
//		touchedSprite->runAction(CCScaleTo::actionWithDuration(0.1f, 1.2f));
	}
}
void LCUtil::actionOnTouchMoved(CCSprite* normalSprite, CCSprite* touchedSprite, CCSet* touches)
{	
	if (touchedSprite->getIsVisible()) {
		if (!LCUtil::isTouchInside(touchedSprite, touches)) {
			normalSprite->setIsVisible(true);
			touchedSprite->setIsVisible(false);
//			normalSprite->stopAllActions();
//			normalSprite->setScale(1.2f);
//			normalSprite->runAction(CCScaleTo::actionWithDuration(0.1f, 1.0f));
		}
	} else {
		if (LCUtil::isTouchInside(normalSprite, touches)) {
			normalSprite->setIsVisible(false);
			touchedSprite->setIsVisible(true);
//			touchedSprite->stopAllActions();
//			touchedSprite->setScale(1.0f);
//			touchedSprite->runAction(CCScaleTo::actionWithDuration(0.1f, 1.2f));
		}
	}
	
}
void LCUtil::actionOnTouchEnded(CCSprite* normalSprite, CCSprite* touchedSprite, CCSet* touches)
{
	if (touchedSprite->getIsVisible()) {
		normalSprite->setIsVisible(true);
		touchedSprite->setIsVisible(false);
//		normalSprite->stopAllActions();
//		normalSprite->setScale(1.2f);
//		normalSprite->runAction(CCScaleTo::actionWithDuration(0.1f, 1.0f));
	}
}

void LCUtil::actionOnTouchBegan(CCSprite* normalSprite, CCSprite* touchedSprite, CCTouch* touch)
{
	CCSet *pTouches = new CCSet();
	pTouches->addObject(touch);
	LCUtil::actionOnTouchBegan(normalSprite, touchedSprite, pTouches);
	pTouches->release();
}
void LCUtil::actionOnTouchMoved(CCSprite* normalSprite, CCSprite* touchedSprite, CCTouch* touch)
{	
	CCSet *pTouches = new CCSet();
	pTouches->addObject(touch);
	LCUtil::actionOnTouchMoved(normalSprite, touchedSprite, pTouches);
	pTouches->release();
}
void LCUtil::actionOnTouchEnded(CCSprite* normalSprite, CCSprite* touchedSprite, CCTouch* touch)
{
	CCSet *pTouches = new CCSet();
	pTouches->addObject(touch);
	LCUtil::actionOnTouchEnded(normalSprite, touchedSprite, pTouches);
	pTouches->release();
}

#pragma mark -
#pragma mark actions for SingleButton

void LCUtil::actionOnTouchBegan(CCSprite* sprite, CCSet* touches)
{
	LCUtil::actionOnTouchBegan(sprite, touches, "");
}
void LCUtil::actionOnTouchMoved(CCSet* touches)
{
	LCButtonTouchUtil::sharedButtonTouchUtil()->actionOnTouchMoved(touches);
}
void LCUtil::actionOnTouchEnded(CCSet* touches)
{
	LCButtonTouchUtil::sharedButtonTouchUtil()->actionOnTouchEnded(touches);
}
void LCUtil::actionOnTouchBegan(CCSprite* sprite, CCTouch* touch)
{
	CCSet *pTouches = new CCSet();
	pTouches->addObject(touch);
	LCUtil::actionOnTouchBegan(sprite, pTouches);
	pTouches->release();
}
void LCUtil::actionOnTouchMoved(CCTouch* touch)
{
	CCSet *pTouches = new CCSet();
	pTouches->addObject(touch);
	LCUtil::actionOnTouchMoved(pTouches);
	pTouches->release();
}
void LCUtil::actionOnTouchEnded(CCTouch* touch)
{
	CCSet *pTouches = new CCSet();
	pTouches->addObject(touch);
	LCUtil::actionOnTouchEnded(pTouches);
	pTouches->release();
}

#pragma mark -
#pragma mark actions for Button and Effect Sound
void LCUtil::actionOnTouchBegan(CCSprite* sprite, CCSet* touches, const char* effectPath)
{
	LCButtonTouchUtil::sharedButtonTouchUtil()->actionOnTouchBegan(sprite, touches, effectPath);
}
void LCUtil::actionOnTouchBegan(CCSprite* sprite, CCTouch* touch, const char* effectPath)
{
	CCSet *pTouches = new CCSet();
	pTouches->addObject(touch);
	LCUtil::actionOnTouchBegan(sprite, pTouches, effectPath);
	pTouches->release();
}
void LCUtil::actionOnTouchBegan(CCSprite* normalSprite, CCSprite* touchedSprite, CCSet* touches, const char* effectPath)
{
	LCUtil::actionOnTouchBegan(normalSprite, touchedSprite, touches);
	
	if (!normalSprite->getIsVisible()) {
		// play effect sound
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(effectPath);
	}
}
void LCUtil::actionOnTouchBegan(CCSprite* normalSprite, CCSprite* touchedSprite, CCTouch* touch, const char* effectPath)
{
	CCSet *pTouches = new CCSet();
	pTouches->addObject(touch);
	LCUtil::actionOnTouchBegan(normalSprite, touchedSprite, pTouches, effectPath);
	pTouches->release();
}
void LCUtil::actionOnTouchMoved(CCSprite* normalSprite, CCSprite* touchedSprite, CCSet* touches, const char* effectPath)
{
	bool isVisible = touchedSprite->getIsVisible();
	LCUtil::actionOnTouchMoved(normalSprite, touchedSprite, touches);
	if (!isVisible && isVisible != touchedSprite->getIsVisible()) {
		// play effect sound
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(effectPath);
	}
}
void LCUtil::actionOnTouchMoved(CCSprite* normalSprite, CCSprite* touchedSprite, CCTouch* touch, const char* effectPath)
{
	CCSet *pTouches = new CCSet();
	pTouches->addObject(touch);
	LCUtil::actionOnTouchMoved(normalSprite, touchedSprite, pTouches, effectPath);
	pTouches->release();
}

#pragma mark -
#pragma mark file management
void LCUtil::copyFileFromBundle(const char* srcFilename, const char* dstFilename)
{
	int c;
	FILE *SRC, *DST;
	string srcPath = CCFileUtils::fullPathFromRelativePath(srcFilename);
    string dstPath = CCFileUtils::getWriteablePath() + dstFilename;
    
	SRC = fopen(srcPath.c_str(), "rb");
	if (SRC == NULL) {
		CCLog("LCUtil::copyFileFromBundle : Source File Error - %s", srcPath.c_str());
        CCLog("- %s", dstPath.c_str());
		return;
	}
	
	// 코코스 버전업 되면서 버그수정됨. / 붙일필요 없음
//	string dstPath = CCFileUtils::getWriteablePath() + "/" + dstFilename;
	DST = fopen(dstPath.c_str(), "wb");
	if (DST == NULL) {
		CCLog("LCUtil::copyFileFromBundle : Destination File Error - %s", dstPath.c_str());
		return;
	}
	
	while ((c=fgetc(SRC)) != EOF) {
		if ((fputc(c, DST)) == EOF) {
			break;
		}
	}
	
	fclose(DST);
	fclose(SRC);
}
void LCUtil::copyFileFromWritablePath(const char* srcFilename, const char* dstFilename)
{
	int c;
	FILE *SRC, *DST;
	string srcPath = CCFileUtils::getWriteablePath() + "/" + srcFilename;
	SRC = fopen(srcPath.c_str(), "rb");
	if (SRC == NULL) {
		CCLog("LCUtil::copyFileFromWritablePath : Source File Error - %s", srcPath.c_str());
		return;
	}
	
	string dstPath = CCFileUtils::getWriteablePath() + "/" + dstFilename;
	DST = fopen(dstPath.c_str(), "wb");
	if (DST == NULL) {
		CCLog("LCUtil::copyFileFromWritablePath : Destination File Error - %s", dstPath.c_str());
		return;
	}
	
	while ((c=fgetc(SRC)) != EOF) {
		if ((fputc(c, DST)) == EOF) {
			break;
		}
	}
	
	fclose(DST);
	fclose(SRC);
}

// added by YoungJae Kwon
// 소스데이터 크기만큼 잘 써졌을경우 true 리턴
bool LCUtil::writeFileAtWritablePath(const unsigned char* srcData, unsigned long srcLength, const char* dstFilename)
{
	unsigned long writtenLength = 0;
	FILE *DST;
		
	string dstPath = CCFileUtils::getWriteablePath() + dstFilename;
	DST = fopen(dstPath.c_str(), "wb");
	if (DST == NULL) 
	{
		CCLog("Dest: %s",dstPath.c_str());
		CCLog("LCUtil::writeFileAtWritablePath : Destination File Error");
		return false;
	}
	writtenLength = fwrite(srcData, 1, srcLength, DST);
	fclose(DST);
	
	if( writtenLength != srcLength )
		return false;
	else
		return true;
}

bool LCUtil::isFileExistAtBundle(const char* filename)
{
	FILE *filePt;
	string path = CCFileUtils::fullPathFromRelativePath(filename);
	filePt = fopen(path.c_str(), "r");
	if (filePt == NULL) {
		return false;
	} else {
		fclose(filePt);
		return true;
	}
}

//deprecated : use CCFileUtils::isFileExistAtWritablePath()
bool LCUtil::isFileExistAtWritablePath(const char* filename)
{
	FILE *filePt;
	string path = CCFileUtils::getWriteablePath() + filename;

	filePt = fopen(path.c_str(), "r");
	if (filePt == NULL) {
		return false;
	} else {
		fclose(filePt);
		return true;
	}
}

#pragma mark -
#pragma mark make stretchable sprite
CCSprite* LCUtil::stretchableSpriteWithFile(const char* filename, int width, int height, int leftCap, int bottomCap)
{	
	GLubyte * pBuffer   = NULL;
    GLubyte * pPixels   = NULL;
	
	leftCap *= CC_CONTENT_SCALE_FACTOR();
	bottomCap *= CC_CONTENT_SCALE_FACTOR();
	
	CCSprite *originalSprite = CCSprite::spriteWithFile(filename);
	originalSprite->setAnchorPoint(ccp(0,0));
	int numberOfComponent = 4; //RGBA
	
	int tx = originalSprite->getContentSize().width * CC_CONTENT_SCALE_FACTOR();
	int ty = originalSprite->getContentSize().height * CC_CONTENT_SCALE_FACTOR();
	int tWidth = width * CC_CONTENT_SCALE_FACTOR();
	int tHeight = height * CC_CONTENT_SCALE_FACTOR();
	
	int bitsPerComponent = 8;
	int bitsPerPixel = bitsPerComponent * numberOfComponent;
	int bytesPerPixel = bitsPerPixel / 8;
	
	int bytesPerRow = bytesPerPixel * tx;
//	int myDataLength = bytesPerRow * ty;
	
	if(! (pBuffer = new GLubyte[tx * ty * bytesPerPixel])) return NULL;
	if(! (pPixels = new GLubyte[tWidth * tHeight * bytesPerPixel])) return NULL;
	
	// load pixel informations from image
	CCRenderTexture* canvas = CCRenderTexture::renderTextureWithWidthAndHeight(tx, ty);
	canvas->begin();
	originalSprite->visit();
	glReadPixels(0,0,tx,ty,GL_RGBA,GL_UNSIGNED_BYTE, pBuffer);
	//glDrawPixels(tx, ty, GL_RGB, GL_UNSIGNED_BYTE, pBuffer);
	canvas->end();
	
	cout << "width height" << tx << " " << ty << endl;
	
	// modify pixel informations to stretch image
	int x,y, c;
	int sourceX, sourceY;
	
	for(y = 0; y <tHeight; y++) {
		if (y < bottomCap) {
			sourceY = y;
		} else if (y >= bottomCap && y < bottomCap + tHeight - ty) {
			sourceY = bottomCap - 1;
		} else {
			sourceY = y - (tHeight - ty);
		}
		
		for(x = 0; x <tWidth; x++) {
			if (x < leftCap) {
				sourceX = x;
			} else if (x >= leftCap && x < leftCap + tWidth - tx) {
				sourceX = leftCap - 1;
			} else {
				sourceX = x - (tWidth - tx);
			}
			
			//sourceX = 20;
			//sourceY = 20;
			
			//std::cout << x << " " << sourceX << "  " << y << " " << sourceY << std::endl;
						
			for (c = 0; c < bytesPerPixel; c++) {
				//if (c == 3) {
//					pPixels[(tHeight - 1 - y) * bytesPerRow + x * bytesPerPixel + c] = '\377'; //pBuffer[sourceY * bytesPerRow + sourceX * bytesPerPixel + c];
//				} else {
//					pPixels[(tHeight - 1 - y) * bytesPerRow + x * bytesPerPixel + c] = '\377'; //pBuffer[sourceY * bytesPerRow + sourceX * bytesPerPixel + c];
//				}
				pPixels[(tHeight - 1 - y) * bytesPerRow + x * bytesPerPixel + c] = pBuffer[sourceY * bytesPerRow + sourceX * bytesPerPixel + c];
			}
			
			//cout << x << " " << (tHeight - 1 - y) << "   " << sourceX << " " << sourceY << endl;
		}
	}
	
	// make sprite with pixel informations
	CCTexture2D* texture = new CCTexture2D();
	texture->initWithData(pPixels, kCCTexture2DPixelFormat_RGBA8888, tWidth, tHeight, CCSizeMake(tWidth, tHeight));
	
	CCSprite* resultSprite = CCSprite::spriteWithTexture(texture);
		
	texture->release();
	
	CC_SAFE_DELETE_ARRAY(pBuffer);
	CC_SAFE_DELETE_ARRAY(pPixels);
	
	return resultSprite;
}

CCSprite* LCUtil::verticalStretchableSpriteWithFile(const char* filename, int height, int bottomCap)
{
//	CCSprite* LCUtil::stretchableSpriteWithFile(const char* filename, int width, int height, int leftCap, int bottomCap);
	CCSprite* originalImage = CCSprite::spriteWithFile(filename);
	return LCUtil::stretchableSpriteWithFile(filename, (int)originalImage->getContentSize().width, height, (int)originalImage->getContentSize().width, bottomCap);
}

#pragma mark -
#pragma mark shuffle array randomly
void LCUtil::shuffleArray(CCArray* array)
{
	srand(time(NULL));
	CCArray *tempArray = CCArray::arrayWithArray(array);
	int count = array->count();
	array->removeAllObjects();
	
	for (int i=0; i<count; i++) {
		CCObject* object = (CCObject*)tempArray->randomObject();
		array->addObject(object);
		tempArray->removeObject(object);
	}
}