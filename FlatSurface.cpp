#include "FlatSurface.h"

FlatSurface::FlatSurface(int x, int y, int width, int height, int xpos, int ypos)
{
    xPos = xpos;
    yPos = ypos;

    hitBox = new SDL_Rect;
    hitBox->w = width;
    hitBox->h = height;
    hitBox->x = x;
    hitBox->y = y;

    activated = false;
}

FlatSurface::FlatSurface()
{
    xPos = -200;
    yPos = -200;

    hitBox->w = 200;
    hitBox->h = 200;
    hitBox->x = -200;
    hitBox->y = -200;
}

FlatSurface::~FlatSurface()
{
    hitBox = NULL;
}
