#ifndef HIGH_PERFORMANCE_COUNTER_H_
#define HIGH_PERFORMANCE_COUNTER_H_

#include <Windows.h>

class HighPerformanceCounter
{
public:
	HighPerformanceCounter::HighPerformanceCounter();

	__int64 currTime;
	__int64 prevTime;

	double secondsPerCount;

	double mtimePerFrame;

	void Start();
	void Tick();

};

#endif