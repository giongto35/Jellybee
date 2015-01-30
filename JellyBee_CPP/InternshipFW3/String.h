#pragma once
#include "object.h"
#include "model.h"
#include "platforms.h"

class String
{
private:
	static String *s_Instance;
	Object *m_ObjChar;
	Model *m_quad;
	Texture *m_bitmap;
	Shader *m_shader;
	String(void);
	~String(void);

public:
	static String* GetInstance();
	static void DestroyInstance();

	void Init();

	void Draw();

	void DrawString(const char *string, Vector3 position, Vector3 scale, Vector3 color = Vector3(1.0f, 1.0f, 1.0f));
};
