#include "TNTgent.h"

TNTgent::TNTgent(int x, int y)
{
    hitBox = new SDL_Rect;
    hitBox->w = 30;
    hitBox->h = 65;
    hitBox->x = x;
    hitBox->y = y;

    hitBoxHead = new SDL_Rect;
    hitBoxHead->w = 19;
    hitBoxHead->h = 35;
    hitBoxHead->x = x;
    hitBoxHead->y = y;

    trigger = new SDL_Rect;
    trigger->w = 700;
    trigger->h = 200;
    trigger->x = x;
    trigger->y = y;

    for(int i = 0; i < 5; i++)
    {
        bullets[i] = new SDL_Rect;
        bullets[i]->w = 6;
        bullets[i]->h = 6;
        bullets[i]->x = 6;
        bullets[i]->y = 6;
    }
    for(int i = 0; i < 5; i++)
    {
        bulletactive[i] = false;
    }

    explosionHitbox = new SDL_Rect;
    explosionHitbox->w = 110;
    explosionHitbox->h = 90;
    explosionHitbox->x = x;
    explosionHitbox->y = y;

    gunPoint.x = x;
    gunPoint.y = y;

    xPos = 10;
    yPos = 10;

    health = 3;
    dead = false;
    prone = false;
    onFlatSurface = false;

    sequence = 0;
    check = 0;

    bullshitsoundboolno1352 = false;

    heightToJumpTo = 0;
    jumping = false;

    animation = 0;

    tossing = false;
    exploded = 0;
    tossSpeed = 20;
    headThrown = 0; ///0 = head isnt thrown yet. 1 = head is soaring through air. 2 = head has exploded from landing

    fuckingshootmeinthegoddamndick = false;
    speakingFrame = 0;

    animationIntLol = 0;
}

TNTgent::~TNTgent()
{
    hitBox = NULL;

    for(int i = 0; i < 5; i++)
    {
        bullets[i] = NULL;
    }

    trigger = NULL;

    hitBoxHead = NULL;

    explosionHitbox = NULL;
}

void TNTgent::reset()
{
    dead = false;
    prone = false;
    onFlatSurface = false;
    sequence = 0;
    check = 0;
    bullshitsoundboolno1352 = false;
    jumping = false;
    animation = 0;
    tossing = false;
    fuckingshootmeinthegoddamndick = false;
    speakingFrame = 0;
    animationIntLol = 0;
    programTimer.stop();
    speakingTimer.stop();
    explosionTimer.stop();
    headThrown = 0;
    exploded = false;
    tossSpeed = 20;
    health = 3;
}

void TNTgent::jump()
{
    if(hitBox->y > heightToJumpTo)
    {
        hitBox->y -= 4;
    }
}

void TNTgent::move(int animationInt)
{
    if(health < 1)
    {
        programTimer.pause();
        jumping = false;
    }

    animationIntLol = animationInt;
    if(animationIntLol == 5)
    {
        if(headThrown == 0)
        {
            headThrown = 1;
            dead = true;
        }
    }

    if(headThrown == 1 && health > 0)
    {
        hitBoxHead->x -= 9;
        hitBoxHead->y -= tossSpeed * 0.27;
        tossSpeed -= 1;
    }

    if(jumping)
    {
        jump();
    }

    if(sequence == 1)
    {
        if(programTimer.getTicks() > 200)
        {
            if(check == 1)
            {
                prone = true;
                hitBox->y += 35;
                check++;
            }
        }

        if(programTimer.getTicks() > 950)
        {
            if(check == 2)
            {
                bullets[0]->x = gunPoint.x;
                bullets[0]->y = gunPoint.y;
                bulletactive[0] = true;
                check++;
            }
        }

        if(programTimer.getTicks() > 1000)
        {
            if(check == 3)
            {
                bullets[1]->x = gunPoint.x;
                bullets[1]->y = gunPoint.y;
                bulletactive[1] = true;
                check++;
            }
        }

        if(programTimer.getTicks() > 1300)
        {
            if(check == 4)
            {
                prone = false;
                hitBox->y -= 50;
                heightToJumpTo = hitBox->y -40;
                jumping = true;
                fuckingshootmeinthegoddamndick = true;
                check++;
            }
        }

        if(programTimer.getTicks() > 1500)
        {
            if(check == 5)
            {
                bullets[2]->x = gunPoint.x;
                bullets[2]->y = gunPoint.y;
                bulletactive[2] = true;
                check++;
            }
        }

        if(programTimer.getTicks() > 1600)
        {
            if(check == 6)
            {
                bullets[3]->x = gunPoint.x;
                bullets[3]->y = gunPoint.y;
                bulletactive[3] = true;
                check++;
            }
        }

        if(programTimer.getTicks() > 1700)
        {
            if(check == 7)
            {
                bullets[4]->x = gunPoint.x;
                bullets[4]->y = gunPoint.y;
                bulletactive[4] = true;
                jumping = false;
                check++;
                //programTimer.stop(); ///delete this
            }
        }

        if(programTimer.getTicks() > 3800)
        {
            if(check == 8)
            {
                tossing = true;
                check++;
                //programTimer.stop(); ///delete this
            }
        }

    }

    if(sequence == 2)
    {
        if(programTimer.getTicks() > 200)
        {
            if(check == 1)
            {
                jumping = true;
                fuckingshootmeinthegoddamndick = true;
                heightToJumpTo = hitBox->y -60;
                check++;
            }
        }

        if(programTimer.getTicks() > 700)
        {
            if(check == 2)
            {
                bullets[0]->x = gunPoint.x;
                bullets[0]->y = gunPoint.y;
                bulletactive[0] = true;
                check++;
            }
        }

        if(programTimer.getTicks() > 770)
        {
            if(check == 3)
            {
                bullets[1]->x = gunPoint.x;
                bullets[1]->y = gunPoint.y;
                bulletactive[1] = true;
                jumping = false;
                check++;
            }
        }

        if(programTimer.getTicks() > 1100)
        {
            if(check == 4)
            {
                prone = true;
                hitBox->y += 35;
                check++;
            }
        }

        if(programTimer.getTicks() > 1500)
        {
            if(check == 5)
            {
                bullets[2]->x = gunPoint.x;
                bullets[2]->y = gunPoint.y;
                bulletactive[2] = true;
                check++;
            }
        }

        if(programTimer.getTicks() > 1600)
        {
            if(check == 6)
            {
                bullets[3]->x = gunPoint.x;
                bullets[3]->y = gunPoint.y;
                bulletactive[3] = true;
                check++;
            }
        }

        if(programTimer.getTicks() > 1700)
        {
            if(check == 7)
            {
                bullets[4]->x = gunPoint.x;
                bullets[4]->y = gunPoint.y;
                bulletactive[4] = true;
                prone = false;
                hitBox->y -= 50;
                check++;
                //programTimer.stop(); ///delete this
            }
        }

        if(programTimer.getTicks() > 3800)
        {
            if(check == 8)
            {
                tossing = true;
                check++;
                //programTimer.stop(); ///delete this
            }
        }

    }

    if(sequence == 3)
    {
        if(programTimer.getTicks() > 200)
        {
            if(check == 1)
            {
                prone = true;
                hitBox->y += 35;
                check++;
            }
        }

        if(programTimer.getTicks() > 1000)
        {
            if(check == 2)
            {
                bullets[0]->x = gunPoint.x;
                bullets[0]->y = gunPoint.y;
                bulletactive[0] = true;
                check++;
            }
        }

        if(programTimer.getTicks() > 1050)
        {
            if(check == 3)
            {
                bullets[1]->x = gunPoint.x;
                bullets[1]->y = gunPoint.y;
                bulletactive[1] = true;
                check++;
            }
        }

        if(programTimer.getTicks() > 1100)
        {
            if(check == 4)
            {
                prone = false;
                hitBox->y -= 50;
                heightToJumpTo = hitBox->y -70;
                jumping = true;
                fuckingshootmeinthegoddamndick = true;
                check++;
            }
        }

        if(programTimer.getTicks() > 1420)
        {
            if(check == 5)
            {
                bullets[2]->x = gunPoint.x;
                bullets[2]->y = gunPoint.y;
                bulletactive[2] = true;
                check++;
            }
        }

        if(programTimer.getTicks() > 1460)
        {
            if(check == 6)
            {
                bullets[3]->x = gunPoint.x;
                bullets[3]->y = gunPoint.y;
                bulletactive[3] = true;
                check++;
            }
        }

        if(programTimer.getTicks() > 1500)
        {
            if(check == 7)
            {
                bullets[4]->x = gunPoint.x;
                bullets[4]->y = gunPoint.y;
                bulletactive[4] = true;
                jumping = false;
                check++;
                //programTimer.stop(); ///delete this
            }
        }

        if(programTimer.getTicks() > 3600)
        {
            if(check == 8)
            {
                tossing = true;
                check++;
                //programTimer.stop(); ///delete this
            }
        }

    }

    if(health > 0)
    {
        if(!prone)
        {
            hitBox->w = 30;
            hitBox->h = 75;
            animation = 0; ///
        }
        else
        {
            hitBox->w = 35;
            hitBox->h = 30;
            animation = 2; ///
        }

        if(!onFlatSurface)
        {
            if(!jumping)
            {
                hitBox->y += 5; //gravity
            }
            else
            {
                ///animation = 1; ///jumping
            }
        }

        if(onFlatSurface && !jumping)
        {
            fuckingshootmeinthegoddamndick = false;
        }

        if(fuckingshootmeinthegoddamndick)
        {
            animation = 1;
        }

        if(tossing)
        {
            animation = 5;
        }

        if(exploded)
        {
            animation = 6;
        }
    }

    onFlatSurface = false;

    for(int i = 0; i < 5; i++)
    {
        if(bulletactive[i])
        {
            bullets[i]->x -= 5;
        }
    }

    if(speakingTimer.getTicks() > 70)
    {
        speakingFrame++;
        speakingTimer.start();
    }

    if(speakingFrame > 38)
    {
        speakingFrame == -1;
        speakingTimer.stop();
    }
}

void TNTgent::update()
{
    xPos = hitBox->x - 20;
    yPos = hitBox->y;

    if(animationIntLol != 5)
    {
        if(!prone)
        {
            if(animationIntLol == 2)
            {
                hitBoxHead->x = hitBox->x -15;
                hitBoxHead->y = hitBox->y +5;
            }
            else
            {
                hitBoxHead->x = hitBox->x +6;
                hitBoxHead->y = hitBox->y -35;
            }
        }
        else
        {
            hitBoxHead->x = hitBox->x -35;
            hitBoxHead->y = hitBox->y -20;
        }
    }

    trigger->x = hitBox->x - 700;
    trigger->y = hitBox->y - 100;


    if(!prone)
    {
        gunPoint.x = hitBox->x - 20;
        gunPoint.y = hitBox->y + 20;
    }
    else
    {
        gunPoint.x = hitBox->x -55;
        gunPoint.y = hitBox->y + 21;
    }

    if(hitColorHead.getTicks() > 80)
    {
        hitColorHead.stop();
    }

    if(explosionTimer.isStarted())
    {
        explosionHitbox->x = hitBoxHead->x - 47;
        explosionHitbox->y = hitBoxHead->y - 30;
    }

    if(explosionTimer.getTicks() > 900)
    {
        explosionTimer.stop();
    }
}
