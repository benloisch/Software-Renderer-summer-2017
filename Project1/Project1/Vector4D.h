#ifndef VECTOR4D_H_
#define VECTOR4D_H_

#include "Matrix4x4.h"
#include <math.h>

class Vector4D
{
public:
	
	double x;
	double y;
	double z;
	double w;

	Vector4D();
	Vector4D(double, double, double, double);
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
	Vector4D& operator*=(const double doubleing);//componentwise floating point multiplication
	Vector4D& operator/=(const int integer);//componentwise integer division
	Vector4D& operator/=(const double floating);//componentwise floating point division
	Vector4D operator*(const int integer);//componentwise integer multiplication
	Vector4D operator*(const double floating);//componentwise floating point multiplication
	Vector4D operator/(const int integer);//componentwise integer division
	Vector4D operator/(const double floating);//componentwise floating point division
	double operator*(const Vector4D &vector);//3D dot product (x*x + y*y + z*z) no w included
	Vector4D operator^(const Vector4D &vector);//3D cross product

	Vector4D operator*(const Matrix4x4 &matrix);//vector * matrix
	Vector4D& operator*=(const Matrix4x4 &matrix);//vector = vector * matrix

	double magnitude();//3D magnitude sqrt(x*x + y*y + z*z)
	void normalize();//normalize 3D portion (x, y, z)

};

//inline functions must be defined inside header

inline
Vector4D Vector4D::operator+(const Vector4D &vector)
{
	return Vector4D(this->x + vector.x, this->y + vector.y, this->z + vector.z, 0.0);
}

inline
Vector4D Vector4D::operator-(const Vector4D &vector)
{
	return Vector4D(this->x - vector.x, this->y - vector.y, this->z - vector.z, 0.0);
}

inline
Vector4D& Vector4D::operator=(const Vector4D &vector)
{
	this->x = vector.x;
	this->y = vector.y;
	this->z = vector.z;

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
Vector4D& Vector4D::operator*=(const double floating)
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
Vector4D& Vector4D::operator/=(const double floating)
{
	this->x /= floating;
	this->y /= floating;
	this->z /= floating;

	return *this;
}

inline
Vector4D Vector4D::operator*(const double floating)
{
	return Vector4D(this->x * floating, this->y * floating, this->z * floating, 0.0);
}

inline
Vector4D Vector4D::operator*(const int integer)
{
	return Vector4D(this->x * integer, this->y * integer, this->z * integer, 0.0);
}

inline
Vector4D Vector4D::operator/(const double floating)
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
double Vector4D::operator*(const Vector4D &vector)
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
	result.z = (this->x * vector.y) - (this->y * vector.z);

	return result;
}

inline
double Vector4D::magnitude()
{
	return sqrt((x * x) + (y * y) + (z * z));
}

inline
void Vector4D::normalize()
{
	double mag = this->magnitude();

	this->x /= mag;
	this->y /= mag;
	this->z /= mag;
}

inline 
Vector4D Vector4D::operator*(const Matrix4x4 &matrix)
{
	Vector4D result;

	result.x = (this->x * matrix.m[0][0]) + (this->y * matrix.m[1][0]) + (this->z * matrix.m[2][0]) + this->w * matrix.m[3][0];
	result.y = (this->x * matrix.m[0][1]) + (this->y * matrix.m[1][1]) + (this->z * matrix.m[2][1]) + this->w * matrix.m[3][1];
	result.z = (this->x * matrix.m[0][2]) + (this->y * matrix.m[1][2]) + (this->z * matrix.m[2][2]) + this->w * matrix.m[3][2];
	result.w = (this->x * matrix.m[0][3]) + (this->y * matrix.m[1][3]) + (this->z * matrix.m[2][3]) + this->w * matrix.m[3][3];

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

#endif