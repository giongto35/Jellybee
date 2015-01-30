#include "AppDataManager.h"
#include "FileManager.h"
#include <cstring>

//---------------------------------------------------------------------------------------------------
/// SetBestScore
//---------------------------------------------------------------------------------------------------
void AppDataManager::SetBestScore(int bestScore)
{
	ConfigData data;
	data.i32 = bestScore;

	Set(ADM_SCORE, ADM_SCORE_BEST, ADM_TYPE_INT, data);
}

//---------------------------------------------------------------------------------------------------
/// GetBestScore
//---------------------------------------------------------------------------------------------------
int AppDataManager::GetBestScore()
{
	ConfigData data = Get(ADM_SCORE, ADM_SCORE_BEST);
	return data.i32;
}

//---------------------------------------------------------------------------------------------------
/// ToggleSound
//---------------------------------------------------------------------------------------------------
void AppDataManager::ToggleSound(bool on)
{
	ConfigData data;
	data.i32 = on? 1: 0;

	Set(ADM_SCORE, ADM_SCORE_BEST, ADM_TYPE_INT, data);
}

//---------------------------------------------------------------------------------------------------
/// IsSoundTurnedOn
//---------------------------------------------------------------------------------------------------
bool AppDataManager::IsSoundTurnedOn()
{
	ConfigData data = Get(ADM_SCORE, ADM_SCORE_BEST);
	return (data.i32 == 1)? true: false;
}

//---------------------------------------------------------------------------------------------------
/// Get
/// EXP: ConfigData data = AppDataManager::GetInstace()->Get(ADM_SCORE, ADM_SCORE_BEST);
//---------------------------------------------------------------------------------------------------
ConfigData AppDataManager::Get(const char* type, const char* subType)
{
	ConfigData data;

	char* path;
	FILE* f = FileManager::GetInstance()->Open(
		path = ConstructPath(type, subType), "r"
	);

	char dataType[ADM_DATA_TYPENAME_LEN + 1];
	fscanf(f, "%s", dataType);

	if (strcmp(dataType, ADM_TYPE_INT) == 0)
	{
		int iData;
		fscanf(f, "%d", &iData);
		data.i32 = iData;
	}
	else // ADM_STR
	{
		char sData[ADM_DATA_STRTYPE_LEN + 1];
		fscanf(f, "%s", sData);
		strcpy(data.str, sData);
	}

	fclose(f);
	SAFE_DEL(path);

	return data;
}

//---------------------------------------------------------------------------------------------------
/// Set
/// EXP: ConfigData data; strcpy(data.str, "on");
/// AppDataManager::GetInstace()->Get(ADM_SETTING, ADM_SETTING_SOUND, ADM_TYPE_STR, data);
//---------------------------------------------------------------------------------------------------
void AppDataManager::Set(const char* type, const char* subType, const char* dataType, ConfigData data)
{
	char* path;
	FILE* f = FileManager::GetInstance()->Open(
		path = ConstructPath(type, subType), "w"
	);

	if (strcmp(dataType, ADM_TYPE_INT) == 0)
	{
		fprintf(f, "int\n%d", data.i32);
	}
	else // ADM_STR
	{
		fprintf(f, "str\n%s", data.str);
	}

	fclose(f);
	SAFE_DEL(path);
}

//---------------------------------------------------------------------------------------------------
/// ConstructPath
//---------------------------------------------------------------------------------------------------
char* AppDataManager::ConstructPath(const char* type, const char* subType)
{
	char* path = new char[SIZE_OF_STRING];
	sprintf(path, "%s/%s-%s.%s", ADM_ROOT, type, subType, ADM_EXT);
	return path;
}

//---------------------------------------------------------------------------------------------------
/// GetInstance
//---------------------------------------------------------------------------------------------------
AppDataManager* AppDataManager::s_Instance = 0;
AppDataManager* AppDataManager::GetInstance()
{
	if (s_Instance == 0)
	{
		s_Instance = new AppDataManager;
	}
	return s_Instance;
}

//---------------------------------------------------------------------------------------------------
/// DestroyInstance
//---------------------------------------------------------------------------------------------------
void AppDataManager::DestroyInstance()
{
	SAFE_DEL(s_Instance);
}

//---------------------------------------------------------------------------------------------------
/// Constructor
//---------------------------------------------------------------------------------------------------
AppDataManager::AppDataManager(void)
{

}

//---------------------------------------------------------------------------------------------------
/// Destructor
//---------------------------------------------------------------------------------------------------
AppDataManager::~AppDataManager(void)
{

}

