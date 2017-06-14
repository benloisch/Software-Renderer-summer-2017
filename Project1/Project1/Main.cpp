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

POINT sMouse;
float diffx = 0;
float diffy = 0;

void checkRotation(float &xs, float &xz, Vector4D &vlook, Matrix4x4 cam)
{
	Matrix4x4 rot;

	float movementspeed = float(0.1);
	if (GetAsyncKeyState(0x57))
	{
		xz += movementspeed;
	}
	else if (GetAsyncKeyState(0x53))
	{
		xz -= movementspeed;
	}

	if (GetAsyncKeyState(0x41))
	{
		xs -= movementspeed;
	}
	else if (GetAsyncKeyState(0x44))
	{
		xs += movementspeed;
	}

	sMouse;
	POINT mouseNow;
	GetCursorPos(&mouseNow);

	if (mouseNow.x - sMouse.x > 0)
	{
		diffy += mouseNow.x - sMouse.x;
		rot.setYrot(diffy / 2);
		vlook = vlook * rot;
		vlook.normalize();
	}
	else if (mouseNow.x - sMouse.x < 0)
	{
		diffy += mouseNow.x - sMouse.x;
		rot.setYrot(diffy / 2);
		vlook = vlook * rot;
		vlook.normalize();
	}
	
	if (mouseNow.y - sMouse.y > 0)
	{
		diffx -= mouseNow.y - sMouse.y;
		rot.setRotArb(cam.m[0][0], cam.m[0][1], cam.m[0][2], diffx / 2);
		vlook = vlook * rot;
		vlook.normalize();
	}
	else if (mouseNow.y - sMouse.y < 0)
	{
		diffx -= mouseNow.y - sMouse.y;
		rot.setRotArb(cam.m[0][0], cam.m[0][1], cam.m[0][2], diffx / 2);
		vlook = vlook * rot;
		vlook.normalize();
	}
	
	sMouse = mouseNow;

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
	Win32WindowBuffer win32WindowBuffer(800, 600);
	if (!win32WindowBuffer.initializeWindow(hInstance, nShowCmd))
		return -1;

	MSG msg = { 0 };

	HighPerformanceCounter hpc;
	hpc.Start();

	unsigned char r = 0;

	//*******************************************Setup Level
	//load models and their textures
	//apply matrix transforms

	//*******************************************Setup camera
	//define camera position / rotation
	Camera cam;
	cam.setOriginPosition(0, 0, -10);
	cam.setLookDirection(0, 0, 1);
	cam.calculateViewMatrix(0, 0);

	//define projection matrix
	cam.setNearPlane(0.1);
	cam.setFarPlane(1000);
	cam.setAspectRatio(win32WindowBuffer.clientWidth, win32WindowBuffer.clientHeight);
	cam.setFieldOfView(70);
	cam.calculateProjectionMatrix();

	//*******************************************Setup input
	sMouse;
	SetCursorPos(win32WindowBuffer.clientWidth / 2, win32WindowBuffer.clientHeight / 2);
	GetCursorPos(&sMouse);

	//***********************************************************StarField3D
	
	//Vertex stars[100000];
	vector<Vertex> stars(10000);
	for (int i = 0; i < 10000; i++)
		stars[i] = Vertex(static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 10.0)) - 5.0, static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 10.0)) - 5.0, 0, 1);
	

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

		win32WindowBuffer.FillBufferColor(255, 255, 255);
		win32WindowBuffer.FillBufferColor(0, 0, 0);

		//********************************************************StarField3D
		
		//update stars
		double delta = hpc.mtimePerFrame;
		
		//for (int i = 0; i < 10000; i++) {
		//	stars[i].v.z -= delta * 10;
		//}

		//reset any stars that go out of screen (into negative z)
		for (int i = 0; i < 10000; i++) {
			if (stars[i].v.z <= 0) {
				//stars[i] = Vertex(rand() % 10 - 5, rand() % 10 - 5, 0, 1);
			}
		}
		
		//*********************************************************Get Mouse Input and move Camera accordingly

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
		



		float xs = 0;
		float xz = 0;
		checkRotation(xs, xz, cam.lookDirection, cam.viewMatrix);

		cam.calculateViewMatrix(xs, xz);
		SetCursorPos(win32WindowBuffer.clientWidth / 2, win32WindowBuffer.clientHeight / 2);
		cout << cam.viewMatrix.m[0][0] << ", " << cam.viewMatrix.m[0][1] << ", " << cam.viewMatrix.m[0][2] << endl;






		win32WindowBuffer.drawBuffer();

		hpc.Tick();
		displayFPS(hpc, *win32WindowBuffer.getWindowHandle());
		
	}

	return (int)msg.wParam;
}