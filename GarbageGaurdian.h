#ifndef GARBAGEGAURDIAN
#define GARBAGEGAURDIAN_H_INCLUDED

#include <SDL.h>
#include "Timer.h"
#include <stdio.h>
#include <cstdlib>
#include "NonFlatSurface.h"

class GarbageGaurdian
{
    public:
		GarbageGaurdian(int x, int y);
		~GarbageGaurdian();

		void reset();

		short int health;
		bool onFlatSurface;

		void move();
		void update();

		float xPos;
		float yPos;

        SDL_Rect* hitBox;
        SDL_Rect* hitBoxBody;
        SDL_Rect* swordHitbox;
        SDL_Rect* shieldHitbox;

        short int animation;
        Timer animationTimer;

        bool shieldFlying;
        bool swordFlying;
        short int shieldHealth;
        bool swordInGround;
        bool rampInitialized;
        NonFlatSurface swordRamp;

        int state;

        Timer hitColorBody;
};

#endif // GARBAGEGAURDIAN_H_INCLUDED
