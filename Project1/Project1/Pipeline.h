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
	
	verticies[0].v /= verticies[0].v.w;
	verticies[1].v /= verticies[1].v.w;
	verticies[2].v /= verticies[2].v.w;
	
	for (int i = 0; i < 3; i++) {
		Vertex v = verticies[i];
		if (v.v.x <= -1.0 || v.v.x >= 1.0 || v.v.y <= -1.0 || v.v.y >= 1.0 || v.v.z <= 0 || v.v.z >= 1)
			return;

		verticies[i].v.x = ((v.v.x + 1.0) * 0.5 * cam->width);
		verticies[i].v.y = ((v.v.y + 1.0) * 0.5 * cam->height);
	}

	//if mid point is on left
	if (((verticies[1].v - verticies[0].v) ^
		(verticies[2].v - verticies[0].v)).z >= 0) {

	}
	else { //if mid point is on right

		//draw from top to middle
		double deltaX_TTB = (verticies[2].v.y - verticies[0].v.y) / (verticies[2].v.x - verticies[0].v.x);
		double deltaX_TTM = (verticies[1].v.y - verticies[0].v.y) / (verticies[1].v.x - verticies[0].v.x);

		//prestep
		
		//prestep to nearest scanline
		//x = ((diffY) * (run / rise)) + (xOrigin)
		double preStepY_left = verticies[0].v.y - ceil(verticies[0].v.y);
		double lineX_left = preStepY_left * deltaX_TTB + verticies[0].v.x;

		double preStepY_right = verticies[0].v.y - ceil(verticies[0].v.y);
		double lineX_right = preStepY_right * deltaX_TTM + verticies[0].v.x;

		for (int y = ceil(verticies[0].v.y); y > verticies[1].v.y; y--) {
			
			for (int x = ceil(lineX_left); x < ceil(lineX_right); x++) {
				char *pixelComponent = buffer + ((y * (int)cam->width + x) * 4);
				*(pixelComponent) = (unsigned char)255;
				*(pixelComponent + 1) = (unsigned char)255;
				*(pixelComponent + 2) = (unsigned char)255;
				*(pixelComponent + 3) = (unsigned char)0;
			}

			lineX_left -= deltaX_TTB;
			lineX_right -= deltaX_TTM;
		}
		
		//draw from middle to bottom
		double deltaX_MTB = (verticies[2].v.y - verticies[1].v.y) / (verticies[2].v.x - verticies[1].v.x);

		for (int y = ceil(verticies[1].v.y); y > verticies[2].v.y; y--) {

			for (int x = ceil(lineX_left); x < ceil(lineX_right); x++) {
				char *pixelComponent = buffer + ((y * (int)cam->width + x) * 4);
				*(pixelComponent) = (unsigned char)255;
				*(pixelComponent + 1) = (unsigned char)255;
				*(pixelComponent + 2) = (unsigned char)255;
				*(pixelComponent + 3) = (unsigned char)0;
			}

			lineX_left -= deltaX_TTB;
			lineX_right -= deltaX_MTB;
		}


	}
	
	/*
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
	*/
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