#include "EffectManager.h"

using namespace std;

EffectManager::EffectManager(void)
{
}

EffectManager::~EffectManager(void)
{
	SAFE_DEL(m_effect);
}

EffectManager* EffectManager::s_Instance = 0;

EffectManager* EffectManager::GetInstance()
{
	if (!s_Instance)
	{
		s_Instance = new EffectManager();
	}
	return s_Instance;
}

void EffectManager::DestroyInstance()
{
	SAFE_DEL(s_Instance);
}

void EffectManager::Load(FILE* pFile)
{
	RM = ResourcesManager::GetInstance();
	SM = ScenesManager::GetInstance();
	//char ID[10];
	char temp[10], shaderID[10], tex[3][10], target[10], firstFboId[10];
	//float data;
	fscanf(pFile, "#NrEffect: %d\n", &m_numEffect);
	m_effect = new Effect[m_numEffect];
	for (int i = 0; i < m_numEffect; i++)
	{
		fscanf(pFile, "\n");
		fscanf(pFile, "ID: %s\n", m_effect[i].ID);
		fscanf(pFile, "NrPass: %d\n", &m_effect[i].numPass);
		fscanf(pFile, "First: %s\n", firstFboId);
		m_effect[i].firstFboId = RM->GetFramebufferById(firstFboId)->GetFboId();
		for (int j = 0; j < m_effect[i].numPass; j++)
		{
			fscanf(pFile, "PassName: %s\n", temp);
			fscanf(pFile, "ShaderID: %s\n", shaderID);
			fscanf(pFile, "Textures: %s %s %s\n", tex[0], tex[1], tex[2]);
			fscanf(pFile, "Target: %s\n", target);
			fscanf(pFile, "OtherData: %f\n", &m_effect[i].p[j].otherData);
			if (strcmp("0", target) == 0)
				m_effect[i].p[j].fboId = 0;
			else
				m_effect[i].p[j].fboId = RM->GetFramebufferById(target)->GetFboId();
			if (strcmp(tex[0], "0") > 0)
				m_effect[i].p[j].texture[0] = RM->GetTextureById(tex[0]);
			if (strcmp(tex[1], "0") > 0)
				m_effect[i].p[j].texture[1] = RM->GetTextureById(tex[1]);
			if (strcmp(tex[2], "0") > 0)
				m_effect[i].p[j].texture[2] = RM->GetTextureById(tex[2]);
			m_effect[i].p[j].shader = RM->GetShaderById(shaderID);
		}
	}
}

Effect EffectManager::GetEffect(char* effectName)
{
	int i = 0;
	for (i = 0; i < m_numEffect; i++)
	{
		if (strcmp(m_effect[i].ID,effectName) == 0)
			break;
	}
	return m_effect[i];;
}

/*
	char shaderID[10];
	Texture* texture[3];
	Framebuffer* fbo;
	float otherData;
	*/

void EffectManager::DrawEffect(char* effectName )
{
	Object screen;
	
	Effect effect = GetEffect(effectName);
	Texture* textureTriplet[3];
	
	if (effect.firstFboId != 0)
		glBindFramebuffer(GL_FRAMEBUFFER, effect.firstFboId);

	InGame::GetInstance()->DrawAll();
	if (effect.firstFboId != 0)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);	
	
	for (int i = 0; i < effect.numPass; i++)
	{
		textureTriplet[0] = effect.p[i].texture[0];
		textureTriplet[1] = effect.p[i].texture[1];
		textureTriplet[2] = effect.p[i].texture[2];
		screen.Init("screen", Vector3(0, 0, 0), Vector3(0, 0, 0), Vector3(1.0, 1.0, 1.0), 3, textureTriplet,RM->GetModelById((char *)"quad"), effect.p[i].shader);
		screen.SetOtherData(effect.p[i].otherData);
		
		Matrix mat;
		mat.SetIdentity();
		if (effect.p[i].fboId != 0)
			glBindFramebuffer(GL_FRAMEBUFFER, effect.p[i].fboId);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		screen.Draw(mat);
		if (effect.p[i].fboId != 0)
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}