#include "ObjectsManager.h"
#include <stdlib.h>

//---------------------------------------------------
/// Get Instance
//---------------------------------------------------
ObjectsManager* ObjectsManager::s_Instance = 0;
ObjectsManager* ObjectsManager::GetInstance()
{
	if (!s_Instance)
	{
		s_Instance = new ObjectsManager();
	}
	return s_Instance;
}

void ObjectsManager::DestroyInstance()
{
	SAFE_DEL(s_Instance);
}

void ObjectsManager::GenerateNextItems()
{
	
	Vector3 pos = Vector3
				  (
					(float)(rand() % 2000 - 1000) / 1000, 
					(float)(rand() % 2000 - 1000) / 100, 
					(float)(rand() % 2000 - 1000) / 100
				  );
	m_items[m_numItems ++]->Init(
									"HoneyPot", 
									pos , 
									Vector3(0, 0, 0), 
									Vector3(0.1f, 0.1f, 0.1f), 
									RM->GetTextureById("woman1"), 
									RM->GetModelById("woman1"), 
									RM->GetShaderById("objShad"), HONEY
								);
}

void ObjectsManager::Draw(Matrix vpmatrix)
{
	for (int i = 0; i < m_numItems; i++)
	{
		m_items[i]->Draw(vpmatrix);
	}
}

void ObjectsManager::Init(int numItems, int numTrees)
{
	RM = ResourcesManager::GetInstance();
	m_numItems = 0;
	m_numTrees = 0;

	m_items = new Item* [numItems];
	
	for (int i = 0; i < numItems; i++)
	{
		m_items[i] = new Item;
	}

	for (int i = 0; i < numItems; i++)
	{
		GenerateNextItems();
	}
	//m_numTrees = numTrees;
}

ObjectsManager::ObjectsManager(void)
{
}

ObjectsManager::~ObjectsManager(void)
{
	for (int i = 0; i < m_numItems; i++)
	{
		SAFE_DEL(m_items[i]);
	}

	SAFE_DEL(m_items);
}
