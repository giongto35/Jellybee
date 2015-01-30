#include "J2CBridge.h"
#include "FileManager.h"
#include "NameSpace.h"

#ifdef _ANDROID

static JavaVM* jVM;

static jobject sound_bridge;
static jmethodID android_initSound;
static jmethodID android_playSound;
static jmethodID android_playBackGround;
static jmethodID android_stop;
static jmethodID android_pauseAll;
static jmethodID android_resumeAll;
static jclass android_soundClass;

static jobject social_bridge;
static jmethodID android_fbSharing;
static jmethodID android_makeScreenShot;
static jclass android_facebookClass;

extern "C" 
{
	void Java_com_android_jellybee_SoundInGame_SoundBridgeJ2C(JNIEnv * env, jobject thiz, jobject obj)
	{
		sound_bridge = obj;

	    (jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);

	    sound_bridge = (jobject)(env)->NewGlobalRef( obj);
	    android_soundClass = (env)->GetObjectClass( sound_bridge);

		android_playSound = (env)->GetStaticMethodID(android_soundClass, "playSound", "(Ljava/lang/String;)V");
	    android_playBackGround = (env)->GetStaticMethodID(android_soundClass, "playBackGround", "(Ljava/lang/String;)V");
	    android_initSound = (env)->GetStaticMethodID(android_soundClass, "initSound", "(Ljava/lang/String;Ljava/lang/String;)V");
	    android_stop = (env)->GetStaticMethodID(android_soundClass, "stop", "()V");
	    android_pauseAll = (env)->GetStaticMethodID(android_soundClass, "pauseAll", "()V");
	    android_resumeAll = (env)->GetStaticMethodID(android_soundClass, "resumeAll", "()V");

		LOGI("SOUND: End C call java init");
	}

	void Java_com_android_jellybee_FacebookSharing_FBSocialBridgeJ2C(JNIEnv * env, jobject thiz, jobject obj)
	{
		social_bridge = obj;

	    (jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);

	    social_bridge = (jobject)(env)->NewGlobalRef( obj);
	    android_facebookClass = (env)->GetObjectClass( social_bridge);

	    android_fbSharing = (env)->GetStaticMethodID(android_facebookClass, "publishFeedDialog", "()I");
		android_makeScreenShot = (env)->GetStaticMethodID(android_facebookClass, "makeScreenShot", "()V");

		LOGI("SOCIAL: End C call java init");
	}

	int	JNI_OnLoad(JavaVM* vm, void* reserved)
	{
	    JNIEnv *env;
	    jVM = vm;

	    LOGI("JNI ONLOAD");

	    if((vm)->GetEnv((void**) &env, JNI_VERSION_1_6) != JNI_OK)
	    {
	        return -1;
	    }

	    return JNI_VERSION_1_6;
	}
};

void J2CBridge::InitSound(const char* id, const char *filePath)
{
	JNIEnv *env;
	(jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);
	jstring jId = env->NewStringUTF(id);
	char temp[MAX_256];
	sprintf(temp, "%s%s", FileManager::GetInstance()->GetRootPath(), filePath);
	jstring path = env->NewStringUTF(temp);
	(env)->CallStaticVoidMethod(android_soundClass, android_initSound, jId, path);  
}

void J2CBridge::PlayBackGround(const char *filePath)
{
	char temp[MAX_256];
	JNIEnv *env;
	(jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);
	sprintf(temp, "%s%s", FileManager::GetInstance()->GetRootPath(), filePath);
	jstring jPath = env->NewStringUTF(temp);
	env->CallStaticVoidMethod(android_soundClass, android_playBackGround, jPath); 
}

void J2CBridge::PlayASound(const char *id)
{
	JNIEnv *env;
	(jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);
	jstring jId = env->NewStringUTF(id);
	env->CallStaticVoidMethod(android_soundClass, android_playSound, jId); 
}

void J2CBridge::PauseAllSound()
{
	JNIEnv *env;
	(jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);
	env->CallStaticVoidMethod(android_soundClass, android_pauseAll);
}

void J2CBridge::ResumeAllSound()
{
	JNIEnv *env;
	(jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);
	env->CallStaticVoidMethod(android_soundClass, android_resumeAll); 
}

void J2CBridge::StopAllSound()
{
	JNIEnv *env;
	(jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);
	env->CallStaticVoidMethod(android_soundClass, android_stop); 
}

// FACEBOOK SHARING
void J2CBridge::FBSharing()
{
	JNIEnv *env;
	(jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);
	env->CallStaticVoidMethod(android_facebookClass, android_fbSharing); 
}

void J2CBridge::MakeScreenShot()
{
	JNIEnv *env;
	(jVM)->GetEnv((void**) &env, JNI_VERSION_1_6);
	env->CallStaticVoidMethod(android_facebookClass, android_makeScreenShot); 
}

#endif

J2CBridge::J2CBridge(void)
{
}

J2CBridge::~J2CBridge(void)
{
}

J2CBridge* J2CBridge::s_instance = NULL;
J2CBridge* J2CBridge::GetInstance()
{
	if (s_instance == NULL)
	{
		s_instance = new J2CBridge();
	}
	return s_instance;
}

void J2CBridge::DestroyInstance()
{
	SAFE_DEL(s_instance);
}