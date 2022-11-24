#include "Door.h"

Door::Door(int x, int y, int ID)
{
    hitBox = new SDL_Rect;
    hitBox->w = 64;
    hitBox->h = 140;
    hitBox->x = x;
    hitBox->y = y;

    switch(ID)
    {
        case 0: destinationX = -2805; //debug area
                destinationY = -1853;
        break;
        case 1: destinationX = 0; //level 1
                destinationY = 0;
        break;
        case 2: destinationX = -3858; //tutorial
                destinationY = -680;
        break;
    }

    IDN = ID;
}

Door::~Door()
{
    hitBox = NULL;
}
