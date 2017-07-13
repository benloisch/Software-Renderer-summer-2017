#ifndef PIPELINE_H_
#define PIPELINE_H_

#include <vector>

#include "Mesh.h"
#include "Camera.h"

class Pipeline {
public:

	MSG *msg;

	float* depthBuffer;

	char* buffer;
	Camera *cam;

	//Vertex verticies[3]; //[0] = top, [1] = mid, [2] = bottom 
	//Vector4D texCoords[3];
	//Vector4D normals[3];

	Mesh *mesh;

	Pipeline();
	void setScreenBuffer(char *inputBuffer);
	void setCamera(Camera *inputCam);
	inline void clearDepthBuffer();

	void transform(Mesh *inputMesh);
	inline void projectVerticies(Vertex verticies[3], Matrix4x4 &MVP);
	inline vector<Vertex> clipVerticies(Vertex verticies[]);
	inline void shadeTriangle(Vertex top, Vertex mid, Vertex bot);
	inline void sortVerticies(Vertex &top, Vertex &mid, Vertex &bot);
	inline void scanline(int ystart, int yend);

	inline float gradientDcDx(float c0, float c1, float c2, Vertex &top, Vertex &mid, Vertex &bot);
	inline float gradientDcDy(float c0, float c1, float c2, Vertex &top, Vertex &mid, Vertex &bot);

	//variables used for shading triangle
	float righthandslope;
	float lefthandslope;
	float textureXXStep;
	float textureXYStep;
	float textureYXStep;
	float textureYYStep;
	float inverseZXStep;
	float inverseZYStep;
	float yPreStep;
	float xPreStep;
	float leftX;
	float rightX;
	float inverseZStartLeft;
	float inverseZNextLineLeft;
	float texCoordStartXLeft;
	float texCoordXNextLineLeft;
	float texCoordStartYLeft;
	float texCoordYNextLineLeft;
	float inverseZStartRight;
	float inverseZNextLineRight;
	float texCoordStartXRight;
	float texCoordXNextLineRight;
	float texCoordStartYRight;
	float texCoordYNextLineRight;
private:





};

inline void Pipeline::clearDepthBuffer() {
	for (int i = 0; i < cam->width*cam->height; i++) {
		depthBuffer[i] = 0.0f;
	}
}

inline void Pipeline::setScreenBuffer(char *inputBuffer) {
	buffer = inputBuffer;
}

inline void Pipeline::setCamera(Camera *inputCam) {
	cam = inputCam;
	depthBuffer = new float[(int)((cam->width)*(cam->height))];
}

inline void Pipeline::projectVerticies(Vertex verticies[3], Matrix4x4 &MVP) {
	verticies[0].v *= MVP;
	verticies[1].v *= MVP;
	verticies[2].v *= MVP;
}

inline vector<Vertex> Pipeline::clipVerticies(Vertex verticies[3]) {
	//check if entire triangle lies outside viewing frustum
	//clip in homogenous clip space
	vector<Vertex> output;

	//clip x, y, and z against -w and w
	//t = (currentW - currentP)  / *(currentW - currentP) - (nextW - nextP))

	Vertex current = verticies[2];
	Vertex next = verticies[0];

	for (unsigned int i = 0; i < 3; i++) {

		if (current.v.x < current.v.w) //if current vertex is inside, add to list
			output.push_back(current);
		//check if one is inside and one is outside and if so, clip and add to copy
		if ((current.v.x < current.v.w && next.v.x > next.v.w) || (current.v.x > current.v.w && next.v.x < next.v.w)) {
			float t = (current.v.w - current.v.x) / ((current.v.w - current.v.x) - (next.v.w - next.v.x));
			Vertex v = current;
			v.v = v.v.lerp(next.v, t);
			v.t = v.t.lerp(next.t, t);
			v.v.x = v.v.w;
			//v.v.x -= 0.0001f;
			output.push_back(v);
		}

		if (i != 2) {
			current = verticies[i];
			next = verticies[i + 1];
		}
	}

	vector<Vertex> input;
	if (output.size() != 0)
		input = output;
	else
		return output;

	output.clear();

	current = input[input.size() - 1];
	next = input[0];

	for (unsigned int i = 0; i < input.size(); i++) {

		if (-current.v.x < current.v.w) //if current vertex is inside, add to list
			output.push_back(current);
		//check if one is inside and one is outside and if so, clip and add to copy
		if ((-current.v.x < current.v.w && -next.v.x > next.v.w) || (-current.v.x > current.v.w && -next.v.x < next.v.w)) {
			float t = (-current.v.w - current.v.x) / ((-current.v.w - current.v.x) - (-next.v.w - next.v.x));
			Vertex v = current;
			v.v = v.v.lerp(next.v, t);
			v.t = v.t.lerp(next.t, t);
			v.v.x = -v.v.w;
			//v.v.x += 0.0001f;
			output.push_back(v);
		}

		if (i != input.size() - 1){
			current = input[i];
			next = input[i + 1];
		}
	}
	
	if (output.size() != 0)
		input = output;
	else
		return output;

	output.clear();

	current = input[input.size() - 1];
	next = input[0];

	for (unsigned int i = 0; i < input.size(); i++) {

		if (current.v.y < current.v.w) //if current vertex is inside, add to list
			output.push_back(current);
		//check if one is inside and one is outside and if so, clip and add to copy
		if ((current.v.y < current.v.w && next.v.y > next.v.w) || (current.v.y > current.v.w && next.v.y < next.v.w)) {
			float t = (current.v.w - current.v.y) / ((current.v.w - current.v.y) - (next.v.w - next.v.y));
			Vertex v = current;
			v.v = v.v.lerp(next.v, t);
			v.t = v.t.lerp(next.t, t);
			v.v.y = v.v.w;
			//v.v.y -= 0.0001f;
			output.push_back(v);
		}

		if (i != input.size() - 1){
			current = input[i];
			next = input[i + 1];
		}
	}
	
	if (output.size() != 0)
		input = output;
	else
		return output;

	output.clear();

	current = input[input.size() - 1];
	next = input[0];

	for (unsigned int i = 0; i < input.size(); i++) {

		if (-current.v.y < current.v.w) //if current vertex is inside, add to list
			output.push_back(current);
		//check if one is inside and one is outside and if so, clip and add to copy
		if ((-current.v.y < current.v.w && -next.v.y > next.v.w) || (-current.v.y > current.v.w && -next.v.y < next.v.w)) {
			float t = (-current.v.w - current.v.y) / ((-current.v.w - current.v.y) - (-next.v.w - next.v.y));
			Vertex v = current;
			v.v = v.v.lerp(next.v, t);
			v.t = v.t.lerp(next.t, t);
			v.v.y = -v.v.w;
			//v.v.y += 0.0001f;
			output.push_back(v);
		}

		if (i != input.size() - 1){
			current = input[i];
			next = input[i + 1];
		}
	}
	/*
	if (output.size() != 0)
		input = output;
	else
		return output;

	output.clear();

	current = input[input.size() - 1];
	next = input[0];

	for (unsigned int i = 0; i < input.size(); i++) {

		if (current.v.z < current.v.w) //if current vertex is inside, add to list
			output.push_back(current);
		//check if one is inside and one is outside and if so, clip and add to copy
		if ((current.v.z < current.v.w && next.v.z > next.v.w) || (current.v.z > current.v.w && next.v.z < next.v.w)) {
			float t = (current.v.w - current.v.z) / ((current.v.w - current.v.z) - (next.v.w - next.v.z));
			Vertex v = current;
			v.v = v.v.lerp(next.v, t);
			v.t = v.t.lerp(next.t, t);
			v.v.z = v.v.w;
			//v.v.z -= 0.0001f;
			output.push_back(v);
		}

		if (i != input.size() - 1){
			current = input[i];
			next = input[i + 1];
		}
	}

	if (output.size() != 0)
		input = output;
	else
		return output;

	output.clear();

	current = input[input.size() - 1];
	next = input[0];

	for (unsigned int i = 0; i < input.size(); i++) {

		if (current.v.z > 0) //if current vertex is inside, add to list
			output.push_back(current);
		//check if one is inside and one is outside and if so, clip and add to copy
		if ((current.v.z > 0 && next.v.z < 0) || (current.v.z < 0 && next.v.z > 0)) {
			float t = (0 - current.v.z) / ((0 - current.v.z) - (0 - next.v.z));
			Vertex v = current;
			v.v = v.v.lerp(next.v, t);
			v.t = v.t.lerp(next.t, t);
			v.v.z = 0.0f;
			//v.v.z += 0.0001f;
			output.push_back(v);
		}

		if (i != input.size() - 1){
			current = input[i];
			next = input[i + 1];
		}
	}
	*/
	return output;
}

inline void Pipeline::scanline(int ystart, int yend) {
	for (int y = ystart - 1; y >= yend; y--) {

		float xPreStep = int(leftX + 1.0) - leftX;
		float xdist = rightX - leftX;
		float XXStep = (texCoordStartXRight - texCoordStartXLeft) / xdist;
		float YXStep = (texCoordStartYRight - texCoordStartYLeft) / xdist;
		float ZXStep = (inverseZStartRight - inverseZStartLeft) / xdist;
		//float ZXDepthStep = (depthZStartRight - depthZStartLeft) / xdist;

		float textureCoordX = texCoordStartXLeft + (XXStep * xPreStep);
		float textureCoordY = texCoordStartYLeft + (YXStep * xPreStep);
		float inverseZ = inverseZStartLeft + (ZXStep * xPreStep);
		//float depthZ = depthZStartLeft + (ZXDepthStep * xPreStep);
		float *pdepth = depthBuffer + (y * cam->width + (int)(leftX));
		unsigned int* ibuffer = (unsigned int*)(buffer + ((y * cam->width + (int)(leftX)) * 4));
		for (int x = (int)(leftX); x < (int)(rightX); x++) {

			//double z = 1.0 / inverseZ;
			//double texX = textureCoordX*z;
			//double texY = textureCoordY*z;

			//if (texX > 1 || texY > 1 || texX < 0 || texY < 0) {
			//	int stop = 0;
			//}

			//Color_RGB color(mesh->texture->getPixelColor((int)((textureCoordX*z)*(mesh->texture->width - 1) + 0.5), (int)((textureCoordY*z)*(mesh->texture->height - 1) + 0.5)));

			/*
			if (z < depthBuffer[y * cam->width + x]) {
			char *pixelComponent = buffer + ((y * (int)cam->width + x) * 4);
			*(pixelComponent) = (unsigned char)(mesh->texture->blue[(int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)][(int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5)]);
			*(pixelComponent + 1) = (unsigned char)(mesh->texture->green[(int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)][(int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5)]);
			*(pixelComponent + 2) = (unsigned char)(mesh->texture->red[(int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)][(int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5)]);
			//*(pixelComponent + 3) = (unsigned char)0;

			depthBuffer[y * cam->width + x] = z;
			}
			*/

			if (inverseZ > *pdepth) {
				//unsigned char red = (unsigned char)(mesh->texture->red[(int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)][(int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5)]);
				//unsigned char green = (unsigned char)(mesh->texture->green[(int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)][(int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5)]);
				//unsigned char blue = (unsigned char)(mesh->texture->blue[(int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)][(int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5)]);

				//unsigned char r = (unsigned char)(mesh->texture->intbuffer[((int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)) * ((int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5))] >> 8);
				//unsigned char g = (unsigned char)(mesh->texture->intbuffer[((int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)) * ((int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5))] >> 16);
				//unsigned char b = (unsigned char)(mesh->texture->intbuffer[((int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)) * ((int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5))] >> 24);
				*ibuffer = mesh->texture->intbuffer[((int)((textureCoordX / inverseZ)*(mesh->texture->width - 1) + 0.5)) + mesh->texture->width * ((int)((textureCoordY / inverseZ)*(mesh->texture->height - 1) + 0.5))];

				//*ibuffer = mesh->texture->intbuffer[((int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)) + mesh->texture->width * ((int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5))];
				*pdepth = inverseZ;
			}
			ibuffer++;
			pdepth++;
			textureCoordX += XXStep;
			textureCoordY += YXStep;
			inverseZ += ZXStep;
			//depthZ += ZXDepthStep;
		}

		leftX -= lefthandslope;
		rightX -= righthandslope;

		texCoordStartXLeft -= texCoordXNextLineLeft;
		texCoordStartYLeft -= texCoordYNextLineLeft;
		texCoordStartXRight -= texCoordXNextLineRight;
		texCoordStartYRight -= texCoordYNextLineRight;
		inverseZStartLeft -= inverseZNextLineLeft;
		inverseZStartRight -= inverseZNextLineRight;
		//depthZStartLeft -= depthZNextLineLeft;
		//depthZStartRight -= depthZNextLineRight;
	}
}

inline void Pipeline::shadeTriangle(Vertex top, Vertex mid, Vertex bot) {
	//check msg again to see if we pressed the escape key
	/*
	if (PeekMessage(msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(msg);
		DispatchMessage(msg);
	}
	if (msg->hwnd == NULL) //cancel all rendering if we pressed esc key
		return;
		*/

	//project from homogenous clip space to NDC space
	top.v /= top.v.w;
	mid.v /= mid.v.w;
	bot.v /= bot.v.w;

	//perform z-culling
	if (((mid.v - top.v) ^
		(bot.v - top.v)).z > 0) {
		//return;
	}

	sortVerticies(top, mid, bot);

	if (1.0 - abs(top.v.x) < 0.00001) {
		if (top.v.x < 0)
			top.v.x = -1.0;
		else
			top.v.x = 1.0;
	}

	if (1.0 - abs(mid.v.x) < 0.00001) {
		if (mid.v.x < 0)
			mid.v.x = -1.0;
		else
			mid.v.x = 1.0;
	}

	if (1.0 - abs(bot.v.x) < 0.001) {
		if (bot.v.x < 0)
			bot.v.x = -1.0;
		else
			bot.v.x = 1.0;
	}

	//this clipping will take place in the clipVerticies() function
	if (top.v.x < -1.0 || top.v.x > 1.0 || top.v.y < -1.0 || top.v.y > 1.0 || top.v.z <= 0.0 || top.v.z > 1.0)
		return;
	if (mid.v.x < -1.0 || mid.v.x > 1.0 || mid.v.y < -1.0 || mid.v.y > 1.0 || mid.v.z <= 0.0 || mid.v.z > 1.0)
		return;
	if (bot.v.x < -1.0 || bot.v.x > 1.0 || bot.v.y < -1.0 || bot.v.y > 1.0 || bot.v.z <= 0.0 || bot.v.z > 1.0)
		return;

	top.v.x = ((top.v.x + 1.0f) * 0.5f * cam->width);
	top.v.y = ((top.v.y + 1.0f) * 0.5f * cam->height);
	mid.v.x = ((mid.v.x + 1.0f) * 0.5f * cam->width);
	mid.v.y = ((mid.v.y + 1.0f) * 0.5f * cam->height);
	bot.v.x = ((bot.v.x + 1.0f) * 0.5f * cam->width);
	bot.v.y = ((bot.v.y + 1.0f) * 0.5f * cam->height);	

	//in full 1 unit steps
	float oneoverdx = 1.0f / (float)(((mid.v.x - bot.v.x)*(top.v.y - bot.v.y)) - ((top.v.x - bot.v.x)*(mid.v.y - bot.v.y)));
	textureXXStep = gradientDcDx(top.t.x * (1.0f / top.v.w), mid.t.x * (1.0f / mid.v.w), bot.t.x * (1.0f / bot.v.w), top, mid, bot) * oneoverdx;
	textureXYStep = gradientDcDy(top.t.x * (1.0f / top.v.w), mid.t.x * (1.0f / mid.v.w), bot.t.x * (1.0f / bot.v.w), top, mid, bot) * -oneoverdx;
	textureYXStep = gradientDcDx(top.t.y * (1.0f / top.v.w), mid.t.y * (1.0f / mid.v.w), bot.t.y * (1.0f / bot.v.w), top, mid, bot) * oneoverdx;
	textureYYStep = gradientDcDy(top.t.y * (1.0f / top.v.w), mid.t.y * (1.0f / mid.v.w), bot.t.y * (1.0f / bot.v.w), top, mid, bot) * -oneoverdx;

	inverseZXStep = gradientDcDx(1.0f / top.v.w, 1.0f / mid.v.w, 1.0f / bot.v.w, top, mid, bot) * oneoverdx;
	inverseZYStep = gradientDcDy(1.0f / top.v.w, 1.0f / mid.v.w, 1.0f / bot.v.w, top, mid, bot) * -oneoverdx;

	//float depthZXStep = gradientDcDx(top.v.z, mid.v.z, bot.v.z, top, mid, bot);
	//float depthZYStep = gradientDcDy(top.v.z, mid.v.z, bot.v.z, top, mid, bot);

	//if mid point is on right
	//Cz = AxBy - AyBx
	bool right = false;
	if (((top.v.x - mid.v.x) * (top.v.y - bot.v.y)) - ((top.v.y - mid.v.y) * (top.v.x - bot.v.x)) < 0)
		right = true;
		
	righthandslope = (top.v.x - mid.v.x) / (top.v.y - mid.v.y);
	lefthandslope = (top.v.x - bot.v.x) / (top.v.y - bot.v.y);

	if (!right) {
		lefthandslope = (top.v.x - mid.v.x) / (top.v.y - mid.v.y);
		righthandslope = (top.v.x - bot.v.x) / (top.v.y - bot.v.y);
	}

	yPreStep = int(top.v.y) - top.v.y;
	xPreStep = yPreStep * lefthandslope;

	leftX = top.v.x + (yPreStep * lefthandslope);
	rightX = top.v.x + (yPreStep * righthandslope);
		
	//z depth coordinate
	//float depthZStartLeft = top.v.z + (depthZXStep * xPreStep) + (depthZYStep * yPreStep);
	//float depthZNextLineLeft = depthZYStep + (depthZXStep * topToBotXStep);
	//inverse z coordinate
	inverseZStartLeft = (1.0f / top.v.w) + (inverseZXStep * xPreStep) + (inverseZYStep * yPreStep);
	inverseZNextLineLeft = inverseZYStep + (inverseZXStep * lefthandslope);
	//x texture coordinate
	texCoordStartXLeft = top.t.x * (1.0f / top.v.w) + (textureXXStep * xPreStep) + (textureXYStep * yPreStep);
	texCoordXNextLineLeft = textureXYStep + (textureXXStep * lefthandslope);
	//y texture coordinate
	texCoordStartYLeft = top.t.y * (1.0f / top.v.w) + (textureYXStep * xPreStep) + (textureYYStep * yPreStep);
	texCoordYNextLineLeft = textureYYStep + (textureYXStep * lefthandslope);

	xPreStep = yPreStep * righthandslope;
	//z depth coordinate
	//float depthZStartRight = top.v.z + (depthZXStep * xPreStep) + (depthZYStep * yPreStep);
	//float depthZNextLineRight = depthZYStep + (depthZXStep * topToMidXStep);
	//inverse z coordinate
	inverseZStartRight = (1.0f / top.v.w) + (inverseZXStep * xPreStep) + (inverseZYStep * yPreStep);
	inverseZNextLineRight = inverseZYStep + (inverseZXStep * righthandslope);
	//x texture coordinate
	texCoordStartXRight = top.t.x * (1.0f / top.v.w) + (textureXXStep * xPreStep) + (textureXYStep * yPreStep);
	texCoordXNextLineRight = textureXYStep + (textureXXStep * righthandslope);
	//y texture coordinate
	texCoordStartYRight = top.t.y * (1.0f / top.v.w) + (textureYXStep * xPreStep) + (textureYYStep * yPreStep);
	texCoordYNextLineRight = textureYYStep + (textureYXStep * righthandslope);
		
	//draw scanlines from top.y to mid.y
	scanline((int)top.v.y, (int)mid.v.y);
		
	if (right) {
		righthandslope = (mid.v.x - bot.v.x) / (mid.v.y - bot.v.y);
		yPreStep = int(mid.v.y) - mid.v.y;
		xPreStep = yPreStep * righthandslope;

		rightX = mid.v.x + (yPreStep * righthandslope);

		xPreStep = yPreStep * righthandslope;
		//z depth coordinate
		//depthZStartRight = mid.v.z + (depthZXStep * xPreStep) + (depthZYStep * yPreStep);
		//depthZNextLineRight = depthZYStep + (depthZXStep * midToBotXStep);
		//inverse z coordinate
		inverseZStartRight = (1.0f / mid.v.w) + (inverseZXStep * xPreStep) + (inverseZYStep * yPreStep);
		inverseZNextLineRight = inverseZYStep + (inverseZXStep * righthandslope);
		//x texture coordinate
		texCoordStartXRight = mid.t.x * (1.0f / mid.v.w) + (textureXXStep * xPreStep) + (textureXYStep * yPreStep);
		texCoordXNextLineRight = textureXYStep + (textureXXStep * righthandslope);
		//y texture coordinate
		texCoordStartYRight = mid.t.y * (1.0f / mid.v.w) + (textureYXStep * xPreStep) + (textureYYStep * yPreStep);
		texCoordYNextLineRight = textureYYStep + (textureYXStep * righthandslope);
	}
	else {
		lefthandslope = (mid.v.x - bot.v.x) / (mid.v.y - bot.v.y);
		//if (mid.v.y - bot.v.y == 0)
		//midToBotXStep = 0;

		yPreStep = int(mid.v.y) - mid.v.y;
		xPreStep = yPreStep * lefthandslope;

		leftX = mid.v.x + (yPreStep * lefthandslope);

		xPreStep = yPreStep * lefthandslope;

		//z depth coordinate
		//depthZStartLeft = mid.v.z + (depthZXStep * xPreStep) + (depthZYStep * yPreStep);
		//depthZNextLineLeft = depthZYStep + (depthZXStep * midToBotXStep);
		//inverse z coordinate
		inverseZStartLeft = (1.0f / mid.v.w) + (inverseZXStep * xPreStep) + (inverseZYStep * yPreStep);
		inverseZNextLineLeft = inverseZYStep + (inverseZXStep * lefthandslope);
		//x texture coordinate
		texCoordStartXLeft = mid.t.x * (1.0f / mid.v.w) + (textureXXStep * xPreStep) + (textureXYStep * yPreStep);
		texCoordXNextLineLeft = textureXYStep + (textureXXStep * lefthandslope);
		//y texture coordinate
		texCoordStartYLeft = mid.t.y * (1.0f / mid.v.w) + (textureYXStep * xPreStep) + (textureYYStep * yPreStep);
		texCoordYNextLineLeft = textureYYStep + (textureYXStep * lefthandslope);
	}

	//draw scanlines from mid.y to bot.y
	scanline((int)mid.v.y, (int)bot.v.y);
}

inline float Pipeline::gradientDcDx(float c0, float c1, float c2, Vertex &top, Vertex &mid, Vertex &bot) {
	return (((c1 - c2)*(top.v.y - bot.v.y)) - ((c0 - c2)*(mid.v.y - bot.v.y)));
}

inline float Pipeline::gradientDcDy(float c0, float c1, float c2, Vertex &top, Vertex &mid, Vertex &bot) {
	return (((c1 - c2)*(top.v.x - bot.v.x)) - ((c0 - c2)*(mid.v.x - bot.v.x)));
}

inline void Pipeline::sortVerticies(Vertex &top, Vertex &mid, Vertex &bot) {
	if (top.v.y < bot.v.y) {
		Vertex temp = top;
		top = bot;
		bot = temp;
	}
	
	if (top.v.y < mid.v.y) {
		Vertex temp = top;
		top = mid;
		mid = temp;
	}

	if (mid.v.y < bot.v.y) {
		Vertex temp = mid;
		mid = bot;
		bot = temp;
	}
}

#endif