#include "TargetMaster.h"

TargetMaster::TargetMaster(int x, int y)
{
    xHome = x;
    yHome = y;

    score = 0;

    twoSecCounter = 0;
}

TargetMaster::~TargetMaster()
{

}

void TargetMaster::move()
{
 //make targets reflected bulelts up down left right whahtever
    if(funTimer.getTicks() > 700) ///every 0.7 seconds, spawn a new target
    {
        twoSecCounter++;
        funTimer.start();
        switch(twoSecCounter)
        {
            case 1:
            targets[0].hitBox->x = xHome + rand() % 1175;
            targets[0].hitBox->y = yHome + rand() % 200;
            targets[0].hitBox->w = 30;
            targets[0].hitBox->h = 30;
            targets[0].color = 2;
            targets[0].alive = true;
            targets[0].funTimerlol.start();
            targets[0].alpha = 255;
            break;
            case 2:
            targets[1].hitBox->x = xHome + rand() % 1175;
            targets[1].hitBox->y = yHome + rand() % 200;
            targets[1].hitBox->w = 50;
            targets[1].hitBox->h = 50;
            targets[1].color = 1;
            targets[1].alive = true;
            targets[1].funTimerlol.start();
            targets[1].alpha = 255;
            break;
            case 3:
            targets[2].hitBox->x = xHome + rand() % 1175;
            targets[2].hitBox->y = yHome + rand() % 200;
            targets[2].hitBox->w = 13;
            targets[2].hitBox->h = 13;
            targets[2].color = 3;
            targets[2].alive = true;
            targets[2].funTimerlol.start();
            targets[2].alpha = 255;
            break;
            case 4:
            targets[3].hitBox->x = xHome + rand() % 1175;
            targets[3].hitBox->y = yHome + rand() % 200;
            targets[3].hitBox->w = 30;
            targets[3].hitBox->h = 30;
            targets[3].color = 2;
            targets[3].alive = true;
            targets[3].funTimerlol.start();
            targets[3].alpha = 255;
            break;
            case 5:
            targets[4].hitBox->x = xHome + rand() % 1175;
            targets[4].hitBox->y = yHome + rand() % 200;
            targets[4].hitBox->w = 50;
            targets[4].hitBox->h = 50;
            targets[4].color = 1;
            targets[4].alive = true;
            targets[4].funTimerlol.start();
            targets[4].alpha = 255;
            break;
            case 6:
            targets[5].hitBox->x = xHome + rand() % 1175;
            targets[5].hitBox->y = yHome + rand() % 200;
            targets[5].hitBox->w = 13;
            targets[5].hitBox->h = 13;
            targets[5].color = 3;
            targets[5].alive = true;
            targets[5].funTimerlol.start();
            targets[5].alpha = 255;
            break;
            case 7:
            targets[6].hitBox->x = xHome + rand() % 1175;
            targets[6].hitBox->y = yHome + rand() % 200;
            targets[6].hitBox->w = 30;
            targets[6].hitBox->h = 30;
            targets[6].color = 2;
            targets[6].alive = true;
            targets[6].funTimerlol.start();
            targets[6].alpha = 255;
            break;
            case 8:
            targets[7].hitBox->x = xHome + rand() % 1175;
            targets[7].hitBox->y = yHome + rand() % 200;
            targets[7].hitBox->w = 50;
            targets[7].hitBox->h = 50;
            targets[7].color = 1;
            targets[7].alive = true;
            targets[7].funTimerlol.start();
            targets[7].alpha = 255;
            break;
            case 9:
            targets[8].hitBox->x = xHome + rand() % 1175;
            targets[8].hitBox->y = yHome + rand() % 200;
            targets[8].hitBox->w = 13;
            targets[8].hitBox->h = 13;
            targets[8].color = 3;
            targets[8].alive = true;
            targets[8].funTimerlol.start();
            targets[8].alpha = 255;
            break;
            case 10:
            targets[9].hitBox->x = xHome + rand() % 1175;
            targets[9].hitBox->y = yHome + rand() % 200;
            targets[9].hitBox->w = 30;
            targets[9].hitBox->h = 30;
            targets[9].color = 2;
            targets[9].alive = true;
            targets[9].funTimerlol.start();
            targets[9].alpha = 255;
            break;
            case 11:
            targets[10].hitBox->x = xHome + rand() % 1175;
            targets[10].hitBox->y = yHome + rand() % 200;
            targets[10].hitBox->w = 50;
            targets[10].hitBox->h = 50;
            targets[10].color = 1;
            targets[10].alive = true;
            targets[10].funTimerlol.start();
            targets[10].alpha = 255;
            break;
            case 12:
            targets[11].hitBox->x = xHome + rand() % 1175;
            targets[11].hitBox->y = yHome + rand() % 200;
            targets[11].hitBox->w = 13;
            targets[11].hitBox->h = 13;
            targets[11].color = 3;
            targets[11].alive = true;
            targets[11].funTimerlol.start();
            targets[11].alpha = 255;
            break;
            case 13:
            targets[12].hitBox->x = xHome + rand() % 1175;
            targets[12].hitBox->y = yHome + rand() % 200;
            targets[12].hitBox->w = 30;
            targets[12].hitBox->h = 30;
            targets[12].color = 2;
            targets[12].alive = true;
            targets[12].funTimerlol.start();
            targets[12].alpha = 255;
            break;
            case 14:
            targets[13].hitBox->x = xHome + rand() % 1175;
            targets[13].hitBox->y = yHome + rand() % 200;
            targets[13].hitBox->w = 50;
            targets[13].hitBox->h = 50;
            targets[13].color = 1;
            targets[13].alive = true;
            targets[13].funTimerlol.start();
            targets[13].alpha = 255;
            break;
            case 15:
            targets[14].hitBox->x = xHome + rand() % 1175;
            targets[14].hitBox->y = yHome + rand() % 200;
            targets[14].hitBox->w = 30;
            targets[14].hitBox->h = 30;
            targets[14].color = 2;
            targets[14].alive = true;
            targets[14].funTimerlol.start();
            targets[14].alpha = 255;
            break;
        }
    }

    if(twoSecCounter > 19)
    {
        funTimer.stop();
    }
}
