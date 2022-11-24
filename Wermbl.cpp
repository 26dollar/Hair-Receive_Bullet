#include "Wermbl.h"

Wermbl::Wermbl(int x, int y)
{
    gacha1 = 1; ///if gacha is 1, it's explodey. 2 is floaty. 3 is shootey.
    gacha2 = 2;
    gacha3 = 3;

    hitBox = new SDL_Rect;
    hitBox->w = 65;
    hitBox->h = 35;
    hitBox->x = x;
    hitBox->y = y;

    xPos = 10;
    yPos = 10;

    hitBoxHead = new SDL_Rect;
    hitBoxHead->w = 25;
    hitBoxHead->h = 30;
    hitBoxHead->x = 0;
    hitBoxHead->y = 0;

    onFlatSurface = false;

    walking = true;
    walkingRight = false;
    flip = SDL_FLIP_NONE;

    gacha1x = 0;
    gacha1y = 0;
    gacha2x = 0;
    gacha2y = 0;
    gacha3x = 0;
    gacha3y = 0;

    numberOfBulletsEaten = 0;

    gachAvtivationState = 0;
    activatingColor = 255;

    flying = false;
    flyingUp = false;
    flyingHome = 0;
    amountToFly = 0;
    flyingSpeed = 40;

    for(int i = 0; i < 3; i++)
    {
        bullets[i] = new SDL_Rect;
        bullets[i]->w = 6;
        bullets[i]->h = 6;
        bullets[i]->x = 6;
        bullets[i]->y = 6;

    }
    for(int i = 0; i < 3; i++)
    {
        bulletactive[i] = false;
    }

    shooting = false;

    explosionHitbox = new SDL_Rect;
    explosionHitbox->w = 270;
    explosionHitbox->h = 130;
    explosionHitbox->x = x;
    explosionHitbox->y = y;

    explosionSoundBoolUgh = false;
}

Wermbl::~Wermbl()
{
    hitBox = NULL;
    hitBoxHead = NULL;
    explosionHitbox = NULL;

    for(int i = 0; i < 3; i++)
    {
        bullets[i] = NULL;
    }
}

void Wermbl::reset()
{
    hitBox->h = 35;
    shooting = false;
    flying = false;
    flyingUp = false;
    flyingHome = 0;
    amountToFly = 0;
    flyingSpeed = 40;
    numberOfBulletsEaten = 0;
    gachAvtivationState = 0;
    walking = true;
    walkingRight = false;
    flip = SDL_FLIP_NONE;

    gacha1 = rand() % 3 + 1;;
    gacha2 = rand() % 3 + 1;;
    gacha3 = rand() % 3 + 1;;

    gachaActivationTimer.stop();
    shootTimer.stop();
    explosionTimer.stop();
}

void Wermbl::move()
{
    flyingUp = false;
    flyingSpeed = 40;

    ///makes the dude fall from gravity
    if(!onFlatSurface && !flying && !shooting)
    {
        hitBox->y += 5; //fall from gravity
    }
    onFlatSurface = false;

    if(walking)
    {
        hitBox->x -=1;
    }
    else if(walkingRight)
    {
        hitBox->x +=1;
    }

    explosionSoundBoolUgh = false;

    if(gachAvtivationState == 0 && gachaActivationTimer.getTicks() > 2000)
    {
        gachaActivationTimer.stop();

        gachAvtivationState = 1;

        switch(numberOfBulletsEaten)
        {
            case 1:
                switch(gacha1)
                {
                    case 1: ///explodey
                            gachAvtivationState = 10;
                            explosionTimer.start();
                            explosionSoundBoolUgh = true;
                    break;
                    case 2: ///fly
                            flying = true;
                            gachAvtivationState = 20;
                    break;
                    case 3: ///shooty
                            gachAvtivationState = 30;
                            shooting = true;
                            shootTimer.start();
                    break;
                }
            break;
            case 2:
                switch(gacha2)
                {
                    case 1: ///explodey
                            gachAvtivationState = 10;
                            explosionTimer.start();
                            explosionSoundBoolUgh = true;
                    break;
                    case 2: ///fly
                            flying = true;
                            gachAvtivationState = 20;
                    break;
                    case 3: ///shooty
                            gachAvtivationState = 30;
                            shooting = true;
                            shootTimer.start();
                    break;
                }
            break;
            case 3:
                switch(gacha3)
                {
                    case 1: ///explodey
                            gachAvtivationState = 10;
                            explosionTimer.start();
                            explosionSoundBoolUgh = true;
                    break;
                    case 2: ///fly
                            flying = true;
                            gachAvtivationState = 20;
                    break;
                    case 3: ///shooty
                            gachAvtivationState = 30;
                            shooting = true;
                            shootTimer.start();
                    break;
                }
            break;
        }
    }

    if(flying)
    {
        fly();
    }

    if(shooting)
    {
        shoot();
    }

    for(int i = 0; i < 3; i++)
    {
        if(bulletactive[i])
        {
            if(flip == SDL_FLIP_HORIZONTAL)
            {
                bullets[i]->x += 5;
            }
            else
            {
                bullets[i]->x -= 5;
            }
        }
    }
}

void Wermbl::update()
{
    activatingColor = 255 - (gachaActivationTimer.getTicks() / 9);

    if(gachAvtivationState == 25)
    {
        if(flyingUp)
        {
            hitBox->y -= 2;
        }
        else if(hitBox->y < flyingHome)
        {
            hitBox->y += 2;
        }
    }

    if(flip == SDL_FLIP_NONE)
    {
        xPos = hitBox->x -25;
        yPos = hitBox->y -30;

        xPosFace = hitBoxHead->x +3;
        yPosFace = hitBoxHead->y +1;

        hitBoxHead->x = hitBox->x -25;
        hitBoxHead->y = hitBox->y -10;

        gacha1x = xPos + 28;
        gacha1y = yPos + 25;
        gacha2x = xPos + 47;
        gacha2y = gacha1y;
        gacha3x = xPos + 69;
        gacha3y = gacha1y;
    }
    else
    {
        xPos = hitBox->x - 10;
        yPos = hitBox->y - 30;

        xPosFace = hitBoxHead->x +3;
        yPosFace = hitBoxHead->y +1;

        hitBoxHead->x = hitBox->x +65;
        hitBoxHead->y = hitBox->y -10;

        gacha1x = xPos + 13;
        gacha1y = yPos + 25;
        gacha2x = xPos + 32;
        gacha2y = gacha1y;
        gacha3x = xPos + 54;
        gacha3y = gacha1y;
    }

    if(explosionTimer.getTicks() > 600)
    {
       explosionTimer.stop();
    }

    if(explosionTimer.isStarted())
    {
        explosionHitbox->x = hitBox->x - 100;
        explosionHitbox->y = hitBox->y - 90;
    }
}

void Wermbl::fly()
{
    switch(gachAvtivationState)
    {
        case 20: //flyingHome = hitBox->y;
                 amountToFly = 4;
                 hitBox->y += 1;
                 gachAvtivationState = 21;
        break;
        case 21:
                if(amountToFly > 0)
                {
                    hitBox->y += 1;
                    amountToFly--;
                }
                else
                {
                    gachAvtivationState = 22;
                    amountToFly = 15;
                }
        break;
        case 22:
                if(amountToFly > 0)
                {
                    if(amountToFly > 10)
                    {
                        hitBox->y -= 2;
                        amountToFly--;
                    }
                    else
                    {
                        hitBox->y -= 1;
                        amountToFly--;
                    }
                }
                else
                {
                    gachAvtivationState = 23;
                    amountToFly = 3;
                }

        break;
        case 23:
                if(amountToFly > 0)
                {
                    hitBox->y += 1;
                    amountToFly--;
                }
                else
                {
                    gachAvtivationState = 24;
                    flyingHome = hitBox->y;
                }
        break;
        case 24:
                hitBox->y -= 1;
                gachAvtivationState = 25;
        break;
    }
}

void Wermbl::shoot()
{
    switch(gachAvtivationState)
    {
        case 30:
                hitBox->y -= 3;
                if(shootTimer.getTicks() > 210)
                {
                    gachAvtivationState = 31;
                }
        break;
        case 31:
                hitBox->y -= 2;
                if(shootTimer.getTicks() > 360)
                {
                    gachAvtivationState = 32;
                }
        break;
        case 32:
                hitBox->y -= 1;
                if(shootTimer.getTicks() > 410)
                {
                    gachAvtivationState = 33;
                }
        break;
        case 33:
                if(shootTimer.getTicks() > 550)
                {
                    gachAvtivationState = 34;
                    hitBox->y -= 1;
                    ///play sound here?
                }
        break;
        case 34:
                bulletactive[0] = true;
                if(flip == SDL_FLIP_HORIZONTAL)
                {
                    bullets[0]->x = hitBoxHead->x -10;
                    bullets[0]->y = hitBoxHead->y +10;
                }
                else
                {
                    bullets[0]->x = hitBoxHead->x +10;
                    bullets[0]->y = hitBoxHead->y +10;
                }
                if(shootTimer.getTicks() > 600)
                {
                    gachAvtivationState = 35;
                }
        break;
        case 35:
                bulletactive[1] = true;
                if(flip == SDL_FLIP_HORIZONTAL)
                {
                    bullets[1]->x = hitBoxHead->x -10;
                    bullets[1]->y = hitBoxHead->y +10;
                }
                else
                {
                    bullets[1]->x = hitBoxHead->x +10;
                    bullets[1]->y = hitBoxHead->y +10;
                }
                if(shootTimer.getTicks() > 680)
                {
                    gachAvtivationState = 36;
                }
        break;
        case 36:
                bulletactive[2] = true;
                if(flip == SDL_FLIP_HORIZONTAL)
                {
                    bullets[2]->x = hitBoxHead->x -10;
                    bullets[2]->y = hitBoxHead->y +10;
                }
                else
                {
                    bullets[2]->x = hitBoxHead->x +10;
                    bullets[2]->y = hitBoxHead->y +10;
                }
                if(shootTimer.getTicks() > 760)
                {
                    gachAvtivationState = 37;
                }
        break;
        case 37:
                if(shootTimer.getTicks() > 900)
                {
                    shooting = false;
                    hitBox->h = 20;
                }
        break;
    }
}
