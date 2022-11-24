#ifndef SHARKROBOT_H_INCLUDED
#define SHARKROBOT_H_INCLUDED

#include <SDL.h>
#include "Timer.h"

class SharkRobot
{
    public:                        ///my body lies over the ocean, my body lies over the sea, i'm no good at astral projection, so bring back my body to me
		SharkRobot(int x, int y);
		~SharkRobot();

		void reset();

		short int health;
		bool dead;

		void move();
		void update();

		float xPos;
		float yPos;

        SDL_Rect* hitBox;

        Timer jumpTimer;
        Timer beingShotInTheFaceTimer;
};

#endif // SHARKROBOT_H_INCLUDED
