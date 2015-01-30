#include "Loading.h"
#include "ResourcesManager.h"
#include <cstring>
#include "String.h"
#include "MainCharacter.h"
#include "PauseMenu.h"
#include "GameOver.h"
#include "SoundManager.h"
#include "FileManager.h"
#include "Timer.h"
#include "NameSpace.h"

Loading::Loading(void)
:State()
{
	m_resManPath = 0;
	m_processing = 0;
	m_total = 0;
	m_types = 0;

	rm = 0;

	m_numOfTextures = 0;
	m_numOfModels = 0;
	m_numOfShaders = 0;
	m_numOfObjects = 0;

	m_textures = 0;
	m_models = 0;
	m_shaders = 0;
	m_objects = 0;
}

float Loading::GetLoadedPercent()
{
	float percent = ((float)m_processing / (float)m_total) * 100.0f;
	return percent;
}

void Loading::Init(const char *id, const char *resManPath)
{
	strcpy(m_id, id);

	m_resManPath = new char[strlen(resManPath) + 1];
	strcpy(m_resManPath, resManPath);

	// Read (init resource infomation)
	rm = ResourcesManager::GetInstance();
	rm->Init(m_resManPath);

	// Init Loading Scene
	const char* fileLoadingState = "Managers/LoadingState.txt";
	FILE* f = FileManager::GetInstance()->Open(fileLoadingState, "r");

	if (f == NULL)
	{
		LOGE("Can not open %s file\n", fileLoadingState);
		return;
	}

	char _id[SIZE_OF_ID];
	char path[100];	// is also pathVS
	char wrap[8];
	char pathFS[100];
	char texture_uv_path[SIZE_OF_STRING];
	char state[10];
	int isReusable;
	int N, M;

	// MODELS
	fscanf(f, "#Model: %d\n", &N);
	m_numOfModels = N;
	m_models = new Model* [m_numOfModels];
	SKIP_ONE_LINE(f);
	
	// NFG Model
	fscanf(f, "#Model NFG: %d\n", &N);
	for(int i = 0; i < N; ++i)
	{
		fscanf(f, "%s %s %d\n", _id, path, &isReusable);
		m_models[i] = new Model();
		m_models[i]->Init(path, _id, GL_DYNAMIC_DRAW, NFG, isReusable);
	}
	// OBJ Model
	fscanf(f, "#Model OBJ: %d\n", &M);
	for(int i = 0; i < M; ++i)
	{
		fscanf(f, "%s %s %d\n", _id, path, &isReusable);
		m_models[i + N] = new Model();		
		m_models[i + N]->Init(path, _id, GL_DYNAMIC_DRAW, OBJ, isReusable);
	}

	// TEXTURES
	fscanf(f, "#Texture: %d\n", &N);
	m_numOfTextures = N;
	m_textures = new Texture*[m_numOfTextures];

	SKIP_ONE_LINE(f);
	// Texture2D
	fscanf(f, "Texture2D: %d\n", &N);
	for(int i = 0; i < N; ++i)
	{
		// id - path - wrapMode
		fscanf(f, "%s %s %s", _id, path, wrap);
		m_textures[i] = new Texture();
		if(strcmp(wrap, "REPEAT") == 0)
		{
			m_textures[i]->Init(path, _id, GL_TEXTURE_2D, GL_REPEAT);
		}
		else if(strcmp(wrap, "CLAMP") == 0)
		{
			m_textures[i]->Init(path, _id, GL_TEXTURE_2D, GL_CLAMP_TO_EDGE);
		}
		else if(strcmp(wrap, "MR_REP") == 0)
		{
			m_textures[i]->Init(path, _id, GL_TEXTURE_2D, GL_MIRRORED_REPEAT);
		}
		else
		{
			LOGE("ERROR: unexpect value of WRAP_MODE (please check ResourcesManager.txt)\n");
		}

		// texture_uv_path
		fscanf(f, "%s\n", texture_uv_path);
		if (strcmp(texture_uv_path, "NULL") != 0)
		{
			m_textures[i]->GetTextureUvList(texture_uv_path);
		}
	}	
	// TextureCube
	fscanf(f, "TextureCube: %d\n", &M);
	for(int i = 0; i < M; ++i)
	{
	}

	// SHADERS
	fscanf(f, "#Shader: %d\n", &N);
	m_numOfShaders = N;
	m_shaders = new Shader* [m_numOfShaders];
	fscanf(f, "%*s %*s %*s %*s\n");

	for(int i = 0; i < m_numOfShaders; ++i)
	{
		fscanf(f, "%s %s %s", _id, path, pathFS);
		m_shaders[i] = new Shader();
		m_shaders[i]->Init(_id, path, pathFS);

		fscanf(f, "%d: ", &M);
		for(int j = 0; j < M; j++)
		{
			fscanf(f, "%s", state);
			if(strcmp(state, "CULL") == 0)
			{
				m_shaders[i]->AddState(GL_CULL_FACE);
			}
			else if(strcmp(state, "DEPTH") == 0)
			{
				m_shaders[i]->AddState(GL_DEPTH_TEST);
			}
			else if(strcmp(state, "BLEND") == 0)
			{
				m_shaders[i]->AddState(GL_BLEND);
			}
			else
			{
				LOGE("ERROR: unexpect value of Shader's state (please check ResourcesManager.txt)\n");
			}	
		}

		fscanf(f, "\n");
	}

	//OBJECT
	int model;	
	char texture[SIZE_OF_ID];
	Texture* textures[MAX_TEX];
	int shader;
	char texture_uv[SIZE_OF_ID];
	Vector3 pos, rot, sca;
	int numTex;

	fscanf(f, "#Object: %d\n", &N);
	m_numOfObjects = N;
	m_objects = new Object* [m_numOfObjects ];

	if (N > 0)
	{
		for(int i = 0; i < N; ++i)
		{
			fscanf(f, "ID: %s\n", _id);
			fscanf(f, "MODEL: %d\n", &model);

			fscanf(f, "TEXTURE: %d", &numTex);
			for (int j = 0; j < numTex; ++j)
			{
				fscanf(f, "%s", texture);
				textures[j] = GetTextureById(texture);
			}
			fscanf(f, "\n");

			fscanf(f, "SHADER: %d\n", &shader);
			fscanf(f, "POSITION: %f, %f, %f\n", &pos.x, &pos.y, &pos.z);
			fscanf(f, "ROTATION: %f, %f, %f\n", &rot.x, &rot.y, &rot.z);
			fscanf(f, "SCALE: %f, %f, %f\n", &sca.x, &sca.y, &sca.z);

			m_objects[i] = new Object();
			m_objects[i]->Init(_id, pos, rot, sca, numTex, textures, m_models[model], m_shaders[shader]);
			// textureUV
			fscanf(f, "TEXTURE_UV: %s\n", texture_uv);
			if(strcmp(texture_uv, "NULL") != 0)
			{
				m_objects[i]->SetTextureUV(texture_uv);
			}
			else
			{
				
			}
		}
	}

	fclose(f);
}

void Loading::Start()
{
	m_status = ON_GOING;
	rm->m_models		= new Model* [rm->m_numOfModels];
	rm->m_textures		= new Texture*[rm->m_numOfTextures + rm->m_numOfFramebuffer];
	rm->m_Framebuffer	= new Framebuffer*[rm->m_numOfFramebuffer];
	rm->m_shaders		= new Shader* [rm->m_numOfShaders];
	rm->m_map			= new Map* [rm->m_numOfMap];

	m_types = new int[5];

	m_total += rm->m_numOfFramebuffer;
	m_types[0] = m_total;

	m_total += rm->m_numOfMap;
	m_types[1] = m_total;

	m_total += rm->m_numOfModels;
	m_types[2] = m_total;

	m_total += rm->m_numOfShaders;
	m_types[3] = m_total;

	m_total += rm->m_numOfTextures;
	m_types[4] = m_total;
}

void Loading::Draw()
{
	if (m_processing <= m_total) 
	{
		LOGI("Loaded %.2f%%\n", GetLoadedPercent());
	}
	
	Matrix matrixI;
	m_objects[1]->m_otherData = (float)m_processing / m_total;
	for (int i = 0; i < m_numOfObjects; ++i)
	{
		m_objects[i]->Draw(matrixI);
	}
}

void Loading::Update()
{	
	if (m_processing == 20)
	{
		m_objects[1]->SetTexture(GetTextureById("load2"));
	}
	if (m_processing == 30)
	{
		m_objects[1]->SetTexture(GetTextureById("load3"));
	}

	if (m_processing < m_total)
	{
		// FRAMEBUFFER
		if (m_processing >= 0 && m_processing < m_types[0])
		{
			rm->m_Framebuffer[m_processing] = new Framebuffer;
			rm->m_Framebuffer[m_processing]->Init(
				rm->fbList[m_processing].id
			);

			rm->m_textures[rm->m_numOfTextures] = new Texture();
			rm->m_textures[rm->m_numOfTextures++]->Init(
				rm->fbList[m_processing].id, 
				rm->m_Framebuffer[m_processing]->GetColorTextureId()
			);
		}

		// MAP
		else if (m_processing >= m_types[0] && m_processing < m_types[1])
		{
			int i = m_processing - m_types[0];
			rm->m_map[i] = new Map();
			rm->m_map[i]->LoadMap(rm->mapList[i].id, rm->mapList[i].path);
		}

		// MODEL
		else if (m_processing >= m_types[1] && m_processing < m_types[2])
		{
			int i = m_processing - m_types[1];
			if (strcmp(rm->modelList[i].type, "NFG") == 0)
			{
				rm->m_models[i] = new Model();
				rm->m_models[i]->Init(
					rm->modelList[i].path, 
					rm->modelList[i].id, 
					GL_DYNAMIC_DRAW, 
					NFG,
					rm->modelList[i].isReusable
				);
			}
			else
			{
				rm->m_models[i] = new Model();
				rm->m_models[i]->Init(
					rm->modelList[i].path, 
					rm->modelList[i].id, 
					GL_DYNAMIC_DRAW, 
					OBJ,
					rm->modelList[i].isReusable
				);
			}
		}

		// SHADER
		else if (m_processing >= m_types[2] && m_processing < m_types[3])
		{
			int i = m_processing - m_types[2];

			rm->m_shaders[i] = new Shader();
			rm->m_shaders[i]->Init(
				rm->shadList[i].id, 
				rm->shadList[i].vsPath, 
				rm->shadList[i].fsPath
			);

			for(int j = 0; j < rm->shadList[i].numOfState; j++)
			{
				if(strcmp(rm->shadList[i].states[j], "CULL") == 0)
				{
					rm->m_shaders[i]->AddState(GL_CULL_FACE);
				}
				else if(strcmp(rm->shadList[i].states[j], "DEPTH") == 0)
				{
					rm->m_shaders[i]->AddState(GL_DEPTH_TEST);
				}
				else if(strcmp(rm->shadList[i].states[j], "BLEND") == 0)
				{
					rm->m_shaders[i]->AddState(GL_BLEND);
				}
				else
				{
					LOGE("ERROR: unexpect value of Shader's state (please check ResourcesManager.txt)\n");
				}	
			}
		}

		// TEXTURE
		else if (m_processing >= m_types[3] && m_processing < m_types[4])
		{
			int i = m_processing - m_types[3];

			if (strcmp(rm->texList[i].type, "2D") == 0)
			{
				rm->m_textures[i] = new Texture();
				if(strcmp(rm->texList[i].wrapMode, "REPEAT") == 0)
				{
					rm->m_textures[i]->Init(
						rm->texList[i].path, 
						rm->texList[i].id, 
						GL_TEXTURE_2D, 
						GL_REPEAT
					);
				}
				else if(strcmp(rm->texList[i].wrapMode, "CLAMP") == 0)
				{
					rm->m_textures[i]->Init(
						rm->texList[i].path, 
						rm->texList[i].id, 
						GL_TEXTURE_2D, 
						GL_CLAMP_TO_EDGE
					);
				}
				else if(strcmp(rm->texList[i].wrapMode, "MR_REP") == 0)
				{
					rm->m_textures[i]->Init(
						rm->texList[i].path, 
						rm->texList[i].id, 
						GL_TEXTURE_2D, 
						GL_MIRRORED_REPEAT
					);
				}
				else
				{
					LOGE("ERROR: unexpect value of WRAP_MODE (please check ResourcesManager.txt)\n");
				}
				
				if (strcmp(rm->texList[i].textureUVPath, "NULL") != 0)
				{
					rm->m_textures[i]->GetTextureUvList(rm->texList[i].textureUVPath);
				}
			}
			else // CUBE
			{
				rm->m_textures[i] = new Texture();
				if(strcmp(rm->texList[i].wrapMode, "REPEAT") == 0)
				{
					rm->m_textures[i]->Init(
						rm->texList[i].path, 
						rm->texList[i].id, 
						GL_TEXTURE_CUBE_MAP, 
						GL_REPEAT
					);
				}
				else if(strcmp(rm->texList[i].wrapMode, "CLAMP") == 0)
				{
					rm->m_textures[i]->Init(
						rm->texList[i].path, 
						rm->texList[i].id, 
						GL_TEXTURE_CUBE_MAP, 
						GL_CLAMP_TO_EDGE
					);
				}
				else if(strcmp(rm->texList[i].wrapMode, "MR_REP") == 0)
				{
					rm->m_textures[i]->Init(
						rm->texList[i].path, 
						rm->texList[i].id, 
						GL_TEXTURE_CUBE_MAP, 
						GL_MIRRORED_REPEAT
					);
				}
				else
				{
					LOGE("ERROR: unexpect value of WRAP_MODE (please check ResourcesManager.txt)\n");
				}	
				
				if (strcmp(rm->texList[i].textureUVPath, "NULL") != 0)
				{
					rm->m_textures[i]->GetTextureUvList(rm->texList[i].textureUVPath);
				}
			}
		}
		
		
	}
	else
	{
		// SOUND MANAGER
		SoundManager::GetInstance()->Init("Managers/SoundManager.txt");

		// MAINCHARACTER
		MainCharacter::GetInstance()->Init("Managers/MainCharacter.txt");

		String::GetInstance()->Init();
		PauseMenu::GetInstance()->Init();
		GameOver::GetInstance()->Init();

		//String::GetInstance()->DrawString("hello world", Vector3(0.0f, 0.0f, 0.0f), Vector3(2, 2, 2));
		SLEEP(1000);

		m_status = FINISHED;
		//Setting::isBackButtonPressed = false;
	}

	++m_processing;
}

//---------------------------------------------------
/// GetInstance
//---------------------------------------------------
Loading* Loading::s_Instance = NULL;
Loading* Loading::GetInstance()
{
	if (!s_Instance)
	{
		s_Instance = new Loading();
	}
	return s_Instance;
}

//---------------------------------------------------
/// Destroy
//---------------------------------------------------
void Loading::DestroyInstance()
{
	SAFE_DEL(s_Instance);
}

void Loading::Destroy()
{
	State::Destroy();
	SAFE_DEL(m_resManPath);
	SAFE_DEL_ARRAY(m_types);

	SAFE_DEL_ARRAY(rm->modelList);
	SAFE_DEL_ARRAY(rm->texList);
	SAFE_DEL_ARRAY(rm->shadList);
	SAFE_DEL_ARRAY(rm->mapList);
	SAFE_DEL_ARRAY(rm->fbList);

	for (int i = 0; i < m_numOfTextures; ++i)
	{
		SAFE_DEL(m_textures[i]);
	}
	SAFE_DEL_ARRAY(m_textures);

	for (int i = 0; i < m_numOfModels; ++i)
	{
		SAFE_DEL(m_models[i]);
	}
	SAFE_DEL_ARRAY(m_models);

	for (int i = 0; i < m_numOfShaders; ++i)
	{
		SAFE_DEL(m_shaders[i]);
	}
	SAFE_DEL_ARRAY(m_shaders);

	for (int i = 0; i < m_numOfObjects; ++i)
	{
		SAFE_DEL(m_objects[i]);
	}
	SAFE_DEL_ARRAY(m_objects);
}

Loading::~Loading(void)
{
	
}

Texture* Loading::GetTextureById(const char* id)
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