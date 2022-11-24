#ifndef TARGETS_H_INCLUDED
#define TARGETS_H_INCLUDED

#include <SDL.h>
#include "Timer.h"


class Targets
{
    public:
		Targets();
		~Targets();

		int color;
		int alpha;
		bool alive;

		void move();
		int xHome;
		int yHome;

		Timer funTimerlol;

		float xPos;
		float yPos;

        SDL_Rect* hitBox;
};

#endif // TARGET_H_INCLUDED_H_INCLUDED
