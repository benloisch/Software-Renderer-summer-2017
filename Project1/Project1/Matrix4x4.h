#ifndef MATRIX4X4_H_
#define MATRIX4X4_H_

#include <math.h>

class Matrix4x4
{
public:

	double m[4][4];

	Matrix4x4();
	Matrix4x4(const Matrix4x4& matrix);
	~Matrix4x4();

	Matrix4x4& operator=(const Matrix4x4 &matrix);
	Matrix4x4 operator*(const Matrix4x4 &inMat);
	Matrix4x4 operator*(const double inScalar);
	Matrix4x4 operator+(const Matrix4x4 &inMat);
	Matrix4x4 operator-(const Matrix4x4 &inMat);

	void setRow1(double x, double y, double z, double w);
	void setRow2(double x, double y, double z, double w);
	void setRow3(double x, double y, double z, double w);
	void setRow4(double x, double y, double z, double w);

	double det3x3(double, double, double, double, double, double, double, double, double);
	void setXrot(double);//in degrees
	void setYrot(double);
	void setZrot(double);

	void setScale(double, double, double);
	void setTranslate(double, double, double);
	void setRotArb(double, double, double, double);

	void setTranspose();
	Matrix4x4 tranpose();
	void setInverse();
	Matrix4x4 inverse();
	void setIdentity();
	void zeroOutElements();
};

inline 
Matrix4x4& Matrix4x4::operator=(const Matrix4x4 &matrix)
{
	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			this->m[r][c] = matrix.m[r][c];
		}
	}

	return *this;
}

inline
Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &inMat)
{
	Matrix4x4 result;

	//[00, 01, 02, 03]  [00, 01, 02, 03]
	//[10, 11, 12, 13]  [10, 11, 12, 13]
	//[20, 21, 22, 23]  [20, 21, 22, 23]
	//[30, 31, 32, 33]  [30, 31, 32, 33]

	result.m[0][0] = (this->m[0][0] * inMat.m[0][0]) + (this->m[0][1] * inMat.m[1][0]) + (this->m[0][2] * inMat.m[2][0]) + (this->m[0][3] * inMat.m[3][0]);
	result.m[0][1] = (this->m[0][0] * inMat.m[0][1]) + (this->m[0][1] * inMat.m[1][1]) + (this->m[0][2] * inMat.m[2][1]) + (this->m[0][3] * inMat.m[3][1]);
	result.m[0][2] = (this->m[0][0] * inMat.m[0][2]) + (this->m[0][1] * inMat.m[1][2]) + (this->m[0][2] * inMat.m[2][2]) + (this->m[0][3] * inMat.m[3][2]);
	result.m[0][3] = (this->m[0][0] * inMat.m[0][3]) + (this->m[0][1] * inMat.m[1][3]) + (this->m[0][2] * inMat.m[2][3]) + (this->m[0][3] * inMat.m[3][3]);

	result.m[1][0] = (this->m[1][0] * inMat.m[0][0]) + (this->m[1][1] * inMat.m[1][0]) + (this->m[1][2] * inMat.m[2][0]) + (this->m[1][3] * inMat.m[3][0]);
	result.m[1][1] = (this->m[1][0] * inMat.m[0][1]) + (this->m[1][1] * inMat.m[1][1]) + (this->m[1][2] * inMat.m[2][1]) + (this->m[1][3] * inMat.m[3][1]);
	result.m[1][2] = (this->m[1][0] * inMat.m[0][2]) + (this->m[1][1] * inMat.m[1][2]) + (this->m[1][2] * inMat.m[2][2]) + (this->m[1][3] * inMat.m[3][2]);
	result.m[1][3] = (this->m[1][0] * inMat.m[0][3]) + (this->m[1][1] * inMat.m[1][3]) + (this->m[1][2] * inMat.m[2][3]) + (this->m[1][3] * inMat.m[3][3]);

	result.m[2][0] = (this->m[2][0] * inMat.m[0][0]) + (this->m[2][1] * inMat.m[1][0]) + (this->m[2][2] * inMat.m[2][0]) + (this->m[2][3] * inMat.m[3][0]);
	result.m[2][1] = (this->m[2][0] * inMat.m[0][1]) + (this->m[2][1] * inMat.m[1][1]) + (this->m[2][2] * inMat.m[2][1]) + (this->m[2][3] * inMat.m[3][1]);
	result.m[2][2] = (this->m[2][0] * inMat.m[0][2]) + (this->m[2][1] * inMat.m[1][2]) + (this->m[2][2] * inMat.m[2][2]) + (this->m[2][3] * inMat.m[3][2]);
	result.m[2][3] = (this->m[2][0] * inMat.m[0][3]) + (this->m[2][1] * inMat.m[1][3]) + (this->m[2][2] * inMat.m[2][3]) + (this->m[2][3] * inMat.m[3][3]);

	result.m[3][0] = (this->m[3][0] * inMat.m[0][0]) + (this->m[3][1] * inMat.m[1][0]) + (this->m[3][2] * inMat.m[2][0]) + (this->m[3][3] * inMat.m[3][0]);
	result.m[3][1] = (this->m[3][0] * inMat.m[0][1]) + (this->m[3][1] * inMat.m[1][1]) + (this->m[3][2] * inMat.m[2][1]) + (this->m[3][3] * inMat.m[3][1]);
	result.m[3][2] = (this->m[3][0] * inMat.m[0][2]) + (this->m[3][1] * inMat.m[1][2]) + (this->m[3][2] * inMat.m[2][2]) + (this->m[3][3] * inMat.m[3][2]);
	result.m[3][3] = (this->m[3][0] * inMat.m[0][3]) + (this->m[3][1] * inMat.m[1][3]) + (this->m[3][2] * inMat.m[2][3]) + (this->m[3][3] * inMat.m[3][3]);

	return result;
}

inline
Matrix4x4 Matrix4x4::operator*(const double inScalar)
{
	Matrix4x4 result;

	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			result.m[r][c] = this->m[r][c] * inScalar;
		}
	}

	return result;
}

inline
Matrix4x4 Matrix4x4::operator+(const Matrix4x4 &inMat)
{
	Matrix4x4 result;

	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			result.m[r][c] = this->m[r][c] + inMat.m[r][c];
		}
	}

	return result;
}

inline
Matrix4x4 Matrix4x4::operator-(const Matrix4x4 &inMat)
{
	Matrix4x4 result;

	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			result.m[r][c] = this->m[r][c] - inMat.m[r][c];
		}
	}

	return result;
}

inline
void Matrix4x4::setRow1(double x, double y, double z, double w)
{
	this->m[0][0] = x;
	this->m[0][1] = y;
	this->m[0][2] = z;
	this->m[0][3] = w;
}

inline
void Matrix4x4::setRow2(double x, double y, double z, double w)
{
	this->m[1][0] = x;
	this->m[1][1] = y;
	this->m[1][2] = z;
	this->m[1][3] = w;
}

inline
void Matrix4x4::setRow3(double x, double y, double z, double w)
{
	this->m[2][0] = x;
	this->m[2][1] = y;
	this->m[2][2] = z;
	this->m[2][3] = w;
}

inline
void Matrix4x4::setRow4(double x, double y, double z, double w)
{
	this->m[3][0] = x;
	this->m[3][1] = y;
	this->m[3][2] = z;
	this->m[3][3] = w;
}

inline
void Matrix4x4::setTranspose()
{
	Matrix4x4 copy = *this;

	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			m[r][c] = copy.m[c][r];
		}
	}
}

inline
Matrix4x4 Matrix4x4::tranpose()
{
	Matrix4x4 result;

	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			result.m[r][c] = m[c][r];
		}
	}

	return result;
}

inline
double Matrix4x4::det3x3(double m11, double m12, double m13, double m21, double m22, double m23, double m31, double m32, double m33)
{
	return (m11 * m22 * m33) + (m12 * m23 * m31) + (m13 * m21 * m32) - (m13 * m22 * m31) - (m12 * m21 * m33) - (m11 * m23 * m32);
}

inline
void Matrix4x4::setInverse()
{
	double det4x4 = (m[0][0] * det3x3(m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]))
		- (m[0][1] * det3x3(m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]))
		+ (m[0][2] * det3x3(m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]))
		- (m[0][3] * det3x3(m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]));

	double tm11 = det3x3(m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]);
	double tm12 = -det3x3(m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]);
	double tm13 = det3x3(m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]);
	double tm14 = -det3x3(m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]);

	double tm21 = -det3x3(m[0][1], m[0][2], m[0][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]);
	double tm22 = det3x3(m[0][0], m[0][2], m[0][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]);
	double tm23 = -det3x3(m[0][0], m[0][1], m[0][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]);
	double tm24 = det3x3(m[0][0], m[0][1], m[0][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]);

	double tm31 = det3x3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[3][1], m[3][2], m[3][3]);
	double tm32 = -det3x3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[3][0], m[3][2], m[3][3]);
	double tm33 = det3x3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[3][0], m[3][1], m[3][3]);
	double tm34 = -det3x3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[3][0], m[3][1], m[3][2]);

	double tm41 = -det3x3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3]);
	double tm42 = det3x3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3]);
	double tm43 = -det3x3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3]);
	double tm44 = det3x3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]);

	m[0][0] = tm11 / det4x4;
	m[0][1] = tm21 / det4x4;
	m[0][2] = tm31 / det4x4;
	m[0][3] = tm41 / det4x4;

	m[1][0] = tm12 / det4x4;
	m[1][1] = tm22 / det4x4;
	m[1][2] = tm32 / det4x4;
	m[1][3] = tm42 / det4x4;

	m[2][0] = tm13 / det4x4;
	m[2][1] = tm23 / det4x4;
	m[2][2] = tm33 / det4x4;
	m[2][3] = tm43 / det4x4;

	m[3][0] = tm14 / det4x4;
	m[3][1] = tm24 / det4x4;
	m[3][2] = tm34 / det4x4;
	m[3][3] = tm44 / det4x4;
}

inline
Matrix4x4 Matrix4x4::inverse()
{
	Matrix4x4 result;

	double det4x4 = (m[0][0] * det3x3(m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]))
		- (m[0][1] * det3x3(m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]))
		+ (m[0][2] * det3x3(m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]))
		- (m[0][3] * det3x3(m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]));

	double tm11 = det3x3(m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]);
	double tm12 = -det3x3(m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]);
	double tm13 = det3x3(m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]);
	double tm14 = -det3x3(m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]);

	double tm21 = -det3x3(m[0][1], m[0][2], m[0][3], m[2][1], m[2][2], m[2][3], m[3][1], m[3][2], m[3][3]);
	double tm22 = det3x3(m[0][0], m[0][2], m[0][3], m[2][0], m[2][2], m[2][3], m[3][0], m[3][2], m[3][3]);
	double tm23 = -det3x3(m[0][0], m[0][1], m[0][3], m[2][0], m[2][1], m[2][3], m[3][0], m[3][1], m[3][3]);
	double tm24 = det3x3(m[0][0], m[0][1], m[0][2], m[2][0], m[2][1], m[2][2], m[3][0], m[3][1], m[3][2]);

	double tm31 = det3x3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[3][1], m[3][2], m[3][3]);
	double tm32 = -det3x3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[3][0], m[3][2], m[3][3]);
	double tm33 = det3x3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[3][0], m[3][1], m[3][3]);
	double tm34 = -det3x3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[3][0], m[3][1], m[3][2]);

	double tm41 = -det3x3(m[0][1], m[0][2], m[0][3], m[1][1], m[1][2], m[1][3], m[2][1], m[2][2], m[2][3]);
	double tm42 = det3x3(m[0][0], m[0][2], m[0][3], m[1][0], m[1][2], m[1][3], m[2][0], m[2][2], m[2][3]);
	double tm43 = -det3x3(m[0][0], m[0][1], m[0][3], m[1][0], m[1][1], m[1][3], m[2][0], m[2][1], m[2][3]);
	double tm44 = det3x3(m[0][0], m[0][1], m[0][2], m[1][0], m[1][1], m[1][2], m[2][0], m[2][1], m[2][2]);

	result.m[0][0] = tm11 / det4x4;
	result.m[0][1] = tm21 / det4x4;
	result.m[0][2] = tm31 / det4x4;
	result.m[0][3] = tm41 / det4x4;

	result.m[1][0] = tm12 / det4x4;
	result.m[1][1] = tm22 / det4x4;
	result.m[1][2] = tm32 / det4x4;
	result.m[1][3] = tm42 / det4x4;

	result.m[2][0] = tm13 / det4x4;
	result.m[2][1] = tm23 / det4x4;
	result.m[2][2] = tm33 / det4x4;
	result.m[2][3] = tm43 / det4x4;

	result.m[3][0] = tm14 / det4x4;
	result.m[3][1] = tm24 / det4x4;
	result.m[3][2] = tm34 / det4x4;
	result.m[3][3] = tm44 / det4x4;

	return result;
}

inline
void Matrix4x4::setXrot(double inRot)
{
	inRot = (inRot * 3.1415926535) / 180.0;

	m[1][1] = cos(inRot);
	m[1][2] = sin(inRot);
	m[2][1] = -sin(inRot);
	m[2][2] = cos(inRot);
}

inline
void Matrix4x4::setYrot(double inRot)
{
	inRot = (inRot * 3.1415926535) / 180.0;

	m[0][0] = cos(inRot);
	m[2][0] = sin(inRot);
	m[0][2] = -sin(inRot);
	m[2][2] = cos(inRot);
}

inline
void Matrix4x4::setZrot(double inRot)
{
	inRot = (inRot * 3.1415926535) / 180.0;

	m[0][0] = cos(inRot);
	m[1][0] = -sin(inRot);
	m[0][1] = sin(inRot);
	m[1][1] = cos(inRot);
}

inline
void Matrix4x4::setScale(double inX, double inY, double inZ)
{
	m[0][0] = inX;
	m[1][1] = inY;
	m[2][2] = inZ;
}

inline
void Matrix4x4::setTranslate(double inX, double inY, double inZ)
{
	m[3][0] = inX;
	m[3][1] = inY;
	m[3][2] = inZ;
}

inline
void Matrix4x4::setIdentity()
{
	for (int r = 0; r < 4; r++)
	for (int c = 0; c < 4; c++)
	{
		if (r == c)
			m[r][c] = 1;
		else
			m[r][c] = 0;
	}
}

inline
void Matrix4x4::setRotArb(double nx, double ny, double nz, double inRot)
{
	inRot = (inRot * 3.1415926535) / 180.0;

	m[0][0] = nx * nx * (1.0 - cos(inRot)) + cos(inRot);
	m[0][1] = nx * ny * (1.0 - cos(inRot)) + (nz * sin(inRot));
	m[0][2] = nx * nz * (1.0 - cos(inRot)) - (ny * sin(inRot));
	m[1][0] = nx * ny * (1.0 - cos(inRot)) - (nz * sin(inRot));
	m[1][1] = ny * ny * (1.0 - cos(inRot)) + cos(inRot);
	m[1][2] = ny * nz * (1.0 - cos(inRot)) + (nx * sin(inRot));
	m[2][0] = nx * nz * (1.0 - cos(inRot)) + (ny * sin(inRot));
	m[2][1] = ny * nz * (1.0 - cos(inRot)) - (nx * sin(inRot));
	m[2][2] = nz * nz * (1.0 - cos(inRot)) + cos(inRot);
}

inline
void Matrix4x4::zeroOutElements()
{
	for (int r = 0; r < 4; r++)
	for (int c = 0; c < 4; c++)
	{
		if (m[r][c] < 0.00000001 || m[r][c] < -0.00000001)
			m[r][c] = 0.0;
	}
}

#endif