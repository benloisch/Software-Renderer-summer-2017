#ifndef PERSPECTIVEPROJECTION_H
#define PERSPECTIVEPROJECTION_H

#include "Camera.h"
#include <iostream>
#include <math.h>
using namespace std;

class PerspectiveProj
{
private:
	double nearPlane;
	double farPlane;
	double aspectRatio;
	double verticalFOV;
	double perspectiveProjM[4][4];

public:

	PerspectiveProj(double nearP = 0, double farP = 0, double width = 0, double height = 0, double vFOV = 0){
		nearPlane = nearP;
		farPlane = farP;
		aspectRatio = width / height;
		verticalFOV = vFOV;
		for (int x = 0; x < 4; x++){
			for (int y = 0; y < 4; y++){
				perspectiveProjM[x][y] = 0;
			}
		}
	}

	void createPerspectiveProjMatrix(){
		perspectiveProjM[0][0] = 1 / (aspectRatio * tan((((verticalFOV / 2) * 3.14159265) / 180)));
		perspectiveProjM[1][1] = 1 / tan((((verticalFOV / 2) * 3.14159265) / 180));
		perspectiveProjM[2][2] = farPlane / (farPlane - nearPlane);
		perspectiveProjM[3][2] = ((-1) * nearPlane * farPlane) / (farPlane - nearPlane);
		perspectiveProjM[2][3] = 1;
	}

	void displayPerspectiveProjToConsole()
	{
		cout << "PERSPECTIVE PROJECTION MATRIX" << endl;
		for (int i = 0; i < 4; i++)
		{
			cout << "[";
			for (int j = 0; j < 4; j++)
			{
				cout << perspectiveProjM[i][j] << ", ";
			}
			cout << "]" << endl;
		}
	}

	double getPerspectiveProjMatrixElement(int element){
		if (element == 11)
			return perspectiveProjM[0][0];
		else if (element == 12)
			return perspectiveProjM[0][1];
		else if (element == 13)
			return perspectiveProjM[0][2];
		else if (element == 14)
			return perspectiveProjM[0][3];
		else if (element == 21)
			return perspectiveProjM[1][0];
		else if (element == 22)
			return perspectiveProjM[1][1];
		else if (element == 23)
			return perspectiveProjM[1][2];
		else if (element == 24)
			return perspectiveProjM[1][3];
		else if (element == 31)
			return perspectiveProjM[2][0];
		else if (element == 32)
			return perspectiveProjM[2][1];
		else if (element == 33)
			return perspectiveProjM[2][2];
		else if (element == 34)
			return perspectiveProjM[2][3];
		else if (element == 41)
			return perspectiveProjM[3][0];
		else if (element == 42)
			return perspectiveProjM[3][1];
		else if (element == 43)
			return perspectiveProjM[3][2];
		else if (element == 44)
			return perspectiveProjM[3][3];
		else
			return 0;
	}
};

#endif