#pragma once
#include "state.h"
#include "Platforms.h"
#include "ColorDataConverter.h"
#include "item.h"
#include "MainCharacter.h"
#include "JoyPad.h"
#include "Number.h"
#include "EffectManager.h"
#include "ObjectsManager.h"
#include "Button.h"

class InGame :
	public State
{
	static InGame *s_Instance;

	Button *m_btPause;
	Joypad *m_joypad;
	Number *m_number;
	Object *m_scoreBoard;

 	//EffectManager* effectManager;
	GLuint m_fboId;
	GLuint m_depthTextureId, m_colorTextureId;
	ColorDataConverter m_unionColor;

	unsigned char m_pixelData[4];

	Item **m_items;
	int m_numOfItems;
	MainCharacter *m_mainCharacter;
	ObjectsManager *m_objManager;

	Object *m_heartIcon;
	Object *m_velo;
	Object *m_magnet;
	Object *m_clock;
	Object *m_fogVision;

	double m_timePool;
	int m_combo;

	InGame(void);
	~InGame(void);

	// Built in LevelManager
	char* m_levelsManagerPath;

	int m_nLevels;

	char** m_levelID;
	char** m_levelPath;
	struct Challenge
	{
		float speed;
	} 
	* m_challenge, m_currentChallenge;
	// ---

public:
	static InGame* GetInstance();
	static void DestroyInstance();

	virtual void Init(const char* id, const char* path);
	virtual void Start();
	virtual void Update();
	virtual void Draw();
	virtual void Destroy();
	void DrawAll();

	// Built in LevelManager
	char* GetCurrentLevelScenePath();
	char* GetLevelScenePathByID(char* levelID);
	char* GetNextLevelScenePath();
	int GetLevelManagementIndex(char* path);
	void SetCurrentLevelScenePath(char* path);
	// ---

};
