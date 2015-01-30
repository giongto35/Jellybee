#pragma once
#include "Platforms.h"
#include "matrix.h"
#include "ScreenResolution.h"

class Camera
{
public:
	Vector3 m_position, m_rotation;
	GLfloat m_near, m_far, m_fovY, m_aspect; 
	GLfloat m_speed;
	Matrix m_projectMatrix, m_worldMatrix;
	char m_id[MAX_10];
	Vector3 m_rotationFake;

public:
	Camera(void);
	virtual ~Camera(void);
	void MoveCamera(Direction dir);
	void RotateCamera(Direction dir);
	void Init(const char * id, const Vector3 & pos, GLfloat fovY, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane, GLfloat speed);
	void Update();
	Matrix CalculateVPMatrix();
	Matrix CalculateWorldMatrix();

	// Setters
	void SetPosition(const Vector3 & pos);
	void SetRotation(const Vector3 & rot);
	void SetSpeed(const GLfloat & speed);
	void SetProjectMatrix(const Matrix & matrix);

	// Getters
	Vector3 & GetPosition();
	Vector3 & GetRotation();
	const Matrix & GetWorldMatrix();
	const char* GetId();
};
