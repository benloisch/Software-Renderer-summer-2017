#include <iostream>
#include "vector.h"
#include "matrix.h"

using namespace std;

int main()
{
	Matrix mat1;
	mat1.setRow1(-0.6, 0.0, -0.7, 0.0);
	mat1.setRow2(-0.1, 0.9, 0.0, 0.0);
	mat1.setRow3(0.7, 0.0, -0.6, 0.0);
	mat1.setRow4(0, 0, 3, 1);

	Matrix mat2 = mat1;
	mat1.setInverse();

	Matrix identity = mat2 * mat1;

	system("PAUSE");
	return 0;
}