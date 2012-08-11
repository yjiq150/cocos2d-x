/*
 *  VVSWFNode.h
 *  Cocos2d_X_BasicTest
 *
 *  Created by YoungJae Kwon on 12. 4. 2.
 *  Copyright 2011  Viscuit Creative Inc. All rights reserved.
 *
 */

#ifndef __VVSWFNode_H__
#define __VVSWFNode_H__


#include "cocos2d.h"
#include "MonkSWF.h"
#include <json/json.h>
//#include "libjson.h"

class VVSWFNode;

class VVSWFNodeDelegate
{
public:
	virtual void didAnimationFinished(VVSWFNode* node){ return; }
	virtual void didSoundStarted(VVSWFNode* node, string& name){ return; }  
};

class VVSWFNode : public CCNode 
{
public:
    VVSWFNode();
    ~VVSWFNode();
    
    bool initWithFile(const char * filename);
    static VVSWFNode* nodeWithFile(const char* filename);
    static void setIsLogOn(bool val){MonkSWF::SWF::setIsLogOn(val);}
    
    void onEnter();
    void onExit();
    
    string getKey();
    void reload();
    void resetToFullAnimationFrom(int frame_idx);
    void setAnimation(const char* aniName, bool isSmoothTransition = false);
    string getCurrentAnimationName();
    
    CCRect getMovieClipRectWithInstanceName(const char* instName);

    void jumpToEnd();
    void jumpTo(string frameName);
    void jumpTo(int frameIdx);
    void play();
    void pause();    
    
    
protected:
    void update(ccTime delta);
    void draw();
    
    CC_SYNTHESIZE(ccTime, m_elapsed, Elapsed);

    CC_SYNTHESIZE(bool, isPaused,IsPaused);
    CC_SYNTHESIZE(bool, isAnimationDefinedInFlash,IsAnimationDefinedInFlash);
    CC_SYNTHESIZE(bool, isSoundDefinedInFlash,IsSoundDefinedInFlash);    
    
    int m_frame;
    int m_frameStart;
    int m_frameEnd;
    int m_frameNextStart;
    int m_frameNextEnd;
    
    ccTime m_updateInterval;
    
    bool isTransitToNewAnimation; // 이 애니메이션이 끝나면 다른 애니로 넘어가는지 표시
    string m_currentAnimationName;
    string m_nextAnimationNameAfterSmoothTransition;
    
    MonkSWF::SWF *m_swf;
    string m_fullPath;
    string m_fileName;
    Json::Value m_jsonObj;
    
    CC_SYNTHESIZE(VVSWFNodeDelegate*, delegate, Delegate);

#ifdef COCOS2D_DEBUG    
    CCLabelTTF* m_debugLabel;
    CC_SYNTHESIZE(int, debugLabelPositionIdx, DebugLabelPositionIdx);
#endif

};

#endif