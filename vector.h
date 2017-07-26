#ifndef VECTOR_H_
#define VECTOR_H_

#include "matrix.h"

class Vector
{
public:
	double x;
	double y;
	double z;
	double w;

	Vector();
	Vector(double, double, double, double = 0);
	Vector(const Vector &inVec);

	Vector operator*(const Vector inVec);//cross product
	Vector operator*(double inScalar);//scalar * vector = vector
	double operator^(const Vector inVec);//dot product
	Vector operator/(double inScalar);
	Vector operator+(const Vector inVec);
	Vector operator-(const Vector inVec);
	Vector &operator=(const Vector inVec);
	Vector operator*(const Matrix inMat);

	void normalize();
	double getMagnitude();

};

#endif