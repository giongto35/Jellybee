#pragma once
#include "Platforms.h"
#include "InputManager.h"

class Button;
class Object;

class GameOver
{
private:
	static GameOver *s_Instance;
	bool m_isVisible;
	GameOver(void);
	~GameOver(void);

	Button **m_button;
	int m_numOfButton;
	Object *m_ground;
	int m_bestScore;

	InputManager* m_inputManager;

public:
	bool IsVisible();
	void SetVisible(bool visible = false);
	void Init();
	void Update();
	void Draw();
	static GameOver* GetInstance();
	static void DestroyInstance();
};
