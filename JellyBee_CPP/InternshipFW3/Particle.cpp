#include "Particle.h"
#include "Timer.h"
#include "ScenesManager.h"
#include "Camera.h"

Particle::Particle(void)
{
	// Resource
	m_model = 0;
	m_shader = 0;
	m_texture = 0;

	// Parameter
	m_startPosition = Vector3(0.0f, 0.0f, 0.0f);
	m_startVelocity = Vector3(0.0f, 0.0f, 0.0f);
	m_startScale = Vector3(0.0f, 0.0f, 0.0f);

	m_position = Vector3(0.0f, 0.0f, 0.0f);
	m_velocity = Vector3(0.0f, 0.0f, 0.0f);
	m_acceleration = Vector3(0.0f, 0.0f, 0.0f);
	m_timeAlive = 0;
	m_lifeTime = 0;
	m_isActive = false;
}

Particle::~Particle(void)
{
}

void Particle::InitResource(Model *model, Shader *shader, Texture *texture)
{
	m_model = model;
	m_shader = shader;
	m_texture = texture;
}

void Particle::InitParameter(Vector3 position, Vector3 scale, Vector3 velocity, Vector3 acceleration, float lifeTime, float timeAlive)
{
	// Backup
	m_startPosition = position;
	m_startVelocity = velocity;
	m_startScale = scale;

	m_position = position;
	m_scale = scale;
	m_velocity = velocity;
	m_acceleration = acceleration;
	m_lifeTime = lifeTime;
	m_timeAlive = timeAlive;
	m_isActive = true; // not used
}

void Particle::ResetParameter()
{
	m_position = m_startPosition;
	m_velocity = m_startVelocity;
	m_scale = m_startScale;
	m_timeAlive = 0;
}

void Particle::Update()
{
	float deltaTime = Timer::FrameTime();

	if (m_timeAlive < m_lifeTime && m_timeAlive >= 0)
	{	
		//m_velocity = m_velocity + m_acceleration * deltaTime;
		m_position = m_position + m_velocity * deltaTime;
		m_timeAlive = m_timeAlive + deltaTime;

		if (m_timeAlive > m_lifeTime/3)
		{
			m_scale = m_scale * (1 - m_timeAlive/m_lifeTime);
		}
	}


	else if (m_timeAlive > m_lifeTime) // m_timeAlive > m_lifeTime (particle died)
	{
		ResetParameter();
		//m_isActive = false;
	}
	else if (m_timeAlive < 0)
	{
		m_timeAlive = m_timeAlive + deltaTime;
	}

}

void Particle::Draw()
{
	if (m_timeAlive > 0)
	{
		// Draw quad
		
		// Update UV if needed

		int target = m_texture->GetTarget();
		glUseProgram(m_shader->program);

		// Calculate WVP matrix
		Matrix wvpMatrix = CalculateWorldMatrix() * ScenesManager::GetInstance()->GetCameraById("sceneCam")->CalculateVPMatrix();
		//wvpMatrix = m_worldMatrix * vpMatrix;

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
		
		if (m_shader->a_uv != -1)
		{
			glEnableVertexAttribArray(m_shader->a_uv);
			glVertexAttribPointer(m_shader->a_uv, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, uv));
		}

		if (m_shader->u_MatWVP != -1)
		{
			glUniformMatrix4fv(m_shader->u_MatWVP, 1, 0, (float*)(&wvpMatrix));

		}
		
		if (m_shader->u_time != -1)
		{
			glUniform1f(m_shader->u_time, 1 - m_timeAlive/m_lifeTime);
		}

		if (m_shader->u_tex[0] != -1)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_texture->GetTextureId());
			glUniform1i(m_shader->u_tex[0], 0);
		}
		
		m_shader->EnableState();
		glDepthMask(GL_FALSE);
		glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
		if (m_model->m_type == NFG)
		{
			glDrawElements(GL_TRIANGLES, m_model->GetNumOfIndices(), GL_UNSIGNED_SHORT, 0);	
		}

		else if (m_model->m_type == OBJ)
		{
			glDrawArrays(GL_TRIANGLES, 0, m_model->GetNumOfVertices());	
		}

		glDepthMask(GL_TRUE);
		
		m_shader->DisableState();

		// bind -> 0
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		if (m_model->m_type == NFG)
		{
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); // IBO
		}

		glBindTexture(target, 0);
	}
}

Matrix Particle::CalculateWorldMatrix()
{
	Matrix rotation, scale, translation, worldMatrix;

	//rotation.SetRotation(m_rotation);
	scale.SetScale(m_scale);
	translation.SetTranslation(m_position);

	worldMatrix = scale * rotation * translation;

	return worldMatrix;
}