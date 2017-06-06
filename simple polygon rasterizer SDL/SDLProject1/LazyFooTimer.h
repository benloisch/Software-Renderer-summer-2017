#ifndef LAZYFOOTIMER_H
#define LAZYFOOTIMER_H
#include <SDL.h>
using namespace std;

//Class Timer credit goes to LazyFoo SDL Tutorials.
class Timer
{
private:
	//The clock time when the timer started
	int startTicks;

public:
	//Initializes variables
	Timer();

	//The various clock actions
	void start();

	//Gets the timer's time
	int get_ticks();

};

Timer::Timer()
{
	//Initialize the variables
	startTicks = 0;
}

void Timer::start()
{
	//Get the current clock time
	startTicks = SDL_GetTicks();
}

int Timer::get_ticks()
{
	return SDL_GetTicks() - startTicks;	
}

#endif