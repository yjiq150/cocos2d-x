

#ifndef CcMenuItemDynamic_h__
#define CcMenuItemDynamic_h__
#include "cocos2d.h"
//  Copyright Eli Yukelzon, base on code by Jose Antonio Andújar Clavell 
//
//  License http://creativecommons.org/licenses/by/3.0/


class CCMenuItemDynamic : public CCMenuItemSprite 
{
public:
	static CCMenuItemSprite * dynamicItemWithTTFLabel(const char* text,  const char* font, int size, const CCSize &minSize,
                                                      const char*normalImage, const char *selectedImage, 
                                                      CCObject* target, SEL_MenuHandler selector, 
                                                      const CCRect& centerRegion=CCRect(20,20,24,24), GLubyte opacity=255);

    static CCMenuItemSprite * dynamicItemWithBMFontLabel(const char* text,  const char* font, const CCSize &minSize, 
                                                         const char*normalImage, const char *selectedImage, 
                                                         CCObject* target, SEL_MenuHandler selector, 
                                                         const CCRect& centerRegion, GLubyte opacity);
    
};
    
#endif // CcMenuItemDynamic_h__
