#pragma once

#define ADM_ROOT "AppData"
#define ADM_EXT "txt"

#define ADM_SCORE "score"
#define ADM_SETTING "setting"

#define ADM_SCORE_BEST "best"
#define ADM_SETTING_SOUND "sound"

#define ADM_DATA_TYPENAME_LEN 3
#define ADM_DATA_STRTYPE_LEN 10

#define ADM_TYPE_INT "int"
#define ADM_TYPE_STR "str"

union ConfigData
{
	char str[ADM_DATA_STRTYPE_LEN + 1];
	int i32;
};

// SINGLETON CLASS
class AppDataManager
{
public:
	// Singleton handlings
	static AppDataManager* GetInstance();
	static void DestroyInstance();

	// Methods
	ConfigData Get(const char* type, const char* subType);
	void Set(const char* type, const char* subType, const char* dataType, ConfigData data);

	// API
	// Best score
	void SetBestScore(int bestScore);
	int GetBestScore();

	// Sound
	void ToggleSound(bool on);
	bool IsSoundTurnedOn();

private:
	// Singleton instace
	static AppDataManager* s_Instance;

	char* ConstructPath(const char* type, const char* subType);

	// Constructor & Destructor
	AppDataManager(void);
	~AppDataManager(void);
};
