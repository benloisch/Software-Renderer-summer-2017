#ifndef WIN32WINDOWBUFFER_H_
#define WIN32WINDOWBUFFER_H_

#include <Windows.h>
#include <mutex>
#include "BufferInterface.h"

using namespace std;

class Win32WindowBuffer {
public:

	Win32WindowBuffer(int width, int height);

	/*
	inline Color_RGB GetPixelColor(int x, int y) {
		return Color_RGB(bytebuffer[(y * clientWidth + x) * 4 + 2],
						bytebuffer[(y * clientWidth + x) * 4 + 1],
						bytebuffer[(y * clientWidth + x) * 4]);
	}
	*/

	/*
	inline void SetPixelColor(int x, int y, float r, float g, float b) {
		char *pixelComponent = bytebuffer + ((y * clientWidth + x) * 4);
		*(pixelComponent) = (unsigned char)b;
		*(pixelComponent + 1) = (unsigned char)g;
		*(pixelComponent + 2) = (unsigned char)r;
		*(pixelComponent + 3) = (unsigned char)0;
	}
	*/

	void FillBufferColor(float r, float g, float b);

	bool initializeWindow(HINSTANCE instanceHandle, int show);

	inline void drawBuffer() {
		BitBlt(hdc, 0, 0, clientWidth, clientHeight, memHdc, 0, 0, SRCCOPY);
	}

	/*
	inline int GetHeight() {
		return height;
	}

	inline int GetWidth() {
		return width;
	}

	inline int GetClientHeight() {
		return clientHeight;
	}

	inline int GetClientWidth() {
		return clientWidth;
	}
	*/

	void setWindowHandle(HWND *windowHandle);
	HWND* getWindowHandle();

	//keep these variables public because function getters are too slow
	char *bytebuffer;
	int padding;
	int width;
	int height;

	//the border of the window hides some of the buffer,
	//so we create a buffer with slightly smaller width and height
	int clientWidth; //seeable width on window
	int clientHeight; //seeable height on window

	//int startBuffer;
	//int endBuffer;
	//int threads;
	//int threadIndex;

private:

	HWND windowHandle;

	HDC hdc;
	HDC memHdc;
	HBITMAP buffer;
	//char *bytebuffer;

};

#endif