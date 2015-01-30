#pragma once
#include "Platforms.h"
#include <cstring>

class InputManager
{
private:
	static InputManager* s_Instance;
	bool m_keyCode[MAX_256];
	int m_x, m_y, m_type;
	int m_dx, m_dy;
	bool m_isDown;

private:
	InputManager(void);
	~InputManager(void);
public:
	void Init();
	static InputManager* GetInstance();
	static void DestroyInstance();
	// events handler
	void OnKey(int keyCode, bool isKeyDown);
	void OnPointer(int type, int x, int y);
	void OnDrag(int x, int y);
	
	int GetX();
	int GetY();
	int GetDx();
	int GetDy();
	bool IsKeyDown(int keyCode);
	bool IsPointerDown();
	bool IsPointerUp();
	bool IsClicked();
	bool IsDrag();
	bool IsPointerOnRect(int x, int y, int width, int height);
};
