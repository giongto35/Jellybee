#include "Model.h"
#include "Vertex.h"
#include "NFG.h"
#include <cstring>
#include "Collision.h"
#include "OBJ.h"

Model::Model(void)
{
	m_vboId = 0;
	m_iboId = 0;
	m_numOfIndices = 0;
	m_numOfVertices = 0;
	m_id[0] = '\0';

	m_center = Vector3(0.0f, 0.0f, 0.0f);
	m_radius = 0;
	m_radiusX = 0;
	m_radiusY = 0;
	m_radiusZ = 0;

	m_verticesData = NULL;
	m_material = 0;
	m_numOfMaterial = 0;
	m_updateData = 0;

	m_isReusable = false;
}
//
//void Model::Init(const char *file, const char *id)
//{
//	// init id
//	strcpy(m_id, id);
//
//	// load Model	
//	Vertex *verticesData;
//	unsigned short *indices;
//	LoadNFG(file, m_numOfVertices, verticesData, m_numOfIndices, indices);
//
//	//buffer object
//	glGenBuffers(1, &m_vboId);
//	glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
//	glBufferData(GL_ARRAY_BUFFER, m_numOfVertices * sizeof(Vertex), verticesData, GL_STATIC_DRAW);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//	glGenBuffers(1, &m_iboId);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numOfIndices * sizeof(unsigned short), indices, GL_STATIC_DRAW);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//	GetRadiusAndCenter(verticesData, m_numOfVertices, this);
//
//	delete[] indices;
//	delete[] verticesData;
//}

void Model::Init(const char *file, const char *id, GLenum usage,  MODEL_TYPE type, bool isReusable)
{
	// init id
	strcpy(m_id, id);

	// load Model	
	Vertex *verticesData;
	unsigned short *indices;

	m_type = type;
	m_isReusable = isReusable;

	if (m_type == NFG)
	{
		LoadNFG(file, m_numOfVertices, verticesData, m_numOfIndices, indices);

		//buffer object
		glGenBuffers(1, &m_vboId);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
		glBufferData(GL_ARRAY_BUFFER, m_numOfVertices * sizeof(Vertex), verticesData, usage);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &m_iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numOfIndices * sizeof(unsigned short), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		delete[] indices;
		
	}
	else if (m_type == OBJ)
	{
		ReadOBJ(file, verticesData, m_numOfVertices, m_material, m_numOfMaterial);

		//buffer object (VBO)
		glGenBuffers(1, &m_vboId);
		glBindBuffer(GL_ARRAY_BUFFER, m_vboId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * m_numOfVertices, verticesData, usage);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	GetRadiusAndCenter(verticesData, m_numOfVertices, this);

	m_verticesData = verticesData;

	m_updateData = new Vertex [m_numOfVertices];
}

GLuint Model::GetVboId()
{
	return m_vboId;
}

GLuint Model::GetIboId()
{
	return m_iboId;
}

GLuint Model::GetNumOfVertices()
{
	return m_numOfVertices;
}

GLuint Model::GetNumOfIndices()
{
	return m_numOfIndices;
}

const char* Model::GetId()
{
	return m_id;
}

Model::~Model(void)
{
	if (m_vboId)
	{
		glDeleteBuffers(1, &m_vboId);
	}

	if (m_iboId)
	{
		glDeleteBuffers(1, &m_iboId);
	}

	SAFE_DEL(m_verticesData);
	SAFE_DEL(m_material);
	SAFE_DEL(m_updateData);
}

void Model::DeleteVerticesData()
{
	LOGI("Delete data model %s\n", m_id);
	SAFE_DEL(m_verticesData);
	SAFE_DEL(m_updateData);
}
