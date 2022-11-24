#include "Sorse.h"

Sorse::Sorse(int x, int y)
{
    onFlatSurface = false;

    hitBoxBody = new SDL_Rect;
    hitBoxBody->w = 70;
    hitBoxBody->h = 113;
    hitBoxBody->x = x;
    hitBoxBody->y = y;

    hitBoxEyes = new SDL_Rect;
    hitBoxEyes->w = 20;
    hitBoxEyes->h = 15;
    hitBoxEyes->x = -9999;
    hitBoxEyes->y = -9999;

    hitBoxFeet = new SDL_Rect;
    hitBoxFeet->w = 110;
    hitBoxFeet->h = 100;
    hitBoxFeet->x = -9999;
    hitBoxFeet->y = -9999;

    hitBoxBlackHole = new SDL_Rect;
    hitBoxBlackHole->w = 280;
    hitBoxBlackHole->h = 140;
    hitBoxBlackHole->x = -9999;
    hitBoxBlackHole->y = -9999;

    hitBoxTrigger = new SDL_Rect;
    hitBoxTrigger->w = 700;
    hitBoxTrigger->h = 300;
    hitBoxTrigger->x = -9999;
    hitBoxTrigger->y = -9999;

    hitBoxExplosion = new SDL_Rect;
    hitBoxExplosion->w = 124;
    hitBoxExplosion->h = 72;
    hitBoxExplosion->x = -9999;
    hitBoxExplosion->y = -9999;

    health = 2;

    xPos = 10;
    yPos = 10;

    flip =  SDL_FLIP_HORIZONTAL;

    alive = true;
    collapsing = false;

    movement = 0;
    test = 0;
    backingUp = true;

    blackHoleActive = false;
    blackHoleFlying = false;
    blackHoleTarget.x = -9999;
    blackHoleTarget.y = -9999;
    blackHoleSuccer.x = -9999;
    blackHoleSuccer.y = -9999;
    blackHoleSwitch = 1;

    followLeft = false;
    followRight = false;

    exploding = false;
}

Sorse::~Sorse()
{
    hitBoxBody = NULL;
    hitBoxEyes= NULL;
    hitBoxFeet = NULL;
    hitBoxTrigger = NULL;
    hitBoxExplosion = NULL;
}

void Sorse::reset()
{
    health = 2;
    alive = true;
    collapsing = false;
    movement = 0;
    test = 0;
    backingUp = true;
    blackHoleActive = false;
    blackHoleFlying = false;
    blackHoleTarget.x = -9999;
    blackHoleTarget.y = -9999;
    followLeft = false;
    followRight = false;
    exploding = false;
    hitBoxBlackHole->x = -9999;
    hitBoxBlackHole->y = -9999;

    hitBoxBody->h = 113;
}

void Sorse::move()
{
    if(alive)
    {
        if(!onFlatSurface)
        {
            hitBoxBody->y += 1; //fall from gravity
        }
        onFlatSurface = false;

        if(test == 6)
        {
            if(!attackAnimation.isStarted() && !summonAnimation.isStarted())
            {
                idle();
            }
        }

        if(test < 6)
        {
            test++;
        }
        else
        {
            test = 1;
        }

        if(!summonAnimation.isStarted() && !attackAnimation.isStarted())
        {
            if(followLeft)
            {
               hitBoxBody->x -= 1;
            }
            else if(followRight)
            {
                hitBoxBody->x += 1;
            }
        }
    }
    else if(collapsing)
    {
        hitBoxBody->y += 5;
    }

    if(blackHoleFlying) ///make black hole fly towards its target
    {
        if(hitBoxBlackHole->x < blackHoleTarget.x)
        {
            hitBoxBlackHole->x += 2;
        }

        if(hitBoxBlackHole->x > blackHoleTarget.x)
        {
            hitBoxBlackHole->x -= 2;
        }


        if(hitBoxBlackHole->y < blackHoleTarget.y)
        {
            hitBoxBlackHole->y += 1;
        }

        if(hitBoxBlackHole->y > blackHoleTarget.y)
        {
            hitBoxBlackHole->y -= 1;
        }

        if(hitBoxBlackHole->x < blackHoleTarget.x + 3 && hitBoxBlackHole->x > blackHoleTarget.x - 3)
        {
            blackHoleFlying = false;
            blackHoleActive = true;
            hitBoxBlackHole->x -= 140;
            hitBoxBlackHole->y -= 70;
            hitBoxBlackHole->w = 280;
            hitBoxBlackHole->h = 140;
        }
    }

    followLeft = false;
    followRight = false;
}

void Sorse::update()
{
    xPos = hitBoxBody->x - 15;
    yPos = hitBoxBody->y;

    hitBoxTrigger->x = hitBoxBody->x - 320;
    hitBoxTrigger->y = hitBoxBody->y - 70;

    hitBoxEyes->x = hitBoxBody->x + 22;

    if(hitColor.getTicks() > 80)
    {
        hitColor.stop();
    }

    if(health < 1)
    {
        alive = false;
    }

    if(103 * (summonAnimation.getTicks() / 40) >= 4120)
    {
        summonAnimation.stop();
    }

    if(124 * (attackAnimation.getTicks() / 40) >= 4712)
    {
        attackAnimation.stop();
    }

    blackHoleSwitch++;
    if(blackHoleSwitch > 15)
    {
        blackHoleSwitch = 1;
    }

    if(blackHoleActive && blackHoleSwitch == 1)
    {
        blackHoleSuccer.x = (hitBoxBlackHole->x + 120) + rand() % 40;
        blackHoleSuccer.y = (hitBoxBlackHole->y + 50) + rand() % 40;
    }

    if(!blackHoleFlying)
    {
        if(103 * (summonAnimation.getTicks() / 50) >= 1957) ///spawn black hole
        {
            blackHoleFlying = true;
            hitBoxBlackHole->w = 30;
            hitBoxBlackHole->h = 30;
            if(flip == SDL_FLIP_NONE)
            {
                hitBoxBlackHole->x = hitBoxBody->x + 68;
                hitBoxBlackHole->y = hitBoxBody->y + 11;
                blackHoleTarget.x = hitBoxBody->x + 270;
                blackHoleTarget.y = hitBoxBody->y + 100;
            }
            else
            {
                hitBoxBlackHole->x = hitBoxBody->x - 38;
                hitBoxBlackHole->y = hitBoxBody->y + 11;
                blackHoleTarget.x = hitBoxBody->x - 270;
                blackHoleTarget.y = hitBoxBody->y + 100;
            }
        }
    }

    if(124 * (attackAnimation.getTicks() / 40) > 2356 && 124 * (attackAnimation.getTicks() / 40) < 3348)
    {
        exploding = true;
    }
    else
    {
        exploding = false;
    }

    hitBoxFeet->x = hitBoxBody->x - 20;
    hitBoxFeet->y = hitBoxBody->y + 110;

    hitBoxExplosion->x = hitBoxBody->x - 28;
    hitBoxExplosion->y = hitBoxBody->y + 130;
}

void Sorse::idle()
{
    if(!backingUp)
    {
        switch(movement)
        {
            case 0: hitBoxBody->x -= 1;
                    hitBoxBody->y += 1;
                    movement++;
            break;
            case 1: hitBoxBody->x -= 1;
                    hitBoxBody->y += 1;
                    movement++;
            break;
            case 2: hitBoxBody->x -= 1;
                    hitBoxBody->y += 1;
                    movement++;
            break;
            case 3: hitBoxBody->x -= 1;
                    hitBoxBody->y += 1;
                    movement++;
            break;
            case 4: hitBoxBody->x -= 1;
                    hitBoxBody->y += 1;
                    movement++;
            break;
            case 5: hitBoxBody->x -= 1;
                    hitBoxBody->y += 1;
                    movement++;
            break;
            case 6: hitBoxBody->x -= 1;
                    hitBoxBody->y += 1;
                    movement++;
            break;
            case 7: hitBoxBody->x -= 1;
                    hitBoxBody->y += 1;
                    movement++;
            break;
            case 8: hitBoxBody->x -= 1;
                    movement++;
            break;
            case 9: hitBoxBody->x -= 1;
                    movement++;
            break;
            case 10:
                    hitBoxBody->x -= 1;
                    movement++;
            break;
            case 11:
                    hitBoxBody->x -= 1;
                    movement++;
            break;
            case 12:
                    hitBoxBody->x -= 1;
                    hitBoxBody->y -= 1;
                    movement++;
            break;
            case 13:
                    hitBoxBody->x -= 1;
                    hitBoxBody->y -= 1;
                    movement++;
            break;
            case 14:
                    hitBoxBody->x -= 1;
                    hitBoxBody->y -= 1;
                    movement++;
            break;
            case 15:
                    hitBoxBody->x -= 1;
                    hitBoxBody->y -= 1;
                    movement++;
            break;
            case 16:
                    hitBoxBody->x -= 1;
                    hitBoxBody->y -= 1;
                    movement++;
            break;
            case 17:
                    hitBoxBody->x -= 1;
                    hitBoxBody->y -= 1;
                    movement++;
            break;
            case 18:
                    hitBoxBody->x -= 1;
                    hitBoxBody->y -= 1;
                    movement++;
            break;
            case 19:
                    hitBoxBody->x -= 1;
                    hitBoxBody->y -= 1;
                    movement = 0;
                    backingUp = true;
            break;
        }
    }
    else
    {
        switch(movement)
        {
            case 0: hitBoxBody->x += 1;
                    hitBoxBody->y += 1;
                    movement++;
            break;
            case 1: hitBoxBody->x += 1;
                    hitBoxBody->y += 1;
                    movement++;
            break;
            case 2: hitBoxBody->x += 1;
                    hitBoxBody->y += 1;
                    movement++;
            break;
            case 3: hitBoxBody->x += 1;
                    hitBoxBody->y += 1;
                    movement++;
            break;
            case 4: hitBoxBody->x += 1;
                    hitBoxBody->y += 1;
                    movement++;
            break;
            case 5: hitBoxBody->x += 1;
                    hitBoxBody->y += 1;
                    movement++;
            break;
            case 6: hitBoxBody->x += 1;
                    hitBoxBody->y += 1;
                    movement++;
            break;
            case 7: hitBoxBody->x += 1;
                    hitBoxBody->y += 1;
                    movement++;
            break;
            case 8: hitBoxBody->x += 1;
                    movement++;
            break;
            case 9: hitBoxBody->x += 1;
                    movement++;
            break;
            case 10:
                    hitBoxBody->x += 1;
                    movement++;
            break;
            case 11:
                    hitBoxBody->x += 1;
                    movement++;
            break;
            case 12:
                    hitBoxBody->x += 1;
                    hitBoxBody->y -= 1;
                    movement++;
            break;
            case 13:
                    hitBoxBody->x += 1;
                    hitBoxBody->y -= 1;
                    movement++;
            break;
            case 14:
                    hitBoxBody->x += 1;
                    hitBoxBody->y -= 1;
                    movement++;
            break;
            case 15:
                    hitBoxBody->x += 1;
                    hitBoxBody->y -= 1;
                    movement++;
            break;
            case 16:
                    hitBoxBody->x += 1;
                    hitBoxBody->y -= 1;
                    movement++;
            break;
            case 17:
                    hitBoxBody->x += 1;
                    hitBoxBody->y -= 1;
                    movement++;
            break;
            case 18:
                    hitBoxBody->x += 1;
                    hitBoxBody->y -= 1;
                    movement++;
            break;
            case 19:
                    hitBoxBody->x += 1;
                    hitBoxBody->y -= 1;
                    movement = 0;
                    backingUp = false;
            break;
        }
    }
}
