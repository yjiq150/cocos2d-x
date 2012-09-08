// #include <JniHelper.h>
#include "SimpleAudioEngineJni.h"
#include <android/log.h>
#include <string>

#define  LOG_TAG    "libSimpleAudioEngine"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define  CLASS_NAME "org/cocos2dx/lib/Cocos2dxActivity"


#define JAVAVM	jvm

using namespace std;
typedef struct JniMethodInfo_
{
	JNIEnv *    env;
	jclass      classID;
	jmethodID   methodID;
} JniMethodInfo;

static JavaVM *jvm = 0;
jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
	jvm = vm;

	return JNI_VERSION_1_4;
}

extern "C"
{
static bool getEnv(JNIEnv **env);
static jclass getClassID_(const char *className, JNIEnv *env)
{
	JNIEnv *pEnv = env;
	jclass ret = 0;

	do 
	{
		if (! pEnv)
		{
			if (! getEnv(&pEnv))
			{
				break;
			}
		}
		
		ret = pEnv->FindClass(className);
		if (! ret)
		{
			 LOGD("Failed to find class of %s", className);
			break;
		}
	} while (0);

	return ret;
}

static bool getStaticMethodInfo_(JniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode)
{
	jmethodID methodID = 0;
	JNIEnv *pEnv = 0;
	bool bRet = false;

    do 
    {
		if (! getEnv(&pEnv))
		{
			break;
		}

        jclass classID = getClassID_(className, pEnv);

        methodID = pEnv->GetStaticMethodID(classID, methodName, paramCode);
        if (! methodID)
        {
            LOGD("Failed to find static method id of %s", methodName);
            break;
        }

		methodinfo.classID = classID;
		methodinfo.env = pEnv;
		methodinfo.methodID = methodID;

		bRet = true;
    } while (0);

    return bRet;
}

static bool getMethodInfo_(JniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode)
{
	jmethodID methodID = 0;
	JNIEnv *pEnv = 0;
	bool bRet = false;

	do 
	{
		if (! getEnv(&pEnv))
		{
			break;
		}

		jclass classID = getClassID_(className, pEnv);

		methodID = pEnv->GetMethodID(classID, methodName, paramCode);
		if (! methodID)
		{
			LOGD("Failed to find method id of %s", methodName);
			break;
		}

		methodinfo.classID = classID;
		methodinfo.env = pEnv;
		methodinfo.methodID = methodID;

		bRet = true;
	} while (0);

	return bRet;
}

static string jstring2string_(jstring jstr)
{
	JNIEnv *env = 0;

	jboolean isCopy;
	if (! getEnv(&env))
	{
		return 0;
	}

	const char* chars = env->GetStringUTFChars(jstr, &isCopy);
	string ret(chars);
	if (isCopy)
	{
		env->ReleaseStringUTFChars(jstr, chars);
	}

	return ret;
}
}

class  JniHelperDenshion
{
public:

	static jclass getClassID(const char *className, JNIEnv *env)
	{
		return getClassID_(className, env);
	}

	static bool getStaticMethodInfo(JniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode)
	{
		return getStaticMethodInfo_(methodinfo, className, methodName, paramCode);
	}

	static bool getMethodInfo(JniMethodInfo &methodinfo, const char *className, const char *methodName, const char *paramCode)
	{
		return getMethodInfo_(methodinfo, className, methodName, paramCode);
	}

	static string jstring2string(jstring str)
	{
		return jstring2string_(str);
	}

};



extern "C"
{

    //////////////////////////////////////////////////////////////////////////
    // java vm helper function
    //////////////////////////////////////////////////////////////////////////

	static bool getEnv(JNIEnv **env)
	{
		bool bRet = false;

		do 
		{
			if (JAVAVM->GetEnv((void**)env, JNI_VERSION_1_4) != JNI_OK)
			{
				LOGD("Failed to get the environment using GetEnv()");
				break;
			}

			if (JAVAVM->AttachCurrentThread(env, 0) < 0)
			{
				LOGD("Failed to get the environment using AttachCurrentThread()");
				break;
			}

			bRet = true;
		} while (0);		

		return bRet;
	}

}


extern "C"
{
	// get env and cache it
	static JNIEnv* getJNIEnv(void)
	{
        if (NULL == jvm) {
            LOGD("Failed to get JNIEnv. JniHelper::getJavaVM() is NULL");
            return NULL;
        }

		JNIEnv *env = NULL;
		// get jni environment
        jint ret = jvm->GetEnv((void**)&env, JNI_VERSION_1_4);

        switch (ret) {
        case JNI_OK :
            // Success!
            return env;

        case JNI_EDETACHED :
            // Thread not attached

            // TODO : If calling AttachCurrentThread() on a native thread
            // must call DetachCurrentThread() in future.
            // see: http://developer.android.com/guide/practices/design/jni.html

            if (jvm->AttachCurrentThread(&env, NULL) < 0)
            {
                LOGD("Failed to get the environment using AttachCurrentThread()");
                return NULL;
            } else {
                // Success : Attached and obtained JNIEnv!
                return env;
            }

        case JNI_EVERSION :
            // Cannot recover from this error
			LOGD("JNI interface version 1.4 not supported");
        default :
			LOGD("Failed to get the environment using GetEnv()");
            return NULL;
        }
	}

	// get class and make it a global reference, release it at endJni().
	static jclass getClassID(JNIEnv *pEnv)
	{
		jclass ret = pEnv->FindClass(CLASS_NAME);
		if (! ret)
		{
			LOGD("Failed to find class of %s", CLASS_NAME);
		}

		return ret;
	}

	static bool getStaticMethodInfo(JniMethodInfo &methodinfo, const char *methodName, const char *paramCode)
    {
		jmethodID methodID = 0;
		JNIEnv *pEnv = 0;
		bool bRet = false;

        do 
        {
			pEnv = getJNIEnv();
			if (! pEnv)
			{
				break;
			}

            jclass classID = getClassID(pEnv);

            methodID = pEnv->GetStaticMethodID(classID, methodName, paramCode);
            if (! methodID)
            {
                LOGD("Failed to find static method id of %s", methodName);
                break;
            }

			methodinfo.classID = classID;
			methodinfo.env = pEnv;
			methodinfo.methodID = methodID;

			bRet = true;
        } while (0);

        return bRet;
    }

	void preloadBackgroundMusicJNI(const char *path)
	{
		// void playBackgroundMusic(String,boolean)
		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "preloadBackgroundMusic", "(Ljava/lang/String;)V"))
		{			
			return;
		}

		jstring stringArg = methodInfo.env->NewStringUTF(path);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
		methodInfo.env->DeleteLocalRef(stringArg);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void playBackgroundMusicJNI(const char *path, bool isLoop)
	{
		// void playBackgroundMusic(String,boolean)

		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "playBackgroundMusic", "(Ljava/lang/String;Z)V"))
		{
			return;
		}

		jstring stringArg = methodInfo.env->NewStringUTF(path);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg, isLoop);
		methodInfo.env->DeleteLocalRef(stringArg);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void stopBackgroundMusicJNI()
	{
		// void stopBackgroundMusic()

		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "stopBackgroundMusic", "()V"))
		{
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void pauseBackgroundMusicJNI()
	{
		// void pauseBackgroundMusic()

		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "pauseBackgroundMusic", "()V"))
		{
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void resumeBackgroundMusicJNI()
	{
		// void resumeBackgroundMusic()

		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "resumeBackgroundMusic", "()V"))
		{
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void rewindBackgroundMusicJNI()
	{
		// void rewindBackgroundMusic()

		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "rewindBackgroundMusic", "()V"))
		{
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	bool isBackgroundMusicPlayingJNI()
	{
		// boolean rewindBackgroundMusic()

		JniMethodInfo methodInfo;
		jboolean ret = false;

		if (! getStaticMethodInfo(methodInfo, "isBackgroundMusicPlaying", "()Z"))
		{
			return ret;
		}

		ret = methodInfo.env->CallStaticBooleanMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);

		return ret;
	}

	float getBackgroundMusicVolumeJNI()
	{
		// float getBackgroundMusicVolume()

		JniMethodInfo methodInfo;
		jfloat ret = -1.0;

		if (! getStaticMethodInfo(methodInfo, "getBackgroundMusicVolume", "()F"))
		{
			return ret;
		}

		ret = methodInfo.env->CallStaticFloatMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);

		return ret;
	}

	void setBackgroundMusicVolumeJNI(float volume)
	{
		// void setBackgroundMusicVolume()

		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "setBackgroundMusicVolume", "(F)V"))
		{
			return ;
		}

	    methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, volume);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	unsigned int playEffectJNI(const char* path, bool bLoop)
	{
		// int playEffect(String)

		JniMethodInfo methodInfo;
		int ret = 0;

		if (! getStaticMethodInfo(methodInfo, "playEffect", "(Ljava/lang/String;Z)I"))
		{
			return ret;
		}

		jstring stringArg = methodInfo.env->NewStringUTF(path);
		ret = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID, stringArg, bLoop);
        methodInfo.env->DeleteLocalRef(stringArg);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);

		return (unsigned int)ret;
	}

	void stopEffectJNI(unsigned int nSoundId)
	{
		// void stopEffect(int)

		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "stopEffect", "(I)V"))
		{
			return ;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, (int)nSoundId);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void endJNI()
	{
		// void end()

		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "end", "()V"))
		{
			return ;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	float getEffectsVolumeJNI()
	{
		// float getEffectsVolume()

		JniMethodInfo methodInfo;
		jfloat ret = -1.0;

		if (! getStaticMethodInfo(methodInfo, "getEffectsVolume", "()F"))
		{
			return ret;
		}

		ret = methodInfo.env->CallStaticFloatMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);

		return ret;
	}

	void setEffectsVolumeJNI(float volume)
	{
		// void setEffectsVolume(float)
		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "setEffectsVolume", "(F)V"))
		{
			return ;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, volume);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void preloadEffectJNI(const char *path)
	{
		// void preloadEffect(String)

		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "preloadEffect", "(Ljava/lang/String;)V"))
		{
			return ;
		}

		jstring stringArg = methodInfo.env->NewStringUTF(path);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
		methodInfo.env->DeleteLocalRef(stringArg);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void unloadEffectJNI(const char* path)
	{
		// void unloadEffect(String)

		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "unloadEffect", "(Ljava/lang/String;)V"))
		{
			return ;
		}

		jstring stringArg = methodInfo.env->NewStringUTF(path);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
		methodInfo.env->DeleteLocalRef(stringArg);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void pauseEffectJNI(unsigned int nSoundId)
	{
		// void pauseEffect(int)

		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "pauseEffect", "(I)V"))
		{
			return ;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, (int)nSoundId);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void pauseAllEffectsJNI()
	{
		// void pauseAllEffects()

		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "pauseAllEffects", "()V"))
		{
			return ;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void resumeEffectJNI(unsigned int nSoundId)
	{
		// void resumeEffect(int)

		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "resumeEffect", "(I)V"))
		{
			return ;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, (int)nSoundId);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void resumeAllEffectsJNI()
	{
		// void resumeAllEffects()

		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "resumeAllEffects", "()V"))
		{
			return ;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void stopAllEffectsJNI()
	{
		// void stopAllEffects()

		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "stopAllEffects", "()V"))
		{
			return ;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
	
	
	
	// Added by YoungJae Kwon
		// ---- voice channel added----
	void playVoiceJNI(const char *path)
	{
		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "playVoice", "(Ljava/lang/String;)V"))
		{
			return;
		}

		jstring stringArg = methodInfo.env->NewStringUTF(path);
		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, stringArg);
		methodInfo.env->DeleteLocalRef(stringArg);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void stopVoiceJNI()
	{
		// void stopVoice()
		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "stopVoice", "()V"))
		{
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void pauseVoiceJNI()
	{
		// void pauseVoice()
		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "pauseVoice", "()V"))
		{
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}

	void resumeVoiceJNI()
	{
		// void resumeVoice()
		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "resumeVoice", "()V"))
		{
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
	void setVoiceMuteJNI(bool isMute)
	{
		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "setVoiceMute", "(Z)V"))
		{
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, isMute);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
	void setEffectMuteJNI(bool isMute)
	{
		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "setEffectMute", "(Z)V"))
		{
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, isMute);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
	void setBGMMuteJNI(bool isMute)
	{
		JniMethodInfo methodInfo;

		if (! getStaticMethodInfo(methodInfo, "setBGMMute", "(Z)V"))
		{
			return;
		}

		methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, isMute);
		methodInfo.env->DeleteLocalRef(methodInfo.classID);
	}
	
}