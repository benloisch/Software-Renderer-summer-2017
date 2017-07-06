#ifndef CAMERA_H_
#define CAMERA_H_

#include <math.h>
#include <Windows.h>

#include "Matrix4x4.h"
#include "Vector4D.h"

class Camera {
public:

	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Vector4D originPosition;
	Vector4D lookDirection;

	float nearPlane;
	float farPlane;
	float aspectRatio;
	float fieldOfView;

	int width;
	int height;

	POINT sMouse;
	float movX, movZ;

	Camera();

	void setOriginPosition(float, float, float);
	void setLookDirection(float, float, float);
	void setNearPlane(float);
	void setFarPlane(float);
	void setAspectRatio(int, int);
	void setFieldOfView(int);

	void calculateViewMatrix();
	void calculateProjectionMatrix();
	void updateCameraOrientation(float, float);

	void getInput(float &delta);

private:


};

inline void Camera::calculateViewMatrix() {	
	//Cross Product
	//Cx = AyBz - AzBy
	//Cy = AzBx - AxBz
	//Cz = AxBy - AyBx

	//calculate x basis vector by taking cross product of world 'up' (0, 1, 0) vector and lookDirection vector
	Vector4D up(0.0f, 1.0f, 0.0f, 0.0f);
	Vector4D xBasis = up ^ lookDirection;

	//calculate y basis vector by taking cross product of lookDirection and x basis vector
	Vector4D yBasis = lookDirection ^ xBasis;

	//the lookDirection is already the z basis vector
	Vector4D zBasis = lookDirection;

	xBasis.normalize();
	yBasis.normalize();
	zBasis.normalize();

	viewMatrix.setRow1(xBasis.x, xBasis.y, xBasis.z, 0.0f);
	viewMatrix.setRow2(yBasis.x, yBasis.y, yBasis.z, 0.0f);
	viewMatrix.setRow3(zBasis.x, zBasis.y, zBasis.z, 0.0f);

	Vector4D trans;
	trans += (xBasis * movX);
	trans += (zBasis * movZ);
	movX = movZ = 0.0;

	viewMatrix.m[3][0] += trans.x;
	viewMatrix.m[3][1] += trans.y; 
	viewMatrix.m[3][2] += trans.z;
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

	float oneOverAspect = 1.0f / aspectRatio;
	if (aspectRatio < 1.0f)
		projectionMatrix.m[0][0] = 1.0f / aspectRatio * tan(fieldOfView / 2.0f);
	else
	projectionMatrix.m[0][0] = 1.0f / oneOverAspect * tan(fieldOfView / 2.0f);
	projectionMatrix.m[1][1] = 1.0f / tan(fieldOfView / 2.0f);
	projectionMatrix.m[2][2] = farPlane / (farPlane - nearPlane);
	projectionMatrix.m[2][3] = 1.0f;
	projectionMatrix.m[3][2] = -nearPlane * farPlane / (farPlane - nearPlane);
	projectionMatrix.m[3][3] = 0.0f;
}

inline void Camera::getInput(float &delta)
{
	Matrix4x4 rot;
	sMouse;
	POINT mouseNow;
	GetCursorPos(&mouseNow);
	float movementspeed = 100.0f;
	float rotationspeed = 10.0f;
	if (GetAsyncKeyState(0x57)) //W
	{
		movZ = movementspeed * delta;
	}
	else if (GetAsyncKeyState(0x53)) //S
	{
		movZ = -movementspeed * delta;
	}

	if (GetAsyncKeyState(0x41)) //A
	{
		movX = -movementspeed * delta;
	}
	else if (GetAsyncKeyState(0x44)) //D
	{
		movX = movementspeed * delta;
	}

	if (mouseNow.x - sMouse.x > 0)
	{
		float diff = float(mouseNow.x - sMouse.x);
		diff *= rotationspeed * delta;
		rot.setYrot(diff);
		lookDirection *= rot;
		lookDirection.normalize();
	}
	else if (mouseNow.x - sMouse.x < 0)
	{
		float diff = float(mouseNow.x - sMouse.x);
		diff *= rotationspeed * delta;
		rot.setYrot(diff);
		lookDirection *= rot;
		lookDirection.normalize();
	}

	if (mouseNow.y - sMouse.y > 0)
	{
		if (lookDirection.y > -0.9) {
			float diff = float(mouseNow.y - sMouse.y);
			diff *= rotationspeed * delta;
			rot.setRotArb(viewMatrix.m[0][0], viewMatrix.m[0][1], viewMatrix.m[0][2], diff); //rotate forward vector about x axis
			lookDirection *= rot;
			lookDirection.normalize();
		}
	}
	else if (mouseNow.y - sMouse.y < 0)
	{
		if (lookDirection.y < 0.9) {
			float diff = float(mouseNow.y - sMouse.y);
			diff *= rotationspeed * delta;
			rot.setRotArb(viewMatrix.m[0][0], viewMatrix.m[0][1], viewMatrix.m[0][2], diff); //rotate forward vector about x axis
			lookDirection *= rot;
			lookDirection.normalize();
		}
	}
	sMouse = mouseNow;
	SetCursorPos((int)(width / 2), (int)(height / 2));
	GetCursorPos(&sMouse);
}

#endif