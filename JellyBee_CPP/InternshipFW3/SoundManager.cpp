#include "SoundManager.h"
#include <stdio.h>
#include "ResourcesManager.h"
#include "FileManager.h"
#include "NameSpace.h"
#include <cstring>

SoundManager* SoundManager::s_instance = NULL;

SoundManager::SoundManager(void)
{
	
	#ifdef _ANDROID
	m_j2cBridge = 0;
	#endif
}

SoundManager::~SoundManager(void)
{
	for (int i = 0; i < m_numOfSound; i++)
	{
		SAFE_DEL(m_sound[i]);
	}

	SAFE_DEL(m_sound);
}

SoundManager* SoundManager::GetInstance()
{
	if (s_instance == NULL)
	{
		s_instance = new SoundManager();
	}
	return s_instance;
}

void SoundManager::DestroyInstance()
{
	SAFE_DEL(s_instance);
}

void SoundManager::Init(const char *fileName)
{
	
	#ifdef _ANDROID
	m_j2cBridge = J2CBridge::GetInstance();
	#endif

	ResourcesManager* resManager = ResourcesManager::GetInstance();

	FILE* f = FileManager::GetInstance()->Open(fileName, "r");

	if (f == NULL)
	{
		LOGE("Can't not open Sound Manager file\n");
		return;
	}

	char id[SIZE_OF_ID];
	char path[256];
	int N;

	// SOUND
	fscanf(f, "#SOUND: %d\n", &N);
	m_numOfSound = N;
	m_sound = new Sound* [m_numOfSound];
	fscanf(f, "%*s %*s\n");
	for(int i = 0; i < m_numOfSound; ++i)
	{
		// id - path
		fscanf(f, "%s %s", id, path);
		m_sound[i] = new Sound(id, path);
	#ifdef _WIN32
		char temp[MAX_256];
		sprintf(temp, "open %s%s alias %s", FileManager::GetInstance()->GetRootPath(), m_sound[i]->m_fileName, m_sound[i]->m_id);
		mciSendString(temp, NULL, 0, NULL);
	#endif

	#ifdef _ANDROID
		m_j2cBridge->InitSound(id, m_sound[i]->m_fileName);
	#endif
	}
}

void SoundManager::PlayBackGround(const char *id)
{
	if (!Setting::isSoundOn)
	{
		return;
	}

	char temp[MAX_256];
	int i;

	for (i = 0; i < m_numOfSound; i++)
	{
		if (strcmp(m_sound[i]->m_id, id) == 0)
		{
			break;
		}
	}

	if (i >= m_numOfSound)
	{
		LOGI("Cant open sound %s\n", id);
		return;
	}
#ifdef _WIN32
	sprintf(temp, "play %s repeat", m_sound[i]->m_id);
	mciSendString(temp, NULL, 0, NULL);
#endif

#ifdef _ANDROID	
	m_j2cBridge->PlayBackGround(m_sound[i]->m_fileName);
#endif
}

void SoundManager::PlayASound(const char *id)
{
	if (!Setting::isSoundOn)
	{
		return;
	}

#ifdef _WIN32
	char temp[MAX_256];
	int i;

	for (i = 0; i < m_numOfSound; i++)
	{
		if (strcmp(m_sound[i]->m_id, id) == 0)
		{
			break;
		}
	}

	if (i >= m_numOfSound)
	{
		LOGI("Cant open sound %s\n", id);
		return;
	}

	char status[SIZE_OF_LINE];
	sprintf(temp, "status %s mode", m_sound[i]->m_id);
	mciSendString(temp, status, SIZE_OF_LINE, NULL);
	
	if (strcmp(status, "playing") != 0)
	{
		sprintf(temp, "stop %s", m_sound[i]->m_id);
		mciSendString(temp, NULL, 0, NULL);

		sprintf(temp, "seek %s to start", m_sound[i]->m_id);
		mciSendString(temp, NULL, 0, NULL);

		sprintf(temp, "play %s", m_sound[i]->m_id);
		mciSendString(temp, NULL, 0, NULL);
	}
#endif

#ifdef _ANDROID
	m_j2cBridge->PlayASound(id);
#endif
}

void SoundManager::PauseAll()
{
#ifdef _ANDROID
	m_j2cBridge->PauseAllSound();
#endif
}

void SoundManager::ResumeAll()
{
#ifdef _ANDROID
	m_j2cBridge->ResumeAllSound();
#endif
}

void SoundManager::Stop()
{
#ifdef _WIN32
	int i;

	for (i = 0; i < m_numOfSound; i++)
	{
		char temp[MAX_256];
		sprintf(temp, "stop %s", m_sound[i]->m_id);
		mciSendString(temp, NULL, NULL, NULL );
	}

#endif

#ifdef _ANDROID
	m_j2cBridge->StopAllSound();
#endif
}
