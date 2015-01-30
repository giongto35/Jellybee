#include "InputManager.h"
#include <string.h>
#include "ScreenResolution.h"

//---------------------------------------------------
/// Constructor
//---------------------------------------------------
InputManager::InputManager(void)
{
	memset(m_keyCode, false, MAX_256 * sizeof(bool));
	m_x = -1;
	m_y = -1;
	m_type = -1;
	m_dx = 0;
	m_dy = 0;
	m_isDown = false;
}

//---------------------------------------------------
/// Init
//---------------------------------------------------
void InputManager::Init()
{
		
}

//---------------------------------------------------
/// EVENTS HANDLER
//---------------------------------------------------
void InputManager::OnKey(int keyCode, bool isKeyDown)
{
	//LOGI("\nKey code: %d - is key down: %d", keyCode, isKeyDown);
	m_keyCode[keyCode] = isKeyDown;
}

void InputManager::OnPointer(int type, int x, int y)
{
	// type = 0 : down
	// type = 1 : up

	//LOGI("\nPointer type: %d - (x = %d, y = %d)", type, x, y);
	if (type == 0)
	{
		m_isDown = true;
	}
	m_type = type;
	m_x = x;
	m_y = y;
}

void InputManager::OnDrag(int x, int y)
{
	// type = 2 : move
	//LOGI("dx = %d   ;  dy = %d\n", m_dx, m_dy);
	m_type = 2;
	m_dx = (x - m_x);// / ScreenResolution::GetInstance()->m_densityDpi;
	m_dy = (y - m_y);// / ScreenResolution::GetInstance()->m_densityDpi;
	m_x = x;
	m_y = y;

}

//---------------------------------------------------
/// GET INPUTS
//---------------------------------------------------
bool InputManager::IsKeyDown(int keyCode)
{
	return m_keyCode[keyCode];
}

bool InputManager::IsPointerDown()
{
	if (m_type == 0)
	{
		return true;
	}

	return false;
}

bool InputManager::IsPointerUp()
{
	if (m_type == 1)
	{
		return true;
	}

	return false;
}

bool InputManager::IsPointerOnRect(int x, int y, int width, int height)
{ 
	if (  (m_x >= x) 
			&& (m_x <= x + width) 
			&& (m_y >= y) 
			&& (m_y <= y + height) 
			&& (m_type == 0))
	{
		//m_type = -1;
		//m_isDown = false;
		return true;
	}

	return false;
}

bool InputManager::IsClicked()
{
	if (m_isDown && m_type == 1)
	{
		m_isDown = false;
		return true;
	}

	return false;
}

bool InputManager::IsDrag()
{
	if (m_type == 2)
	{
		m_type = -1;
		return true;
	}

	return false;
}

int InputManager::GetX()
{
	return m_x;
}

int InputManager::GetY()
{
	return m_y;
}

int InputManager::GetDx()
{
	return m_dx;
}

int InputManager::GetDy()
{
	return m_dy;
}

//---------------------------------------------------
/// Get Instance
//---------------------------------------------------
InputManager* InputManager::s_Instance = 0;
InputManager* InputManager::GetInstance()
{
	if (!s_Instance)
	{
		s_Instance = new InputManager();
	}
	return s_Instance;
}

//---------------------------------------------------
/// Destroy Instance
//---------------------------------------------------
void InputManager::DestroyInstance() 
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
InputManager::~InputManager(void)
{
}
