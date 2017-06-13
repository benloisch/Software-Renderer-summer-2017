#include <Windows.h>
#include <sstream>
#include <math.h>

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

//void testFunc(Win32WindowBuffer *win32buf) {
//	win32buf->FillBufferColor(255, 0, 255);
//}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{
	Win32WindowBuffer win32WindowBuffer(1920, 1080);
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
	cam.setOriginPosition(0, 0, 0);
	cam.setLookDirection(0, 0, 1);
	cam.calculateViewMatrix();

	//define projection matrix
	cam.setNearPlane(0.1);
	cam.setFarPlane(100);
	cam.setAspectRatio(win32WindowBuffer.clientWidth, win32WindowBuffer.clientHeight);
	cam.setFieldOfView(70);
	cam.calculateProjectionMatrix();

	//StarField3D
	Vertex stars[10000];
	for (int i = 0; i < 10000; i++)
		stars[i] = Vertex((rand() % 200) - 100, (rand() % 200) - 100, (rand() % 100) + 1, 1);



	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}



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

		//********************************************************StarField
		//update stars
		float delta = hpc.mtimePerFrame;
		
		for (int i = 0; i < 10000; i++) {
			stars[i].v.z -= delta * 10;
		}

		//reset any stars that go out of screen (into negative z)
		for (int i = 0; i < 10000; i++) {
			if (stars[i].v.z <= 0) {
				stars[i] = Vertex((rand() % 200) - 100, (rand() % 200) - 100, (rand() % 100) + 1, 1);
			}
		}

		Matrix4x4 viewProjection = cam.projectionMatrix;// cam.viewMatrix/*.inverse()*/ * cam.projectionMatrix;

		//project verticies, divide by z (w), transform from NDC to screen space, finally draw 
		for (int i = 0; i < 10000; i++) {
			Vertex v = (stars[i].v * viewProjection);
			v.v /= v.v.w;

			if (v.v.x <= -1.0 || v.v.x >= 1.0 || v.v.y <= -1.0 || v.v.y >= 1.0)
				continue;

			int screenX = (v.v.x + 1) * 0.5 * win32WindowBuffer.clientWidth;
			int screenY = (v.v.y + 1) * 0.5 * win32WindowBuffer.clientHeight;

			char *pixelComponent = win32WindowBuffer.bytebuffer + ((screenY * win32WindowBuffer.clientWidth + screenX) * 4);
			*(pixelComponent) = (unsigned char)255;
			*(pixelComponent + 1) = (unsigned char)255;
			*(pixelComponent + 2) = (unsigned char)255;
			*(pixelComponent + 3) = (unsigned char)0;
		}

		win32WindowBuffer.drawBuffer();

		hpc.Tick();
		displayFPS(hpc, *win32WindowBuffer.getWindowHandle());
		
	}

	return (int)msg.wParam;
}