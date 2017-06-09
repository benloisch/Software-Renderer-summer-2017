#ifndef WIN32WINDOWBUFFER_H_
#define WIN32WINDOWBUFFER_H_

#include <Windows.h>
<<<<<<< HEAD
#include <mutex>
#include "BufferInterface.h"

using namespace std;

class Win32WindowBuffer {
=======
#include "BufferInterface.h"

class Win32WindowBuffer : public BufferInterface {
>>>>>>> 02b1ec391f0386c09dee4158af7074c40fa1e51b
public:

	Win32WindowBuffer(int width, int height);

<<<<<<< HEAD
	/*
=======
>>>>>>> 02b1ec391f0386c09dee4158af7074c40fa1e51b
	inline Color_RGB GetPixelColor(int x, int y) {
		return Color_RGB(bytebuffer[(y * clientWidth + x) * 4 + 2],
						bytebuffer[(y * clientWidth + x) * 4 + 1],
						bytebuffer[(y * clientWidth + x) * 4]);
	}
<<<<<<< HEAD
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
=======

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
>>>>>>> 02b1ec391f0386c09dee4158af7074c40fa1e51b

	bool initializeWindow(HINSTANCE instanceHandle, int show);

	inline void drawBuffer() {
		BitBlt(hdc, 0, 0, clientWidth, clientHeight, memHdc, 0, 0, SRCCOPY);
	}

<<<<<<< HEAD
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
=======
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

>>>>>>> 02b1ec391f0386c09dee4158af7074c40fa1e51b
	int padding;
	int width;
	int height;

	//the border of the window hides some of the buffer,
	//so we create a buffer with slightly smaller width and height
	int clientWidth; //seeable width on window
	int clientHeight; //seeable height on window

<<<<<<< HEAD
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
=======
	HDC hdc;
	HDC memHdc;
	HBITMAP buffer;
	char *bytebuffer;
>>>>>>> 02b1ec391f0386c09dee4158af7074c40fa1e51b

};

#endif