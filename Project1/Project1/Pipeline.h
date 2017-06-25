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

	//project from homogenous clip space to NDC space
	verticies[0].v /= verticies[0].v.w;
	verticies[1].v /= verticies[1].v.w;
	verticies[2].v /= verticies[2].v.w;

	//perform z-culling
	if (((verticies[1].v - verticies[0].v) ^
		(verticies[2].v - verticies[0].v)).z > 0) {
		return;
	}

	sortVerticies();

	//this clipping will take place in the clipVerticies() function
	for (int i = 0; i < 3; i++) {
		if (verticies[i].v.x <= -1.0 || verticies[i].v.x >= 1.0 || verticies[i].v.y <= -1.0 || verticies[i].v.y >= 1.0 || verticies[i].v.z <= 0 || verticies[i].v.z >= 1)
			return;
	}

	Vertex top = verticies[0];
	Vertex mid = verticies[1];
	Vertex bot = verticies[2];

	top.v.x = ((top.v.x + 1.0) * 0.5 * cam->width);
	top.v.y = ((top.v.y + 1.0) * 0.5 * cam->height);
	mid.v.x = ((mid.v.x + 1.0) * 0.5 * cam->width);
	mid.v.y = ((mid.v.y + 1.0) * 0.5 * cam->height);
	bot.v.x = ((bot.v.x + 1.0) * 0.5 * cam->width);
	bot.v.y = ((bot.v.y + 1.0) * 0.5 * cam->height);	

	//if mid point is on right
	//Cz = AxBy - AyBx
	if (((top.v.x - mid.v.x) * (top.v.y - bot.v.y)) - ((top.v.y - mid.v.y) * (top.v.x - bot.v.x)) < 0) {
		
		double topToMidXStep = (top.v.x - mid.v.x) / (top.v.y - mid.v.y);
		double topToBotXStep = (top.v.x - bot.v.x) / (top.v.y - bot.v.y);

		double leftX = top.v.x + ((floor(top.v.y) - top.v.y) * topToBotXStep);
		//double leftX = top.v.x;
		double rightX = top.v.x + ((floor(top.v.y) - top.v.y) * topToMidXStep);
		//double rightX = top.v.x;

		//draw scanlines from top.y to mid.y
		for (int y = floor(top.v.y); y > floor(mid.v.y); y--) {
			for (int x = floor(leftX); x <= floor(rightX); x++) {

				char *pixelComponent = buffer + ((y * (int)cam->width + x) * 4);
				*(pixelComponent) = (unsigned char)255;
				*(pixelComponent + 1) = (unsigned char)255;
				*(pixelComponent + 2) = (unsigned char)255;
				*(pixelComponent + 3) = (unsigned char)0;
			}
			
			leftX -= topToBotXStep;
			rightX -= topToMidXStep;

		}

		
		double midToBotXStep = (mid.v.x - bot.v.x) / (mid.v.y - bot.v.y);
		
		rightX = mid.v.x + ((floor(mid.v.y) - mid.v.y) * midToBotXStep);
		//rightX = mid.v.x;

		for (int y = floor(mid.v.y); y > floor(bot.v.y); y--) {
			for (int x = floor(leftX); x <= floor(rightX); x++) {

				char *pixelComponent = buffer + ((y * (int)cam->width + x) * 4);
				*(pixelComponent) = (unsigned char)255;
				*(pixelComponent + 1) = (unsigned char)255;
				*(pixelComponent + 2) = (unsigned char)255;
				*(pixelComponent + 3) = (unsigned char)0;
			}

			leftX -= topToBotXStep;
			rightX -= midToBotXStep;

		}
		
	}
	else {
		
		double topToMidXStep = (top.v.x - mid.v.x) / (top.v.y - mid.v.y);
		double topToBotXStep = (top.v.x - bot.v.x) / (top.v.y - bot.v.y);

		double leftX = top.v.x + ((floor(top.v.y) - top.v.y) * topToMidXStep);
		//double leftX = top.v.x;
		double rightX = top.v.x + ((floor(top.v.y) - top.v.y) * topToBotXStep);
		//double rightX = top.v.x;

		//draw scanlines from top.y to mid.y
		for (int y = floor(top.v.y); y > floor(mid.v.y); y--) {
			for (int x = floor(leftX); x <= floor(rightX); x++) {

				char *pixelComponent = buffer + ((y * (int)cam->width + x) * 4);
				*(pixelComponent) = (unsigned char)255;
				*(pixelComponent + 1) = (unsigned char)255;
				*(pixelComponent + 2) = (unsigned char)255;
				*(pixelComponent + 3) = (unsigned char)0;
			}

			leftX -= topToMidXStep;
			rightX -= topToBotXStep;

		}


		double midToBotXStep = (mid.v.x - bot.v.x) / (mid.v.y - bot.v.y);
		
		leftX = mid.v.x + ((floor(mid.v.y) - mid.v.y) * midToBotXStep);
		//leftX = mid.v.x;

		for (int y = floor(mid.v.y); y > floor(bot.v.y); y--) {
			for (int x = floor(leftX); x <= floor(rightX); x++) {

				char *pixelComponent = buffer + ((y * (int)cam->width + x) * 4);
				*(pixelComponent) = (unsigned char)255;
				*(pixelComponent + 1) = (unsigned char)255;
				*(pixelComponent + 2) = (unsigned char)255;
				*(pixelComponent + 3) = (unsigned char)0;
			}

			leftX -= midToBotXStep;
			rightX -= topToBotXStep;

		}
		
	}

	/*
	for (int i = 0; i < 3; i++) {
		int screenX = (int)((verticies[i].v.x + 1) * 0.5 * cam->width);
		int screenY = (int)((verticies[i].v.y + 1) * 0.5 * cam->height);

		char *pixelComponent = buffer + ((screenY * (int)cam->width + screenX) * 4);
		*(pixelComponent) = (unsigned char)255;
		*(pixelComponent + 1) = (unsigned char)0;
		*(pixelComponent + 2) = (unsigned char)0;
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