#include "ScenesManager.h"
#include <stdio.h>
#include <math.h>
#include "InputManager.h"
#include "FileManager.h"
#include "Object.h"
#include "Camera.h"
#include "ResourcesManager.h"
#include "Matrix.h"
#include "Shader.h"
#include "Timer.h"
#include "ColorDataConverter.h"
#include "Map.h"
#include "Button.h"
#include "String.h"

Camera* sceneCamera = NULL;
Shader* tmpShader = NULL;

ColorDataConverter unionObjectColor;

//---------------------------------------------------
/// Constructor
//---------------------------------------------------
ScenesManager::ScenesManager(void)
{
	m_objects = 0;
	m_cameras = 0;

	m_numOfObjects = 0;
	m_numOfCameras = 0;
	
	m_blocks = 0;
	m_numOfBlocks = 0;
}

//---------------------------------------------------
/// Init
//---------------------------------------------------
void ScenesManager::Init(const char* file)
{
	ResourcesManager* resManager = ResourcesManager::GetInstance();

	FILE* f = FileManager::GetInstance()->Open(file, "r");

	if (f == NULL)
	{
		LOGE("Can't not open Scene Manager file\n");
		return;
	}

	char id[SIZE_OF_ID];
	char model[SIZE_OF_ID];	
	char texture[SIZE_OF_ID];
	char shader[SIZE_OF_ID];
	char texture_uv[SIZE_OF_ID];
	int visible;
	Vector3 pos, rot, sca;
	int N, M, numBlock, mapHeight, mapWidth;
	bool mapConstructing = false;

	// MAP
	fscanf(f, "#Map: %s\n", id);
	numBlock = 0;
	if (strcmp(id,"0") != 0) mapConstructing  = true;
	if (mapConstructing)
	{
		m_map = resManager->GetMapById(id);
		mapHeight = m_map->GetHeight();
		mapWidth = m_map->GetWidth();
		for (int i = 0; i < mapHeight; i++)
		for (int j = 0; j < mapWidth; j++)
		{
			if (m_map->GetPoint(i,j) == TREE)
			{
				numBlock++;
			}
		}

		m_numOfBlocks = numBlock;
	}

	// OBJECTS
	fscanf(f, "#Objects: %d\n", &N);
	m_numOfObjects = N;
	m_objects = new Object* [m_numOfObjects ];
	
	// OBJECT
	fscanf(f, "#Object: %d\n", &N);
	if (N > 0)
	{
		SKIP_ONE_LINE(f);
		for(int i = 0; i < N; ++i)
		{
			// id  - model - texture - shader
			fscanf(f, "%s %s %s %s", id, model, texture, shader);
			// position - rotation - scale
			fscanf(f, "%f, %f, %f", &pos.x, &pos.y, &pos.z);
			fscanf(f, "%f, %f, %f", &rot.x, &rot.y, &rot.z);
			fscanf(f, "%f, %f, %f", &sca.x, &sca.y, &sca.z);
			// textureUV
			fscanf(f, "%s\n", texture_uv);
			m_objects[i] = new Object();
			if(strcmp(texture_uv, "NULL") == 0)
			{
				m_objects[i]->Init(id, pos, rot, sca, resManager->GetTextureById(texture), resManager->GetModelById(model), resManager->GetShaderById(shader), NULL);
			}
			else
			{
				m_objects[i]->Init(id, pos, rot, sca, resManager->GetTextureById(texture), resManager->GetModelById(model), resManager->GetShaderById(shader), texture_uv);
			}
		}
	}

	// BUTTON
	fscanf(f, "#Button: %d\n", &M);
	if (M > 0)
	{
		SKIP_ONE_LINE(f);
		for(int i = 0; i < M; ++i)
		{
			// id  - model - texture - shader
			fscanf(f, "%s %s %s %s", id, model, texture, shader);
			// position - rotation - scale
			fscanf(f, "%f, %f, %f", &pos.x, &pos.y, &pos.z);
			fscanf(f, "%f, %f, %f", &rot.x, &rot.y, &rot.z);
			fscanf(f, "%f, %f, %f", &sca.x, &sca.y, &sca.z);
			// textureUV
			fscanf(f, "%s", texture_uv);
			m_objects[i + N] = new Button();
			if(strcmp(texture_uv, "NULL") == 0)
			{
				m_objects[i + N]->Init(id, pos, rot, sca, resManager->GetTextureById(texture), resManager->GetModelById(model), resManager->GetShaderById(shader), NULL);
			}
			else
			{
				m_objects[i + N]->Init(id, pos, rot, sca, resManager->GetTextureById(texture), resManager->GetModelById(model), resManager->GetShaderById(shader), texture_uv);
			}

			fscanf(f, "%d\n", &visible);
			if (visible)
			{
				m_objects[i + N]->m_isVisible = true;
			}
			else
			{
				m_objects[i + N]->m_isVisible = false;
			}
			
		}
	}

	// Generate Tree
	srand(time(0));
	if (mapConstructing)
	{
		m_blocks = new Object* [m_numOfBlocks];
		int counter = 0;
	
		for (int i = 0; i < mapHeight; i++)
		for (int j = 0; j < mapWidth; j++)
		{
			if (m_map->GetPoint(i,j) == TREE)
			{
				char treeModelID[SIZE_OF_ID];
				float a = 0;

				sprintf(treeModelID, "Tree%d", rand()%2 + 1);
				m_blocks[counter] = new Object();

				m_blocks[counter]->Init("Tree", Vector3((j - mapWidth / 2.0f), 0 , (float)-i) * MAPFACTOR, 
					                            Vector3(0, 0, 0), 
												Vector3(0.5f, 0.5f, 0.5f), 
												resManager->GetTextureById("bigTex"),
												resManager->GetModelById(treeModelID),
												resManager->GetShaderById("objShad"),
												NULL);
				counter++;
			}
		}
		m_numOfBlocks = counter;
		LOGI("Num of blocks: %d size = %d bytes\n", m_numOfBlocks, m_numOfBlocks * sizeof(Object));
	}

	// CAMERA
	fscanf(f, "#Camera: %d\n", &N);
	m_numOfCameras = N;
	m_cameras = new Camera* [m_numOfCameras];

	fscanf(f,"%*s %*s %*s %*s %*s\n");
	GLfloat fovY, nearPlane, farPlane, speed;
	for(int i = 0; i < m_numOfCameras; ++i)
	{
		// id - fov - nearPlane - farPlane - speed
		fscanf(f, "%s %f %f %f %f", id, &fovY, &nearPlane, &farPlane, &speed);
		m_cameras[i] = new Camera();
		m_cameras[i]->Init( id, 
							Vector3(0.0f, 0.0f,0.0f), 
							fovY * PI / 180, 
							ScreenResolution::GetInstance()->GetRatio(), 
							nearPlane, 
							farPlane, 
							speed);
	}
	
	// CLOSE FILE
	fclose(f);

	/////////////
	sceneCamera = GetCameraById("sceneCam");
	tmpShader = resManager->GetShaderById("sShad");

}

//---------------------------------------------------
/// UPDATE
//---------------------------------------------------
void ScenesManager::Update()
{
	for (int i = 0; i < m_numOfObjects; ++i)
	{
		m_objects[i]->Update();
	}
}

//---------------------------------------------------
/// DRAW
//---------------------------------------------------

float Distance(Vector3 u, Vector3 v)
{
	return sqrt((u.x-v.x)*(u.x-v.x) + (u.y-v.y)*(u.y-v.y) +(u.z-v.z)*(u.z-v.z) );
}

void ScenesManager::Draw()
{
	// clear screen
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// calculate View-Project Matrix
	Matrix vpMatrix = sceneCamera->CalculateVPMatrix();

	//Object *ground = GetObjectById("ground");
	//Object *skybox = GetObjectById("skydome");
	
	// draw objects
	for(int i = 0; i < m_numOfObjects; ++i)
	{
		/*if (
			(Distance(m_objects[i]->GetPosition(),sceneCamera->GetPosition()) < 10)
			||
			(m_objects[i] == skybox)
			||
			(m_objects[i] == ground)
			)*/
		if(m_objects[i]->m_isVisible)
		{
			m_objects[i]->Draw(vpMatrix);
		}
	}

	// Prepare data to test distance
	
	Vector4 moveL(0, 0, -1, 0);
	Vector3 moveW = Vector3((moveL * sceneCamera->GetWorldMatrix()).xyz);
	Vector2 cameraVector = Vector2(moveW.x, moveW.z);
	Vector3 v;
	Vector2 itemVector;
	//int count = 0, count2 = 0;

	for(int i = 0; i < m_numOfBlocks; ++i)
	{
		if ((Distance(m_blocks[i]->GetPosition(), sceneCamera->GetPosition()) < DISTANCE))
		{
			v = m_blocks[i]->GetPosition() - sceneCamera->GetPosition();
			itemVector = Vector2(v.x, v.z);

			if (cameraVector.Dot(itemVector) >= 0)
			{
				m_blocks[i]->Draw(vpMatrix);
				//count++;
			}
			//count2++;
		}
	}
	/*char countString[30];
	sprintf(countString, "Num of blocks : %d %d", count, count2);
	String::GetInstance()->DrawString(countString, Vector3(-0.5f, 0.8f, 0.0f), Vector3(1.0f, 1.0f, 1.0f));*/

}

void ScenesManager::DrawWithColor()
{
	// calculate View-Project Matrix
	Matrix vpMatrix = sceneCamera->CalculateVPMatrix();

	// draw objects
	for(int i = 0; i < m_numOfObjects; ++i)
	{
		if (Distance(m_objects[i]->GetPosition(),sceneCamera->GetPosition()) < DISTANCE){
		Shader* oldShader = m_objects[i]->GetShader();

		m_objects[i]->SetShader(tmpShader);
		// convert color data int 4 byte -> Vector4
		unionObjectColor.iValue = i;
		Vector4 color((float) unionObjectColor.r / 255, 
					  (float) unionObjectColor.g / 255, 
					  (float) unionObjectColor.b / 255, 
					  (float) unionObjectColor.a / 255);
		
		m_objects[i]->Draw(vpMatrix, color);

		m_objects[i]->SetShader(oldShader);
		}
	}
}

Object* ScenesManager::ObjectDetection(int a)
{
	Object* obj = NULL;

	if (a >= m_numOfObjects)
	{
		LOGI("Object not identified!\n");
	}
	else
	{
		obj = m_objects[a];
		LOGI("touch on %s \n", obj->GetId());
	}

	return obj;
}

//---------------------------------------------------
/// Get Object by id
//---------------------------------------------------
Object* ScenesManager::GetObjectById(const char* id)
{
	Object* object = NULL;
	for(int i = 0; i < m_numOfObjects; ++i)
	{
		if(strcmp(m_objects[i]->GetId(), id) == 0)
		{
			object = m_objects[i];
			break;
		}
	}

	return object;
}

//---------------------------------------------------
/// Get Camera by id
//---------------------------------------------------
Camera* ScenesManager::GetCameraById(const char * id)
{
	Camera* cam = NULL;
	for(int i = 0; i < m_numOfCameras; ++i)
	{
		if(strcmp(m_cameras[i]->GetId(), id) == 0)
		{
			cam = m_cameras[i];
			break;
		}
	}

	return cam;
}

//---------------------------------------------------
/// GET INSTANCE
//---------------------------------------------------
ScenesManager* ScenesManager::s_Instance = 0;
ScenesManager* ScenesManager::GetInstance()
{
	if (!s_Instance)
	{
		s_Instance = new ScenesManager();
	}
	return s_Instance;
}

//---------------------------------------------------
/// DESTROY INSTANCE
//---------------------------------------------------
void ScenesManager::DestroyInstance()
{
	if (s_Instance )
	{
		delete s_Instance;
		s_Instance = NULL;
	}
}

//---------------------------------------------------
/// Destructor
//---------------------------------------------------
ScenesManager::~ScenesManager(void)
{
	for(int i = 0; i < m_numOfCameras; ++i)
	{
		SAFE_DEL(m_cameras[i]);
	}
	SAFE_DEL_ARRAY(m_cameras);

	for(int i = 0; i < m_numOfObjects; ++i)
	{
		SAFE_DEL(m_objects[i]);
	}
	SAFE_DEL_ARRAY(m_objects);

	for(int i = 0; i < m_numOfBlocks; ++i)
	{
		SAFE_DEL(m_blocks[i]);
}
	SAFE_DEL_ARRAY(m_blocks);
}

GLint ScenesManager::GetNumOfObjects()
{
 return m_numOfObjects;
}

Object** ScenesManager::GetObjectsPointer()
{
 return m_objects;
}