#ifndef PIPELINE_H_
#define PIPELINE_H_

#include "Mesh.h"
#include "Camera.h"

class Pipeline {
public:

	char* buffer;
	Camera *cam;

	Vertex verticies[3]; //[0] = top, [1] = mid, [2] = bottom 
	Vector4D texCoords[3];
	Vector4D normals[3];

	Matrix4x4 modelMatrix;
	Matrix4x4 MVP;

	Pipeline();
	void setScreenBuffer(char *inputBuffer);
	void setCamera(Camera *inputCam);
	void transform(Mesh &inputMesh);
	inline void projectVerticies();
	inline void clipVerticies();
	inline void shadeTriangle();
	inline void sortVerticies();

private:





};

inline void Pipeline::setScreenBuffer(char *inputBuffer) {
	buffer = inputBuffer;
}

inline void Pipeline::setCamera(Camera *inputCam) {
	cam = inputCam;
}

inline void Pipeline::projectVerticies() {
	verticies[0].v *= MVP;
	verticies[1].v *= MVP;
	verticies[2].v *= MVP;
}

inline void Pipeline::clipVerticies() {
	//check if entire triangle lies outside viewing frustum
	//clip in homogenous clip space
}

inline void Pipeline::shadeTriangle() {
	sortVerticies();

	//create a class that manages draw from left to right, scanline by scanline

	/*
	find longest edge and determine whether on left or right
		//longest = (top - bottom)

	other two egdes are on the opposite side of longest
	
	gradients for stepping along edge are calculated from start of edge to end of edge
	
	draw from top vertex to middle vertex
	draw from middle vertex to bottom vertex
	*/

	for (int i = 0; i < 3; i++) {
		Vertex v = Vertex(verticies[i].v);
			v.v /= v.v.w;

		if (v.v.x <= -1.0 || v.v.x >= 1.0 || v.v.y <= -1.0 || v.v.y >= 1.0 || v.v.z <= 0 || v.v.z >= 1)
			continue;

		int screenX = (int)((v.v.x + 1) * 0.5 * cam->width);
		int screenY = (int)((v.v.y + 1) * 0.5 * cam->height);

		char *pixelComponent = buffer + ((screenY * (int)cam->width + screenX) * 4);
		*(pixelComponent) = (unsigned char)255;
		*(pixelComponent + 1) = (unsigned char)255;
		*(pixelComponent + 2) = (unsigned char)255;
		*(pixelComponent + 3) = (unsigned char)0;
	}
}

inline void Pipeline::sortVerticies() {
	if (verticies[0].v.y < verticies[2].v.y) {
		Vertex temp = verticies[0];
		verticies[0] = verticies[2];
		verticies[2] = temp;
	}
	
	if (verticies[0].v.y < verticies[1].v.y) {
		Vertex temp = verticies[0];
		verticies[0] = verticies[1];
		verticies[1] = temp;
	}

	if (verticies[1].v.y < verticies[2].v.y) {
		Vertex temp = verticies[1];
		verticies[1] = verticies[2];
		verticies[2] = temp;
	}
}

#endif