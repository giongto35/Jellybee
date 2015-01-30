#include "Platforms.h"
#include "MainCharacter.h"
#include "ScenesManager.h"
#include "Timer.h"
#include "FileManager.h"
#include "ResourcesManager.h"

MainCharacter* MainCharacter::s_Instance = NULL;

MainCharacter* MainCharacter::GetInstance()
{
	if (s_Instance == 0)
	{
		s_Instance = new MainCharacter;
	}
	return s_Instance;
}

void MainCharacter::DestroyInstance()
{
	SAFE_DEL(s_Instance);
}

MainCharacter::MainCharacter(void) : Object()
{
	m_blood = 3;
	m_speed = Vector3(0.0f, 0.0f, 0.0f);
	m_score = 0;
	m_magnet = 0.0f;

	m_modelID = 0;
	m_frameTime = 0;
	m_numOfModels = 0;

	m_pointTime = 0;
	m_inputManager = 0;
	m_sceneCamera = 0;

}

MainCharacter::~MainCharacter(void)
{
	for (int i = 0; i < m_numOfModels; ++i)
	{
		SAFE_DEL(m_modelID[i]);
	}
	SAFE_DEL_ARRAY(m_modelID);
}

void MainCharacter::Init(const char* id, const Vector3 & pos, const Vector3 & rot, 
						const Vector3 & sca, Texture* texture, Model* model, Shader* shader, 
						int blood, Vector3 & speed, int score, const char *textureUV)
{
	Object::Init(id, pos, rot, sca, texture, model, shader, textureUV);
	m_blood = blood;
	m_speed = speed;
	m_score = score;

	m_inputManager = InputManager::GetInstance();
	//m_sceneCamera = ScenesManager::GetInstance()->GetCameraById("sceneCam");
}

float MainCharacter::GetMagnet()
{
	return m_magnet;
}

void MainCharacter::SetMagnet(float magnet)
{
	m_magnet = magnet;
}

int MainCharacter::GetBlood()
{
	return m_blood;
}

const Vector3 & MainCharacter::GetSpeed()
{
	return m_speed;
}

int MainCharacter::GetScore()
{
	return m_score;
}

void MainCharacter::SetBlood(int blood)
{
	m_blood = blood;
}

void MainCharacter::SetSpeed(const Vector3 & speed)
{
	m_speed = speed;
}

void MainCharacter::SetScore(int score)
{
	m_score = score;
}

void MainCharacter::Update()
{
	static Vector3 oldRot;
	 oldRot = m_sceneCamera->GetRotation();
	 oldRot.x = m_sceneCamera->m_rotationFake.x;
	 m_sceneCamera->Update();
	 
	 Vector3 deltaRot = Vector3(m_sceneCamera->m_rotationFake.x - oldRot.x, 
	  m_sceneCamera->GetRotation().y - oldRot.y,
	  m_sceneCamera->GetRotation().z - oldRot.z);
	 deltaRot.z = deltaRot.y;

	 if (InputManager::GetInstance()->IsKeyDown(ARROW_UP) || InputManager::GetInstance()->IsKeyDown(ARROW_DOWN))
	 {
	  m_rotation = m_rotation + deltaRot;
	  m_rotation.x = max(min(m_rotation.x, PI / 10),- PI / 10);
	  m_rotation.y = max(min(m_rotation.y, PI / 10),- PI / 10);
	  m_rotation.z = max(min(m_rotation.z, PI / 4),- PI / 4);
	  SetRotation(m_rotation + deltaRot);
	 }
	 else
	 {
	  if (m_rotation.x > 0)
	  {
	   m_rotation.x = max(m_rotation.x - m_sceneCamera->m_speed * 2.0f * Timer::FrameTime(), 0);
	  }

	  if (m_rotation.x < 0)
	  {
	   m_rotation.x = min(m_rotation.x + m_sceneCamera->m_speed * 2.0f * Timer::FrameTime(), 0);
	  }
	 }

	 if (InputManager::GetInstance()->IsKeyDown(ARROW_LEFT) || InputManager::GetInstance()->IsKeyDown(ARROW_RIGHT))
	 {
	  m_rotation = m_rotation + deltaRot;
	  m_rotation.x = max(min(m_rotation.x, PI / 10),- PI / 10);
	  m_rotation.y = max(min(m_rotation.y, PI / 10),- PI / 10);
	  m_rotation.z = max(min(m_rotation.z, PI / 4),- PI / 4);
	  SetRotation(m_rotation + deltaRot);
	 }
	 else
	 {
	  if (m_rotation.y > 0)
	  {
	   m_rotation.y = max(m_rotation.y - m_sceneCamera->m_speed * 2.0f * Timer::FrameTime(), 0);
	  }

	  if (m_rotation.y < 0) 
	  {
	   m_rotation.y = min(m_rotation.y + m_sceneCamera->m_speed * 2.0f * Timer::FrameTime(), 0);
	  }

	  if (m_rotation.z > 0)
	  {
	   m_rotation.z = max(m_rotation.z - m_sceneCamera->m_speed * 2.0f * Timer::FrameTime(), 0);
	  }
	  
	  if (m_rotation.z < 0)
	  {
	   m_rotation.z = min(m_rotation.z + m_sceneCamera->m_speed * 2.0f * Timer::FrameTime(), 0);
	  }

	  SetRotation(m_rotation);
	  //m_sceneCamera->SetRotation(Vector3(m_rotation.x, m_sceneCamera->GetRotation().y, m_sceneCamera->GetRotation().z));

 }

	//ANIMATION
	 static int frameIndex = 0;
	 GLfloat time = Timer::GameTime ();
	 if ( ( m_frameTime + m_pointTime ) <= time )
	 {
		 m_pointTime += m_frameTime;
		 if ( frameIndex < m_numOfModels - 1 )
		 {
			 frameIndex ++;
		 }
		 else
		 {
			 frameIndex = 0;
		 }
		 //LOGI ("Frame index: %d", m_frameIndex );
		 m_model = ResourcesManager::GetInstance()->GetModelById (m_modelID [ frameIndex ] );
	 }
}

void MainCharacter::Init(const char* file)
{

	ResourcesManager* resManager = ResourcesManager::GetInstance();

	FILE* f = FileManager::GetInstance()->Open(file, "r");

	if (f == NULL)
	{
		LOGE("Can't not open %s file\n", file);
		return;
	}

	char id[SIZE_OF_ID];
	char texture[SIZE_OF_ID];
	char shader[SIZE_OF_ID];
	char texture_uv[SIZE_OF_ID];
	Vector3 pos, rot, sca;

	// ID
	fscanf(f, "#ID: %s\n", id);

	// MODEL
	fscanf(f, "#MODEL: %d\n", &m_numOfModels);
	m_modelID = new char* [m_numOfModels];
	for (int i = 0; i < m_numOfModels; ++i)
	{
		m_modelID[i] = new char[SIZE_OF_ID];
		fscanf(f, "%s", m_modelID[i]);
	}
	fscanf(f, "\n");

	// TEXTURE
	fscanf(f, "#TEXTURE: %s\n", texture);

	// SHADER
	fscanf(f, "#SHADER: %s\n", shader);

	// POSITION
	fscanf(f, "#POSITION: %f, %f, %f\n", &pos.x, &pos.y, &pos.z);
	
	// ROTATION
	fscanf(f, "#ROTATION: %f, %f, %f\n", &rot.x, &rot.y, &rot.z);

	// SCALE
	fscanf(f, "#SCALE: %f, %f, %f\n", &sca.x, &sca.y, &sca.z);

	// TEXTURE_UV
	fscanf(f, "#TEXTURE_UV: %s\n", texture_uv);

	if(strcmp(texture_uv, "NULL") == 0)
	{
		Object::Init(id, pos, rot, sca, resManager->GetTextureById(texture), resManager->GetModelById(m_modelID[0]), resManager->GetShaderById(shader), NULL);
	}
	else
	{
		Object::Init(id, pos, rot, sca, resManager->GetTextureById(texture), resManager->GetModelById(m_modelID[0]), resManager->GetShaderById(shader), texture_uv);
	}

	for (int i = 0; i < m_numOfModels; ++i)
	{
		Model *model = resManager->GetModelById(m_modelID[i]);
		SetModel(model);
	}
	
	// FRAME_TIME
	fscanf(f, "#FRAME_TIME: %f\n", &m_frameTime);

	// BLOOD
	fscanf(f, "#BLOOD: %d\n", &m_blood);

	// SPEED
	fscanf(f, "#SPEED: %f, %f, %f\n", &m_speed.x, &m_speed.y, &m_speed.z);

	// SCORE
	fscanf(f, "#SCORE: %d\n", &m_score);

	fclose(f);

	m_inputManager = InputManager::GetInstance();
}