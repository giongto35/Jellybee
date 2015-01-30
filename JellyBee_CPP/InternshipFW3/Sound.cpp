#include "Sound.h"
#include <cstring>

Sound::Sound(const char *id, const char *fileName)
{
	strcpy(m_id, id);
	m_fileName = new char [strlen(fileName) + 1];
	strcpy(m_fileName, fileName);
}

Sound::~Sound(void)
{
	SAFE_DEL(m_fileName);
}
