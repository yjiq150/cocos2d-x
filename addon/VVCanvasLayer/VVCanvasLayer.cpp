//
//  VVCanvasLayer.cpp
//  CocosX_test
//
//  Created by YoungJae Kwon on 11. 5. 21..
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

#include "VVCanvasLayer.h"
#include "VVStringHelper.h"
#include "support/ccUtils.h"

#define BASE_BRUSH_SCALE 2

USING_NS_CC;
VVCanvasLayer::VVCanvasLayer()
{
	m_pData = NULL;
    baseScale = BASE_BRUSH_SCALE;
    canvasColor = ccc4(255, 255, 255, 255);
}

bool VVCanvasLayer::init()
{
	if( !CCLayerColor::initWithColor(ccc4(0, 0, 0, 0)) )
		return false;

	brush = NULL;
    onGoingAction = NULL;
    
	return true;
}

bool VVCanvasLayer::initWithBox(CCSize s, char *guideName)
{	
	if( !this->initWithBox(s, guideName, NULL) )
	{
		return false;
	}
	
	return true;
}

bool VVCanvasLayer::initWithBox(CCSize s, const char* guideName, CCSprite *backgroundFrame)
{
	if( !this->init() )
	{
		return false;
	}

	canvasSize = s;

	// 가로세로 회전가능한 모드: 회전해도 칼러레이어가 꽉차도록 가로세로 큰값만큼 흰바탕 칠하기
	/*
	float max;
	if (s.width > s.height) 
		max = s.width;
	else
		max = s.height;
	
	// 가로,세로 모드 지원을 위해 텍스쳐는 정사각형으로 넉넉히 생성
	textureSize = CCSizeMake(max, max);
	this->setContentSize(textureSize);
	this->setIsTouchEnabled(true);
	// create a render texture, this is what we're going to draw into
	canvas = CCRenderTexture::renderTextureWithWidthAndHeight(max, max);
	canvas->setPosition(ccp(s.width/2,s.height/2));
	*/
	
	// 그냥 한방향 모드
	textureSize = canvasSize;
	this->setContentSize(s);
	this->setIsTouchEnabled(true);
	// create a render texture, this is what we're going to draw into
	canvas = CCRenderTexture::renderTextureWithWidthAndHeight(s.width, s.height);
	canvas->setPosition(ccp(s.width/2,s.height/2));
	

	string key = string("key_canvas_") + guideName;
	VVStringHelper::stdReplaceAll(key,"/","_");	
	canvas->setRenderTextureName(key);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	CCTextureCache::sharedTextureCache()->registerRenderTexture(canvas);
#endif
	
	
	clearCanvas();
	
	
	// It's possible to modify the RenderTexture blending function by
	//		[[canvas sprite] setBlendFunc:(ccBlendFunc) {GL_ONE, GL_ONE_MINUS_SRC_ALPHA}];
	
	// note that the render texture is a CCNode, and contains a sprite of its texture for convience,
	// so we can just parent it to the scene like any other CCNode
	this->addChild(canvas, kTag_CanvasCanvas);
	
	// create a brush image to draw into the texture with
	brush = CCSprite::spriteWithFile("brush/fire.png");
	brush->retain();
	brush->setOpacity(30);

	ccBlendFunc bf = { GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
    brush->setBlendFunc( bf);
	
	brush->setScale(baseScale);// 아이패드 버전은 브러쉬 크기를 1.2배로
	
	// 가이드 이미지 로딩
	sprGuide = CCSprite::spriteWithFile(guideName);
	sprGuide->setPosition(ccp(s.width/2, s.height/2));
	this->addChild(sprGuide, kTag_CanvasGuide);
	
    
	// 백그라운드 이미지 로딩
	if (backgroundFrame) {
		backgroundFrame->setPosition(ccp(s.width/2, s.height/2));
		this->addChild(backgroundFrame, kTag_CanvasFrame);
	}
	
	return true;
}


// deprecated: use screenshot rendertexture
void VVCanvasLayer::executeFadeOut(ccTime dur)
{

    if( dur >= 0.99)
    {
        canvas->getSprite()->setIsVisible(false);
        sprGuide->setIsVisible(false);
    }
    
    /*
    CCFiniteTimeAction* fadeOut1 = CCFadeOut::actionWithDuration(dur);
    CCFiniteTimeAction* fadeOut2 = CCFadeOut::actionWithDuration(dur);
    
    sprGuide->runAction(fadeOut1);
//    ccBlendFunc blendFunc = {GL_ONE, GL_ONE_MINUS_SRC_ALPHA };
    ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
    CCSprite* renderSpr = canvas->getSprite();
    renderSpr->setBlendFunc(blendFunc);
    renderSpr->runAction(fadeOut2);
     */
    

}

void VVCanvasLayer::clearCanvas()
{
	// 캔버스 canvasColor로 클리어
	canvas->begin();
    
	GLfloat	clearColor[4];
	glGetFloatv(GL_COLOR_CLEAR_VALUE,clearColor);
    
	glClearColor(canvasColor.r/255.0f, canvasColor.g/255.0f, canvasColor.b/255.0f, canvasColor.a/255.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	// restore clear color
	glClearColor(clearColor[0], clearColor[1], clearColor[2], clearColor[3]);    

	canvas->end(false);
}

void VVCanvasLayer::clearCanvasAsAction()
{
    clearCanvas();
    
    ClearAction* action = new ClearAction(this);
    saveHistory(action);    
}

bool VVCanvasLayer::saveToFile(const char* sdcardFullPath)
{
	// 텍스쳐 크기가 화면크기보다 크기때문에 찍는 점을 평행이동을 해줘야 제대로 포인팅 됨
	float translation_x = (textureSize.width-canvasSize.width)/2;
	float translation_y = (textureSize.height-canvasSize.height)/2;
	
	canvas->begin();
	// 가이드라인을 그려줄때도 translate 해야 텍스쳐 좌표계와 가이드 좌표계가 맞음
	CCPoint originalPos = sprGuide->getPosition();
	sprGuide->setPosition( ccp(translation_x + originalPos.x, translation_y + originalPos.y) );
	sprGuide->visit(); // 가이드라인을 같이 캡쳐하기 위해서 캔바스에 그려준다
	sprGuide->setPosition(originalPos);
	canvas->end();
	
	return canvas->saveBuffer(sdcardFullPath, translation_x, translation_y, canvasSize.width, canvasSize.height);
}

CCRawImageData *VVCanvasLayer::getCanvasImage()
{
	// 텍스쳐 크기가 화면크기보다 크기때문에 찍는 점을 평행이동을 해줘야 제대로 포인팅 됨
	float translation_x = (textureSize.width-canvasSize.width)/2;
	float translation_y = (textureSize.height-canvasSize.height)/2;
	
	canvas->begin();
		// 가이드라인을 그려줄때도 translate 해야 텍스쳐 좌표계와 가이드 좌표계가 맞음
		CCPoint originalPos = sprGuide->getPosition();
		sprGuide->setPosition( ccp(translation_x + originalPos.x, translation_y + originalPos.y) );
		sprGuide->visit(); // 가이드라인을 같이 캡쳐하기 위해서 캔바스에 그려준다
		sprGuide->setPosition(originalPos);
	canvas->end();

	return canvas->getRawImageFromBuffer(kCCImageFormatPNG,CCRectMake(translation_x, translation_y, canvasSize.width * CCDirector::sharedDirector()->getContentScaleFactor(), canvasSize.height * CCDirector::sharedDirector()->getContentScaleFactor()));
}


CCSprite* VVCanvasLayer::getCopiedCanvasSprite()
{
    CCImage *ccImg = new CCImage;
    canvas->getUIImageFromBuffer(ccImg);
    

    
    CCTexture2D* tex = new CCTexture2D();
    tex->initWithImage(ccImg);
    delete ccImg;
    
    CCSprite* spr = CCSprite::spriteWithTexture(tex);
    tex->release();
//MOD: 이 텍스쳐이미지는 캐싱이 안되고있음, 필요시 volatileTexture에 추가 해야한다!!!!!    
//    VolatileTexture::addCCImageTexture(tex,ccImg);
    
    return spr;
}

void VVCanvasLayer::redrawCanvasWithSprite(CCSprite* pSprite)
{
	float translation_x = (textureSize.width-canvasSize.width)/2;
	float translation_y = (textureSize.height-canvasSize.height)/2;

    clearCanvas();
	
	if (!pSprite) {
		return;
	}
    
	CCPoint oriPosition = pSprite->getPosition();
	pSprite->setPosition(ccp(translation_x + canvas->getPosition().x, translation_y + canvas->getPosition().y));
	
    canvas->begin();
        pSprite->visit();
	canvas->end(false);
    
	pSprite->setPosition(oriPosition);
}

void VVCanvasLayer::setBrushScale(float aScale)
{
    baseScale = aScale;
	brush->setScale(baseScale);
}

void VVCanvasLayer::setBrushOpacity(unsigned int aOpacity)
{
	brush->setOpacity(aOpacity);
}

void VVCanvasLayer::setBrushColor(ccColor3B aColor)
{
	brush->setColor(aColor);
}

void VVCanvasLayer::eraseView()
{
	CCLOG("erase_shaked");
}

void VVCanvasLayer::fillLayer()
{
	canvas->clear(brush->getColor().r / 255.0f, brush->getColor().g / 255.0f, brush->getColor().b / 255.0f, 1);
}

// 캔버스가 scaling 되도 동일하게 동작하도록 코드 수정
void VVCanvasLayer::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
	
	// 텍스쳐 크기가 화면크기보다 크기때문에 찍는 점을 평행이동을 해줘야 제대로 포인팅 됨
	float translation_x = (textureSize.width-canvasSize.width)/2;
	float translation_y = (textureSize.height-canvasSize.height)/2;
	
	CCTouch *touch = (CCTouch*)touches->anyObject();
	CCPoint start = this->convertTouchToNodeSpace(touch);

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	//MOD: 갤노트에서 이상하게 이 코드에 딜레이가 생긴다.
	canvas->begin();
		brush->setPosition(ccp(translation_x+start.x - this->getPosition().x + this->getContentSize().width * this->getAnchorPoint().x,
							   translation_y+start.y - this->getPosition().y + this->getContentSize().height * this->getAnchorPoint().y));
		// Call visit to draw the brush, don't call draw..
		brush->visit();
	canvas->end();
#endif

    if( onGoingAction != NULL)
        delete onGoingAction;
    
    onGoingAction = new StrokeAction(this);
}

// 캔버스가 scaling 되도 동일하게 동작하도록 코드 수정
void VVCanvasLayer::ccTouchesMoved(CCSet* touches, CCEvent* event)
{
    if( !onGoingAction )
        return;
    
	CCTouch *touch = (CCTouch*)touches->anyObject();
    CCPoint start = this->convertTouchToNodeSpace(touch);
	CCPoint end = touch->previousLocationInView();
	end = CCDirector::sharedDirector()->convertToGL(end);
	end = this->convertToNodeSpace(end);
    
    StrokeInfo stroke;
    stroke.start = start;
    stroke.end = end;
    stroke.type = touch->getType();
    stroke.pressure = touch->getPressure();    
    stroke.color = brush->getColor();

    onGoingAction->addStroke(stroke);
    
    drawStroke(stroke);

}

void VVCanvasLayer::drawStroke(StrokeInfo aStroke)
{
    CCPoint start = aStroke.start;
    CCPoint end = aStroke.end;
    
    // 텍스쳐 크기가 화면크기보다 크기때문에 찍는 점을 평행이동을 해줘야 제대로 포인팅 됨
	float translation_x = (textureSize.width-canvasSize.width)/2*this->getScale();
	float translation_y = (textureSize.height-canvasSize.height)/2*this->getScale();
    
   
	canvas->begin();
	float distance = ccpDistance(start, end);
	if (distance > 1)
	{
		int d = (int)distance;
		for (int i = 0; i < d; i++)
		{            
			float difx = end.x - start.x;
			float dify = end.y - start.y;
			float delta = (float)i / distance;
			brush->setPosition(ccp(translation_x + start.x + (difx * delta), translation_y + start.y + (dify * delta)) );
            if( aStroke.type == TOOL_TYPE_STYLUS )
            {
                brush->setColor(aStroke.color);
                brush->setScale( (baseScale-0.2)*(aStroke.pressure));
                brush->visit();
            }
            else if (aStroke.type == TOOL_TYPE_ERASER )
            {
                brush->setColor(ccc3(255, 255, 255));
                brush->setScale(baseScale);
                brush->visit();
                brush->setColor(aStroke.color);
            }
            else
            {
                brush->setColor(aStroke.color);
                brush->setScale(baseScale);
                brush->visit();
            }
            
		}
	}
	//안드로이드는 Volatile Texturecache를 사용해야함
	//CCRenderTextureTest 코드를 참조할것
#if CC_ENABLE_CACHE_TEXTTURE_DATA
    canvas->end(false);
#else
	canvas->end();
#endif 
}
void VVCanvasLayer::ccTouchesEnded(CCSet* touches, CCEvent* event)
{
    if( onGoingAction != NULL)
    {
        saveHistory(onGoingAction);
        onGoingAction = NULL;
    }
    
#if CC_ENABLE_CACHE_TEXTTURE_DATA
	// write on VolatileTextureCache
	// end(true)를 설정한경우 CCTextureCache에 이름이 유니크하게 setting된 RenderTexture를 register 해줘야한다.
	// 해당 클래스가 끝날때 unregister 하는것도 잊지 말것.
	canvas->begin();
	canvas->end(false);
#endif
}

void VVCanvasLayer::drawBrushAt(CCPoint pos)
{
	/*
	GLubyte oriOpacity = brush->getOpacity();
	canvas->begin();
		brush->setOpacity(160);
		brush->setPosition(pos);
		brush->visit();
		brush->setOpacity(oriOpacity);
	 
	 //안드로이드는 Volatile Texturecache를 사용해야함
	 //CCRenderTextureTest 코드를 참조할것
	 #if CC_ENABLE_CACHE_TEXTTURE_DATA
	 canvas->end(false);
	 #else
	 canvas->end();
	 #endif
	 */
	
}


void VVCanvasLayer::drawLineBetween(CCPoint start, CCPoint end, bool isUsingCache)
{
	canvas->begin();
	float distance = ccpDistance(start, end);
	
	if (distance > 1)
	{
		int d = (int)distance;
		for (int i = 0; i < d; i++)
		{
			float difx = end.x - start.x;
			float dify = end.y - start.y;
			float delta = (float)i / distance;
			brush->setPosition(ccp(start.x + (difx * delta),  start.y + (dify * delta)) );
			// Call visit to draw the brush, don't call draw..
			brush->visit();
		}
	}
	//안드로이드는 Volatile Texturecache를 사용해야함
	//CCRenderTextureTest 코드를 참조할것
#if CC_ENABLE_CACHE_TEXTTURE_DATA
    canvas->end(isUsingCache);
#else
	canvas->end();
#endif
}


void VVCanvasLayer::undo()
{
    if(undoHistory.size() == 0)
        return;
    
    // 언두 히스토리에서  마지막 작업을 리두로 넘기고 모두 클리어 한 후에 최근작업까지를 새로 반복.
    int lastIdx = undoHistory.size()-1;
    redoHistory.push( undoHistory[lastIdx] );
    
    undoHistory.erase(undoHistory.begin() + lastIdx );
    
    clearCanvas();
    for (int i = 0; i < undoHistory.size(); i++)
    {
        undoHistory[i]->execute();
    }
}
void VVCanvasLayer::redo()
{
    if(redoHistory.size() == 0)
        return;
    
    CanvasAction* action = redoHistory.top();
    undoHistory.push_back( action );
    action->execute();    
    redoHistory.pop();
        
}

void VVCanvasLayer::saveHistory(CanvasAction* action)
{
    undoHistory.push_back(action);
    while (redoHistory.size() > 0) 
    {
        delete redoHistory.top();
        redoHistory.pop();
    }
}

void VVCanvasLayer::purgeHistory()
{
    for(int i = 0; i < undoHistory.size(); i++ )
    {
        delete undoHistory[i];
    }
    
    while( redoHistory.size() > 0)
    {
        delete redoHistory.top();
        redoHistory.pop();
    }
}

void VVCanvasLayer::onExit()
{
	CCTextureCache::sharedTextureCache()->removeUnusedTextures();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	CCTextureCache::sharedTextureCache()->unregisterRenderTexture(canvas);
#endif
	
	CCLayerColor::onExit();
}

VVCanvasLayer::~VVCanvasLayer()
{
	CC_SAFE_RELEASE_NULL(brush);
}