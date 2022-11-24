#include "SharkRobot.h"

SharkRobot::SharkRobot(int x, int y)
{
    hitBox = new SDL_Rect;
    hitBox->w = 60;
    hitBox->h = 30;
    hitBox->x = x;
    hitBox->y = y;

    xPos = 10;
    yPos = 10;

    health = 3;
    dead = false;

    //jumpTimer.start();
}

SharkRobot::~SharkRobot()
{
    hitBox = NULL;
}

void SharkRobot::reset()
{
    health = 3;
    dead = false;
    jumpTimer.start();
}

void SharkRobot::move()
{
    if(dead == false)
    {
        if(jumpTimer.isStarted()) { hitBox->y += 9; } else { hitBox->y += 1;} ///gravity shenanigans
    }

    if(health < 1 && !dead && !jumpTimer.isStarted())
    {
        hitBox->y -= 5;
    }


    if(dead == false)
    {
        hitBox->x -= 2;


        if(jumpTimer.isStarted())
        {
            if(jumpTimer.getTicks() < 600)
            {
                hitBox->y -= 950 * 0.016; //go up


                if(jumpTimer.getTicks() < 500)
                {
                    hitBox->y += jumpTimer.getTicks() / 3 * 0.016;
                }
                else
                {
                    hitBox->y += 700 / 2 * 0.016;
                }
            }
        }
    }

    if(beingShotInTheFaceTimer.getTicks() > 800)
    {
        beingShotInTheFaceTimer.stop();
    }
}

void SharkRobot::update()
{
    xPos = hitBox->x;
    yPos = hitBox->y - 14;
}
