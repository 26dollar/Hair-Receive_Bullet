#include "RoboMaggot.h"

RoboMaggot::RoboMaggot(int x, int y)
{
    hitBox = new SDL_Rect;
    hitBox->w = 20;
    hitBox->h = 30;
    hitBox->x = x;
    hitBox->y = y;

    amountToMoveUp = 0;
    amountToMoveDown = 0;

    xPos = 10;
    yPos = 10;

    shoot = false;

    bullet = -1;

    sequence = 1;

    direction = 4;

    score = 0;

    firstTry = true;
}

RoboMaggot::~RoboMaggot()
{
    hitBox = NULL;
}

void RoboMaggot::move()
{
    aTeleSound = false;

    if(amountToMoveUp > 15)
    {
        hitBox->y -= 4;
        amountToMoveUp -= 4;
    }
    else if(amountToMoveUp > 0)
    {
        hitBox->y -= 1;
        amountToMoveUp -= 1;
    }

    if(amountToMoveDown > 15)
    {
        hitBox->y += 4;
        amountToMoveDown -= 4;
    }
    else if(amountToMoveDown > 0)
    {
        hitBox->y += 1;
        amountToMoveDown -= 1;
    }

    /*if(volleyTimer.getTicks() > 2000 && volleyTimer.getTicks() < 3000)
    {
        if(!bufferTimer.isStarted())
        {
            shoot = true;
            bullet++;
            bufferTimer.start();
            if(sequence == 0)
            {
                amountToMoveUp += 10;
                sequence++;
            }
            else if(sequence == 1)
            {
                amountToMoveDown += 10;
                sequence--;
            }
        }
    }*/

    if(volleyTimer.isStarted())
    {
        switch ( sequence ) ///complex ai lole
                        {
                            case 1:
                                    if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveUp+= 10; sequence++; bullet++; direction = 4; hitBox->y = -26;}
                                    if(bufferTimer.getTicks() > 250) { bufferTimer.stop();}
                                break;
                            case 2: if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveDown+= 10; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 250) { bufferTimer.stop();}
                                break;
                            case 3: if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveUp+= 10; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 250) { bufferTimer.stop();}
                                break;
                            case 4: if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveUp+= 0; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 250) { bufferTimer.stop();}
                                break;
                            case 5:
                                    if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); hitBox->y -= 50; amountToMoveUp+= 10; sequence++; bullet++; direction = 4;}
                                    if(bufferTimer.getTicks() > 1950) { bufferTimer.stop();}
                                break;
                            case 6: if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveUp+= 10; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 200) { bufferTimer.stop();}
                                break;
                            case 7: if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveDown+= 10; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 200) { bufferTimer.stop();}
                                break;
                            case 8: if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveUp+= 45; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 500) { bufferTimer.stop();}
                            break;
                            case 9:
                                    if(!bufferTimer.isStarted()) {  bufferTimer.start(); hitBox->x -= 1215; hitBox->y = -141; amountToMoveDown+= 35; sequence++; direction = 2; aTeleSound = true;} ///PLAY THE SOUND HERE
                                    if(bufferTimer.getTicks() > 1550) { bufferTimer.stop();}
                            break;
                            case 10:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveDown+= 35; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 1950) { bufferTimer.stop();}
                            break;
                            case 11:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveUp+= 85; sequence++; bullet++; }
                                    if(bufferTimer.getTicks() > 300) { bufferTimer.stop();}
                            break;
                            case 12:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveUp+= 45; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 1950) { bufferTimer.stop();}
                            break;
                            case 13:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveUp+= 15; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 150) { bufferTimer.stop();}
                            break;
                            case 14:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveDown+= 280; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 100) { bufferTimer.stop();}
                            break;
                            case 15:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 1500) { bufferTimer.stop();}
                            break;
                            case 16:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 150) { bufferTimer.stop();}
                            break;
                            case 17:if(!bufferTimer.isStarted()) { bufferTimer.start(); amountToMoveUp+= 25; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 150) { bufferTimer.stop();}
                            break;
                            case 18:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveDown+= 15; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 1950) { bufferTimer.stop();}
                            break;
                            case 19:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveUp+= 15; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 400) { bufferTimer.stop();}
                            case 20:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveUp+= 75; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 850) { bufferTimer.stop();}
                            break;
                            case 21:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveDown+= 25; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 1950) { bufferTimer.stop();}
                            break;
                            case 22:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveDown+= 25; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 110) { bufferTimer.stop();}
                            break;
                            case 23:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveDown+= 25; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 110) { bufferTimer.stop();}
                            break;
                            case 24:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveDown+= 25; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 110) { bufferTimer.stop();}
                            break;
                            case 25:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveDown+= 35; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 110) { bufferTimer.stop();}
                            break;
                            case 26:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 1950) { bufferTimer.stop();}
                            break;
                            case 27:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); hitBox->y -= 95; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 50) { bufferTimer.stop();}
                            break;
                            case 28:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); hitBox->y += 45; sequence++; bullet++; amountToMoveUp+= 35;}
                                    if(bufferTimer.getTicks() > 150) { bufferTimer.stop();}
                            break;
                            case 29:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start();  sequence++; bullet++; amountToMoveUp+= 20;}
                                    if(bufferTimer.getTicks() > 1950) { bufferTimer.stop();}
                            break;
                            case 30:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start();  sequence++; bullet++; amountToMoveDown+= 15;}
                                    if(bufferTimer.getTicks() > 200) { bufferTimer.stop();}
                            break;
                            case 31:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start();  sequence++; bullet++; amountToMoveDown+= 15;}
                                    if(bufferTimer.getTicks() > 200) { bufferTimer.stop();}
                            break;
                            case 32:
                                    if(!bufferTimer.isStarted()) {  bufferTimer.start(); hitBox->x += 1215; sequence++; direction = 4; amountToMoveUp+= 35; aTeleSound = true;} ///PLAY THE SOUND HERE
                                    if(bufferTimer.getTicks() > 2350) { bufferTimer.stop();}
                            break;
                            case 33:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveUp+= 15; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 200) { bufferTimer.stop();}
                            break;
                            case 34:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveUp+= 25; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 90) { bufferTimer.stop();}
                            break;
                            case 35:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveDown+= 15; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 1950) { bufferTimer.stop();}
                            break;
                            case 36:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveDown+= 15; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 90) { bufferTimer.stop();}
                            break;
                            case 37:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveUp+= 85; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 90) { bufferTimer.stop();}
                            break;
                            case 38:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); amountToMoveDown+= 0; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 3150) { bufferTimer.stop();}
                            break;
                            case 39:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); hitBox->y += 85; sequence++; bullet++;}
                                    if(bufferTimer.getTicks() > 490) { bufferTimer.stop();}
                            break;
                            case 40:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); sequence++; bullet++; amountToMoveDown+= 20;}
                                    if(bufferTimer.getTicks() > 3450) { bufferTimer.stop();}
                            break;
                            case 41:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); sequence++; bullet++; hitBox->x -= 1215; direction = 2; amountToMoveUp+= 20; aTeleSound = true;}
                                    if(bufferTimer.getTicks() > 50) { bufferTimer.stop();}
                            break;
                            case 42:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); sequence++; bullet++;  amountToMoveDown+= 20; }
                                    if(bufferTimer.getTicks() > 50) { bufferTimer.stop();}
                            break;
                            case 43:if(!bufferTimer.isStarted()) { shoot = true; bufferTimer.start(); sequence++; bullet++; hitBox->x += 1215; direction = 4; aTeleSound = true;}
                                    if(bufferTimer.getTicks() > 50) { bufferTimer.stop();}
                            break;
                        }
    }


    if(bullet >= 18)
    {
        bullet = 0; /// okay iduno why -1 doesnt work here
    }
}

void RoboMaggot::update()
{
    shoot = false;

    if(flip == SDL_FLIP_NONE)
    {
        xPos = hitBox->x -5;
        yPos = hitBox->y -5;
    }
    else
    {
        xPos = hitBox->x +2;
        yPos = hitBox->y -5;
    }


    if(colorBody.getTicks() > 80)
    {
        colorBody.stop();
    }

    if(direction == 4)
    {
        flip = SDL_FLIP_NONE;
    }
    else
    {
        flip = SDL_FLIP_HORIZONTAL;
    }
}
