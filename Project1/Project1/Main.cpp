#include <Windows.h>
#include <sstream>
#include <math.h>
#include <iostream>
#include <vector>

//#include <thread>

using namespace std;

#include "High_Performance_Counter.h"
#include "Win32WindowBuffer.h"
//#include "BufferInterface.h"
#include "Matrix4x4.h"
#include "Vector4D.h"
#include "Vertex.h"
#include "Camera.h"
#include "Mesh.h"
#include "Pipeline.h"

void displayFPS(HighPerformanceCounter timer, HWND windowHandle)
{
	static int frames;
	frames++;

	static double second;
	second += timer.mtimePerFrame;

	if (second >= 1.0)
	{
		std::wostringstream outs;
		outs.precision(6);
		outs << "FPS: " << frames << " MSPF: " << 1000 * timer.mtimePerFrame;
		SetWindowText(windowHandle, outs.str().c_str());

		frames = 0;
		second = 0;
	}
}

//********************************************************multithreading attempt
/*void testFunc(Win32WindowBuffer *win32buf) {
	win32buf->FillBufferColor(255, 0, 255);
}*/

//call main so we can create a console for output
int main() {
	return WinMain(GetModuleHandle(NULL), NULL, NULL, SW_SHOW);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{
	Win32WindowBuffer win32WindowBuffer(1366, 768);
	if (!win32WindowBuffer.initializeWindow(hInstance, nShowCmd))
		return -1;

	MSG msg = { 0 };

	HighPerformanceCounter hpc;
	hpc.Start();

	//*******************************************Setup Level
	//load models and their textures
	//apply matrix transforms

	//*******************************************Setup camera
	//define camera position / rotation
	Camera cam;
	cam.setOriginPosition(0, 0, 0);
	cam.setLookDirection(0, 0, 1);
	cam.calculateViewMatrix();

	//define projection matrix
	cam.setNearPlane(0.1);
	cam.setFarPlane(1000);
	cam.setAspectRatio(win32WindowBuffer.clientWidth, win32WindowBuffer.clientHeight);
	cam.setFieldOfView(60);
	cam.calculateProjectionMatrix();

	//*******************************************Setup input
	//ShowCursor(false);
	SetCursorPos(win32WindowBuffer.clientWidth / 2, win32WindowBuffer.clientHeight / 2);
	GetCursorPos(&cam.sMouse);

	//*******************************************Setup pipeline
	Pipeline pipeline;
	pipeline.setCamera(&cam);
	pipeline.setScreenBuffer(win32WindowBuffer.bytebuffer);

	Mesh mesh;

	//top left triangle
	mesh.verticies.push_back(Vertex(-1.0, -1.0, 0, 1.0));
	mesh.verticies.push_back(Vertex(-1.0, 1.0, 0, 1.0));
	mesh.verticies.push_back(Vertex(1.0, 1.0, 0, 1.0));
	
	//bottom right triangle
	mesh.verticies.push_back(Vertex(1.0, -1.0, 0, 1.0));
	mesh.verticies.push_back(Vertex(-1.0, -1.0, 0, 1.0));
	mesh.verticies.push_back(Vertex(1.0, 1.0, 0, 1.0));

	/*
	for (int i = 0; i < 5000; i++) {
		mesh.verticies.push_back(Vertex(-1.0, -1.0, i, 1.0));
		mesh.verticies.push_back(Vertex(-1.0, 1.0, i, 1.0));
		mesh.verticies.push_back(Vertex(1.0, 1.0, i, 1.0));

		//bottom right triangle
		mesh.verticies.push_back(Vertex(1.0, -1.0, i, 1.0));
		mesh.verticies.push_back(Vertex(-1.0, -1.0, i, 1.0));
		mesh.verticies.push_back(Vertex(1.0, 1.0, i, 1.0));
	}
	*/
	
	mesh.texture = new ImageBMP;
	

	//***********************************************************Old StarField3D code
	/*
	//Vertex stars[100000];
	vector<Vertex> stars(10000);
	for (int i = 0; i < 10000; i++)
		stars[i] = Vertex(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 10.0)) - 5.0, static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 10.0)) - 5.0, 0, 1);
	*/

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		//********************************************************multithreading draw buffer attempt
		/*
		thread funcTest(&Win32WindowBuffer::FillBufferColor, &win32WindowBuffer, 0, 0, 0);
		funcTest.join();

		thread funcTest2(&Win32WindowBuffer::FillBufferColor, &win32WindowBuffer, 100, 100, 0);
		funcTest2.join();

		thread funcTest3(&Win32WindowBuffer::FillBufferColor, &win32WindowBuffer, 255, 0, 0);
		funcTest3.join();

		thread funcTest4(&Win32WindowBuffer::FillBufferColor, &win32WindowBuffer, 0, 255, 0);
		funcTest4.join();


		//win32WindowBuffer.FillBufferColor(255, 0, 255);

		for (int y = 0; y < win32WindowBuffer.clientHeight; y++) {
		for (int x = 0; x < win32WindowBuffer.clientWidth; x++) {
		char *pixelComponent = win32WindowBuffer.bytebuffer + ((y * win32WindowBuffer.clientWidth + x) * 4);
		*(pixelComponent) = (unsigned char)255;
		*(pixelComponent + 1) = (unsigned char)255;
		*(pixelComponent + 2) = (unsigned char)255;
		*(pixelComponent + 3) = (unsigned char)0;
		}
		}


		//400
		//100
		//50
		//37
		//28
		//23

		//win32WindowBuffer.FillBufferColor(0, 255, 255);
		//win32WindowBuffer.FillBufferColor(0, 255, 255);
		//win32WindowBuffer.FillBufferColor(0, 255, 255);
		//win32WindowBuffer.FillBufferColor(0, 255, 255);
		//win32WindowBuffer.FillBufferColor(0, 255, 255);
		//win32WindowBuffer.FillBufferColor(0, 255, 255);
		*/

		//********************************************************Old StarField3D code
		/*
		//update stars
		//double delta = hpc.mtimePerFrame;

		//for (int i = 0; i < 10000; i++) {
		//	stars[i].v.z -= delta * 10;
		//}

		//reset any stars that go out of screen (into negative z)
		for (int j = 0; j < 1; j++) {
			for (int i = 0; i < 10000; i++) {
				if (stars[i].v.z <= 0) {
					//stars[i] = Vertex(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 10.0)) - 5.0, static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 10.0)) - 5.0, 0, 1);
				}
			}
		}
		*/

		//***********************************************************Old starfield code
		/*
		static double rot;
		rot += delta * 100;

		//Matrix4x4 modelRotate;
		//modelRotate.setYrot(rot);
		//modelRotate.setRotArb(-1, 0, 0, rot);
		//Matrix4x4 modelTranslate;
		//modelTranslate.setTranslate(0, -200, 300);
		Matrix4x4 model;// = modelRotate * modelTranslate;

		Matrix4x4 viewProjection = model * cam.viewMatrix.inverse() * cam.projectionMatrix;

		//project verticies, divide by z (w), transform from NDC to screen space, finally draw 
		for (int i = 0; i < 10000; i++) {
			Vertex v = (stars[i].v * viewProjection);
			v.v /= v.v.w;

			if (v.v.x <= -1.0 || v.v.x >= 1.0 || v.v.y <= -1.0 || v.v.y >= 1.0 || v.v.z <= 0 || v.v.z >= 1)
				continue;

			int screenX = (int)((v.v.x + 1) * 0.5 * win32WindowBuffer.clientWidth);
			int screenY = (int)((v.v.y + 1) * 0.5 * win32WindowBuffer.clientHeight);

			char *pixelComponent = win32WindowBuffer.bytebuffer + ((screenY * win32WindowBuffer.clientWidth + screenX) * 4);
			*(pixelComponent) = (unsigned char)255;
			*(pixelComponent + 1) = (unsigned char)255;
			*(pixelComponent + 2) = (unsigned char)255;
			*(pixelComponent + 3) = (unsigned char)0;
		}
		*/

		//*********************************************************Get delta (time spent rendering a single frame in miliseconds)
		double delta = hpc.mtimePerFrame;
		
		static double rot = 0;
		rot += delta;

		mesh.modelMesh.setYrot(rot * 10);
		mesh.modelMesh.setTranslate(0, 0, 5);

		win32WindowBuffer.FillBufferColor(255, 255, 255);
		win32WindowBuffer.FillBufferColor(0, 0, 0);

		//*********************************************************Get Mouse Input and move Camera
		cam.getInput(delta); //delta used to smooth mouse relative to how fast framerate is
		cam.calculateViewMatrix(); //after rotating lookDirection vector, recalculate camera matrix
		pipeline.setCamera(&cam);

		//**********************************************************Render mesh objects

		pipeline.transform(mesh);

		//pipeline.transform verticies
			//perform z-culling first
			//project, (don't divide by w)
		//pipeline.clip verticies
			//check if entire triangle lies outside viewing frustum
			//clip in homogenous clip space
		//pipeline. shade triangle (model, lights)
			//sort verticies
			//generate vertex, 1/z depth, z depth, lighting, and texture gradients
			//step down (y axis) scan line, draw from left to right
			//interpolate the textures, 1/z depth, z depth, and lighting





		win32WindowBuffer.drawBuffer();

		hpc.Tick();
		//if (hpc.mtimePerFrame > 0.033)
			//Sleep(1000.0 * (hpc.mtimePerFrame - 0.033));
		displayFPS(hpc, *win32WindowBuffer.getWindowHandle());
		
	}

	return (int)msg.wParam;
}