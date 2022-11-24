#ifndef ROBOMAGGOT_H_INCLUDED
#define ROBOMAGGOT_H_INCLUDED

#include <SDL.h>
#include "Timer.h"


class RoboMaggot
{
    public:                        ///my body lies over the ocean, my body lies over the sea, i'm no good at astral projection, so bring back my body to me
		RoboMaggot(int x, int y);
		~RoboMaggot();

		void move();
		void update();

		int score;

		float xPos;
		float yPos;

		int amountToMoveUp;
        int amountToMoveDown;

        SDL_Rect* hitBox;

        Timer colorBody;

        Timer volleyTimer;
        Timer bufferTimer;
        bool shoot;
        int short bullet;
        short int direction;

        SDL_RendererFlip flip;

        bool aTeleSound;
        bool firstTry;

        int sequence;
};

#endif // ROBOMAGGOT_H_INCLUDED_H_INCLUDED
