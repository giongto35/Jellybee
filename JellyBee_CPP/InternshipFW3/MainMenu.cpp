#include "MainMenu.h"
#include "Timer.h"
#include "ScreenResolution.h"
#include "Framebuffer.h"
#include "ResourcesManager.h"
#include "FileManager.h"
#include "Application.h"
#include <cstdio>
#include "String.h"
#include "SoundManager.h"
#include "NameSpace.h"
#include "AppDataManager.h"
#include "MainCharacter.h"

MainMenu* MainMenu::s_Instance = NULL;

MainMenu::MainMenu(void)
:State()
{
	m_fboId = 0;
	m_depthTextureId = 0;
	m_colorTextureId = 0;

	m_numOfButton = 0;
	m_button = 0;

	m_info = 0;
	m_help = 0;
	m_panelGround = 0;
	m_ground = 0;
}

MainMenu::~MainMenu(void)
{
	
}

MainMenu* MainMenu::GetInstance()
{
	if (s_Instance == NULL)
	{
		s_Instance = new MainMenu();
	}
	return s_Instance;
}

void MainMenu::Destroy()
{
	State::Destroy();
	
	SAFE_DEL_ARRAY(m_button);
}

void MainMenu::DestroyInstance()
{
	if (s_Instance != NULL)
	{
		delete s_Instance;
		s_Instance = NULL;
	}
}

void MainMenu::Start()
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

	m_numOfButton = NUM_OF_BUTTON_IN_MAIN_MENU;
	const char* buttonID[NUM_OF_BUTTON_IN_MAIN_MENU] = { "btnStart", "SoundOn", "SoundOff", "btnQuit", "btHelp", "btInfo", "btBack"};

	m_button = new Button* [m_numOfButton];

	for(int i = 0; i < m_numOfButton; i++)
	{
		m_button[i] = (Button*) m_sceneManager->GetObjectById(buttonID[i]);
	}
	Setting::isSoundOn = AppDataManager::GetInstance()->IsSoundTurnedOn();
	m_button[1]->m_isVisible = !Setting::isSoundOn;
	m_button[2]->m_isVisible = Setting::isSoundOn;

	m_info = m_sceneManager->GetObjectById("infoPanel");
	m_info->m_isVisible = false;
	m_help = m_sceneManager->GetObjectById("helpPanel");
	m_help->m_isVisible = false;	
	m_panelGround = m_sceneManager->GetObjectById("panelGround");
	m_panelGround->m_isVisible = false;	

	m_ground = m_sceneManager->GetObjectById("mainmenu");

}

//-------------------------------------------------------------
/// UPDATE
//-------------------------------------------------------------
void MainMenu::Update()
{
	m_sceneManager->Update();
	for (int i = 0; i < m_numOfButton; ++i)
	{		
		if (m_button[i]->IsClicked())
		{
			OnButtonClick(i);
		}
	}

	if (Setting::isBackButtonPressed)
	{
		if (m_button[6]->m_isVisible) // back button
		{
			OnButtonClick(6);
		}
		else
		{
			AppDataManager::GetInstance()->ToggleSound(Setting::isSoundOn);
			Application::DestroyInstance();
			exit(0);
		}

		Setting::isBackButtonPressed = false;
	}
}


//-------------------------------------------------------------
/// ON BUTTON CLICK HANDLER
//-------------------------------------------------------------
void MainMenu::OnButtonClick(int index)
{
	switch(index)
	{
		case 0: // Start button
		{
			AppDataManager::GetInstance()->ToggleSound(Setting::isSoundOn);
			m_status = FINISHED;
			MainCharacter::GetInstance()->SetBlood(3);
			MainCharacter::GetInstance()->SetScore(0); 
		}
		break;

		case 1: // Option: sound on
		{
			//m_button[1]->m_isVisible = !m_button[1]->m_isVisible;
			//m_button[2]->m_isVisible = !m_button[2]->m_isVisible;
			Setting::isSoundOn = true;
			m_button[1]->m_isVisible = false;
			m_button[2]->m_isVisible = true;
			m_button[2]->m_isClicked = false;
			LOGI("sound on\n");
		}
		break;

		case 2: // Option: sound off
		{
			Setting::isSoundOn = false;
			m_button[2]->m_isVisible = false;
			m_button[1]->m_isVisible = true;
			m_button[1]->m_isClicked = false;
			LOGI("sound off\n");
		}
		break;

		case 3: // Quit button
		{
			AppDataManager::GetInstance()->ToggleSound(Setting::isSoundOn);
			Application::DestroyInstance();
			exit(0);			
		}
		break;

		case 4: // Help Button
		{
			for (int i = 0; i < m_numOfButton - 1; ++i)
			{
				m_button[i]->m_isVisible = false;
			}
			m_ground->m_isVisible = false;

			m_panelGround->m_isVisible = true;
			m_help->m_isVisible = true;
			m_button[6]->m_isVisible = true;
			Timer::Reset();
		}
		break;

		case 5: // Intruction Button
		{
			for (int i = 0; i < m_numOfButton - 1; ++i)
			{
				m_button[i]->m_isVisible = false;
			}
			m_ground->m_isVisible = false;

			m_panelGround->m_isVisible = true;
			m_info->m_isVisible = true;
			m_button[6]->m_isVisible = true;
			Timer::Reset();
		}
		break;

		case 6: // Back button
		{
			for (int i = 0; i < m_numOfButton - 1; ++i)
			{
				m_button[i]->m_isVisible = true;
			}
			m_button[1]->m_isVisible = !Setting::isSoundOn;
			m_button[2]->m_isVisible = Setting::isSoundOn;
			m_ground->m_isVisible = true;

			m_help->m_isVisible = false;
			m_info->m_isVisible = false;
			m_button[6]->m_isVisible = false;
			m_panelGround->m_isVisible = false;
			Timer::Reset();
		}
		break;

		default:
		{
			LOGE("ERROR unexpected value of main menu button index!\n");
			return;
		}
	}
}

//-------------------------------------------------------------
/// DRAW
//-------------------------------------------------------------
void MainMenu::Draw()
{
	m_sceneManager->Draw();
}

Button* MainMenu::ButtonDetection(int a)
{
	Button* btn = NULL;

	if (a > m_numOfButton || a == 0)
	{
		LOGI("Button not identified!\n");
	}
	else
	{
		btn = m_button[a - 1];
		LOGI("touch on %s \n", btn->GetId());
	}

	return btn;
}
