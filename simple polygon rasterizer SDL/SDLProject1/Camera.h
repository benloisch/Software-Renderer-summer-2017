#ifndef CAMERA_H
#define CAMERA_H

#include "Vertex.h"
#include <iostream>
using namespace std;

class Camera
{
private:
	double CameraMatrix[4][4];
	double CopyCameraMatrix[4][4];
	double CameraTranslationMatrix[4][4];
	double CameraRotationMatrix[4][4];
	double AdjointMatrix[4][4];
	double TransposeAdjointMatrix[4][4];
	double InverseCameraMatrix[4][4];
	double CameraXAngle[4][4];
	double CameraYAngle[4][4];
	Vertex lookAtVec;
	double camX;
	double camY;
	double camZ;
	double determinant;
	Vertex right;
	Vertex up;
	//Vertex forward;
	bool calculateFirstTarget;
public:

	Camera()
	{
		camX = 0;
		camY = 0;
		camZ = 0;
		lookAtVec.setX(0);
		lookAtVec.setY(0);
		lookAtVec.setZ(0);
		lookAtVec.setW(1);
		determinant = 0;
		calculateFirstTarget = true;
		for (int x = 0; x < 4; x++){
			for (int y = 0; y < 4; y++){
				CameraMatrix[x][y] = 0;
				CopyCameraMatrix[x][y] = 0;
				AdjointMatrix[x][y] = 0;
				TransposeAdjointMatrix[x][y] = 0;
				InverseCameraMatrix[x][y] = 0;
				CameraTranslationMatrix[x][y] = 0;
				CameraXAngle[x][y] = 0;
				CameraYAngle[x][y] = 0;
				CameraRotationMatrix[x][y] = 0;
				if (x == y){
					CameraTranslationMatrix[x][y] = 1;
					CameraXAngle[x][y] = 1;
					CameraYAngle[x][y] = 1;
					CameraRotationMatrix[x][y] = 1;
					CopyCameraMatrix[x][y] = 1;
				}
			}
		}
	}

	Camera(double cX, double cY, double cZ, double lookatX, double lookatY, double lookatZ) : camX(cX), camY(cY), camZ(cZ)
	{
		lookAtVec.setX(lookatX);
		lookAtVec.setY(lookatY);
		lookAtVec.setZ(lookatZ);
		lookAtVec.setW(1);
		calculateFirstTarget = true;
		for (int x = 0; x < 4; x++){
			for (int y = 0; y < 4; y++){
				CameraMatrix[x][y] = 0;
				CopyCameraMatrix[x][y] = 0;
				AdjointMatrix[x][y] = 0;
				TransposeAdjointMatrix[x][y] = 0;
				InverseCameraMatrix[x][y] = 0;
				CameraTranslationMatrix[x][y] = 0;
				CameraXAngle[x][y] = 0;
				CameraYAngle[x][y] = 0;
				CameraRotationMatrix[x][y] = 0;
				if (x == y){
					CameraTranslationMatrix[x][y] = 1;
					CameraXAngle[x][y] = 1;
					CameraYAngle[x][y] = 1;
					CameraRotationMatrix[x][y] = 1;
					CopyCameraMatrix[x][y] = 1;
				}
			}
		}
		lookAtVec.setX(lookAtVec.getX() - camX);
		lookAtVec.setY(lookAtVec.getY() - camY);
		lookAtVec.setZ(lookAtVec.getZ() - camZ);
	}

	void CreateCamMatrix()
	{
		if (calculateFirstTarget == true){
			double x = lookAtVec.getX();
			double y = lookAtVec.getY();
			double z = lookAtVec.getZ();
			lookAtVec.setX((x * CameraRotationMatrix[0][0]) + (y * CameraRotationMatrix[1][0]) + (z * CameraRotationMatrix[2][0])); 
			lookAtVec.setY((x * CameraRotationMatrix[0][1]) + (y * CameraRotationMatrix[1][1]) + (z * CameraRotationMatrix[2][1]));
			lookAtVec.setZ((x * CameraRotationMatrix[0][2]) + (y * CameraRotationMatrix[1][2]) + (z * CameraRotationMatrix[2][2]));
		}

		//Cross Product is
		//Cx = AyBz - AzBy
		//Cy = AzBx - AxBz
		//Cz = AxBy - AyBx

		//Calculate the right Vertex (x axis) by taking cross product of world 'up'(0, 1, 0) and lookat Vertex
		right.setX((1 * lookAtVec.getZ()) - (0 * lookAtVec.getY()));
		right.setY((0 * lookAtVec.getX()) - (0 * lookAtVec.getZ()));
		right.setZ((0 * lookAtVec.getY()) - (1 * lookAtVec.getX()));
		
		//cout << "RIGHT" << right.getX() << " / " << right.getY() << " / " << right.getZ() << " / " << endl;
		//Calculate camera up Vertex (y axis) by taking the cross product of lookat Vertex and right(just above) Vertex

		up.setX((lookAtVec.getY() * right.getZ()) - (lookAtVec.getZ() * right.getY()));
		up.setY((lookAtVec.getZ() * right.getX()) - (lookAtVec.getX() * right.getZ()));
		up.setZ((lookAtVec.getX() * right.getY()) - (lookAtVec.getY() * right.getX()));
	
		//up.setX(0);
		
		//cout << "UP: " << "X: " << up.getX() << "Y: " << up.getY() << "Z: " << up.getZ() << endl;
		//Last, calculate forward Vertex (positive z axis) which is just the lookat Vertex

		//forward.setX(lookAtVec.getX());
		//forward.setY(lookAtVec.getY());
		//forward.setZ(lookAtVec.getZ());

		//Now, to make the camera transformation matrix, everything must be normalized

		right.normalize();
		up.normalize();
		lookAtVec.normalize();
		//cout << "LOOKAT " << " X: " << lookAtVec.getX() << " Y: " << lookAtVec.getY() << " Z: " << lookAtVec.getZ() << endl;
		//cout << "LOOKAT" << " X: " << lookAtVec.getX() << " Y: " << lookAtVec.getY() << " Z: " << lookAtVec.getZ() << endl;
		//cout << "Forward" << "X: " << forward.getX() << "Y: " << forward.getY() << "Z: " << forward.getZ() << endl;
		
		//cout << "DOTPRODUCT: " << ((forward.getX() * right.getX()) + (forward.getY() * right.getY()) + (forward.getZ() * right.getZ())) << endl;
		//Now, setup the camera matrix
		//right, up, and forward represent the camera's x, y, and z axis
		//the translation of the camera is the camera's origin in world space
		/*
		CameraMatrix[0][0] = right.getX();
		CameraMatrix[0][1] = right.getY();
		CameraMatrix[0][2] = right.getZ();
		CameraMatrix[0][3] = 0;

		CameraMatrix[1][0] = up.getX();
		CameraMatrix[1][1] = up.getY();
		CameraMatrix[1][2] = up.getZ();
		CameraMatrix[1][3] = 0;

		CameraMatrix[2][0] = forward.getX();
		CameraMatrix[2][1] = forward.getY();
		CameraMatrix[2][2] = forward.getZ();
		CameraMatrix[2][3] = 0;

		CameraMatrix[3][0] = camX;
		CameraMatrix[3][1] = camY;
		CameraMatrix[3][2] = camZ;
		CameraMatrix[3][3] = 1;
		*/
		CopyCameraMatrix[0][0] = right.getX();
		CopyCameraMatrix[0][1] = right.getY();
		CopyCameraMatrix[0][2] = right.getZ();
		//CopyCameraMatrix[0][3] = 0;
		
		CopyCameraMatrix[1][0] = up.getX();
		CopyCameraMatrix[1][1] = up.getY();
		CopyCameraMatrix[1][2] = up.getZ();
		//CopyCameraMatrix[1][3] = 0;

		CopyCameraMatrix[2][0] = lookAtVec.getX();
		CopyCameraMatrix[2][1] = lookAtVec.getY();
		CopyCameraMatrix[2][2] = lookAtVec.getZ();
		//CopyCameraMatrix[2][3] = 0;

		CopyCameraMatrix[3][0] = camX;
		CopyCameraMatrix[3][1] = camY;
		CopyCameraMatrix[3][2] = camZ;
	}

	double calculate3x3Determinant(double m11, double m12, double m13, double m21, double m22, double m23, double m31, double m32, double m33){
		//x + y + z = determinant
		double x = m11 * ((m22 * m33) - (m23 * m32));
		double y = m12 * ((m23 * m31) - (m21 * m33));
		double z = m13 * ((m21 * m32) - (m22 * m31));
		return x + y + z;
	}

	void calculateCameraDeterminant(){
		//a - b + c - d = determinant
		double a = CopyCameraMatrix[0][0] * calculate3x3Determinant(CopyCameraMatrix[1][1], CopyCameraMatrix[1][2], CopyCameraMatrix[1][3], CopyCameraMatrix[2][1], CopyCameraMatrix[2][2], CopyCameraMatrix[2][3], CopyCameraMatrix[3][1], CopyCameraMatrix[3][2], CopyCameraMatrix[3][3]);
		double b = CopyCameraMatrix[0][1] * calculate3x3Determinant(CopyCameraMatrix[1][0], CopyCameraMatrix[1][2], CopyCameraMatrix[1][3], CopyCameraMatrix[2][0], CopyCameraMatrix[2][2], CopyCameraMatrix[2][3], CopyCameraMatrix[3][0], CopyCameraMatrix[3][2], CopyCameraMatrix[3][3]);
		double c = CopyCameraMatrix[0][2] * calculate3x3Determinant(CopyCameraMatrix[1][0], CopyCameraMatrix[1][1], CopyCameraMatrix[1][3], CopyCameraMatrix[2][0], CopyCameraMatrix[2][1], CopyCameraMatrix[2][3], CopyCameraMatrix[3][0], CopyCameraMatrix[3][1], CopyCameraMatrix[3][3]);
		double d = CopyCameraMatrix[0][3] * calculate3x3Determinant(CopyCameraMatrix[1][0], CopyCameraMatrix[1][1], CopyCameraMatrix[1][2], CopyCameraMatrix[2][0], CopyCameraMatrix[2][1], CopyCameraMatrix[2][2], CopyCameraMatrix[3][0], CopyCameraMatrix[3][1], CopyCameraMatrix[3][2]);
		determinant = a - b + c - d;
	}

	void calculateAdjointMatrix(){
		/*Adjoint Matrix
		[a -b c -d]
		[-e f -g h]
		[i -j k -l]
		[-m n -o p] Then Transpose this matrix.
		*/
		AdjointMatrix[0][0] = calculate3x3Determinant(CopyCameraMatrix[1][1], CopyCameraMatrix[1][2], CopyCameraMatrix[1][3], CopyCameraMatrix[2][1], CopyCameraMatrix[2][2], CopyCameraMatrix[2][3], CopyCameraMatrix[3][1], CopyCameraMatrix[3][2], CopyCameraMatrix[3][3]);
		AdjointMatrix[0][1] = (-1) * calculate3x3Determinant(CopyCameraMatrix[1][0], CopyCameraMatrix[1][2], CopyCameraMatrix[1][3], CopyCameraMatrix[2][0], CopyCameraMatrix[2][2], CopyCameraMatrix[2][3], CopyCameraMatrix[3][0], CopyCameraMatrix[3][1], CopyCameraMatrix[3][3]);
		AdjointMatrix[0][2] = calculate3x3Determinant(CopyCameraMatrix[1][0], CopyCameraMatrix[1][1], CopyCameraMatrix[1][3], CopyCameraMatrix[2][0], CopyCameraMatrix[2][1], CopyCameraMatrix[2][3], CopyCameraMatrix[3][0], CopyCameraMatrix[3][1], CopyCameraMatrix[3][3]);
		AdjointMatrix[0][3] = (-1) * calculate3x3Determinant(CopyCameraMatrix[1][0], CopyCameraMatrix[1][1], CopyCameraMatrix[1][2], CopyCameraMatrix[2][0], CopyCameraMatrix[2][1], CopyCameraMatrix[2][2], CopyCameraMatrix[3][0], CopyCameraMatrix[3][1], CopyCameraMatrix[3][2]);
		AdjointMatrix[1][0] = (-1) * calculate3x3Determinant(CopyCameraMatrix[0][1], CopyCameraMatrix[0][2], CopyCameraMatrix[0][3], CopyCameraMatrix[2][1], CopyCameraMatrix[2][2], CopyCameraMatrix[2][3], CopyCameraMatrix[3][1], CopyCameraMatrix[3][2], CopyCameraMatrix[3][3]);
		AdjointMatrix[1][1] = calculate3x3Determinant(CopyCameraMatrix[0][0], CopyCameraMatrix[0][2], CopyCameraMatrix[0][3], CopyCameraMatrix[2][0], CopyCameraMatrix[2][2], CopyCameraMatrix[2][3], CopyCameraMatrix[3][0], CopyCameraMatrix[3][2], CopyCameraMatrix[3][3]);
		AdjointMatrix[1][2] = (-1) * calculate3x3Determinant(CopyCameraMatrix[0][0], CopyCameraMatrix[0][1], CopyCameraMatrix[0][3], CopyCameraMatrix[2][0], CopyCameraMatrix[2][1], CopyCameraMatrix[2][3], CopyCameraMatrix[3][0], CopyCameraMatrix[3][1], CopyCameraMatrix[3][3]);
		AdjointMatrix[1][3] = calculate3x3Determinant(CopyCameraMatrix[0][0], CopyCameraMatrix[0][1], CopyCameraMatrix[0][2], CopyCameraMatrix[2][0], CopyCameraMatrix[2][1], CopyCameraMatrix[2][2], CopyCameraMatrix[3][0], CopyCameraMatrix[3][1], CopyCameraMatrix[3][2]);
		AdjointMatrix[2][0] = calculate3x3Determinant(CopyCameraMatrix[0][1], CopyCameraMatrix[0][2], CopyCameraMatrix[0][3], CopyCameraMatrix[1][1], CopyCameraMatrix[1][2], CopyCameraMatrix[1][3], CopyCameraMatrix[3][1], CopyCameraMatrix[3][2], CopyCameraMatrix[3][3]);
		AdjointMatrix[2][1] = (-1) * calculate3x3Determinant(CopyCameraMatrix[0][0], CopyCameraMatrix[0][2], CopyCameraMatrix[0][3], CopyCameraMatrix[1][0], CopyCameraMatrix[1][2], CopyCameraMatrix[1][3], CopyCameraMatrix[3][0], CopyCameraMatrix[3][2], CopyCameraMatrix[3][3]);
		AdjointMatrix[2][2] = calculate3x3Determinant(CopyCameraMatrix[0][0], CopyCameraMatrix[0][1], CopyCameraMatrix[0][3], CopyCameraMatrix[1][0], CopyCameraMatrix[1][1], CopyCameraMatrix[1][3], CopyCameraMatrix[3][0], CopyCameraMatrix[3][1], CopyCameraMatrix[3][3]);
		AdjointMatrix[2][3] = (-1) * calculate3x3Determinant(CopyCameraMatrix[0][0], CopyCameraMatrix[0][1], CopyCameraMatrix[0][2], CopyCameraMatrix[1][0], CopyCameraMatrix[1][1], CopyCameraMatrix[1][2], CopyCameraMatrix[3][0], CopyCameraMatrix[3][1], CopyCameraMatrix[3][2]);
		AdjointMatrix[3][0] = (-1) * calculate3x3Determinant(CopyCameraMatrix[0][1], CopyCameraMatrix[0][2], CopyCameraMatrix[0][3], CopyCameraMatrix[1][1], CopyCameraMatrix[1][2], CopyCameraMatrix[1][3], CopyCameraMatrix[2][1], CopyCameraMatrix[2][2], CopyCameraMatrix[2][3]);
		AdjointMatrix[3][1] = calculate3x3Determinant(CopyCameraMatrix[0][0], CopyCameraMatrix[0][2], CopyCameraMatrix[0][3], CopyCameraMatrix[1][0], CopyCameraMatrix[1][2], CopyCameraMatrix[1][3], CopyCameraMatrix[2][0], CopyCameraMatrix[2][2], CopyCameraMatrix[2][3]);
		AdjointMatrix[3][2] = (-1) * calculate3x3Determinant(CopyCameraMatrix[0][0], CopyCameraMatrix[0][1], CopyCameraMatrix[0][3], CopyCameraMatrix[1][0], CopyCameraMatrix[1][1], CopyCameraMatrix[1][3], CopyCameraMatrix[2][0], CopyCameraMatrix[2][1], CopyCameraMatrix[2][3]);
		AdjointMatrix[3][3] = calculate3x3Determinant(CopyCameraMatrix[0][0], CopyCameraMatrix[0][1], CopyCameraMatrix[0][2], CopyCameraMatrix[1][0], CopyCameraMatrix[1][1], CopyCameraMatrix[1][2], CopyCameraMatrix[2][0], CopyCameraMatrix[2][1], CopyCameraMatrix[2][2]);
		
		//Now transpose the matrix
		for (int x = 0; x < 4; x++){
			for (int y = 0; y < 4; y++){
				TransposeAdjointMatrix[x][y] = AdjointMatrix[y][x];
			}
		}
	}

	void calculateInverseCameraMatrix(){
		for (int x = 0; x < 4; x++){
			for (int y = 0; y < 4; y++){
				InverseCameraMatrix[x][y] = TransposeAdjointMatrix[x][y] / determinant;
			}
		}
	}

	void translate(double x, double y, double z){
		CameraTranslationMatrix[3][0] = x;
		CameraTranslationMatrix[3][1] = y;
		CameraTranslationMatrix[3][2] = z;
	}

	void rotateXAxis(double angle){
		calculateFirstTarget = true;

		double radianAngle = (angle * 3.14159265) / 180;
		CameraXAngle[0][0] = ((right.getX() * right.getX()) * (1 - cos(radianAngle))) + cos(radianAngle);
		CameraXAngle[0][1] = ((right.getX() * right.getY()) * (1 - cos(radianAngle))) + (right.getZ() * sin(radianAngle));
		CameraXAngle[0][2] = ((right.getX() * right.getZ()) * (1 - cos(radianAngle))) - (right.getY() * sin(radianAngle));
		CameraXAngle[1][0] = ((right.getX() * right.getY()) * (1 - cos(radianAngle))) - (right.getZ() * sin(radianAngle));
		CameraXAngle[1][1] = ((right.getY() * right.getY()) * (1 - cos(radianAngle))) + cos(radianAngle);
		CameraXAngle[1][2] = ((right.getY() * right.getZ()) * (1 - cos(radianAngle))) + (right.getX() * sin(radianAngle));
		CameraXAngle[2][0] = ((right.getX() * right.getZ()) * (1 - cos(radianAngle))) + (right.getY() * sin(radianAngle));
		CameraXAngle[2][1] = ((right.getY() * right.getZ()) * (1 - cos(radianAngle))) - (right.getX() * sin(radianAngle));
		CameraXAngle[2][2] = ((right.getZ() * right.getZ()) * (1 - cos(radianAngle))) + cos(radianAngle);
		CameraXAngle[3][3] = 1;
	}

	void rotateYAxis(double angle){
		calculateFirstTarget = true;
		double radianAngle = (angle * 3.14159265) / 180;
		/*
		up.setXPrime(0);
		up.setYPrime(1.0);
		up.setZPrime(0);
		
		CameraYAngle[0][0] = ((up.getXPrime() * up.getXPrime()) * (1.0 - cos(radianAngle))) + cos(radianAngle);
		CameraYAngle[0][1] = ((up.getXPrime() * up.getYPrime()) * (1.0 - cos(radianAngle))) + (up.getZPrime() * sin(radianAngle));
		CameraYAngle[0][2] = ((up.getXPrime() * up.getZPrime()) * (1.0 - cos(radianAngle))) - (up.getYPrime() * sin(radianAngle));
		CameraYAngle[1][0] = ((up.getXPrime() * up.getYPrime()) * (1.0 - cos(radianAngle))) - (up.getZPrime() * sin(radianAngle));
		CameraYAngle[1][1] = ((up.getYPrime() * up.getYPrime()) * (1.0 - cos(radianAngle))) + cos(radianAngle);
		CameraYAngle[1][2] = ((up.getYPrime() * up.getZPrime()) * (1.0 - cos(radianAngle))) + (up.getXPrime() * sin(radianAngle));
		CameraYAngle[2][0] = ((up.getXPrime() * up.getZPrime()) * (1.0 - cos(radianAngle))) + (up.getYPrime() * sin(radianAngle));
		CameraYAngle[2][1] = ((up.getYPrime() * up.getZPrime()) * (1.0 - cos(radianAngle))) - (up.getXPrime() * sin(radianAngle));
		CameraYAngle[2][2] = ((up.getZPrime() * up.getZPrime()) * (1.0 - cos(radianAngle))) + cos(radianAngle);
		CameraYAngle[3][3] = 1.0;
		*/
		
		CameraYAngle[0][0] = cos(radianAngle);
		CameraYAngle[0][2] = -(sin(radianAngle));
		CameraYAngle[2][0] = sin(radianAngle);
		CameraYAngle[2][2] = cos(radianAngle);
		CameraYAngle[1][1] = 1;
		CameraYAngle[3][3] = 1;
	}

	void multiplyRotationMatrix(){
		CameraRotationMatrix[0][0] = (CameraYAngle[0][0] * CameraXAngle[0][0]) + (CameraYAngle[0][1] * CameraXAngle[1][0]) + (CameraYAngle[0][2] * CameraXAngle[2][0]) + (CameraYAngle[0][3] * CameraXAngle[3][0]);
		CameraRotationMatrix[0][1] = (CameraYAngle[0][0] * CameraXAngle[0][1]) + (CameraYAngle[0][1] * CameraXAngle[1][1]) + (CameraYAngle[0][2] * CameraXAngle[2][1]) + (CameraYAngle[0][3] * CameraXAngle[3][1]);
		CameraRotationMatrix[0][2] = (CameraYAngle[0][0] * CameraXAngle[0][2]) + (CameraYAngle[0][1] * CameraXAngle[1][2]) + (CameraYAngle[0][2] * CameraXAngle[2][2]) + (CameraYAngle[0][3] * CameraXAngle[3][2]);
		CameraRotationMatrix[0][3] = (CameraYAngle[0][0] * CameraXAngle[0][3]) + (CameraYAngle[0][1] * CameraXAngle[1][3]) + (CameraYAngle[0][2] * CameraXAngle[2][3]) + (CameraYAngle[0][3] * CameraXAngle[3][3]);

		CameraRotationMatrix[1][0] = (CameraYAngle[1][0] * CameraXAngle[0][0]) + (CameraYAngle[1][1] * CameraXAngle[1][0]) + (CameraYAngle[1][2] * CameraXAngle[2][0]) + (CameraYAngle[1][3] * CameraXAngle[3][0]);
		CameraRotationMatrix[1][1] = (CameraYAngle[1][0] * CameraXAngle[0][1]) + (CameraYAngle[1][1] * CameraXAngle[1][1]) + (CameraYAngle[1][2] * CameraXAngle[2][1]) + (CameraYAngle[1][3] * CameraXAngle[3][1]);
		CameraRotationMatrix[1][2] = (CameraYAngle[1][0] * CameraXAngle[0][2]) + (CameraYAngle[1][1] * CameraXAngle[1][2]) + (CameraYAngle[1][2] * CameraXAngle[2][2]) + (CameraYAngle[1][3] * CameraXAngle[3][2]);
		CameraRotationMatrix[1][3] = (CameraYAngle[1][0] * CameraXAngle[0][3]) + (CameraYAngle[1][1] * CameraXAngle[1][3]) + (CameraYAngle[1][2] * CameraXAngle[2][3]) + (CameraYAngle[1][3] * CameraXAngle[3][3]);

		CameraRotationMatrix[2][0] = (CameraYAngle[2][0] * CameraXAngle[0][0]) + (CameraYAngle[2][1] * CameraXAngle[1][0]) + (CameraYAngle[2][2] * CameraXAngle[2][0]) + (CameraYAngle[2][3] * CameraXAngle[3][0]);
		CameraRotationMatrix[2][1] = (CameraYAngle[2][0] * CameraXAngle[0][1]) + (CameraYAngle[2][1] * CameraXAngle[1][1]) + (CameraYAngle[2][2] * CameraXAngle[2][1]) + (CameraYAngle[2][3] * CameraXAngle[3][1]);
		CameraRotationMatrix[2][2] = (CameraYAngle[2][0] * CameraXAngle[0][2]) + (CameraYAngle[2][1] * CameraXAngle[1][2]) + (CameraYAngle[2][2] * CameraXAngle[2][2]) + (CameraYAngle[2][3] * CameraXAngle[3][2]);
		CameraRotationMatrix[2][3] = (CameraYAngle[2][0] * CameraXAngle[0][3]) + (CameraYAngle[2][1] * CameraXAngle[1][3]) + (CameraYAngle[2][2] * CameraXAngle[2][3]) + (CameraYAngle[2][3] * CameraXAngle[3][3]);

		CameraRotationMatrix[3][0] = (CameraYAngle[3][0] * CameraXAngle[0][0]) + (CameraYAngle[3][1] * CameraXAngle[1][0]) + (CameraYAngle[3][2] * CameraXAngle[2][0]) + (CameraYAngle[3][3] * CameraXAngle[3][0]);
		CameraRotationMatrix[3][1] = (CameraYAngle[3][0] * CameraXAngle[0][1]) + (CameraYAngle[3][1] * CameraXAngle[1][1]) + (CameraYAngle[3][2] * CameraXAngle[2][1]) + (CameraYAngle[3][3] * CameraXAngle[3][1]);
		CameraRotationMatrix[3][2] = (CameraYAngle[3][0] * CameraXAngle[0][2]) + (CameraYAngle[3][1] * CameraXAngle[1][2]) + (CameraYAngle[3][2] * CameraXAngle[2][2]) + (CameraYAngle[3][3] * CameraXAngle[3][2]);
		CameraRotationMatrix[3][3] = (CameraYAngle[3][0] * CameraXAngle[0][3]) + (CameraYAngle[3][1] * CameraXAngle[1][3]) + (CameraYAngle[3][2] * CameraXAngle[2][3]) + (CameraYAngle[3][3] * CameraXAngle[3][3]);
	}

	void displayCopyCameraMatrixtoConsole()
	{
		cout << "COPY CAM MATRIX" << endl;
		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				cout << CopyCameraMatrix[i][j] << " ";
			}
			cout << endl;
		}
	}

	void displayAdjointMatrixtoConsole()
	{
		for (int i = 0; i < 4; i++)
		{
			cout << "[";
			for (int j = 0; j < 4; j++)
			{
				cout << AdjointMatrix[i][j] << ", ";
			}
			cout << "]" << endl;
		}
	}

	void displayTransposeAdjointMatrixtoConsole()
	{
		for (int i = 0; i < 4; i++)
		{
			cout << "[";
			for (int j = 0; j < 4; j++)
			{
				cout << TransposeAdjointMatrix[i][j] << ", ";
			}
			cout << "]" << endl;
		}
	}

	void displayInverseCameraMatrixtoConsole()
	{
		cout << "INVERSE CAMERA MATRIX" << endl;
		for (int i = 0; i < 4; i++)
		{
			cout << "[";
			for (int j = 0; j < 4; j++)
			{
				cout << InverseCameraMatrix[i][j] << ", ";
			}
			cout << "]" << endl;
		}
	}

	void displayTranslationCameraMatrixtoConsole()
	{
		cout << "TRANSLATION CAMERA MATRIX" << endl;
		for (int i = 0; i < 4; i++)
		{
			cout << "[";
			for (int j = 0; j < 4; j++)
			{
				cout << CameraTranslationMatrix[i][j] << ", ";
			}
			cout << "]" << endl;
		}
	}

	void displayDeterminant(){
		cout << "Cam determinant: " << determinant << endl;
	}

	double getInverseMatrixElement(int element){
		if (element == 11)
			return InverseCameraMatrix[0][0];
		else if (element == 12)
			return InverseCameraMatrix[0][1];
		else if (element == 13)
			return InverseCameraMatrix[0][2];
		else if (element == 14)
			return InverseCameraMatrix[0][3];
		else if (element == 21)
			return InverseCameraMatrix[1][0];
		else if (element == 22)
			return InverseCameraMatrix[1][1];
		else if (element == 23)
			return InverseCameraMatrix[1][2];
		else if (element == 24)
			return InverseCameraMatrix[1][3];
		else if (element == 31)
			return InverseCameraMatrix[2][0];
		else if (element == 32)
			return InverseCameraMatrix[2][1];
		else if (element == 33)
			return InverseCameraMatrix[2][2];
		else if (element == 34)
			return InverseCameraMatrix[2][3];
		else if (element == 41)
			return InverseCameraMatrix[3][0];
		else if (element == 42)
			return InverseCameraMatrix[3][1];
		else if (element == 43)
			return InverseCameraMatrix[3][2];
		else if (element == 44)
			return InverseCameraMatrix[3][3];
		else
			return 0;
	}

	double getTranslationElement(int element){
		if (element == 11)
			return CameraTranslationMatrix[0][0];
		else if (element == 12)
			return CameraTranslationMatrix[0][1];
		else if (element == 13)
			return CameraTranslationMatrix[0][2];
		else if (element == 14)
			return CameraTranslationMatrix[0][3];
		else if (element == 21)
			return CameraTranslationMatrix[1][0];
		else if (element == 22)
			return CameraTranslationMatrix[1][1];
		else if (element == 23)
			return CameraTranslationMatrix[1][2];
		else if (element == 24)
			return CameraTranslationMatrix[1][3];
		else if (element == 31)
			return CameraTranslationMatrix[2][0];
		else if (element == 32)
			return CameraTranslationMatrix[2][1];
		else if (element == 33)
			return CameraTranslationMatrix[2][2];
		else if (element == 34)
			return CameraTranslationMatrix[2][3];
		else if (element == 41)
			return CameraTranslationMatrix[3][0];
		else if (element == 42)
			return CameraTranslationMatrix[3][1];
		else if (element == 43)
			return CameraTranslationMatrix[3][2];
		else if (element == 44)
			return CameraTranslationMatrix[3][3];
		else
			return 0;
	}

	double getRotationElement(int element){
		if (element == 11)
			return CameraRotationMatrix[0][0];
		else if (element == 12)
			return CameraRotationMatrix[0][1];
		else if (element == 13)
			return CameraRotationMatrix[0][2];
		else if (element == 14)
			return CameraRotationMatrix[0][3];
		else if (element == 21)
			return CameraRotationMatrix[1][0];
		else if (element == 22)
			return CameraRotationMatrix[1][1];
		else if (element == 23)
			return CameraRotationMatrix[1][2];
		else if (element == 24)
			return CameraRotationMatrix[1][3];
		else if (element == 31)
			return CameraRotationMatrix[2][0];
		else if (element == 32)
			return CameraRotationMatrix[2][1];
		else if (element == 33)
			return CameraRotationMatrix[2][2];
		else if (element == 34)
			return CameraRotationMatrix[2][3];
		else if (element == 41)
			return CameraRotationMatrix[3][0];
		else if (element == 42)
			return CameraRotationMatrix[3][1];
		else if (element == 43)
			return CameraRotationMatrix[3][2];
		else if (element == 44)
			return CameraRotationMatrix[3][3];
		else
			return 0;
	}

	/*
	void multiplyVertexbyInverseCameraMatrix(Vertex &vec)
	{
		double vecXunChanged = vec.getX();
		double vecYunChanged = vec.getY();
		double vecZunChanged = vec.getZ();

		vec.setX((vecXunChanged * CameraMatrix[0][0]) + (vecYunChanged  * CameraMatrix[1][0]) + (vecZunChanged * CameraMatrix[2][0]) + (1 * CameraMatrix[3][0]));
		vec.setY((vecXunChanged * CameraMatrix[0][1]) + (vecYunChanged  * CameraMatrix[1][1]) + (vecZunChanged * CameraMatrix[2][1]) + (1 * CameraMatrix[3][1]));
		vec.setZ((vecXunChanged * CameraMatrix[0][2]) + (vecYunChanged  * CameraMatrix[1][2]) + (vecZunChanged * CameraMatrix[2][2]) + (1 * CameraMatrix[3][2]));
	}
	*/
	double getxOrigin()
	{
		return camX;
	}

	double getyOrigin()
	{
		return camY;
	}

	double getzOrigin()
	{
		return camZ;
	}

	Vertex getRight(){
		return right;
	}

	

	void moveCameraRight(){
		calculateFirstTarget = false;
		camX += right.getX() / 2;
		camY += right.getY() / 2;
		camZ += right.getZ() / 2;
	}

	void moveCameraLeft(){
		calculateFirstTarget = false;
		camX -= right.getX() / 2;
		camY -= right.getY() / 2;
		camZ -= right.getZ() / 2;
	}

	void moveCameraForward(){
		calculateFirstTarget = false;
		camX += lookAtVec.getX() / 2;
		camY += lookAtVec.getY() / 2;
		camZ += lookAtVec.getZ() / 2;
	}

	void moveCameraBack(){
		calculateFirstTarget = false;
		camX -= lookAtVec.getX() / 2;
		camY -= lookAtVec.getY() / 2;
		camZ -= lookAtVec.getZ() / 2;
	}

	void camRotationIdentity(){
		for (int x = 0; x < 4; x++){
			for (int y = 0; y < 4; y++){
				CameraXAngle[x][y] = 0;
				CameraYAngle[x][y] = 0;
				CameraRotationMatrix[x][y] = 0;
				if (x == y){
					CameraXAngle[x][y] = 1;
					CameraYAngle[x][y] = 1;
					CameraRotationMatrix[x][y] = 1;
					CopyCameraMatrix[x][y] = 1;
				}
			}
		}
	}
	
};

#endif