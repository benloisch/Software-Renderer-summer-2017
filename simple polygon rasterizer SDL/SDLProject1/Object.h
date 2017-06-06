#ifndef OBJECT_H
#define OBJECT_H

#include "PerspectiveProjection.h"
#include "Vertex.h"
#include "WorldViewProjection.h"
#include <iostream>
#include <vector>

using namespace std;

class Object
{
private:
	double objXCoord;
	double objYCoord;
	double objZCoord;
	double translateMatrix[4][4];
	double rotateXAngle[4][4];
	double rotateYAngle[4][4];
	double rotationMatrix[4][4];
	double finalMatrix[4][4];
	int colorR;
	int colorG;
	int colorB;

public:
	//Public vector to let worldviewprojection.h get access too
	vector <Vertex> objVerticies;

	Object(){
		colorR = 255;
		colorG = 255;
		colorB = 255;
		for (int x = 0; x < 4; x++){
			for (int y = 0; y < 4; y++){
				translateMatrix[x][y] = 0;
				rotationMatrix[x][y] = 0;
				rotateXAngle[x][y] = 0;
				rotateYAngle[x][y] = 0;
				finalMatrix[x][y] = 0;
				if (x == y){
					translateMatrix[x][y] = 1;
					rotationMatrix[x][y] = 1;
					rotateXAngle[x][y] = 1;
					rotateYAngle[x][y] = 1;
					finalMatrix[x][y] = 1;
				}
			}
		}
	}

	void addVertex(double x, double y, double z, double w){
		objVerticies.push_back(Vertex(x, y, z, w));
	}

	void translate(double x, double y, double z){
		translateMatrix[3][0] = x;
		translateMatrix[3][1] = y;
		translateMatrix[3][2] = z;
	}

	void rotateXAxis(double angle, Vertex right){
		double radianAngle = (angle * 3.14159265) / 180;
		rotateXAngle[0][0] = ((right.getX() * right.getX()) * (1 - cos(radianAngle))) + cos(radianAngle);
		rotateXAngle[0][1] = ((right.getX() * right.getY()) * (1 - cos(radianAngle))) + (right.getZ() * sin(radianAngle));
		rotateXAngle[0][2] = ((right.getX() * right.getZ()) * (1 - cos(radianAngle))) - (right.getY() * sin(radianAngle));
		rotateXAngle[1][0] = ((right.getX() * right.getY()) * (1 - cos(radianAngle))) - (right.getZ() * sin(radianAngle));
		rotateXAngle[1][1] = ((right.getY() * right.getY()) * (1 - cos(radianAngle))) + cos(radianAngle);
		rotateXAngle[1][2] = ((right.getY() * right.getZ()) * (1 - cos(radianAngle))) + (right.getX() * sin(radianAngle));
		rotateXAngle[2][0] = ((right.getX() * right.getZ()) * (1 - cos(radianAngle))) + (right.getY() * sin(radianAngle));
		rotateXAngle[2][1] = ((right.getY() * right.getZ()) * (1 - cos(radianAngle))) - (right.getX() * sin(radianAngle));
		rotateXAngle[2][2] = ((right.getZ() * right.getZ()) * (1 - cos(radianAngle))) + cos(radianAngle);
		rotateXAngle[3][3] = 1;
	}

	void rotateYAxis(double angle){
		double radianAngle = (angle * 3.14159265) / 180;

		rotateYAngle[0][0] = cos(radianAngle);
		rotateYAngle[0][2] = -(sin(radianAngle));
		rotateYAngle[2][0] = sin(radianAngle);
		rotateYAngle[2][2] = cos(radianAngle);
		rotateYAngle[1][1] = 1;
		rotateYAngle[3][3] = 1;
	}

	void multiplyRotationMatrix(){
		rotationMatrix[0][0] = (rotateYAngle[0][0] * rotateXAngle[0][0]) + (rotateYAngle[0][1] * rotateXAngle[1][0]) + (rotateYAngle[0][2] * rotateXAngle[2][0]) + (rotateYAngle[0][3] * rotateXAngle[3][0]);
		rotationMatrix[0][1] = (rotateYAngle[0][0] * rotateXAngle[0][1]) + (rotateYAngle[0][1] * rotateXAngle[1][1]) + (rotateYAngle[0][2] * rotateXAngle[2][1]) + (rotateYAngle[0][3] * rotateXAngle[3][1]);
		rotationMatrix[0][2] = (rotateYAngle[0][0] * rotateXAngle[0][2]) + (rotateYAngle[0][1] * rotateXAngle[1][2]) + (rotateYAngle[0][2] * rotateXAngle[2][2]) + (rotateYAngle[0][3] * rotateXAngle[3][2]);
		rotationMatrix[0][3] = (rotateYAngle[0][0] * rotateXAngle[0][3]) + (rotateYAngle[0][1] * rotateXAngle[1][3]) + (rotateYAngle[0][2] * rotateXAngle[2][3]) + (rotateYAngle[0][3] * rotateXAngle[3][3]);

		rotationMatrix[1][0] = (rotateYAngle[1][0] * rotateXAngle[0][0]) + (rotateYAngle[1][1] * rotateXAngle[1][0]) + (rotateYAngle[1][2] * rotateXAngle[2][0]) + (rotateYAngle[1][3] * rotateXAngle[3][0]);
		rotationMatrix[1][1] = (rotateYAngle[1][0] * rotateXAngle[0][1]) + (rotateYAngle[1][1] * rotateXAngle[1][1]) + (rotateYAngle[1][2] * rotateXAngle[2][1]) + (rotateYAngle[1][3] * rotateXAngle[3][1]);
		rotationMatrix[1][2] = (rotateYAngle[1][0] * rotateXAngle[0][2]) + (rotateYAngle[1][1] * rotateXAngle[1][2]) + (rotateYAngle[1][2] * rotateXAngle[2][2]) + (rotateYAngle[1][3] * rotateXAngle[3][2]);
		rotationMatrix[1][3] = (rotateYAngle[1][0] * rotateXAngle[0][3]) + (rotateYAngle[1][1] * rotateXAngle[1][3]) + (rotateYAngle[1][2] * rotateXAngle[2][3]) + (rotateYAngle[1][3] * rotateXAngle[3][3]);

		rotationMatrix[2][0] = (rotateYAngle[2][0] * rotateXAngle[0][0]) + (rotateYAngle[2][1] * rotateXAngle[1][0]) + (rotateYAngle[2][2] * rotateXAngle[2][0]) + (rotateYAngle[2][3] * rotateXAngle[3][0]);
		rotationMatrix[2][1] = (rotateYAngle[2][0] * rotateXAngle[0][1]) + (rotateYAngle[2][1] * rotateXAngle[1][1]) + (rotateYAngle[2][2] * rotateXAngle[2][1]) + (rotateYAngle[2][3] * rotateXAngle[3][1]);
		rotationMatrix[2][2] = (rotateYAngle[2][0] * rotateXAngle[0][2]) + (rotateYAngle[2][1] * rotateXAngle[1][2]) + (rotateYAngle[2][2] * rotateXAngle[2][2]) + (rotateYAngle[2][3] * rotateXAngle[3][2]);
		rotationMatrix[2][3] = (rotateYAngle[2][0] * rotateXAngle[0][3]) + (rotateYAngle[2][1] * rotateXAngle[1][3]) + (rotateYAngle[2][2] * rotateXAngle[2][3]) + (rotateYAngle[2][3] * rotateXAngle[3][3]);

		rotationMatrix[3][0] = (rotateYAngle[3][0] * rotateXAngle[0][0]) + (rotateYAngle[3][1] * rotateXAngle[1][0]) + (rotateYAngle[3][2] * rotateXAngle[2][0]) + (rotateYAngle[3][3] * rotateXAngle[3][0]);
		rotationMatrix[3][1] = (rotateYAngle[3][0] * rotateXAngle[0][1]) + (rotateYAngle[3][1] * rotateXAngle[1][1]) + (rotateYAngle[3][2] * rotateXAngle[2][1]) + (rotateYAngle[3][3] * rotateXAngle[3][1]);
		rotationMatrix[3][2] = (rotateYAngle[3][0] * rotateXAngle[0][2]) + (rotateYAngle[3][1] * rotateXAngle[1][2]) + (rotateYAngle[3][2] * rotateXAngle[2][2]) + (rotateYAngle[3][3] * rotateXAngle[3][2]);
		rotationMatrix[3][3] = (rotateYAngle[3][0] * rotateXAngle[0][3]) + (rotateYAngle[3][1] * rotateXAngle[1][3]) + (rotateYAngle[3][2] * rotateXAngle[2][3]) + (rotateYAngle[3][3] * rotateXAngle[3][3]);
	}

	void calculateFinalMatrix(){

		finalMatrix[0][0] = (rotationMatrix[0][0] * translateMatrix[0][0]) + (rotationMatrix[0][1] * translateMatrix[1][0]) + (rotationMatrix[0][2] * translateMatrix[2][0]) + (rotationMatrix[0][3] * translateMatrix[3][0]);
		finalMatrix[0][1] = (rotationMatrix[0][0] * translateMatrix[0][1]) + (rotationMatrix[0][1] * translateMatrix[1][1]) + (rotationMatrix[0][2] * translateMatrix[2][1]) + (rotationMatrix[0][3] * translateMatrix[3][1]);
		finalMatrix[0][2] = (rotationMatrix[0][0] * translateMatrix[0][2]) + (rotationMatrix[0][1] * translateMatrix[1][2]) + (rotationMatrix[0][2] * translateMatrix[2][2]) + (rotationMatrix[0][3] * translateMatrix[3][2]);
		finalMatrix[0][3] = (rotationMatrix[0][0] * translateMatrix[0][3]) + (rotationMatrix[0][1] * translateMatrix[1][3]) + (rotationMatrix[0][2] * translateMatrix[2][3]) + (rotationMatrix[0][3] * translateMatrix[3][3]);

		finalMatrix[1][0] = (rotationMatrix[1][0] * translateMatrix[0][0]) + (rotationMatrix[1][1] * translateMatrix[1][0]) + (rotationMatrix[1][2] * translateMatrix[2][0]) + (rotationMatrix[1][3] * translateMatrix[3][0]);
		finalMatrix[1][1] = (rotationMatrix[1][0] * translateMatrix[0][1]) + (rotationMatrix[1][1] * translateMatrix[1][1]) + (rotationMatrix[1][2] * translateMatrix[2][1]) + (rotationMatrix[1][3] * translateMatrix[3][1]);
		finalMatrix[1][2] = (rotationMatrix[1][0] * translateMatrix[0][2]) + (rotationMatrix[1][1] * translateMatrix[1][2]) + (rotationMatrix[1][2] * translateMatrix[2][2]) + (rotationMatrix[1][3] * translateMatrix[3][2]);
		finalMatrix[1][3] = (rotationMatrix[1][0] * translateMatrix[0][3]) + (rotationMatrix[1][1] * translateMatrix[1][3]) + (rotationMatrix[1][2] * translateMatrix[2][3]) + (rotationMatrix[1][3] * translateMatrix[3][3]);

		finalMatrix[2][0] = (rotationMatrix[2][0] * translateMatrix[0][0]) + (rotationMatrix[2][1] * translateMatrix[1][0]) + (rotationMatrix[2][2] * translateMatrix[2][0]) + (rotationMatrix[2][3] * translateMatrix[3][0]);
		finalMatrix[2][1] = (rotationMatrix[2][0] * translateMatrix[0][1]) + (rotationMatrix[2][1] * translateMatrix[1][1]) + (rotationMatrix[2][2] * translateMatrix[2][1]) + (rotationMatrix[2][3] * translateMatrix[3][1]);
		finalMatrix[2][2] = (rotationMatrix[2][0] * translateMatrix[0][2]) + (rotationMatrix[2][1] * translateMatrix[1][2]) + (rotationMatrix[2][2] * translateMatrix[2][2]) + (rotationMatrix[2][3] * translateMatrix[3][2]);
		finalMatrix[2][3] = (rotationMatrix[2][0] * translateMatrix[0][3]) + (rotationMatrix[2][1] * translateMatrix[1][3]) + (rotationMatrix[2][2] * translateMatrix[2][3]) + (rotationMatrix[2][3] * translateMatrix[3][3]);

		finalMatrix[3][0] = (rotationMatrix[3][0] * translateMatrix[0][0]) + (rotationMatrix[3][1] * translateMatrix[1][0]) + (rotationMatrix[3][2] * translateMatrix[2][0]) + (rotationMatrix[3][3] * translateMatrix[3][0]);
		finalMatrix[3][1] = (rotationMatrix[3][0] * translateMatrix[0][1]) + (rotationMatrix[3][1] * translateMatrix[1][1]) + (rotationMatrix[3][2] * translateMatrix[2][1]) + (rotationMatrix[3][3] * translateMatrix[3][1]);
		finalMatrix[3][2] = (rotationMatrix[3][0] * translateMatrix[0][2]) + (rotationMatrix[3][1] * translateMatrix[1][2]) + (rotationMatrix[3][2] * translateMatrix[2][2]) + (rotationMatrix[3][3] * translateMatrix[3][2]);
		finalMatrix[3][3] = (rotationMatrix[3][0] * translateMatrix[0][3]) + (rotationMatrix[3][1] * translateMatrix[1][3]) + (rotationMatrix[3][2] * translateMatrix[2][3]) + (rotationMatrix[3][3] * translateMatrix[3][3]);
	}

	void displayObjectMatrixToConsole()
	{
		cout << "ObjectTranslateMatrix" << endl;
		for (int i = 0; i < 4; i++)
		{
			cout << "[";
			for (int j = 0; j < 4; j++)
			{
				cout << translateMatrix[i][j] << ", ";
			}
			cout << "]" << endl;
		}
	}

	double getElement(int element){
		if (element == 11)
			return finalMatrix[0][0];
		else if (element == 12)
			return finalMatrix[0][1];
		else if (element == 13)
			return finalMatrix[0][2];
		else if (element == 14)
			return finalMatrix[0][3];
		else if (element == 21)
			return finalMatrix[1][0];
		else if (element == 22)
			return finalMatrix[1][1];
		else if (element == 23)
			return finalMatrix[1][2];
		else if (element == 24)
			return finalMatrix[1][3];
		else if (element == 31)
			return finalMatrix[2][0];
		else if (element == 32)
			return finalMatrix[2][1];
		else if (element == 33)
			return finalMatrix[2][2];
		else if (element == 34)
			return finalMatrix[2][3];
		else if (element == 41)
			return finalMatrix[3][0];
		else if (element == 42)
			return finalMatrix[3][1];
		else if (element == 43)
			return finalMatrix[3][2];
		else if (element == 44)
			return finalMatrix[3][3];
		else
			return 0;
	}

	int returnRed(){
		return colorR;
	}

	int returnGreen(){
		return colorG;
	}

	int returnBlue(){
		return colorB;
	}

	void setColor(int r, int g, int b){
		colorR = r;
		colorG = g;
		colorB = b;
	}
};

#endif