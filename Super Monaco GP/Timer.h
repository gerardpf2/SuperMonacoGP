#ifndef _TIMER_
#define _TIMER_

#include "Types.h"

class Timer
{
	public:

		Timer();

		virtual ~Timer();

		void start();

		uint getElapsedTimeMs() const;

	private:

		uint lastTimeMs = 0;
};

#endif