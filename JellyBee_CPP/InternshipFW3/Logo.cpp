#include "Logo.h"
#include "Timer.h"
#include "NameSpace.h"
#include "Object.h"
#include "Texture.h"
#include "ResourcesManager.h"

Logo* Logo::s_Instance = NULL;

Logo::Logo(void)
:State()
{
}

Logo::~Logo(void)
{
}

Logo* Logo::GetInstance()
{
	if (s_Instance == NULL)
	{
		s_Instance = new Logo();
	}
	return s_Instance;
}

void Logo::DestroyInstance()
{
	if (s_Instance != NULL)
	{
		delete s_Instance;
		s_Instance = NULL;
	}
}

void Logo::Start()
{
	State::Start();
	Matrix matrix;
	matrix.SetOrtho(-1.0f, 
		             1.0f, 
					-1.0f, 
					 1.0f, 
					 0.5f, 
					50.0f);
	m_sceneCamera->SetProjectMatrix(matrix);

	Object* logo = m_sceneManager->GetObjectById("logo");
	Texture** textures = new Texture*[3];
	textures[0] = ResourcesManager::GetInstance()->GetTextureById("logo");
	textures[1] = ResourcesManager::GetInstance()->GetTextureById("uvdMap");
	textures[2] = ResourcesManager::GetInstance()->GetTextureById("mask");
	logo->Init( logo->GetId(), 
			    logo->GetPosition(), 
				logo->GetRotation(), 
				logo->GetScale(),
				3, 
				textures, 
				logo->GetModel(), 
				logo->GetShader());
	SAFE_DEL_ARRAY(textures);

}

void Logo::Update()
{
	if (Timer::GameTime() >= 2.0f)
	{
		m_status = FINISHED;
		//Setting::isBackButtonPressed = false;
	}
}

void Logo::Draw()
{
	m_sceneManager->Draw();
}
