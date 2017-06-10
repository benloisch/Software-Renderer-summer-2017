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