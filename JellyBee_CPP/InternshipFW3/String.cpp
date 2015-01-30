#include "String.h"
#include <string.h>
#include "ResourcesManager.h"
#include "vertex.h"
#include "ScreenResolution.h"

String::String(void)
{
	m_ObjChar = NULL;
	m_bitmap = NULL;
	m_quad = NULL;
	m_shader = NULL;
}

String::~String(void)
{
	SAFE_DEL(m_ObjChar);
}

//---------------------------------------------------
/// Get Instance
//---------------------------------------------------
String* String::s_Instance = 0;

String* String::GetInstance()
{
	if (!s_Instance)
	{
		s_Instance = new String();
	}
	return s_Instance;
}

//---------------------------------------------------
/// Destroy Instance
//---------------------------------------------------
void String::DestroyInstance() 
{
	if (s_Instance)
	{
		delete s_Instance;
		s_Instance = 0;
	}
}

void String::Init()
{
	if (m_ObjChar == NULL)
	{
		m_quad = ResourcesManager::GetInstance()->GetModelById("char");
		m_bitmap = ResourcesManager::GetInstance()->GetTextureById("bitmap");
		m_shader = ResourcesManager::GetInstance()->GetShaderById("Char");

		m_ObjChar = new Object;
		m_ObjChar->Init("character", 
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3(0.0f, 0.0f, 0.0f),
			Vector3((float) (BITMAPFONT_W_H / 16) / ScreenResolution::GetInstance()->m_screenWidth, (float)32 / ScreenResolution::GetInstance()->m_screenHeight, 1.0f),
			m_bitmap,
			m_quad,
			m_shader,
			NULL);
	}
}

void String::DrawString(const char *string, Vector3 position, Vector3 scale, Vector3 color)
{
	int i, j, k;
	int length = strlen(string);
	Matrix string_matrixI;

	float del = (float) (BITMAPFONT_W_H / 16) / 800;
	float x = position.x;

	// modify buffer data

	int numOfVertices = m_quad->GetNumOfVertices();

	memcpy(m_quad->m_updateData, m_quad->m_verticesData, sizeof(Vertex) * numOfVertices);

	Vertex *ptr = m_quad->m_updateData;

	int width = 16;
	float w = (float) 1 / 16;

	for (k = 0; k < length; ++k)
	{
		i = 15 - (int)string[k] / width;
		j = (int)string[k] % width;

		{
			ptr[0].uv = Vector2(j * w, i * w);
			ptr[1].uv = Vector2(j * w + w, i * w);
			ptr[2].uv = Vector2(j * w, i * w + w);
			ptr[3].uv = Vector2(j * w + w, i * w + w);
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_quad->GetVboId());
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numOfVertices, ptr, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		x += del * scale.x;
		m_ObjChar->SetPosition(Vector3(x, position.y, position.z));
		m_ObjChar->SetScale(Vector3((float)(BITMAPFONT_W_H / 16) * scale.x / 800, 
											(float)(BITMAPFONT_W_H / 16) * scale.y / 480, 
											1.0f));
		m_ObjChar->Draw(string_matrixI, color);
	}

	ptr = NULL;
}