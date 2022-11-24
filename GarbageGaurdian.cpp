#include "GarbageGaurdian.h"
#include "NonFlatSurface.h"

GarbageGaurdian::GarbageGaurdian(int x, int y)
{
    hitBox = new SDL_Rect;
    hitBox->w = 45;
    hitBox->h = 75;
    hitBox->x = x;
    hitBox->y = y;

    hitBoxBody = new SDL_Rect;
    hitBoxBody->w = 25;
    hitBoxBody->h = 55;
    hitBoxBody->x = x;
    hitBoxBody->y = y;

    swordHitbox = new SDL_Rect;
    swordHitbox->w = 25;
    swordHitbox->h = 25;
    swordHitbox->x = -9999;
    swordHitbox->y = -9999;

    shieldHitbox = new SDL_Rect;
    shieldHitbox->w = 15;
    shieldHitbox->h = 45;
    shieldHitbox->x = -9999;
    shieldHitbox->y = -9999;

    xPos = 10;
    yPos = 10;

    health = 3;

    onFlatSurface = false;

    animation = 1;
    state = 1;

    animationTimer.start();

    shieldFlying = false;
    swordFlying = false;

    shieldHealth = 1;
    swordInGround = false;
    rampInitialized = false;
}

GarbageGaurdian::~GarbageGaurdian()
{
    hitBox = NULL;
    hitBoxBody = NULL;
    swordHitbox = NULL;
    shieldHitbox = NULL;
}

void GarbageGaurdian::reset()
{
    health = 3;

    animation = 1;
    state = 1;

    animationTimer.start();

    shieldFlying = false;
    swordFlying = false;

    shieldHealth = 1;
    swordInGround = false;
    rampInitialized = false;


    swordRamp.xPos = -999;
    swordRamp.yPos = -999;

    swordRamp.x1 = -999;
    swordRamp.y1 = -999;

    swordRamp.slope = 0;

    swordRamp.x2 = -999;
    swordRamp.y2 = -999;

    swordHitbox->w = 25;
    swordHitbox->h = 25;
    swordHitbox->x = -9999;
    swordHitbox->y = -9999;

    shieldHitbox->w = 15;
    shieldHitbox->h = 45;
    shieldHitbox->x = -9999;
    shieldHitbox->y = -9999;
}

void GarbageGaurdian::move()
{
    if(!onFlatSurface)
    {
        hitBox->y += 5; //fall from gravity
    }
    onFlatSurface = false;

    if(swordFlying)
    {
        swordHitbox->x -= 11;
        swordHitbox->y += 3;
    }

    if(shieldFlying)
    {
        shieldHitbox->x -= 7;
    }
}

void GarbageGaurdian::update()
{
    xPos = hitBox->x;
    yPos = hitBox->y;

    hitBoxBody->x = hitBox->x + 10;
    hitBoxBody->y = hitBox->y - 55;

    if(shieldHealth < 1)
    {
        shieldFlying = false;
    }

    if(swordInGround && !rampInitialized)
    {
        swordRamp.x1 = swordHitbox->x;
        swordRamp.y1 = swordHitbox->y + 15;

        swordRamp.x2 = swordRamp.x1 + (1 * 60);
        swordRamp.y2 = swordRamp.y1 - 60;

        if(swordRamp.y1 > swordRamp.y2)
        {
            swordRamp.slopingUp = true;
        }
        else
        {
            swordRamp.slopingUp = false;
        }

        rampInitialized = true;

        swordRamp.slope = 1;
    }

    switch(state)
    {
        case 1:
                if(animationTimer.getTicks() > 6000)
                {
                    state = 2;
                    animation = 2;
                    shieldHitbox->x = hitBox->x - 30;
                    shieldHitbox->y = hitBox->y - 40;
                }
        break;
        case 2:
                if(animationTimer.getTicks() > 9000)
                {
                    state = 3;
                    animation = 3;
                }
        break;
        case 3:
                if(animationTimer.getTicks() > 10000)
                {
                    state = 4;
                    animation = 4;
                    swordFlying = true;
                    swordHitbox->x = hitBox->x + 35;
                    swordHitbox->y = hitBox->y - 55;
                }
        break;
        case 4:
                if(animationTimer.getTicks() > 11000)
                {
                    state = 5;
                    animation = 5;
                    shieldHitbox->x = -9999;
                    shieldHitbox->y = -9999;
                }
        break;
        case 5:
                if(animationTimer.getTicks() > 12000)
                {
                    state = 6;
                    animation = 6;
                    shieldHitbox->x = hitBox->x + 10;
                    shieldHitbox->y = hitBox->y - 65;
                    shieldHitbox->w = 45;
                    shieldHitbox->h = 10;
                    shieldFlying = true;
                }
        break;
        case 6:
        break;
        case 7:
        break;
    }

    if(hitColorBody.getTicks() > 80)
    {
        hitColorBody.stop();
    }

}
