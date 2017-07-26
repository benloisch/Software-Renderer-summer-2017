#ifndef VECTOR4D_H_
#define VECTOR4D_H_

#include "Matrix4x4.h"
#include <math.h>

class Vector4D
{
public:
	
	float x;
	float y;
	float z;
	float w;

	Vector4D();
	Vector4D(float, float, float, float);
	Vector4D(int, int, int, int);
	Vector4D(const Vector4D &vector);
	~Vector4D();

	//all operation take place on the 3D components (x, y, z) 
	//only the matrix operation takes place on 4D (x, y, z, w)
	Vector4D operator+(const Vector4D &vector);//add two vectors componentwise 
	Vector4D operator-(const Vector4D &vector);//subtract two vectors componentwise
	Vector4D& operator=(const Vector4D &vector);
	Vector4D& operator+=(const Vector4D &vector);
	Vector4D& operator-=(const Vector4D &vector);
	Vector4D& operator*=(const int integer);//componentwise integer multiplication
	Vector4D& operator*=(const float floating);//componentwise floating point multiplication
	Vector4D& operator/=(const int integer);//componentwise integer division
	Vector4D& operator/=(const float floating);//componentwise floating point division
	Vector4D operator*(const int integer);//componentwise integer multiplication
	Vector4D operator*(const float floating);//componentwise floating point multiplication
	Vector4D operator/(const int integer);//componentwise integer division
	Vector4D operator/(const float floating);//componentwise floating point division
	float operator*(const Vector4D &vector);//3D dot product (x*x + y*y + z*z) no w included
	Vector4D operator^(const Vector4D &vector);//3D cross product

	Vector4D operator*(const Matrix4x4 &matrix);//vector * matrix
	Vector4D& operator*=(const Matrix4x4 &matrix);//vector = vector * matrix

	float inline magnitude();//3D magnitude sqrt(x*x + y*y + z*z)
	void inline normalize();//normalize 3D portion (x, y, z)
	Vector4D inline lerp(Vector4D &toVector, float t);

};

//inline functions must be defined inside header

inline
Vector4D Vector4D::operator+(const Vector4D &vector)
{
	return Vector4D(this->x + vector.x, this->y + vector.y, this->z + vector.z, this->w + vector.w);
}

inline
Vector4D Vector4D::operator-(const Vector4D &vector)
{
	return Vector4D(this->x - vector.x, this->y - vector.y, this->z - vector.z, this->w - vector.w);
}

inline
Vector4D& Vector4D::operator=(const Vector4D &vector)
{
	this->x = vector.x;
	this->y = vector.y;
	this->z = vector.z;
	this->w = vector.w;

	return *this;
}

inline
Vector4D& Vector4D::operator+=(const Vector4D &vector)
{
	this->x += vector.x;
	this->y += vector.y;
	this->z += vector.z;

	return *this;
}

inline
Vector4D& Vector4D::operator-=(const Vector4D &vector)
{
	this->x -= vector.x;
	this->y -= vector.y;
	this->z -= vector.z;

	return *this;
}

inline
Vector4D& Vector4D::operator*=(const int integer)
{
	this->x *= integer;
	this->y *= integer;
	this->z *= integer;

	return *this;
}

inline
Vector4D& Vector4D::operator*=(const float floating)
{
	this->x *= floating;
	this->y *= floating;
	this->z *= floating;

	return *this;
}

inline
Vector4D& Vector4D::operator/=(const int integer)
{
	this->x /= integer;
	this->y /= integer;
	this->z /= integer;

	return *this;
}

inline
Vector4D& Vector4D::operator/=(const float floating)
{
	this->x /= floating;
	this->y /= floating;
	this->z /= floating;

	return *this;
}

inline
Vector4D Vector4D::operator*(const float floating)
{
	return Vector4D(this->x * floating, this->y * floating, this->z * floating, this->w * floating);
}

inline
Vector4D Vector4D::operator*(const int integer)
{
	return Vector4D(this->x * integer, this->y * integer, this->z * integer, this->w * integer);
}

inline
Vector4D Vector4D::operator/(const float floating)
{
	this->x /= floating;
	this->y /= floating;
	this->z /= floating;
}

inline
Vector4D Vector4D::operator/(const int integer)
{
	this->x /= integer;
	this->y /= integer;
	this->z /= integer;
}

inline
float Vector4D::operator*(const Vector4D &vector)
{
	return (this->x * vector.x) + (this->y * vector.y) + (this->z * vector.z);
}

inline
Vector4D Vector4D::operator^(const Vector4D &vector)
{
	//x = (v1y*v2z - v1z*v2y)
	//y = (v1z*v2x - v1x*v2z)
	//z = (v1x*v2y - v1y*v1x);

	Vector4D result;
	result.x = (this->y * vector.z) - (this->z * vector.y);
	result.y = (this->z * vector.x) - (this->x * vector.z);
	result.z = (this->x * vector.y) - (this->y * vector.x);

	return result;
}

inline
float Vector4D::magnitude()
{
	return sqrt((x * x) + (y * y) + (z * z));
}

inline
void Vector4D::normalize()
{
	float mag = this->magnitude();

	if (mag < 0.0001)
		return;

	this->x /= mag;
	this->y /= mag;
	this->z /= mag;
}

inline 
Vector4D Vector4D::operator*(const Matrix4x4 &matrix)
{
	Vector4D result;

	result.x = (this->x * matrix.m[0][0]) + (this->y * matrix.m[1][0]) + (this->z * matrix.m[2][0]) + (this->w * matrix.m[3][0]);
	result.y = (this->x * matrix.m[0][1]) + (this->y * matrix.m[1][1]) + (this->z * matrix.m[2][1]) + (this->w * matrix.m[3][1]);
	result.z = (this->x * matrix.m[0][2]) + (this->y * matrix.m[1][2]) + (this->z * matrix.m[2][2]) + (this->w * matrix.m[3][2]);
	result.w = (this->x * matrix.m[0][3]) + (this->y * matrix.m[1][3]) + (this->z * matrix.m[2][3]) + (this->w * matrix.m[3][3]);

	return result;
}

inline
Vector4D& Vector4D::operator*=(const Matrix4x4 &matrix)
{
	Vector4D result;

	result.x = (this->x * matrix.m[0][0]) + (this->y * matrix.m[1][0]) + (this->z * matrix.m[2][0]) + this->w * matrix.m[3][0];
	result.y = (this->x * matrix.m[0][1]) + (this->y * matrix.m[1][1]) + (this->z * matrix.m[2][1]) + this->w * matrix.m[3][1];
	result.z = (this->x * matrix.m[0][2]) + (this->y * matrix.m[1][2]) + (this->z * matrix.m[2][2]) + this->w * matrix.m[3][2];
	result.w = (this->x * matrix.m[0][3]) + (this->y * matrix.m[1][3]) + (this->z * matrix.m[2][3]) + this->w * matrix.m[3][3];

	this->x = result.x;
	this->y = result.y;
	this->z = result.z;
	this->w = result.w;

	return *this;
}

Vector4D inline Vector4D::lerp(Vector4D &toVector, float t) {
	return *this + ((toVector - *this) * t);
}

#endif