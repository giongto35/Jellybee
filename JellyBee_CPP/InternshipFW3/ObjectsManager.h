#pragma once
#include "MainCharacter.h"
#include "Item.h"
#include "ResourcesManager.h"

class ObjectsManager
{
private:
	static ObjectsManager* s_Instance;

	MainCharacter* m_mainCharacter;
	int m_numItems;
	int m_numTrees;
	//Item m_items[200];
	Item **m_items;
	void Update();
	ResourcesManager* RM;

public:
	static ObjectsManager* GetInstance();
	static void DestroyInstance();
	void Init(int numItems, int numTrees);
	void GenerateNextItems();
	void GenerateNextTrees();
	void Draw(Matrix vpmatrix);
	ObjectsManager(void);
	~ObjectsManager(void);
};
