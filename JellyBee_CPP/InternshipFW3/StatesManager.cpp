#include "StatesManager.h"
#include "Timer.h"
#include "Logo.h"
#include "MainMenu.h"
#include "InGame.h"
#include <cstring>
#include "PauseMenu.h"
#include "GameOver.h"
#include "Loading.h"
#include "NameSpace.h"
#include "SoundManager.h"
#include "AppDataManager.h"

//---------------------------------------------------
/// Constructor
//---------------------------------------------------
StatesManager::StatesManager(void)
{
	m_currentState = 0;
}

//---------------------------------------------------
/// GetInstance
//---------------------------------------------------
StatesManager* StatesManager::s_Instance = NULL;
StatesManager* StatesManager::GetInstance()
{
	if (!s_Instance)
	{
		s_Instance = new StatesManager();
	}
	return s_Instance;
}

//---------------------------------------------------
/// Init
/// Define list of states' information & set the opening state
//---------------------------------------------------
void StatesManager::Init()
{
	// States list
	State *state = NULL;

	state = Loading::GetInstance();
	state->Init("Loading", "Managers/ResourcesManager.txt");
	m_states.push_back(state);
	
	state = Logo::GetInstance();
	state->Init("Logo", "Managers/LogoState.txt");
	m_states.push_back(state);

	state = MainMenu::GetInstance();
	state->Init("MainMenu", "Managers/MainMenuState.txt");
	m_states.push_back(state);

	state = InGame::GetInstance();
	state->Init("InGame", "Managers/Levels.txt");
	m_states.push_back(state);

	m_currentState = m_states[0]; // State Loading go first
	m_currentState->Start();

	//PauseMenu::GetInstance()->Init();
	//GameOver::GetInstance()->Init();
}

//---------------------------------------------------
/// Update
/// Game's front script, manage states' status onchanged
//---------------------------------------------------
void StatesManager::Update()
{
	// ON RESUME: reset opaque & start update
	if (m_currentState->m_status == ON_GOING) 
	{
		m_currentState->Update();
	}

	else if (m_currentState->m_status == FINISHED)
	{
		SwitchState(GetNextState());
	}

	// ON PAUSE: set opaque 0.5 & stop update
	else if (m_currentState->m_status == ON_PAUSE)
	{
		PauseMenu::GetInstance()->SetVisible(true);
		PauseMenu::GetInstance()->Update();
	}

	else if (m_currentState->m_status == ON_RESUME)
	{
		Setting::isBackButtonPressed = false;

		PauseMenu::GetInstance()->SetVisible(false);
		GameOver::GetInstance()->SetVisible(false);
		m_currentState->m_status = ON_GOING;
	}

	else if (m_currentState->m_status == GAME_OVER)
	{
		GameOver::GetInstance()->SetVisible(true);
		GameOver::GetInstance()->Update();
	}
}

//---------------------------------------------------
/// Get State By Id
//---------------------------------------------------
State* StatesManager::GetStateById(const char* stateId)
{
	for (GLuint i = 0; i < m_states.size(); ++i)
	{
		if (strcmp(m_states[i]->GetId(), stateId) == 0)
		{
			return m_states[i];
		}
	}
	return NULL;
}

//---------------------------------------------------
/// Get Next State
//---------------------------------------------------
State* StatesManager::GetNextState()
{
	static unsigned int i;
	for (unsigned int j = 0; j < m_states.size(); j++)
	{
		if (m_states[j] == m_currentState)
		{
			i = j;
		}
	}
	i += 1;
	if (i == m_states.size())
	{
		i = 2;
	}

	return m_states[i];
}


//---------------------------------------------------
/// SwitchState
//---------------------------------------------------
void StatesManager::SwitchState(State* newState)
{
	AppDataManager::GetInstance()->ToggleSound(Setting::isSoundOn);
	Setting::isBackButtonPressed = false;
	if (newState)
	{
		if (m_currentState)
		{
			m_currentState->Destroy();
		}
		m_currentState = newState;
		m_currentState->Start();
		Timer::Reset();
	}
	else
	{
		LOGE("ERROR: cannot set current state\n");
	}

	//m_currentState->Start();
}

//---------------------------------------------------
/// Destroy
//---------------------------------------------------
void StatesManager::DestroyInstance()
{
	SAFE_DEL(s_Instance);
}

//---------------------------------------------------
/// Destructor
//---------------------------------------------------
StatesManager::~StatesManager(void)
{
	m_currentState->Destroy();
	m_states.clear();

	Logo::DestroyInstance();
	MainMenu::DestroyInstance();
	InGame::DestroyInstance();
	Loading::DestroyInstance();

	//PauseMenu::DestroyInstance();
	//GameOver::DestroyInstance();
}