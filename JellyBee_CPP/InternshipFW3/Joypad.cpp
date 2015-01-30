#include "Joypad.h"
#include "ResourcesManager.h"
#include "StatesManager.h"
#include "ScreenResolution.h"
#include <cmath>

//---------------------------------------------------------------------------------------------------
/// GetButtonState
/// @return -1 if JOY_DEFAULT otherwise return joypad degree value, ranged 0-360 (JOY_DRAGGING)
//---------------------------------------------------------------------------------------------------
int Joypad::GetButtonState()
{
	if (	m_type == JOY_DRAGGING 
		&&	
			GetButtonMovement() >= JPSENSITIVE_INACTIVE_AREA
	   )
	{
		return m_degree;
	}

	return -1;
}

float Joypad::GetButtonMovement()
{
	return sqrt(m_joyX*m_joyX + m_joyY*m_joyY);
}

//---------------------------------------------------------------------------------------------------
/// Update
//---------------------------------------------------------------------------------------------------
static bool stop = false;
void Joypad::Update()
{
	// Bind Framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer->GetFboId());

	static bool isDown = false;
	// Mouse/touch up
	if (isDown && this->m_inputManager->IsPointerUp())
	{
		isDown = false;
		
		m_button->SetPosition(Vector3(m_position.x, m_position.y, 0.0f));
		//float x = (float) this->m_inputManager->GetX();
		//float y = (float) this->m_inputManager->GetY();

		this->m_type = JOY_DEFAULT;
	}
	// Mouse down or drag
	if (this->m_inputManager->IsDrag() || this->m_inputManager->IsPointerDown())
	{
		isDown = true;
		unsigned int data;
		// Get color at the clicked pixel
		glReadPixels(	this->m_inputManager->GetX(), 
						this->m_screenResolution->m_screenHeight - this->m_inputManager->GetY(), 
						1, 
						1, 
						GL_RGBA, 
						GL_UNSIGNED_BYTE, 
						&data
					);

		// If the clicked pixel is in the active area of joypad
		ColorDataConverter color, color1;
		color1.r = 0;
		color1.g = 0;
		color1.b = 0;
		color1.a = 255;
		color.iValue = data;

		if (color.iValue != color1.iValue)
		{
			// Convert pixel value to O(x, y) coord value
			Vector2 O(m_screenResolution->m_screenWidth / 2.0f, m_screenResolution->m_screenHeight / 2.0f);
			int x = (int) ((m_inputManager->GetX() > O.x)? (m_inputManager->GetX() - O.x): (O.x - m_inputManager->GetX()));
			int y = (int) ((m_inputManager->GetY() > O.y)? (m_inputManager->GetY() - O.y): (O.y - m_inputManager->GetY()));
			int dx = (int) ((m_inputManager->GetX() > O.x)? 1: -1);
			int dy = (int) ((m_inputManager->GetY() > O.y)? -1: 1);
			float worldX = (float) ((x * dx + 0.0) / O.x);
			float worldY = (float) ((y * dy + 0.0) / O.y);

			// Stick the joy button to mouse pointer
			m_button->SetPosition(Vector3(worldX, worldY, 0.0f));

			// Move the coords from O(0, 0) to O' (posx, posy)
			O.x = m_position.x;
			O.y = m_position.y;
			float x2 = (worldX > O.x)? (worldX - O.x): (O.x - worldX);
			float y2 = (worldY > O.y)? (worldY - O.y): (O.y - worldY);
			float dx2 = (worldX > O.x)? m_padSize: -m_padSize;
			float dy2 = (worldY > O.y)? m_padSize*m_screenRatio: -m_padSize*m_screenRatio;

			// Set output
			m_joyX = x2 / dx2;
			m_joyY = y2 / dy2;

			// Calc the bound point:
			// (in case the pointer is out of bound, the put the pointer in the circle bound)
			// Get the current joy's coords' ratio
			// Put in circle equation to find the correspond projection of joy's coords on the circle
			if (m_joyX*m_joyX + m_joyY*m_joyY > 1)
			{
				float _x = 0, _y = 0;

				if (m_joyY != 0)
				{
					float ratio = m_joyX/m_joyY;

					_y = sqrt(1 / (1 + ratio * ratio));
					_x = _y * ratio;

					if (m_joyY < 0)
					{
						_x = -_x;
						_y = -_y;
					}
				}
				else
				{
					float ratio = m_joyY/m_joyX;

					_x = sqrt(1 / (1 + ratio * ratio));
					_y = _x * ratio;

					if (m_joyX < 0)
					{
						_x = -_x;
						_y = -_y;
					}
				}

				x2 = _x * m_padSize;
				y2 = _y * m_padSize * m_screenRatio;

				// Move the coords from O'(posx, posy) to O(0, 0)		
				worldX = O.x + x2;
				worldY = O.y + y2;

				// Set the button pos to the calculated point
				m_button->SetPosition(Vector3(worldX, worldY, 0.0f));

				// Set output to the calculated point
				m_joyX = _x;
				m_joyY = _y;
			}

			// Set output information
			this->m_type = JOY_DRAGGING;

			if (m_joyX == 0)
			{
				if (m_joyY > 0)
				{
					this->m_degree = 90;
				}
				else
				{
					this->m_degree = 270;
				}
			}
			else
			{
				this->m_degree = (int) (atan(m_joyY / m_joyX) * 180 / PI);
				// resolve tan
				if (m_joyX < 0) 
				{
					this->m_degree += 180;
				}
				else if (m_joyY < 0) 
				{
					this->m_degree += 360;
				}
			}
		}
	}
	
	if (GetButtonState() != -1)
	{
		// RIGHT
		if (m_degree >= 305 && m_degree <= 360 || m_degree >= 0 && m_degree <= 45)
		{
			m_inputManager->OnKey(ARROW_RIGHT, true);
		}
		else
		{
			m_inputManager->OnKey(ARROW_RIGHT, false);
		}

		// LEFT
		if (m_degree >= 135 && m_degree <= 225)
		{
			m_inputManager->OnKey(ARROW_LEFT, true);
		}
		else
		{
			m_inputManager->OnKey(ARROW_LEFT, false);
		}

		// UP
		if (m_degree >= 45 && m_degree <= 135)
		{
			m_inputManager->OnKey(ARROW_UP, true);
		}
		else
		{
			m_inputManager->OnKey(ARROW_UP, false);
		}

		// DOWN
		if (m_degree >= 225 && m_degree <= 305)
		{
			m_inputManager->OnKey(ARROW_DOWN, true);			
		}
		else
		{
			m_inputManager->OnKey(ARROW_DOWN, false);
		}

		stop = true;
	}
	else
	{
		if (stop == true)
		{
			m_inputManager->OnKey(ARROW_RIGHT, false);
			m_inputManager->OnKey(ARROW_UP, false);
			m_inputManager->OnKey(ARROW_DOWN, false);
			m_inputManager->OnKey(ARROW_LEFT, false);
			stop = false;
		}
		
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//---------------------------------------------------------------------------------------------------
/// Draw
//---------------------------------------------------------------------------------------------------
void Joypad::Draw()
{
	Matrix joypad_vp;

	/*glEnable(GL_BLEND);
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );*/
	m_button->Draw(joypad_vp);
	m_pad->Draw(joypad_vp);
	//glDisable(GL_BLEND);

}

//---------------------------------------------------------------------------------------------------
/// Constructors
//---------------------------------------------------------------------------------------------------
Joypad::Joypad(Vector2 position, 
			   float padSize, 
			   float buttonSize, 
			   float padActiveRatio)
{
	ResourcesManager* rm = ResourcesManager::GetInstance();
	this->m_screenResolution = ScreenResolution::GetInstance();
	this->m_inputManager = InputManager::GetInstance();
	ScreenResolution* screen = ScreenResolution::GetInstance();

	// Set member
	m_position = position;
	m_padSize = padSize;
	m_buttonSize = buttonSize;
	m_screenRatio = screen->GetRatio();

	m_type = JOY_DEFAULT;
	m_degree = -1;	

	// Init Framebuffer
	m_Framebuffer = new Framebuffer();
	m_Framebuffer->Init();

	// Init pad
	m_pad = new Object();
	m_pad->Init("joy_pad", Vector3(position.x, position.y, 0.0f), 
				Vector3(0.0f, 0.0f, 0.0f), 
				Vector3(padSize, padSize * m_screenRatio, padSize), 
				rm->GetTextureById("bigTex"),
				rm->GetModelById("quad"),
				rm->GetShaderById("staticObj"),
				"joypad.tga");

	// Init button
	m_button = new Object();
	m_button->Init("joy_b", Vector3(position.x, position.y, 0.0f), 
				Vector3(0.0f, 0.0f, 0.0f), 
				Vector3(buttonSize, buttonSize * m_screenRatio, buttonSize),  
				rm->GetTextureById("bigTex"),
				rm->GetModelById("quad"), 
				rm->GetShaderById("staticObj"),
				"joybutton.tga");

	// Init active area
	m_padActiveArea = new Object();
	m_padActiveArea->Init("area", Vector3(position.x, position.y, 0.0f), 
						  Vector3(0.0f, 0.0f, 0.0f), 
						  Vector3(padSize*padActiveRatio, padSize*padActiveRatio*m_screenRatio, padSize*padActiveRatio), 
						  NULL,
						  rm->GetModelById("circle"), 
						  rm->GetShaderById("sShad") );



	//// FRAME BUFF
	Matrix joypad_vp;
	Vector4 joypad_area_color(1.0f, 0.0f, 0.0f, 1.0f);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Framebuffer->GetFboId());

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	m_padActiveArea->Draw(joypad_vp, joypad_area_color);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//---------------------------------------------------------------------------------------------------
/// Destructor
//---------------------------------------------------------------------------------------------------
Joypad::~Joypad(void)
{
	SAFE_DEL(m_pad);
	SAFE_DEL(m_button);
	SAFE_DEL(m_padActiveArea);
	SAFE_DEL(m_Framebuffer);
}