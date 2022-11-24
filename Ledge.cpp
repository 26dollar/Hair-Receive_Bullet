#include "Ledge.h"

Ledge::Ledge(int x, int y, bool right)
{
    hitBox = new SDL_Rect;
    hitBox->w = 15;
    hitBox->h = 15;
    hitBox->x = x;
    hitBox->y = y;

    facingRight = right;

    if(facingRight)
    {
        xPos = x - 35;
        yPos = y + 3;
        flip = SDL_FLIP_NONE;
    }
    else
    {
        xPos = x - 1;
        yPos = y + 3;
        flip = SDL_FLIP_HORIZONTAL;
    }
}

Ledge::~Ledge()
{
    hitBox = NULL;
}
