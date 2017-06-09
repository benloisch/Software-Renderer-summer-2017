#ifndef BUFFERINTERFACE_H_
#define BUFFERINTERFACE_H_

#include "Color_RGB.h"

class BufferInterface {
public:
	
	virtual Color_RGB GetPixelColor(int x, int y);
	virtual void SetPixelColor(int x, int y, float r, float g, float b);
	virtual void FillBufferColor(float r, float g, float b);
	virtual int GetWidth();
	virtual int GetHeight();

private:



};

#endif