#include "FileManager.h"
#include "ObjObject.h"
#include "Texture.h"
#include "Vertex.h"
#include "Shader.h"
#include "InGame.h"
#include <stdlib.h>

///////////////////////////////
// Constructor
///////////////////////////////

ObjObject::ObjObject(void)
{
	m_textures = NULL;
	m_shader = NULL;

	m_usemtls = NULL;
	m_verticesData = NULL;
	m_vboId = 0;
	
	m_numOfModel = 0;
	m_numOfTexture = 0;

	m_isBinding = false;
	m_isError = false;

	m_center = Vector3(0.0f, 0.0f, 0.0f);
	m_radius = 0;
	m_radiusX = 0;
	m_radiusY = 0;
	m_radiusZ = 0;

	CalculateWorldMatrix();
}
///////////////////////////////
///  Destructor
///////////////////////////////

ObjObject::~ObjObject(void)
{
	if (strcmp(m_id, "Tree") != 0)
	{

		DeleteBuffers ( );
		for ( GLuint texId = 0; texId < m_numOfTexture; texId ++ )
		{
			SAFE_DEL ( m_textures [ texId ] );
		}
		SAFE_DEL_ARRAY (m_textures)


			for ( GLuint id = 0; id < m_numOfModel; id ++ )
			{
				SAFE_DEL ( m_usemtls [ id ] );
			}
			SAFE_DEL_ARRAY ( m_usemtls );
			SAFE_DEL_ARRAY ( m_verticesData );
			SAFE_DEL_ARRAY ( m_offsets );
	}

	//m_shader = NULL;
}

///////////////////////////////
///  Init Function
///////////////////////////////
void ObjObject::Init(const char* id, const Vector3 & pos, const Vector3 & rot, const Vector3 & sca, Shader* shader, ObjObject* objObject)
{
	strcpy(m_id, id);
	m_position = pos;
	m_rotation = rot;
	m_scale = sca;
	m_shader = shader;

	m_vboId = objObject->m_vboId;
	m_numOfModel = objObject->m_numOfModel;
	m_offsets = objObject->m_offsets;
	m_verticesData = objObject->m_verticesData;
	// ID of texture in Material is UseMtl
	m_usemtls = objObject->m_usemtls;

	m_isBinding = objObject->m_isBinding;
	
	m_numOfTexture = objObject->m_numOfTexture;
	m_textures = objObject->m_textures;

	CalculateWorldMatrix();
	CalculateCenter();
	/*m_radius = m_model->m_radius * m_scale.x;
	m_radiusX = m_model->m_radiusX * m_scale.x;
	m_radiusY = m_model->m_radiusY * m_scale.y;
	m_radiusZ = m_model->m_radiusZ * m_scale.z;*/
}
///////////////////////////////
void ObjObject::Init (	const char* path, 
						const char* objId, 
						const GLuint &target, 
						const GLuint& wrappingMode 
						)
{
	strcpy(m_id, objId);	
	
	LOGI ( "\nLoading Model Obj File: %s", path );
	FILE *f;
	f = FileManager::GetInstance()->Open ( path, "rt" );
	if ( f == NULL )
	{
		LOGE ( "\nCan not load Model OBJ from file: %s", path );
	}
	else
	{
		GLuint numOfVertices = 0;
		GLuint numOfPos = 0;
		GLuint numOfUV = 0;
		GLuint numOfNorm = 0;

		char* line = new char[ SIZE_OF_STRING ];
		int format = - 1;
		bool isReadMaterial = false;
		
		// Count the numbers Pos, UV, Norm Vertor and Face.
		do
		{
			fscanf ( f, "%s", line );

			if (( strcmp ( line, "mtllib" ) == 0 ) && !isReadMaterial )
			{
				char* pFile = new char [ SIZE_OF_STRING ];
				fscanf ( f, "%s", pFile );
				ModifiedPath ( pFile, path );
				ReadMaterial ( pFile, target, wrappingMode);
				SAFE_DEL_ARRAY ( pFile );
				isReadMaterial = true;
			}
			
			else if ( strcmp ( line, "usemtl" ) == 0 )
			{
				m_numOfModel++;
			}
			else if ( strcmp ( line, "s" ) == 0 )
			{
			}
			else if ( strcmp ( line, "v" ) == 0 )
			{
				numOfPos++;
			}
			else if ( strcmp ( line, "vt" ) == 0 )
			{
				numOfUV++;
			}
			else if ( strcmp ( line, "vn" ) == 0 )
			{
				numOfNorm++;
			}
			else if ( strcmp ( line, "f" ) == 0 )
			{
				if ( format == -1 )
				{
					char* tmp = new char [ SIZE_OF_ID ];
					int t;
					fscanf ( f , "%d", &t);
					fgets ( tmp, SIZE_OF_ID, f ); 
					int index = 0;
					format = 0;

					// format 0: v
					// format 1: v/vt
					// format 2: v//vn
					// format 3: v/vt/vn

					while ( tmp [ index ] != '\0' && tmp [ index ] != 32 )
					{
						index ++;
						if ( tmp [ index ] == '/' )	
						{
							if ( tmp [ index - 1 ] != '/' )	
							{
								format = 3 ;
							}
							else
							{
								format = 2;
							}
							break;
						}
						else if ( tmp [ index ] == 32 )
						{
							format = 1;
						}
					}
					SAFE_DEL_ARRAY ( tmp );
				}		
				numOfVertices += 3;
			}
		}
		while ( !feof ( f ) );

		fseek ( f, 0, SEEK_SET );

		m_verticesData = new Vertex [ numOfVertices ];
		GLuint numVer = 0;

		m_usemtls = new char* [ m_numOfModel ];
		for ( GLuint id = 0; id < m_numOfModel; id ++ )
		{			
			m_usemtls [ id ] = new char [ SIZE_OF_STRING ];
		}
		m_offsets = new GLuint [ m_numOfModel + 1 ];

		GLuint numMod = 0;		
		//m_offsets [ m_numOfModel ] = numOfVertices;

		Vector3* vecPos = new Vector3 [ numOfPos ];
		Vector2* vecUV = new Vector2 [ numOfUV ];
		Vector3* vecNorm = new Vector3 [ numOfNorm ];

		numOfPos = 0;
		numOfUV = 0;
		numOfNorm = 0;

		char* ID = new char [ SIZE_OF_ID ];
		char* data = new char [ SIZE_OF_LINE ];

		bool isReadDone = false;

		// Get the data.
		do
		{
			if (! isReadDone )
			{
				fscanf ( f, "%s", line );
			}
			isReadDone = false;

			if ( strcmp ( line, "g" ) == 0 )
			{
				fscanf ( f , "%s" , ID );
			}
			else if ( strcmp ( line, "usemtl" ) == 0 )
			{
				m_offsets [ numMod ] = numVer;
				fscanf ( f , "%s" , m_usemtls [ numMod++ ] );
			}
			else if ( strcmp ( line, "s" ) == 0 )
			{
			}
			else if ( strcmp ( line, "v" ) == 0 )
			{
				ReadVector3 ( f, vecPos [ numOfPos ] );
				numOfPos ++;
			}
			else if ( strcmp ( line, "vt" ) == 0 )
			{
				ReadVector2 ( f, vecUV [ numOfUV ] );
				numOfUV ++;
			}
			else if ( strcmp ( line, "vn" ) == 0 )
			{	
				ReadVector3 ( f, vecNorm [ numOfNorm ]);
				numOfNorm ++;
			}
			else
			{
				while ( strcmp ( line, "f" ) == 0 )
				{	
					fgets ( data, SIZE_OF_LINE, f );
					ReadFace ( data, format, numVer, vecPos, numOfPos, vecUV, numOfUV, vecNorm, numOfNorm );
					fscanf ( f, "%s", line );
					isReadDone = true;
					if ( feof (f) )
					{
						break;
					}
				}
			}
		}
		while ( !feof ( f ) );
		
		m_offsets [ m_numOfModel ] = numVer;
		LOGI ( "\nLoad model succes:\n - num vertices: %d\n - num textures: %d\n - num models: %d\n", 
			m_offsets [ m_numOfModel ], m_numOfTexture, m_numOfModel );
		
		SAFE_DEL_ARRAY ( data );
		SAFE_DEL_ARRAY ( ID );
		SAFE_DEL_ARRAY ( vecPos );
		SAFE_DEL_ARRAY ( vecUV );
		SAFE_DEL_ARRAY ( vecNorm );
		SAFE_DEL_ARRAY ( line );

		fclose ( f );
	}
}

///////////////////////////////
///  Buffer Function
///////////////////////////////
void ObjObject::BindBuffer()
{
	//buffer object
	if ( m_numOfModel && !m_isBinding)
	{
		glGenBuffers ( 1, &m_vboId );
		glBindBuffer ( GL_ARRAY_BUFFER, m_vboId );
		glBufferData (	GL_ARRAY_BUFFER, 
						m_offsets [ m_numOfModel ] * sizeof ( Vertex ),
						(void*) m_verticesData, 
						GL_STATIC_DRAW 
						);

		glBindBuffer ( GL_ARRAY_BUFFER, 0 );
		
		/*for ( GLuint texId = 0; texId < m_numOfTexture; texId ++ )
		{
			m_textures [ texId ]->BindTexture ();
		}*/
	
		LOGI ("\nLoad Obj Models %s Succes", GetId());
		m_isBinding = true;
	}
}

void ObjObject::DeleteBuffers()
{
	if ( m_isBinding )
	{
		if ( m_vboId )
		{
			glDeleteBuffers ( 1, & m_vboId );
		}

		/*for ( GLuint texId = 0; texId < m_numOfTexture; texId ++ )
		{
			if ( m_textures [ texId ] )
			{
				m_textures [ texId ]->DeleteTexture ();
			}
		}*/
		
		LOGI ("\nDelete Buffers Obj Models %s Succes", GetId());
		m_isBinding = false;
	}
}

///////////////////////////////
///  Setters Function
///////////////////////////////
void ObjObject::SetShader( Shader *&shader )
{
	m_shader = shader;
}

///////////////////////////////
///  Getters Function
///////////////////////////////

Shader*& ObjObject::GetShader ()
{
	return m_shader;
}

///////////////////////////////
///  Gamer Function
///////////////////////////////
void ObjObject::Draw ( Matrix &vpMatrix )
{
	if (( m_shader == NULL ) || (m_textures == NULL ) || m_isError )
	{
		LOGE ( "\nObject %s is Error", GetId ( ) );
		return;
	}
	else 
	{
		if (!m_isBinding)
		{
			BindBuffer();
		}

		// use program
		glUseProgram ( m_shader->program );
		m_shader->EnableState ( );

		glBindBuffer ( GL_ARRAY_BUFFER, m_vboId );

		// Calculate WVP matrix
		Matrix wvpMatrix;
		wvpMatrix = m_worldMatrix * vpMatrix;

		GLuint texId;
		GLuint target = GL_TEXTURE_2D;
		GLuint textureId = 0;
		//GLuint modId = 0;
		for ( GLuint modId = 0; modId < m_numOfModel; modId ++ )
		{
			// get texture target
			for (texId  = 0; texId < m_numOfTexture; texId ++ )
			{
				if ( strcmp ( m_usemtls [ modId ], m_textures [ texId ]->GetId () ) == 0 )
				{
					textureId = m_textures [ texId ]->GetTextureId ( );
					break;
				}
			}

			if ( texId == m_numOfTexture )
			{
				continue;
			};

			
			if (m_shader->a_position != -1)
			{
				glEnableVertexAttribArray(m_shader->a_position);
				glVertexAttribPointer(m_shader->a_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, pos));
			}
			if (m_shader->a_normal != -1)
			{
				glEnableVertexAttribArray(m_shader->a_normal);
				glVertexAttribPointer(m_shader->a_normal, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
			}
			if (m_shader->a_uv != -1)
			{
				glEnableVertexAttribArray(m_shader->a_uv);
				glVertexAttribPointer(m_shader->a_uv, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
			}

			if (m_shader->u_MatWVP != -1)
			{
				glUniformMatrix4fv(m_shader->u_MatWVP, 1, 0, (float*)(&wvpMatrix));

			}
			if (m_shader->u_light != -1)
			{
				Vector3 vec_light(8.0f, 8.0f, 8.0f);
				glUniform3fv(m_shader->u_light, 1, (float*)&vec_light);
			}
			if (m_shader->u_campos != -1)
			{
				glUniform3fv(m_shader->u_campos, 1, (float*)&InGame::GetInstance()->m_sceneCamera->GetPosition());
			}
			if (m_shader->u_MatW != -1)
			{
				glUniformMatrix4fv(m_shader->u_MatW, 1, GL_FALSE, m_worldMatrix.m);
			}
			if (m_shader->a_CubeVertexPos != -1)
			{
				glEnableVertexAttribArray(m_shader->a_CubeVertexPos);
				glVertexAttribPointer(m_shader->a_CubeVertexPos, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, pos));
			}

			if (m_shader->u_CubeMVPMatrix != -1)
			{
				glUniformMatrix4fv(m_shader->u_CubeMVPMatrix, 1, 0, (float*)(&wvpMatrix));

			}

			glBindTexture ( target, textureId );
			glActiveTexture ( GL_TEXTURE0 );

			if ( m_shader->u_tex[0] != -1 )
			{
				glUniform1i ( m_shader->u_tex[0], 0 );
			}

			glDrawArrays ( GL_TRIANGLES, m_offsets [ modId ],  m_offsets [ modId + 1] -  m_offsets [ modId ] );

			// bind -> 0
			glBindTexture ( target, 0 );
		}
		glBindBuffer ( GL_ARRAY_BUFFER, 0 );
		m_shader->DisableState ( );
	}
}

void ObjObject::Draw ( Matrix &vpMatrix, Vector4 &color )
{
}

void ObjObject::Update ( )
{
}

///////////////////////////////
///  Private Function
///////////////////////////////
void ObjObject::ReadMaterial ( const char *path, const GLuint &target, const GLuint &wrappingMode )
{
	FILE *f;
	f = FileManager::GetInstance()->Open ( path, "r" );

	if ( f == NULL )
	{
		LOGE ( "\nCan not load Model OBJ from file: %s", path );
	}
	else
	{
		char* line = new char [ SIZE_OF_STRING ];	
		// Count the numbers Pos, UV, Norm Vertor and Face.
		do
		{
			fscanf ( f, "%s", line );
			
			if ( strcmp ( line, "map_Kd" ) == 0 )
			{
				m_numOfTexture++;
			}
			else
			{
				char* line = new char [ SIZE_OF_LINE ];
				fgets ( line, SIZE_OF_LINE, f );
				SAFE_DEL ( line );
			}
		}
		while ( !feof ( f ) );
		fseek ( f, 0, SEEK_SET );
		if ( m_numOfTexture )
		{
			m_textures = new Texture* [ m_numOfTexture ];
		}
	/*	for ( GLuint i = 0; i < m_numOfTexture; i ++ )
		{
			m_textures [ i ] = new Texture();
		}*/
		GLuint numTex = 0;

		char* pFile = new char [ SIZE_OF_STRING ];
		char* ID = new char[ SIZE_OF_ID ];

		do
		{
			fscanf ( f, "%s", line );
			
			if ( strcmp ( line, "newmtl" ) == 0 )
			{
				fscanf ( f, "%s", ID );
			}

			else if ( strcmp ( line, "map_Kd" ) == 0 )
			{
				fscanf ( f, "%s", pFile );
				ModifiedPath ( pFile, path );
				strcpy ( pFile + strlen ( pFile ) - 3, "tga" );
				m_textures [ numTex ] = new Texture();
				m_textures [ numTex ]->Init ( pFile, ID, target, wrappingMode );
				numTex++;
			}
		}
		while ( !feof ( f ) && ( numTex < m_numOfTexture ) );

		SAFE_DEL_ARRAY ( pFile );
		SAFE_DEL_ARRAY ( ID );
		SAFE_DEL_ARRAY ( line );
		LOGI ("\nFinish load Material path: %s", path);
		fclose ( f );
	}
}

// Pre process, modified path to Read Material
void ObjObject::ModifiedPath ( char *& io_pathDst, const char* pathSrc )
{
	GLuint fileOffset1 = strlen ( pathSrc ) - 4;
	GLuint fileOffset2 = strlen ( io_pathDst ) - 4;
	do
	{
		if (( pathSrc [ fileOffset1 - 1 ] == '\\' ) || ( pathSrc [ fileOffset1 - 1 ] == '/' ))
			break;
		fileOffset1--;
	} 
	while ( fileOffset1 != 0 );
	do
	{
		if (( io_pathDst [ fileOffset2 - 1 ] == '\\' ) || ( io_pathDst [ fileOffset2 - 1 ] == '/' ))
			break;
		fileOffset2--;
	} 
	while ( fileOffset2 != 0 );

	char* tmp = new char[fileOffset1 - fileOffset2 + strlen ( io_pathDst ) + 1 ];
	memcpy ( tmp, pathSrc, fileOffset1 );
	tmp [ fileOffset1 ] = '\0';
	strcat ( tmp, io_pathDst + fileOffset2 );
	SAFE_DEL_ARRAY ( io_pathDst );
	io_pathDst = tmp;
}

void ObjObject::ReadVector3 ( FILE*& file, Vector3& vec3 )
{
	fscanf ( file, "%f %f %f", & vec3.x, &vec3.y, &vec3.z );
}

void ObjObject::ReadVector2 ( FILE*& file, Vector2& vec2 )
{
	fscanf ( file, "%f %f", &vec2.x, &vec2.y );
}

void ObjObject::ReadFace (	char* data, 
							int format,
							GLuint& io_index,
							const Vector3* vecPos, 
							const GLuint& numPos,
							const Vector2* vecUV,
							const GLuint& numUV,
							const Vector3* vecNormal,
							const GLuint& numNormal
						)
{
	char* ptrBegin = data;
	char* ptrEnd;
	for ( int i = 0; i < 3; i++)
	{
		GLint posId, uvId, norId ;
			
		posId = (GLint) strtod ( ptrBegin, &ptrEnd );
		posId += ( posId < 0 ) ? ( numPos ) : ( - 1 );
		posId = (( posId > numPos) || ( posId < 0 )) ? (numPos - 1) : posId;
		m_verticesData [ io_index ].pos = vecPos [ posId ];

		ptrBegin = ptrEnd + 1;

		if ( format & 1 )
		{
			uvId = (GLint) strtod ( ptrBegin, &ptrEnd );
			uvId += ( uvId < 0 ) ? ( numUV ) : ( - 1 );
			uvId = (( uvId > numUV ) || ( uvId < 0 )) ? (numUV - 1) : uvId;
			m_verticesData [ io_index ].uv = vecUV [ uvId ];

			ptrBegin = ptrEnd + 1;
		}

		if ( format & 2 )
		{
			norId =  (GLint) strtod ( ptrBegin, &ptrEnd );			
			norId += ( norId < 0 ) ? (  numNormal ) : ( - 1 );
			norId = (( norId > numNormal ) || ( norId < 0 )) ? ( numNormal - 1) : norId;
			m_verticesData [ io_index ].normal = vecNormal [ norId ];
			ptrBegin = ptrEnd + 1;
		}
		io_index ++;
	}
}

//---------------------------------------------------
/// These functions follow are setters
//---------------------------------------------------
void ObjObject::SetPosition(const Vector3 & pos)
{
	m_position = pos;
	CalculateWorldMatrix();
	CalculateCenter();
}

void ObjObject::SetRotation(const Vector3 & rot)
{
	m_rotation = rot;
	CalculateWorldMatrix();
	CalculateCenter();
}

void ObjObject::SetScale(const Vector3 & sca)
{
	m_scale = sca;
	CalculateWorldMatrix();
	CalculateCenter();

	// if m_scale.x == m_scale.y == m_scale.z
	/*m_radius = m_model->m_radius * m_scale.x;
	m_radiusX = m_model->m_radiusX * m_scale.x;
	m_radiusY = m_model->m_radiusY * m_scale.y;
	m_radiusZ = m_model->m_radiusZ * m_scale.z;*/
}

void ObjObject::SetWorldMatrix(const Matrix & wMat)
{
	m_worldMatrix = wMat;
}

//---------------------------------------------------
/// These functions follow are getters
//---------------------------------------------------
const char* ObjObject::GetId()
{
	return m_id;
}

const Vector3 & ObjObject::GetPosition()
{
	return m_position;
}

const Vector3 & ObjObject::GetRotation()
{
	return m_rotation;
}

const Vector3 & ObjObject::GetScale()
{
	return m_scale;
}


const Matrix & ObjObject::GetWorldMatrix()
{
	return m_worldMatrix;
}

//---------------------------------------------------
/// CalculateCenter
//---------------------------------------------------
void ObjObject::CalculateCenter()
{
	/*Vector4 temp = Vector4(m_model->m_center, 1.0f);
	temp = temp * m_worldMatrix;
	m_center = temp.xyz;*/
}

//---------------------------------------------------
/// Calculate object's World Matrix
//---------------------------------------------------
void ObjObject::CalculateWorldMatrix()
{
	Matrix rotation, scale, translation;

	rotation.SetRotation(m_rotation);
	scale.SetScale(m_scale);
	translation.SetTranslation(m_position);

	m_worldMatrix = scale * rotation * translation;
}