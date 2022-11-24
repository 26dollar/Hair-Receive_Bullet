#include "Enemy.h"

Enemy::Enemy(int x, int y)
{
    hitBox = new SDL_Rect;
    hitBox->w = 40;
    hitBox->h = 60;
    hitBox->x = x;
    hitBox->y = y;

    hitBoxLeft = new SDL_Rect;
    hitBoxLeft->w = 10;
    hitBoxLeft->h = 12;
    hitBoxLeft->x = 999;
    hitBoxLeft->y = 999;

    hitBoxRight = new SDL_Rect;
    hitBoxRight->w = 10;
    hitBoxRight->h = 16;
    hitBoxRight->x = 999;
    hitBoxRight->y = 999;

    xPos = 10;
    yPos = 10;

    xPosLeft = -90;
    yPosLeft = -90;

    xPosRight = -90;
    yPosRight = -90;

    home = x;

    onFlatSurface = false;
    patrolRight = true;
    patrolLeft = false;

    leftFalling = false;
    rightFalling = false;

    leftActive = true;
    rightActive = true;

    health = 2;
    leftHealth = 3;
    rightHealth = 3;
    alive = true;
}

Enemy::~Enemy()
{
    hitBox = NULL;
    hitBoxLeft = NULL;
    hitBoxRight = NULL;
}

void Enemy::reset()
{
    alive = true;
    health = 2;
    leftHealth = 3;
    rightHealth = 3;
    respawnTimer.stop();
    leftActive = true;
    leftFalling = false;
    rightActive = true;
    rightFalling = true;
}

void Enemy::move()
{
    ///makes the dude fall from gravity
    if(!onFlatSurface)
    {
        hitBox->y += 6; //fall from gravity
    }
    onFlatSurface = false;

    if(alive)
    {
        ///makes the dude patrol left and right
        if(hitBox->x > home + 90)
        {
            patrolLeft = true;
            patrolRight = false;
        }

        if(hitBox->x < home - 90)
        {
            patrolRight = true;
            patrolLeft = false;
        }

        if(patrolRight)
        {
            hitBox->x += 2;
        }
        else
        {
            hitBox->x -= 2;
        }
    }



}

void Enemy::update(int animation)
{


    xPos = hitBox->x -26;
    yPos = hitBox->y -5;

    if(leftActive)
    {
        if(animation < 60) ///based on the current frame of animation, the hitbox moves
        {
            hitBoxLeft->x = hitBox->x - 18;
            hitBoxLeft->y = hitBox->y + 16;
        }
        else if(animation < 500)
        {
            hitBoxLeft->x = hitBox->x - 13;
            hitBoxLeft->y = hitBox->y;
        }
        else if(animation > 650 && animation < 1300)
        {
            hitBoxLeft->x = hitBox->x - 24;
            hitBoxLeft->y = hitBox->y + 34;
        }
        else
        {
            hitBoxLeft->x = hitBox->x - 18;
            hitBoxLeft->y = hitBox->y + 16;
        }
    }

    if(leftHealth < 1)
    {
        if(leftActive)
        {
            xPosLeft = xPos;
            yPosLeft = yPos;
            leftFalling = true;
        }
        leftActive = false;
    }

    if(rightActive)
    {
        if(animation < 60) ///based on the current frame of animation, the hitbox moves
        {
            hitBoxRight->x = hitBox->x + 45;
            hitBoxRight->y = hitBox->y + 18;
        }
        else if(animation < 500)
        {
            hitBoxRight->x = hitBox->x + 53;
            hitBoxRight->y = hitBox->y + 33;
        }
        else if(animation > 650 && animation < 900)
        {
            hitBoxRight->x = hitBox->x + 36;
            hitBoxRight->y = hitBox->y;
        }
        else
        {
            hitBoxRight->x = hitBox->x + 45;
            hitBoxRight->y = hitBox->y + 18;
        }
    }

    if(rightHealth < 1)
    {
        if(rightActive)
        {
            xPosRight = xPos;
            yPosRight = yPos;
            rightFalling = true;
        }
        rightActive = false;
    }

    if(health < 1 && !respawnTimer.isStarted())
    {
        respawnTimer.start();
        alive = false;
        leftHealth = 0;
        rightHealth = 0;
    }

    //check early build here

    if(respawnTimer.getTicks() > 3500) ///enemy respawns after 3500ms
    {
        reset();
    }

    if(leftFalling)
    {
        hitBoxLeft->y += 3;
        yPosLeft += 3;
    }

    if(rightFalling)
    {
        hitBoxRight->y += 3;
        yPosRight += 3;
    }

    if(hitColorLeft.getTicks() > 100)
    {
        hitColorLeft.stop();
    }

    if(hitColorRight.getTicks() > 100)
    {
        hitColorRight.stop();
    }

    if(hitColorBody.getTicks() > 80)
    {
        hitColorBody.stop();
    }
}
