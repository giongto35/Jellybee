#pragma once
#include "Shader.h"
#include "Texture.h"
#include "Framebuffer.h"
#include "ResourcesManager.h"
#include "ScenesManager.h"
#include "StatesManager.h"
#include "Object.h"
#include "InGame.h"
#include <map>

using namespace std;

struct Pass
{
	char shaderID[10];
	Texture* texture[3];
	int fboId;
	Shader* shader;
	float otherData;
};

struct Effect
{
	char ID[10];
	int numPass;
	Pass p[10];
	int firstFboId;
};

class EffectManager
{
private:
	static EffectManager* s_Instance;
	Effect* m_effect;
	int m_numEffect;
	Effect GetEffect(char* effectName);
	ResourcesManager* RM;
	ScenesManager* SM;
public:
	Framebuffer m_fbo[5];
	bool m_drawShadow;
	static EffectManager* GetInstance();//for singleton
	static void DestroyInstance();
	void DrawEffect(char* ID);
	void Load(FILE* pfile);

	EffectManager(void);
	~EffectManager(void);
};
