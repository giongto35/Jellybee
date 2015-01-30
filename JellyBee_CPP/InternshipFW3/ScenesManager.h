#pragma once
#include "Platforms.h"
#include "Vector3.h"

class Object;
class Camera;
class Map;

float Distance(Vector3 u, Vector3 v);

class ScenesManager
{
private:
	Object** m_objects;
	Object** m_blocks;
	Camera** m_cameras;
	GLint m_numOfObjects, m_numOfCameras, m_numOfBlocks;
	static ScenesManager* s_Instance;

private:
	ScenesManager(void);
	~ScenesManager(void);
public:
	Map* m_map;
	void Init(const char* file);
	void Update();
	void Draw();
	void DrawWithColor();

	Object* ObjectDetection(int a);
	Object* GetObjectById(const char* id);
	Camera* GetCameraById(const char* id);
	GLint GetNumOfObjects();
	Object** GetObjectsPointer();


	static ScenesManager* GetInstance();
	static void DestroyInstance();
};
