#include "Win32WindowBuffer.h"

//mutex mut;

LRESULT CALLBACK Win32WindowBuffer::WndProc(HWND windowHandle, UINT msg, WPARAM wParam, LPARAM lParam)
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

bool Win32WindowBuffer::initializeWindowApp(HINSTANCE instanceHandle, int show, Win32WindowBuffer *win32buf)
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

	HWND *handle = new HWND;
	*handle = NULL;
		
	*handle = CreateWindow(
	L"MainWindowClass",
	L"Software Rendering",
	WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	win32buf->width,
	win32buf->height,
	0,
	0,
	instanceHandle,
	0);

	win32buf->setWindowHandle(handle);

	if (win32buf->getWindowHandle() == 0)
	{
		MessageBox(0, L"CreateWindow() failed", 0, 0);
		return false;
	}

	ShowWindow(*win32buf->getWindowHandle(), show);
	UpdateWindow(*win32buf->getWindowHandle());

	return true;
}

Win32WindowBuffer::Win32WindowBuffer(int width, int height) {
	windowHandle = 0;

	this->width = width;
	this->height = height;

	clientHeight = 0;
	clientWidth = 0;

	hdc = NULL;
	memHdc = NULL;
	buffer = NULL;
	bytebuffer = NULL;
	padding = 0;

	//startBuffer = 0;
	//endBuffer = 0;
	//this->threads = threads;
	//threadIndex = 0;
}

bool Win32WindowBuffer::initializeWindow(HINSTANCE instanceHandle, int show) {

	if(!(initializeWindowApp(instanceHandle, show, this)))
		return 0;

	RECT rect;
	GetClientRect(*this->getWindowHandle(), &rect);
	clientWidth = rect.right - rect.left;
	clientHeight = rect.bottom - rect.top;

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
		return 0;
	}

	return 1;
}

HWND* Win32WindowBuffer::getWindowHandle() {
	return &windowHandle;
}

void Win32WindowBuffer::setWindowHandle(HWND *handle) {
	this->windowHandle = *handle;
}

void Win32WindowBuffer::FillBufferColor(float r, float g, float b) {
	for (int y = 0; y < clientHeight; y++)
	{
		for (int x = 0; x < clientWidth; x++)
		{
			char *pixelComponent = bytebuffer + ((y * clientWidth + x) * 4);
			*(pixelComponent) = (unsigned char)b;
			*(pixelComponent + 1) = (unsigned char)g;
			*(pixelComponent + 2) = (unsigned char)r;
			*(pixelComponent + 3) = (unsigned char)0;
		}

		char *pixelComponent = bytebuffer + ((y * clientWidth + clientWidth) * 4);
		for (int i = 0; i < padding; i++) {
			bytebuffer[(y * clientWidth + clientWidth) * 4 + i] = (unsigned char)0;
		}
	}
}

/*
void Win32WindowBuffer::FillBufferColor(float r, float g, float b) {
	mut.lock();
	threadIndex = 0;
	if (threadIndex >= threads)
		threadIndex = 0;
	mut.unlock();

	int fractionOfHeight = clientHeight / threads;

	b = (float)threadIndex / (float)threads * 254;

	startBuffer = threadIndex * fractionOfHeight;
	endBuffer = (threadIndex * fractionOfHeight) + fractionOfHeight;

	if (threadIndex == threads - 1)
		endBuffer = clientHeight;

	for (int y = startBuffer; y < endBuffer; y++)
	{
		for (int x = 0; x < clientWidth; x++)
		{
			char *pixelComponent = bytebuffer + ((y * clientWidth + x) * 4);
			*(pixelComponent) = (unsigned char)b;
			*(pixelComponent + 1) = (unsigned char)g;
			*(pixelComponent + 2) = (unsigned char)r;
			*(pixelComponent + 3) = (unsigned char)0;
		}

		char *pixelComponent = bytebuffer + ((y * clientWidth + clientWidth) * 4);
		for (int i = 0; i < padding; i++) {
			//bytebuffer[(y * clientWidth + clientWidth) * 4 + i] = (unsigned char)0;
			*(pixelComponent + i) = (unsigned char)0;
		}
	}
	
	mut.lock();
	threadIndex++;
	mut.unlock();
}
*/