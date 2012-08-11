/*
 *  LCMenuItem.cpp
 *  CollinsDictionaryHD
 *
 *  Created by 태호 이 on 11/22/11.
 *  Copyright 2011 Viscuit Creative. All rights reserved.
 *
 */

#include "LCMenuItem.h"
#include "SimpleAudioEngine.h"

#pragma mark -
#pragma mark LCMenuItem

void LCMenuItem::selected()
{
	CCMenuItem::selected();
	
	// additional selected event begin
	if (m_sEffectSoundPath.length()) {
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(m_sEffectSoundPath.c_str());
	}
	// additional selected event end
}

#pragma mark -
#pragma mark LCMenuItemToggle

void LCMenuItemToggle::selected()
{
	CCMenuItem::selected();
	
	// additional selected event begin
	if (m_sEffectSoundPath.length()) {
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(m_sEffectSoundPath.c_str());
	}
	// additional selected event end
	
	m_pSubItems->getObjectAtIndex(m_uSelectedIndex)->selected();
}

#pragma mark -
#pragma mark LCMenuItemSprite

LCMenuItemSprite * LCMenuItemSprite::itemFromNormalSprite(CCNode* normalSprite, CCNode* selectedSprite)
{
	return LCMenuItemSprite::itemFromNormalSprite(normalSprite, selectedSprite, NULL, NULL, NULL);
}
LCMenuItemSprite * LCMenuItemSprite::itemFromNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, SelectorProtocol* target, SEL_MenuHandler selector)
{
	return LCMenuItemSprite::itemFromNormalSprite(normalSprite, selectedSprite, NULL, target, selector);
}
LCMenuItemSprite * LCMenuItemSprite::itemFromNormalSprite(CCNode *normalSprite, CCNode *selectedSprite, CCNode *disabledSprite, SelectorProtocol *target, SEL_MenuHandler selector)
{
	LCMenuItemSprite *pRet = new LCMenuItemSprite();
	pRet->initFromNormalSprite(normalSprite, selectedSprite, disabledSprite, target, selector); 
	pRet->autorelease();
	return pRet;
}

LCMenuItemSprite * LCMenuItemSprite::itemFromNormalImage(const char *normalImage)
{
	return LCMenuItemSprite::itemFromNormalImage(normalImage, NULL, NULL, NULL, NULL);
}
LCMenuItemSprite * LCMenuItemSprite::itemFromNormalImage(const char *normalImage, SelectorProtocol* target, SEL_MenuHandler selector)
{
	return LCMenuItemSprite::itemFromNormalImage(normalImage, NULL, NULL, target, selector);
}
LCMenuItemSprite * LCMenuItemSprite::itemFromNormalImage(const char *normalImage, const char *selectedImage)
{
	return LCMenuItemSprite::itemFromNormalImage(normalImage, selectedImage, NULL, NULL, NULL);
}
LCMenuItemSprite * LCMenuItemSprite::itemFromNormalImage(const char *normalImage, const char *selectedImage, SelectorProtocol* target, SEL_MenuHandler selector)
{
	return LCMenuItemSprite::itemFromNormalImage(normalImage, selectedImage, NULL, target, selector);
}
LCMenuItemSprite * LCMenuItemSprite::itemFromNormalImage(const char *normalImage, const char *selectedImage, const char *disabledImage, SelectorProtocol* target, SEL_MenuHandler selector)
{
	LCMenuItemSprite *pRet = new LCMenuItemSprite();
	if (pRet && pRet->initFromNormalImage(normalImage, selectedImage, disabledImage, target, selector))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
LCMenuItemSprite * LCMenuItemSprite::itemFromNormalImage(const char *normalImage, const char *selectedImage, const char *disabledImage)
{
	LCMenuItemSprite *pRet = new LCMenuItemSprite();
	if (pRet && pRet->initFromNormalImage(normalImage, selectedImage, disabledImage, NULL, NULL))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}
bool LCMenuItemSprite::initFromNormalImage(const char *normalImage, const char *selectedImage, const char *disabledImage, SelectorProtocol* target, SEL_MenuHandler selector)
{
	CCNode *normalSprite = CCSprite::spriteWithFile(normalImage);
	CCNode *selectedSprite = NULL;
	CCNode *disabledSprite = NULL;
	
	if (selectedImage)
	{
		selectedSprite = CCSprite::spriteWithFile(selectedImage);
	} else {
		selectedSprite = CCSprite::spriteWithFile(normalImage);
		((CCSprite*)selectedSprite)->setColor(ccGRAY);
	}

	
	if(disabledImage)
	{
		disabledSprite = CCSprite::spriteWithFile(disabledImage);
	}
	return initFromNormalSprite(normalSprite, selectedSprite, disabledSprite, target, selector);
}

void LCMenuItemSprite::selected()
{
	CCMenuItem::selected();
	
	// additional selected event begin
	if (m_sEffectSoundPath.length()) {
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(m_sEffectSoundPath.c_str());
	}
	// additional selected event end
	
	if (m_pDisabledImage)
	{
		m_pDisabledImage->setIsVisible(false);
	}
	
	if (m_pSelectedImage)
	{
		m_pNormalImage->setIsVisible(false);
		m_pSelectedImage->setIsVisible(true);
	}
	else
	{
		m_pNormalImage->setIsVisible(true);
	}
}

#pragma mark -
#pragma mark LCMenuItemScaleOnTouch

LCMenuItemScaleOnTouch * LCMenuItemScaleOnTouch::itemFromImage(const char *normalImage, SelectorProtocol* target, SEL_MenuHandler selector)
{
	LCMenuItemScaleOnTouch *pRet = new LCMenuItemScaleOnTouch();
	if (pRet && pRet->initFromImage(normalImage,target, selector))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

void LCMenuItemScaleOnTouch::selected()
{
	CCMenuItem::selected();
	
	// additional selected event begin
	if (m_sEffectSoundPath.length()) {
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(m_sEffectSoundPath.c_str());
	}
	// additional selected event end
	
	
	m_pNormalImage->setScale(scaleAtSelect);
	
	
	m_pNormalImage->setPosition(ccp(m_pNormalImage->getPosition().x-m_pNormalImage->getContentSize().width/2 * (scaleAtSelect - 1),
									m_pNormalImage->getPosition().y-m_pNormalImage->getContentSize().height/2 * (scaleAtSelect - 1) ));
}