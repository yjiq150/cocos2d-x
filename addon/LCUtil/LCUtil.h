/*
 *  LCUtil.h
 *  HelloWorld
 *
 *  Created by 태호 이 on 11. 6. 15..
 *  Copyright 2011 Viscuit Creative. All rights reserved.
 *
 */

#ifndef __LC_UTIL_H__
#define __LC_UTIL_H__

#include "cocos2d.h"

USING_NS_CC;

enum LCUTIL_ACTION_LIST{
	LCUTIL_ACTION_SCALE_DOWN_UP,
	LCUTIL_ACTION_SCALE_UP_DOWN,
	LCUTIL_ACTION_SCALE_AND_SHAKE
};

class LCUtil
{
protected:
public:
	static CCAction* actionForButtonTouched(int index);
	
	// convert CCTouch to CCTouches with CCSet
	static CCSet* convertTouchToTouches(CCTouch* pTouch);
	
	// calculate with position, anchorpoint, contentSize, scale to know touched in the node
	static bool isTouchInside(CCNode* node, CCTouch* touch);
	static bool isTouchInside(CCNode* node, CCSet* touches);
	static bool isTouchInside(CCNode* node, CCTouch* touch, bool isVisibleSensitive);
	static bool isTouchInside(CCNode* node, CCSet* touches, bool isVisibleSensitive);
	
	// actions for SwitchButton
	static void actionOnTouchBegan(CCSprite* normalSprite, CCSprite* touchedSprite, CCSet* touches);
	static void actionOnTouchMoved(CCSprite* normalSprite, CCSprite* touchedSprite, CCSet* touches);
	static void actionOnTouchEnded(CCSprite* normalSprite, CCSprite* touchedSprite, CCSet* touches);
	static void actionOnTouchBegan(CCSprite* normalSprite, CCSprite* touchedSprite, CCTouch* touch);
	static void actionOnTouchMoved(CCSprite* normalSprite, CCSprite* touchedSprite, CCTouch* touch);
	static void actionOnTouchEnded(CCSprite* normalSprite, CCSprite* touchedSprite, CCTouch* touch);
	
	// actions for SingleButton
	static void actionOnTouchBegan(CCSprite* sprite, CCSet* touches);
	static void actionOnTouchMoved(CCSet* touches);
	static void actionOnTouchEnded(CCSet* touches);
	static void actionOnTouchBegan(CCSprite* sprite, CCTouch* touch);
	static void actionOnTouchMoved(CCTouch* touch);
	static void actionOnTouchEnded(CCTouch* touch);
	
	// actions for Button and Effect Sound
	static void actionOnTouchBegan(CCSprite* sprite, CCSet* touches, const char* effectPath);
	static void actionOnTouchBegan(CCSprite* sprite, CCTouch* touch, const char* effectPath);
	static void actionOnTouchBegan(CCSprite* normalSprite, CCSprite* touchedSprite, CCSet* touches, const char* effectPath);
	static void actionOnTouchBegan(CCSprite* normalSprite, CCSprite* touchedSprite, CCTouch* touch, const char* effectPath);
	static void actionOnTouchMoved(CCSprite* normalSprite, CCSprite* touchedSprite, CCSet* touches, const char* effectPath);
	static void actionOnTouchMoved(CCSprite* normalSprite, CCSprite* touchedSprite, CCTouch* touch, const char* effectPath);
	
	// for file management
	static void copyFileFromBundle(const char* srcFilename, const char* dstFilename);
	static void copyFileFromWritablePath(const char* srcFilename, const char* dstFilename);
	// added by YoungJaeKwon
	static bool writeFileAtWritablePath(const unsigned char* srcData,unsigned long srcLength, const char* dstFilename);
	static bool isFileExistAtBundle(const char* filename);
	static bool isFileExistAtWritablePath(const char* filename);
	
	// make stretchable sprite
	static CCSprite* stretchableSpriteWithFile(const char* filename, int width, int height, int leftCap, int bottomCap);
	static CCSprite* verticalStretchableSpriteWithFile(const char* filename, int height, int bottomCap);
	
	// shuffle array randomly
	static void shuffleArray(CCArray* array);
protected:
};

#endif // __LC_UTIL_H__