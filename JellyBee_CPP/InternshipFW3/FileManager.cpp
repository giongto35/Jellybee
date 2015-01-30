#include "FileManager.h"
#include <cstring>

//---------------------------------------------------
/// Constructor
//---------------------------------------------------
FileManager::FileManager(void)
{
	m_rootPath = NULL;
}

//---------------------------------------------------
/// Init
//---------------------------------------------------
void FileManager::Init(const char * path)
{
	m_rootPath = new char[strlen(path) + 1];
	strcpy(m_rootPath, path);
}

//---------------------------------------------------
/// Open file
//---------------------------------------------------
FILE* FileManager::Open(const char * path, const char * mode)
{
	// get final file path
	char* finalPath = new char[strlen(m_rootPath) + strlen(path) + 1];
	strcpy(finalPath, m_rootPath);
	strcat(finalPath, path);

	FILE* f = fopen(finalPath, mode);
	if(!f)
	{
		LOGE("ERROR: cannot open file %s\n", finalPath);
	}
	delete finalPath;

	return f;
}

//---------------------------------------------------
/// Get Instance
//---------------------------------------------------
FileManager* FileManager::s_Instance = 0;
FileManager* FileManager::GetInstance()
{
	if (!s_Instance)
	{
		s_Instance = new FileManager();
	}
	return s_Instance;
}

//---------------------------------------------------
/// Destroy Instance
//---------------------------------------------------
void FileManager::DestroyInstance() 
{
	if (s_Instance )
	{
		delete s_Instance;
		s_Instance = NULL;
	}
}

//---------------------------------------------------
/// Destructor
//---------------------------------------------------
FileManager::~FileManager(void)
{
	SAFE_DEL(m_rootPath);
}

const char* FileManager::GetRootPath()
{
	return m_rootPath;
}