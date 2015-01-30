#include "Emitter.h"
#include <ctime>
#include <math.h>

float RandomFloat(float min, float max);

Emitter::Emitter(void)
{
	//Resource
	m_model = 0;
	m_shader = 0;
	m_texture = 0;
	m_particles = 0;
	m_numOfParticle = 0;
	m_type = 0;

	// Parameter
	m_position = Vector3(0.0f, 0.0f, 0.0f);
	m_velocity = Vector3(0.0f, 0.0f, 0.0f);
	m_acceleration = Vector3(0.0f, 0.0f, 0.0f);
	m_force = Vector3(0.0f, 0.0f, 0.0f);
	m_lifeTime = 0;
}

Emitter::~Emitter(void)
{
	for (int i = 0; i < m_numOfParticle; i++)
	{
		SAFE_DEL(m_particles[i]);
	}

	SAFE_DEL(m_particles);
}

void Emitter::InitResource(Model *model, Shader *shader, Texture *texture)
{
	m_model = model;
	m_shader = shader;
	m_texture = texture;
}

void Emitter::InitParameter(int numOfParticle, Vector3 position, Vector3 scale, Vector3 velocity, Vector3 force, Vector3 acceleration, int type, float lifeTime)
{
	/*if (m_particles != NULL)
	{
		SAFE_DEL(m_particles);
	}*/

	m_numOfParticle = numOfParticle;

	m_particles = new Particle* [m_numOfParticle];

	for (int i = 0; i < m_numOfParticle; i++)
	{
		m_particles[i] = new Particle;
	}

	m_position = position;
	m_scale = scale;
	m_velocity = velocity;
	m_force = force;
	m_acceleration = acceleration;
	m_type = type;
	m_lifeTime = lifeTime;

	srand(time(0));

	for (int i = 0; i < m_numOfParticle; i++)
	{
		m_particles[i]->InitResource(m_model, m_shader, m_texture);
		InitParticleParameter(i);
	}

	//if (m_type == PS_FIREWORK)
	//{
	//	Vector3 randomForce;
	//	Vector3 tempVelocity;
	//	Vector3 tempAcceleration;

	//	srand(time(0));

	//	for (int i = 0; i < m_numOfParticle; i++)
	//	{
	//		// Prepare temp data
	//		randomForce.x = RandomFloat(-1, 1);
	//		randomForce.y = RandomFloat(-1, 1);
	//		randomForce.z = RandomFloat(-1, 1);

	//		tempVelocity = Vector3(randomForce.x * m_velocity.x, 
	//								randomForce.y * m_velocity.y, 
	//								randomForce.z * m_velocity.z);

	//		if (tempAcceleration.x * tempVelocity.x < 0)
	//		{
	//			tempAcceleration.x = - tempAcceleration.x;
	//		}

	//		if (tempAcceleration.y * tempVelocity.y < 0)
	//		{
	//			tempAcceleration.y = - tempAcceleration.y;
	//		}

	//		if (tempAcceleration.z * tempVelocity.z < 0)
	//		{
	//			tempAcceleration.z = - tempAcceleration.z;
	//		}

	//		// Set
	//		m_particles[i]->InitResource(m_model, m_shader, m_texture);
	//		m_particles[i]->InitParameter(m_position, tempVelocity, m_acceleration, m_lifeTime);
	//	}
	//}
}

void Emitter::Update()
{

	for (int i = 0; i < m_numOfParticle; i++)
	{
		m_particles[i]->Update();
	}
}

void Emitter::Draw()
{
	for (int i = 0; i < m_numOfParticle; i++)
	{
		m_particles[i]->Draw();
	}
}

void Emitter::InitParticleParameter(int index)
{
	Vector3 randomForce;
	Vector3 tempVelocity;
	Vector3 tempAcceleration = m_acceleration;

	if (m_type == PS_FIREWORK)
	{
		{
			// Prepare temp data
			randomForce.x = RandomFloat(-1, 1);
			randomForce.y = RandomFloat(-1, 1);
			randomForce.z = RandomFloat(-1, 1);
		}
	}

	else if (m_type == PS_CIRCLE)
	{
		float angle = index * ((2 * PI) / (m_numOfParticle/PS_PART));
		randomForce.x = cos(angle);
		randomForce.y = sin(angle);
		randomForce.z = RandomFloat(-1, 1);	
	}
	
	else if (m_type == PS_POLAR_ROSE)
	{
		float angle = index * ((2 * PI) / (m_numOfParticle/PS_PART));

		randomForce.x = cos(4 * angle) * sin(angle);
		randomForce.y = cos(4 * angle) * cos(angle);
		randomForce.z = RandomFloat(-1, 1);
	}

	else if (m_type == PS_HEART)
	{
		float angle = index * ((2 * PI) / (m_numOfParticle/PS_PART));

		randomForce.x = 3.9 * pow(sin(angle), 3.0f);
		randomForce.y = 3 * cos(angle) - 1.2 * cos(2 * angle) - 0.6 * cos(3 * angle) - 0.2 * cos(4 * angle);
		randomForce.z = RandomFloat(-1, 1);
	}

	tempVelocity = Vector3(randomForce.x * m_velocity.x, 
									randomForce.y * m_velocity.y, 
									randomForce.z * m_velocity.z);

	if (tempAcceleration.x * tempVelocity.x < 0)
	{
		tempAcceleration.x = - tempAcceleration.x;
	}

	if (tempAcceleration.y * tempVelocity.y < 0)
	{
		tempAcceleration.y = - tempAcceleration.y;
	}

	if (tempAcceleration.z * tempVelocity.z < 0)
	{
		tempAcceleration.z = - tempAcceleration.z;
	}

	m_particles[index]->InitParameter(m_position, m_scale, tempVelocity, tempAcceleration, m_lifeTime, - index / (m_numOfParticle/PS_PART));// - index * 0.01);
}

float RandomFloat(float min, float max)
{
	//return (min + (rand() % (int)(max - min + 1))); // nghieng ve min nhieu hon
	//return ((float) rand() % (max-min)) + min;
	//return (float)(rand() % 10000) / 5000 - 1;

	int precision = 3;
	int n = 1;
	int r;
	for (int i = 0; i < precision; ++i)
	{
		n *= 10;
	}
	min *= n;
	max *= n;
	r = rand() % (int)(max - min + 1) + min;
	//LOGI("%.3f\n", r / (float) n);
	return (r / (float) n);
}