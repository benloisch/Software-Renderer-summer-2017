#include "High_Performance_Counter.h"

HighPerformanceCounter::HighPerformanceCounter()
{
	currTime = 0;
	prevTime = 0;
	secondsPerCount = 0;
	mtimePerFrame = 0;
}

void HighPerformanceCounter::Start()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTime);

	__int64 countsPerSecond;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);

	secondsPerCount = 1.0 / (double)countsPerSecond;
}

void HighPerformanceCounter::Tick()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mtimePerFrame = (currTime - prevTime) * secondsPerCount;

	prevTime = currTime;
}