#ifndef ARMOREDTRUCK_H
#define ARMOREDTRUCK_H

#include <SDL.h>
#include "Timer.h"
#include <stdio.h>


class ArmoredTruck
{
    public:
        ArmoredTruck(int x, int y);
        ~ArmoredTruck();

    void reset();

    int home;

    char health;

    SDL_Rect* hitBoxGlass;
    SDL_Rect* hitBoxTop;
    SDL_Rect* hitBoxBottom;
    SDL_Rect* hitBoxTrigger;
    SDL_Rect* hitBoxTrigger2;

    float xPos;
    float yPos;

    float yVelocity;
    float xVelocity;

    void move();

    bool onFlatSurface;
    bool charging;
    bool reversing;

    Timer triggeredTimer;
    Timer respawnTimer;
    bool respawnAfterDeath;
};

#endif // ARMOREDTRUCK_H
