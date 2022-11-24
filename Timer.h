#ifndef TIMER_H_INCLUDED
#define TIMER_H_INCLUDED
#include <SDL.h>

class Timer
{
    public:
		Timer();

		//The various clock actions
		void start();
		void stop();
		void pause();
		void unpause();

		//Gets the timer's time
		Uint32 getTicks();

		//Checks the status of the timer
		bool isStarted();
		bool isPaused();

    //private:
		//The clock time when the timer started
		Uint32 startTicks;

		//The ticks stored when the timer was paused
		Uint32 pausedTicks;

        //aa
        Uint32 gamePausedTicks;

		//The timer status
		bool paused;
		bool started;
};

#endif // TIMER_H_INCLUDED
