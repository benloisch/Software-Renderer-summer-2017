#ifndef WIN32WINDOWBUFFER_H_
#define WIN32WINDOWBUFFER_H_

#include <Windows.h>
#include "BufferInterface.h"

class Win32WindowBuffer : public BufferInterface {
public:

	Win32WindowBuffer(int width, int height);

	inline Color_RGB GetPixelColor(int x, int y) {
		return Color_RGB(bytebuffer[(y * clientWidth + x) * 4 + 2],
						bytebuffer[(y * clientWidth + x) * 4 + 1],
						bytebuffer[(y * clientWidth + x) * 4]);
	}

	inline void SetPixelColor(int x, int y, float r, float g, float b) {
		bytebuffer[(y * clientWidth + x) * 4] = (unsigned char)b;
		bytebuffer[(y * clientWidth + x) * 4 + 1] = (unsigned char)g;
		bytebuffer[(y * clientWidth + x) * 4 + 2] = (unsigned char)r;
		bytebuffer[(y * clientWidth + x) * 4 + 3] = (unsigned char)0;
	}

	inline void FillBufferColor(float r, float g, float b) {
		for (int y = 0; y < clientHeight; y++)
		{
			for (int x = 0; x < clientWidth; x++)
			{
				bytebuffer[(y * clientWidth + x) * 4] = (unsigned char)b;
				bytebuffer[(y * clientWidth + x) * 4 + 1] = (unsigned char)g;
				bytebuffer[(y * clientWidth + x) * 4 + 2] = (unsigned char)r;
				bytebuffer[(y * clientWidth + x) * 4 + 3] = (unsigned char)0;
			}

			for (int i = 0; i < padding; i++) {
				bytebuffer[(y * clientWidth + clientWidth) * 4 + i] = (unsigned char)0;
			}
		}
	}

	bool initializeWindow(HINSTANCE instanceHandle, int show);

	inline void drawBuffer() {
		BitBlt(hdc, 0, 0, clientWidth, clientHeight, memHdc, 0, 0, SRCCOPY);
	}

	inline int GetHeight() {
		return clientHeight;
	}

	inline int GetWidth() {
		return clientWidth;
	}

	void setWindowHandle(HWND windowHandle);
	HWND getWindowHandle();

private:

	HWND windowHandle;

	int padding;
	int width;
	int height;

	//the border of the window hides some of the buffer,
	//so we create a buffer with slightly smaller width and height
	int clientWidth; //seeable width on window
	int clientHeight; //seeable height on window

	HDC hdc;
	HDC memHdc;
	HBITMAP buffer;
	char *bytebuffer;

};

#endif