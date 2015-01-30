#pragma once
#include "Platforms.h"

class Sound
{
public:
	char m_id[SIZE_OF_ID];
	char *m_fileName;

public:
	Sound(const char *id, const char *fileName);
	~Sound(void);
};
