#ifndef MATRIX4X4_H_
#define MATRIX4X4_H_

class Matrix4x4
{
public:

	double m[4][4];

	Matrix4x4();
	Matrix4x4(const Matrix4x4& matrix);
	~Matrix4x4();

	void setRow1(double x, double y, double z, double w);
	void setRow2(double x, double y, double z, double w);
	void setRow3(double x, double y, double z, double w);
	void setRow4(double x, double y, double z, double w);

	Matrix4x4& operator=(const Matrix4x4 &matrix);
	Matrix4x4 operator*(const Matrix4x4 &matrix);//matrix multiplication 


	void translate(double x, double y, double z);
};

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
Matrix4x4 Matrix4x4::operator*(const Matrix4x4 &matrix)
{
	Matrix4x4 result;

	result.m[0][0] = (this->m[0][0] * matrix.m[0][0]) + (this->m[0][1] * matrix.m[1][0]) + (this->m[0][2] * matrix.m[2][0]) + (this->m[0][3] * matrix.m[3][0]);
	result.m[0][1] = (this->m[0][0] * matrix.m[0][1]) + (this->m[0][1] * matrix.m[1][1]) + (this->m[0][2] * matrix.m[2][1]) + (this->m[0][3] * matrix.m[3][1]);
	result.m[0][2] = (this->m[0][0] * matrix.m[0][2]) + (this->m[0][1] * matrix.m[1][2]) + (this->m[0][2] * matrix.m[2][2]) + (this->m[0][3] * matrix.m[3][2]);
	result.m[0][3] = (this->m[0][0] * matrix.m[0][3]) + (this->m[0][1] * matrix.m[1][3]) + (this->m[0][2] * matrix.m[2][3]) + (this->m[0][3] * matrix.m[3][3]);

	result.m[1][0] = (this->m[1][0] * matrix.m[0][0]) + (this->m[1][1] * matrix.m[1][0]) + (this->m[1][2] * matrix.m[2][0]) + (this->m[1][3] * matrix.m[3][0]);
	result.m[1][1] = (this->m[1][0] * matrix.m[0][1]) + (this->m[1][1] * matrix.m[1][1]) + (this->m[1][2] * matrix.m[2][1]) + (this->m[1][3] * matrix.m[3][1]);
	result.m[1][2] = (this->m[1][0] * matrix.m[0][2]) + (this->m[1][1] * matrix.m[1][2]) + (this->m[1][2] * matrix.m[2][2]) + (this->m[1][3] * matrix.m[3][2]);
	result.m[1][3] = (this->m[1][0] * matrix.m[0][3]) + (this->m[1][1] * matrix.m[1][3]) + (this->m[1][2] * matrix.m[2][3]) + (this->m[1][3] * matrix.m[3][3]);

	result.m[2][0] = (this->m[2][0] * matrix.m[0][0]) + (this->m[2][1] * matrix.m[1][0]) + (this->m[2][2] * matrix.m[2][0]) + (this->m[2][3] * matrix.m[3][0]);
	result.m[2][1] = (this->m[2][0] * matrix.m[0][1]) + (this->m[2][1] * matrix.m[1][1]) + (this->m[2][2] * matrix.m[2][1]) + (this->m[2][3] * matrix.m[3][1]);
	result.m[2][2] = (this->m[2][0] * matrix.m[0][2]) + (this->m[2][1] * matrix.m[1][2]) + (this->m[2][2] * matrix.m[2][2]) + (this->m[2][3] * matrix.m[3][2]);
	result.m[2][3] = (this->m[2][0] * matrix.m[0][3]) + (this->m[2][1] * matrix.m[1][3]) + (this->m[2][2] * matrix.m[2][3]) + (this->m[2][3] * matrix.m[3][3]);

	result.m[3][0] = (this->m[3][0] * matrix.m[0][0]) + (this->m[3][1] * matrix.m[1][0]) + (this->m[3][2] * matrix.m[2][0]) + (this->m[3][3] * matrix.m[3][0]);
	result.m[3][1] = (this->m[3][0] * matrix.m[0][1]) + (this->m[3][1] * matrix.m[1][1]) + (this->m[3][2] * matrix.m[2][1]) + (this->m[3][3] * matrix.m[3][1]);
	result.m[3][2] = (this->m[3][0] * matrix.m[0][2]) + (this->m[3][1] * matrix.m[1][2]) + (this->m[3][2] * matrix.m[2][2]) + (this->m[3][3] * matrix.m[3][2]);
	result.m[3][3] = (this->m[3][0] * matrix.m[0][3]) + (this->m[3][1] * matrix.m[1][3]) + (this->m[3][2] * matrix.m[2][3]) + (this->m[3][3] * matrix.m[3][3]);

	return result;
}

inline
void Matrix4x4::translate(double x, double y, double z)
{
	this->m[3][0] = x;
	this->m[3][1] = y;
	this->m[3][2] = z;
}

#endif