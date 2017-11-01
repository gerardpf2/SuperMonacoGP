#ifndef _TIMER_
#define _TIMER_

class Timer
{
	public:

		Timer();

		~Timer();

		void start();

		unsigned int getElapsedTimeMs() const;

	private:

		unsigned int lastTimeMs;
};

#endif