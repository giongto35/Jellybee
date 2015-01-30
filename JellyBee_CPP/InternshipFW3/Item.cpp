#include "Item.h"

Item::Item(void)
{
	// m_type = -1;
}

Item::~Item(void)
{
}

void Item::Init(const char* id, const Vector3 & pos, const Vector3 & rot, const Vector3 & sca, Texture* texture, Model* model, Shader* shader, MapType type, const char *textureUvId)
{
	Object::Init(id, pos, rot, sca, texture, model, shader, textureUvId);
	m_type = type;
}