#ifndef TNTGENT_H_INCLUDED
#define TNTGENT_H_INCLUDED

#include <SDL.h>
#include "Timer.h"

#include <stdio.h>

class TNTgent
{
    public:
		TNTgent(int x, int y);
		~TNTgent();

		void reset();

		int health;
		bool dead;
		bool onFlatSurface;
		bool prone;

		void move(int animationInt);
		int animationIntLol;
		void update();

		void jump();
		int heightToJumpTo;
		bool jumping;

		float xPos;
		float yPos;

        SDL_Rect* hitBox;
        SDL_Rect* hitBoxHead;

        SDL_Rect* trigger;

        SDL_Rect* bullets[5];
        SDL_Point gunPoint;
        bool bulletactive[5];

        SDL_Rect* explosionHitbox;
        Timer explosionTimer;


        Timer programTimer;
        int sequence;
        int check;

        int animation;

        bool tossing;
        bool exploded;
        int tossSpeed;
        int headThrown;

        bool fuckingshootmeinthegoddamndick;
        Timer speakingTimer;
        int speakingFrame;

        bool bullshitsoundboolno1352;

        Timer hitColorHead;
};

#endif // TNTGENT_H_INCLUDED
