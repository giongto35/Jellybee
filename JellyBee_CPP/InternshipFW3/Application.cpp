#include "Application.h"
#include "String.h"
#include "MainCharacter.h"
#include "PauseMenu.h"
#include "GameOver.h"
#include "Timer.h"
#include "SoundManager.h"
#include "AppDataManager.h"
#include "J2CBridge.h"

//---------------------------------------------------
/// Constructor
//---------------------------------------------------
Application::Application(void)
{
	m_fileManager = 0;
	m_inputManager = 0;
	m_resourcesManager = 0;
	m_statesManager = 0;
}

//---------------------------------------------------
/// Get Instance
//---------------------------------------------------
Application* Application::s_Instance = NULL;
Application* Application::GetInstance()
{
	if (s_Instance == NULL)
	{
		s_Instance = new Application();
	}
	return s_Instance;
}

//---------------------------------------------------
/// INIT
//---------------------------------------------------
void Application::Init()
{
	// input manager
	m_inputManager = InputManager::GetInstance();

	// init file manager
	m_fileManager = FileManager::GetInstance();
	#ifdef _ANDROID
		m_fileManager->Init("sdcard/Resources/");
	#else
	#ifdef _WIN32
		m_fileManager->Init("../Resources/");
	#endif
	#endif

	// init resources manager
	m_resourcesManager = ResourcesManager::GetInstance();
	//m_resourcesManager->Init("Managers/ResourcesManager.txt");

	// init states manager
	m_statesManager = StatesManager::GetInstance();
	m_statesManager->Init();

	//String::GetInstance()->Init();
}

//---------------------------------------------------
/// UPDATE
//---------------------------------------------------
void Application::Update()
{
	//LOGI("fps = %.0f\n", 1 / Timer::FrameTime());
	m_statesManager->Update();	
}

//---------------------------------------------------
/// DRAW
//---------------------------------------------------
void Application::Draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_statesManager->m_currentState->Draw();
}

//---------------------------------------------------
/// Destroy Instance
//---------------------------------------------------
void Application::DestroyInstance()
{
	SAFE_DEL(s_Instance);
}

//---------------------------------------------------
/// Destructor
//---------------------------------------------------
Application::~Application(void)
{
	InputManager::DestroyInstance();
	FileManager::DestroyInstance();
	ResourcesManager::DestroyInstance();
	StatesManager::DestroyInstance();

	ScreenResolution::DestroyInstance();
	String::DestroyInstance();
	MainCharacter::DestroyInstance();

	PauseMenu::DestroyInstance();
	GameOver::DestroyInstance();
	SoundManager::DestroyInstance();
	AppDataManager::DestroyInstance();
	J2CBridge::DestroyInstance();
}