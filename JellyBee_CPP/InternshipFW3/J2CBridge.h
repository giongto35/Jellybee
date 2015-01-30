#pragma once
#include "Platforms.h"

#ifdef _ANDROID
#include <jni.h>		
#endif

class J2CBridge
{
public:
	static J2CBridge *s_instance;

private:
	J2CBridge(void);
	~J2CBridge(void);
public:
	static J2CBridge* GetInstance();
	static void DestroyInstance();

	void InitSound(const char* id, const char *filePath);
	void PlayBackGround(const char *id);
	void PlayASound(const char *filePath);
	void PauseAllSound();
	void ResumeAllSound();
	void StopAllSound();

	void FBSharing();
	void MakeScreenShot();
};
