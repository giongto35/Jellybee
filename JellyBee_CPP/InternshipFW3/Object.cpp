#include "Object.h"
#include "Texture.h"
#include "Model.h"
#include "Shader.h"
#include "Vertex.h"
#include "string.h"
#include "InGame.h"
#include "EffectManager.h"
#include "ResourcesManager.h"
#include "Timer.h"

//---------------------------------------------------
/// Constructor
//---------------------------------------------------
Object::Object(void)
: m_position(0.0f, 0.0f, 0.0f), 
m_rotation(0.0f, 0.0f, 0.0f), 
m_scale(1.0f, 1.0f, 1.0f)
{
	m_NumTex = 0;
	m_model = 0;
	m_shader = 0;
	m_otherData = 0;
	m_id[0] = '\0';

	m_center = Vector3(0.0f, 0.0f, 0.0f);
	m_radius = 0;
	m_radiusX = 0;
	m_radiusY = 0;
	m_radiusZ = 0;

	m_textures = 0;

	CalculateWorldMatrix();

	m_textureUV = 0;
	m_isVisible = true;
	m_otherData = 0.0f;
}

//---------------------------------------------------
/// Init
//---------------------------------------------------
void Object::Init(const char * id, const Vector3 & pos, const Vector3 & rot, const Vector3 & sca, int num_tex, Texture** texture, Model* model, Shader* shader)
{
	//init id
	strcpy(m_id, id);

	m_position = pos;
	m_rotation = rot;
	m_scale = sca;
	CalculateWorldMatrix();
	m_NumTex = num_tex;

	SAFE_DEL(m_textures);
	m_textures = new Texture*[m_NumTex];

	for (int i = 0; i < m_NumTex; i++)
		m_textures[i] = texture[i];
	m_model = model;
	m_shader = shader;

	CalculateCenter();
	m_radius = m_model->m_radius * m_scale.x;
	m_radiusX = m_model->m_radiusX * m_scale.x;
	m_radiusY = m_model->m_radiusY * m_scale.y;
	m_radiusZ = m_model->m_radiusZ * m_scale.z;
}

void Object::Init(const char * id, const Vector3 & pos, const Vector3 & rot, const Vector3 & sca, Texture* texture, Model* model, Shader* shader, const char *textureUvId)
{
	//init id
	strcpy(m_id, id);

	m_position = pos;
	m_rotation = rot;
	m_scale = sca;
	CalculateWorldMatrix();

	m_NumTex = 1;

	SAFE_DEL(m_textures);
	m_textures = new Texture*[m_NumTex];

	m_textures[0] = texture; 
	m_model = model;
	m_shader = shader;

	if (textureUvId == NULL)
	{
		m_textureUV = NULL;	
	}

	else
	{
		m_textureUV = (TextureUV*)m_textures[0]->GetTextureUvById(textureUvId);
	}

	// set uv (use big texture)
	// OBJ model

	if ((m_model->m_type == OBJ && m_textureUV == NULL) || (m_model->m_type == NFG && m_textureUV != NULL)) 
	{
		// update one time
		UpdateTextureUV();
	}

	if (m_textureUV == NULL && m_model->m_type == NFG && m_model->m_isReusable == false)
	{
		// no update
		m_model->DeleteVerticesData();
	}

	//////////////////

	CalculateCenter();
	m_radius = m_model->m_radius * m_scale.x;
	m_radiusX = m_model->m_radiusX * m_scale.x;
	m_radiusY = m_model->m_radiusY * m_scale.y;
	m_radiusZ = m_model->m_radiusZ * m_scale.z;
}

//---------------------------------------------------
/// Calculate object's World Matrix
//---------------------------------------------------
void Object::CalculateWorldMatrix()
{
	Matrix rotation, scale, translation;

	rotation.SetRotation(m_rotation);
	scale.SetScale(m_scale);
	translation.SetTranslation(m_position);

	m_worldMatrix = scale * rotation * translation;
}

//---------------------------------------------------
/// DRAW OBJECT
//---------------------------------------------------
void Object::Draw(Matrix & vpMatrix, Vector3 ucolor)
{
	// Update UV if needed
	if (m_model->m_isReusable)
	{
		UpdateTextureUV();
	}

	int target = m_textures[0]->GetTarget();
	// use program
	//Shadow
	//if (EffectManager::GetInstance()->m_drawShadow == true)
	//{
	//	glUseProgram(ResourcesManager::GetInstance()->GetShaderById("LightCast")->program);
	//}
	//else
		glUseProgram(m_shader->program);

	// Calculate WVP matrix
	Matrix wvpMatrix;
	wvpMatrix = m_worldMatrix * vpMatrix;

	// binding buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVboId());
	if (m_model->m_type == NFG)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->GetIboId());
	}

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
		glVertexAttribPointer(m_shader->a_uv, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
	}

	if (m_shader->u_MatWVP != -1)
	{
		glUniformMatrix4fv(m_shader->u_MatWVP, 1, 0, (float*)(&wvpMatrix));

	}
	if (m_shader->u_light != -1)
	{
		Vector3 vec_light;
		MainCharacter *bee = dynamic_cast <MainCharacter*> (this);
		if (bee != NULL)
		{
			vec_light = Vector3(8.0f, 8.0f, 8.0f);
		}
		else
		{
			vec_light = Vector3(ScenesManager::GetInstance()->GetCameraById("sceneCam")->GetPosition());
		}
		glUniform3fv(m_shader->u_light, 1, (float*)&vec_light);
	}
	if (m_shader->u_campos != -1)
	{
		glUniform3fv(m_shader->u_campos, 1, (float*)&InGame::GetInstance()->m_sceneCamera->GetPosition());
	}
	if (m_shader->u_otherdata != -1)
	{
		glUniform1f(m_shader->u_otherdata, m_otherData);
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

	if (m_shader->u_time != -1)
	{
		glUniform1f(m_shader->u_time, Timer::GameTime());
	}

	if (m_shader->u_color != -1)
	{
		glUniform3fv(m_shader->u_color, 1, (float*)(&ucolor));
	}

	//// make active a texture unit
	//unsigned short textureUnit = 0;
	//glActiveTexture(GL_TEXTURE0 + textureUnit); 
	//// bind the texture to the currently active texture unit
	//glBindTexture(target, m_tex[0]->GetTextureId());

	// set the uniform u_texture
	// (target == GL_TEXTURE_2D)

	for (int i = 0; i < m_NumTex; i++)
	{
		if (m_shader->u_tex[i] != -1)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_textures[i]->GetTextureId());
			glUniform1i(m_shader->u_tex[i], i);
		}
	}

	// (target == GL_TEXTURE_CUBE_MAP)
	if (m_shader->u_samplerCubeMap != -1)
	{

		// make active a texture unit
		unsigned short textureUnit = 0;
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		// bind the texture to the currently active texture unit
		glBindTexture(target, m_textures[0]->GetTextureId());
		glUniform1i(m_shader->u_samplerCubeMap, textureUnit);
	}
	
	m_shader->EnableState();
	if (m_model->m_type == NFG)
	{
		glDrawElements(GL_TRIANGLES, m_model->GetNumOfIndices(), GL_UNSIGNED_SHORT, 0);	
	}

	else if (m_model->m_type == OBJ)
	{
		glDrawArrays(GL_TRIANGLES, 0, m_model->GetNumOfVertices());	
	}
	
	m_shader->DisableState();

	// bind -> 0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (m_model->m_type == NFG)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(target, 0);

}

void Object::Draw(Matrix & vpMatrix, Vector4 & color)
{
	// use program
	glUseProgram(m_shader->program);

	// Calculate WVP matrix
	Matrix wvpMatrix;
	wvpMatrix = m_worldMatrix * vpMatrix;

	// binding buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVboId());
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_model->GetIboId());
	
	if (m_shader->a_position != -1)
	{
		glEnableVertexAttribArray(m_shader->a_position);
		glVertexAttribPointer(m_shader->a_position, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, pos));
	}

	if (m_shader->u_MatWVP != -1)
	{
		glUniformMatrix4fv(m_shader->u_MatWVP, 1, 0, (float*)(&wvpMatrix));

	}

	if (m_shader->u_color != -1)
	{
		glUniform4fv(m_shader->u_color, 1, (float*)(&color));
	}
	
	m_shader->EnableState();
	glDrawElements(GL_TRIANGLES, m_model->GetNumOfIndices(), GL_UNSIGNED_SHORT, 0);
	m_shader->DisableState();

	// bind -> 0
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//---------------------------------------------------
/// UPDATE
//---------------------------------------------------
void Object::Update()
{

}

//---------------------------------------------------
/// These functions follow are setters
//---------------------------------------------------
void Object::SetPosition(const Vector3 & pos)
{
	m_position = pos;
	CalculateWorldMatrix();
	CalculateCenter();
}

void Object::SetRotation(const Vector3 & rot)
{
	m_rotation = rot;
	CalculateWorldMatrix();
	CalculateCenter();
}

void Object::SetScale(const Vector3 & sca)
{
	m_scale = sca;
	CalculateWorldMatrix();
	CalculateCenter();

	// if m_scale.x == m_scale.y == m_scale.z
	m_radius = m_model->m_radius * m_scale.x;
	m_radiusX = m_model->m_radiusX * m_scale.x;
	m_radiusY = m_model->m_radiusY * m_scale.y;
	m_radiusZ = m_model->m_radiusZ * m_scale.z;
}

void Object::SetOtherData(float otherData)
{
	m_otherData = otherData;
}

void Object::SetTexture(Texture * texture)
{
	m_textures[0] = texture;
}

void Object::SetModel( Model * model)
{
	m_model = model;
	UpdateTextureUV();
}

void Object::SetShader(Shader * shader)
{
	m_shader = shader;
}

void Object::SetWorldMatrix(const Matrix & wMat)
{
	m_worldMatrix = wMat;
}

//---------------------------------------------------
/// These functions follow are getters
//---------------------------------------------------
const char* Object::GetId()
{
	return m_id;
}

const Vector3 & Object::GetPosition()
{
	return m_position;
}

const Vector3 & Object::GetRotation()
{
	return m_rotation;
}

const Vector3 & Object::GetScale()
{
	return m_scale;
}

Texture * Object::GetTexture()
{
	return m_textures[0];
}

Model * Object::GetModel()
{
	return m_model;
}

Shader * Object::GetShader()
{
	return m_shader;
}

const Matrix & Object::GetWorldMatrix()
{
	return m_worldMatrix;
}

//---------------------------------------------------
/// CalculateCenter
//---------------------------------------------------
void Object::CalculateCenter()
{
	Vector4 temp = Vector4(m_model->m_center, 1.0f);
	temp = temp * m_worldMatrix;
	m_center = temp.xyz;
}

//---------------------------------------------------
/// Deconstructor
//---------------------------------------------------
Object::~Object(void)
{
	SAFE_DEL_ARRAY(m_textures);
}

//---------------------------------------------------
/// Update Textcoord (TextureUV)
//---------------------------------------------------
void Object::UpdateTextureUV()
{
	// set uv (use big texture)
	// OBJ model
	if (m_textureUV == NULL && m_model->m_type == NFG)
	{
		//LOGI("This object %s (NFG model) doesn't have m_textureUV\n", m_id);
		//return;
	}

	else if (m_model->m_updateData == NULL)
	{
		//LOGI("This object %s->m_model is not reusable.\n", m_id);
		//return;
	}

	else
	{
		// prepare data
		int numOfVertices = m_model->GetNumOfVertices();

		memcpy(m_model->m_updateData, m_model->m_verticesData, sizeof(Vertex) * numOfVertices);

		if (m_model->m_type == OBJ)
		{
			TextureUV *textureUV;
			
			for (int j = 0; j < m_model->m_numOfMaterial; j++)
			{
				for (int i = m_model->m_material[j].offset; i < m_model->m_material[j].offset + m_model->m_material[j].length; i++)
				{
					textureUV = (TextureUV*)m_textures[0]->GetTextureUvById(m_model->m_material[j].textureUvId);
				
					m_model->m_updateData[i].uv.x = m_model->m_verticesData[i].uv.x * textureUV->w + textureUV->x;
					m_model->m_updateData[i].uv.y = m_model->m_verticesData[i].uv.y * textureUV->h + textureUV->y;
				}
			}
			LOGI("Update vertex %s\n", m_model->GetId());
		}

		// NFG model
		else if (m_model->m_type == NFG)
		{
			for (int i = 0; i < numOfVertices; i++)
			{
				m_model->m_updateData[i].uv.x = m_model->m_verticesData[i].uv.x * m_textureUV->w + m_textureUV->x;
				m_model->m_updateData[i].uv.y = m_model->m_verticesData[i].uv.y * m_textureUV->h + m_textureUV->y;
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, m_model->GetVboId());
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * numOfVertices, m_model->m_updateData, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		if (m_model->m_isReusable == false)
		{
			// no more update
			m_model->DeleteVerticesData();
		}
	}
}

//---------------------------------------------------
/// Set TextureUV
//---------------------------------------------------
void Object::SetTextureUV(const char *textureUvId)
{
	if (textureUvId == NULL)
	{
		m_textureUV = NULL;	
	}

	else
	{
		m_textureUV = (TextureUV*)m_textures[0]->GetTextureUvById(textureUvId);
		UpdateTextureUV();
	}
}
