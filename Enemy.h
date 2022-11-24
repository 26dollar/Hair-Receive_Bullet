#ifndef ENEMY_H_INCLUDED
#define ENEMY_H_INCLUDED

#include <SDL.h>
#include "Timer.h"
#include <stdio.h>

class Enemy
{
    public:                        ///my body lies over the ocean, my body lies over the sea, i'm no good at astral projection, so bring back my body to me
		Enemy(int x, int y);
		~Enemy();

		void reset();

		short int health;
		short int leftHealth;
		short int rightHealth;
		bool onFlatSurface;
		bool patrolLeft;
		bool patrolRight;

		short int home;

		void move();
		void update(int animation);

		float xPos;
		float yPos;

		float xPosLeft;
		float yPosLeft;

		float xPosRight;
		float yPosRight;

        SDL_Rect* hitBox;
        SDL_Rect* hitBoxLeft;
        SDL_Rect* hitBoxRight;
        bool leftActive;
        bool rightActive;
        bool leftFalling;
        bool rightFalling;

        Timer respawnTimer;
        bool alive;
        Timer hitColorBody;
        Timer hitColorLeft;
        Timer hitColorRight;
};

#endif // ENEMY_H_INCLUDED
