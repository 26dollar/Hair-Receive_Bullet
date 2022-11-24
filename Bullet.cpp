#include "Bullet.h"

Bullet::Bullet()
{
    speed2 = 6;

    xPos = 0;
    yPos = 0;

    direction = 0; //1 = up, 2 = right, 3 = down, 4 = left

    active = false;

    following = false;

    hitBox = new SDL_Rect;
    hitBox->w = 6;
    hitBox->h = 6;
    hitBox->x = 0;
    hitBox->y = 0;
    testing = false;

    beingReflected = false;

    followingX = 0;
    followingY = 0;

    following = false;

    inBlackHole = false;
    blackHoleX = 0;
    blackHoleY = 0;
}

Bullet::~Bullet()
{
    hitBox = NULL;
}

void Bullet::restart(int x, int y, char drction, SDL_RendererFlip flip) //so this like places the bullets in their starting position right after they get shot out of the gun? based on where the end of the barrel is which is based off of which direction the gun is???
{
    direction = drction;
    following = false;
    inBlackHole = false;

    switch ( direction )
    {
        case 1:
            if(flip == SDL_FLIP_NONE)
            {
                hitBox->x = x - 9;
                hitBox->y = y - 35;
            }
            else
            {
                hitBox->x = x + 32;
                hitBox->y = y - 35;
            }
            break;
        case 2:
            hitBox->x = x + 70;
            hitBox->y = y + 59;
            break;
        case 3:
            hitBox->x = x + 30;
            hitBox->y = y - 150;
            break;
        case 4:
            hitBox->x = x - 47;
            hitBox->y = y + 59;
            break;
        case 5: //jellyshot
            hitBox->x = x;
            hitBox->y = y;
            direction = 4;
            break;
    }

    active = true;
}

void Bullet::move(float timestep)
{
    if(testing2)
    {
        test2();
    }

    if(active && !following && !inBlackHole)
    {
        switch ( direction )
        {
            case 1:
                hitBox->y -= speed * timestep;
                break;
            case 2:
                hitBox->x += speed * timestep;
                break;
            case 3:
                hitBox->y += speed * timestep;
                break;
            case 4:
                hitBox->x -= speed * timestep;
                break;
        }
    }


    if(testing)
    {
        test();
    }

    if(following)
    {
        follow();
    }

    if(inBlackHole)
    {
        followBlackHole();
    }
}

void Bullet::move2()
{
    if(testing2)
    {
        test2();
    }

    if(active)
    {
        switch ( direction )
    {
        case 1:
            hitBox->y -= speed2;
            break;
        case 2:
            hitBox->x += speed2;
            break;
        case 3:
            hitBox->y += speed2;
            break;
        case 4:
            hitBox->x -= speed2;
            break;
    }
    }


    if(testing)
    {
        test();
    }
}

void Bullet::update()
{
    xPos = hitBox->x;
    yPos = hitBox->y;
}

void Bullet::test()
{


    ///lol im just copying jump code
    if(!testTimer.isStarted())
    {
        testTimer.start();
    }

    hitBox->y -= 14;
    hitBox->y += 6.4;


    hitBox->y += testTimer.getTicks() / 2 * 0.16;


    if(testTimer.getTicks() >= 450)
    {
        testing = false;
        testTimer.stop();
        direction = 3;
    }
}

void Bullet::test2()
{
    if(!beingReflected)
    {
        if(direction == 2)
        {
            direction = 4;
            beingReflected = true;
        }
        else if(direction == 4)
        {
            direction = 2;
            beingReflected = true;
        }
        else if(direction == 1)
        {
            direction = 3;
            beingReflected = true;
        }
    }

    ///lol im just copying jump code
    if(!testTimer.isStarted())
    {
        testTimer.start();
    }

    hitBox->y -= 14;
    hitBox->y += 6.4;


    hitBox->y += testTimer.getTicks() / 2 * 0.16;


    if(testTimer.getTicks() >= 450)
    {
        testing2 = false;
        beingReflected = false;
        testTimer.stop();
        direction = 3;
    }
}

void Bullet::restart2(int x, int y, char drction) //so this like places the bullets in their starting position right after they get shot out of the gun? based on where the end of the barrel is which is based off of which direction the gun is???
{
    direction = drction;

    switch ( direction )
    {
        case 1:
            hitBox->x = x - 5;
            hitBox->y = y + 5;
            break;
        case 2:
            hitBox->x = x - 5;
            hitBox->y = y + 5;
            break;
        case 3:
            hitBox->x = x - 5;
            hitBox->y = y + 5;
            break;
        case 4:
            hitBox->x = x - 5;
            hitBox->y = y + 5;
            break;
    }

    active = true;
}

void Bullet::follow()
{
    if(deletionTimer.getTicks() > 800)
    {
        following = false;
        active = false;
    }

    if(hitBox->x < followingX)
    {
        hitBox->x += 1;
    }
    else if(hitBox->x > followingX)
    {
        hitBox->x -= 1;
    }

    if(hitBox->y < followingY)
    {
        hitBox->y += 1;
    }
    else if(hitBox->y > followingY)
    {
        hitBox->y -= 1;
    }

    if(jellySpeed > 0)
    {
        jellySpeed -= 30;
    }

    switch ( direction )
        {
            case 1:
                hitBox->y -= jellySpeed * 0.016;
                break;
            case 2:
                hitBox->x += jellySpeed * 0.016;
                break;
            case 3:
                hitBox->y += jellySpeed * 0.016;
                break;
            case 4:
                hitBox->x -= jellySpeed * 0.016;
                break;
        }
}

void Bullet::followBlackHole()
{
    if(hitBox->x < blackHoleX)
    {
        hitBox->x += rand() % 3 + 1;
    }
    else if(hitBox->x > blackHoleX)
    {
        hitBox->x -= rand() % 3 + 1;
    }

    if(hitBox->y < blackHoleY)
    {
        hitBox->y += rand() % 3 + 1;
    }
    else if(hitBox->y > blackHoleY)
    {
        hitBox->y -= rand() % 3 + 1;
    }
}
