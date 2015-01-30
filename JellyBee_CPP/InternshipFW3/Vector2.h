#pragma once
#include "Platforms.h"
//Vector2

class Vector2
{
public:
	//Constructors
	Vector2() : x(0.0f), y(0.0f){}
	Vector2(GLfloat _x, GLfloat _y) : x(_x), y(_y) {}
	Vector2(GLfloat * pArg) : x(pArg[0]), y(pArg[1]) {}
	Vector2( const Vector2 & vector) : x(vector.x), y(vector.y) {}

	//Vector's operations
	GLfloat Length() const;
	Vector2 & Normalize();
	Vector2 operator + ( const Vector2 & vector) const;
	Vector2 & operator += ( const Vector2 & vector);
	Vector2 operator - () const;
	Vector2 operator - ( const Vector2 & vector) const;
	Vector2 & operator -= ( const Vector2 & vector);
	Vector2 operator * (GLfloat k) const;
	Vector2 & operator *= (GLfloat k);
	Vector2 operator / (GLfloat k) const;
	Vector2 & operator /= (GLfloat k);
	Vector2 & operator = ( const Vector2 & vector);
	Vector2 & operator = (GLfloat* pArg);
	Vector2 Modulate( const Vector2 & vector) const;
	GLfloat Dot( const Vector2 & vector) const;

	//access to elements
	GLfloat operator [] (unsigned int idx) const;

	//data members
	GLfloat x;
	GLfloat y;
};
