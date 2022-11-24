#include "Player.h"

Player::Player()
{
    velocity = -3860;
    yVelocity = -2313;

    speed = 250;
    health = 6;

    xPos = 10;
    yPos = 10;

    hitBox = new SDL_Rect;
    hitBox->w = 30;
    hitBox->h = 121;
    hitBox->x = 10;
    hitBox->y = 10;

    rectangleForTheHair = new SDL_Rect;
    rectangleForTheHair->w = 40;
    rectangleForTheHair->h = 15;
    rectangleForTheHair->x = 10;
    rectangleForTheHair->y = 10;

    meleeHitbox = new SDL_Rect;
    meleeHitbox->w = 83;
    meleeHitbox->h = 13;
    meleeHitbox->x = 10;
    meleeHitbox->y = 10;

    muzzleHitbox = new SDL_Rect;
    muzzleHitbox->w = 15;
    muzzleHitbox->h = 10;
    muzzleHitbox->x = 10;
    muzzleHitbox->y = 10;


    jumping = false;
    doubleJumping = false;
    doubleDucking = false;
    doubleDuckingUnlocked = false;
    canDoubleJump = false;
    doubleFalling = false;
    gunBouncing = false;
    gunBounceDirection = 4;
    bounceHit = false;
    fallen = false;
    rising = false;
    missileToLeft = false;
    missileToRight = false;
    missileJuice = 0;
    released = false;
    onFlatSurface = false;
    goinLeft = false;
    goinRight = false;
    foo = false;
    bar = false;
    baz = false;
    cantGoRight = false;

    hasGun = true;
    gunDirection = 2;
    holdingUp = false;

    bullet = -1;
    shoot = false;

    uncrouch = false;
    canUncrouch = true;
    crouched = false;

    clip = 0;
    clipSize = 15;
    reserve = 900;

    animation = 0;
    flip =  SDL_FLIP_NONE;

    onZipline = false;

    nearOxygen = false;
    succingOxygen = false;

    controllable = true;

    secretUnlocked = false;
    secretSpeed = false;

    testingLeft = false;
    testingRight = false;

    meleeHit = false;
    endMeleeHit = false;

    paused = false;
    clicky = false;
    unclicky = false;
    clicking = false;

    walkToX = 0;
    walkingToLocation = 0;
    whatToDoWhenAtLocation = 0;

    ledgeHanging = false;
    ledgeLeaping = false;

    shootBuffer = false;

    granadoHitbox = new SDL_Rect;
    granadoHitbox->w = 8;
    granadoHitbox->h = 8;
    granadoHitbox->x = -2000;
    granadoHitbox->y = -2000;
    granadoActive = false;
	granadoLeft = false;
	granadoRight = false;
	granadoOnFlatSurface = false;
	granadoXpos = -2000;
	granadoYpos = -2000;
	granadoVelocityX = -2000;
	granadoVelocityY = -2000;
	granadoSpin = 360;
	granadoColor = 0;
	granadoExploding = false;
	time2explodeGranado = false;
	time2sizzle = false;
	granadoContact = false;
	blastJumping = false;
    blastJumpingRight = false;
	blastJumpingLeft = false;
	blastJumpSpeed = 0;

	ASSsoundMissile = false;
	ASSsoundRising = false;
	ASSsoundLand = false;

    moonGravity = false;

    nearItemButtonOrMachine = false;
    useItemButtonOrMachine = false;
}

Player::~Player()
{
    hitBox = NULL;
	rectangleForTheHair = NULL;
	meleeHitbox = NULL;
	muzzleHitbox = NULL;
	granadoHitbox = NULL;
}

void Player::handleMenuEvents( SDL_Event& event )
{
    if( event.type == SDL_MOUSEBUTTONDOWN )
    {
        clicky = true;
        clicking = true;
    }


    if( event.type == SDL_MOUSEBUTTONUP )
    {
        clicking = false;
        unclicky = true;
    }


    if( event.type == SDL_KEYDOWN && event.key.repeat == 0 ) //key pressed
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_LEFT:
                {

                } break;
            case SDLK_RIGHT:
                {

                } break;

            case SDLK_z:
                {

                } break;

            case SDLK_x:
                {

                } break;
            case SDLK_DOWN:
                {

                } break;
            case SDLK_UP:
                {

                } break;
            case SDLK_ESCAPE:
                {
                    if(!paused)
                    {
                        paused = true;
                    }
                    else
                    {
                        paused = false;
                    }
                }
        }
    }
    else if( event.type == SDL_KEYUP && event.key.repeat == 0 ) //key released
    {
        switch( event.key.keysym.sym )
        {
            case SDLK_LEFT: goinLeft = false; break;
            case SDLK_RIGHT: goinRight = false; break;
            case SDLK_z: released = true; break;
            case SDLK_w: released = true; break;
            case SDLK_x: { reloadTimer.stop(); }break;
            case SDLK_DOWN:{ uncrouch = true; } break;
            case SDLK_UP:
                {
                    holdingUp = false;
                    if(hasGun) ///why does it check if meleetimer isnt already started? youd want to restart it anyways
                    {
                        meleeTimer.start();
                    }
                } break;
        }
    }

}

void Player::handleEvent( SDL_Event& event )
{
    if( event.type == SDL_KEYDOWN && event.key.repeat == 0 ) //key pressed
    {
        if(controllable)
        {
            switch( event.key.keysym.sym )
            {
                case SDLK_LEFT:
                    {
                        if(onZipline)
                        {
                            goinLeft = true;
                            break;
                        }
                        else
                        {
                            goinLeft = true;
                            if(!flip) {ledgeHanging = false;} //this only exists so it plays the sound right, lole
                            //gunDirection = 4;
                            flip = SDL_FLIP_HORIZONTAL;
                        }
                    } break;

                case SDLK_RIGHT:
                    {
                        if(onZipline)
                        {
                            goinRight = true;
                            break;
                        }
                        else
                        {
                            goinRight = true;
                            if(flip) {ledgeHanging = false;} //this only exists so it plays the sound right, lole
                            //if(!gunDirection == 4) { gunDirection = 2;} ///WHY WHERE THESE HERE??? THIS IS ALREADY DECIDED SOMEWHERE ELSE?? and if its done here theres a bug where you can shoot right or left in 1 frame while holding up
                            flip = SDL_FLIP_NONE;
                        }
                    } break;

                case SDLK_z:
                    {
                        if(onFlatSurface)
                        {
                            jumping = true;
                            jumpTimer.start(); //okay iduno if this will break anything but when this wasnt here you couldnt jump
                            released = false;
                            canDoubleJump = true;
                        }

                        if(doubleDuckingUnlocked)
                        {
                            if(doubleFalling)  //cut feature
                            {
                                doubleDucking = true;
                                //yVelocity += 20;
                            }
                        }

                        if(gunBouncing && !gunBounceTimer.isStarted())
                        {
                            gunBounceTimer.start();
                            gunBounceDirection++;
                        }

                         if(canDoubleJump && !doubleJumping && !onFlatSurface && reloadTimer.getTicks() < 400)
                        {
                            reloadTimer.stop();
                            holdingUp = false;
                            doubleJumping = true;
                            jumpTimer.start();
                            released = false;
                            canDoubleJump = false;
                            doubleFalling = true;
                        }
                    } break;

                case SDLK_w:
                    {
                        if(onFlatSurface)
                        {
                            jumping = true;
                            jumpTimer.start();
                            released = false;
                            canDoubleJump = true;
                        }

                        if(gunBouncing && !gunBounceTimer.isStarted())
                        {
                            gunBounceTimer.start();
                            gunBounceDirection++;
                        }

                         if(canDoubleJump && !doubleJumping && !onFlatSurface && reloadTimer.getTicks() < 400)
                        {
                            reloadTimer.stop();
                            holdingUp = false;
                            doubleJumping = true;
                            jumpTimer.start();
                            released = false;
                            canDoubleJump = false;
                            doubleFalling = true;
                        }
                    } break;

                case SDLK_x:
                    {
                        if(!reloadTimer.isStarted() && reserve > 0 /*&& clip < 1*/ && hasGun && !doubleFalling)
                        {
                            reloadTimer.start();
                        }

                        if(clip > 0 && !doubleDucking)
                        {
                            shootBuffer = true;
                        }
                        else
                        if(!gunTimer.isStarted() && hasGun  && reloadTimer.getTicks() < 400 && !doubleDucking)
                        {
                            gunTimer.start();
                            if(clip > 0)
                            {
                                shoot = true;
                                bullet++;
                                shootBuffer = false;
                            }
                        }

                    } break;
                case SDLK_c:
                    {
                        if(nearOxygen && !crouched && onFlatSurface)
                        {
                            controllable = false;
                            walkingToLocation = true;
                        }
                        else if(!granadoActive && clip > 4 && ! nearItemButtonOrMachine)
                        {
                            clip -= 5;
                            granadoActive = true;
                        }
                        useItemButtonOrMachine = true;
                    }break;

                case SDLK_DOWN:
                    {
                        if(gunBouncing)
                        {
                            //lol
                        }
                        else if(!doubleFalling)
                        {
                          if(onFlatSurface)
                            {
                                //yVelocity += 10;
                            }
                            if(!fallen) hitBox->h = 91;
                            uncrouch = false;
                            crouched = true;
                        }

                        if(doubleFalling && hasGun)
                        {
                            gunBouncing = true;
                        }

                    } break;
                case SDLK_UP:
                    {
                        if(!doubleFalling && !reloadTimer.isStarted()) {
                        holdingUp = true; }
                        if(!gunBouncing) { meleeHit = false; }
                        meleeTimer.stop();
                        gunTimer.stop();
                    } break;
                case SDLK_ESCAPE:
                    {
                        if(!paused)
                        {
                            paused = true;
                        }
                        else
                        {
                            paused = false;
                        }
                    }
            }
        }
        else
        {
            switch( event.key.keysym.sym )
            {
                case SDLK_ESCAPE:
                    {
                        if(!paused)
                        {
                            paused = true;
                        }
                        else
                        {
                            paused = false;
                        }
                    }
                    break;
                case SDLK_z:
                    {
                        if(fallen)
                        {
                                yVelocity -= 110;
                                hitBox->w = 30;
                                hitBox->h = 121;
                                fallen = false;
                                rising = true;
                                ASSsoundRising = true;
                                risingTimer.start();
                        }
                        else if (rising)
                        {
                            ///do a spin jump
                            if(risingTimer.getTicks() > 450)
                            {
                                reloadTimer.stop(); ///delete this shit later
                                holdingUp = false;
                                doubleJumping = true;
                                jumpTimer.start();
                                released = false;
                                canDoubleJump = false;
                                doubleFalling = true;
                                ASSsoundMissile = true;

                                if(goinRight)
                                {
                                    missileToRight = true;
                                    missileJuice = 60;
                                    flip = SDL_FLIP_NONE;
                                }
                                else if(goinLeft)
                                {
                                    missileToLeft = true;
                                    missileJuice = 60;
                                    flip = SDL_FLIP_HORIZONTAL;
                                }
                                else
                                {
                                    missileToLeft = true; ///change this to a upward jump later
                                    missileJuice = 60;
                                    flip = SDL_FLIP_HORIZONTAL;
                                }
                            rising = false;
                            risingTimer.stop();
                            }
                        }

                    } break;
                    case SDLK_w:
                    {
                        if(fallen)
                        {
                                yVelocity -= 90;
                                hitBox->w = 30;
                                hitBox->h = 121;
                                fallen = false;
                                rising = true;
                                ASSsoundRising = true;
                                risingTimer.start();
                        }
                        else if (rising)
                        {
                            ///do a spin jump
                            if(risingTimer.getTicks() > 450)
                            {
                                reloadTimer.stop(); ///delete this shit later
                                holdingUp = false;
                                doubleJumping = true;
                                jumpTimer.start();
                                released = false;
                                canDoubleJump = false;
                                doubleFalling = true;
                                ASSsoundMissile = true;

                                if(goinRight)
                                {
                                    missileToRight = true;
                                    missileJuice = 60;
                                    flip = SDL_FLIP_NONE;
                                }
                                else if(goinLeft)
                                {
                                    missileToLeft = true;
                                    missileJuice = 60;
                                    flip = SDL_FLIP_HORIZONTAL;
                                }
                                else
                                {
                                    missileToLeft = true; ///change this to a upward jump later
                                    missileJuice = 60;
                                    flip = SDL_FLIP_HORIZONTAL;
                                }
                            rising = false;
                            risingTimer.stop();
                            }
                        }

                    } break;

                    case SDLK_LEFT:
                    {
                        if(onZipline)
                        {
                            goinLeft = true;
                            break;
                        }
                        else
                        {
                            goinLeft = true;
                            if(!flip) {ledgeHanging = false;} //this only exists so it plays the sound right, lole
                            //gunDirection = 4;
                        }
                    } break;

                case SDLK_RIGHT:
                    {
                        if(onZipline)
                        {
                            goinRight = true;
                            break;
                        }
                        else
                        {
                            goinRight = true;
                            if(flip) {ledgeHanging = false;} //this only exists so it plays the sound right, lole
                            //if(!gunDirection == 4) { gunDirection = 2;} ///WHY WHERE THESE HERE??? THIS IS ALREADY DECIDED SOMEWHERE ELSE?? and if its done here theres a bug where you can shoot right or left in 1 frame while holding up

                        }
                    } break;



                if( event.type == SDL_KEYUP && event.key.repeat == 0 ) //key released  //squash
                {
                    switch( event.key.keysym.sym )
                    {
                        case SDLK_LEFT: goinLeft = false; break;
                        case SDLK_RIGHT: goinRight = false; break;
                        case SDLK_z: released = true; break;
                        case SDLK_w: released = true; break;
                        case SDLK_x: { reloadTimer.stop(); }break;
                        case SDLK_DOWN:{ uncrouch = true; } break;
                    }
                }
            }
        }
    }

    else if( event.type == SDL_KEYUP && event.key.repeat == 0 ) //key released
    {
            switch( event.key.keysym.sym )
            {
                case SDLK_LEFT: goinLeft = false; break;
                case SDLK_RIGHT: goinRight = false; break;
                case SDLK_z: released = true; break;
                case SDLK_w: released = true; break;
                case SDLK_x: { if(reloadTimer.getTicks() < 400) { reloadTimer.stop();} }break;
                case SDLK_DOWN:{ if(crouched) {uncrouch = true;} } break;
                case SDLK_UP:
                    {
                        gunTimer.stop();                                                                                                        //what???
                        holdingUp = false;                                                                                                          //what???
                        if(hasGun) ///why does it check if meleetimer isnt already started? youd want to restart it anyways
                        {
                            meleeTimer.start();
                        }

                        if(ledgeHanging)
                        {
                            ledgeLeaping = true;
                        }
                    } break;
        }
    }
}

void Player::walkToLocation()
{
    switch(whatToDoWhenAtLocation)
    {
        case 0:
        // do nothing
        break;
        case 1: /// 1 = start using oxygen tank
        {
            holdingUp = false;
            if(hitBox->x < walkToX)
            {
                goinRight = true;
                goinLeft = false;
                flip = SDL_FLIP_NONE;

            }

            if(hitBox->x > walkToX)
            {
                goinLeft = true;
                goinRight = false;
                flip = SDL_FLIP_HORIZONTAL;
            }

            if(velocity > walkToX - 8 && velocity < walkToX + 8)
            {
                flip = SDL_FLIP_NONE;
                goinLeft = false;
                goinRight = false;
                succingOxygen = true;
                velocity = walkToX;
                walkingToLocation = false;
            }
        }
        break;
    }

}

void Player::move( float timeStep)
{
    if(endMeleeHit)
    {
        endMeleeHit = false;
        meleeHit = true;
    }

    if(gunBounceTimer.isStarted() && bounceHit)
    {
        {
            yVelocity -= 85 * 0.16; //go up


            yVelocity += gunBounceTimer.getTicks() / 10 * 0.16;
        }
    }

    if(shootBuffer && !gunTimer.isStarted() && hasGun  && reloadTimer.getTicks() < 400 && !doubleDucking && !gunBouncing)
            {
                gunTimer.start();
                if(clip > 0)
                {
                    shoot = true;
                    bullet++;
                    shootBuffer = false;
                }
            }


    if(gunDirection != 1) {ledgeHanging = false;}

    if(walkingToLocation)
    {
        walkToLocation();
    }

    //move horizontally
    if(goinLeft && !onZipline && !baz && !fallen & !rising && !succingOxygen) // && !cantGoLeft)
    {
        velocity -= speed * timeStep;
        //if(blastJumpingRight) { blastJumpSpeed -= 1; }
    }

    if(goinRight && !onZipline && !bar && !fallen && !rising && !succingOxygen)//&& !cantGoRight)
    {
        velocity += speed * timeStep;
    }

    //move vertically
    if(!onFlatSurface)
    {
        if(moonGravity)
        {
            yVelocity += 2;
        }
        else
        {
            yVelocity += 6.4; //fall from gravity
        }
    }

    if(jumping || doubleJumping)
    {
        jump(timeStep);
    }

    if(blastJumping || blastJumpingLeft || blastJumpingRight)
    {
        blastJump();
    }

    hitBox->x = velocity;
    hitBox->y = yVelocity;

    if(meleeTimer.getTicks() > 399)
    {
        meleeTimer.stop();
    }

    if(gunBounceTimer.getTicks() > 699)
    {
        gunBounceTimer.stop();
        meleeHit = false;
        bounceHit = false;
    }

    if(testingLeft || testingRight)
    {
        kbTest();
    }

    if(ledgeLeaping && !onFlatSurface) {  ledgeLeap();}

    foo = false;
    bar = false;
    baz = false;
    ect = false;

    if(crouched && !fallen && !doubleFalling)
    {
        hitBox->h = 91;
    }

    if(risingTimer.getTicks() > 800)
    {
        rising = false;
        controllable = true;
        risingTimer.stop();
        if(flip == SDL_FLIP_NONE)
        {
            flip = SDL_FLIP_HORIZONTAL;
        }
        else
        {
            flip = SDL_FLIP_NONE;
        }
    }

    if(missileToRight)
    {
        missileRight();
    }
    else if(missileToLeft)
    {
        missileLeft();
    }

    if(damageColor.getTicks() > 80)
    {
        damageColor.stop();
    }

    nearItemButtonOrMachine = false;
}

void Player::jump(float timeStep)
{
    //do a FUCKing retarded jump that lasts a max of 600ms
   // if(!jumpTimer.isStarted())
   // {
   //     jumpTimer.start();
   // }

   if(moonGravity)
   {
       yVelocity -= 350 * timeStep;
   }
   else
   {
        yVelocity -= 700 * timeStep; //go up
   }


    if(moonGravity)
    {
        if(jumpTimer.getTicks() < 500)
        {
            if(!released) //if the jump key is released you drop like a baby
            {
                yVelocity += (jumpTimer.getTicks() / 1 * timeStep) / 2;
            }
            else
            {
                yVelocity += (jumpTimer.getTicks() / 0.5 * timeStep) / 2;
            }
        }
        else
        {
            if(!released) //if the jump key is released you drop like a baby
            {
                yVelocity += 250 / 2 * timeStep;
            }
            else
            {
                yVelocity += 250 / 1 * timeStep;
            }
        }
    }
    else
    {
        if(jumpTimer.getTicks() < 500)
        {
            if(!released) //if the jump key is released you drop like a baby
            {
                yVelocity += jumpTimer.getTicks() / 2 * timeStep;
            }
            else
            {
                yVelocity += jumpTimer.getTicks() / 1 * timeStep;
            }
        }
        else
        {
            if(!released) //if the jump key is released you drop like a baby
            {
                yVelocity += 500 / 2 * timeStep;
            }
            else
            {
                yVelocity += 500 / 1 * timeStep;
            }
        }
    }

    if(jumpTimer.getTicks() >= 600)
    {
        jumping = false;
        doubleJumping = false;
        jumpTimer.stop();

    }
}

void Player::blastJump() ///goddamnit i am a dumbass, i only wrote it this way because of an old bug, dont use timers use speed int //what
{
    if(blastJumpingRight)
    {
        velocity += blastJumpSpeed * 0.16; //go right


        if(onFlatSurface && blastJumpSpeed > 0)
        {
            blastJumpSpeed -= 5;
        }

        if(blastJumpSpeed < 1)
        {
            blastJumpingRight = false;
        }

        blastJumpSpeed -= 0.2;
    }

    if(blastJumpingLeft)
    {
        velocity -= blastJumpSpeed * 0.16; //go right


        if(onFlatSurface && blastJumpSpeed > 0)
        {
            blastJumpSpeed -= 5;
        }

        if(blastJumpSpeed < 1)
        {
            blastJumpingLeft = false;
        }

        blastJumpSpeed -= 0.2;
    }

    if(blastJumping)
    {
        if(!blastJumpTimer.isStarted())
        {
            blastJumpTimer.start();
        }

        if(!ect)
        {
            yVelocity -= 150 * 0.16; //go up


            yVelocity += blastJumpTimer.getTicks() / 5 * 0.16;
        }
        else ///delete this if else statement??
        {
            blastJumping = false;
            blastJumpTimer.stop();
        }
    }


    if(blastJumpTimer.getTicks() >= 670)
    {
        blastJumping = false;
        blastJumpTimer.stop();
    }
}

void Player::missileLeft()
{
    hitBox->h = 40;

    velocity -= missileJuice * 0.27;

    if(missileJuice < 7)
    {

    }
    else
    {
        missileJuice -= 1;
    }

    if(missileJuice <= 0)
    {
        missileToLeft = false;
        controllable = true;
        //hitBox->h = 121;
        //yVelocity -= 60;
    }
}

void Player::missileRight()
{
    hitBox->h = 40;

    velocity += missileJuice * 0.27;

    if(missileJuice < 7)
    {

    }
    else
    {
        missileJuice -= 1;
    }

    if(missileJuice <= 0)
    {
        missileToRight = false;
        controllable = true;
        //hitBox->h = 121;
        //yVelocity -= 60;
    }
}

void Player::kbTest() ///uh i think this is knockback code
{
    if(testingLeft)
    {
        ///lol im just copying jump code
        if(!testLeftTimer.isStarted())
        {
            testLeftTimer.start();
        }

        velocity -= 13;


        velocity += testLeftTimer.getTicks() / 2 * 0.16;


        if(testLeftTimer.getTicks() >= 160)
        {
            testingLeft = false;
            testLeftTimer.stop();
        }
    }

    if(testingRight)
    {
        ///lol im just copying jump code
        if(!testRightTimer.isStarted())
        {
            testRightTimer.start();
        }

        velocity += 13;


        velocity -= testRightTimer.getTicks() / 2 * 0.16;


        if(testRightTimer.getTicks() >= 160)
        {
            testingRight = false;
            testRightTimer.stop();
        }
    }
}

void Player::ledgeLeap()
{
        if(!ledgeTimer.isStarted())
        {
            ledgeTimer.start();
        }

        if(flip)
        {
            velocity += 3;
        }
        else
        {
            velocity -= 3;
        }
        yVelocity -= 22;

        yVelocity += ledgeTimer.getTicks() / 6 * 0.16;

        if(ledgeTimer.getTicks() >= 490)
        {
            ledgeLeaping = false;
            ledgeTimer.stop();
        }
}

void Player::update()                      ///UpDAATEEE
{
                ASSsoundMissile = false;
                ASSsoundRising = false;
                ASSsoundLand = false;

                if(gunBounceDirection > 4)
                {
                    gunBounceDirection = 1;
                }

                if(walkingToLocation)
                {
                    speed = 230;
                }
                else if(!crouched || doubleFalling)
                {
                    if(!secretSpeed)
                    {
                        speed = 350;
                    }
                    else
                    {
                        speed = 1050; //950
                    }
                }
                else
                {
                    if(!onFlatSurface) // && !jumpTimer.isStarted()) //released)
                    {
                        if(released || jumpTimer.getTicks() > 500 || !jumpTimer.isStarted())
                        {
                            speed = 410; ///crouch jumping
                        }
                        else
                        {
                            printf("SDFSSFABC");

                            if(jumpTimer.getTicks() > 420)
                            {
                                speed = 350;
                            }
                            else if(jumpTimer.getTicks() > 370)
                            {
                                speed = 300;
                            }
                            else if(jumpTimer.getTicks() > 270)
                            {
                                 speed = 200;
                            }
                            else if(jumpTimer.getTicks() > 200)
                            {
                                 speed = 120;
                            }
                        }
                    }
                    else
                    {
                        speed = 120;
                    }
                }

                if(foo) //if collision with flat surface detected
                {
                    onFlatSurface = true;
                }
                else
                {
                    onFlatSurface = false;
                }

                if(onFlatSurface)
                {
                    doubleJumping = false;
                    canDoubleJump = false;
                    if(doubleFalling)
                    {
                        if(doubleDucking || gunBouncing)
                        {
                            if(!crouched)
                            {
                                yVelocity -= 91; ///71 REMOVEZ
                                hitBox->h = 121;
                            }
                            else
                            {
                                yVelocity -= 90;
                                hitBox->h = 91; ///91
                            }
                        }
                        else
                        {
                            hitBox->h = 20;
                            hitBox->w = 30;
                            controllable = false;
                            fallen = true;
                            yVelocity += 20;
                            crouched = false;
                            doubleDucking = false;

                            missileToLeft = false;
                            missileToRight = false;
                            missileJuice = 0;
                            ASSsoundLand = true;
                            if(flip)
                            {
                                //velocity -= 25;
                            }
                            else
                            {
                                //velocity -= 20;
                            }
                        }
                    }
                    doubleFalling = false;
                    doubleDucking = false;
                    gunBouncing = false;
                }




                /*if(bar) //if collision with left wall detected
                {
                    cantGoRight = true;
                }
                else
                {
                    cantGoRight = false;
                }

                if(baz) //if collision with right wall detected
                {
                    cantGoLeft = true;
                }
                else
                {
                    cantGoLeft = false;
                }*/

                if(crouched && uncrouch && !doubleFalling && !fallen && !rising) ///i probably dont need to check all these bools, doing them should automatically set crouch to false anyways
                {
                    if(onFlatSurface && canUncrouch)
                    {
                        yVelocity -= 30;
                        hitBox->h = 121;
                        uncrouch = false;
                        crouched = false;
                        hairHitboxTimerLol.start();
                    }
                    else if(onZipline)
                    {
                       hitBox->h = 121;
                       uncrouch = false;
                       crouched = false;
                    }
                }

                if(doubleFalling)
                {
                    hitBox->h = 40;
                }


                hitBox->x = velocity;
                hitBox->y = yVelocity;
                xPos = hitBox->x - 18;
                yPos = hitBox->y - 16;
                rectangleForTheHair->x = hitBox->x - 5;
                rectangleForTheHair->y = hitBox->y - 15;
                if(fallen)
                {
                    if(flip)
                    {
                        rectangleForTheHair->x = hitBox->x -30;
                        rectangleForTheHair->y = hitBox->y;
                        rectangleForTheHair->w = 15;
                        rectangleForTheHair->h = 20;
                    }
                    else
                    {
                        rectangleForTheHair->x = hitBox->x + 40;
                        rectangleForTheHair->y = hitBox->y;
                        rectangleForTheHair->w = 15;
                        rectangleForTheHair->h = 20;
                    }
                }
                else if(missileJuice > 0)
                {
                    if(flip)
                    {
                        rectangleForTheHair->x = hitBox->x -25;
                        rectangleForTheHair->y = hitBox->y;
                        rectangleForTheHair->w = 20;
                        rectangleForTheHair->h = 40;
                    }
                    else
                    {
                        rectangleForTheHair->x = hitBox->x + 35;
                        rectangleForTheHair->y = hitBox->y;
                        rectangleForTheHair->w = 20;
                        rectangleForTheHair->h = 40;
                    }
                }
                else if(gunBouncing)
                {
                    rectangleForTheHair->y = hitBox->y + 41;
                }
                else
                {
                    rectangleForTheHair->h = 15;
                    rectangleForTheHair->w = 40;
                }

                if(doubleDucking)
                {
                    if(flip)
                    {
                        rectangleForTheHair->x = hitBox->x -40;
                        rectangleForTheHair->y = hitBox->y + 25;
                        rectangleForTheHair->w = 15;
                        rectangleForTheHair->h = 35;
                    }
                    else
                    {
                        rectangleForTheHair->x = hitBox->x +55;
                        rectangleForTheHair->y = hitBox->y +25 ;
                        rectangleForTheHair->w = 15;
                        rectangleForTheHair->h = 35;
                    }
                }

                if(flip) { muzzleHitbox->x = rectangleForTheHair->x + 29; } else {muzzleHitbox->x = rectangleForTheHair->x - 14;}
                muzzleHitbox->y = rectangleForTheHair->y - 25;

                if(gunBouncing)
                {
                    meleeHitbox->w = 73;
                    meleeHitbox->h = 43;
                    if(flip == SDL_FLIP_NONE)
                    { meleeHitbox->x = hitBox ->x - 15; }
                    else
                    { meleeHitbox->x = hitBox ->x - 37;}
                    meleeHitbox->y = hitBox ->y + 59;
                }
                else
                {
                    meleeHitbox->w = 83;
                    meleeHitbox->h = 13;
                    if(flip == SDL_FLIP_NONE)
                    { meleeHitbox->x = hitBox ->x - 15; }
                    else
                    { meleeHitbox->x = hitBox ->x - 37;}
                    meleeHitbox->y = hitBox ->y + 59;
                }



                shoot = false;
                if(gunTimer.getTicks() >= 150) //have to wait 100ms until ye can shoot
                {
                    gunTimer.stop();
                }
                if(bullet >= 18)
                {
                    bullet = -1;
                }
                if(reloadTimer.getTicks() > 1930) //1530ms reload time
                {
                    reload();
                }

                if(holdingUp)
                {
                    gunDirection = 1;
                }
                else if(flip == SDL_FLIP_NONE)
                {
                    gunDirection = 2;  ///gun pointing right??? why did i make this numbers without commentating ???
                }
                else
                {
                    gunDirection = 4; ///gun pointing left??
                }

               /* if(holdingUp)
                {
                    gunDirection = 1; ///WHY IS THIS HERE TWICE??
                }*/

                if(gunDirection == 1) //can't reload when you're pointing up
                {
                    reloadTimer.stop();
                }


                ///GRANADO SHIT                                         GRANADO SHIT                      GRANADO SHIT                      GRANADO SHIT
                time2explodeGranado = false;
                time2sizzle = false; //obnocious fucking sound bools

                if(granadoContact)
                {
                    granadoTimer.startTicks += granadoTimer.getTicks() - 3000;
                    granadoContact = false;
                }

                if(granadoTimer.getTicks() > 3600) //explosion finished
                {
                    granadoExploding = false;
                    granadoVelocityX = -2000;
                    granadoVelocityY = -2000;
                    granadoHitbox->h = 8;
                    granadoHitbox->w = 8;
                    granadoSpeed = 0;
                    granadoSpeedY = 0;
                    granadoOnFlatSurface = false;

                    granadoActive = false;
                    granadoTimer.stop();
                }
                else if(granadoTimer.getTicks() > 3000)
                {
                    if(!granadoExploding) { granadoVelocityY -= 77; granadoVelocityX -= 35; time2explodeGranado = true;}
                    granadoExploding = true; //times up, it explodes now
                    granadoLeft = false;
                    granadoRight = false;
                    granadoHitbox->h = 85;
                    granadoHitbox->w = 85;
                    granadoVelocityY -=6;
                }
                else if(granadoTimer.getTicks() > 2400)
                {
                    if(granadoColor > 0)
                    {
                        granadoColor -= 15;
                    }
                    else
                    {
                        granadoColor = 240;
                    }
                }
                else
                {
                    if(granadoColor > 0)
                    {
                        granadoColor -= 5;
                    }
                    else
                    {
                        granadoColor = 175;
                    }
                }


                if(granadoLeft) ///make grenade spin fast in air, slower if on floor, but FAST if kicked
                {
                    if(granadoOnFlatSurface)
                    {
                        granadoSpin -= 1.5;
                        if(granadoSpeed > 20)
                        {
                            granadoSpin -= 2.5;
                        }
                    }
                    else
                    {
                        granadoSpin -= 8;
                    }

                    if(granadoSpin < 0)
                    {
                        granadoSpin = 360;
                    }
                }
                else if(granadoRight) ///copy the stuff from over here to the stuff above for granadoLeft lol
                {
                    if(granadoOnFlatSurface)
                    {
                        if(granadoSpeed > 0)
                        {
                            granadoSpin += 2.5;
                        }
                        else
                        {
                            granadoSpin -= 2.5;
                        }
                        if(granadoSpeed > 20)
                        {
                            granadoSpin += 4.5; //it spins faster when going to the right for some reason?
                        }
                        if(granadoSpeed < -20)
                        {
                            granadoSpin -= 4.5; //it spins faster when going to the right for some reason?
                        }
                    }
                    else
                    {
                        granadoSpin += 8;
                    }

                    if(granadoSpin > 360)
                    {
                        granadoSpin = 0;
                    }
                }



                granadoVelocityY += 6; //gravity

                if(granadoActive)  ///player has pressed C
                {
                    if(!granadoTimer.isStarted()) ///makes a grenade if one isnt already active
                    {
                        granadoTimer.start();
                        time2sizzle = true;

                        switch( gunDirection ) //god fucking daamn switch statements look so fucking ugly //seriosurly i cant fucking stand it
                        {
                            case 1:
                                {
                                    ///able to catch grande in hair hitbox for free refund
                                    granadoSpeedY = 185;
                                    if(flip == SDL_FLIP_HORIZONTAL) { granadoRight = true;  granadoVelocityX = hitBox->x + 11;}
                                    if(flip == SDL_FLIP_NONE) { granadoLeft = true; granadoVelocityX = hitBox->x - 3; }
                                    granadoVelocityY = hitBox->y - 10;
                                } break;
                            case 2:
                                {
                                    granadoLeft = true;
                                    granadoVelocityX = hitBox->x;
                                    granadoVelocityY = hitBox->y;
                                    granadoSpeed = 75;
                                    granadoSpeedY = 85;
                                } break;

                            case 3:
                                {
                                    //lol imagine if bullets went down
                                    //unless...?
                                } break;

                            case 4:
                                {
                                    granadoRight = true;
                                    granadoVelocityX = hitBox->x + 15;
                                    granadoVelocityY = hitBox->y;
                                    granadoSpeed = 75;
                                    granadoSpeedY = 85;
                                } break;
                        }
                    }
                }

                if(granadoLeft)
                {
                    //left, goes slower as it goes on
                    granadoVelocityX -= granadoSpeed / 13;
                    granadoVelocityY -= granadoSpeedY / 10;
                    if(granadoSpeed > 10) { granadoSpeed -= 2.3;}
                    if(granadoSpeedY > 10) { granadoSpeedY -= 3;}
                    if(granadoSpeed < 10) { granadoSpeed += 2.3;}
                }
                else if(granadoRight)
                {
                    //left, goes slower as it goes on, speed starts out at 75x 85y and decreases by 2.3x 3y
                    granadoVelocityX += granadoSpeed / 13;
                    granadoVelocityY -= granadoSpeedY / 10;
                    if(granadoSpeed > 10) { granadoSpeed -= 2.3;}
                    if(granadoSpeedY > 10) { granadoSpeedY -= 3;}
                    if(granadoSpeed < 10) { granadoSpeed += 2.3;}
                }

                granadoHitbox->x = granadoVelocityX;
                granadoHitbox->y = granadoVelocityY;

                granadoXpos = granadoHitbox->x - 4;
                granadoYpos = granadoHitbox->y - 4;

                if(granadoExploding)
                {
                    granadoXpos = granadoHitbox->x + 10;
                    granadoYpos = granadoHitbox->y + 24;
                }

        if(hairHitboxTimerLol.isStarted())
        {
            rectangleForTheHair->h = 35;
        }

        if(hairHitboxTimerLol.getTicks() > 200)
        {
            hairHitboxTimerLol.stop();
        }

        useItemButtonOrMachine = false;
}

void Player::reload()
{
    reloadTimer.stop();

    while(clip < clipSize)
    {
        reserve--;
        clip++;
        if(reserve < 1)
        {
            return;
        }
    }
}

short int Player::getAnimation()  ///whats the point of the animation variable if i can just use this function
{
    if(gunBouncing)
    {
        animation = 12;
        return 12;
    }

    if(doubleDucking)
    {
        animation = 11;
        return 11;
    }

    if(rising)
    {
        animation = 9;
        return 9;
    }

    if(missileJuice > 5)
    {
        animation = 10;
        return 10;
    }

    if(fallen)
    {
        animation = 8;
        return 8;
    }

    if(succingOxygen)
    {
        animation = 6;
        return 6;
    }

    if(doubleFalling)
    {
        animation = 7;
        return 7;
    }

    if(!onFlatSurface)
    {
        if(!crouched)
        {
            animation = 3;
            return  3;
        }
        else
        {
            animation = 4;
            return  4;
        }
    }
    else if(crouched)
    {
        if(goinLeft || goinRight)
        {
            animation = 5;
            return  5;
        }
        else
        {
            animation = 4;
            return  4;
        }
    }
    else if(goinLeft && goinRight)
    {
        animation = 0;
        return 0;
    }
    else if(onFlatSurface && goinLeft || goinRight)
    {
        animation = 2;
        return 2;
    }
    else
    {
        animation = 0;
        return 0;
    }
}
