#include "Targets.h"

Targets::Targets()
{
    hitBox = new SDL_Rect;

    xPos = 10;
    yPos = 10;

    color = 0;
    alive = false;

    alpha = 255;
}

Targets::~Targets()
{
    hitBox = NULL;
}

void Targets::move()
{
    if(alive && funTimerlol.getTicks() > 3000)
    {
        if(alpha > 1) {alpha -= 3;}
    }

    if(alive && funTimerlol.getTicks() > 4000)
    {
        alive = false;
        alpha = 0;
    }

    if(!alive)
    {
        if(color == 1)
        {
            hitBox->y -= 1;
        }
        else if (color == 2)
        {
           hitBox->y -= 2;
        }
        else
        {
            hitBox->y -= 3;
        }
        if(alpha > 1) {alpha -= 3;}
    }

}
