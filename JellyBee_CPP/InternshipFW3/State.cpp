#include "State.h"
#include <cstring>

State::State(void)
{
	m_id[0] = '\0';
	m_sceneFilePath = 0;
	m_sceneManager = 0;
	m_inputManager = 0;
	m_sceneCamera = 0;
	m_Framebuffer = 0;
	m_status = FINISHED;
}

void State::Init(const char* id, const char* path)
{
	m_inputManager = InputManager::GetInstance();
	strcpy(m_id, id);
	m_sceneFilePath = new char[strlen(path) + 1];
	strcpy(m_sceneFilePath, path);	
}

void State::Start()
{
	ScenesManager::DestroyInstance();
	m_sceneManager = ScenesManager::GetInstance();
	m_sceneManager->Init(m_sceneFilePath);

	m_sceneCamera = m_sceneManager->GetCameraById("sceneCam");
	m_status = ON_GOING;
}

const char* State::GetId()
{
	return m_id;
}


State::~State(void)
{
	SAFE_DEL(m_sceneFilePath);
}

void State::Update() 
{
}

void State::Draw()
{
}

void State::Destroy() 
{
	ScenesManager::DestroyInstance();
	SAFE_DEL(m_Framebuffer);
	//m_status = FINISHED;
}