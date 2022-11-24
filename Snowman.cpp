#include "Snowman.h"


Snowman::Snowman(int x, int y)              ///DONT CHANGE ANYTHING IN HERE EVER BECAUSE I FIXED ALL THE BUGS AND DONT WANT ANYMORE
{                                                                                                                   //theres more bugs
    hitBox = new SDL_Rect;
    hitBoxNose = new SDL_Rect;
    hitBoxHead = new SDL_Rect;

    health = 0;
    onFlatSurface = false;

    xPos = 999;
	yPos = 999;
    xPosNose = 999;
	yPosNose = 999;

	hitBox->h = 40;
	hitBox->w = 30;
    hitBox->x = x;
	hitBox->y = y;

	hitBoxHead->h = 20;
	hitBoxHead->w = 24;

	hitBoxNose->h = 10;
	hitBoxNose->w = 10;
	hitBoxNose->x = 970;
	hitBoxNose->y = 970;

	rotation = 0;

	alive = true;
	openFire = false;
	time2shoot = false;
	firstStrike = false;
	noseActive = false;     ///too many false bools
	goingDown = false;
	time2explode = false;
	inFlight = false;
	exploding = false;
	noseLoaded = true;
}

Snowman::~Snowman()
{
   hitBox = NULL;
   hitBoxNose = NULL;
   hitBoxHead = NULL;
}

void Snowman::move(bool animationOn, bool time2fly, float playerPositionX, float playerPositionY, bool loadedNose)
{
    if(loadedNose) //if the animation of nose reloading is done then it like makes this bool of the nose being loaded true or something
    {
        noseLoaded = true;
    }

    if(!alive && !respawnTimer.isStarted())
    {
        respawnTimer.start();
    }

    if(respawnTimer.getTicks() > 5000)
    {
        rotation = 0;
        alive = true;
        openFire = false;
        time2shoot = false;
        firstStrike = false;
        noseActive = false;     ///too many false bools
        goingDown = false;
        time2explode = false;
        inFlight = false;
        exploding = false;
        noseLoaded = true;
        respawnTimer.stop();
    }

    if(!onFlatSurface)
    {
       hitBox->y += 6; //fall from gravity
    }

    hitBoxHead->x = hitBox->x + 3;
	hitBoxHead->y = hitBox->y - 20;

	time2explode = false;
	inFlight = false;

	if(noseActive && time2fly)
    {
        if(hitBoxNose->y > playerPositionY + 30 && hitBoxNose->y < playerPositionY + 131)
        {
            if(hitBoxNose->x < playerPositionX + 60 && hitBoxNose->x > playerPositionX - 45)
            {
                goingDown = false;
                time2fly = false;
                noseActive = false;
                time2explode = true; ///explosion!!!
                rotation = 0;
            }
        }
    }


	if(!animationOn)
    {
        time2shoot = false;
    }

    if(noseActive)
    {
        if(!goingDown)
        {
            if(time2fly && hitBoxNose->y > hitBoxHead->y - 350)
            {
                hitBoxNose->y -= 6;
                rotation = 0;
                inFlight = true;
            }
            else if(hitBoxNose->x < playerPositionX + 50 && time2fly)
            {
                goingDown = true;
            }
            else if(hitBoxNose->y < hitBoxHead->y - 300)
            {
                hitBoxNose->x -= 6;
                rotation = 270;
                inFlight = true;
            }
        }
        else
        {
            hitBoxNose->y += 8;
            rotation = 180;
            inFlight = true;
        }
    }


    if(alive)
    {
        if(!firstStrike && !noseActive && noseLoaded) ///utter spaghetti shit
        {
            if(openFire && !shootTime.isStarted())
            {
                shootTime.start();
            }

            if(shootTime.getTicks() > 1300)
            {
                hitBoxNose->x = hitBoxHead->x - 14;
                hitBoxNose->y = hitBoxHead->y - 23;
                shootTime.stop();
                time2shoot = true;
                firstStrike = true;
                noseActive = true;
            }
        }
        else if(!noseActive && noseLoaded)
        {
            if(openFire && !shootTime.isStarted())
            {
                shootTime.start();
            }

            if(shootTime.getTicks() > 700)
            {
                hitBoxNose->x = hitBoxHead->x - 14;
                hitBoxNose->y = hitBoxHead->y - 23;
                shootTime.stop();
                time2shoot = true;
                firstStrike = true; //dont think i need this here but iduno
                noseActive = true;
            }
        }
    }

    if(time2explode)
    {
        explosionTimer.start();
    }

    if(explosionTimer.getTicks() > 500)
    {
        explosionTimer.stop();
    }

    if(explosionTimer.isStarted())
    {
        exploding = true;
    }
    else
    {
        exploding = false;
    }

    xPos = hitBox->x - 58;//maybe this should be in update(), refer to bullet.cpp
    yPos = hitBox->y - 73;

    xPosNose = hitBoxNose->x - 1;
	yPosNose = hitBoxNose->y - 8;
}
