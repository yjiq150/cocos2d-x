//
//  VVCanvasLayer.h
//  CocosX_test
//
//  Created by YoungJae Kwon on 11. 5. 21..
//  Copyright __MyCompanyName__ 2011. All rights reserved.
//

#ifndef __VVCanvasLayer_H__
#define __VVCanvasLayer_H__

#include "cocos2d.h"
#include "CCRawImageData.h"
#include "CCGL.h" // glClear 바로 사용위함

#include <stack>
#include <vector>

enum {
	kTag_CanvasCanvas = 1,
	kTag_CanvasGuide = 2,
	kTag_CanvasFrame = 3
};

using namespace cocos2d;
class CanvasAction;
class StrokeAction;

typedef struct 
{
    typedef enum {
        ActionType_Began,
        ActionType_Move,
        ActionType_End
    } StrokeActionType;
    
    StrokeActionType actionType;

    CCPoint start;
    CCPoint end;
    float pressure;
    ccColor3B color;
    float scale;
    TouchType type;
    
} StrokeInfo;

class VVCanvasLayer : public CCLayerColor
{
	
public:
    VVCanvasLayer();
	virtual bool init();  
    virtual bool initWithBox(CCSize s, char* sprName);
	virtual bool initWithBox(CCSize s, const char* guideName, CCSprite *backgroundFrame);

	virtual ~VVCanvasLayer();
	virtual void onExit();
	//virtual void onEnter();
	
	void ccTouchesBegan(CCSet* touches, CCEvent* event);
	void ccTouchesMoved(CCSet* touches, CCEvent* event);
	void ccTouchesEnded(CCSet* touches, CCEvent* event);
	

	CC_SYNTHESIZE_RETAIN(CCSprite*,brush,Brush);
	
	CCRawImageData *getCanvasImage();
	void rotateCanvas(CCSize s);
	void setBrushScale(float aScale);
	void setBrushOpacity(unsigned int aOpacity);
	void setBrushColor(ccColor3B color);
	void eraseView();
	void fillLayer();
	void clearCanvas();
    void clearCanvasAsAction();
	

	CCSprite*   getCopiedCanvasSprite();
	void        redrawCanvasWithSprite(CCSprite* pSprite);
    
	bool saveToFile(const char* sdcardFullPath);

	void drawBrushAt(CCPoint pos);
	void drawLineBetween(CCPoint start, CCPoint end, bool isUsingCache);
    
    void drawStroke(StrokeInfo stroke);
    void undo();
    void redo();
    
    void executeFadeOut(ccTime dur);
    
protected:
    
	CC_SYNTHESIZE(ccColor4B, canvasColor, CanvasColor);
	CC_SYNTHESIZE_READONLY(CCSprite*, sprGuide, GuideSprite);
	CC_SYNTHESIZE_READONLY(CCRenderTexture*, canvas, CanvasRenderTexture);
	CC_SYNTHESIZE_READONLY(CCSize, canvasSize, CanvasSize);
	CC_SYNTHESIZE_READONLY(CCSize, textureSize, TextureSize);
	
	unsigned char* m_pData; // pixel data for getCanvasSprite
    
    // undo manager
    void saveHistory(CanvasAction* action);
    void purgeHistory();
    vector<CanvasAction*> undoHistory;
    stack<CanvasAction*> redoHistory;
    
    
    StrokeAction* onGoingAction;
    
    float baseScale;
};




class CanvasAction
{
public:
    virtual void execute(){};

protected:    
    VVCanvasLayer* refVVCanvasLayer;
    
};

class StrokeAction : public CanvasAction
{

public:   
    StrokeAction(){ CCAssert(0, "Use constructor StrokeAction(VVCanvasLayer* aVVCanvasLayer)"); }
    StrokeAction(VVCanvasLayer* aVVCanvasLayer){ refVVCanvasLayer = aVVCanvasLayer; }
    virtual void execute()
    {
        int count = strokesInAction.size();
        for(int i=0; i < count; i++)
        {
            refVVCanvasLayer->drawStroke( strokesInAction[i] );
        }       
    }
    void addStroke(StrokeInfo aStroke)
    {
        strokesInAction.push_back(aStroke);
    }
    
private:    
    vector<StrokeInfo> strokesInAction;    
};

class ClearAction : public CanvasAction
{
    
public:   
    ClearAction(){ CCAssert(0, "Use constructor StrokeAction(VVCanvasLayer* aVVCanvasLayer)"); }
    ClearAction(VVCanvasLayer* aVVCanvasLayer){ refVVCanvasLayer = aVVCanvasLayer; }
    virtual void execute()
    {
        refVVCanvasLayer->clearCanvas();     
    }
    
private:    

};

#endif // __CANVAS_LAYER_H__
