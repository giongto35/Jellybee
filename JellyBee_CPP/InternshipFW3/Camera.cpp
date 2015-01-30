#include "Camera.h"
#include "Timer.h"
#include <string.h>
#include "InputManager.h"
#include "ScreenResolution.h"
#include "math.h"
#include "ScenesManager.h"

InputManager* imToCamara = NULL;

Camera::Camera(void)
{
	m_position = Vector3(0.0f, 0.0f, 0.0f);
	m_rotation = Vector3(0.0f, 0.0f, 0.0f);
	m_rotationFake = Vector3(0.0f, 0.0f, 0.0f);
	m_speed = 1.0f;
	m_id[0] = '\0';

	m_projectMatrix.SetPerspective(PI / 4, ScreenResolution::GetInstance()->GetRatio(), 0.1f, 500.0f);
}

//---------------------------------------------------
/// INITIALIZE
//---------------------------------------------------
void Camera::Init(const char * id, const Vector3 & pos, GLfloat fovY, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane, GLfloat speed)
{
	strcpy(m_id, id);
	SetPosition(pos);
	SetSpeed(speed);
	m_projectMatrix.SetPerspective(fovY, aspect, nearPlane, farPlane);

	imToCamara = InputManager::GetInstance();
}

//---------------------------------------------------
/// UPDATE
//---------------------------------------------------
void Camera::Update()
{
	//if (imToCamara->IsDrag())
	//{
	//	int dx = imToCamara->GetDx();
	//	int dy = imToCamara->GetDy();
	//	m_speed = sqrt(float (dx * dx + dy * dy)) / 4;

	//	if (dx < 0)
	//	{
	//		//RotateCamera(LEFT);
	//	}
	//	else if (dx > 0)
	//	{
	//		//RotateCamera(RIGHT);
	//	}

	//	if (dy < 0)
	//	{
	//		//RotateCamera(UP);			
	//		MoveCamera(DOWN);
	//	}
	//	else if (dy > 0)
	//	{
	//		//RotateCamera(DOWN);
	//		MoveCamera(UP);
	//	}
	//}

	// movement
	
	//if (imToCamara->IsKeyDown(87))
	//{
	//	MoveCamera(DOWN);
	//}
	//else if (imToCamara->IsKeyDown(83))
	//{
	//MoveCamera(FORWARD);
	//}

	//if (imToCamara->IsKeyDown(65))
	//{
	//	MoveCamera(LEFT);
	//}
	//else if (imToCamara->IsKeyDown(68))
	//{
	//	MoveCamera(RIGHT);
	//}

	// rotation
	if (imToCamara->IsKeyDown(38))
	{
		//RotateCamera(UP);
		MoveCamera(UP);
	}
	else if (imToCamara->IsKeyDown(40))
	{
		//RotateCamera(DOWN);
		MoveCamera(DOWN);
	}

	if (imToCamara->IsKeyDown(37))
	{
		RotateCamera(LEFT);
	}
	else if (imToCamara->IsKeyDown(39))
	{
		RotateCamera(RIGHT);
	}

	/*
	if (m_position.y < 0.05f)
	{
		Vector4 moveL = Vector4(0, 0.1f, 0, 1);
		Vector4 camNewPos = moveL * CalculateWorldMatrix();
		SetPosition(Vector3(camNewPos.x, camNewPos.y, camNewPos.z));
	}

	if (m_position.y > 1.0f)
	{
		Vector4 moveL = Vector4(0, -0.3f, 0, 1);
		Vector4 camNewPos = moveL * CalculateWorldMatrix();
		SetPosition(Vector3(camNewPos.x, camNewPos.y, camNewPos.z));
	}
	*/

	/*
	Vector4 moveL = Vector4(0, 0, -0.2, 1);
	Vector4 MainCharacterPosition;
	
	MainCharacterPosition = moveL  * ScenesManager::GetInstance()->GetCameraById("sceneCam")->CalculateWorldMatrix();

	if (MainCharacterPosition.y < 0.05f)
	{
		Vector4 moveL = Vector4(0, 0.1f, 0, 1);
		Vector4 camNewPos = moveL * CalculateWorldMatrix();
		SetPosition(Vector3(camNewPos.x, camNewPos.y, camNewPos.z));
	}
	*/
}

//---------------------------------------------------
/// Camera movement
//---------------------------------------------------
void Camera::MoveCamera(Direction dir)
{
	GLfloat delta = Timer::FrameTime() * m_speed;
	Vector4 moveL;

	switch(dir)
	{
		case LEFT:
		{
			moveL = Vector4(-delta, 0, 0, 1);
		}
		break;

		case RIGHT:
		{
			moveL = Vector4(delta, 0, 0, 1);
		}
		break;

		case FORWARD:
		{
			moveL = Vector4(0, 0, -delta, 1);
		}
		break;

		case BACKWARD:
		{
			moveL = Vector4(0, 0, delta, 1);
		}
		break;

		case UP:
		{
			moveL = Vector4(0, delta, 0, 1);
			m_rotationFake.x += delta;
		}
		break;

		case DOWN:
		{
			moveL = Vector4(0, -delta, 0, 1);
			m_rotationFake.x -= delta;
		}
		break;

		default:
			return;
			//LOGE("ERROR : unexpected value\n");
	}

	Vector4 moveW = moveL * CalculateWorldMatrix();
	m_position = Vector3(moveW.x, min(max(moveW.y, 0.05f), 0.75f), moveW.z);
}

//---------------------------------------------------
/// Camera rotation
//---------------------------------------------------
void Camera::RotateCamera(Direction dir)
{
	GLfloat a = Timer::FrameTime() * m_speed;

	switch(dir)
	{
		case UP:
		{
			m_rotation.x += a;
		}
		break;

		case DOWN:
		{
			m_rotation.x -= a;
		}
		break;

		case LEFT:
		{
			m_rotation.y += a;
		}
		break;

		case RIGHT:
		{
			m_rotation.y -= a;
		}
		break;

		default:
			LOGE("ERROR : unexpected value\n");
	}
}

Matrix Camera::CalculateWorldMatrix()
{
	Matrix trans, rot;

	trans.SetTranslation(m_position);
	rot.SetRotation(m_rotation);

	m_worldMatrix = rot * trans;

	return m_worldMatrix;
}

Matrix Camera::CalculateVPMatrix()
{
	Matrix rot, trans;

	trans.SetTranslation(-m_position);

	// Yaw-Pitch-Roll rule
	Matrix rotationXMatrix, rotationYMatrix, rotationZMatrix;
	rotationXMatrix.SetRotationX(-m_rotation.x);
	rotationYMatrix.SetRotationY(-m_rotation.y);
	rotationZMatrix.SetRotationZ(-m_rotation.z);
	rot = rotationYMatrix * rotationXMatrix * rotationZMatrix;

	return (trans * rot * m_projectMatrix);
}

//---------------------------------------------------
/// These functions follow are setters
//---------------------------------------------------
void Camera::SetPosition(const Vector3 & pos)
{
	m_position = pos;
	CalculateVPMatrix();
}
void Camera::SetRotation(const Vector3 & rot)
{
	m_rotation = rot;
	CalculateVPMatrix();
}

void Camera::SetSpeed(const GLfloat & speed)
{
	m_speed = speed;
}

void Camera::SetProjectMatrix(const Matrix & matrix)
{
	m_projectMatrix = matrix;
}

//---------------------------------------------------
/// Getters
//---------------------------------------------------
const char* Camera::GetId()
{
	return m_id;
}

Vector3 & Camera::GetPosition()
{
	return m_position;
}

Vector3 & Camera::GetRotation()
{
	return m_rotation;
}

const Matrix & Camera::GetWorldMatrix()
{
	return m_worldMatrix;
}

//---------------------------------------------------
/// Deconstructor
//---------------------------------------------------
Camera::~Camera(void)
{
}
