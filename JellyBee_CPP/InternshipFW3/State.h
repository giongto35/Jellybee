#pragma once
#include "Platforms.h"
#include "ScenesManager.h"
#include "Camera.h"
#include "InputManager.h"
#include "Framebuffer.h"

class State
{
public:
	ScenesManager *m_sceneManager;
	InputManager *m_inputManager;
	char m_id[MAX_10];
	char *m_sceneFilePath;
	int m_status;
	Camera *m_sceneCamera;
	Framebuffer *m_Framebuffer;

public:
	State();
	virtual ~State(void);

	virtual void Init(const char* id, const char* path);
	virtual void Start();
	virtual void Update();
	virtual void Draw();
	virtual void Destroy();

	// SETTERS

	// GETTERS
	const char* GetId();
	
};