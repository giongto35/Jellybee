#pragma once
#include "Platforms.h"
#include "Vector4.h"
#define DEGREES_TO_RADIANS(x) (x*0.017453292f)
class Matrix;

class Matrix
{
public:
	//constructors
	Matrix(){this->SetIdentity();}
	Matrix(GLfloat val);
	Matrix( const Matrix & mat);

	// Matrix operations
	Matrix & SetZero();
	Matrix & SetIdentity();
	
	void SetUpVector(const Vector4 & up);
	void SetForwardVector(const Vector4 & forward);
	void SetRightVector(const Vector4 & right);
	void SetTranslationVector(const Vector4 & translation);

	void SetUpVector(const Vector3 & up);
	void SetForwardVector(const Vector3 & forward);
	void SetRightVector(const Vector3 & right);
	void SetTranslationVector(const Vector3 & translation);
	
	Matrix & SetLookAt(const Vector3 & eyePosition, const Vector3 & target, const Vector3 &up = Vector3(0.0f, 1.0f, 0.0f));
	Matrix & SetRotationX(GLfloat angle);
	Matrix & SetRotationY(GLfloat angle);
	Matrix & SetRotationZ(GLfloat angle);
	Matrix & SetRotationAngleAxis( float angle, float x, float y, float z );
	Matrix & SetRotation( float x, float y, float z );
	//Rotaion with roll - pitch - yaw rules
	Matrix & SetRotation( const Vector3 & rotation );

	Matrix & SetScale(GLfloat scale);
	Matrix & SetScale(GLfloat scaleX, GLfloat scaleY, GLfloat scaleZ);
	Matrix & SetScale(GLfloat * pScale);
	Matrix & SetScale( const Vector3 &scaleVec);

	Matrix & SetTranslation(GLfloat x, GLfloat y, GLfloat z);
	Matrix & SetTranslation(GLfloat *pTrans);
	Matrix & SetTranslation( const Vector3 &vec);
	Matrix & SetTranspose();
	Matrix & SetInverse();
	Matrix & SetPerspective(GLfloat fovY, GLfloat aspect, GLfloat nearPlane, GLfloat farPlane);
	Matrix& SetOrtho(float left, float right, float bottom, float top, float near, float far);
	GLfloat Determinant() const;
	Matrix Transpose() const;
	Matrix Inverse() const;

	Matrix operator + ( const Matrix & mat) const ;
	Matrix & operator += ( const Matrix & mat);
	Matrix operator - ( const Matrix & mat) const ;
	Matrix &operator -= ( const Matrix & mat);

	Matrix operator * ( const Matrix & mat) const ;
	Matrix operator * (GLfloat k) const ;
	Matrix & operator *= (GLfloat k);

	Vector4 operator * ( const Vector4 & vec) const ;

	Matrix & operator = ( const Matrix & mat);
	
	//data members
	union
	{
		GLfloat mm[4][4];
		GLfloat m[16];
		struct
		{
			GLfloat rightVector[4];
			GLfloat upVector[4];
			GLfloat forwardVector[4];
			GLfloat translationVector[4];
		};
		struct 
		{
			GLfloat _00, _01, _02, _03;
			GLfloat _10, _11, _12, _13;
			GLfloat _20, _21, _22, _23;
			GLfloat _30, _31, _32, _33;
		};
	};
};
