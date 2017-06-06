#include <Windows.h>
#include <sstream>

#include <vector.h>
#include <matrix.h>

#include "Object.h"
#include "Renderer.h"
#include "High_Performance_Counter.h"

HWND mainWindowHandle = 0;
int width = 1366;
int height = 768;
int clientWidth = 0;
int clientHeight = 0;

HDC *hdc = NULL;
HDC *memHdc = NULL;

POINT sMouse;
float diffx = 0;
float diffy = 0;

bool test = true;

LRESULT CALLBACK WndProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
		
	//case WM_PAINT:
	//{
		/*
		HDC hdc = GetDC(mainWindowHandle);

		BitBlt(*hdc, 0, 0, clientWidth, clientHeight, *memHdc, 0, 0, SRCCOPY);

		DeleteDC(hdc);
		
		PAINTSTRUCT ps;
		HDC hdc;
		hdc = BeginPaint(mainWindowHandle, &ps);
		int window_width = ps.rcPaint.right - ps.rcPaint.left;
		int window_height = ps.rcPaint.bottom - ps.rcPaint.top;

		BITMAPINFO bmpinfo;
		bmpinfo.bmiHeader.biBitCount = 24;
		bmpinfo.bmiHeader.biClrImportant = 0;
		bmpinfo.bmiHeader.biClrUsed = 0;
		bmpinfo.bmiHeader.biCompression = BI_RGB;
		bmpinfo.bmiHeader.biHeight = -window_height;
		bmpinfo.bmiHeader.biWidth = window_width;
		bmpinfo.bmiHeader.biPlanes = 1;
		bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFO);
		bmpinfo.bmiHeader.biSizeImage = window_width * window_height * 3;
		bmpinfo.bmiHeader.biXPelsPerMeter = 0;
		bmpinfo.bmiHeader.biYPelsPerMeter = 0;

		HDC memHdc = CreateCompatibleDC(hdc);

		HBITMAP buffer;

		char *bytebuffer = NULL;

		buffer = CreateDIBSection(memHdc, &bmpinfo, DIB_RGB_COLORS, (void**)&bytebuffer, NULL, 0);
		SelectObject(memHdc, buffer);

		if (buffer == NULL)
		{
			MessageBox(0, L"HBITMAP buffer is null", 0, 0);
		}

		for (int i = 0; i < window_width * window_height * 3; i += 3)
		{
			bytebuffer[i] = 20;
			bytebuffer[i + 1] = 200;
			bytebuffer[i + 2] = 50;
		}

		BitBlt(hdc, 0, 0, window_width, window_height, memHdc, 0, 0, SRCCOPY);

		DeleteObject(buffer);
		DeleteDC(memHdc);
		EndPaint(mainWindowHandle, &ps);
		*/
	//	return 0;
	//}

	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(mainWindowHandle);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(windowHandle, msg, wParam, lParam);
}

bool initializeWindowApp(HINSTANCE instanceHandle, int show)
{
	WNDCLASS wc;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = instanceHandle;
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszMenuName = 0;
	wc.lpszClassName = L"MainWindowClass";

	if (!RegisterClass(&wc))
	{
		MessageBox(0, L"Failed to register window class", 0, 0);
	}

	mainWindowHandle = CreateWindow(
		L"MainWindowClass",
		L"Software Rasterizer",
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		width,
		height,
		0,
		0,
		instanceHandle,
		0);

	if (mainWindowHandle == 0)
	{
		MessageBox(0, L"CreateWindow() failed", 0, 0);
		return false;
	}

	ShowWindow(mainWindowHandle, show);
	UpdateWindow(mainWindowHandle);

	return true;
}

void displayFPS(Timer fpsTimer)
{
	static int frames;
	frames++;

	static double second;
	second += fpsTimer.mtimePerFrame;

	if (second >= 1.0)
	{
		std::wostringstream outs;
		outs.precision(6);
		outs << "FPS: " << frames << " MSPF: " << 1000 * fpsTimer.mtimePerFrame;
		SetWindowText(mainWindowHandle, outs.str().c_str());

		frames = 0;
		second = 0;
	}
}

void checkRotation(float &xs, float &xz, Vector &vlook, Matrix cam)
{
	Matrix rot;
 
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
	}
	else if (mouseNow.x - sMouse.x < 0)
	{
		diffy += mouseNow.x - sMouse.x;
		rot.setYrot(diffy / 2);
		vlook = vlook * rot;
	}

	if (mouseNow.y - sMouse.y > 0)
	{
		diffx -= mouseNow.y - sMouse.y;
		rot.setRotArb(cam.m[0][0], cam.m[0][1], cam.m[0][2], diffx / 2);
		vlook = vlook * rot;
	}
	else if (mouseNow.y - sMouse.y < 0)
	{
		diffx -= mouseNow.y - sMouse.y;
		rot.setRotArb(cam.m[0][0], cam.m[0][1], cam.m[0][2], diffx / 2);
		vlook = vlook * rot;
	}
	sMouse = mouseNow;

}

int run()
{
	MSG msg = { 0 };
	
	Timer fpstimer;
	fpstimer.Start();

	//create buffers
	RECT rect;
	GetClientRect(mainWindowHandle, &rect);
	clientWidth = rect.right - rect.left;
	clientHeight = rect.bottom - rect.top;

	//load in objects
	Object obj(1024, 1024);
	obj.loadData("Earth.obj");
	obj.loadTexture(L"Earth.bmp");
	obj.matrix.setTranslate(0, 0, 0);

	Renderer renderer(clientWidth, clientHeight, &mainWindowHandle);
	renderer.setViewMatrix(0, 0, 0, 0, 1);
	renderer.view.m[3][2] = -3;
	renderer.setProjectionMatrix(65, float(clientWidth) / float(clientHeight), 1.0, 10.0);
	renderer.onlyLines = true;

	hdc = &renderer.hdc;
	memHdc = &renderer.memHdc;

	//load in objects into main rendering class through a pointer
	renderer.object = &obj;
	
	//vector used to control where camera looks
	Vector lookat(0, 0, 1);
	sMouse;
	SetCursorPos(clientWidth / 2, clientHeight / 2);
	GetCursorPos(&sMouse);

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			if (GetAsyncKeyState(0x20))
			{
				if (test)
					test = false;
				else
					test = true;
				GetCursorPos(&sMouse);
			}

			if (test)
			{
				float xs = 0;
				float xz = 0;
				//checkRotation(xs, xz, lookat, renderer.view);
				//renderer.setViewMatrix(xs, xz, float(lookat.x), float(lookat.y), float(lookat.z));
				//SetCursorPos(clientWidth / 2, clientHeight / 2);
			}
			renderer.draw();
			fpstimer.Tick();
			displayFPS(fpstimer);
			//RedrawWindow(mainWindowHandle, 0, 0, RDW_INVALIDATE | RDW_UPDATENOW);

			BitBlt(*hdc, 0, 0, clientWidth, clientHeight, *memHdc, 0, 0, SRCCOPY);
		}
	}

	return (int)msg.wParam;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int nShowCmd)
{
	if (!initializeWindowApp(hInstance, nShowCmd))
		return 0;

	return run();
}