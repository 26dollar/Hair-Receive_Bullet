#ifndef ZIPLINE_H_INCLUDED
#define ZIPLINE_H_INCLUDED

#include <SDL.h>
#include "Timer.h"
#include <stdio.h>

class Zipline
{
public:
    Zipline(int x, int y);
    ~Zipline();

    int xPos;
    int yPos;
    int linePosX;
    int linePosY;

    int xVelocity;
    int yVelocity;

    bool docked;

    SDL_Rect* hitBox;

    Timer powerTimer;

    void move(float timestep);
    void update();
    //void update();
    static const short int horizontalSpeed = 350;
    static const short int verticalSpeed = 200;
    bool carrying;
};

#endif // ZIPLINE_H_INCLUDED
