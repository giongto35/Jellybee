#include "Enemy.h"

Enemy::Enemy(void)
{
	//m_type = -1;
}

Enemy::~Enemy(void)
{
}

void Enemy::Init(const char* id, const Vector3 & pos, const Vector3 & rot, 
				 const Vector3 & sca, Texture* texture, Model* model, Shader* shader, 
				 MapType type)
{
	Object::Init(id, pos, rot, sca, texture, model, shader);
	m_type = type;
}

void Enemy::Update()
{

}