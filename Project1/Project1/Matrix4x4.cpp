#include "Matrix4x4.h"

Matrix4x4::Matrix4x4()
{
	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			if (r == c)
			{
				this->m[r][c] = 1.0;
			}
			else
			{
				this->m[r][c] = 0.0;
			}
		}
	}
}

Matrix4x4::Matrix4x4(const Matrix4x4 &matrix)
{
	for (int r = 0; r < 4; r++)
	{
		for (int c = 0; c < 4; c++)
		{
			this->m[r][c] = matrix.m[r][c];
		}
	}
}

Matrix4x4::~Matrix4x4()
{

}

double Matrix4x4::det3x3(double m11, double m12, double m13, double m21, double m22, double m23, double m31, double m32, double m33)
{
	return (m11 * m22 * m33) + (m12 * m23 * m31) + (m13 * m21 * m32) - (m13 * m22 * m31) - (m12 * m21 * m33) - (m11 * m23 * m32);
}