#pragma once

#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "Vector3.h"
#include "Particle.h"

#define PS_PART 3

class Emitter
{
private:
	Model *m_model;
	Shader *m_shader;
	Texture *m_texture;
	int m_type;

	Particle **m_particles;
	int m_numOfParticle;

public:
	Vector3 m_position;
	Vector3 m_scale;
	Vector3 m_velocity;
	Vector3 m_force;
	Vector3 m_acceleration;
	float m_lifeTime;

public:
	Emitter(void);
	~Emitter(void);

	void InitResource(Model *model, Shader *shader, Texture *texture);
	void InitParameter(int numOfParticle, Vector3 position, Vector3 scale, Vector3 velocity, Vector3 force, Vector3 acceleration, int type, float lifeTime);

	void Update();
	void Draw();

	void InitParticleParameter(int index);

};
