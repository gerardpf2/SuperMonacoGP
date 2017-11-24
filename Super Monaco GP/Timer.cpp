#include "Timer.h"

#include <SDL.h>

Timer::Timer()
{
	start();
}

Timer::~Timer()
{ }

void Timer::start()
{
	lastTimeMs = SDL_GetTicks();
}

uint Timer::getElapsedTimeMs() const
{
	return SDL_GetTicks() - lastTimeMs;
}