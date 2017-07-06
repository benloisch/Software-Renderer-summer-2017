#include "Camera.h"

Camera::Camera() {
	originPosition = Vector4D(0.0f, 0.0f, 0.0f, 0.0f);
	lookDirection = Vector4D(0.0f, 0.0f, 0.0f, 0.0f);

	nearPlane = farPlane = aspectRatio = fieldOfView = 0.0;
	movX = movZ = 0.0;
}

void Camera::setOriginPosition(float ox, float oy, float oz) {
	originPosition = Vector4D(ox, oy, oz, 0.0);
	viewMatrix.setRow4(originPosition.x, originPosition.y, originPosition.z, 1.0);
}

void Camera::setLookDirection(float lx, float ly, float lz) {
	lookDirection = Vector4D(lx, ly, lz, 0.0);
}

void Camera::setNearPlane(float np) {
	nearPlane = np;
}

void Camera::setFarPlane(float fp) {
	farPlane = fp;
}

void Camera::setAspectRatio(int width, int height) {
	aspectRatio = (float)width / (float)height;
	this->width = width;
	this->height = height;
}

void Camera::setFieldOfView(int fov) {
	//convert from degrees to radians
	fieldOfView = (3.14159265359f / 180.0f) * fov;
}