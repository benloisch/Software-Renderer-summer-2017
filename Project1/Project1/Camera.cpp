#include "Camera.h"

Camera::Camera() {
	originPosition = Vector4D(0.0, 0.0, 0.0, 0.0);
	lookDirection = Vector4D(0.0, 0.0, 0.0, 0.0);

	nearPlane = farPlane = aspectRatio = fieldOfView = 0.0;
	movX = movZ = 0.0;
}

void Camera::setOriginPosition(double ox, double oy, double oz) {
	originPosition = Vector4D(ox, oy, oz, 0.0);
	viewMatrix.setRow4(originPosition.x, originPosition.y, originPosition.z, 1.0);
}

void Camera::setLookDirection(double lx, double ly, double lz) {
	lookDirection = Vector4D(lx, ly, lz, 0.0);
}

void Camera::setNearPlane(double np) {
	nearPlane = np;
}

void Camera::setFarPlane(double fp) {
	farPlane = fp;
}

void Camera::setAspectRatio(double width, double height) {
	aspectRatio = width / height;
	this->width = width;
	this->height = height;
}

void Camera::setFieldOfView(double fov) {
	//convert from degrees to radians
	fieldOfView = (3.14159265359 / 180.0) * fov;
}