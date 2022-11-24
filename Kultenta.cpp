#include "Kultenta.h"

Kultenta::Kultenta(int x, int y)
{
    hitBox = new SDL_Rect;
    hitBox->w = 40;
    hitBox->h = 50;
    hitBox->x = x;
    hitBox->y = y;

    for(int i = 0; i < 8; i++)
    {
        bullets[i] = new SDL_Rect;
        bullets[i]->w = 6;
        bullets[i]->h = 6;
        bullets[i]->x = 6;
        bullets[i]->y = 6;

    }

    for(int i = 0; i < 8; i++)
    {
        bulletactive[i] = false;
    }

    xPos = 10;
    yPos = 10;

    dead = false;
    fallingToDEath = false;
    imadethisbooltosaveoncpu = false;

    clip = 0;
    clip1 = 0;

    animationTime.start();

    gunPoint.x = x;
    gunPoint.y = y;

    storage1Full = false;
    storage2Full = false;
    storage3Full = false;
    storage4Full = false;
    storage5Full = false;

    storage1visibol = false;
    storage2visibol = false;
    storage3visibol = false;
    storage4visibol = false;
    storage5visibol = false;

    storageOffsetX = 0;
    storageOffsetY = 0;


    /*bullet1active = false;
    bullet2active = false;
    bullet3active = false;
    bullet4active = false;
    bullet5active = false;
    bullet6active = false;
    bullet7active = false;
    bullet8active = false;*/

    rootyTootyTimetoShootyTimer.start();
    shoot = false;
    bullet = 1;

    gayAssSoundBool = false;
}

Kultenta::~Kultenta()
{
    hitBox = NULL;

    for(int i = 0; i < 8; i++)
    {
        bullets[i] = NULL;
    }
}

void Kultenta::reset()
{
    dead = false;
    fallingToDEath = false;
    storage1Full = false;
    storage2Full = false;
    storage3Full = false;
    storage4Full = false;
    storage5Full = false;
    storage1visibol = false;
    storage2visibol = false;
    storage3visibol = false;
    storage4visibol = false;
    storage5visibol = false;
}

void Kultenta::shootgun()
{
    switch( bullet )
        {
            case 1:
            {
              bulletactive[0] = true;
              bullets[0]->x = gunPoint.x;
              bullets[0]->y = gunPoint.y;
            }
            break;
            case 2:
            {
              bulletactive[1] = true;
              bullets[1]->x = gunPoint.x;
              bullets[1]->y = gunPoint.y;
            }
            break;
            case 3:
            {
              bulletactive[2] = true;
              bullets[2]->x = gunPoint.x;
              bullets[2]->y = gunPoint.y;
            }
            break;
            case 4:
            {
              bulletactive[3] = true;
              bullets[3]->x = gunPoint.x;
              bullets[3]->y = gunPoint.y;
            }
            break;
            case 5:
            {
              bulletactive[4] = true;
              bullets[4]->x = gunPoint.x;
              bullets[4]->y = gunPoint.y;
            }
            break;
            case 6:
            {
              bulletactive[5] = true;
              bullets[5]->x = gunPoint.x;
              bullets[5]->y = gunPoint.y;
            }
            break;
            case 7:
            {
              bulletactive[6] = true;
              bullets[6]->x = gunPoint.x;
              bullets[6]->y = gunPoint.y;
            }
            break;
            case 8:
            {
              bulletactive[7] = true;
              bullets[7]->x = gunPoint.x;
              bullets[7]->y = gunPoint.y;
            }
            break;
        }
}

void Kultenta::move()
{
    gayAssSoundBool = false;
    if(hitColor.getTicks() > 300)
    {
        hitColor.stop();
    }

    if(imadethisbooltosaveoncpu)
    {
        if(storage1visibol && storage2visibol && storage3visibol && storage4visibol && storage5visibol) ///have to wait until the bullets are actually visibol to kill it because otherwise the playerbullets wont be deleted yet
        {
            dead = true;
            hitBox->x += 17;
            hitBox->y -= 19;
            imadethisbooltosaveoncpu = false;
            beingDeadTimer.start();
            gayAssSoundBool = true;
        }
    }
    if(166 * (beingDeadTimer.getTicks() / 30) == 1992)
    {
        fallingToDEath = true;
    }


    if(!dead)
    {
    if(bullet > 8)
    {
        bullet = 1;
    }

    shoot = false;
    if(rootyTootyTimetoShootyTimer.getTicks() > 1000)
    {
        rootyTootyTimetoShootyTimer.start();
        shoot = true;
    }

    if(shoot)
    {
        if(storage1visibol)
        {
            shootgun();
            storage1Full = false;
            storage1visibol = false;
            bullet++;
        }
        else if(storage2visibol)
        {
            shootgun();
            storage2Full = false;
            storage2visibol = false;
            bullet++;
        }
        else if(storage3visibol)
        {
            shootgun();
            storage3Full = false;
            storage3visibol = false;
            bullet++;
        }
        else if(storage4visibol)
        {
            shootgun();
            storage4Full = false;
            storage4visibol = false;
            bullet++;
        }
        else if(storage5visibol)
        {
            shootgun();
            storage5Full = false;
            storage5visibol = false;
            bullet++;
        }
    }
    }

    if(fallingToDEath)
    {
        hitBox->y += 5;
    }

    for(int i = 0; i < 8; i++)
    {
        if(bulletactive[i])
        {
            bullets[i]->x -= 4;
        }
    }


                if(replaceTimer1.getTicks() > 790)
                {
                    replaceTimer1.stop();
                    storage1visibol = true;
                }

                if(replaceTimer2.getTicks() > 790)
                {
                    replaceTimer2.stop();
                    storage2visibol = true;
                }

                if(replaceTimer3.getTicks() > 790)
                {
                    replaceTimer3.stop();
                    storage3visibol = true;
                }

                if(replaceTimer4.getTicks() > 790)
                {
                    replaceTimer4.stop();
                    storage4visibol = true;
                }

                if(replaceTimer5.getTicks() > 790)
                {
                    replaceTimer5.stop();
                    storage5visibol = true;
                }
}

void Kultenta::update()
{
    xPos = hitBox->x - 20;
    yPos = hitBox->y;

    if(!dead)
    {
        clip = 43 * (animationTime.getTicks() / 125);
        if(clip > 559)
        {
            animationTime.start();
            clip = 559;
        }

        clip1 = 51 * (animationTime.getTicks() / 125);
        if(clip1 > 663)
        {
            animationTime.start();
            clip1 = 663;
        }
    }

    switch( clip )
    {
        case 0:
        {
            gunPoint.x = hitBox->x - 69;
            gunPoint.y = hitBox->y + 35;
        } break;
        case 43:
        {
            gunPoint.x = hitBox->x - 68;
            gunPoint.y = hitBox->y + 30;
        } break;
        case 86:
        {
            gunPoint.x = hitBox->x - 68;
            gunPoint.y = hitBox->y + 24;
        } break;
        case 129:
        {
            gunPoint.x = hitBox->x - 68;
            gunPoint.y = hitBox->y + 18;
        } break;
        case 172:
        {
            gunPoint.x = hitBox->x - 68;
            gunPoint.y = hitBox->y + 12;
        } break;
        case 215:
        {
            gunPoint.x = hitBox->x - 68;
            gunPoint.y = hitBox->y + 5;
        } break;
        case 258:
        {
            gunPoint.x = hitBox->x - 67;
            gunPoint.y = hitBox->y - 2;
        } break;
        case 301:
        {
            gunPoint.x = hitBox->x - 67;
            gunPoint.y = hitBox->y - 9;
        } break;
        case 344:
        {
            gunPoint.x = hitBox->x - 67;
            gunPoint.y = hitBox->y;
        } break;
        case 387:
        {
            gunPoint.x = hitBox->x - 67;
            gunPoint.y = hitBox->y + 5;
        } break;
        case 430:
        {
            gunPoint.x = hitBox->x - 67;
            gunPoint.y = hitBox->y +12;
        } break;
        case 473:
        {
            gunPoint.x = hitBox->x - 68;
            gunPoint.y = hitBox->y + 18;
        } break;
        case 516:
        {
            gunPoint.x = hitBox->x - 69;
            gunPoint.y = hitBox->y + 24;
        } break;
        case 559:
        {
            gunPoint.x = hitBox->x - 69;
            gunPoint.y = hitBox->y + 30;
        } break;
    }

    if(dead)
    {
        if(beingDeadTimer.getTicks() > 400)
         {
             storageOffsetX = 0;
             storageOffsetY = 0;
         }
         else if(beingDeadTimer.getTicks() > 300)
         {
             storageOffsetX = 2;
             storageOffsetY = 2;
         }
         else if(beingDeadTimer.getTicks() > 200)
         {
             storageOffsetX = 5;
             storageOffsetY = 5;
         }
         else if(beingDeadTimer.getTicks() > 100)
         {
             storageOffsetX = 9;
             storageOffsetY = 8;
         }
         else if(beingDeadTimer.getTicks() < 100)
         {
             storageOffsetX = 14;
             storageOffsetY = 10;
         }
    }

         bulletStorage1.x = hitBox->x + 30 - storageOffsetX;
         bulletStorage2.x = hitBox->x + 2 - storageOffsetX;
         bulletStorage3.x = hitBox->x + 32 - storageOffsetX;
         bulletStorage4.x = hitBox->x + 4 - storageOffsetX;
         bulletStorage5.x = hitBox->x + 18 - storageOffsetX;

         bulletStorage1.y = hitBox->y + 34 + storageOffsetY;
         bulletStorage2.y = hitBox->y + 17 + storageOffsetY;
         bulletStorage3.y = hitBox->y + 16 + storageOffsetY;
         bulletStorage4.y = hitBox->y + 32 + storageOffsetY;
         bulletStorage5.y = hitBox->y + 8 + storageOffsetY;
}
