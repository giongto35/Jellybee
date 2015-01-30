#pragma once
#include "object.h"

class Bee :
	public Object
{
private: 
	Vector3 m_speed;

public:
	Bee(void);
	~Bee(void);

	void Init(const char* id, const Vector3 & pos, const Vector3 & rot,
		const Vector3 & sca, Texture* texture, Model* model, Shader* shader, 
		Vector3 & speed);
};
