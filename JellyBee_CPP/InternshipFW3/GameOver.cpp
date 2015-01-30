#include "GameOver.h"
#include "InGame.h"
#include "Button.h"
#include "Object.h"
#include "StatesManager.h"
#include "ScreenResolution.h"
#include "ResourcesManager.h"
#include "FileManager.h"
#include <cstring>
#include <stdio.h>
#include "String.h"
#include "AppDataManager.h"
#include "SoundManager.h"
#include "J2CBridge.h"

//---------------------------------------------------
/// Constructor
//---------------------------------------------------
GameOver::GameOver(void)
{
	m_isVisible = false;

	m_button = 0;
	m_numOfButton = 0;

	m_ground = 0; 
	m_inputManager = InputManager::GetInstance();
}

//---------------------------------------------------
/// INIT
//---------------------------------------------------
void GameOver::Init()
{
	ResourcesManager *resManager = ResourcesManager::GetInstance();

	FILE* f = FileManager::GetInstance()->Open("Managers/GameOver.txt", "r");

	if (f == NULL)
	{
		LOGE("Can't not open GameOver file\n");
		return;
	}

	char id[SIZE_OF_ID];
	char model[SIZE_OF_ID];	
	char texture[SIZE_OF_ID];
	char shader[SIZE_OF_ID];
	char texture_uv[SIZE_OF_ID];
	int visible;
	Vector3 pos, rot, sca;
	int N;
	
	// OBJECT
	fscanf(f, "#Object: \n");
	SKIP_ONE_LINE(f);
	
	// id  - model - texture - shader
	fscanf(f, "%s %s %s %s", id, model, texture, shader);
	// position - rotation - scale
	fscanf(f, "%f, %f, %f", &pos.x, &pos.y, &pos.z);
	fscanf(f, "%f, %f, %f", &rot.x, &rot.y, &rot.z);
	fscanf(f, "%f, %f, %f", &sca.x, &sca.y, &sca.z);
	// textureUV
	fscanf(f, "%s\n", texture_uv);
	m_ground = new Object();
	if(strcmp(texture_uv, "NULL") == 0)
	{
		m_ground->Init(id, pos, rot, sca, resManager->GetTextureById(texture), resManager->GetModelById(model), resManager->GetShaderById(shader), NULL);
	}
	else
	{
		m_ground->Init(id, pos, rot, sca, resManager->GetTextureById(texture), resManager->GetModelById(model), resManager->GetShaderById(shader), texture_uv);
	}

	// BUTTON
	fscanf(f, "#Button: %d\n", &N);
	if (N > 0)
	{
		m_numOfButton = N;
		m_button = new Button*[m_numOfButton];

		SKIP_ONE_LINE(f);
		for(int i = 0; i < m_numOfButton; ++i)
		{
			// id  - model - texture - shader
			fscanf(f, "%s %s %s %s", id, model, texture, shader);
			// position - rotation - scale
			fscanf(f, "%f, %f, %f", &pos.x, &pos.y, &pos.z);
			fscanf(f, "%f, %f, %f", &rot.x, &rot.y, &rot.z);
			fscanf(f, "%f, %f, %f", &sca.x, &sca.y, &sca.z);
			// textureUV
			fscanf(f, "%s", texture_uv);
			m_button[i] = new Button();
			if(strcmp(texture_uv, "NULL") == 0)
			{
				m_button[i]->Init(id, pos, rot, sca, resManager->GetTextureById(texture), resManager->GetModelById(model), resManager->GetShaderById(shader), NULL);
			}
			else
			{
				m_button[i]->Init(id, pos, rot, sca, resManager->GetTextureById(texture), resManager->GetModelById(model), resManager->GetShaderById(shader), texture_uv);
			}

			fscanf(f, "%d\n", &visible);
			if (visible)
			{
				m_button[i]->m_isVisible = true;
			}
			else
			{
				m_button[i]->m_isVisible = false;
			}
			
		}
	}

	fclose(f);
}

//---------------------------------------------------
/// UPDATE
//---------------------------------------------------
void GameOver::Update()
{
	for (int i = 0; i < m_numOfButton; ++i)
	{
		m_button[i]->Update();
	}

	// btn Restart Level
	if (m_button[0]->IsClicked())
	{
		char str[SIZE_OF_STRING];
		strcpy(str, InGame::GetInstance()->m_sceneFilePath);
		LOGI("Restart Level %s\n", str);
		MainCharacter::GetInstance()->SetBlood(3);
		MainCharacter::GetInstance()->SetScore(0); // score of the previous level
		InGame::GetInstance()->SetCurrentLevelScenePath(str);
		InGame::GetInstance()->Start();
		InGame::GetInstance()->m_status = ON_RESUME;
	}
	// btn Back to main menu
	else if (m_button[1]->IsClicked())
	{
		SoundManager::GetInstance()->Stop();

		m_isVisible = false;
		InGame::GetInstance()->m_status = GAME_OVER;
		SoundManager::GetInstance()->PlayASound("gameover");
		StatesManager::GetInstance()->SwitchState(StatesManager::GetInstance()->GetStateById("MainMenu"));
	}
	// Share to Facebook
	else if (m_button[2]->IsClicked())
	{
	#ifdef _ANDROID
		J2CBridge::GetInstance()->MakeScreenShot();
	#endif
		//m_isVisible = false;
		LOGI("Share to FB\n");
	#ifdef _ANDROID
		J2CBridge::GetInstance()->FBSharing();
	#endif
	}
}

//---------------------------------------------------
/// DRAW
//---------------------------------------------------
void GameOver::Draw()
{
	Matrix gameOverMat;
	m_ground->Draw(gameOverMat);
	for (int i = 0; i < m_numOfButton; i++)
	{
		m_button[i]->Draw(gameOverMat);
	}

	int s = MainCharacter::GetInstance()->GetScore();
	char score[MAX_256];
	sprintf(score, "Score: %d", s);
	String::GetInstance()->DrawString(score, Vector3(-0.3f, 0.2f, 1.0f), Vector3(1.5f, 1.5f, 1.5f), Vector3(1.0f, 0.5f, 0.0f));

	m_bestScore = AppDataManager::GetInstance()->GetBestScore();
	if (s > m_bestScore)
	{
		m_bestScore = s;
		AppDataManager::GetInstance()->SetBestScore(m_bestScore);
	}
	sprintf(score, "Best: %d", m_bestScore);
	String::GetInstance()->DrawString(score, Vector3(-0.3f, -0.0f, 1.0f), Vector3(1.5f, 1.5f, 1.5f), Vector3(0.0f, 0.65f, 0.086f));
}

//---------------------------------------------------
/// Get Instance
//---------------------------------------------------
GameOver* GameOver::s_Instance = 0;
GameOver* GameOver::GetInstance()
{
	if (!s_Instance)
	{
		s_Instance = new GameOver();
	}
	return s_Instance;
}

//---------------------------------------------------
/// Destroy Instance
//---------------------------------------------------
void GameOver::DestroyInstance() 
{
	if (s_Instance)
	{
		delete s_Instance;
		s_Instance = NULL;
	}
}

//---------------------------------------------------
/// Destructor
//---------------------------------------------------
GameOver::~GameOver(void)
{
	SAFE_DEL(m_ground);
	for (int i = 0; i < m_numOfButton; i++)
	{
		SAFE_DEL(m_button[i]);
	}
	SAFE_DEL_ARRAY(m_button);
}

bool GameOver::IsVisible()
{
	return m_isVisible;
}

void GameOver::SetVisible(bool visible)
{
	m_isVisible = visible;
}