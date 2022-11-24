#include "KongaLee.h"

KongaLee::KongaLee(int x, int y, int type)
{
    hitBoxBody = new SDL_Rect;
    hitBoxBody->w = 40;
    hitBoxBody->h = 40;
    hitBoxBody->x = x;
    hitBoxBody->y = y;

    hitBoxFace = new SDL_Rect;
    hitBoxFace->w = 15;
    hitBoxFace->h = 15;
    hitBoxFace->x = -9999;
    hitBoxFace->y = -9999;

    hitBoxFoot = new SDL_Rect;
    hitBoxFoot->w = 15;
    hitBoxFoot->h = 15;
    hitBoxFoot->x = -9999;
    hitBoxFoot->y = -9999;

    hitBoxBackFoot = new SDL_Rect;
    hitBoxBackFoot->w = 15;
    hitBoxBackFoot->h = 15;
    hitBoxBackFoot->x = -9999;
    hitBoxBackFoot->y = -9999;

    xPos = 10;
    yPos = 10;

    variant = type;

    alive = true;
    onFloorGoingLeft = false;
    onFloorGoingRight = false;
    onLeftWallGoingUp = false;
    onLeftWallGoingDown = false;
    onRightWallGoingUp = false;
    onRightWallGoingDown = false;
    onCeilingGoingLeft = false;
    onCeilingGoingRight = false;

    faceHasDetectedASurface = false;
    footHasDetectedASurface = false;

    freeFall = true;

    step = 0;
    angle = 0;
    footSpeed = 80;

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
    for(int i = 0; i < 3; i++)
    {
        bulletSpeedX[i] = 500;
    }
    for(int i = 0; i < 3; i++)
    {
        bulletSpeedY[i] = 500;
    }

    spitting = false;

    exploded = false;
    explosionHitbox = new SDL_Rect;
    explosionHitbox->w = 320;
    explosionHitbox->h = 320;
    explosionHitbox->x = x;
    explosionHitbox->y = y;

    executingTurn_GOINGLEFT_ONFLOOR_TO_LEFTWALL = false;
    executingTurn_GOINGDOWN_ONLEFTWALL_TO_CEILING = false;
    executingTurn_GOINGRIGHT_ONCEILING_TO_RIGHTWALL = false;
    executingTurn_GOINGUP_ONRIGHTWALL_TO_FLOOR = false;

    executingTurn_GOINGLEFT_ONFLOOR_TO_RIGHTWALL = false;
    executingTurn_GOINGUP_ONRIGHTWALL_TO_CEILING = false;
    executingTurn_GOINGRIGHT_ONCEILING_TO_LEFTWALL = false;
    executingTurn_GOINGDOWN_ONLEFTWALL_TO_FLOOR = false;
}

KongaLee::~KongaLee()
{
    hitBoxBody = NULL;
    hitBoxFace = NULL;
    hitBoxFoot = NULL;
    hitBoxBackFoot = NULL;
    explosionHitbox = NULL;

    for(int i = 0; i < 3; i++)
    {
        bullets[i] = NULL;
    }
}

void KongaLee::reset()
{
    variant = rand() % 3;

    alive = true;
    onFloorGoingLeft = false;
    onFloorGoingRight = false;
    onLeftWallGoingUp = false;
    onLeftWallGoingDown = false;
    onRightWallGoingUp = false;
    onRightWallGoingDown = false;
    onCeilingGoingLeft = false;
    onCeilingGoingRight = false;

    faceHasDetectedASurface = false;
    footHasDetectedASurface = false;

    freeFall = true;

    step = 0;
    angle = 0;
    footSpeed = 80;

    spitting = false;
    exploded = false;

    executingTurn_GOINGLEFT_ONFLOOR_TO_LEFTWALL = false;
    executingTurn_GOINGDOWN_ONLEFTWALL_TO_CEILING = false;
    executingTurn_GOINGRIGHT_ONCEILING_TO_RIGHTWALL = false;
    executingTurn_GOINGUP_ONRIGHTWALL_TO_FLOOR = false;

    executingTurn_GOINGLEFT_ONFLOOR_TO_RIGHTWALL = false;
    executingTurn_GOINGUP_ONRIGHTWALL_TO_CEILING = false;
    executingTurn_GOINGRIGHT_ONCEILING_TO_LEFTWALL = false;
    executingTurn_GOINGDOWN_ONLEFTWALL_TO_FLOOR = false;

    for(int i = 0; i < 3; i++)
    {
        bulletactive[i] = false;
    }
    for(int i = 0; i < 3; i++)
    {
        bulletSpeedX[i] = 500;
    }
    for(int i = 0; i < 3; i++)
    {
        bulletSpeedY[i] = 500;
    }

    shootTimer.stop();
    explosionTimer.stop();
}

void KongaLee::move()
{
    if(freeFall)
    {
        hitBoxBody->y += 4; //fall from gravity
    }

    if(alive)
    {
       if(onFloorGoingLeft)
       {
           hitBoxBody->x -= 1;
       }

       if(onLeftWallGoingDown)
       {
           hitBoxBody->y += 1;
       }

       if(onCeilingGoingRight)
       {
           hitBoxBody->x += 1;
       }

       if(onRightWallGoingUp)
       {
           hitBoxBody->y -= 1;
       }
    }

    if(executingTurn_GOINGLEFT_ONFLOOR_TO_LEFTWALL)
    {
        goingleftonfloortoleftwall();
    }

    if(executingTurn_GOINGDOWN_ONLEFTWALL_TO_CEILING)
    {
        goingdownonleftwalltoceling();
    }

    if(executingTurn_GOINGRIGHT_ONCEILING_TO_RIGHTWALL)
    {
        goingrightonceilingtorightwall();
    }

    if(executingTurn_GOINGUP_ONRIGHTWALL_TO_FLOOR)
    {
        goinguponrightwalltofloor();
    }

    if(executingTurn_GOINGLEFT_ONFLOOR_TO_RIGHTWALL)
    {
        goingleftonfloortorightwall();
    }

    if(executingTurn_GOINGUP_ONRIGHTWALL_TO_CEILING)
    {
        goinguponrightwalltoceiling();
    }

    if(executingTurn_GOINGRIGHT_ONCEILING_TO_LEFTWALL)
    {
        goingrightonceilingtoleftwall();
    }

    if(executingTurn_GOINGDOWN_ONLEFTWALL_TO_FLOOR)
    {
        goingdownonleftwalltofloor();
    }

    footHasDetectedASurface = false;
    faceHasDetectedASurface = false;

    if(spitting)
    {
        ///shoot bullets to the left
        if(angle == -90 || angle == 270)
        {
            for(int i = 0; i < 3; i++)
            {
                if(bulletactive[i])
                {
                    bullets[i]->x -= bulletSpeedX[i] / 50;
                    bullets[i]->y += 5;
                    if(bulletSpeedX[i] > 0)
                    {
                        bulletSpeedX[i] -= 20;
                    }
                }
            }


            if(!bulletactive[0] && shootTimer.getTicks() > 100)
            {
                bulletactive[0] = true;
                bullets[0]->x = hitBoxBody->x + 20;
                bullets[0]->y = hitBoxBody->y + 10;
            }
            if(!bulletactive[1] &&shootTimer.getTicks() > 250)
            {
                bulletactive[1] = true;
                bullets[1]->x = hitBoxBody->x + 20;
                bullets[1]->y = hitBoxBody->y + 10;
            }
            if(!bulletactive[2] &&shootTimer.getTicks() > 350)
            {
                bulletactive[2] = true;
                bullets[2]->x = hitBoxBody->x + 20;
                bullets[2]->y = hitBoxBody->y + 10;
                shootTimer.stop();
            }
        }
        ///shoot bullets to the right
        if(angle == -270 || angle == 90)
        {
            for(int i = 0; i < 3; i++)
            {
                if(bulletactive[i])
                {
                    bullets[i]->x += bulletSpeedX[i] / 50;
                    bullets[i]->y += 5;
                    if(bulletSpeedX[i] > 0)
                    {
                        bulletSpeedX[i] -= 20;
                    }
                }
            }


            if(!bulletactive[0] && shootTimer.getTicks() > 100)
            {
                bulletactive[0] = true;
                bullets[0]->x = hitBoxBody->x + 20;
                bullets[0]->y = hitBoxBody->y + 10;
            }
            if(!bulletactive[1] &&shootTimer.getTicks() > 250)
            {
                bulletactive[1] = true;
                bullets[1]->x = hitBoxBody->x + 20;
                bullets[1]->y = hitBoxBody->y + 10;
            }
            if(!bulletactive[2] &&shootTimer.getTicks() > 350)
            {
                bulletactive[2] = true;
                bullets[2]->x = hitBoxBody->x + 20;
                bullets[2]->y = hitBoxBody->y + 10;
                shootTimer.stop();
            }
        }
        ///shoot bullets downward
        if(angle == -180 || angle == 180)
        {
            for(int i = 0; i < 3; i++)
            {
                if(bulletactive[i])
                {
                    bullets[i]->y += bulletSpeedY[i] / 150;
                    bulletSpeedY[i] += 80;
                }
            }


            if(!bulletactive[0] && shootTimer.getTicks() > 100)
            {
                bulletactive[0] = true;
                bullets[0]->x = hitBoxBody->x + 10;
                bullets[0]->y = hitBoxBody->y + 15;
            }
            if(!bulletactive[1] &&shootTimer.getTicks() > 250)
            {
                bulletactive[1] = true;
                bullets[1]->x = hitBoxBody->x + 10;
                bullets[1]->y = hitBoxBody->y + 15;
            }
            if(!bulletactive[2] &&shootTimer.getTicks() > 350)
            {
                bulletactive[2] = true;
                bullets[2]->x = hitBoxBody->x + 10;
                bullets[2]->y = hitBoxBody->y + 15;
                shootTimer.stop();
            }
        }
        ///shoot bullets upward
        if(angle == 0 || angle == -360 || angle == 360)
        {
            for(int i = 0; i < 3; i++)
            {
                if(bulletactive[i])
                {
                    bullets[i]->y -= bulletSpeedY[i] / 30;
                    bulletSpeedY[i] -= 17;
                }
            }


            if(!bulletactive[0] && shootTimer.getTicks() > 100)
            {
                bulletactive[0] = true;
                bullets[0]->x = hitBoxBody->x + 19;
                bullets[0]->y = hitBoxBody->y + 15;
            }
            if(!bulletactive[1] &&shootTimer.getTicks() > 250)
            {
                bulletactive[1] = true;
                bullets[1]->x = hitBoxBody->x + 19;
                bullets[1]->y = hitBoxBody->y + 15;
            }
            if(!bulletactive[2] &&shootTimer.getTicks() > 350)
            {
                bulletactive[2] = true;
                bullets[2]->x = hitBoxBody->x + 19;
                bullets[2]->y = hitBoxBody->y + 15;
                shootTimer.stop();
            }
        }
        ///shoot bullets North West
        if(angle < -1 && angle > -90 || angle > 270 && angle < 360)
        {
            for(int i = 0; i < 3; i++)
            {
                if(bulletactive[i])
                {
                    bullets[i]->x -= bulletSpeedX[i] / 50;
                    bullets[i]->y -= bulletSpeedY[i] / 50;
                    bullets[i]->y += 6;
                    if(bulletSpeedX[i] > 0)
                    {
                        bulletSpeedX[i] -= 15;
                    }
                    if(bulletSpeedY[i] > 0)
                    {
                        bulletSpeedY[i] -= 15;
                    }
                }
            }


            if(!bulletactive[0] && shootTimer.getTicks() > 100)
            {
                bulletactive[0] = true;
                bullets[0]->x = hitBoxBody->x + 19;
                bullets[0]->y = hitBoxBody->y + 15;
            }
            if(!bulletactive[1] &&shootTimer.getTicks() > 250)
            {
                bulletactive[1] = true;
                bullets[1]->x = hitBoxBody->x + 19;
                bullets[1]->y = hitBoxBody->y + 15;
            }
            if(!bulletactive[2] &&shootTimer.getTicks() > 350)
            {
                bulletactive[2] = true;
                bullets[2]->x = hitBoxBody->x + 19;
                bullets[2]->y = hitBoxBody->y + 15;
                shootTimer.stop();
            }
        }
        ///shoot bullets SW
        if(angle < -90 && angle > -180 || angle > 180 && angle < 270)
        {
            for(int i = 0; i < 3; i++)
            {
                if(bulletactive[i])
                {
                    bullets[i]->x -= bulletSpeedX[i] / 50;
                    bullets[i]->y += 6;
                    if(bulletSpeedX[i] > 0)
                    {
                        bulletSpeedX[i] -= 25;
                    }
                }
            }


            if(!bulletactive[0] && shootTimer.getTicks() > 100)
            {
                bulletactive[0] = true;
                bullets[0]->x = hitBoxBody->x + 19;
                bullets[0]->y = hitBoxBody->y + 15;
            }
            if(!bulletactive[1] &&shootTimer.getTicks() > 250)
            {
                bulletactive[1] = true;
                bullets[1]->x = hitBoxBody->x + 19;
                bullets[1]->y = hitBoxBody->y + 15;
            }
            if(!bulletactive[2] &&shootTimer.getTicks() > 350)
            {
                bulletactive[2] = true;
                bullets[2]->x = hitBoxBody->x + 19;
                bullets[2]->y = hitBoxBody->y + 15;
                shootTimer.stop();
            }
        }
        ///shoot bullets SE
        if(angle < -180 && angle > -270 || angle > 90 && angle < 180)
        {
            for(int i = 0; i < 3; i++)
            {
                if(bulletactive[i])
                {
                    bullets[i]->x += bulletSpeedX[i] / 50;
                    bullets[i]->y += 6;
                    if(bulletSpeedX[i] > 0)
                    {
                        bulletSpeedX[i] -= 25;
                    }
                }
            }


            if(!bulletactive[0] && shootTimer.getTicks() > 100)
            {
                bulletactive[0] = true;
                bullets[0]->x = hitBoxBody->x + 19;
                bullets[0]->y = hitBoxBody->y + 15;
            }
            if(!bulletactive[1] &&shootTimer.getTicks() > 250)
            {
                bulletactive[1] = true;
                bullets[1]->x = hitBoxBody->x + 19;
                bullets[1]->y = hitBoxBody->y + 15;
            }
            if(!bulletactive[2] &&shootTimer.getTicks() > 350)
            {
                bulletactive[2] = true;
                bullets[2]->x = hitBoxBody->x + 19;
                bullets[2]->y = hitBoxBody->y + 15;
                shootTimer.stop();
            }
        }
        ///shoot bullets SE
        if(angle < -270 && angle > -360 || angle > 0 && angle < 90)
        {
            for(int i = 0; i < 3; i++)
            {
                if(bulletactive[i])
                {
                    bullets[i]->x += bulletSpeedX[i] / 50;
                    bullets[i]->y -= bulletSpeedY[i] / 50;
                    bullets[i]->y += 6;
                    if(bulletSpeedX[i] > 0)
                    {
                        bulletSpeedX[i] -= 15;
                    }
                    if(bulletSpeedY[i] > 0)
                    {
                        bulletSpeedY[i] -= 15;
                    }
                }
            }


            if(!bulletactive[0] && shootTimer.getTicks() > 100)
            {
                bulletactive[0] = true;
                bullets[0]->x = hitBoxBody->x + 19;
                bullets[0]->y = hitBoxBody->y + 15;
            }
            if(!bulletactive[1] &&shootTimer.getTicks() > 250)
            {
                bulletactive[1] = true;
                bullets[1]->x = hitBoxBody->x + 19;
                bullets[1]->y = hitBoxBody->y + 15;
            }
            if(!bulletactive[2] &&shootTimer.getTicks() > 350)
            {
                bulletactive[2] = true;
                bullets[2]->x = hitBoxBody->x + 19;
                bullets[2]->y = hitBoxBody->y + 15;
                shootTimer.stop();
            }
        }
    }
}

void KongaLee::update()
{
    xPos = hitBoxBody->x;
    yPos = hitBoxBody->y;

    if(explosionTimer.isStarted())
    {
        explosionHitbox->x = hitBoxBody->x - 140;
        explosionHitbox->y = hitBoxBody->y - 140;
    }

    if(explosionTimer.getTicks() > 900)
    {
        explosionTimer.stop();
    }

    if(onFloorGoingLeft)
    {
        hitBoxFace->x = hitBoxBody->x - 15;
        hitBoxFace->y = hitBoxBody->y + 10;

        hitBoxFoot->x = hitBoxBody->x;
        hitBoxFoot->y = hitBoxBody->y + 34;

        if(!footHasDetectedASurface)
        {
            executingTurn_GOINGLEFT_ONFLOOR_TO_LEFTWALL = true;
            onFloorGoingLeft = false;
        }

        if(faceHasDetectedASurface)
        {
            executingTurn_GOINGLEFT_ONFLOOR_TO_RIGHTWALL = true;
            onFloorGoingLeft = false;
        }
    }

    if(onLeftWallGoingDown)
    {
        hitBoxFace->x = hitBoxBody->x + 20;
        hitBoxFace->y = hitBoxBody->y + 40;

        hitBoxFoot->x = hitBoxBody->x + 40;
        hitBoxFoot->y = hitBoxBody->y + 27;

        if(!footHasDetectedASurface)
        {
            executingTurn_GOINGDOWN_ONLEFTWALL_TO_CEILING = true;
            onLeftWallGoingDown = false;
        }

        if(faceHasDetectedASurface)
        {
            executingTurn_GOINGDOWN_ONLEFTWALL_TO_FLOOR = true;
            onLeftWallGoingDown = false;
        }
    }

    if(onCeilingGoingRight)
    {
        hitBoxFace->x = hitBoxBody->x + 40;
        hitBoxFace->y = hitBoxBody->y + 5;

        hitBoxFoot->x = hitBoxBody->x + 25;
        hitBoxFoot->y = hitBoxBody->y - 14;

        if(!footHasDetectedASurface)
        {
            executingTurn_GOINGRIGHT_ONCEILING_TO_RIGHTWALL = true;
            onCeilingGoingRight = false;
        }

        if(faceHasDetectedASurface)
        {
            executingTurn_GOINGRIGHT_ONCEILING_TO_LEFTWALL = true;
            onCeilingGoingRight = false;
        }

    }

    if(onRightWallGoingUp)
    {
        hitBoxFace->x = hitBoxBody->x + 5;
        hitBoxFace->y = hitBoxBody->y - 15;

        hitBoxFoot->x = hitBoxBody->x - 14;
        hitBoxFoot->y = hitBoxBody->y;

        if(!footHasDetectedASurface)
        {
            executingTurn_GOINGUP_ONRIGHTWALL_TO_FLOOR = true;
            onRightWallGoingUp = false;
        }

        if(faceHasDetectedASurface)
        {
            executingTurn_GOINGUP_ONRIGHTWALL_TO_CEILING = true;
            onRightWallGoingUp = false;
        }
    }
}

void KongaLee::goingleftonfloortoleftwall()
{
    switch(step)
    {
        case 0: angle = -2;
                hitBoxBody->x -= 1;
                step++;
                footSpeed = 50;
        break;
        case 1: angle = -4;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 2:
                angle = -6;
                hitBoxBody->x -= 1;
                hitBoxBody->y += 1;
                step++;
        break;
                step++;
        case 3:
                angle = -8;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 4:
                angle = -10;
                hitBoxBody->x -= 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 5:
                angle = -12;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 6:
                angle = -14;
                hitBoxBody->x -= 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 7: angle = -16;
                hitBoxBody->x -= 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 8:
                angle = -18;
                hitBoxBody->x -= 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 9:
                angle = -20;
                hitBoxBody->y += 1;
                step++;
        break;
        case 10:
                angle = -22;
                hitBoxBody->y += 1;
                step++;
        break;
        case 11:
                angle = -24;
                hitBoxBody->y += 1;
                step++;
        break;
        case 12:
                angle = -26;
                hitBoxBody->y += 1;
                step++;
        break;
        case 13:
                angle = -28;
                hitBoxBody->x -= 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 14:
                angle = -30;
                hitBoxBody->y += 1;
                step++;
        break;
        case 15:
                angle = -33;
                hitBoxBody->x -= 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 16:
                angle = -36;
                hitBoxBody->x -= 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 17:
                angle = -38;
                hitBoxBody->x -= 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 18:
                angle = -40;
                hitBoxBody->y += 1;
                step++;
        break;
        case 19:
                angle = -42;
                hitBoxBody->x -= 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 20:
                angle = -44;
                hitBoxBody->y += 1;
                step++;
        break;
        case 21:
                angle = -46;
                hitBoxBody->x -= 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 22:
                angle = -48;
                hitBoxBody->y += 1;
                step++;
        break;
        case 23:
                angle = -50;
                hitBoxBody->x -= 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 24:
                angle = -52;
                hitBoxBody->x -= 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 25:
                angle = -54;
                hitBoxBody->y += 1;
                step++;
        break;
        case 26:
                angle = -56;
                hitBoxBody->x -= 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 27:
                angle = -57;
                hitBoxBody->x -= 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 28:
                angle = -58;
                hitBoxBody->x -= 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 29:
                angle = -60;
                hitBoxBody->y += 1;
                step++;
        break;
        case 30:
                angle = -63;
                hitBoxBody->y += 1;
                step++;
        break;
        case 31:
                angle = -65;
                hitBoxBody->y += 1;
                step++;
        break;
        case 32:
                angle = -68;
                hitBoxBody->y += 1;
                step++;
        break;
        case 33:
                angle = -70;
                hitBoxBody->y += 1;
                step++;
        break;
        case 34:
                angle = -73;
                hitBoxBody->y += 1;
                step++;
        break;
        case 35:
                angle = -76;
                hitBoxBody->x -= 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 36:
                angle = -79;
                hitBoxBody->x -= 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 37:
                angle = -82;
                hitBoxBody->x -= 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 38:
                angle = -84;
                hitBoxBody->y += 1;
                step++;
        break;
        case 39:
                angle = -86;
                hitBoxBody->y += 1;
                step++;
        break;
        case 40:
                angle = -86;
                hitBoxBody->y += 1;
                step++;
        break;
        case 41:
                angle = -88;
                hitBoxBody->y += 1;
                step++;
        break;
        case 42:
                angle = -90;
                hitBoxBody->y += 1;
                step++;
        break;
        case 43:
                executingTurn_GOINGLEFT_ONFLOOR_TO_LEFTWALL = false;
                onLeftWallGoingDown = true;
                hitBoxFace->x = hitBoxBody->x + 20;
                hitBoxFace->y = hitBoxBody->y + 40;

                hitBoxFoot->x = hitBoxBody->x + 40;
                hitBoxFoot->y = hitBoxBody->y + 27;
                step = 0;
                footSpeed = 80;
        break;
    }
}

void KongaLee::goingdownonleftwalltoceling()
{
    switch(step)
    {
        case 0: angle = -92;
                hitBoxBody->x += 1;
                step++;
                footSpeed = 50;
        break;
        case 1: angle = -94;
                hitBoxBody->x += 1;
                step++;
        break;
        case 2:
                angle = -96;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
                step++;
        case 3:
                angle = -98;
                hitBoxBody->x += 1;
                step++;
        break;
        case 4:
                angle = -100;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 5:
                angle = -102;
                hitBoxBody->x += 1;
                step++;
        break;
        case 6:
                angle = -104;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 7: angle = -106;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 8:
                angle = -108;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 9:
                angle = -110;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 10:
                angle = -112;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 11:
                angle = -114;
                hitBoxBody->y += 1;
                step++;
        break;
        case 12:
                angle = -116;
                hitBoxBody->y += 1;
                step++;
        break;
        case 13:
                angle = -118;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 14:
                angle = -120;
                hitBoxBody->y += 1;
                step++;
        break;
        case 15:
                angle = -123;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 16:
                angle = -126;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 17:
                angle = -128;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 18:
                angle = -130;
                hitBoxBody->y += 1;
                step++;
        break;
        case 19:
                angle = -132;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 20:
                angle = -134;
                hitBoxBody->x += 1;
                step++;
        break;
        case 21:
                angle = -136;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 22:
                angle = -138;
                hitBoxBody->x += 1;
                step++;
        break;
        case 23:
                angle = -140;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 24:
                angle = -142;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 25:
                angle = -144;
                hitBoxBody->x += 1;
                step++;
        break;
        case 26:
                angle = -146;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 27:
                angle = -147;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 28:
                angle = -148;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 29:
                angle = -150;
                hitBoxBody->x += 1;
                step++;
        break;
        case 30:
                angle = -153;
                hitBoxBody->x += 1;;
                step++;
        break;
        case 31:
                angle = -155;
                hitBoxBody->x += 1;
                step++;
        break;
        case 32:
                angle = -158;
                hitBoxBody->x += 1;
                step++;
        break;
        case 33:
                angle = -160;
                hitBoxBody->x += 1;
                step++;
        break;
        case 34:
                angle = -163;
                hitBoxBody->x += 1;
                step++;
        break;
        case 35:
                angle = -166;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 36:
                angle = -169;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 37:
                angle = -172;
                hitBoxBody->x += 1;
                hitBoxBody->y += 1;
                step++;
        break;
        case 38:
                angle = -174;
                hitBoxBody->x += 1;
                step++;
        break;
        case 39:
                angle = -176;
                hitBoxBody->x += 1;
                step++;
        break;
        case 40:
                angle = -176;
                hitBoxBody->x += 1;
                step++;
        break;
        case 41:
                angle = -178;
                hitBoxBody->x += 1;
                step++;
        break;
        case 42:
                angle = -180;
                hitBoxBody->x += 1;
                step++;
        break;
        case 43:
                executingTurn_GOINGDOWN_ONLEFTWALL_TO_CEILING = false;
                onCeilingGoingRight = true;
                hitBoxFace->x = hitBoxBody->x + 40;
                hitBoxFace->y = hitBoxBody->y + 5;

                hitBoxFoot->x = hitBoxBody->x + 25;
                hitBoxFoot->y = hitBoxBody->y - 14;
                step = 0;
                footSpeed = 80;
        break;
    }
}

void KongaLee::goingrightonceilingtorightwall()
{
    switch(step)
    {
        case 0: angle = -182;
                hitBoxBody->x += 1;
                step++;
                footSpeed = 50;
        break;
        case 1: angle = -184;
                hitBoxBody->x += 1;
                step++;
        break;
        case 2:
                angle = -186;
                hitBoxBody->x += 1;
                hitBoxBody->y -= 1;
                step++;
        break;
                step++;
        case 3:
                angle = -188;
                hitBoxBody->x += 1;
                step++;
        break;
        case 4:
                angle = -190;
                hitBoxBody->x += 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 5:
                angle = -192;
                hitBoxBody->x += 1;
                step++;
        break;
        case 6:
                angle = -194;
                hitBoxBody->x += 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 7: angle = -196;
                hitBoxBody->x += 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 8:
                angle = -198;
                hitBoxBody->x += 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 9:
                angle = -200;
                hitBoxBody->x += 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 10:
                angle = -202;
                hitBoxBody->x += 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 11:
                angle = -204;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 12:
                angle = -206;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 13:
                angle = -208;
                hitBoxBody->x += 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 14:
                angle = -210;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 15:
                angle = -213;
                hitBoxBody->x += 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 16:
                angle = -216;
                hitBoxBody->x += 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 17:
                angle = -218;
                hitBoxBody->x += 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 18:
                angle = -220;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 19:
                angle = -222;
                hitBoxBody->x += 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 20:
                angle = -224;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 21:
                angle = -226;
                hitBoxBody->x += 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 22:
                angle = -228;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 23:
                angle = -230;
                hitBoxBody->x += 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 24:
                angle = -232;
                hitBoxBody->x += 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 25:
                angle = -234;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 26:
                angle = -236;
                hitBoxBody->x += 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 27:
                angle = -237;
                hitBoxBody->x += 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 28:
                angle = -238;
                hitBoxBody->x += 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 29:
                angle = -240;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 30:
                angle = -243;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 31:
                angle = -245;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 32:
                angle = -248;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 33:
                angle = -250;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 34:
                angle = -253;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 35:
                angle = -256;
                hitBoxBody->x += 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 36:
                angle = -259;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 37:
                angle = -262;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 38:
                angle = -264;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 39:
                angle = -266;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 40:
                angle = -266;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 41:
                angle = -268;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 42:
                angle = -270;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 43:
                executingTurn_GOINGRIGHT_ONCEILING_TO_RIGHTWALL = false;
                onRightWallGoingUp = true;
                hitBoxFace->x = hitBoxBody->x + 5;
                hitBoxFace->y = hitBoxBody->y - 15;

                hitBoxFoot->x = hitBoxBody->x - 14;
                hitBoxFoot->y = hitBoxBody->y;
                step = 0;
                footSpeed = 80;
        break;
    }
}

void KongaLee::goinguponrightwalltofloor()
{
    switch(step)
    {
        case 0: angle = -270;
                hitBoxBody->x -= 1;
                step++;
                footSpeed = 50;
        break;
        case 1: angle = -274;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 2:
                angle = -276;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
                step++;
        case 3:
                angle = -278;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 4:
                angle = -280;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 5:
                angle = -282;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 6:
                angle = -284;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 7: angle = -286;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 8:
                angle = -288;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 9:
                angle = -290;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 10:
                angle = -292;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 11:
                angle = -294;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 12:
                angle = -296;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 13:
                angle = -298;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 14:
                angle = -300;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 15:
                angle = -303;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 16:
                angle = -306;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 17:
                angle = -308;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 18:
                angle = -310;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 19:
                angle = -312;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 20:
                angle = -314;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 21:
                angle = -316;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 22:
                angle = -318;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 23:
                angle = -320;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 24:
                angle = -322;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 25:
                angle = -324;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 26:
                angle = -326;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 27:
                angle = -327;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 28:
                angle = -328;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 29:
                angle = -330;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 30:
                angle = -333;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 31:
                angle = -335;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 32:
                angle = -338;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 33:
                angle = -340;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 34:
                angle = -343;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 35:
                angle = -346;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 36:
                angle = -349;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 37:
                angle = -352;
                hitBoxBody->x -= 1;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 38:
                angle = -354;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 39:
                angle = -356;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 40:
                angle = -356;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 41:
                angle = -358;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 42:
                angle = -360;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 43:
                executingTurn_GOINGUP_ONRIGHTWALL_TO_FLOOR = false;
                onFloorGoingLeft = true;
                hitBoxFace->x = hitBoxBody->x - 15;
                hitBoxFace->y = hitBoxBody->y + 10;

                hitBoxFoot->x = hitBoxBody->x;
                hitBoxFoot->y = hitBoxBody->y + 34;
                step = 0;
                footSpeed = 80;
        break;
    }
}

void KongaLee::goingleftonfloortorightwall()
{
    switch(step)
    {
        case 0: angle = 2;
                hitBoxBody->x -= 1;
                step++;
                footSpeed = 50;
        break;
        case 1: angle = 4;
                step++;
        break;
        case 2:
                angle = 6;
                step++;
        break;
        case 3:
                angle = 8;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 4:
                angle = 10;

                step++;
        break;
        case 5:
                angle = 12;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 6:
                angle = 14;

                step++;
        break;
        case 7: angle = 16;

                step++;
        break;
        case 8:
                angle = 18;
                hitBoxBody->x -= 1;

                step++;
        break;
        case 9:
                angle = 20;

                step++;
        break;
        case 10:
                angle = 22;

                step++;
        break;
        case 11:
                angle = 24;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 12:
                angle = 26;

                step++;
        break;
        case 13:
                angle = 28;
                hitBoxBody->x -= 1;

                step++;
        break;
        case 14:
                angle = 30;

                step++;
        break;
        case 15:
                angle = 33;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 16:
                angle = 36;

                step++;
        break;
        case 17:
                angle = 38;
                hitBoxBody->x -= 1;

                step++;
        break;
        case 18:
                angle = 40;

                step++;
        break;
        case 19:
                angle = 42;

                step++;
        break;
        case 20:
                angle = 44;

                step++;
        break;
        case 21:
                angle = 46;
                hitBoxBody->x -= 1;

                step++;
        break;
        case 22:
                angle = 48;

                step++;
        break;
        case 23:
                angle = 50;

                step++;
        break;
        case 24:
                angle = 52;
                hitBoxBody->x -= 1;

                step++;
        break;
        case 25:
                angle = 54;

                step++;
        break;
        case 26:
                angle = 56;

                step++;
        break;
        case 27:
                angle = 57;

                step++;
        break;
        case 28:
                angle = 58;
                hitBoxBody->x -= 1;

                step++;
        break;
        case 29:
                angle = 60;

                step++;
        break;
        case 30:
                angle = 63;

                step++;
        break;
        case 31:
                angle = 65;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 32:
                angle = 68;

                step++;
        break;
        case 33:
                angle = 70;

                step++;
        break;
        case 34:
                angle = 73;

                step++;
        break;
        case 35:
                angle = 76;
                hitBoxBody->x -= 1;

                step++;
        break;
        case 36:
                angle = 79;

                step++;
        break;
        case 37:
                angle = 82;

                step++;
        break;
        case 38:
                angle = 84;

                step++;
        break;
        case 39:
                angle = 86;
                hitBoxBody->x -= 1;
                step++;
        break;
        case 40:
                angle = 86;
                step++;
        break;
        case 41:
                angle = 88;
                step++;
        break;
        case 42:
                angle = 90;
                step++;
        break;
        case 43:
                executingTurn_GOINGLEFT_ONFLOOR_TO_RIGHTWALL = false;
                onRightWallGoingUp = true;
                hitBoxFace->x = hitBoxBody->x + 5;
                hitBoxFace->y = hitBoxBody->y - 15;

                hitBoxFoot->x = hitBoxBody->x - 14;
                hitBoxFoot->y = hitBoxBody->y;
                step = 0;
                footSpeed = 80;
        break;
    }
}

void KongaLee::goinguponrightwalltoceiling()
{
    switch(step)
    {
        case 0: angle = 92;
                hitBoxBody->y -= 1;
                step++;
                footSpeed = 40;
        break;
        case 1: angle = 94;
                step++;
        break;
        case 2:
                angle = 96;
                step++;
        break;
        case 3:
                angle = 98;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 4:
                angle = 100;

                step++;
        break;
        case 5:
                angle = 102;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 6:
                angle = 104;

                step++;
        break;
        case 7: angle = 106;

                step++;
        break;
        case 8:
                angle = 108;
                hitBoxBody->y -= 1;

                step++;
        break;
        case 9:
                angle = 110;

                step++;
        break;
        case 10:
                angle = 112;

                step++;
        break;
        case 11:
                angle = 114;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 12:
                angle = 116;

                step++;
        break;
        case 13:
                angle = 118;
                hitBoxBody->y -= 1;

                step++;
        break;
        case 14:
                angle = 120;

                step++;
        break;
        case 15:
                angle = 123;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 16:
                angle = 126;

                step++;
        break;
        case 17:
                angle = 128;
                hitBoxBody->y -= 1;

                step++;
        break;
        case 18:
                angle = 130;

                step++;
        break;
        case 19:
                angle = 132;

                step++;
        break;
        case 20:
                angle = 134;

                step++;
        break;
        case 21:
                angle = 136;
                hitBoxBody->y -= 1;

                step++;
        break;
        case 22:
                angle = 138;

                step++;
        break;
        case 23:
                angle = 140;

                step++;
        break;
        case 24:
                angle = 142;
                hitBoxBody->y -= 1;

                step++;
        break;
        case 25:
                angle = 144;

                step++;
        break;
        case 26:
                angle = 146;

                step++;
        break;
        case 27:
                angle = 147;

                step++;
        break;
        case 28:
                angle = 148;
                hitBoxBody->y -= 1;

                step++;
        break;
        case 29:
                angle = 150;

                step++;
        break;
        case 30:
                angle = 153;

                step++;
        break;
        case 31:
                angle = 155;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 32:
                angle = 158;

                step++;
        break;
        case 33:
                angle = 160;

                step++;
        break;
        case 34:
                angle = 163;

                step++;
        break;
        case 35:
                angle = 166;
                hitBoxBody->y -= 1;

                step++;
        break;
        case 36:
                angle = 169;

                step++;
        break;
        case 37:
                angle = 172;

                step++;
        break;
        case 38:
                angle = 174;

                step++;
        break;
        case 39:
                angle = 176;
                hitBoxBody->y -= 1;
                step++;
        break;
        case 40:
                angle = 176;
                step++;
        break;
        case 41:
                angle = 178;
                step++;
        break;
        case 42:
                angle = 180;
                step++;
        break;
        case 43:
                executingTurn_GOINGUP_ONRIGHTWALL_TO_CEILING = false;
                onCeilingGoingRight = true;
                hitBoxFace->x = hitBoxBody->x + 40;
                hitBoxFace->y = hitBoxBody->y + 5;

                hitBoxFoot->x = hitBoxBody->x + 25;
                hitBoxFoot->y = hitBoxBody->y - 14;
                step = 0;
                footSpeed = 80;
        break;
    }
}

void KongaLee::goingrightonceilingtoleftwall()
{
    switch(step)
    {
        case 0: angle = 182;
                hitBoxBody->x += 1;
                step++;
                footSpeed = 50;
        break;
        case 1: angle = 184;
                step++;
        break;
        case 2:
                angle = 186;
                step++;
        break;
        case 3:
                angle = 188;
                hitBoxBody->x += 1;
                step++;
        break;
        case 4:
                angle = 190;

                step++;
        break;
        case 5:
                angle = 192;
                hitBoxBody->x += 1;
                step++;
        break;
        case 6:
                angle = 194;

                step++;
        break;
        case 7: angle = 196;

                step++;
        break;
        case 8:
                angle = 198;
                hitBoxBody->x += 1;

                step++;
        break;
        case 9:
                angle = 200;

                step++;
        break;
        case 10:
                angle = 202;

                step++;
        break;
        case 11:
                angle = 204;
                hitBoxBody->x += 1;
                step++;
        break;
        case 12:
                angle = 206;

                step++;
        break;
        case 13:
                angle = 208;
                hitBoxBody->x += 1;

                step++;
        break;
        case 14:
                angle = 210;

                step++;
        break;
        case 15:
                angle = 213;
                hitBoxBody->x += 1;
                step++;
        break;
        case 16:
                angle = 216;

                step++;
        break;
        case 17:
                angle = 218;
                hitBoxBody->x += 1;

                step++;
        break;
        case 18:
                angle = 220;

                step++;
        break;
        case 19:
                angle = 222;

                step++;
        break;
        case 20:
                angle = 224;

                step++;
        break;
        case 21:
                angle = 226;
                hitBoxBody->x += 1;

                step++;
        break;
        case 22:
                angle = 228;

                step++;
        break;
        case 23:
                angle = 230;

                step++;
        break;
        case 24:
                angle = 232;
                hitBoxBody->x += 1;

                step++;
        break;
        case 25:
                angle = 234;

                step++;
        break;
        case 26:
                angle = 236;

                step++;
        break;
        case 27:
                angle = 237;

                step++;
        break;
        case 28:
                angle = 238;
                hitBoxBody->x += 1;

                step++;
        break;
        case 29:
                angle = 240;

                step++;
        break;
        case 30:
                angle = 243;

                step++;
        break;
        case 31:
                angle = 245;
                hitBoxBody->x += 1;
                step++;
        break;
        case 32:
                angle = 248;

                step++;
        break;
        case 33:
                angle = 250;

                step++;
        break;
        case 34:
                angle = 253;

                step++;
        break;
        case 35:
                angle = 256;
                hitBoxBody->x += 1;

                step++;
        break;
        case 36:
                angle = 259;

                step++;
        break;
        case 37:
                angle = 262;

                step++;
        break;
        case 38:
                angle = 264;

                step++;
        break;
        case 39:
                angle = 266;
                hitBoxBody->x += 1;
                step++;
        break;
        case 40:
                angle = 266;
                step++;
        break;
        case 41:
                angle = 268;
                step++;
        break;
        case 42:
                angle = 270;
                step++;
        break;
        case 43:
                executingTurn_GOINGRIGHT_ONCEILING_TO_LEFTWALL = false;
                onLeftWallGoingDown = true;
                hitBoxFace->x = hitBoxBody->x + 20;
                hitBoxFace->y = hitBoxBody->y + 40;

                hitBoxFoot->x = hitBoxBody->x + 40;
                hitBoxFoot->y = hitBoxBody->y + 27;
                step = 0;
                footSpeed = 80;
        break;
    }
}

void KongaLee::goingdownonleftwalltofloor()
{
    switch(step)
    {
        case 0: angle = 272;
                hitBoxBody->y += 1;
                step++;
                footSpeed = 50;
        break;
        case 1: angle = 274;
                step++;
        break;
        case 2:
                angle = 276;
                step++;
        break;
        case 3:
                angle = 278;
                hitBoxBody->y += 1;
                step++;
        break;
        case 4:
                angle = 280;

                step++;
        break;
        case 5:
                angle = 282;
                hitBoxBody->y += 1;
                step++;
        break;
        case 6:
                angle = 284;

                step++;
        break;
        case 7: angle = 286;

                step++;
        break;
        case 8:
                angle = 288;
                hitBoxBody->y += 1;

                step++;
        break;
        case 9:
                angle = 290;

                step++;
        break;
        case 10:
                angle = 292;

                step++;
        break;
        case 11:
                angle = 294;
                hitBoxBody->y += 1;
                step++;
        break;
        case 12:
                angle = 296;

                step++;
        break;
        case 13:
                angle = 298;
                hitBoxBody->y += 1;

                step++;
        break;
        case 14:
                angle = 300;

                step++;
        break;
        case 15:
                angle = 303;
                hitBoxBody->y += 1;
                step++;
        break;
        case 16:
                angle = 306;

                step++;
        break;
        case 17:
                angle = 308;
                hitBoxBody->y += 1;

                step++;
        break;
        case 18:
                angle = 310;

                step++;
        break;
        case 19:
                angle = 312;

                step++;
        break;
        case 20:
                angle = 314;

                step++;
        break;
        case 21:
                angle = 316;
                hitBoxBody->y += 1;

                step++;
        break;
        case 22:
                angle = 318;

                step++;
        break;
        case 23:
                angle = 320;

                step++;
        break;
        case 24:
                angle = 322;
                hitBoxBody->y += 1;

                step++;
        break;
        case 25:
                angle = 324;

                step++;
        break;
        case 26:
                angle = 326;

                step++;
        break;
        case 27:
                angle = 327;

                step++;
        break;
        case 28:
                angle = 328;
                hitBoxBody->y += 1;

                step++;
        break;
        case 29:
                angle = 330;

                step++;
        break;
        case 30:
                angle = 333;

                step++;
        break;
        case 31:
                angle = 335;
                hitBoxBody->y += 1;
                step++;
        break;
        case 32:
                angle = 338;

                step++;
        break;
        case 33:
                angle = 340;

                step++;
        break;
        case 34:
                angle = 343;

                step++;
        break;
        case 35:
                angle = 346;
                hitBoxBody->y += 1;

                step++;
        break;
        case 36:
                angle = 349;

                step++;
        break;
        case 37:
                angle = 352;

                step++;
        break;
        case 38:
                angle = 354;

                step++;
        break;
        case 39:
                angle = 356;
                hitBoxBody->y += 1;
                step++;
        break;
        case 40:
                angle = 356;
                step++;
        break;
        case 41:
                angle = 358;
                step++;
        break;
        case 42:
                angle = 360;
                step++;
        break;
        case 43:
                executingTurn_GOINGDOWN_ONLEFTWALL_TO_FLOOR = false;
                onFloorGoingLeft = true;
                hitBoxFace->x = hitBoxBody->x - 15;
                hitBoxFace->y = hitBoxBody->y + 10;

                hitBoxFoot->x = hitBoxBody->x;
                hitBoxFoot->y = hitBoxBody->y + 34;
                step = 0;
                footSpeed = 80;
        break;
    }
}
