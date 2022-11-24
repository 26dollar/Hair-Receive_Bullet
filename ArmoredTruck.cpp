#include "ArmoredTruck.h"

ArmoredTruck::ArmoredTruck(int x, int y)
{
    health = 3;

    xPos = -2000;
    yPos = -2000;

    hitBoxBottom = new SDL_Rect;
    hitBoxGlass = new SDL_Rect;
    hitBoxTop = new SDL_Rect;
    hitBoxTrigger = new SDL_Rect;
    hitBoxTrigger2 = new SDL_Rect;

    xVelocity = x;
    yVelocity = y;

    hitBoxBottom->x = -2000;
    hitBoxBottom->y = -2000;
    hitBoxBottom->w = 200;
    hitBoxBottom->h = 70;

    hitBoxGlass->x = -2000;
    hitBoxGlass->y = -2000;
    hitBoxGlass->w = 20;
    hitBoxGlass->h = 45;

    hitBoxTop->x = -2000;
    hitBoxTop->y = -2000;
    hitBoxTop->w = 160;
    hitBoxTop->h = 65;

    hitBoxTrigger->x = -2000;
    hitBoxTrigger->y = -2000;
    hitBoxTrigger->w = 65;
    hitBoxTrigger->h = 120;

    hitBoxTrigger2->x = x - 440;
    hitBoxTrigger2->y = y - 50;
    hitBoxTrigger2->w = 455;
    hitBoxTrigger2->h = 120;

    home = x;

    onFlatSurface = false;
    charging = false;
    reversing = false;

    respawnAfterDeath = false;
}

ArmoredTruck::~ArmoredTruck()
{
    hitBoxGlass = NULL;
    hitBoxTop = NULL;
    hitBoxBottom = NULL;
    hitBoxTrigger = NULL;
    hitBoxTrigger2 = NULL;
}

void ArmoredTruck::reset()
{
    respawnTimer.stop();
    health = 3;
    charging = false; //bug?
    xVelocity = home;
}

void ArmoredTruck::move()
{
    hitBoxBottom->x = xVelocity;
    hitBoxBottom->y = yVelocity;

    hitBoxGlass->x = hitBoxBottom->x + 10;
    hitBoxGlass->y = hitBoxBottom->y - 45;
    hitBoxTop->x = hitBoxBottom->x + 20;
    hitBoxTop->y = hitBoxBottom->y - 70;

    if(triggeredTimer.getTicks()  > 150 && triggeredTimer.getTicks() < 250)
    {
        hitBoxTrigger->x = hitBoxBottom->x - 85;
        hitBoxTrigger->y = hitBoxBottom->y - 60;
        hitBoxTrigger->w = 75;
        hitBoxTrigger->h = 120;
    }
    else
    {
        hitBoxTrigger->x = hitBoxBottom->x - 75;
        hitBoxTrigger->y = hitBoxBottom->y - 60;
        hitBoxTrigger->w = 65;
        hitBoxTrigger->h = 120;
    }


    if(!onFlatSurface)
    {
        yVelocity += 1; //gravity
        hitBoxTrigger2->y += 1;
    }

    xPos = xVelocity - 85;
    yPos = yVelocity - 80;

    if(triggeredTimer.getTicks() > 600)
    {
        triggeredTimer.stop();
    }

    if(hitBoxBottom->x < hitBoxTrigger2->x)
    {
        charging = false;
    }

    if(health > 0 && charging && !triggeredTimer.isStarted())
    {
        xVelocity -= 6;
    }
    else if(triggeredTimer.getTicks() > 0 && triggeredTimer.getTicks() < 150)
    {
        xVelocity -= 1;
        charging = false;
    }
    else
    {
        charging = false;
    }

    reversing = false;
    if(health > 0 && hitBoxBottom->x < hitBoxTrigger2->x + hitBoxTrigger2->w + 30 && !charging && !triggeredTimer.isStarted())
    {
        xVelocity += 6;
        reversing = true;
    }

    if(respawnAfterDeath && health < 1 && !respawnTimer.isStarted())
    {
        respawnTimer.start();
    }

    if(respawnTimer.getTicks() > 9000)
    {
        reset();
    }
}
