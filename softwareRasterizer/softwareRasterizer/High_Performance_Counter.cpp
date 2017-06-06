#include "High_Performance_Counter.h"

Timer::Timer()
{
	currTime = 0;
	prevTime = 0;
	secondsPerCount = 0;
	mtimePerFrame = 0;
}

void Timer::Start()
{
	QueryPerformanceCounter((LARGE_INTEGER*)&prevTime);

	__int64 countsPerSecond;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);

	secondsPerCount = 1.0 / (double)countsPerSecond;
}

void Timer::Tick()
{
	//curr = b
	//prev = a

	// (b - a) * seconds per count

	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	mtimePerFrame = (currTime - prevTime) * secondsPerCount;

	prevTime = currTime;
}