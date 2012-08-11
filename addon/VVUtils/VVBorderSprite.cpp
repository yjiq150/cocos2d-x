/*
 *  VVBorderSprite.cpp
 *  CollinsPictureCardHD
 *
 *  Created by YoungJae Kwon on 11. 11. 1..
 *  Copyright 2011 Beyondi Soft All rights reserved.
 *
 */

#include "VVBorderSprite.h"

unsigned long ccNextPOT(unsigned long x)
{
    x = x - 1;
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >>16);
    return x + 1;
}

CCTexture2D* VVBorderSprite::createBorderTextureWithSprite(CCSprite* spr, float borderSize)
{
	
	// 스티커 이미지를 조작해서 alpha이미지를 만들어 내고 이를 통해 테두리를 만든다.
	CCSize sprSize = spr->getContentSizeInPixels();
	float scaleX = (sprSize.width + borderSize ) / sprSize.width;
	float scaleY = (sprSize.height + borderSize ) / sprSize.height;
	
	int alphaTexWidth = sprSize.width * scaleX;
	int alphaTexHeight = sprSize.height * scaleY;

	
	CCPoint originalPos = spr->getPosition();
	CCPoint originalAnchor = spr->getAnchorPoint();
	// 형태정보(알파값) 추출을 위해 renderTexture를 이용해서 메모리에 텍스쳐를 그림
	CCRenderTexture *renderTex = CCRenderTexture::renderTextureWithWidthAndHeight(alphaTexWidth, alphaTexHeight);
	spr->setAnchorPoint(ccp(0,0));
	spr->setPosition(ccp(0,0));
	spr->setScaleX(scaleX);
	spr->setScaleY(scaleY);
	renderTex->begin();
	spr->visit();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    renderTex->end();
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	renderTex->end(false);    
#endif

	
	// 원래 셋팅으로 돌려놓아야 스티커가 정상적으로 보임
	spr->setAnchorPoint(originalAnchor);
	spr->setPosition(originalPos);
	spr->setScaleX(1);
	spr->setScaleY(1);
	
	// rendertexture에서 raw data를 뽑아서 직접 조작한다.
	CCImage *alphaImg = new CCImage();
	renderTex->getUIImageFromBuffer(alphaImg, 0, 0, alphaTexWidth, alphaTexHeight);
	unsigned char* data = alphaImg->getData();
	
	for (int i = 0; i <  alphaTexWidth*alphaTexHeight; i++) 
	{
		// 알파값만 유의미하게 남겨두고 나머지는 모두 흰색으로 set
        // 시작 포인터로부터 3바이트를 모두 255로 셋팅.
		memset(data+i*4,255,3);
	}
	
	CCTexture2D* alphaTexture = new CCTexture2D();
	alphaTexture->initWithImage(alphaImg);
	alphaTexture->autorelease();
	
#if CC_ENABLE_CACHE_TEXTTURE_DATA

    
    int POTWide = ccNextPOT(alphaTexWidth);
    int POTHigh = ccNextPOT(alphaTexHeight);

    // this 'dataToKeep' will be released by VolatileTexture::removeTexture() in CCTexture2D.
    unsigned char* dataToKeep = new unsigned char[POTHigh * POTWide * 4];
    
    if(alphaTexWidth == POTWide && alphaTexHeight == POTHigh)
    {
        memcpy(dataToKeep, data , POTHigh * POTWide * 4);
    }
    else
    {
        memset(dataToKeep, 0, POTHigh * POTWide * 4);
        
        unsigned char* pPixelData = (unsigned char*) data;
        unsigned char* pTargetData = (unsigned char*) dataToKeep;
        
        for(int y = 0; y < alphaTexHeight; ++y)
        {
            memcpy(pTargetData+POTWide*4*y, pPixelData+(alphaTexWidth)*4*y, (alphaTexWidth)*4);
        }
    }
    
    VolatileTexture::addDataTexture(alphaTexture, dataToKeep, kTexture2DPixelFormat_RGBA8888, alphaTexture->getContentSizeInPixels() );    
#endif
	
	CC_SAFE_DELETE(alphaImg);
	
	return alphaTexture;
}