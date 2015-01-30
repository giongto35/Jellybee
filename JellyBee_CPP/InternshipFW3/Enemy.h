#pragma once
#include "object.h"

class Enemy :
	public Object
{
public:
	MapType m_type;

public:
	Enemy(void);
	~Enemy(void);
	void Init(const char* id, const Vector3 & pos, const Vector3 & rot, 
		const Vector3 & sca, Texture* texture, Model* model, Shader* shader, 
		MapType type);
	void Update();
};
