#pragma once
#include "Platforms.h"

#include "Model.h"
#include "Texture.h"
#include "Shader.h"
#include "Map.h"
#include "Framebuffer.h"

struct ModelInfo
{
	char type[SIZE_OF_ID]; // NFG, OBJ
	char id[SIZE_OF_ID];
	char path[SIZE_OF_STRING];
	int isReusable;
};

struct FrameBufferInfo
{
	char id[SIZE_OF_ID];
};

struct TextureInfo
{
	char type[SIZE_OF_ID]; // 2D, CUBE
	char id[SIZE_OF_ID];
	char path[SIZE_OF_STRING];
	char wrapMode[SIZE_OF_ID];
	char textureUVPath[SIZE_OF_STRING];
};

struct ShaderInfo
{
	char id[SIZE_OF_ID];
	char vsPath[SIZE_OF_STRING];
	char fsPath[SIZE_OF_STRING];
	int numOfState;
	char states[3][SIZE_OF_ID];
};

struct MapInfo
{
	char id[SIZE_OF_ID];
	char path[SIZE_OF_STRING];
};

class ResourcesManager
{
private:
	static ResourcesManager* s_Instance;
public:
	Model** m_models;
	Texture** m_textures;
	Shader** m_shaders;
	Map** m_map;
	Framebuffer** m_Framebuffer;
	GLint m_numOfModels, m_numOfTextures, m_numOfShaders, m_numOfMap, m_numOfFramebuffer;

	ModelInfo* modelList;
	TextureInfo* texList;
	ShaderInfo* shadList;
	MapInfo* mapList;
	FrameBufferInfo* fbList;

private:
	ResourcesManager(void);
	~ResourcesManager(void);
public:
	void Init(const char* file);
	Model* GetModelById(const char* id);
	Texture* GetTextureById(const char* id);
	Shader* GetShaderById(const char* id);
	Map* GetMapById(const char* id);
	Framebuffer* GetFramebufferById(const char* id);
	static ResourcesManager* GetInstance();
	static void DestroyInstance();

};
