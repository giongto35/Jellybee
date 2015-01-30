#pragma once
#include "Platforms.h"
#include "Vector3.h"
#include "Vertex.h"
#include "MaterialStruct.h"

class Model
{
private:
	GLuint m_vboId, m_iboId;
	int m_numOfVertices;
	GLint m_numOfIndices;
	char m_id[SIZE_OF_ID];

public:
	Vector3 m_center;
	float m_radius;
	float m_radiusX;
	float m_radiusY;
	float m_radiusZ;
	Vertex *m_verticesData;

	MODEL_TYPE m_type;
	Material *m_material;
	int m_numOfMaterial;
	Vertex *m_updateData;
	
	bool m_isReusable;
public:
	Model(void);
	~Model(void);
	//void Init(const char *file, const char *id);
	void Init(const char *file, const char *id, GLenum usage = GL_DYNAMIC_DRAW, MODEL_TYPE type = NFG, bool isReusable = false);
	GLuint GetVboId();
	GLuint GetIboId();
	GLuint GetNumOfVertices();
	GLuint GetNumOfIndices();
	const char* GetId();

	void DeleteVerticesData();
};
