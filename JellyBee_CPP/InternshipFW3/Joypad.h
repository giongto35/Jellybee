#pragma once
#include "inputmanager.h"
#include "object.h"
#include "Framebuffer.h"
#include "ScreenResolution.h"
#include "ColorDataConverter.h"

#define JPSENSITIVE_INACTIVE_AREA 0.5f

#define JOY_DEFAULT 0
#define JOY_DRAGGING 1

// JOYPAD INPUT
class Joypad
{
public:
	Joypad(
			Vector2 position, 
			float padSize			= 0.1f, 
			float buttonSize		= 0.04f, 
			float padActiveRatio	= 1.5f
		  );

	~Joypad(void);

	void Update();
	void Draw();

	// Output getter
	int GetButtonState();
	float GetButtonMovement();

protected:
	Object* m_pad;
	Object* m_button;
	Object* m_padActiveArea;

	// 2D position (x, y)
	Vector2 m_position;

	// Pad
	float m_padSize;
	Vector3 m_padColor;

	// Button
	float m_buttonSize;
	Vector3 m_buttonColor;

	// Output
	int m_type;
	int m_degree;

	float m_joyX;
	float m_joyY;

	Framebuffer* m_Framebuffer;
	InputManager* m_inputManager;
	ScreenResolution* m_screenResolution;
	float m_screenRatio;
};
