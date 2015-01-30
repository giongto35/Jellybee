#include "InGame.h"
#include "Timer.h"
#include "ScreenResolution.h"
#include "Framebuffer.h"
#include "Object.h"
#include "ResourcesManager.h"
#include "FileManager.h"
#include "Collision.h"
#include "Map.h"
#include "ScenesManager.h"
#include <stdlib.h>
#include "String.h"
#include "PauseMenu.h"
#include "GameOver.h"
#include "SoundManager.h"
#include "NameSpace.h"
#include <cmath>
#include <sstream>

InGame* InGame::s_Instance = NULL;
int endIndex = 1;

//---------------------------------------------------------------------------------------------------
/// Start
//---------------------------------------------------------------------------------------------------
void InGame::Start()
{
	if (m_status == GAME_OVER)
	{
		Destroy();
		Init("InGame", "Managers/Levels.txt");
		MainCharacter::GetInstance()->SetBlood(3);
	}
	srand(time(0));

	State::Start();

	///
	ResourcesManager* resManager = ResourcesManager::GetInstance();

	Object* ground = m_sceneManager->GetObjectById("ground");
	Texture** textures = new Texture*[4];
	textures[0] = ResourcesManager::GetInstance()->GetTextureById("blendMap");
	textures[1] = ResourcesManager::GetInstance()->GetTextureById("Dirt");
	textures[2] = ResourcesManager::GetInstance()->GetTextureById("grass");
	textures[3] = ResourcesManager::GetInstance()->GetTextureById("rock");
	
	ground->Init( ground->GetId(), 
			    ground->GetPosition(), 
				ground->GetRotation(), 
				ground->GetScale(),
				4, 
				textures, 
				ground->GetModel(), 
				ground->GetShader());
	SAFE_DEL_ARRAY(textures);

	// z = 0.2 ~ 0.7
	if (m_sceneManager->m_map)
	{
		m_joypad = new Joypad(Vector2(0.75f, -0.65f), 0.12f, 0.09f, 5.0f);
		m_number = Number::GetInstance();
		m_number->Init();
		m_number->SetPosition(Vector3(0.91f, 0.810f, 0.0f));
		m_scoreBoard = new Object();
		m_scoreBoard->Init("scores", 
						 Vector3(0.75f, 0.825f, 0.0f), 
						 Vector3(0.0f, 0.0f, 0.0f), 
						 Vector3(0.22625f, 0.1167f, 1.0f), 
						 resManager->GetTextureById("bigTex"), 
						 resManager->GetModelById("quad"), 
						 resManager->GetShaderById("staticObj"),
						 "score.tga");
		

		// Life hearts
		m_heartIcon = new Object();
		m_heartIcon->Init(	 
				"hearts", 
				Vector3(0.88f, 0.4f, 0.0f), 
				Vector3(0.0f, 0.0f, 0.0f), 
				Vector3(0.064f, 0.1f, 0.0f), 
				resManager->GetTextureById("bigTex"), 
				resManager->GetModelById("quad"), 
				resManager->GetShaderById("staticObj"),
				"heart.tga");

		m_velo = new Object();
		m_velo->Init(	 
				"velocity", 
				Vector3(0.2f, 0.85f, 0.0f), 
				Vector3(0.0f, 0.0f, 0.0f), 
				Vector3(0.03f, 0.05f, 0.0f), 
				//Vector3(1.0f, 1.0f, 1.0f), 
				resManager->GetTextureById("velocity"), 
				resManager->GetModelById("static_quad"), 
				resManager->GetShaderById("staticObj"),
				NULL);

		m_magnet = new Object();
		m_magnet->Init(	 
				"magnet", 
				Vector3(-0.2f, 0.85f, 0.0f), 
				Vector3(0.0f, 0.0f, 0.0f), 
				Vector3(0.03f, 0.05f, 0.0f), 
				//Vector3(1.0f, 1.0f, 1.0f), 
				resManager->GetTextureById("magnet"), 
				resManager->GetModelById("static_quad"), 
				resManager->GetShaderById("staticObj"),
				NULL);

		m_clock = new Object();
		m_clock->Init(	 
				"clock", 
				Vector3(-0.6f, 0.85f, 0.0f), 
				Vector3(0.0f, 0.0f, 0.0f), 
				Vector3(0.03f, 0.05f, 0.0f), 
				//Vector3(1.0f, 1.0f, 1.0f), 
				resManager->GetTextureById("clock"), 
				resManager->GetModelById("static_quad"), 
				resManager->GetShaderById("staticObj"),
				NULL);

		m_mainCharacter = MainCharacter::GetInstance();
		m_sceneCamera->SetSpeed(m_currentChallenge.speed);
		m_mainCharacter->SetMagnet(0.0f);
		m_mainCharacter->m_sceneCamera = m_sceneCamera;

		m_numOfItems = m_sceneManager->m_map->m_nItems;
		m_items = new Item* [m_numOfItems];

		m_fogVision = new Object();
		m_fogVision->Init(	 
				"fog", 
				m_mainCharacter->GetPosition(), 
				Vector3(0.0f, 0.0f, 0.0f), 
				Vector3(0.1f, 0.1f, 0.1f), 
				//Vector3(1.0f, 1.0f, 1.0f), 
				resManager->GetTextureById("fog"), 
				resManager->GetModelById("bila"), 
				resManager->GetShaderById("objShad"),
				NULL);

		// Set start position
		Vector2 sPos = m_sceneManager->m_map->m_start;
		m_sceneCamera->SetPosition(	
			Vector3(sPos.y - m_sceneManager->m_map->GetWidth()/2.0f, 
					0.3f, 
					-sPos.x) 
			* MAPFACTOR
		);

		int counter = 0;
		for (int i = 0; i < m_sceneManager->m_map->GetHeight(); i++)
		for (int j = 0; j < m_sceneManager->m_map->GetWidth(); j++)
		{

			float randY = (rand() % 80 + 20.0f) / 100;
			if (m_sceneManager->m_map->GetPoint(i,j) == HONEY)
			{
				float randY = (rand() % 80 + 20.0f) / 100;
				m_items[counter] = new Item();
				m_items[counter]->Init("Honey", 
										Vector3((float)(j - m_sceneManager->m_map->GetWidth() / 2.0f), randY , (float)-i) * MAPFACTOR, 
										Vector3(0, 0, 0), 
										Vector3(0.35f, 0.35f, 0.35f), 
										resManager->GetTextureById("bigTex"), 
										resManager->GetModelById("honey"), 
										resManager->GetShaderById("objShad"),
										HONEY, NULL);
				counter++;
			}

			if (m_sceneManager->m_map->GetPoint(i,j) == HONEY_FAKE)
			{
				float randY = (rand() % 80 + 20.0f) / 100;
				m_items[counter] = new Item();
				m_items[counter]->Init("HoneyFake", 
										Vector3((j - m_sceneManager->m_map->GetWidth() / 2.0f), randY , -i) * MAPFACTOR, 
										Vector3(0, 0, 0), 
										Vector3(0.35f, 0.35f, 0.35f), 
										resManager->GetTextureById("bigTex"), 
										resManager->GetModelById("honey_fake"), 
										resManager->GetShaderById("objShad"),
										HONEY_FAKE, NULL);
				counter++;
			}

			if (m_sceneManager->m_map->GetPoint(i,j) == HEART)
			{
				float randY = (rand() % 80 + 20.0f) / 100;
				m_items[counter] = new Item();
				m_items[counter]->Init("Heart", 
										Vector3((j - m_sceneManager->m_map->GetWidth() / 2.0f), randY , -i) * MAPFACTOR, 
										Vector3(0, 0, 0), 
										Vector3(0.2f, 0.2f, 0.2f),  
										resManager->GetTextureById("bigTex"), 
										resManager->GetModelById("heart"), 
										resManager->GetShaderById("objShad"),
										HEART, NULL);
				counter++;
			}

			if (m_sceneManager->m_map->GetPoint(i,j) == MOUNTAIN_DEW)
			{
				float randY = (rand() % 80 + 20.0f) / 100;
				m_items[counter] = new Item();
				m_items[counter]->Init("MD", 
										Vector3((j - m_sceneManager->m_map->GetWidth() / 2.0f), randY , -i) * MAPFACTOR, 
										Vector3(0, 0, 0), 
										Vector3(0.15f, 0.1f, 0.15f),  
										resManager->GetTextureById("bigTex"), 
										resManager->GetModelById("moutain"), 
										resManager->GetShaderById("objShad"),
										MOUNTAIN_DEW, NULL);
				counter++;
			}

			if (m_sceneManager->m_map->GetPoint(i,j) == SPIDER_WEB)
			{
				float randY = (rand() % 80 + 20.0f) / 100;
				m_items[counter] = new Item();
				m_items[counter]->Init("SW", 
										Vector3((j - m_sceneManager->m_map->GetWidth() / 2.0f), randY , -i) * MAPFACTOR, 
										Vector3(0, 0, 0), 
										Vector3(0.35f, 0.35f, 0.35f),
										resManager->GetTextureById("bigTex"), 
										resManager->GetModelById("honey_black"), 
										resManager->GetShaderById("objShad"),
										SPIDER_WEB, NULL);
				counter++;
			}

			if (m_sceneManager->m_map->GetPoint(i,j) == END)
			{
				float randY = 1.0f;
				endIndex = counter;
				m_items[counter] = new Item();
				m_items[counter]->Init("End", 
										Vector3((j - m_sceneManager->m_map->GetWidth() / 2.0f), randY , -i) * MAPFACTOR, 
										Vector3(0, 0, 0), 
										Vector3(0.2f, 0.2f, 0.2f),  
										resManager->GetTextureById("bigTex"), 
										resManager->GetModelById("hive"), 
										resManager->GetShaderById("objShad"),
										END, NULL);
				counter++;
			}

			if (
					(m_sceneManager->m_map->GetPoint(i,j) >= 100)
				&&  
					(m_sceneManager->m_map->GetPoint(i,j) <= 200)
			   )
			{
				float randY = 1.5f;
				m_items[counter] = new Item();
				m_items[counter]->Init("Telegate", 
										Vector3((j - m_sceneManager->m_map->GetWidth() / 2.0f), randY , -i) * MAPFACTOR, 
										Vector3(0, 0, 0), 
										Vector3(0.2f, 0.2f, 0.2f),  
										resManager->GetTextureById("bigTex"), 
										resManager->GetModelById("teleport"), 
										resManager->GetShaderById("objShad"),
										m_sceneManager->m_map->GetPoint(i,j),
										NULL);
				counter++;
			}
		}
	}

	
	m_objManager = ObjectsManager::GetInstance();
	m_objManager->Init(0, 0);

	//effect
	//FILE* f = FileManager::GetInstance()->Open("Managers/EffectManager.txt", "r");
	//EffectManager::GetInstance()->Load(f);
	//fclose(f);

	//pauseBT
	m_btPause = (Button*) m_sceneManager->GetObjectById("btnPause");
	
	// SOUND
	SoundManager::GetInstance()->PlayBackGround("doreamon");	

	/*emitter1 = new Emitter;
	Model *model = ResourcesManager::GetInstance()->GetModelById("particle");
	Shader *shader = ResourcesManager::GetInstance()->GetShaderById("Particle");
	Texture *texture = ResourcesManager::GetInstance()->GetTextureById("heart");*/

	/*emitter1->InitResource(model, shader, texture);
	emitter1->InitParameter(50, 
		m_sceneCamera->GetPosition(), 
							Vector3(0.02f, 0.02f, 0.02f),
							Vector3(0.05f, 0.05f, 0.05f), 
							Vector3(0.0f, 0.0f, 0.0f),
							Vector3(0.5f, 0.5f, 0.5f),
							PS_FIREWORK, 
							5);*/
}

//---------------------------------------------------------------------------------------------------
/// (Non-member) DistanceV3
//---------------------------------------------------------------------------------------------------
float DistanceV3(Vector3 u, Vector3 v)
{
	return sqrt((u.x - v.x)*(u.x - v.x) + (u.y - v.y)*(u.y - v.y) + (u.z - v.z)*(u.z - v.z));
}

// Main character
#define INGAME_CONFIG_MAGNET_MAX_RADIUS 0.5f
#define INGAME_CONFIG_SPEED_MIN 0.5f
#define INGAME_CONFIG_SPEED_MAX 3.0f

// Items
#define INGAME_CONFIG_MOUNTAINDEW_SPEED 0.15f
#define INGAME_CONFIG_HONEYFAKE_SPEED -0.15f

#define INGAME_CONFIG_MOUNTAINDEW_MAGNET 0.2f

#define INGAME_CONFIG_BOOST_TIME 5.0f // sec

#define INGAME_CONFIG_HONEY_SCORE 1
#define INGAME_CONFIG_HONEYFAKE_SCORE -10

//---------------------------------------------------------------------------------------------------
/// Update 
//---------------------------------------------------------------------------------------------------

// Boost & deboost speed handling vars
// Additional boost for both time & speed are also added
static float boostTime = 0.0f;
static float totalBoostTime = 0.0f;

static float boostSpeed = 0.0f;
static float additionalBoostSpeed = 0.0f;
static float boostMagnet = 0.0f;
static float additionalBoostMagnet = 0.0f;

void InGame::Update()
{
	m_items[endIndex]->m_rotation.y += 0.1f;
	m_items[endIndex]->SetRotation(m_items[endIndex]->m_rotation);

	m_sceneManager->Update();

	// GAME OVER
	if (m_inputManager->IsKeyDown(13))
	{
		m_status = GAME_OVER;
		SoundManager::GetInstance()->PlayASound("gameover");
	}

	// PAUSE GAME
	if (    m_inputManager->IsKeyDown(27)
		||  m_btPause->IsClicked()
		||  Setting::isBackButtonPressed)
	{
		Setting::isBackButtonPressed = false;
		SoundManager::GetInstance()->PlayASound("pause");
		m_status = ON_PAUSE;
		SoundManager::GetInstance()->PauseAll();
	}

	// JELLY BEE
	m_mainCharacter->Update();

	Vector4 moveL = Vector4(0, 0, -0.5, 1);
	Vector4 MainCharacterPosition;
	MainCharacterPosition = moveL * m_sceneCamera->CalculateWorldMatrix();

	// GAME SCRIPT
	// Item collision
	for(int i = 0; i < m_numOfItems; ++i)
	{
		if (m_items[i])
		{
			// Magnet
			float magnetSpeed = 1.0/2.0f;
			Vector3 itemPos = m_items[i]->GetPosition();
			if (	(m_items[i]->m_type == HONEY)
				&&
					DistanceV3(itemPos, MainCharacterPosition.xyz) < m_mainCharacter->GetMagnet()
			   )
			{
				m_items[i]->SetPosition(
					itemPos 
					+ 
					Vector3(
						(MainCharacterPosition.x - itemPos.x), 
						(MainCharacterPosition.y - itemPos.y), 
						(MainCharacterPosition.z - itemPos.z)
					) * magnetSpeed
				);
			}

			// Check collision
			if (IsHit(m_mainCharacter, m_items[i]))
			{
				// HEART: +1 blood
				if (m_items[i]->m_type == HEART)
				{
					m_mainCharacter->SetBlood(m_mainCharacter->GetBlood()+1);
					SAFE_DEL(m_items[i]);
					SoundManager::GetInstance()->PlayASound("heart");
				}

				// HONEY: +1 point
				else if (m_items[i]->m_type == HONEY)
				{
					m_mainCharacter->SetScore(m_mainCharacter->GetScore() + m_combo);

					m_mainCharacter->SetScore(
						m_mainCharacter->GetScore() + INGAME_CONFIG_HONEY_SCORE + m_combo
					);

					m_combo++;
					m_timePool = 0.5;

					if (m_mainCharacter->GetScore() > 99999)
					{
						m_mainCharacter->SetBlood(m_mainCharacter->GetBlood()+1);
						m_mainCharacter->SetScore(0);
					}
					SAFE_DEL(m_items[i]);	
					SoundManager::GetInstance()->PlayASound("honey");
				}

				// HONEY FAKE: -10 points, -0.2 speed (default 1.0) in 5s
				else if (m_items[i]->m_type == HONEY_FAKE)
				{
					m_mainCharacter->SetScore(
						m_mainCharacter->GetScore() + INGAME_CONFIG_HONEYFAKE_SCORE
					);

					SAFE_DEL(m_items[i]);
					SoundManager::GetInstance()->PlayASound("honey_fake");

					boostSpeed += INGAME_CONFIG_HONEYFAKE_SPEED;
					totalBoostTime += INGAME_CONFIG_BOOST_TIME;
				}

				// MOUNTAIN DEW: +0.2 speed (default 1.0), +0.15 magnet in 5s
				else if (m_items[i]->m_type == MOUNTAIN_DEW)
				{
					SAFE_DEL(m_items[i]);
					SoundManager::GetInstance()->PlayASound("moutain_dew");

					boostMagnet += INGAME_CONFIG_MOUNTAINDEW_MAGNET;
					boostSpeed += INGAME_CONFIG_MOUNTAINDEW_SPEED;
					totalBoostTime += INGAME_CONFIG_BOOST_TIME;
				}

				// SPIDER WEB: -1 blood
				else if (m_items[i]->m_type == SPIDER_WEB)
				{
					m_mainCharacter->SetBlood(m_mainCharacter->GetBlood()-1);
					SAFE_DEL(m_items[i]);
					SoundManager::GetInstance()->PlayASound("honey_black");
				}

				// END: finish map, go on next
				else if (m_items[i]->m_type == END)
				{
					SoundManager::GetInstance()->PlayASound("end");
					SetCurrentLevelScenePath(
						GetNextLevelScenePath()
					);
					Start();
				}
			}
			
		}
	}

	m_timePool -= Timer::FrameTime();
	if (m_timePool < 0)
	{
		m_timePool = 0.5;
		m_combo = 0;
	}

	// Boost & deboost speed handling
	if (totalBoostTime > 0.0f)
	{
		//LOGI("%.2f %.2f %.2f %.2f\n", boostTime, totalBoostTime, m_sceneCamera->m_speed, m_mainCharacter->GetMagnet());
		// Handle additional boost
		if (boostTime == 0.0f || additionalBoostSpeed != boostSpeed)
		{
			float speed = m_sceneCamera->m_speed - additionalBoostSpeed + boostSpeed;

			if (speed < INGAME_CONFIG_SPEED_MIN)
			{
				speed = INGAME_CONFIG_SPEED_MIN;
			}

			if (speed > INGAME_CONFIG_SPEED_MAX)
			{
				speed = INGAME_CONFIG_SPEED_MAX;
			}

			m_sceneCamera->SetSpeed(
				speed
			);

			m_mainCharacter->SetMagnet(
				min(
					INGAME_CONFIG_MAGNET_MAX_RADIUS, 
					m_mainCharacter->GetMagnet() - additionalBoostMagnet + boostMagnet
				)
			);
					
			additionalBoostSpeed = boostSpeed;
			additionalBoostMagnet = boostMagnet;
		}

		// Count valid boost time
		boostTime += Timer::FrameTime();

		// Def
		if (boostTime >= totalBoostTime)
		{
			m_sceneCamera->SetSpeed(m_sceneCamera->m_speed + (-boostSpeed));
			m_mainCharacter->SetMagnet(0.0f);
			boostTime = 0.0f;
			totalBoostTime = 0.0f;
			boostSpeed = 0.0f;
			boostMagnet = 0.0f;
			additionalBoostSpeed = 0.0f;
			additionalBoostMagnet = 0.0f;
		}
	}

	MapType n = END;
	n = m_sceneManager->m_map->GetFromFloatPoint(
		MainCharacterPosition.z, 
		MainCharacterPosition.x
	);

	if (n == TREE || n == BLOCK)
	{
		// stop
		SoundManager::GetInstance()->PlayASound("block");
		//m_sceneCamera->MoveCamera(FORWARD);
	}
	else
	{
		m_sceneCamera->MoveCamera(FORWARD);

		if (n == TELE1)
		{
			SoundManager::GetInstance()->PlayASound("teleport");
			Vector2 mappos = m_sceneManager->m_map->GetMapPos(
				MainCharacterPosition.z, 
				MainCharacterPosition.x
			);
			Vector2 telePos = m_sceneManager->m_map->Tele1(mappos);
			
			m_sceneCamera->SetPosition(
				Vector3(
					(telePos.y - m_sceneManager->m_map->GetWidth() / 2.0), 
					m_sceneCamera->GetPosition().y,
					-telePos.x) 
				* MAPFACTOR
			);
		}

		if (n == TELE2)
		{
			SoundManager::GetInstance()->PlayASound("teleport");
			Vector2 mappos = m_sceneManager->m_map->GetMapPos(
				MainCharacterPosition.z, 
				MainCharacterPosition.x
			);
			Vector2 telePos = m_sceneManager->m_map->Tele2(mappos);
			
			m_sceneCamera->SetPosition(
				Vector3(
					(telePos.y - m_sceneManager->m_map->GetWidth() / 2.0), 
					m_sceneCamera->GetPosition().y,
					-telePos.x) 
				* MAPFACTOR
			);
		}
	}

	// Joypad
	m_joypad->Update();

	// Score & Blood
	if (m_mainCharacter->GetScore() < 0)
	{
		m_mainCharacter->SetBlood(m_mainCharacter->GetBlood()-1);
		m_mainCharacter->SetScore(0);
	}

	// Consider GAME_OVER or update scoreboard / bloodboard
	if (m_mainCharacter->GetBlood() <= 0)
	{
		SoundManager::GetInstance()->PlayASound("gameover");
		m_status = GAME_OVER;

		boostSpeed = 0.0f;
		boostTime = 0.0f;
		totalBoostTime = 0.0f;
		boostMagnet = 0.0f;
		additionalBoostSpeed = 0.0f;
		additionalBoostMagnet = 0.0f;

		m_mainCharacter->SetMagnet(0.0f);
	}
	else
	{
		m_number->SetNumber(m_mainCharacter->GetScore());
	}

	if (m_combo == 3)
	{
		SoundManager::GetInstance()->PlayASound("score");
	}

	else if (m_combo == 9)
	{
		SoundManager::GetInstance()->PlayASound("cool");
	}

	else if (m_combo == 15)
	{
		SoundManager::GetInstance()->PlayASound("yowza");
	}
}

//---------------------------------------------------------------------------------------------------
/// Init - Level manager
//---------------------------------------------------------------------------------------------------
void InGame::Init(const char* id, const char* path)
{
	m_inputManager = InputManager::GetInstance();
	strcpy(m_id, id);

	m_levelsManagerPath = new char[strlen(path) + 1];
	strcpy(m_levelsManagerPath, path);	

	FILE* f = FileManager::GetInstance()->Open(m_levelsManagerPath, "r");

	if (f)
	{
		int n;
		char id[SIZE_OF_ID];
		char path[SIZE_OF_STRING];
		float speed;

		fscanf(f, "#Levels: %d\n", &n);

		m_levelID = new char*[n];
		m_levelPath = new char*[n];
		m_challenge = new Challenge[n];

		m_nLevels = n;

		// ID	Path
		fscanf(f, "%*s %*s %*s\n");
		for (int i = 0; i < n; ++i)
		{	
			fscanf(f, "%s %s %f\n", id, path, &speed);
			LOGI("%d\n", n);
			m_levelID[i] = new char[strlen(id) + 1];
			strcpy(m_levelID[i], id);

			m_levelPath[i] = new char[strlen(path) + 1];
			strcpy(m_levelPath[i], path);
			m_challenge[i].speed = speed;
		}


		SetCurrentLevelScenePath(
			m_levelPath[0]
		);

	}
	else
	{
		LOGI("COULD NOT READ LEVELS FILE!\n");
	}
}

//---------------------------------------------------------------------------------------------------
/// Draw
//---------------------------------------------------------------------------------------------------
void InGame::Draw()
{
	//Shadow
	//glBindFramebuffer(GL_FRAMEBUFFER, ResourcesManager::GetInstance()->GetFramebufferById("ShadowMap")->GetFboId());
	//EffectManager::GetInstance()->m_drawShadow = true;
	DrawAll();
	//EffectManager::GetInstance()->m_drawShadow = false;
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//EffectManager::GetInstance()->DrawEffect("Bloom");

	char time[5];
	char sp[5];
	char magnet[5];


	sprintf(sp, "%d", (int)(m_sceneCamera->m_speed * 100.0 / 3.0));

	sprintf(time, "%d", (int)(totalBoostTime - boostTime));

	String::GetInstance()->DrawString(
			sp, 
			Vector3(0.23f, 0.84f, 0.0f),  
			Vector3(0.7f, 0.7f, 0.7f)
		);

	String::GetInstance()->DrawString(
			time, 
			Vector3(-0.57f, 0.84f, 0.0f),  
			Vector3(0.7f, 0.7f, 0.7f)
		);

	sprintf(magnet, "%d", (int)(m_mainCharacter->GetMagnet()*200));
	String::GetInstance()->DrawString(
			magnet, 
			Vector3(-0.17f, 0.84f, 0.0f),  
			Vector3(0.7f, 0.7f, 0.7f)
		);

	std::stringstream ss;
	ss.clear();
	ss << "+";
	ss << m_combo - 1;

	static float y = 0.65f;
	static int prev = m_combo;
	if (y > 0.95f && prev != m_combo)
	{
		y = 0.65f;
		prev = m_combo;
	}
	if (m_combo > 1)
	{
		String::GetInstance()->DrawString(
			ss.str().c_str(), 
			Vector3(0.34f, y+=0.01f, 1.0f), 
			Vector3(1.2f, 1.2f, 1.2f)
		);
	}

	if (Timer::GameTime() < 3.0f)
	{
		char level[15];
		sprintf(level, "Level %d", Setting::levelId);
		String::GetInstance()->DrawString(level, Vector3(0.0f - Timer::GameTime()/5, 0.0f, 1.0f), Vector3(1.5f, 1.5f, 1.5f) * Timer::GameTime());
	}

	char fps[MAX_256];
	sprintf(fps, "FPS: %.0f", (float) 1 / Timer::FrameTime());
	String::GetInstance()->DrawString(fps, Vector3(-0.99f, -0.95f, 1.0f), Vector3(0.5f, 0.5f, 0.5f));
}

//---------------------------------------------------------------------------------------------------
/// DrawAll
//---------------------------------------------------------------------------------------------------
void InGame::DrawAll()
{
	m_sceneManager->Draw();

	Matrix vpMatrix = m_sceneCamera->CalculateVPMatrix();

	// Prepare data to test distance
	
	Vector4 moveL(0, 0, -1, 0);
	Vector3 moveW = Vector3((moveL * m_sceneCamera->GetWorldMatrix()).xyz);
	Vector2 cameraVector = Vector2(moveW.x, moveW.z);
	Vector3 v;
	Vector2 itemVector;
	//int count = 0, count2 = 0;

	for (int i = 0; i < m_numOfItems; i++)
	{
		if (	
				m_items[i]
				&& 
				Distance(m_items[i]->GetPosition(), m_sceneCamera->GetPosition()) < DISTANCE
		   )
		{
			v = m_items[i]->GetPosition() - m_sceneCamera->GetPosition();
			itemVector = Vector2(v.x, v.z);

			if (cameraVector.Dot(itemVector) >= 0)
			{
				m_items[i]->Draw(vpMatrix);
				//count++;
			}
			//count2++;
		}
	}

	/*char countString[30];
	sprintf(countString, "Num of item : %d %d", count, count2);
	String::GetInstance()->DrawString(countString, Vector3(-0.5f, 0.5f, 0.0f), Vector3(1.0f, 1.0f, 1.0f));*/

	m_mainCharacter->Draw(m_sceneCamera->m_projectMatrix);

	
	moveL = Vector4(0, 0, -0.5, 1);
	Vector4 MainCharacterPosition;
	MainCharacterPosition = moveL * m_sceneCamera->CalculateWorldMatrix();
	m_fogVision->SetPosition(
		Vector3(MainCharacterPosition.x, 
		MainCharacterPosition.y,
		MainCharacterPosition.z)
	);

	Vector3 cen = m_fogVision->GetModel()->m_center;
	Matrix trans;
	trans.SetTranslation(-cen);
	Vector4 pos = Vector4(
		m_fogVision->GetPosition().x, 
		m_fogVision->GetPosition().y, 
		m_fogVision->GetPosition().z, 
		1
	);
	pos = pos * trans;

	trans.SetTranslation(
		Vector3(
			MainCharacterPosition.x, 
			MainCharacterPosition.y,
			MainCharacterPosition.z
		)
	);

	pos = pos * trans;

	m_fogVision->SetPosition(
		Vector3(pos.x, pos.y, pos.z)
	);

	m_fogVision->Draw(vpMatrix);

	// joypad
	m_joypad->Draw();

	// score board
	Matrix mat;
	m_scoreBoard->UpdateTextureUV();
	m_scoreBoard->Draw(mat);
	m_number->Draw();

	m_objManager->Draw(vpMatrix);

	// blood
	float x = 0.88f + 0.13f;
	for (int i = 0; i < m_mainCharacter->GetBlood(); ++i)
	{
		m_heartIcon->SetPosition(Vector3(x -= 0.13f, 0.575f, 0.0f));
		m_heartIcon->UpdateTextureUV();
		m_heartIcon->Draw(mat);
	}

	m_velo->Draw(mat);
	m_magnet->Draw(mat);
	m_clock->Draw(mat);

	//m_magnetVision->Draw(vpMatrix);

	// btPpause
	m_btPause->Draw(mat);

	//menu Pause
	if (PauseMenu::GetInstance()->IsVisible())
	{
		PauseMenu::GetInstance()->Draw();
	}

	//GAMEOVER
	if (GameOver::GetInstance()->IsVisible())
	{
		GameOver::GetInstance()->Draw();
	}
}

//---------------------------------------------------------------------------------------------------
/// Destroy: Clear state components
//---------------------------------------------------------------------------------------------------
void InGame::Destroy() 
{
	State::Destroy();

	for (int i = 0; i < m_numOfItems; i++)
	{
		SAFE_DEL(m_items[i]);
	}
	m_numOfItems = 0;
	SAFE_DEL(m_items);
	
	SAFE_DEL(m_joypad);
	//SAFE_DEL(m_number);
	Number::DestroyInstance();
	SAFE_DEL(m_scoreBoard);

	SAFE_DEL(m_heartIcon);
	SAFE_DEL(m_velo);
	SAFE_DEL(m_magnet);
	SAFE_DEL(m_clock);
	SAFE_DEL(m_fogVision);

	SAFE_DEL(m_levelsManagerPath);
	for (int i = 0; i < m_nLevels; ++i)
	{
		SAFE_DEL(m_levelID[i]);
		SAFE_DEL(m_levelPath[i]);
	}
	m_nLevels = 0;
	SAFE_DEL(m_levelID);
	SAFE_DEL(m_levelPath);
	
	SAFE_DEL(m_challenge);

	ObjectsManager::DestroyInstance();
	EffectManager::DestroyInstance();

	//SAFE_DEL(m_btPause);
}

//---------------------------------------------------------------------------------------------------
/// GetInstance
//---------------------------------------------------------------------------------------------------
InGame* InGame::GetInstance()
{
	if (s_Instance == NULL)
	{
		s_Instance = new InGame();
	}
	return s_Instance;
}

//---------------------------------------------------------------------------------------------------
/// DestroyInstance
//---------------------------------------------------------------------------------------------------
void InGame::DestroyInstance()
{
	SAFE_DEL(s_Instance);
}

//---------------------------------------------------------------------------------------------------
/// Constructor
//---------------------------------------------------------------------------------------------------
InGame::InGame(void):State()
{
	m_fboId = 0;
	m_depthTextureId = 0;
	m_colorTextureId = 0;

	m_joypad = 0;
	m_number = 0;

	m_heartIcon = 0;
	m_velo = 0;
	m_magnet = 0;
	m_clock = 0;
	m_fogVision = 0;

	m_levelsManagerPath = 0;
	m_nLevels = 0;
	m_levelID = 0;
	m_levelPath = 0;
	
	m_numOfItems = 0;
	m_items = 0;

	m_challenge = 0;

	m_scoreBoard = 0;
	m_btPause = 0;

	m_objManager = 0;
	m_mainCharacter = 0;

	
}

//---------------------------------------------------------------------------------------------------
/// Destructor
//---------------------------------------------------------------------------------------------------
InGame::~InGame(void)
{
	SAFE_DEL(m_levelsManagerPath);
	for (int i = 0; i < m_nLevels; ++i)
	{
		SAFE_DEL(m_levelID[i]);
		SAFE_DEL(m_levelPath[i]);
	}
	m_nLevels = 0;
	SAFE_DEL(m_levelID);
	SAFE_DEL(m_levelPath);
	
	SAFE_DEL(m_challenge);
}

//---------------------------------------------------------------------------------------------------
/// LEVELS MANAGER: GetCurrent
//---------------------------------------------------------------------------------------------------
char* InGame::GetCurrentLevelScenePath()
{
	return m_sceneFilePath;
}

//---------------------------------------------------------------------------------------------------
/// LEVELS MANAGER: GetByID
//---------------------------------------------------------------------------------------------------
char* InGame::GetLevelScenePathByID(char* levelID)
{
	for (int i = 0; i < m_nLevels; ++i)
	{
		if (strcmp(levelID, m_levelID[i]) == 0)
		{
			return m_levelPath[i];
		}
	}
	return NULL;
}

//---------------------------------------------------------------------------------------------------
/// LEVELS MANAGER: GetNext (base on current scene path index)
//---------------------------------------------------------------------------------------------------
char* InGame::GetNextLevelScenePath()
{
	char current[SIZE_OF_STRING];
	strcpy(current, GetCurrentLevelScenePath());

	for (int i = 0; i < m_nLevels; ++i)
	{
		if (strcmp(current, m_levelPath[i]) == 0 && i + 1 < m_nLevels)
		{
			return m_levelPath[i+1];
		}
	}
	return NULL;
}

//---------------------------------------------------------------------------------------------------
/// LEVELS MANAGER: GetNext (base on current scene path index)
//---------------------------------------------------------------------------------------------------
int InGame::GetLevelManagementIndex(char* path)
{
	for (int i = 0; i < m_nLevels; ++i)
	{
		if (strcmp(path, m_levelPath[i]) == 0)
		{
			return i;
		}
	}
	return -1;
}

//---------------------------------------------------------------------------------------------------
/// LEVELS MANAGER: SetCurrent (base on current scene path index)
//---------------------------------------------------------------------------------------------------
void InGame::SetCurrentLevelScenePath(char* path)
{
	if (path)
	{
		boostSpeed = 0.0f;
		boostTime = 0.0f;
		totalBoostTime = 0.0f;
		boostMagnet = 0.0f;
		additionalBoostSpeed = 0.0f;
		additionalBoostMagnet = 0.0f;

		//m_mainCharacter->SetMagnet(0.0f);

		SAFE_DEL(m_sceneFilePath);

		for (int i = 0; i < m_numOfItems; i++)
		{
			SAFE_DEL(m_items[i]);
		}
		m_numOfItems = 0;
		SAFE_DEL(m_items);

		SAFE_DEL(m_joypad);
		//SAFE_DEL(m_number);
		Number::DestroyInstance();
		SAFE_DEL(m_scoreBoard);

		SAFE_DEL(m_heartIcon);
		//SAFE_DEL(m_btPause);

		m_sceneFilePath = new char[strlen(path) + 1];
		strcpy(m_sceneFilePath, path);

		int index = GetLevelManagementIndex(path);
		Setting::levelId = index + 1;
		if (index != -1)
		{
			m_currentChallenge.speed = m_challenge[index].speed;
		}
		else
		{
			m_currentChallenge.speed = 1.0f;
		}

		ObjectsManager::DestroyInstance();
		EffectManager::DestroyInstance();

		Timer::Reset();
		if (m_mainCharacter)
		{
			m_mainCharacter->m_pointTime = 0;
		}
	}
	else
	{
		//path = "Ve nuoc";
		//m_sceneFilePath = new char[strlen(path) + 1];
		//strcpy(m_sceneFilePath, path);
	}
}