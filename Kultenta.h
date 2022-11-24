#ifndef KULTENTA_H_INCLUDED
#define KULTENTA_H_INCLUDED

#include <SDL.h>
#include "Timer.h"

class Kultenta
{
    public:
		Kultenta(int x, int y);
		~Kultenta();

		void reset();

		bool dead;
		Timer beingDeadTimer;
		bool fallingToDEath;
		bool imadethisbooltosaveoncpu;

		void move();
		void update();

		float xPos;
		float yPos;

        SDL_Rect* hitBox;
        Timer hitColor;

        SDL_Rect* bullets[8];

        bool bulletactive[8];

        /*bool bullet1active;
        bool bullet2active;
        bool bullet3active;
        bool bullet4active;
        bool bullet5active;
        bool bullet6active;
        bool bullet7active;
        bool bullet8active;*/
        int bullet;


        Timer animationTime;
        int clip;
        int clip1;

        SDL_Point gunPoint;

        SDL_Point bulletStorage1;
        SDL_Point bulletStorage2;
        SDL_Point bulletStorage3;
        SDL_Point bulletStorage4;
        SDL_Point bulletStorage5;

        int storageOffsetX;
        int storageOffsetY;



        Timer rootyTootyTimetoShootyTimer;
        Timer replaceTimer1;
        Timer replaceTimer2;
        Timer replaceTimer3;
        Timer replaceTimer4;
        Timer replaceTimer5;
        bool shoot;
        void shootgun();
        bool storage1Full;
        bool storage2Full;
        bool storage3Full;
        bool storage4Full;
        bool storage5Full;

        bool storage1visibol;
        bool storage2visibol;
        bool storage3visibol;
        bool storage4visibol;
        bool storage5visibol;

        bool gayAssSoundBool;
};

#endif // KULTENTA_H_INCLUDED
