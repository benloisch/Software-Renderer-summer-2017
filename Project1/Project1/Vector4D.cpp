#include "Vector4D.h"

Vector4D::Vector4D()
{
	this->x = this->y = this->z = 0.0;
}

Vector4D::Vector4D(float x, float y, float z, float w)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector4D::Vector4D(int x, int y, int z, int w)
{
	this->x = float(x);
	this->y = float(y);
	this->z = float(z);
	this->w = float(w);
}

Vector4D::Vector4D(const Vector4D &vector)
{
	this->x = vector.x;
	this->y = vector.y;
	this->z = vector.z;
	this->w = vector.w;
}

Vector4D::~Vector4D()
{

}