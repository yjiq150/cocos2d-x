#include "cocos2d.h"
#include "VVSWFCache.h"
#include <jni.h>
#include <android/log.h>

#define  LOG_TAG    "VVSWFCacheJNI"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

extern "C"
{

void Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeReloadSWFs(JNIEnv*  env, jobject thiz)
{
    if (!cocos2d::CCDirector::sharedDirector()->getOpenGLView())
    {
		// first initilzation do nothing
    }
    else
    {
        VVSWFCache::sharedSWFCache()->reloadAll();
    }
}

}
