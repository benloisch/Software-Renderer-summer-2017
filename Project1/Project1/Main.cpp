#include <Windows.h>
#include <sstream>

#include <thread>

using namespace std;

#include "High_Performance_Counter.h"
#include "Win32WindowBuffer.h"
//#include "BufferInterface.h"

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

void testFunc(Win32WindowBuffer *win32buf) {
	win32buf->FillBufferColor(255, 0, 255);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{
	Win32WindowBuffer win32WindowBuffer(1920, 1080);
	if (!win32WindowBuffer.initializeWindow(hInstance, nShowCmd))
		return -1;

	MSG msg = { 0 };

	HighPerformanceCounter hpc;
	hpc.Start();

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

		win32WindowBuffer.FillBufferColor(0, 0, 0);
		win32WindowBuffer.FillBufferColor(255, 255, 255);

		win32WindowBuffer.drawBuffer();

		hpc.Tick();
		displayFPS(hpc, *win32WindowBuffer.getWindowHandle());
		
	}

	return (int)msg.wParam;
}