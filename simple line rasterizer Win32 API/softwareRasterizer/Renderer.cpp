#include "Renderer.h"

Renderer::Renderer(int width, int height, HWND *mainWindowHandle)
{
	onlyLines = true;

	rwidth = width;
	rheight = height;

	depthBuffer = new float[width * height];

	hdc = GetDC(*mainWindowHandle);

	BITMAPINFO bmpinfo;
	bmpinfo.bmiHeader.biBitCount = 24;
	bmpinfo.bmiHeader.biClrImportant = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biHeight = -height;
	bmpinfo.bmiHeader.biWidth = width;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmpinfo.bmiHeader.biSizeImage = width * height * 3;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;

	memHdc = CreateCompatibleDC(hdc);

	buffer = CreateDIBSection(memHdc, &bmpinfo, DIB_RGB_COLORS, (void**)&bytebuffer, NULL, 0);
	SelectObject(memHdc, buffer);

	if (buffer == NULL)
	{
		MessageBox(0, L"HBITMAP buffer is null", 0, 0);
	}
}

Renderer::~Renderer()
{
	delete depthBuffer;

	DeleteObject(buffer);
	DeleteDC(memHdc);
	DeleteDC(hdc);
}

void Renderer::setProjectionMatrix(float fov, float aspectRatio, float nearPlane, float farPlane)
{
	fov = (float(fov) * float(3.1415926535)) / float(180.0);

	projection.m[0][0] = 1.0 / (aspectRatio * tan(fov / 2));
	projection.m[1][1] = 1.0 / tan(fov / 2);
	projection.m[2][2] = farPlane / (farPlane - nearPlane);
	projection.m[3][2] = (-1 * nearPlane * farPlane) / (farPlane - nearPlane);
	projection.m[2][3] = 1.0;
	projection.m[3][3] = 0.0;
}

void Renderer::setViewMatrix(float xs, float zs, float xlook, float ylook, float zlook)
{
	Vector worldUp(0, 1, 0);

	Vector forward(xlook, ylook, zlook);
	Vector right;
	Vector up;

	right = worldUp * forward;
	up = forward * right;

	right.normalize();
	up.normalize();
	forward.normalize();

	view.setRow1(right.x, right.y, right.z, 0);
	view.setRow2(up.x, up.y, up.z, 0);
	view.setRow3(forward.x, forward.y, forward.z, 0);

	Vector trans;
	trans = trans + (right * xs);
	trans = trans + (forward * zs);
	view.setTranslate(view.m[3][0] + trans.x, view.m[3][1] + trans.y, view.m[3][2] + trans.z);
}

void Renderer::draw()
{
	//blank out the backbuffer
	for (int y = 0; y < rheight; y++)
	{
		for (int x = 0; x < rwidth; x++)
		{
			bytebuffer[(y * rwidth + x) * 3] = 20;//object->textureRGB[(y * object->textureWidth + x) * 3];//blue
			bytebuffer[(y * rwidth + x) * 3 + 1] = 20;//object->textureRGB[(y * object->textureWidth + x) * 3 + 1];//green
			bytebuffer[(y * rwidth + x) * 3 + 2] = 20;//object->textureRGB[(y * object->textureWidth + x) * 3 + 2];//red
		}
	}

	//set depth buffer entries to 1.0f
	for (int i = 0; i < rwidth * rheight; i++)
	{
		depthBuffer[i] = 1.0f;
	}

	//now work your way through the stages of the rendering pipeline
	for (unsigned int i = 0; i < object->faces.size() - 3; i += 3)
	{
		//input one face, which defines 3 verticies, vertex normals, and texture coordinates
		
		vector<Vector> vertexSet;
		vertexSet.push_back(object->verticies[unsigned int(object->faces[i].x - 1)]);
		vertexSet.push_back(object->verticies[unsigned int(object->faces[i + 1].x - 1)]);
		vertexSet.push_back(object->verticies[unsigned int(object->faces[i + 2].x - 1)]);
		
		

		//first test in function, order of verticies and clip if face normal . z of camera =< 0
		bool cullTest = backfaceCulling(vertexSet);
		if (cullTest)
			continue;

		//verticies * object matrix * view * projection = leaves it in homogeneous clip space
		vertexShader(vertexSet);

		//hard step! clip the three verticies against viewing frustum in homogeneous space
		if (!geometryShader(vertexSet))
			continue;

		//if onlyLines is on, render a wireframe, no textures or shading or shadows
		if (onlyLines)
		{
			rasterOnlyLines(vertexSet);
			continue;
		}

		//rasterize()
		//hard step! interpolate uv coordinates, vertex normals, and depth values across face of triangle, 

		//pixel shader()
		//shade pixels by plucking out interpolated values, performing lighting calculations, and replacing previous pixels if depth value is less than it
	}
}

bool Renderer::backfaceCulling(vector<Vector> v)
{
	//dot product of camera.forward and (cross of v2-v1 * v3-v1)
	Vector camForward(view.m[2][0], view.m[2][1], view.m[2][2], 0);
	Vector polyFaceNormal((v[2] - v[0]) * (v[1] - v[0]));
	if ((camForward ^ polyFaceNormal) >= 0)
		return false;

	return true;
}

void Renderer::vertexShader(vector<Vector> &v)
{
	Matrix wvp = object->matrix * view.inverse() * projection;

	v[0] = v[0] * wvp;
	v[1] = v[1] * wvp;
	v[2] = v[2] * wvp;
}

bool Renderer::geometryShader(vector<Vector> &v)
{
	//clip in homogeneous space

	/*
	test every segment: v1v2, v1v3, and v2v3

	if endpoints are both inside, don't do anything
	if endponits are both outside, don't do anything

	if one is inside, and one is outside, clip against plane and record t

	
	
	
	
	*/


	//after clipping is done, project on w plane and thus the coordinates will be in NDC coordinate space
	for (unsigned int i = 0; i < v.size(); i++)
	{
		if (v[i].x < -v[i].w || v[i].x > v[i].w)
			return false;
		if (v[i].y < -v[i].w || v[i].y > v[i].w)
			return false;
		if (v[i].z < 0.01 || v[i].z > v[i].w)
			return false;

		v[i] = v[i] / v[i].w;
	}

	return true;
}

void Renderer::drawLine(float x1, float y1, float x2, float y2)
{
	float xdiff = (x2 - x1);
	float ydiff = (y2 - y1);

	if (xdiff == 0.0f && ydiff == 0.0f)
	{
		SetPixelWhite(x1, y1);
		return;
	}

	if (fabs(xdiff) > fabs(ydiff))
	{
		float xmin, xmax;

		if (x1 < x2)
		{
			xmin = x1;
			xmax = x2;
		}
		else
		{
			xmin = x2;
			xmax = x1;
		}

		float slope = ydiff / xdiff;
		for (float x = xmin; x <= xmax; x += 1.0f) {
			float y = y1 + ((x - x1) * slope);
			SetPixelWhite(x, y);
		}
	}
	else
	{
		float ymin, ymax;

		// set ymin to the lower y value given
		// and ymax to the higher value
		if (y1 < y2) {
			ymin = y1;
			ymax = y2;
		}
		else {
			ymin = y2;
			ymax = y1;
		}

		// draw line in terms of x slope
		float slope = xdiff / ydiff;
		for (float y = ymin; y <= ymax; y += 1.0f) {
			float x = x1 + ((y - y1) * slope);
			SetPixelWhite(x, y);
		}
	}
}

void Renderer::SetPixelWhite(int x, int y)
{
	bytebuffer[(y * rwidth + x) * 3] = 200;
	bytebuffer[(y * rwidth + x) * 3 + 1] = 200;
	bytebuffer[(y * rwidth + x) * 3 + 2] = 200;
}

void Renderer::rasterOnlyLines(vector<Vector> v)
{
	int xWin1 = (v[0].x + 1) * rwidth * 0.5;
	int yWin1 = (v[0].y + 1) * rheight * 0.5;

	int xWin2 = (v[1].x + 1) * rwidth * 0.5;
	int yWin2 = (v[1].y + 1) * rheight * 0.5;

	int xWin3 = (v[2].x + 1) * rwidth * 0.5;
	int yWin3 = (v[2].y + 1) * rheight * 0.5;

	drawLine(xWin1, yWin1, xWin2, yWin2);
	drawLine(xWin2, yWin2, xWin3, yWin3);
	drawLine(xWin3, yWin3, xWin1, yWin1);

	/*
	for (int i = 0; i < v.size(); i += 3)
	{
		int xWin = (v[i].x + 1) * rwidth * 0.5;
		int yWin = (v[i].y + 1) * rheight * 0.5;



		//bytebuffer[(yWin * rwidth + xWin) * 3] = 200;
		//bytebuffer[(yWin * rwidth + xWin) * 3 + 1] = 200;
		//bytebuffer[(yWin * rwidth + xWin) * 3 + 2] = 200;
	}
	*/
}