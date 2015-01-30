#pragma once
#include "object.h"
#include "InputManager.h"

class Button :
	public Object
{
public:
	bool m_isClicked;
	int m_width;
	int m_height;
	int m_topleftX;
	int m_topleftY;

	InputManager *m_inputManager;
	Vector3 m_scaleWhenPointerDown;
	Vector3 m_scaleWhenPointerUp;
	bool m_isDown;

public:
	Button(void);
	~Button(void);
	void Init(const char * id, const Vector3 & pos, const Vector3 & rot, const Vector3 & sca, Texture* texture,  Model* model, Shader* shader, const char* textureUvId = 0);
	bool IsClicked();
	void Draw(Matrix &vpMatrix, Vector3 ucolor = Vector3(1, 1, 1));
	void Update();
};
