#include "Bee.h"

Bee::Bee(void)
{
}

Bee::~Bee(void)
{
}

void Bee::Init(const char* id, const Vector3 & pos, const Vector3 & rot,
		const Vector3 & sca, Texture* texture, Model* model, Shader* shader, 
		Vector3 & speed)
{
	Object::Init(id, pos, rot, sca, texture, model, shader);
	m_speed = speed;
}