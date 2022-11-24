#include "Zipline.h"

Zipline::Zipline(int x, int y)
{
    xPos = 0;
    yPos = 0;
    linePosX = x;
    linePosY = y;

    xVelocity = x;
    yVelocity = y;

    hitBox = new SDL_Rect;
    hitBox->w = 25;
    hitBox->h = 25;
    hitBox->x = x;
    hitBox->y = y;

    docked = 1;
    carrying = 0;
}

Zipline::~Zipline()
{
    hitBox = NULL;
}

void Zipline::move(float timestep) //i should really comment more
{
    if(powerTimer.isStarted())
    {
        if(carrying)
        {
            xVelocity -= 6;
            yVelocity -= 3;
            if(powerTimer.getTicks() > 800)
            {
                powerTimer.stop();
            }

        }
        else
        {
            xVelocity -= 6;
            yVelocity -= 3;
        }
    }
    else if(carrying && !docked)
    {
        xVelocity += 6;
        yVelocity += 3;

    }
}

void Zipline::update()
{
    hitBox->x = xVelocity;
    hitBox->y = yVelocity;
    yPos = hitBox->y - 20;
    xPos = hitBox->x - 14;
}
