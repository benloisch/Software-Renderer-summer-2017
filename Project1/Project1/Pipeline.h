#ifndef PIPELINE_H_
#define PIPELINE_H_

#include <vector>

#include "Mesh.h"
#include "Camera.h"

class Pipeline {
public:

	char* buffer;
	Camera *cam;

	//Vertex verticies[3]; //[0] = top, [1] = mid, [2] = bottom 
	//Vector4D texCoords[3];
	//Vector4D normals[3];

	Mesh *mesh;

	Pipeline();
	void setScreenBuffer(char *inputBuffer);
	void setCamera(Camera *inputCam);
	void transform(Mesh *inputMesh);
	inline void projectVerticies(Vertex verticies[3], Matrix4x4 &MVP);
	inline void clipVerticies(Vertex verticies[3]);
	inline void shadeTriangle(Vertex verticies[3], Vector4D texCoords[3], Vector4D normals[3]);
	inline void sortVerticies(Vertex verticies[3]);

	inline double gradientDcDx(double &c1, double &c2, double &c3, Vertex verticies[3]);
	inline double gradientDcDy(double &c1, double &c2, double &c3, Vertex verticies[3]);

private:





};

inline void Pipeline::setScreenBuffer(char *inputBuffer) {
	buffer = inputBuffer;
}

inline void Pipeline::setCamera(Camera *inputCam) {
	cam = inputCam;
}

inline void Pipeline::projectVerticies(Vertex verticies[3], Matrix4x4 &MVP) {
	verticies[0].v *= MVP;
	verticies[1].v *= MVP;
	verticies[2].v *= MVP;
}

inline void Pipeline::clipVerticies(Vertex verticies[3]) {
	//check if entire triangle lies outside viewing frustum
	//clip in homogenous clip space
}

inline void Pipeline::shadeTriangle(Vertex verticies[3], Vector4D texCoords[3], Vector4D normals[3]) {

	//project from homogenous clip space to NDC space
	verticies[0].v /= verticies[0].v.w;
	verticies[1].v /= verticies[1].v.w;
	verticies[2].v /= verticies[2].v.w;

	//perform z-culling
	if (((verticies[1].v - verticies[0].v) ^
		(verticies[2].v - verticies[0].v)).z > 0) {
		//return;
	}

	sortVerticies(verticies);

	//this clipping will take place in the clipVerticies() function
	for (int i = 0; i < 3; i++) {
		if (verticies[i].v.x <= -1.0 || verticies[i].v.x >= 1.0 || verticies[i].v.y <= -1.0 || verticies[i].v.y >= 1.0 || verticies[i].v.z <= 0 || verticies[i].v.z >= 1)
			return;
	}

	for (int i = 0; i < 3; i++) {
		verticies[i].v.x = ((verticies[i].v.x + 1.0) * 0.5 * cam->width);
		verticies[i].v.y = ((verticies[i].v.y + 1.0) * 0.5 * cam->height);
	}

	Vertex top = verticies[0];
	Vertex mid = verticies[1];
	Vertex bot = verticies[2];

	//top.v.x = ((top.v.x + 1.0) * 0.5 * cam->width);
	//top.v.y = ((top.v.y + 1.0) * 0.5 * cam->height);
	//mid.v.x = ((mid.v.x + 1.0) * 0.5 * cam->width);
	//mid.v.y = ((mid.v.y + 1.0) * 0.5 * cam->height);
	//bot.v.x = ((bot.v.x + 1.0) * 0.5 * cam->width);
	//bot.v.y = ((bot.v.y + 1.0) * 0.5 * cam->height);		

	//in full 1 unit steps
	double textureXXStep = gradientDcDx(top.t.x, mid.t.x, bot.t.x, verticies);
	double textureXYStep = gradientDcDy(top.t.x, mid.t.x, bot.t.x, verticies);
	double textureYXStep = gradientDcDx(top.t.y, mid.t.y, bot.t.y, verticies);
	double textureYYStep = gradientDcDy(top.t.y, mid.t.y, bot.t.y, verticies);

	//if mid point is on right
	//Cz = AxBy - AyBx
	if (((top.v.x - mid.v.x) * (top.v.y - bot.v.y)) - ((top.v.y - mid.v.y) * (top.v.x - bot.v.x)) < 0) {
		
		double topToMidXStep = (top.v.x - mid.v.x) / (top.v.y - mid.v.y);
		double topToBotXStep = (top.v.x - bot.v.x) / (top.v.y - bot.v.y);

		double yPreStep = floor(top.v.y) - top.v.y;
		double xPreStep = yPreStep * topToBotXStep;

		double leftX = top.v.x + (yPreStep * topToBotXStep);
		double rightX = top.v.x + (yPreStep * topToMidXStep);
		
		//x texture coordinate
		double texCoordStartXLeft = top.t.x + (textureXXStep * xPreStep) + (textureXYStep * yPreStep);
		double texCoordXNextLineLeft = textureXYStep + (textureXXStep * topToBotXStep);
		//y texture coordinate
		double texCoordStartYLeft = top.t.y + (textureYXStep * xPreStep) + (textureYYStep * yPreStep);
		double texCoordYNextLineLeft = textureYYStep + (textureYXStep * topToBotXStep);

		xPreStep = yPreStep * topToMidXStep;
		//x texture coordinate
		double texCoordStartXRight = top.t.x + (textureXXStep * xPreStep) + (textureXYStep * yPreStep);
		double texCoordXNextLineRight = textureXYStep + (textureXXStep * topToMidXStep);
		//y texture coordinate
		double texCoordStartYRight = top.t.y + (textureYXStep * xPreStep) + (textureYYStep * yPreStep);
		double texCoordYNextLineRight = textureYYStep + (textureYXStep * topToMidXStep);
		
		//draw scanlines from top.y to mid.y
		for (int y = (int)ceil(top.v.y); y > (int)ceil(mid.v.y); y--) {
			
			double xPreStep = ceil(leftX) - leftX;
			double xdist = rightX - leftX;
			double XXStep = (texCoordStartXRight - texCoordStartXLeft) / xdist;
			double YXStep = (texCoordStartYRight - texCoordStartYLeft) / xdist;
			
			double textureCoordX = texCoordStartXLeft +(XXStep * xPreStep);
			double textureCoordY = texCoordStartYLeft +(YXStep * xPreStep);
			

			for (int x = (int)ceil(leftX); x < (int)ceil(rightX); x++) {

				Color_RGB color(mesh->texture->getPixelColor((int)(textureCoordX*(mesh->texture->width - 1) + 0.5), (int)(textureCoordY*(mesh->texture->height - 1) + 0.5)));

				char *pixelComponent = buffer + ((y * (int)cam->width + x) * 4);
				*(pixelComponent) = (unsigned char)color.b;
				*(pixelComponent + 1) = (unsigned char)color.g;
				*(pixelComponent + 2) = (unsigned char)color.r;
				*(pixelComponent + 3) = (unsigned char)0;

				textureCoordX += XXStep;
				textureCoordY += YXStep;
				//textureXCoord += textureXXStep;
				//textureYCoord += textureYXStep;
			}
			
			leftX -= topToBotXStep;
			rightX -= topToMidXStep;
			
			texCoordStartXLeft -= texCoordXNextLineLeft;
			texCoordStartYLeft -= texCoordYNextLineLeft;
			texCoordStartXRight -= texCoordXNextLineRight;
			texCoordStartYRight -= texCoordYNextLineRight;
			
		}

		double midToBotXStep = (mid.v.x - bot.v.x) / (mid.v.y - bot.v.y);
		
		yPreStep = floor(mid.v.y) - mid.v.y;
		xPreStep = yPreStep * midToBotXStep;

		rightX = mid.v.x + (yPreStep * midToBotXStep);

		xPreStep = yPreStep * midToBotXStep;
		//x texture coordinate
		texCoordStartXRight = mid.t.x + (textureXXStep * xPreStep) + (textureXYStep * yPreStep);
		texCoordXNextLineRight = textureXYStep + (textureXXStep * midToBotXStep);
		//y texture coordinate
		texCoordStartYRight = mid.t.y + (textureYXStep * xPreStep) + (textureYYStep * yPreStep);
		texCoordYNextLineRight = textureYYStep + (textureYXStep * midToBotXStep);

		for (int y = (int)ceil(mid.v.y); y > (int)ceil(bot.v.y); y--) {

			double xPreStep = ceil(leftX) - leftX;
			double xdist = rightX - leftX;
			double XXStep = (texCoordStartXRight - texCoordStartXLeft) / xdist;
			double YXStep = (texCoordStartYRight - texCoordStartYLeft) / xdist;

			double textureCoordX = texCoordStartXLeft + (XXStep * xPreStep);
			double textureCoordY = texCoordStartYLeft + (YXStep * xPreStep);

			for (int x = (int)ceil(leftX); x < (int)ceil(rightX); x++) {

				Color_RGB color(mesh->texture->getPixelColor((int)(textureCoordX*(mesh->texture->width - 1) + 0.5), (int)(textureCoordY*(mesh->texture->height - 1) + 0.5)));

				char *pixelComponent = buffer + ((y * (int)cam->width + x) * 4);
				*(pixelComponent) = (unsigned char)color.b;
				*(pixelComponent + 1) = (unsigned char)color.g;
				*(pixelComponent + 2) = (unsigned char)color.r;
				*(pixelComponent + 3) = (unsigned char)0;

				textureCoordX += XXStep;
				textureCoordY += YXStep;
				//textureXCoord += textureXXStep;
				//textureYCoord += textureYXStep;
			}

			leftX -= topToBotXStep;
			rightX -= midToBotXStep;

			texCoordStartXLeft -= texCoordXNextLineLeft;
			texCoordStartYLeft -= texCoordYNextLineLeft;
			texCoordStartXRight -= texCoordXNextLineRight;
			texCoordStartYRight -= texCoordYNextLineRight;
		}
		
	}
	else {
		
		double topToMidXStep = (top.v.x - mid.v.x) / (top.v.y - mid.v.y);
		double topToBotXStep = (top.v.x - bot.v.x) / (top.v.y - bot.v.y);

		double yPreStep = floor(top.v.y) - top.v.y;
		double xPreStep = floor(top.v.x) - top.v.x;

		double leftX = top.v.x + (yPreStep * topToMidXStep);
		double rightX = top.v.x + (yPreStep * topToBotXStep);

		//draw scanlines from top.y to mid.y
		for (int y = (int)floor(top.v.y); y > floor(mid.v.y); y--) {
			for (int x =(int) floor(leftX); x <= floor(rightX); x++) {

				Color_RGB color(255, 255, 255);

				char *pixelComponent = buffer + ((y * (int)cam->width + x) * 4);
				*(pixelComponent) = (unsigned char)color.b;
				*(pixelComponent + 1) = (unsigned char)color.g;
				*(pixelComponent + 2) = (unsigned char)color.r;
				*(pixelComponent + 3) = (unsigned char)0;
			}

			leftX -= topToMidXStep;
			rightX -= topToBotXStep;

		}


		double midToBotXStep = (mid.v.x - bot.v.x) / (mid.v.y - bot.v.y);
		
		yPreStep = floor(mid.v.y) - mid.v.y;
		xPreStep = floor(mid.v.x) - mid.v.x;

		leftX = mid.v.x + (yPreStep * midToBotXStep);

		for (int y = (int)floor(mid.v.y); y > floor(bot.v.y); y--) {
			for (int x = (int)floor(leftX); x <= floor(rightX); x++) {

				Color_RGB color(50, 50, 50);

				char *pixelComponent = buffer + ((y * (int)cam->width + x) * 4);
				*(pixelComponent) = (unsigned char)color.b;
				*(pixelComponent + 1) = (unsigned char)color.g;
				*(pixelComponent + 2) = (unsigned char)color.r;
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

inline double Pipeline::gradientDcDx(double &c0, double &c1, double &c2, Vertex verticies[3]) {
	return (((c1 - c2)*(verticies[0].v.y - verticies[2].v.y)) - ((c0 - c2)*(verticies[1].v.y - verticies[2].v.y)))/
		(((verticies[1].v.x - verticies[2].v.x)*(verticies[0].v.y - verticies[2].v.y)) - ((verticies[0].v.x - verticies[2].v.x)*(verticies[1].v.y - verticies[2].v.y)));
}

inline double Pipeline::gradientDcDy(double &c0, double &c1, double &c2, Vertex verticies[3]) {
	return (((c1 - c2)*(verticies[0].v.x - verticies[2].v.x)) - ((c0 - c2)*(verticies[1].v.x - verticies[2].v.x))) /
		(((verticies[0].v.x - verticies[2].v.x)*(verticies[1].v.y - verticies[2].v.y)) - ((verticies[1].v.x - verticies[2].v.x)*(verticies[0].v.y - verticies[2].v.y)));
}

inline void Pipeline::sortVerticies(Vertex verticies[3]) {
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