#ifndef TARGETMASTER_H_INCLUDED
#define TARGETMASTER_H_INCLUDED

#include <SDL.h>
#include "Timer.h"
#include "Targets.h"
#include <cstdlib>


class TargetMaster
{
    public:
		TargetMaster(int x, int y);
		~TargetMaster();

		void move();
		int xHome;
		int yHome;

		Timer funTimer;
		int twoSecCounter;

		int score;

		Targets targets[15];
};

#endif // TARGETMASTER_H_INCLUDED_H_INCLUDED
