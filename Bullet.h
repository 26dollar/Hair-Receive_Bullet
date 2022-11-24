#ifndef BULLET_H_INCLUDED
#define BULLET_H_INCLUDED

#include <SDL.h>
#include "Timer.h"
#include <cstdlib>

class Bullet
{
    public:
        Bullet();
        ~Bullet();

        static const short int speed = 770;
        short int speed2;

        float xPos;
        float yPos;

        char direction; //1 = up, 2 = right, 3 = down, 4 = left

        bool active;


        SDL_Rect* hitBox;

        void restart(int x, int y, char direction, SDL_RendererFlip flip);

        void restart2(int x, int y, char direction);

        void move(float timestep);
        void move2();
        void update();

        Timer testTimer;
        void test();
        void test2();
        bool testing;
        bool testing2;
        bool beingReflected;

        bool following;
        void follow();
        int followingX;
        int followingY;
        int jellySpeed;
        Timer deletionTimer; //once it starts, 700ms until the bullet is made inactive!

        bool inBlackHole;
        void followBlackHole();
        int blackHoleX;
        int blackHoleY;
};


#endif // BULLET_H_INCLUDED
