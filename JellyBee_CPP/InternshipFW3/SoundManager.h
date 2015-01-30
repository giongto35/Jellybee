#include "Platforms.h"
#ifdef _WIN32
#pragma comment(lib, "winmm")

#else
#ifdef _ANDROID
#pragma once
#include "J2CBridge.h"
#endif
#endif

#include "Sound.h"

class SoundManager
{
private:
	static SoundManager *s_instance;
	int m_numOfSound;
	Sound **m_sound;

	SoundManager(void);
	~SoundManager(void);

	#ifdef _ANDROID
	J2CBridge *m_j2cBridge;
	#endif

public:
	static SoundManager* GetInstance();
	static void DestroyInstance();
	
	void Init(const char *fileName);
	
	void PlayBackGround(const char *id);
	void PlayASound(const char *id);
	void PauseAll();
	void ResumeAll();
	void Stop();
};
