#include "ResourcesManager.h"
#include "FileManager.h"
#include <string.h>

//---------------------------------------------------
/// Constructor
//---------------------------------------------------
ResourcesManager::ResourcesManager(void)
{
	m_models = 0;
	m_textures = 0;
	m_shaders = 0;
	m_map = 0;

	m_numOfModels = 0;
	m_numOfTextures = 0;
	m_numOfShaders = 0;
	m_numOfMap = 0;

	m_numOfFramebuffer = 0;
	m_Framebuffer = 0;

	modelList = 0;
	texList = 0;
	shadList = 0;
	mapList = 0;
	fbList = 0;
}

//---------------------------------------------------
/// Init
//---------------------------------------------------
void ResourcesManager::Init(const char* file)
{
	FILE* f = FileManager::GetInstance()->Open(file, "r");

	if (f == NULL)
	{
		LOGE("Can not open Resource Manager file\n");
		return;
	}

	int N, M;

	// MODELS
	fscanf(f, "#Model: %d\n", &N);
	m_numOfModels = N;
	modelList = new ModelInfo[m_numOfModels];
	SKIP_ONE_LINE(f);
	
	// NFG Model
	fscanf(f, "#Model NFG: %d\n", &N);
	for(int i = 0; i < N; ++i)
	{
		fscanf(f, "%s %s %d\n", modelList[i].id, modelList[i].path, &modelList[i].isReusable);
		strcpy(modelList[i].type, "NFG");
	}
	// OBJ Model
	fscanf(f, "#Model OBJ: %d\n", &M);
	for(int i = 0; i < M; ++i)
	{
		fscanf(f, "%s %s %d\n", modelList[i + N].id, modelList[i + N].path, &modelList[i + N].isReusable);
		strcpy(modelList[i + N].type, "OBJ");
	}

	// FRAMEBUFFER
	fscanf(f, "#Framebuffer: %d\n", &m_numOfFramebuffer);
	fbList = new FrameBufferInfo[m_numOfFramebuffer];
	for (int i = 0; i < m_numOfFramebuffer; i++)
	{
		fscanf(f, "%s", fbList[i].id);
	}
	fscanf(f, "\n");

	// TEXTURES
	fscanf(f, "#Texture: %d\n", &N);
	m_numOfTextures = N;
	texList = new TextureInfo[m_numOfTextures];

	SKIP_ONE_LINE(f);
	// Texture2D
	fscanf(f, "Texture2D: %d\n", &N);
	for(int i = 0; i < N; ++i)
	{
		strcpy(texList[i].type, "2D");

		// id - path - wrapMode
		fscanf(f, "%s %s %s", texList[i].id, texList[i].path, texList[i].wrapMode);		

		// texture_uv_path
		fscanf(f, "%s\n", texList[i].textureUVPath);
	}
		
	// TextureCube
	fscanf(f, "TextureCube: %d\n", &M);
	for(int i = 0; i < M; ++i)
	{
		strcpy(texList[i + N].type, "CUBE");

		fscanf(f, "%s %s %s", texList[i + N].id, texList[i + N].path, texList[i + N].wrapMode);

		// texture_uv_path
		fscanf(f, "%s\n", texList[i + N].textureUVPath);
	}

	// SHADERS
	fscanf(f, "#Shader: %d\n", &N);
	m_numOfShaders = N;

	shadList = new ShaderInfo[m_numOfShaders];

	SKIP_ONE_LINE(f);

	for(int i = 0; i < m_numOfShaders; ++i)
	{
		fscanf(f, "%s %s %s", shadList[i].id, shadList[i].vsPath, shadList[i].fsPath);
		fscanf(f, "%d: ", &(shadList[i].numOfState));
		for(int j = 0; j < shadList[i].numOfState; j++)
		{
			fscanf(f, "%s", shadList[i].states[j]);			
		}

		fscanf(f, "\n");
	}

	// MAP
	fscanf(f, "#Map: %d\n", &N);
	m_numOfMap = N;
	mapList = new MapInfo[m_numOfMap];

	SKIP_ONE_LINE(f);

	// Texture2D
	for(int i = 0; i < N; ++i)
	{
		fscanf(f, "%s %s\n", mapList[i].id, mapList[i].path);
	}

	fclose(f);
}

//---------------------------------------------------
/// Get Model by id
//---------------------------------------------------
Model* ResourcesManager::GetModelById(const char* id)
{
	Model* model = NULL;
	for(int i = 0; i < m_numOfModels; ++i)
	{
		if(strcmp(m_models[i]->GetId(), id) == 0)
		{
			model = m_models[i];
			break;
		}
	}

	return model;
}

//---------------------------------------------------
/// Get Framebuffer by id
//---------------------------------------------------
Framebuffer* ResourcesManager::GetFramebufferById(const char* id)
{
	Framebuffer* Framebuffer = NULL;
	for (int i = 0; i < m_numOfFramebuffer; ++i)
	{
		if (strcmp(m_Framebuffer[i]->GetId(), id) == 0)
		{
			Framebuffer = m_Framebuffer[i];
			break;
		}
	}

	return Framebuffer;
}

//---------------------------------------------------
/// Get Texture by id
//---------------------------------------------------
Texture* ResourcesManager::GetTextureById(const char* id)
{
	Texture* texture = NULL;
	for(int i = 0; i < m_numOfTextures; ++i)
	{
		if(strcmp(m_textures[i]->GetId(), id) == 0)
		{
			texture = m_textures[i];
			break;
		}
	}

	return texture;
}

//---------------------------------------------------
/// Get Map by id
//---------------------------------------------------
Map* ResourcesManager::GetMapById(const char* id)
{
	Map* map = NULL;
	for(int i = 0; i < m_numOfMap; ++i)
	{
		if(strcmp(m_map[i]->GetId(), id) == 0)
		{
			map = m_map[i];
			break;
		}
	}

	return map;
}

//---------------------------------------------------
/// Get Shader by id
//---------------------------------------------------
Shader* ResourcesManager::GetShaderById(const char* id)
{
	Shader* shader = NULL;
	for(int i = 0; i < m_numOfShaders; ++i)
	{
		if(strcmp(m_shaders[i]->GetId(), id) == 0)
		{
			shader = m_shaders[i];
			break;
		}
	}

	return shader;
}

//---------------------------------------------------
/// Get Instance
//---------------------------------------------------
ResourcesManager* ResourcesManager::s_Instance = 0;
ResourcesManager* ResourcesManager::GetInstance()
{
	if (!s_Instance)
	{
		s_Instance = new ResourcesManager();
	}
	return s_Instance;
}

//---------------------------------------------------
/// Destroy Instance
//---------------------------------------------------
void ResourcesManager::DestroyInstance() 
{
	if (s_Instance)
	{
		delete s_Instance;
		s_Instance = NULL;
	}
}

//---------------------------------------------------
/// Destructor
//---------------------------------------------------
ResourcesManager::~ResourcesManager(void)
{
	for(int i = 0; i < m_numOfModels; ++i)
	{
		SAFE_DEL(m_models[i]);
	}
	SAFE_DEL_ARRAY(m_models);

	for(int i = 0; i < m_numOfTextures; ++i)
	{
		SAFE_DEL(m_textures[i]);
	}
	SAFE_DEL_ARRAY(m_textures);

	for(int i = 0; i < m_numOfShaders; ++i)
	{
		SAFE_DEL(m_shaders[i]);
	}
	SAFE_DEL_ARRAY(m_shaders);

	for(int i = 0; i < m_numOfMap; ++i)
	{
		SAFE_DEL(m_map[i]);
	}
	SAFE_DEL_ARRAY(m_map);

	for(int i = 0; i < m_numOfFramebuffer; ++i)
	{
		SAFE_DEL(m_Framebuffer[i]);
	}
	SAFE_DEL_ARRAY(m_Framebuffer);

	SAFE_DEL_ARRAY(modelList);
	SAFE_DEL_ARRAY(texList);
	SAFE_DEL_ARRAY(shadList);
	SAFE_DEL_ARRAY(mapList);
	SAFE_DEL_ARRAY(fbList);
}
