#pragma once
#include "matrix.h"
#include "ScreenResolution.h"

class CameraLookat
{
private:
	Vector3 m_position;
	Vector3 m_target, m_up;
	GLfloat m_near, m_far, m_fovY, m_aspect; 
	GLfloat m_speed;
	Matrix m_projectMatrix, m_worldMatrix;
	char m_id[MAX_10];

public:
	CameraLookat(void);
	virtual ~CameraLookat(void);
	void MoveCamera(Direction dir);
	void RotateCamera(Direction dir);
	void Init(const char * id, const Vector3 & pos, GLfloat fovY, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane, GLfloat speed);
	void Update();
	Matrix CalculateVPMatrix();
	Matrix CalculateWorldMatrix();

	// Setters
	void SetTarget(const Vector3 & target);
	void SetVectorUp(const Vector3 & up);
	void SetPosition(const Vector3 & pos);
	void SetSpeed(const GLfloat & speed);
	void SetProjectMatrix(GLfloat fovY, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane);

	// Getters
	Vector3 & GetPosition();
	Vector3 & GetTarget();
	const Matrix & GetWorldMatrix();
	const char* GetId();
};
