#ifndef DOOR_H_INCLUDED
#define DOOR_H_INCLUDED

#include <SDL.h>

class Door
{
    public:
    Door( int x, int y, int ID);
    ~Door();

    int IDN;

    int destinationX;
    int destinationY;

    SDL_Rect* hitBox;
};

#endif // DOOR_H_INCLUDED
