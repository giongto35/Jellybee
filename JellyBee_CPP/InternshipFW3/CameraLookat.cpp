#include "CameraLookat.h"
#include "Timer.h"
#include <string.h>
#include "InputManager.h"
#include "ScenesManager.h"
#include "Object.h"
#include "ScreenResolution.h"
#include "math.h"

InputManager* imToCamaraLookat = NULL;

CameraLookat::CameraLookat(void)
{
	m_up = Vector3(0.0f, 1.0f, 0.0f);
	m_position = Vector3(0.0f, 0.0f, 0.0f);
	m_speed = 1.0f;
	m_id[0] = '\0';

	m_projectMatrix.SetPerspective(PI / 4, ScreenResolution::GetInstance()->GetRatio(), 0.1f, 500.0f);
	imToCamaraLookat = InputManager::GetInstance();
}

//---------------------------------------------------
/// INITIALIZE
//---------------------------------------------------
void CameraLookat::Init(const char * id, const Vector3 & pos, GLfloat fovY, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane, GLfloat speed)
{
	strcpy(m_id, id);
	SetPosition(pos);
	SetSpeed(speed);
	SetProjectMatrix(fovY, aspect, nearPlane, farPlane);

	imToCamaraLookat = InputManager::GetInstance();
}

//---------------------------------------------------
/// UPDATE
//---------------------------------------------------
void CameraLookat::Update()
{
	if (imToCamaraLookat->IsDrag())
	{
		int dx = imToCamaraLookat->GetDx();
		int dy = imToCamaraLookat->GetDy();
		m_speed = sqrt(float (dx * dx + dy * dy)) / 4;

		if (dx < 0)
		{
			RotateCamera(LEFT);
		}
		else if (dx > 0)
		{
			RotateCamera(RIGHT);
		}

		if (dy < 0)
		{
			RotateCamera(UP);			
		}
		else if (dy > 0)
		{
			RotateCamera(DOWN);
		}
	}

	// movement
	m_speed = 1.0f;
	//MoveCamera(FORWARD);
	if (imToCamaraLookat->IsKeyDown(87))
	{
		MoveCamera(FORWARD);
	}
	else if (imToCamaraLookat->IsKeyDown(83))
	{
		MoveCamera(BACKWARD);
	}

	if (imToCamaraLookat->IsKeyDown(65))
	{
		MoveCamera(LEFT);
	}
	else if (imToCamaraLookat->IsKeyDown(68))
	{
		MoveCamera(RIGHT);
	}

}

//---------------------------------------------------
/// Camera movement
//---------------------------------------------------
void CameraLookat::MoveCamera(Direction dir)
{
	GLfloat delta = Timer::FrameTime() * m_speed;
	Vector3 zaxis = (m_position - m_target).Normalize();
	Vector3 xaxis = (m_up.Cross(zaxis)).Normalize();
	Vector3 deltaMove;

	switch(dir)
	{
		case LEFT:
		{
			deltaMove = -xaxis * delta;
		}
		break;

		case RIGHT:
		{
			deltaMove = xaxis * delta;
		}
		break;

		case FORWARD:
		{
			deltaMove = -zaxis * delta;
		}
		break;

		case BACKWARD:
		{
			deltaMove = zaxis * delta;
		}
		break;

		default:
			LOGE("ERROR : unexpected value\n");
	}

	m_position += deltaMove;
	m_target += deltaMove;
}

//---------------------------------------------------
/// Camera rotation
//---------------------------------------------------
void CameraLookat::RotateCamera(Direction dir)
{
	GLfloat a = Timer::FrameTime() * m_speed;
	Vector4 localTarget = Vector4(0, 0, -(m_position - m_target).Length(), 1);
	Vector4 localNewTarget;

	switch(dir)
	{
		case UP:
		{
			localNewTarget = localTarget * Matrix().SetRotationX(a);
		}
		break;

		case DOWN:
		{
			localNewTarget = localTarget * Matrix().SetRotationX(-a);
		}
		break;

		case LEFT:
		{
			localNewTarget = localTarget * Matrix().SetRotationY(-a);
		}
		break;

		case RIGHT:
		{
			localNewTarget = localTarget * Matrix().SetRotationY(a);
		}
		break;

		default:
			LOGE("ERROR : unexpected value\n");
	}
	localNewTarget = localNewTarget * m_worldMatrix;
	m_target = Vector3(localNewTarget.x, localNewTarget.y, localNewTarget.z);

}

Matrix CameraLookat::CalculateWorldMatrix()
{
	Vector3 zaxis = (m_position - m_target).Normalize();
	Vector3 xaxis = (m_up.Cross(zaxis)).Normalize();
	Vector3 yaxis = (zaxis.Cross(xaxis)).Normalize();
	
	Matrix rotationMatrix, translationMatrix;

	rotationMatrix.SetRightVector(xaxis);
	rotationMatrix.SetUpVector(yaxis);
	rotationMatrix.SetForwardVector(zaxis);
	rotationMatrix.SetTranslationVector(Vector3(0.0f, 0.0f, 0.0f));

	translationMatrix.SetTranslation(m_position);
	m_worldMatrix = rotationMatrix * translationMatrix;

	return m_worldMatrix;
}

Matrix CameraLookat::CalculateVPMatrix()
{
	/*Vector3 zaxis = ( m_position - m_target ).Normalize();
	Vector3 xaxis = ( m_up.Cross( zaxis )).Normalize();
	Vector3 yaxis = ( zaxis.Cross( xaxis ) ).Normalize();
	
	Matrix rotationMatrix, translationMatrix;

	rotationMatrix.SetRightVector(-xaxis);
	rotationMatrix.SetUpVector(-yaxis);
	rotationMatrix.SetForwardVector(-zaxis);
	rotationMatrix.SetTranslationVector(Vector3(.0f, 0.0f, 0.0f));

	translationMatrix.SetTranslation( -m_position );*/
	//Matrix rot, trans;

	//trans.SetTranslation(-m_position);

	//// Yaw-Pitch-Roll rule
	//Matrix rotationXMatrix, rotationYMatrix, rotationZMatrix;
	//rotationXMatrix.SetRotationX(-m_rotation.x);
	//rotationYMatrix.SetRotationY(-m_rotation.y);
	//rotationZMatrix.SetRotationZ(-m_rotation.z);
	//rot = rotationYMatrix * rotationXMatrix * rotationZMatrix;

	//return (translationMatrix * rotationMatrix * m_projectMatrix);
	return m_worldMatrix.Inverse() * m_projectMatrix;
}

//---------------------------------------------------
/// These functions follow are setters
//---------------------------------------------------
void CameraLookat::SetTarget(const Vector3 & target)
{
	m_target = target;
	CalculateWorldMatrix();
}

void CameraLookat::SetVectorUp(const Vector3 & up)
{
	m_up = up;
}

void CameraLookat::SetPosition(const Vector3 & pos)
{
	m_position = pos;
}

void CameraLookat::SetSpeed(const GLfloat & speed)
{
	m_speed = speed;
}

void CameraLookat::SetProjectMatrix(GLfloat fovY, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane)
{
	m_near = nearPlane;
	m_far = farPlane;
	m_fovY = fovY;
	m_aspect = aspect;

	m_projectMatrix.SetPerspective(fovY, aspect, nearPlane, farPlane);
}

//---------------------------------------------------
/// Getters
//---------------------------------------------------
const char* CameraLookat::GetId()
{
	return m_id;
}

Vector3 & CameraLookat::GetPosition()
{
	return m_position;
}

Vector3 & CameraLookat::GetTarget()
{
	return m_target;
}

const Matrix & CameraLookat::GetWorldMatrix()
{
	return m_worldMatrix;
}

//---------------------------------------------------
/// Deconstructor
//---------------------------------------------------
CameraLookat::~CameraLookat(void)
{
}

