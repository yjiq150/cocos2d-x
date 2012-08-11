/*
 *  VVSWFNode.cpp
 *  Cocos2d_X_BasicTest
 *
 *  Created by YoungJae Kwon on 12. 4. 2.
 *  Copyright 2011  Viscuit Creative Inc. All rights reserved.
 *
 */

#include "VVSWFNode.h"
#include <vg/openvg.h>
#include <vg/vgu.h>

#include "VVSWFCache.h"


VVSWFNode::VVSWFNode() 
{
    m_swf = NULL;
    m_frame = 0;
    m_frameStart = 0;
    m_frameEnd = 0;
    m_elapsed = 0;
    m_fullPath = "";
    m_fileName = "";
    m_updateInterval = 0;
    m_currentAnimationName = "not set";
    delegate = NULL;
    isPaused = true;
    isAnimationDefinedInFlash = false;
    isSoundDefinedInFlash = false;
    
#ifdef COCOS2D_DEBUG       
    debugLabelPositionIdx = 0;    
#endif
    // register
    VVSWFCache::sharedSWFCache()->addSWFNode(this);

}

VVSWFNode::~VVSWFNode() 
{
    // this action is litte different from CCTextureCache : it caches texture by filename
    // VVSWFCache : it only cache VVSWFNode pointers for reload in Anroid platform, no need for iOS

    // unregister
    VVSWFCache::sharedSWFCache()->removeSWFNode(this);
    delete m_swf;

#ifdef COCOS2D_DEBUG   
    m_debugLabel->release();
#endif
}


bool VVSWFNode::initWithFile(const char * filename)
{
    m_fileName = filename;
    m_fullPath = CCFileUtils::fullPathFromRelativePath(filename);
    CCFileData data(m_fullPath.c_str(), "rb"); // 'data' will be automatically cleaned up after local scope.

    int length = (int)data.getSize();
    char *buffer = (char*)data.getBuffer();
    
    if(buffer == NULL)
        CCAssert(0, "SWF file open error");
    
    if (MonkSWF::SWF::isLogOn())
    {
        CCLog("=====================================");
        CCLog("Opening '%s'",m_fileName.c_str());
        CCLog("=====================================");
    }
    MonkSWF::Reader reader(buffer, length);
    m_swf = new MonkSWF::SWF();
    m_swf->initialize();
    m_swf->read(&reader);
    
    // SWF frame size == CCNode contentSize 
    this->setContentSize(CCSizeMake(m_swf->getFrameWidth(), m_swf->getFrameHeight()));
    this->setAnchorPoint(ccp(0.5, 0.5));

    m_updateInterval = 1.0f / m_swf->getFrameRate();
    
    // initial setting : play frames [0 end]
    // play interval will be changed by setAnimation(aniName)
    m_frameEnd = m_swf->numFrames() - 1;
    
    
    // load animation info file
    string aniFilePath = m_fullPath + ".ani"; // Filename example: flash.swf.ani 
    CCFileData aniFileData(aniFilePath.c_str(), "rb");
	unsigned long size = aniFileData.getSize();
	char *pBuffer = (char*) aniFileData.getBuffer();
	
    
    
	if (pBuffer != NULL)
    {
        Json::Reader* jsonReader = new Json::Reader();
        bool isParsingSuccessful = jsonReader->parse( pBuffer, pBuffer+size, m_jsonObj,false );

        if ( !isParsingSuccessful )
        {
            string err = "JSON parsing Error :" + aniFilePath;
            CCAssert(0,err.c_str());
        }
        
#if !(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
        // sth wrong in NDK free() mechanism, so let it leak on android(BUG).
        delete jsonReader;
#endif
        
	}

//    libjson example code
//    std::string json = "{\"RootA\":\"Value in parent node\",\"ChildNode\":{\"ChildA\":\"String Value\",\"ChildB\":42}}";
//    JSONNode n = libjson::parse(json);

#ifdef COCOS2D_DEBUG 
    //m_debugLabel = CCLabelBMFont::labelWithString("","font/log18.fnt");
    m_debugLabel = CCLabelTTF::labelWithString("", "Arial", 16);
    m_debugLabel->retain();
#endif
    
    return true;
}
CCRect VVSWFNode::getMovieClipRectWithInstanceName(const char* instName)
{

    MonkSWF::RECT r = m_swf->getMovieClipRectWithInstanceName(instName,m_frame);

    if( r.xmax == 0 && r.xmin == 0 && r.ymin == 0 && r.ymax == 0 )
        return CCRectMake(0, 0, 0, 0);
    
    // change "y axis in opposite", and take the anchor point to "left bottom"
    float x =       r.xmin/20.0f;
    float y =       m_swf->getFrameHeight()-(r.ymax/20.0f);
    float width  =  (r.xmax-r.xmin)/20.0f;
    float height =  (r.ymax-r.ymin)/20.0f;
    
    //주의: 디버그 드로잉과 실제 렉트 영역은 origin셋팅이 다르다!!
    return CCRectMake(m_tPosition.x + x - m_tContentSize.width * m_tAnchorPoint.x*m_fScaleX, 
                      m_tPosition.y + y - m_tContentSize.height * m_tAnchorPoint.y*m_fScaleY,
                      width*m_fScaleX, 
                      height*m_fScaleY);
 
}

void VVSWFNode::resetToFullAnimationFrom(int frame_idx)
{
    isTransitToNewAnimation = false;
    
    m_currentAnimationName = "not set";
    m_frameStart = 0;
    m_frameEnd = m_swf->numFrames() - 1;
    
    m_frame = frame_idx - 1; // MOD: 여기 -1 맞나?    
}

/*
void VVSWFNode::ParseJSON(const JSONNode & n)
{
    JSONNode::const_iterator i = n.begin();
    while (i != n.end()){

        if (i -> type() == JSON_ARRAY || i -> type() == JSON_NODE){
            ParseJSON(*i);
        }
        
        std::string node_name = i -> name();
        
//#define JSON_NULL '\0'
//#define JSON_STRING '\1'
//#define JSON_NUMBER '\2'
//#define JSON_BOOL '\3'
//#define JSON_ARRAY '\4'
//#define JSON_NODE '\5'
        i->as_array()
        // find out where to store the values
        if (node_name == "RootA"){
            rootA = i -> as_string();
        }
        else if (node_name == "ChildA"){
            childA = i -> as_string();
        }
        else if (node_name == "ChildB")
            childB = i -> as_int();
        
        //increment the iterator
        ++i;
    }
    
}
 */

void VVSWFNode::setAnimation(const char* aniName, bool isSmoothTransition)
{
    int startVal;
    int endVal;
    
    if(isAnimationDefinedInFlash)
    {
        MonkSWF::AniInterval* interval = m_swf->getAniInterval(aniName);
//      CCAssert(interval, "aniName is not defined in flash file");
        if( interval == NULL)
        {
            CCLog("VVSWFNODE ERROR: '%s' is not defined.",aniName);
            return;
        }
        
        startVal = interval->start-1;
        endVal   = interval->end-1;
    }
    else
    {
        if( m_jsonObj.empty() )
            CCAssert(0, "no animation info found");
        
        const Json::Value interval = m_jsonObj[aniName];
        
        if( interval.empty() )
            CCAssert(0, "aniName not found Error");
        
        if (interval.size() != 2) 
            CCAssert(0, "aniInterval should be [st end]");
        
        startVal =  interval[0u].asInt()-1;
        endVal   =  interval[1].asInt()-1;
        
    }
    
    
    
    if( isSmoothTransition )
    {
        isTransitToNewAnimation = true;

        // set current frame to first frame of new ani
        // (flash frame idx - 1) == real frame idx
        m_frameNextStart = startVal;
        m_frameNextEnd   = endVal;
        
        m_nextAnimationNameAfterSmoothTransition = aniName;
    }
    else 
    {
        isTransitToNewAnimation = false;
        
        m_frameStart = startVal;
        m_frameEnd   = endVal;

        m_frame = m_frameStart;
        m_currentAnimationName = aniName;
    }
}
string VVSWFNode::getCurrentAnimationName()
{
    return m_currentAnimationName;
}

void VVSWFNode::play()
{
    if (!isPaused) 
        return;

    isPaused = false;
    scheduleUpdate();
}


void VVSWFNode::pause()
{
    if (isPaused) 
        return;
    
    isPaused = true;
    unscheduleUpdate();
}

// this reload method is only used for android volatile texture characteristics (related to glContext)
// therefore don't need to reload json
void VVSWFNode::reload()
{
    CCAssert(m_swf,"Reload can be called only after initWithFile" );
    delete m_swf; // delete exisiting swf
    
    CCFileData data(m_fullPath.c_str(), "rb"); // 'data' will be automatically cleaned up after local scope.
    
    int length = (int)data.getSize();
    char *buffer = (char*)data.getBuffer();
    
    if(buffer == NULL)
        CCAssert(0, "SWF file open error");
    
    // create a reader using the buffer
    MonkSWF::Reader reader(buffer, length);
    m_swf = new MonkSWF::SWF();
    m_swf->initialize();
    m_swf->read(&reader);
      
}

VVSWFNode* VVSWFNode::nodeWithFile(const char* filename)
{
    VVSWFNode* node = new VVSWFNode();
    node->initWithFile(filename);
    node->autorelease();
    return node;
}

void VVSWFNode::onEnter()
{
    CCNode::onEnter();
    
    // to prevent weird bug on Android 
    // don't update frame when file has only one frame.
    if( m_swf->numFrames() != 1)
        scheduleUpdate();
    
    isPaused = false;
}

void VVSWFNode::update(ccTime delta)
{    
    m_elapsed += delta;
    
    while (m_elapsed > m_updateInterval) 
    {
        m_elapsed -= m_updateInterval;
        m_frame++;
   
        // this is required to keep independent timeline of MovieClips
        m_swf->updateFrameIdx();
        
        if( isSoundDefinedInFlash )
        {
            string fileName;
            fileName = m_swf->getSoundFileNameAt(m_frame);
            //Warning! if this string value is "", this empty string cause libc dlfree crash in android NDK when fileName is deallocated!
            if( fileName != " " )
            {
                if(delegate)
                    delegate->didSoundStarted(this, fileName); 
            }
        }
        else
        {
            Json::Value sound = m_jsonObj["sound"];
            if( !sound.isNull())
            {
                for( Json::ValueIterator it = sound.begin() ; it != sound.end() ; it++ ) 
                {
                    // 사운드 시작프레임값과 현재프레임이 같으면 사운드 재생
                    if( (*it).isInt() && (*it).asInt() == m_frame )            
                    {
                        string name =  it.key().asString();
                        
                        // 동일이름 사운드 여러번 재생시 중복키값막기.
                        if( name.size() == 9)
                            name = name.substr(0,7);
                        
                        if(delegate)
                            delegate->didSoundStarted(this, name); 
                        
                        break;
                    }
                }
            }
        }
        
        if (m_frame > m_frameEnd) 
        {
            
            if (isTransitToNewAnimation) 
            {
                // 진행중이던 애니메이션이 끝난 후 원래 예정되어있던 애니메이션으로 값 세팅
                m_frameStart = m_frameNextStart;
                m_frameEnd = m_frameNextEnd;
                m_currentAnimationName = m_nextAnimationNameAfterSmoothTransition;
                
                m_frame = m_frameStart;
            }
            else 
            {
                // should be called before delegation.
                // for appropriate operation when jumpTo is called in didAnimationFinished
                m_frame = m_frameStart; 
                
                // didAnimationFinished is not called when animation is finished manually by setAnimation()
                if(delegate)
                    delegate->didAnimationFinished(this);    
            }            
        }
    }

}

void VVSWFNode::onExit()
{
    isPaused = true;
    unscheduleUpdate();
    CCNode::onExit();
}

string VVSWFNode::getKey()
{
    CCAssert(m_fullPath.length(), "swf init error");
    return m_fullPath;
}

// this only works when frame is within current animation's interval.
// if you want to jumpTo outside the interval, place setAnimation() first and then jumpTo().
void VVSWFNode::jumpTo(string frameName)
{
    m_frame = m_swf->getFrameNumberWithFrameLabel(frameName);
}

void VVSWFNode::jumpToEnd()
{
    m_frame = m_frameEnd;
}
void VVSWFNode::jumpTo(int frameIdx)
{
    m_frame = frameIdx;
}
void VVSWFNode::draw()
{    
    /*
    //ToDo: VVSWFNode is currently only works on projection2D mode
    CCDirector *director;
    director = [CCDirector sharedDirector];
    
    ccDirectorProjection projection;
    projection = director.projection;
    [director setProjection:kCCDirectorProjection2D];
    */
    
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    glPushMatrix();
    
    vgLoadIdentity();


    //앵커기준 플래쉬 좌표계 사용시(좌상단 시작점)
//    vgTranslate(-m_swf->getFrameWidth() - m_swf->getFrameWidth() * getAnchorPoint().x * -1.0f,
//                -m_swf->getFrameHeight() - m_swf->getFrameHeight() * getAnchorPoint().y * -1.0f);

    //앵커기준 OpenGL 좌표계사용시
    vgTranslate( m_swf->getFrameWidth() * getAnchorPoint().x * -1.0f,
                -m_swf->getFrameHeight() - m_swf->getFrameHeight() * getAnchorPoint().y * -1.0f);

    // convert flash coordinate to openvg
//    vgScale(getScaleX() / CC_CONTENT_SCALE_FACTOR() * 1.0f, getScaleY() / CC_CONTENT_SCALE_FACTOR() * -1.0f);
    vgScale(getScaleX() * 1.0f, getScaleY() * -1.0f);    
    vgRotate(getRotation() * -1.0f);
    vgTranslate(this->getPositionInPixels().x, this->getPositionInPixels().y);

    //CCLog("cur frame:%d",m_frame);
    if( m_frame < 0 || m_frame >= 65535)
    {
        CCLog("frame_jump error occured!!!: This error occurs only in Simulator by Unknown cause");
        m_frame = 0;
        m_frameStart = 0;
        m_frameEnd = m_swf->numFrames()-1;
    }
    else 
        m_swf->drawFrame(m_frame);
    
	
    glPopMatrix();
    
    glEnable(GL_TEXTURE_2D);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    //ToDo: recover prev projection setting
    //[director setProjection:projection];
    

#ifdef COCOS2D_DEBUG // debug Drawnig code

    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_TEXTURE_2D);
    //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
    
    // set Color of MovieClip Area for touch 
    static ccColor4B  col[4];
    for( unsigned int i=0; i < 4; i++ )
    {
        col[i].r = 200;
        col[i].g = 0;
        col[i].b = 0;
        col[i].a = 30;
    }
    
    glColorPointer(4, GL_UNSIGNED_BYTE, 0, col);
    
    // draw touchable movieClip rects
    vector<MonkSWF::RECT> *rects = m_swf->getMovieClipRects(m_frame);
    for( int i = 0; i < rects->size(); i++ )
    {
        MonkSWF::RECT r = rects->at(i);
        // change "y axis in opposite", and take the anchor point to "left bottom"
        float x =       r.xmin/20.0f;
        float y =       m_swf->getFrameHeight()-(r.ymax/20.0f);
        float width  =  (r.xmax-r.xmin)/20.0f;
        float height =  (r.ymax-r.ymin)/20.0f;
        
        x = x - m_tContentSize.width * m_tAnchorPoint.x*m_fScaleX ;
        y = y - m_tContentSize.height * m_tAnchorPoint.y*m_fScaleY;
        CCSize s = CCSizeMake(width*m_fScaleX, height*m_fScaleY);

        static ccVertex2F vert[4];
        
        vert[0].x = x;         vert[0].y = y;
        vert[1].x = x+s.width; vert[1].y = y;
        vert[2].x = x+s.width; vert[2].y = y+s.height;
        vert[3].x = x;         vert[3].y = y+s.height;
        
        glVertexPointer(2, GL_FLOAT, 0, vert);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    }
    
    // restore default GL state
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnable(GL_TEXTURE_2D);
    
    
    static char buf[128];

    sprintf(buf,"[%s]: '%s'\t(%d, %d, %d)",m_fileName.c_str(),m_currentAnimationName.c_str(), m_frameStart+1, m_frame+1, m_frameEnd+1);
    m_debugLabel->setString(buf);
    
    glTranslatef(200, 30*debugLabelPositionIdx, 0);
    m_debugLabel->draw();
    glTranslatef(200, -30*debugLabelPositionIdx, 0);    
#endif
    
}
