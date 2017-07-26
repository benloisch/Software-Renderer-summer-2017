#ifndef PIPELINE_H_
#define PIPELINE_H_

#include <vector>
#include <thread>

#include "Mesh.h"
#include "Camera.h"

class Pipeline {
public:

	int trianglesRendered;

	MSG *msg;

	float* depthBuffer;

	char* buffer;
	Camera *cam;

	//Vertex verticies[3]; //[0] = top, [1] = mid, [2] = bottom 
	//Vector4D texCoords[3];
	//Vector4D normals[3];

	Vertex verticies[14];

	Mesh *mesh;

	Pipeline();
	void setScreenBuffer(char *inputBuffer);
	void setCamera(Camera *inputCam);
	inline void clearDepthBuffer();

	void transform(Mesh *inputMesh);
	inline void projectVerticies(Matrix4x4 &MVP);
	inline int clipVerticies();
	inline void shadeTriangle(Vertex top, Vertex mid, Vertex bot);
	inline void sortVerticies(Vertex &top, Vertex &mid, Vertex &bot);
	inline void scanline(int ystart, int yend);

	inline float saturate(float lightVal);

	inline float gradientDcDx(float c0, float c1, float c2, Vertex &top, Vertex &mid, Vertex &bot);
	inline float gradientDcDy(float c0, float c1, float c2, Vertex &top, Vertex &mid, Vertex &bot);

	//variables used for shading triangle
	Vector4D directionalLight;

	float righthandslope;
	float lefthandslope;
	float lightXStep;
	float lightYStep;
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
	float lightStartLeft;
	float lightNextLineLeft;
	float texCoordStartXLeft;
	float texCoordXNextLineLeft;
	float texCoordStartYLeft;
	float texCoordYNextLineLeft;
	float inverseZStartRight;
	float inverseZNextLineRight;
	float lightStartRight;
	float lightNextLineRight;
	float texCoordStartXRight;
	float texCoordXNextLineRight;
	float texCoordStartYRight;
	float texCoordYNextLineRight;

	int textureWidth;
	int textureHeight;
	int textureWidthMinusOne;
	int textureHeightMinusOne;

	float topTexX;
	float topTexY;
	float midTexX;
	float midTexY;
	float botTexX;
	float botTexY;

	//int localIntBuffer[16];
private:





};

inline float Pipeline::saturate(float lightVal) {
	if (lightVal <= 0.0f) {
		return 0.0f;
	}
	if (lightVal > 1.0f) {
		return 1.0f;
	}

	return lightVal;
}

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

inline void Pipeline::projectVerticies(Matrix4x4 &MVP) {
	verticies[0].v *= MVP;
	verticies[1].v *= MVP;
	verticies[2].v *= MVP;
}

inline int Pipeline::clipVerticies() {
	//check if entire triangle lies outside viewing frustum
	//clip in homogenous clip space
	//vector<Vertex> output;

	//clip x, y, and z against -w and w
	//t = (currentW - currentP)  / *(currentW - currentP) - (nextW - nextP))

	
	float currVal, currW, nextVal, nextW;

	//******************************************************************************** clip x against w
	Vertex current;
	Vertex next;
	verticies[3] = verticies[0];
	int topOfStack = 13;

	for (unsigned int i = 0; i < 3; i++) {

		currVal = (verticies + i)->v.x;
		currW = (verticies + i)->v.w;
		nextVal = (verticies + i + 1)->v.x;
		nextW = (verticies + i + 1)->v.w;
		
		if (currVal < currW) { //if current vertex is inside, add to list
			verticies[topOfStack] = verticies[i];
			topOfStack--;
		}
		//check if one is inside and one is outside and if so, clip and add to copy
		if ((currVal < currW && nextVal > nextW) || (currVal > currW && nextVal < nextW)) {
			float t = (currW - currVal) / ((currW - currVal) - (nextW - nextVal));
			Vertex v = verticies[i];
			v.v = v.v.lerp((verticies + i + 1)->v, t);
			v.t = v.t.lerp((verticies + i + 1)->t, t);
			v.v.x = v.v.w;
			verticies[topOfStack] = v;
			topOfStack--;
		}
	}

	if (topOfStack > 11)
		return 0;	
	
	//******************************************************************************** clip x against -w
	verticies[topOfStack] = verticies[13];
	int bottomOfStack = 0;

	for (int i = 13; i > topOfStack; i--) {

		currVal = (verticies + i)->v.x;
		currW = (verticies + i)->v.w;
		nextVal = (verticies + i - 1)->v.x;
		nextW = (verticies + i - 1)->v.w;

		if (-currVal < currW) { //if current vertex is inside, add to list
			verticies[bottomOfStack] = verticies[i];
			bottomOfStack++;
		}

		//check if one is inside and one is outside and if so, clip and add to copy
		if ((-currVal < currW && -nextVal > nextW) || (-currVal > currW && -nextVal < nextW)) {
			float t = (-currW - currVal) / ((-currW - currVal) - (-nextW - nextVal));
			Vertex v = verticies[i];
			v.v = v.v.lerp((verticies + i - 1)->v, t);
			v.t = v.t.lerp((verticies + i - 1)->t, t);
			v.v.x = -v.v.w;
			verticies[bottomOfStack] = v;
			bottomOfStack++;
		}
	}
	
	if (bottomOfStack < 2)
		return 0;
	
	/*
	//******************************************************************************** clip x against w
	Vertex current;
	Vertex next;
	verticies[3] = verticies[0];
	int topOfStack = 13;

	for (unsigned int i = 0; i < 3; i++) {

		current = verticies[i];
		next = verticies[i + 1];

		if (current.v.x < current.v.w) { //if current vertex is inside, add to list
			verticies[topOfStack] = current;
			topOfStack--;
		}
		//check if one is inside and one is outside and if so, clip and add to copy
		if ((current.v.x < current.v.w && next.v.x > next.v.w) || (current.v.x > current.v.w && next.v.x < next.v.w)) {
			float t = (current.v.w - current.v.x) / ((current.v.w - current.v.x) - (next.v.w - next.v.x));
			Vertex v = current;
			v.v = v.v.lerp(next.v, t);
			v.t = v.t.lerp(next.t, t);
			v.v.x = v.v.w;
			verticies[topOfStack] = v;
			topOfStack--;
		}
	}

	if (topOfStack > 11)
		return 0;	
	
	//******************************************************************************** clip x against -w
	verticies[topOfStack] = verticies[13];
	int bottomOfStack = 0;

	for (int i = 13; i > topOfStack; i--) {

		current = verticies[i];
		next = verticies[i - 1];

		if (-current.v.x < current.v.w) { //if current vertex is inside, add to list
			verticies[bottomOfStack] = current;
			bottomOfStack++;
		}
		//check if one is inside and one is outside and if so, clip and add to copy
		if ((-current.v.x < current.v.w && -next.v.x > next.v.w) || (-current.v.x > current.v.w && -next.v.x < next.v.w)) {
			float t = (-current.v.w - current.v.x) / ((-current.v.w - current.v.x) - (-next.v.w - next.v.x));
			Vertex v = current;
			v.v = v.v.lerp(next.v, t);
			v.t = v.t.lerp(next.t, t);
			v.v.x = -v.v.w;
			verticies[bottomOfStack] = v;
			bottomOfStack++;
		}
	}
	
	if (bottomOfStack < 2)
		return 0;
		*/
	//******************************************************************************** clip y against w
	verticies[bottomOfStack] = verticies[0];
	topOfStack = 13;

	for (int i = 0; i < bottomOfStack; i++) {

		currVal = (verticies + i)->v.y;
		currW = (verticies + i)->v.w;
		nextVal = (verticies + i + 1)->v.y;
		nextW = (verticies + i + 1)->v.w;

		if (currVal < currW) { //if current vertex is inside, add to list
			verticies[topOfStack] = verticies[i];
			topOfStack--;
		}
		//check if one is inside and one is outside and if so, clip and add to copy
		if ((currVal < currW && nextVal > nextW) || (currVal > currW && nextVal < nextW)) {
			float t = (currW - currVal) / ((currW - currVal) - (nextW - nextVal));
			Vertex v = verticies[i];
			v.v = v.v.lerp((verticies + i + 1)->v, t);
			v.t = v.t.lerp((verticies + i + 1)->t, t);
			v.v.y = v.v.w;
			verticies[topOfStack] = v;
			topOfStack--;
		}
	}

	if (topOfStack > 11)
		return 0;

	//******************************************************************************** clip y against -w
	verticies[topOfStack] = verticies[13];
	bottomOfStack = 0;

	for (int i = 13; i > topOfStack; i--) {

		currVal = (verticies + i)->v.y;
		currW = (verticies + i)->v.w;
		nextVal = (verticies + i - 1)->v.y;
		nextW = (verticies + i - 1)->v.w;

		if (-currVal < currW) { //if current vertex is inside, add to list
			verticies[bottomOfStack] = verticies[i];
			bottomOfStack++;
		}

		//check if one is inside and one is outside and if so, clip and add to copy
		if ((-currVal < currW && -nextVal > nextW) || (-currVal > currW && -nextVal < nextW)) {
			float t = (-currW - currVal) / ((-currW - currVal) - (-nextW - nextVal));
			Vertex v = verticies[i];
			v.v = v.v.lerp((verticies + i - 1)->v, t);
			v.t = v.t.lerp((verticies + i - 1)->t, t);
			v.v.y = -v.v.w;
			verticies[bottomOfStack] = v;
			bottomOfStack++;
		}
	}

	if (bottomOfStack < 2)
		return 0;

	//******************************************************************************** clip z against w
	verticies[bottomOfStack] = verticies[0];
	topOfStack = 13;

	for (int i = 0; i < bottomOfStack; i++) {

		currVal = (verticies + i)->v.z;
		currW = (verticies + i)->v.w;
		nextVal = (verticies + i + 1)->v.z;
		nextW = (verticies + i + 1)->v.w;

		if (currVal < currW) { //if current vertex is inside, add to list
			verticies[topOfStack] = verticies[i];
			topOfStack--;
		}
		//check if one is inside and one is outside and if so, clip and add to copy
		if ((currVal < currW && nextVal > nextW) || (currVal > currW && nextVal < nextW)) {
			float t = (currW - currVal) / ((currW - currVal) - (nextW - nextVal));
			Vertex v = verticies[i];
			v.v = v.v.lerp((verticies + i + 1)->v, t);
			v.t = v.t.lerp((verticies + i + 1)->t, t);
			v.v.z = v.v.w;
			verticies[topOfStack] = v;
			topOfStack--;
		}
	}

	if (topOfStack > 11)
		return 0;

	//******************************************************************************** clip z against 0
	verticies[topOfStack] = verticies[13];
	bottomOfStack = 0;

	for (int i = 13; i > topOfStack; i--) {

		currVal = (verticies + i)->v.z;
		currW = 0;
		nextVal = (verticies + i - 1)->v.z;
		nextW = 0;

		if (-currVal < currW) { //if current vertex is inside, add to list
			verticies[bottomOfStack] = verticies[i];
			bottomOfStack++;
		}

		//check if one is inside and one is outside and if so, clip and add to copy
		if ((-currVal < currW && -nextVal > nextW) || (-currVal > currW && -nextVal < nextW)) {
			float t = (-currW - currVal) / ((-currW - currVal) - (-nextW - nextVal));
			Vertex v = verticies[i];
			v.v = v.v.lerp((verticies + i - 1)->v, t);
			v.t = v.t.lerp((verticies + i - 1)->t, t);
			v.v.z = 0.001f;
			verticies[bottomOfStack] = v;
			bottomOfStack++;
		}
	}

	if (bottomOfStack < 2)
		return 0;

	return bottomOfStack;
}

inline void Pipeline::scanline(int ystart, int yend) {
	
	//UINT tileW = mesh->texture.tileW;
	//UINT tileH = mesh->texture.tileH;
	//UINT widthInTiles = (textureWidth + tileW - 1) / tileW;
	
	//int tilePixels[64] = { 0 };
	//for (int i = 0; i < 64; i++)
	//	tilePixels[i] = 9999999;
	//int prevTile = -1;
	/*
	int textureBufferLength = (botTexY - topTexY)*textureWidth*((max(max(botTexX, midTexX), topTexX)) - (min(min(botTexX, midTexX), topTexX)))*textureHeight;
	int texBoxMinPixelX = min(min(botTexX, midTexX), topTexX) * textureWidth;
	int texBoxMinPixelY = min(min(botTexY, midTexY), topTexY) * textureHeight;
	int texBoxMaxPixelX = max(max(botTexX, midTexX), topTexX) * textureWidth;
	int texBoxMaxPixelY = max(max(botTexY, midTexY), topTexY) * textureHeight;
	int texBoxWidth = texBoxMaxPixelX - texBoxMinPixelX;
	int texBoxHeight = texBoxMaxPixelY - texBoxMinPixelY;
	int index = 0;
	//int *textureBuffer = new int[16777216];
	int *textureBuffer = new int[1024];
	for (int i = 0; i < 1024; i++)
		textureBuffer[i] = 99999999;
	int bufferIndex = 0;
	*/
	/*
	for (int y = texBoxMinPixelY; y < texBoxMaxPixelY; y++) {
		for (int x = texBoxMinPixelX; x < texBoxMaxPixelX; x++) {
			textureBuffer[bufferIndex] = mesh.texture.intbuffer[x + y*textureWidth];
			bufferIndex++;
		}
	}
	*/
	for (int y = ystart - 1; y >= yend; y--) {

		float xPreStep = int(leftX + 1.0) - leftX;
		float xdist = rightX - leftX;
		float XXStep = (texCoordStartXRight - texCoordStartXLeft) / xdist;
		float YXStep = (texCoordStartYRight - texCoordStartYLeft) / xdist;
		float ZXStep = (inverseZStartRight - inverseZStartLeft) / xdist;
		//float lghtXStep = (lightStartRight - lightStartLeft) / xdist;
		//float ZXDepthStep = (depthZStartRight - depthZStartLeft) / xdist;

		//float lightValue = lightStartLeft + (lightXStep * xPreStep);
		float textureCoordX = texCoordStartXLeft + (XXStep * xPreStep);
		float textureCoordY = texCoordStartYLeft + (YXStep * xPreStep);
		float inverseZ = inverseZStartLeft + (ZXStep * xPreStep);
		//float depthZ = depthZStartLeft + (ZXDepthStep * xPreStep);
		float *pdepth = depthBuffer + (y * cam->width + (int)(leftX));
		unsigned int* ibuffer = (unsigned int*)(buffer + ((y * cam->width * 2 + (int)(leftX)) * 4 * 2));
		for (int x = (int)(leftX); x < (int)(rightX); x++) {

			//if (pressed#2)
			//	x += ((int)rightX - (int)leftX);

			if (inverseZ > *pdepth) {
				float test = textureCoordX / inverseZ;
				float test2 = textureCoordY / inverseZ;
				if (test > 1.0)
					test = 1.0;
				if (test < 0.0)
					test = 0.0;
				if (test2 > 1.0)
					test2 = 1.0;
				if (test2 < 0.0)
					test2 = 0.0;

				int test3 = ((int)((test)*(textureWidthMinusOne)+0.5)) + textureWidth * ((int)((test2)*(textureHeightMinusOne)+0.5));
				*ibuffer = mesh->texture.intbuffer[test3];
				*(ibuffer + 1) = *ibuffer;
				*(ibuffer + cam->width * 2) = *ibuffer;
				*(ibuffer + cam->width * 2 + 1) = *ibuffer;
				//int origTexX = ((int)((test)*(textureWidthMinusOne)+0.5));
				//int origTexY = ((int)((test2)*(textureHeightMinusOne)+0.5));
				
				//int x = (int)((test)*(textureWidthMinusOne)+0.5);
				//int y = (int)((test2)*(textureHeightMinusOne)+0.5);
				
				//UINT tileX = x / tileW;
				//UINT tileY = y / tileH;
				//UINT inTileX = x % tileW;
				//UINT inTileY = y % tileH;

				//int whatTile = tileX + tileY * widthInTiles;
				//int whatTilePixel = inTileX + inTileY * tileW;
				
				//if (tileX + tileY * widthInTiles != prevTile) {
				//	prevTile = tileX + tileY * widthInTiles;

					//tilePixels = (mesh->texture.tiles[tileX + tileY * widthInTiles]);

					//for (int i = 0; i < tileW * tileH; i++) {
					//	tilePixels[i] = 256 << 8;// mesh->texture.tiles[tileX + tileY * widthInTiles][x + y * tileW];
					//}
				//}
				
				//*ibuffer = tilePixels[inTileX + inTileY * tileW];

				//*ibuffer = mesh->texture.tiles[tileX + tileY * widthInTiles][inTileX + inTileY * tileW];
				//if (inTileX + inTileY * tileW % tileW == 0 || inTileX + inTileY * tileW < tileW)
					//*ibuffer = 0;
				//*ibuffer = mesh->texture.intbuffer[(tileY * widthInTiles + tileX) * (tileW * tileH) + inTileY * tileW + inTileX];

				//*ibuffer = localIntBuffer[inTileX + inTileY * tileW];

				//*ibuffer = mesh->texture->intbuffer[(tileY * widthInTiles + tileX) * (tileW * tileH) + inTileY * tileW + inTileX];
				//*ibuffer = mesh->texture->intbuffer[test4 * textureWidth];
				//*ibuffer = tilePixels[((int)((test)*(textureWidthMinusOne)+0.5) % tileW) + (((int)((test2)*(textureHeightMinusOne)+0.5)) % tileH) * tileW];

				//int texBufX = texBoxMaxPixelX - origTexX;
				//int texBufY = texBoxMaxPixelY - origTexY;
				//*ibuffer = textureBuffer[(texBoxWidth - texBufX) + (texBoxHeight - texBufY) * texBoxWidth];
				//*ibuffer = textureBuffer[((int)((test)*(31)+0.5)) + 32 * ((int)((test2)*(31)+0.5))];
				/*
				int RGB = mesh->texture->intbuffer[test3];
				unsigned char b = ((unsigned char*)(&RGB))[0] * lightValue;
				unsigned char g = ((unsigned char*)(&RGB))[1] * lightValue;
				unsigned char r = ((unsigned char*)(&RGB))[2] * lightValue;

				RGB = 0;
				RGB = RGB | ((unsigned int)b);
				RGB = RGB | ((unsigned int)g << 8);
				RGB = RGB | ((unsigned int)r << 16);

				*ibuffer = RGB;
				*/
				*pdepth = inverseZ;
			}

			ibuffer+=2;
			pdepth++;
			//lightValue += lghtXStep;
			textureCoordX += XXStep;
			textureCoordY += YXStep;
			inverseZ += ZXStep;
			//depthZ += ZXDepthStep;
		}

		leftX -= lefthandslope;
		rightX -= righthandslope;

		//lightStartLeft -= lightNextLineLeft;
		//lightStartRight -= lightNextLineRight;
		texCoordStartXLeft -= texCoordXNextLineLeft;
		texCoordStartYLeft -= texCoordYNextLineLeft;
		texCoordStartXRight -= texCoordXNextLineRight;
		texCoordStartYRight -= texCoordYNextLineRight;
		inverseZStartLeft -= inverseZNextLineLeft;
		inverseZStartRight -= inverseZNextLineRight;
		//depthZStartLeft -= depthZNextLineLeft;
		//depthZStartRight -= depthZNextLineRight;
	}

	//delete textureBuffer;
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

	topTexX = top.t.x;
	topTexY = top.t.y;
	midTexX = mid.t.x;
	midTexY = mid.t.y;
	botTexX = bot.t.x;
	botTexY = bot.t.y;

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

	lightXStep = gradientDcDx(saturate(top.n * directionalLight)*0.8f + 0.2f, saturate(mid.n * directionalLight)*0.8f + 0.2f, saturate(bot.n * directionalLight)*0.8f + 0.2f, top, mid, bot) * oneoverdx;
	lightYStep = gradientDcDy(saturate(top.n * directionalLight)*0.8f + 0.2f, saturate(mid.n * directionalLight)*0.8f + 0.2f, saturate(bot.n * directionalLight)*0.8f + 0.2f, top, mid, bot) * -oneoverdx;

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
	//light value
	lightStartLeft = saturate(top.n * directionalLight)*0.8f + 0.2f + (lightXStep * xPreStep) + (lightYStep * yPreStep);
	lightNextLineLeft = lightYStep + (lightXStep * lefthandslope);
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
	//light value
	lightStartRight = saturate(top.n * directionalLight)*0.8f + 0.2f + (lightXStep * xPreStep) + (lightYStep * yPreStep);
	lightNextLineRight = lightYStep + (lightXStep * righthandslope);
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
		//light value
		lightStartRight = saturate(mid.n * directionalLight)*0.8f + 0.2f + (lightXStep * xPreStep) + (lightYStep * yPreStep);
		lightNextLineRight = lightYStep + (lightXStep * righthandslope);
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
		//light value
		lightStartLeft = saturate(mid.n * directionalLight)*0.8f + 0.2f + (lightXStep * xPreStep) + (lightYStep * yPreStep);
		lightNextLineLeft = lightYStep + (lightXStep * lefthandslope);
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