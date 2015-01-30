#include "Button.h"
#include "ScreenResolution.h"
#include "SoundManager.h"

Button::Button(void) : Object()
{
	m_isClicked = false;
	m_width = 0;
	m_height = 0;
	m_topleftX = 0;
	m_topleftY = 0;
	
	m_inputManager = 0;
	m_isDown = false;
	m_isVisible = false;
}

Button::~Button(void)
{
}

void Button::Init(const char * id, const Vector3 & pos, const Vector3 & rot, const Vector3 & sca, Texture* texture,  Model* model, Shader* shader, const char* textureUvId)
{
	Object::Init(id, pos, rot, sca, texture, model, shader, textureUvId);
	ScreenResolution *screen = ScreenResolution::GetInstance();
	m_width = (int) (screen->m_screenWidth * sca.x);
	m_height = (int) (screen->m_screenHeight * sca.y);
	m_topleftX = (int) ((m_position.x + 1) * screen->m_screenWidth/2 - m_width/2);
	m_topleftY = (int) ((-m_position.y + 1) * screen->m_screenHeight/2 - m_height/2);

	m_inputManager = InputManager::GetInstance();

	m_scaleWhenPointerDown = sca / 1.06f;
	m_scaleWhenPointerUp = sca;
}

bool Button::IsClicked()
{
	if (m_isClicked)
	{
		SoundManager::GetInstance()->PlayASound("yes");
		m_isClicked = false;
		return true;
	}
	else
	{
		return false;
	}
}

void Button::Update()
{
	if (m_inputManager->IsPointerOnRect( m_topleftX, 
										 m_topleftY, 
										 m_width, 
										 m_height) && m_isVisible )
	{
		m_isDown = true;
		m_isClicked = false;
		SetScale(m_scaleWhenPointerDown);
	}
	if (m_inputManager->IsPointerUp() && m_isDown)
	{
			m_isDown = false;
			m_isClicked = true;
			SetScale(m_scaleWhenPointerUp);
	}
}

void Button::Draw(Matrix &vpMatrix, Vector3 ucolor)
{
	Object::Draw(vpMatrix);
}