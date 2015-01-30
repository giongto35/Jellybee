#pragma once
#include "State.h"
#include "ResourcesManager.h"
#include "Texture.h"
#include "Model.h"
#include "Shader.h"
#include "Object.h"

class Loading :
	public State
{
private:
	static Loading* s_Instance;

	char *m_resManPath;
	int m_total;
	int m_processing;
	int* m_types;
	ResourcesManager* rm;
	
	// Loading Scene
	GLint m_numOfTextures, m_numOfModels, m_numOfShaders, m_numOfObjects;
	Texture **m_textures;
	Model **m_models;
	Shader **m_shaders;
	Object **m_objects;

public:
	void Init(const char *id, const char *resManPath);
	void Start();
	void Draw();
	void Update();
	void Destroy();
	float GetLoadedPercent();

	// SINGLETON HANDLING
	static Loading* GetInstance();
	static void DestroyInstance();
private:
	Loading(void);
	~Loading(void);
	Texture* GetTextureById(const char* id);
};
