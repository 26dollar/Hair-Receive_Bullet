#ifndef SORSE
#define SORSE_H_INCLUDED

#include <SDL.h>
#include "Timer.h"
#include <stdio.h>
#include <cstdlib>

class Sorse
{
    public:
		Sorse(int x, int y);
		~Sorse();
		void reset();

		void move();
		void update();

		float xPos;
		float yPos;

		bool onFlatSurface;
		int health;

        SDL_Rect* hitBoxBody;
        SDL_Rect* hitBoxEyes;
        SDL_Rect* hitBoxFeet;
        SDL_Rect* hitBoxBlackHole;
        SDL_Rect* hitBoxTrigger;
        SDL_Rect* hitBoxExplosion;

        bool alive;
        bool collapsing;

        SDL_RendererFlip flip;
        int movement;
        int test;
        void idle();
        bool backingUp;

        Timer summonAnimation;
        bool blackHoleFlying;
        bool blackHoleActive;
        SDL_Point blackHoleTarget;
        SDL_Point blackHoleSuccer;
        int blackHoleSwitch;

        Timer attackAnimation;
        bool exploding;

        bool followLeft;
        bool followRight;

        Timer hitColor;
};

#endif // SORSE_H_INCLUDED
