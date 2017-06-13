#ifndef CAMERA_H_
#define CAMERA_H_

#include <math.h>

#include "Matrix4x4.h"
#include "Vector4D.h"

class Camera {
public:

	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Vector4D originPosition;
	Vector4D lookDirection;

	double nearPlane;
	double farPlane;
	double aspectRatio;
	double fieldOfView;

	Camera();

	void setOriginPosition(double, double, double);
	void setLookDirection(double, double, double);
	void setNearPlane(double);
	void setFarPlane(double);
	void setAspectRatio(double, double);
	void setFieldOfView(double);

	void calculateViewMatrix();
	void calculateProjectionMatrix();

private:


};

inline void Camera::calculateViewMatrix() {	
	//Cross Product
	//Cx = AyBz - AzBy
	//Cy = AzBx - AxBz
	//Cz = AxBy - AyBx

	//calculate x basis vector by taking cross product of world 'up' (0, 1, 0) vector and lookDirection vector
	Vector4D up(0.0, 1.0, 0.0, 0.0);
	Vector4D xBasis = up ^ lookDirection;

	//calculate y basis vector by taking cross product of lookDirection and x basis vector
	Vector4D yBasis = lookDirection ^ xBasis;

	//the lookDirection is already the z basis vector
	Vector4D zBasis = lookDirection;

	xBasis.normalize();
	yBasis.normalize();
	zBasis.normalize();

	viewMatrix.setRow1(xBasis.x, xBasis.y, xBasis.z, 0.0);
	viewMatrix.setRow2(yBasis.x, yBasis.y, yBasis.z, 0.0);
	viewMatrix.setRow3(zBasis.x, zBasis.y, zBasis.z, 0.0);
	viewMatrix.setRow4(originPosition.x, originPosition.y, originPosition.z, 1.0);


}

inline void Camera::calculateProjectionMatrix() {
	/*
	[w, 0, 0, 0]
	[0, h, 0, 0]
	[0, 0, Q, 1]
	[0, 0, V, 0]
	
	w = cot(FOV_width / 2)
	h = cot(FOV_height / 2)
	Q = farPlane / (farPlane - nearPlane)
	V = (farPlane / (farPlane - nearPlane)) * nearPlane * (-1)
	
	takes 3D point and converts to NDC where
	-1 <= x' <= 1
	-1 <= y' <= 1
	0 <= z' <= 1
	w = z
	*/

	if (aspectRatio < 1.0)
		projectionMatrix.m[0][0] = 1.0 / aspectRatio * tan(fieldOfView / 2.0);
	else
		projectionMatrix.m[0][0] = aspectRatio / tan(fieldOfView / 2.0);
	projectionMatrix.m[1][1] = 1.0 / tan(fieldOfView / 2.0);
	projectionMatrix.m[2][2] = farPlane / (farPlane - nearPlane);
	projectionMatrix.m[2][3] = 1.0;
	projectionMatrix.m[3][2] = -nearPlane * farPlane / (farPlane - nearPlane);
	projectionMatrix.m[3][3] = 0.0;
}

#endif