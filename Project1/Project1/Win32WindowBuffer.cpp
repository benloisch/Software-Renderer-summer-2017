#include "Win32WindowBuffer.h"

LRESULT CALLBACK WndProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
			DestroyWindow(windowHandle);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(windowHandle, msg, wParam, lParam);
}

bool initializeWindowApp(HINSTANCE instanceHandle, int show, Win32WindowBuffer win32buf)
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

	win32buf.setWindowHandle(CreateWindow(
		L"MainWindowClass",
		L"Software Rasterizer",
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		win32buf.GetWidth(),
		win32buf.GetHeight(),
		0,
		0,
		instanceHandle,
		0));

	if (win32buf.getWindowHandle() == 0)
	{
		MessageBox(0, L"CreateWindow() failed", 0, 0);
		return false;
	}

	ShowWindow(win32buf.getWindowHandle(), show);
	UpdateWindow(win32buf.getWindowHandle());

	return true;
}

Win32WindowBuffer::Win32WindowBuffer(int width, int height) {
	windowHandle = 0;
	
	RECT rect;
	GetClientRect(windowHandle, &rect);
	clientWidth = rect.right - rect.left;
	clientHeight = rect.bottom - rect.top;

	hdc = NULL;
	memHdc = NULL;
	buffer = NULL;
	bytebuffer = NULL;
	padding = 0;
}

bool Win32WindowBuffer::initializeWindow(HINSTANCE instanceHandle, int show) {

	if(!(initializeWindowApp(instanceHandle, show, *this)))
		return 0;

	hdc = GetDC(windowHandle);

	padding = 0;
	if ((clientWidth * 3) % 4 != 0) {
		padding = 4 - ((clientWidth * 3) % 4);
	}

	BITMAPINFO bmpinfo;
	bmpinfo.bmiHeader.biBitCount = 32;
	bmpinfo.bmiHeader.biClrImportant = 0;
	bmpinfo.bmiHeader.biClrUsed = 0;
	bmpinfo.bmiHeader.biCompression = BI_RGB;
	bmpinfo.bmiHeader.biHeight = clientHeight;
	bmpinfo.bmiHeader.biWidth = clientWidth;
	bmpinfo.bmiHeader.biPlanes = 1;
	bmpinfo.bmiHeader.biSize = sizeof(BITMAPINFO);
	bmpinfo.bmiHeader.biSizeImage = 0;//((width * 3) + pad) * height;// width * height * 3;
	bmpinfo.bmiHeader.biXPelsPerMeter = 0;
	bmpinfo.bmiHeader.biYPelsPerMeter = 0;

	memHdc = CreateCompatibleDC(hdc);

	buffer = CreateDIBSection(memHdc, &bmpinfo, DIB_RGB_COLORS, (void**)&bytebuffer, NULL, 0);
	SelectObject(memHdc, buffer);

	if (buffer == NULL)
	{
		MessageBox(0, L"HBITMAP buffer is null", 0, 0);
	}

	return 1;
}

HWND Win32WindowBuffer::getWindowHandle() {
	return windowHandle;
}

void Win32WindowBuffer::setWindowHandle(HWND handle) {
	this->windowHandle = handle;
}