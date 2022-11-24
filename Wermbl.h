#ifndef WERMBL
#define WERMBL_H_INCLUDED

#include <SDL.h>
#include "Timer.h"
#include <stdio.h>
#include <cstdlib>

class Wermbl
{
    public:
		Wermbl(int x, int y);
		~Wermbl();

		void reset();

		bool onFlatSurface;

		void move();
		void update();

		float xPos;
		float yPos;

		float xPosFace;
		float yPosFace;

        SDL_Rect* hitBox;
        SDL_Rect* hitBoxHead;

        bool walking;
        bool walkingRight;
        SDL_RendererFlip flip;

        int gacha1; //gacha ball types
        int gacha2;
        int gacha3;

        int gacha1x;
        int gacha1y;
        int gacha2x;
        int gacha2y;
        int gacha3x;
        int gacha3y;

        int numberOfBulletsEaten; //i think this one is self explanatory

        Timer gachaActivationTimer;
        int gachAvtivationState;
        int activatingColor;

        Timer explosionTimer;
        SDL_Rect* explosionHitbox;
        bool explosionSoundBoolUgh;

        bool flying;
        bool flyingUp;
        float flyingHome;
        void fly();
        int amountToFly;
        int flyingSpeed;

        SDL_Rect* bullets[3];
        bool bulletactive[3];
        bool shooting;
        Timer shootTimer;
        void shoot();
};

#endif // WERMBL_H_INCLUDED
