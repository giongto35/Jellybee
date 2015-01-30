#pragma once
#include "Platforms.h"
#include "state.h"
#include "ColorDataConverter.h"
#include "Button.h"

class MainMenu :
	public State
{
	static MainMenu *s_Instance;
	GLuint m_fboId;
	GLuint m_depthTextureId, m_colorTextureId;
	ColorDataConverter m_unionColor;
	unsigned char m_pixelData[4];

	Button **m_button;
	Object *m_info;
	Object *m_help;
	Object *m_panelGround;
	Object *m_ground;
	int m_numOfButton;

	MainMenu(void);
	~MainMenu(void);

public:
	static MainMenu* GetInstance();
	static void DestroyInstance();
	void OnButtonClick(int index);

	virtual void Start();
	virtual void Update();
	virtual void Draw();
	virtual void Destroy();
	Button* ButtonDetection(int a);
};
