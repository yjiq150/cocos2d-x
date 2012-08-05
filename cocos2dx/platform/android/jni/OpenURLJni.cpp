#include "OpenURLJni.h"
#include "android/jni/JniHelper.h"

#include <android/log.h>
#include <jni.h>

#define  LOG_TAG    "OpenURLJNI"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

using namespace cocos2d;

extern "C"
{

	void openURLJNI(const char * url)
	{
		
		if (! url)
		{
			return;
		}

        JniMethodInfo t;
		if (JniHelper::getStaticMethodInfo(t
            , "org/cocos2dx/lib/Cocos2dxActivity"
            , "openURL"
            , "(Ljava/lang/String;)V"))
		{
			LOGD("Opening URL:%s",url);
			jstring StringArg = t.env->NewStringUTF(url);
			t.env->CallStaticVoidMethod(t.classID, t.methodID, StringArg);
		}
	}
}