#pragma once

#include "Object.h"

class Number
{
private:
	Number(void);
	~Number(void);

public:
	static Number *s_Instance;
	static Number* GetInstance();
	static void DestroyInstance();

	void Init();

	// GETTERS & SETTERS
	int GetNumber();
	void SetNumber(int n);
	void SetPosition(Vector3 position);

	// METHODS
	void Draw();

protected:
	
	// ATTRIBUTES
	int m_number;
	Vector3 m_position;

	int m_nChar;
	int m_charArr[MAX_NUMBER_CHARACTERS];

	Object** m_charObj;

	// METHODS
	void SplitNumber();

	Model* m_model;
	Shader* m_shader;
	Texture* m_texture;
};
