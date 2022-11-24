#ifndef LEDGE_H
#define LEDGE_H

#include <SDL.h>

class Ledge //dokusonmaru
{
    public:
    Ledge( int x, int y, bool right);
    ~Ledge();

    int xPos, yPos;
    SDL_RendererFlip flip;

    bool facingRight; ///errrrr, if facing right is true then its on a left wall, also i compleyl forgot how to use enumerations--not even sure if they are in C++


    SDL_Rect* hitBox;
};
#endif // LEDGE_H
