#pragma once
#include "ResourcesManager.h"
#include "FileManager.h"
#include "InputManager.h"
#include "StatesManager.h"

class Application
{
private:
	
	// ATTRIBUTES
	InputManager* m_inputManager;
	FileManager* m_fileManager;
	StatesManager* m_statesManager;
	ResourcesManager* m_resourcesManager;

	static Application *s_Instance;
	
private:
	Application(void);
	~Application(void);
public:
	// SINGLETON
	static Application* GetInstance();
	static void DestroyInstance();

	// ACTIONS
	void Init();
	void Update();
	void Draw();
};
