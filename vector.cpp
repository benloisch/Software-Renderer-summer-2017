#include "vector.h"
#include <math.h>

Vector::Vector()
{
	x = 0;
	y = 0;
	z = 0;
	w = 0;
}

Vector::Vector(double inx, double iny, double inz, double inw)
{
	x = inx;
	y = iny;
	z = inz;
	w = inw;
}

Vector::Vector(const Vector &inVec)
{
	this->x = inVec.x;
	this->y = inVec.y;
	this->z = inVec.z;
	this->w = inVec.w;
}

Vector Vector::operator*(const Vector inVec)
{
	Vector result((this->y * inVec.z) - (this->z * inVec.y), (this->z * inVec.x) - (this->x * inVec.z), (this->x * inVec.y) - (this->y * inVec.x));
	return result;
}

Vector Vector::operator*(double inScalar)
{
	Vector result(this->x * inScalar, this->y * inScalar, this->z * inScalar, this->w * inScalar);
	return result;
}

double Vector::operator^(const Vector inVec)
{
	double result = (this->x * inVec.x) + (this->y * inVec.y) + (this->z * inVec.z) + (this->w * inVec.w);
	return result;
}

Vector Vector::operator/(double inScalar)
{
	Vector result(this->x / inScalar, this->y / inScalar, this->z / inScalar, this->w / inScalar);
	return result;
}

Vector Vector::operator+(const Vector inVec)
{
	Vector result(this->x + inVec.x, this->y + inVec.y, this->z + inVec.z, this->w + inVec.w);
	return result;
}

Vector Vector::operator-(const Vector inVec)
{
	Vector result(this->x - inVec.x, this->y - inVec.y, this->z - inVec.z, this->w - inVec.w);
	return result;
}

Vector &Vector::operator=(const Vector inVec)
{
	this->x = inVec.x;
	this->y = inVec.y;
	this->z = inVec.z;
	this->w = inVec.w;

	return *this;
}

Vector Vector::operator*(const Matrix inMat)
{
	Vector result;
	//[x, y, z, w]      [00, 01, 02, 03]
	//                  [10, 11, 12, 13]
	//                  [20, 21, 22, 23]
	//                  [30, 31, 32, 33]

	result.x = (this->x * inMat.m[0][0]) + (this->y * inMat.m[1][0]) + (this->z * inMat.m[2][0]) + (this->w * inMat.m[3][0]);
	result.y = (this->x * inMat.m[0][1]) + (this->y * inMat.m[1][1]) + (this->z * inMat.m[2][1]) + (this->w * inMat.m[3][1]);
	result.z = (this->x * inMat.m[0][2]) + (this->y * inMat.m[1][2]) + (this->z * inMat.m[2][2]) + (this->w * inMat.m[3][2]);
	result.w = (this->x * inMat.m[0][3]) + (this->y * inMat.m[1][3]) + (this->z * inMat.m[2][3]) + (this->w * inMat.m[3][3]);

	return result;
}

void Vector::normalize()
{
	double tempx = x / sqrt((x * x) + (y * y) + (z * z) + (w * w));
	double tempy = y / sqrt((x * x) + (y * y) + (z * z) + (w * w));
	double tempz = z / sqrt((x * x) + (y * y) + (z * z) + (w * w));
	double tempw = w / sqrt((x * x) + (y * y) + (z * z) + (w * w));

	x = tempx;
	y = tempy;
	z = tempz;
	w = tempw;
}

double Vector::getMagnitude()
{
	return sqrt((x * x) + (y * y) + (z * z) + (w * w));
}