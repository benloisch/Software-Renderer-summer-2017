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

	inline float gradientDcDx(float c0, float c1, float c2, Vertex &top, Vertex &mid, Vertex &bot);
	inline float gradientDcDy(float c0, float c1, float c2, Vertex &top, Vertex &mid, Vertex &bot);

private:





};

inline void Pipeline::clearDepthBuffer() {
	for (int i = 0; i < cam->width*cam->height; i++) {
		depthBuffer[i] = FLT_MAX;
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

		if (current.v.x > -current.v.w) //if current vertex is inside, add to list
			output.push_back(current);
		//check if one is inside and one is outside and if so, clip and add to copy
		if ((current.v.x > -current.v.w && next.v.x < -next.v.w) || (current.v.x < -current.v.w && next.v.x > -next.v.w)) {
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

		if (current.v.y > -current.v.w) //if current vertex is inside, add to list
			output.push_back(current);
		//check if one is inside and one is outside and if so, clip and add to copy
		if ((current.v.y > -current.v.w && next.v.y < -next.v.w) || (current.v.y < -current.v.w && next.v.y > -next.v.w)) {
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
	
	return output;
}

inline void Pipeline::shadeTriangle(Vertex top, Vertex mid, Vertex bot) {
	//check msg again to see if we pressed the escape key
	if (PeekMessage(msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(msg);
		DispatchMessage(msg);
	}
	if (msg->hwnd == NULL) //cancel all rendering if we pressed esc key
		return;

	//project from homogenous clip space to NDC space
	top.v /= top.v.w;
	mid.v /= mid.v.w;
	bot.v /= bot.v.w;

	//perform z-culling
	if (((mid.v - top.v) ^
		(bot.v - top.v)).z > 0) {
		return;
	}

	sortVerticies(top, mid, bot);

	//this clipping will take place in the clipVerticies() function
	if (top.v.x < -1.0 || top.v.x > 1.0 || top.v.y < -1.0 || top.v.y > 1.0 || top.v.z < 0 || top.v.z > 1)
		return;
	if (mid.v.x < -1.0 || mid.v.x > 1.0 || mid.v.y < -1.0 || mid.v.y > 1.0 || mid.v.z < 0 || mid.v.z > 1)
		return;
	if (bot.v.x < -1.0 || bot.v.x > 1.0 || bot.v.y < -1.0 || bot.v.y > 1.0 || bot.v.z < 0 || bot.v.z > 1)
		return;

	top.v.x = ((top.v.x + 1.0f) * 0.5f * cam->width);
	top.v.y = ((top.v.y + 1.0f) * 0.5f * cam->height);
	mid.v.x = ((mid.v.x + 1.0f) * 0.5f * cam->width);
	mid.v.y = ((mid.v.y + 1.0f) * 0.5f * cam->height);
	bot.v.x = ((bot.v.x + 1.0f) * 0.5f * cam->width);
	bot.v.y = ((bot.v.y + 1.0f) * 0.5f * cam->height);

	//Vertex top = verticies[0];
	//Vertex mid = verticies[1];
	//Vertex bot = verticies[2];

	//top.v.x = ((top.v.x + 1.0) * 0.5 * cam->width);
	//top.v.y = ((top.v.y + 1.0) * 0.5 * cam->height);
	//mid.v.x = ((mid.v.x + 1.0) * 0.5 * cam->width);
	//mid.v.y = ((mid.v.y + 1.0) * 0.5 * cam->height);
	//bot.v.x = ((bot.v.x + 1.0) * 0.5 * cam->width);
	//bot.v.y = ((bot.v.y + 1.0) * 0.5 * cam->height);		

	//in full 1 unit steps
	float textureXXStep = gradientDcDx(top.t.x * (1.0f / top.v.w), mid.t.x * (1.0f / mid.v.w), bot.t.x * (1.0f / bot.v.w), top, mid, bot);
	float textureXYStep = gradientDcDy(top.t.x * (1.0f / top.v.w), mid.t.x * (1.0f / mid.v.w), bot.t.x * (1.0f / bot.v.w), top, mid, bot);
	float textureYXStep = gradientDcDx(top.t.y * (1.0f / top.v.w), mid.t.y * (1.0f / mid.v.w), bot.t.y * (1.0f / bot.v.w), top, mid, bot);
	float textureYYStep = gradientDcDy(top.t.y * (1.0f / top.v.w), mid.t.y * (1.0f / mid.v.w), bot.t.y * (1.0f / bot.v.w), top, mid, bot);

	float inverseZXStep = gradientDcDx(1.0f / top.v.w, 1.0f / mid.v.w, 1.0f / bot.v.w, top, mid, bot);
	float inverseZYStep = gradientDcDy(1.0f / top.v.w, 1.0f / mid.v.w, 1.0f / bot.v.w, top, mid, bot);

	float depthZXStep = gradientDcDx(top.v.z, mid.v.z, bot.v.z, top, mid, bot);
	float depthZYStep = gradientDcDy(top.v.z, mid.v.z, bot.v.z, top, mid, bot);

	//if mid point is on right
	//Cz = AxBy - AyBx
	if (((top.v.x - mid.v.x) * (top.v.y - bot.v.y)) - ((top.v.y - mid.v.y) * (top.v.x - bot.v.x)) < 0) {
		
		float topToMidXStep = (top.v.x - mid.v.x) / (top.v.y - mid.v.y);
		//if (top.v.y - mid.v.y == 0)
			//topToMidXStep = 0;
		float topToBotXStep = (top.v.x - bot.v.x) / (top.v.y - bot.v.y);

		float yPreStep = int(top.v.y) - top.v.y;
		float xPreStep = yPreStep * topToBotXStep;

		float leftX = top.v.x + (yPreStep * topToBotXStep);
		float rightX = top.v.x + (yPreStep * topToMidXStep);
		
		//z depth coordinate
		float depthZStartLeft = top.v.z + (depthZXStep * xPreStep) + (depthZYStep * yPreStep);
		float depthZNextLineLeft = depthZYStep + (depthZXStep * topToBotXStep);
		//inverse z coordinate
		float inverseZStartLeft = (1.0f / top.v.w) + (inverseZXStep * xPreStep) + (inverseZYStep * yPreStep);
		float inverseZNextLineLeft = inverseZYStep + (inverseZXStep * topToBotXStep);
		//x texture coordinate
		float texCoordStartXLeft = top.t.x * (1.0f / top.v.w) + (textureXXStep * xPreStep) + (textureXYStep * yPreStep);
		float texCoordXNextLineLeft = textureXYStep + (textureXXStep * topToBotXStep);
		//y texture coordinate
		float texCoordStartYLeft = top.t.y * (1.0f / top.v.w) + (textureYXStep * xPreStep) + (textureYYStep * yPreStep);
		float texCoordYNextLineLeft = textureYYStep + (textureYXStep * topToBotXStep);

		xPreStep = yPreStep * topToMidXStep;
		//z depth coordinate
		float depthZStartRight = top.v.z + (depthZXStep * xPreStep) + (depthZYStep * yPreStep);
		float depthZNextLineRight = depthZYStep + (depthZXStep * topToMidXStep);
		//inverse z coordinate
		float inverseZStartRight = (1.0f / top.v.w) + (inverseZXStep * xPreStep) + (inverseZYStep * yPreStep);
		float inverseZNextLineRight = inverseZYStep + (inverseZXStep * topToMidXStep);
		//x texture coordinate
		float texCoordStartXRight = top.t.x * (1.0f / top.v.w) + (textureXXStep * xPreStep) + (textureXYStep * yPreStep);
		float texCoordXNextLineRight = textureXYStep + (textureXXStep * topToMidXStep);
		//y texture coordinate
		float texCoordStartYRight = top.t.y * (1.0f / top.v.w) + (textureYXStep * xPreStep) + (textureYYStep * yPreStep);
		float texCoordYNextLineRight = textureYYStep + (textureYXStep * topToMidXStep);
		
		//draw scanlines from top.y to mid.y
		for (int y = (int)(top.v.y) - 1; y >= (int)(mid.v.y); y--) {
			
			float xPreStep = int(leftX + 1.0) - leftX;
			float xdist = rightX - leftX;
			float XXStep = (texCoordStartXRight - texCoordStartXLeft) / xdist;
			float YXStep = (texCoordStartYRight - texCoordStartYLeft) / xdist;
			float ZXStep = (inverseZStartRight - inverseZStartLeft) / xdist;
			float ZXDepthStep = (depthZStartRight - depthZStartLeft) / xdist;

			float textureCoordX = texCoordStartXLeft + (XXStep * xPreStep);
			float textureCoordY = texCoordStartYLeft + (YXStep * xPreStep);
			float inverseZ = inverseZStartLeft + (ZXStep * xPreStep);
			//float depthZ = depthZStartLeft + (ZXDepthStep * xPreStep);

			for (int x = (int)(leftX); x < (int)(rightX); x++) {

				double z = 1.0 / inverseZ;
				//double texX = textureCoordX*z;
				//double texY = textureCoordY*z;

				//if (texX > 1 || texY > 1 || texX < 0 || texY < 0) {
				//	int stop = 0;
				//}

				//Color_RGB color(mesh->texture->getPixelColor((int)((textureCoordX*z)*(mesh->texture->width - 1) + 0.5), (int)((textureCoordY*z)*(mesh->texture->height - 1) + 0.5)));

				if (z < depthBuffer[y * cam->width + x]) {
					char *pixelComponent = buffer + ((y * (int)cam->width + x) * 4);
					*(pixelComponent) = (unsigned char)(mesh->texture->blue[(int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)][(int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5)]);
					*(pixelComponent + 1) = (unsigned char)(mesh->texture->green[(int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)][(int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5)]);
					*(pixelComponent + 2) = (unsigned char)(mesh->texture->red[(int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)][(int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5)]);
					//*(pixelComponent + 3) = (unsigned char)0;

					depthBuffer[y * cam->width + x] = z;
				}

				textureCoordX += XXStep;
				textureCoordY += YXStep;
				inverseZ += ZXStep;
				//textureCoordX += textureXXStep;
				//textureCoordY += textureYXStep;
				//inverseZ += inverseZXStep;
			}
			
			leftX -= topToBotXStep;
			rightX -= topToMidXStep;
			
			texCoordStartXLeft -= texCoordXNextLineLeft;
			texCoordStartYLeft -= texCoordYNextLineLeft;
			texCoordStartXRight -= texCoordXNextLineRight;
			texCoordStartYRight -= texCoordYNextLineRight;
			inverseZStartLeft -= inverseZNextLineLeft;
			inverseZStartRight -= inverseZNextLineRight;
			depthZStartLeft -= depthZNextLineLeft;
			depthZStartRight -= depthZNextLineRight;
		}
		
		float midToBotXStep = (mid.v.x - bot.v.x) / (mid.v.y - bot.v.y);
		//if (mid.v.y - bot.v.y == 0)
			//midToBotXStep = 0;
		yPreStep = int(mid.v.y) - mid.v.y;
		xPreStep = yPreStep * midToBotXStep;

		rightX = mid.v.x + (yPreStep * midToBotXStep);

		xPreStep = yPreStep * midToBotXStep;
		//z depth coordinate
		depthZStartRight = mid.v.z + (depthZXStep * xPreStep) + (depthZYStep * yPreStep);
		depthZNextLineRight = depthZYStep + (depthZXStep * midToBotXStep);
		//inverse z coordinate
		inverseZStartRight = (1.0f / mid.v.w) + (inverseZXStep * xPreStep) + (inverseZYStep * yPreStep);
		inverseZNextLineRight = inverseZYStep + (inverseZXStep * midToBotXStep);
		//x texture coordinate
		texCoordStartXRight = mid.t.x * (1.0f / mid.v.w) + (textureXXStep * xPreStep) + (textureXYStep * yPreStep);
		texCoordXNextLineRight = textureXYStep + (textureXXStep * midToBotXStep);
		//y texture coordinate
		texCoordStartYRight = mid.t.y * (1.0f / mid.v.w) + (textureYXStep * xPreStep) + (textureYYStep * yPreStep);
		texCoordYNextLineRight = textureYYStep + (textureYXStep * midToBotXStep);

		for (int y = (int)(mid.v.y) - 1; y >= (int)(bot.v.y); y--) {

			float xPreStep = int(leftX + 1.0) - leftX;
			float xdist = rightX - leftX;
			float XXStep = (texCoordStartXRight - texCoordStartXLeft) / xdist;
			float YXStep = (texCoordStartYRight - texCoordStartYLeft) / xdist;
			float ZXStep = (inverseZStartRight - inverseZStartLeft) / xdist;
			float ZXDepthStep = (depthZStartRight - depthZStartLeft) / xdist;

			float textureCoordX = texCoordStartXLeft + (XXStep * xPreStep);
			float textureCoordY = texCoordStartYLeft + (YXStep * xPreStep);
			float inverseZ = inverseZStartLeft + (ZXStep * xPreStep);
			//float depthZ = depthZStartLeft + (ZXDepthStep * xPreStep);

			for (int x = (int)(leftX); x < (int)(rightX); x++) {

				double z = 1.0 / inverseZ;
				//double texX = textureCoordX*z;
				//double texY = textureCoordY*z;

				//if (texX > 1 || texY > 1 || texX < 0 || texY < 0) {
				//	int stop = 0;
				//}

				//Color_RGB color(mesh->texture->getPixelColor((int)((texX)*(mesh->texture->width - 1) + 0.5), (int)((texY)*(mesh->texture->height - 1) + 0.5)));

				if (z < depthBuffer[y * cam->width + x]) {
					char *pixelComponent = buffer + ((y * (int)cam->width + x) * 4);
					*(pixelComponent) = (unsigned char)(mesh->texture->blue[(int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)][(int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5)]);
					*(pixelComponent + 1) = (unsigned char)(mesh->texture->green[(int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)][(int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5)]);
					*(pixelComponent + 2) = (unsigned char)(mesh->texture->red[(int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)][(int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5)]);
					//*(pixelComponent + 3) = (unsigned char)0;

					depthBuffer[y * cam->width + x] = z;
				}

				textureCoordX += XXStep;
				textureCoordY += YXStep;
				inverseZ += ZXStep;
				//textureCoordX += textureXXStep;
				//textureCoordY += textureYXStep;
				//inverseZ += inverseZXStep;
			}

			leftX -= topToBotXStep;
			rightX -= midToBotXStep;

			texCoordStartXLeft -= texCoordXNextLineLeft;
			texCoordStartYLeft -= texCoordYNextLineLeft;
			texCoordStartXRight -= texCoordXNextLineRight;
			texCoordStartYRight -= texCoordYNextLineRight;
			inverseZStartLeft -= inverseZNextLineLeft;
			inverseZStartRight -= inverseZNextLineRight;
			depthZStartLeft -= depthZNextLineLeft;
			depthZStartRight -= depthZNextLineRight;
		}
		
	}
	else { //if midpoint is on the left
		
		float topToMidXStep = (top.v.x - mid.v.x) / (top.v.y - mid.v.y);
		//if (top.v.y - mid.v.y == 0)
			//topToMidXStep = 0;
		float topToBotXStep = (top.v.x - bot.v.x) / (top.v.y - bot.v.y);

		float yPreStep = int(top.v.y) - top.v.y;
		float xPreStep = yPreStep * topToMidXStep;

		float leftX = top.v.x + (yPreStep * topToMidXStep);
		float rightX = top.v.x + (yPreStep * topToBotXStep);

		//z depth coordinate
		float depthZStartLeft = top.v.z + (depthZXStep * xPreStep) + (depthZYStep * yPreStep);
		float depthZNextLineLeft = depthZYStep + (depthZXStep * topToBotXStep);
		//inverse z coordinate
		float inverseZStartLeft = (1.0f / top.v.w) + (inverseZXStep * xPreStep) + (inverseZYStep * yPreStep);
		float inverseZNextLineLeft = inverseZYStep + (inverseZXStep * topToMidXStep);
		//x texture coordinate
		float texCoordStartXLeft = top.t.x * (1.0f / top.v.w) + (textureXXStep * xPreStep) + (textureXYStep * yPreStep);
		float texCoordXNextLineLeft = textureXYStep + (textureXXStep * topToMidXStep);
		//y texture coordinate
		float texCoordStartYLeft = top.t.y * (1.0f / top.v.w) + (textureYXStep * xPreStep) + (textureYYStep * yPreStep);
		float texCoordYNextLineLeft = textureYYStep + (textureYXStep * topToMidXStep);

		xPreStep = yPreStep * topToBotXStep;
		//z depth coordinate
		float depthZStartRight = top.v.z + (depthZXStep * xPreStep) + (depthZYStep * yPreStep);
		float depthZNextLineRight = depthZYStep + (depthZXStep * topToBotXStep);
		//inverse z coordinate
		float inverseZStartRight = (1.0f / top.v.w) + (inverseZXStep * xPreStep) + (inverseZYStep * yPreStep);
		float inverseZNextLineRight = inverseZYStep + (inverseZXStep * topToBotXStep);
		//x texture coordinate
		float texCoordStartXRight = top.t.x * (1.0f / top.v.w) + (textureXXStep * xPreStep) + (textureXYStep * yPreStep);
		float texCoordXNextLineRight = textureXYStep + (textureXXStep * topToBotXStep);
		//y texture coordinate
		float texCoordStartYRight = top.t.y * (1.0f / top.v.w) + (textureYXStep * xPreStep) + (textureYYStep * yPreStep);
		float texCoordYNextLineRight = textureYYStep + (textureYXStep * topToBotXStep);

		//draw scanlines from top.y to mid.y
		for (int y = (int)(top.v.y) - 1; y >= (int)(mid.v.y); y--) {

			float xPreStep = int(leftX + 1.0) - leftX;
			float xdist = rightX - leftX;
			float XXStep = (texCoordStartXRight - texCoordStartXLeft) / xdist;
			float YXStep = (texCoordStartYRight - texCoordStartYLeft) / xdist;
			float ZXStep = (inverseZStartRight - inverseZStartLeft) / xdist;
			float ZXDepthStep = (depthZStartRight - depthZStartLeft) / xdist;

			float textureCoordX = texCoordStartXLeft + (XXStep * xPreStep);
			float textureCoordY = texCoordStartYLeft + (YXStep * xPreStep);
			float inverseZ = inverseZStartLeft + (ZXStep * xPreStep);
			//float depthZ = depthZStartLeft + (ZXDepthStep * xPreStep);

			for (int x = (int)(leftX); x < (int)(rightX); x++) {

				double z = 1.0 / inverseZ;
				//double texX = textureCoordX*z;
				//double texY = textureCoordY*z;

				//if (texX > 1 || texY > 1 || texX < 0 || texY < 0) {
				//	int stop = 0;
				//}

				//Color_RGB color(mesh->texture->getPixelColor((int)((textureCoordX*z)*(mesh->texture->width - 1) + 0.5), (int)((textureCoordY*z)*(mesh->texture->height - 1) + 0.5)));

				if (z < depthBuffer[y * cam->width + x]) {
					char *pixelComponent = buffer + ((y * (int)cam->width + x) * 4);
					*(pixelComponent) = (unsigned char)(mesh->texture->blue[(int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)][(int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5)]);
					*(pixelComponent + 1) = (unsigned char)(mesh->texture->green[(int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)][(int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5)]);
					*(pixelComponent + 2) = (unsigned char)(mesh->texture->red[(int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)][(int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5)]);
					//*(pixelComponent + 3) = (unsigned char)0;

					depthBuffer[y * cam->width + x] = z;
				}

				textureCoordX += XXStep;
				textureCoordY += YXStep;
				inverseZ += ZXStep;
				//textureCoordX += textureXXStep;
				//textureCoordY += textureYXStep;
				//inverseZ += inverseZXStep;
			}

			leftX -= topToMidXStep;
			rightX -= topToBotXStep;

			texCoordStartXLeft -= texCoordXNextLineLeft;
			texCoordStartYLeft -= texCoordYNextLineLeft;
			texCoordStartXRight -= texCoordXNextLineRight;
			texCoordStartYRight -= texCoordYNextLineRight;
			inverseZStartLeft -= inverseZNextLineLeft;
			inverseZStartRight -= inverseZNextLineRight;
			depthZStartLeft -= depthZNextLineLeft;
			depthZStartRight -= depthZNextLineRight;
		}
		
		float midToBotXStep = (mid.v.x - bot.v.x) / (mid.v.y - bot.v.y);
		//if (mid.v.y - bot.v.y == 0)
			//midToBotXStep = 0;

		yPreStep = int(mid.v.y) - mid.v.y;
		xPreStep = yPreStep * midToBotXStep;

		leftX = mid.v.x + (yPreStep * midToBotXStep);

		xPreStep = yPreStep * midToBotXStep;

		//z depth coordinate
		depthZStartLeft = mid.v.z + (depthZXStep * xPreStep) + (depthZYStep * yPreStep);
		depthZNextLineLeft = depthZYStep + (depthZXStep * midToBotXStep);
		//inverse z coordinate
		inverseZStartLeft = (1.0f / mid.v.w) + (inverseZXStep * xPreStep) + (inverseZYStep * yPreStep);
		inverseZNextLineLeft = inverseZYStep + (inverseZXStep * midToBotXStep);
		//x texture coordinate
		texCoordStartXLeft = mid.t.x * (1.0f / mid.v.w) + (textureXXStep * xPreStep) + (textureXYStep * yPreStep);
		texCoordXNextLineLeft = textureXYStep + (textureXXStep * midToBotXStep);
		//y texture coordinate
		texCoordStartYLeft = mid.t.y * (1.0f / mid.v.w) + (textureYXStep * xPreStep) + (textureYYStep * yPreStep);
		texCoordYNextLineLeft = textureYYStep + (textureYXStep * midToBotXStep);

		for (int y = (int)(mid.v.y) - 1; y >= (int)(bot.v.y); y--) {

			float xPreStep = int(leftX + 1.0) - leftX;
			float xdist = rightX - leftX;
			float XXStep = (texCoordStartXRight - texCoordStartXLeft) / xdist;
			float YXStep = (texCoordStartYRight - texCoordStartYLeft) / xdist;
			float ZXStep = (inverseZStartRight - inverseZStartLeft) / xdist;
			float ZXDepthStep = (depthZStartRight - depthZStartLeft) / xdist;

			float textureCoordX = texCoordStartXLeft + (XXStep * xPreStep);
			float textureCoordY = texCoordStartYLeft + (YXStep * xPreStep);
			float inverseZ = inverseZStartLeft + (ZXStep * xPreStep);
			//float depthZ = depthZStartLeft + (ZXDepthStep * xPreStep);

			for (int x = (int)(leftX); x < (int)(rightX); x++) {

				double z = 1.0 / inverseZ;
				//double texX = textureCoordX*z;
				//double texY = textureCoordY*z;

				//if (texX > 1 || texY > 1 || texX < 0 || texY < 0) {
				//	int stop = 0;
				//}

				//Color_RGB color(mesh->texture->getPixelColor((int)((texX)*(mesh->texture->width - 1) + 0.5), (int)((texY)*(mesh->texture->height - 1) + 0.5)));

				if (z < depthBuffer[y * cam->width + x]) {
					char *pixelComponent = buffer + ((y * (int)cam->width + x) * 4);
					*(pixelComponent) = (unsigned char)(mesh->texture->blue[(int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)][(int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5)]);
					*(pixelComponent + 1) = (unsigned char)(mesh->texture->green[(int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)][(int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5)]);
					*(pixelComponent + 2) = (unsigned char)(mesh->texture->red[(int)((textureCoordX * z)*(mesh->texture->width - 1) + 0.5)][(int)((textureCoordY * z)*(mesh->texture->height - 1) + 0.5)]);
					//*(pixelComponent + 3) = (unsigned char)0;

					depthBuffer[y * cam->width + x] = z;
				}

				textureCoordX += XXStep;
				textureCoordY += YXStep;
				inverseZ += ZXStep;
				//depthZ += ZXDepthStep;
				//textureCoordX += textureXXStep;
				//textureCoordY += textureYXStep;
				//inverseZ += inverseZXStep;
			}

			leftX -= midToBotXStep;
			rightX -= topToBotXStep;

			texCoordStartXLeft -= texCoordXNextLineLeft;
			texCoordStartYLeft -= texCoordYNextLineLeft;
			texCoordStartXRight -= texCoordXNextLineRight;
			texCoordStartYRight -= texCoordYNextLineRight;
			inverseZStartLeft -= inverseZNextLineLeft;
			inverseZStartRight -= inverseZNextLineRight;
			depthZStartLeft -= depthZNextLineLeft;
			depthZStartRight -= depthZNextLineRight;
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

inline float Pipeline::gradientDcDx(float c0, float c1, float c2, Vertex &top, Vertex &mid, Vertex &bot) {
	return (((c1 - c2)*(top.v.y - bot.v.y)) - ((c0 - c2)*(mid.v.y - bot.v.y))) /
		(((mid.v.x - bot.v.x)*(top.v.y - bot.v.y)) - ((top.v.x - bot.v.x)*(mid.v.y - bot.v.y)));
}

inline float Pipeline::gradientDcDy(float c0, float c1, float c2, Vertex &top, Vertex &mid, Vertex &bot) {
	return (((c1 - c2)*(top.v.x - bot.v.x)) - ((c0 - c2)*(mid.v.x - bot.v.x))) /
		(((top.v.x - bot.v.x)*(mid.v.y - bot.v.y)) - ((mid.v.x - bot.v.x)*(top.v.y - bot.v.y)));
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