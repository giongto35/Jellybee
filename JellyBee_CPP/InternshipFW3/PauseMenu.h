#pragma once
#include "InputManager.h"

class Button;
class Object;

class PauseMenu
{
private:
	static PauseMenu *s_Instance;
	bool m_isVisible;
	PauseMenu(void);
	~PauseMenu(void);

	Button **m_button;
	int m_numOfButton;
	Object *m_ground;

	InputManager* m_inputManager;

public:
	bool IsVisible();
	void SetVisible(bool visible = false);
	void Init();
	void Update();
	void Draw();
	static PauseMenu* GetInstance();
	static void DestroyInstance();
};
