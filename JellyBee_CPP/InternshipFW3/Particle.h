#pragma once

#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "Vector3.h"

class Particle
{
private:
	Model *m_model;
	Shader *m_shader;
	Texture *m_texture;

public:
	Vector3 m_startPosition;
	Vector3 m_startVelocity;
	Vector3 m_startScale;

	Vector3 m_position;
	Vector3 m_scale;
	Vector3 m_velocity;
	Vector3 m_acceleration;

	float m_timeAlive; // The amount of time that this particle has been alive.
	float m_lifeTime;
	bool m_isActive; // not used

public:
	Particle(void);
	~Particle(void);

	void InitResource(Model *model, Shader *shader, Texture *texture);
	void InitParameter(Vector3 position, Vector3 velocity, Vector3 scale, Vector3 acceleration, float lifeTime, float timeAlive = 0);

	void Update();
	void Draw();

	void ResetParameter();

	Matrix CalculateWorldMatrix();
};
