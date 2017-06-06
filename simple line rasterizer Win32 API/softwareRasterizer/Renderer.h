#ifndef RENDERER_H_
#define RENDERER_H_

#include "Object.h"
#include <matrix.h>
#include <vector.h>

class Renderer
{
public:
	
	bool onlyLines;

	int rwidth;
	int rheight;

	float *depthBuffer;

	Matrix view;
	Matrix projection;

	Object *object;

	HDC hdc;
	HDC memHdc;
	HBITMAP buffer;
	char *bytebuffer;

	Renderer(int, int, HWND *mainWindowHandle);
	~Renderer();


	void setProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane);
	void setViewMatrix(float xs, float zs, float xlook, float ylook, float zlook);
	void draw();

	bool backfaceCulling(vector<Vector> v);
	void vertexShader(vector<Vector> &v);
	bool geometryShader(vector<Vector> &v);
	void rasterOnlyLines(vector<Vector> v);
	void drawLine(float x1, float y1, float x2, float y2);
	void SetPixelWhite(int x, int y);

};

#endif