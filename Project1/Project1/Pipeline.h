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
	
	verticies[0].v /= verticies[0].v.w;
	verticies[1].v /= verticies[1].v.w;
	verticies[2].v /= verticies[2].v.w;

	for (int i = 0; i < 3; i++) {
		if (verticies[i].v.x <= -1.0 || verticies[i].v.x >= 1.0 || verticies[i].v.y <= -1.0 || verticies[i].v.y >= 1.0 || verticies[i].v.z <= 0 || verticies[i].v.z >= 1)
			return;
	}

	//if mid point is on left
	//Cz = AxBy - A

	for (int i = 0; i < 3; i++) {
		int screenX = (int)((verticies[i].v.x + 1) * 0.5 * cam->width);
		int screenY = (int)((verticies[i].v.y + 1) * 0.5 * cam->height);

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