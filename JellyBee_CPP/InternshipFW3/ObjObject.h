#pragma once
#include "Platforms.h"
#include "Vertex.h"

class Texture;
class Shader;

class ObjObject
{
public:
	Vector3 m_position, m_rotation, m_scale;
	bool m_isVisible;
	Vector3 m_center;
	float m_radius;
	float m_radiusX;
	float m_radiusY;
	float m_radiusZ;

	Matrix m_worldMatrix;
	char m_id[MAX_10];

	GLuint m_vboId;
	GLuint m_numOfModel;
	GLuint* m_offsets;
	Vertex* m_verticesData;
	// ID of texture in Material is UseMtl
	char** m_usemtls;

	bool m_isBinding;
	bool m_isError;
	
	GLuint m_numOfTexture;
	Texture** m_textures;

	//Shader* m_frameShader;
	Shader* m_shader;

public:
	ObjObject(void);
	~ObjObject(void);
	
	void Init (	const char* path, 
				const char* id, 
				const GLuint& target = GL_TEXTURE_2D , 
				const GLuint& wrappingMode = GL_REPEAT 
				);
	void Init(const char* id, const Vector3 & pos, const Vector3 & rot, const Vector3 & sca, Shader* shader, ObjObject* objObject);

	void Draw ( Matrix & vpMatrix );
	void Update ( );
	void Draw ( Matrix & vpMatrix, Vector4 & color );

	//Setters
	void SetShader ( Shader *& shader );

	// Buffer Function
	void BindBuffer ();
	void DeleteBuffers ( );

	// Getters
	Shader*& GetShader ( );
	
public:
	void ReadMaterial( const char* path, const GLuint& target, const GLuint& wrappingMode );
	void ModifiedPath( char*& io_pathDst , const char* pathSrc );
	void ReadVector3 ( FILE*& file, Vector3& vec3 );
	void ReadVector2 ( FILE*& file, Vector2& vec2 );
	void ReadFace (	char* data, 
					int format,
					GLuint& io_index,
					const Vector3* vecPos, 
					const GLuint& numPos,
					const Vector2* vecUV,
					const GLuint& numUV,
					const Vector3* vecNormal,
					const GLuint& numNormal
					);

	// Setters
	void SetPosition(const Vector3 & pos);
	void SetRotation(const Vector3 & rot);
	void SetScale(const Vector3 & sca);
	void SetWorldMatrix(const Matrix & wMat);

	// Getters
	const Vector3 & GetPosition();
	const Vector3 & GetRotation();
	const Vector3 & GetScale();
	const Matrix & GetWorldMatrix();
	const char* GetId();

	void CalculateCenter();

	void CalculateWorldMatrix();
};
