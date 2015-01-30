#pragma once
#include "Platforms.h"
#include <cstdio>

class FileManager
{
private:
	static FileManager* s_Instance;
	char* m_rootPath;

private:
	FileManager(void);
	~FileManager(void);
public:
	void Init(const char * path);
	FILE* Open(const char * path, const char * mode);
	static FileManager* GetInstance();
	static void DestroyInstance();

	const char* GetRootPath();
};
