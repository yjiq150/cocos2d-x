/*
 *  LCMenuItem.h
 *  CollinsDictionaryHD
 *
 *  Created by 태호 이 on 11/22/11.
 *  Copyright 2011 Viscuit Creative. All rights reserved.
 *
 */

#ifndef __LC_MENUITEM_H__
#define __LC_MENUITEM_H__

#include "cocos2d.h"

USING_NS_CC;

class LCMenuItem : public CCMenuItem
{
public:
	LCMenuItem():m_sEffectSoundPath(NULL){}
	virtual void selected();
	
	CC_SYNTHESIZE(string, m_sEffectSoundPath, EffectSoundPath);
};


class LCMenuItemToggle : public CCMenuItemToggle {
public:
	LCMenuItemToggle():m_sEffectSoundPath(NULL){}
	virtual void selected();
	
	CC_SYNTHESIZE(string, m_sEffectSoundPath, EffectSoundPath);
};

class LCMenuItemSprite : public CCMenuItemSprite {
public:
	LCMenuItemSprite():m_sEffectSoundPath(""){}
	
	/** creates a menu item with a normal and selected image*/
	static LCMenuItemSprite * itemFromNormalSprite(CCNode* normalSprite, CCNode* selectedSprite);
	/** creates a menu item with a normal and selected image with target/selector */
	static LCMenuItemSprite * itemFromNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, SelectorProtocol* target, SEL_MenuHandler selector);
	/** creates a menu item with a normal,selected  and disabled image with target/selector */
	static LCMenuItemSprite * itemFromNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, SelectorProtocol* target, SEL_MenuHandler selector);
	
	// creates a menu item with a normal and grayed normal image
	static LCMenuItemSprite* itemFromNormalImage(const char *normalImage);
	static LCMenuItemSprite* itemFromNormalImage(const char *normalImage, SelectorProtocol* target, SEL_MenuHandler selector);
	
	static LCMenuItemSprite* itemFromNormalImage(const char *normalImage, const char *selectedImage);
	/** creates a menu item with a normal,selected  and disabled image*/
	static LCMenuItemSprite* itemFromNormalImage(const char *normalImage, const char *selectedImage, const char *disabledImage);
	/** creates a menu item with a normal and selected image with target/selector */
	static LCMenuItemSprite* itemFromNormalImage(const char *normalImage, const char *selectedImage, SelectorProtocol* target, SEL_MenuHandler selector);
	/** creates a menu item with a normal,selected  and disabled image with target/selector */
	static LCMenuItemSprite* itemFromNormalImage(const char *normalImage, const char *selectedImage, const char *disabledImage, SelectorProtocol* target, SEL_MenuHandler selector);
	
	/** initializes a menu item with a normal, selected  and disabled image with target/selector */
	bool initFromNormalImage(const char *normalImage, const char *selectedImage, const char *disabledImage, SelectorProtocol* target, SEL_MenuHandler selector);
	
	virtual void selected();
	
	CC_SYNTHESIZE(string, m_sEffectSoundPath, EffectSoundPath);
};

class LCMenuItemScaleOnTouch : public CCMenuItemScaleOnTouch {
public:
	LCMenuItemScaleOnTouch():m_sEffectSoundPath(NULL){}
	static LCMenuItemScaleOnTouch * itemFromImage(const char* normalImage, SelectorProtocol* target, SEL_MenuHandler selector);
	virtual void selected();
	
	CC_SYNTHESIZE(string, m_sEffectSoundPath, EffectSoundPath);
};

#endif // __LC_MENUITEM_H__
