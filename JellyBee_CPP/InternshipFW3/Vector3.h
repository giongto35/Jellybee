#pragma once
#include "Vector2.h"
//Vector3

class Vector3
{
public:
	//Constructors
	Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(GLfloat _x, GLfloat _y, GLfloat _z) : x(_x), y(_y), z(_z) {}
	Vector3(GLfloat * pArg) : x(pArg[0]), y(pArg[1]), z(pArg[2]) {}
	Vector3( const Vector3 & vector) : x(vector.x), y(vector.y), z(vector.z) {}
	//Vector's operations
	GLfloat Length() const ;
	Vector3 & Normalize();
	Vector3 operator + ( const Vector3 & vector) const ;
	Vector3 & operator += ( const Vector3 & vector);
	Vector3 operator - () const ;
	Vector3 operator - ( const Vector3 & vector) const ;
	Vector3 & operator -= ( const Vector3 & vector);
	Vector3 operator * (GLfloat k) const ;
	Vector3 & operator *= (GLfloat k);
	Vector3 operator / (GLfloat k) const ;
	Vector3 & operator /= (GLfloat k);
	Vector3 & operator = ( const Vector3 & vector);
	Vector3 & operator = (GLfloat* pArg);
	bool operator == (const Vector3 & vector) const;
	Vector3 Modulate( const Vector3 & vector) const ;
	GLfloat Dot( const Vector3 & vector) const ;
	Vector3 Cross( const Vector3 & vector) const ;

	//access to elements
	GLfloat operator [] (unsigned int idx) const ;

	// data members
	union
	{
		struct
		{
			GLfloat xy[2];
		};
		struct
		{
			GLfloat x;
			GLfloat y;
			GLfloat z;
		};
	};
};

