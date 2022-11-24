#include "Texture.h"

Texture::Texture()
{
	//Initialize
	texture = NULL;
	width = 0;
	height = 0;

	animationClip = {0, 0, 0, 0};

	animationTimer1.start();
	animationTimer2.start();
	animationTimer3.start();
	/*animationTimer4.stop();
	animationTimer5.stop();
	animationTimer6.stop();*/

    gunOffset = 0;
    robotAnimation = 0;
    animationBool = false;
    animationBool2 = false;
    animationBool3 = true; ///WHY IS IT TRUE HERE
    animationInt = 0;
    flip1 =  SDL_FLIP_NONE;
}

Texture::Texture(int w, int h)
{
	//Initialize
	texture = NULL;
	width = 0;
	height = 0;

	animationTimer1.start();
	animationTimer2.start();
	animationTimer3.start();
	/*animationTimer4.stop();
	animationTimer5.stop();
	animationTimer6.stop();*/

	animationClip = {0, 0, w, h};
    gunOffset = 0;
    animationBool = false;
    animationBool2 = false;
    animationBool3 = false; ///BUT FALSE HERE?
    animationInt = 0;
    flip1 =  SDL_FLIP_NONE;
}


Texture::~Texture()
{
	//Deallocate
	free();
}

bool Texture::loadFile( std::string path, SDL_Renderer* renderer )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Failed to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
	    //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Failed to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
		    width = loadedSurface->w;
			height = loadedSurface->h;
		}
		SDL_FreeSurface( loadedSurface );
	}
	texture = newTexture;
	newTexture = NULL;
	return texture != NULL;
}

bool Texture::loadFromRenderedText( std::string textureText, SDL_Color textColor, TTF_Font* font, SDL_Renderer* renderer )
{
    free();

	SDL_Surface* textSurface = TTF_RenderText_Solid( font, textureText.c_str(), textColor );
	if( textSurface != NULL )
    {
        texture = SDL_CreateTextureFromSurface( renderer, textSurface );
		if( texture == NULL )
		{
			printf( "Failed to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			width = textSurface->w;
			height = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Failed to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}


	//Return success
	return texture != NULL;
}

void Texture::free()
{
	//Free texture if it exists
	if( texture != NULL )
	{
		SDL_DestroyTexture( texture );
		texture = NULL;
		width = 0;
		height = 0;
	}
}

void Texture::setColor( short red, short green, short blue )
{
	SDL_SetTextureColorMod( texture, red, green, blue );
}

void Texture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( texture, blending );
}

void Texture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( texture, alpha );
}

void Texture::render( SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, width, height };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( renderer, texture, clip, &renderQuad, angle, center, flip );
}

void Texture::render2( SDL_Renderer* renderer, int x, int y, int wee, int hee, int ax, int ay, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen


	//Set clip rendering dimensions
	/*if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}*/

	animationClip.x = ax;
    animationClip.w = 22;
    animationClip.h = 70;

	//Render to screen

	SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
    SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0 , flip1);
}

void Texture::render3( SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, width, height };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx( renderer, orig, clip, &renderQuad, angle, center, flip );
}

void Texture::defaultAnimate( SDL_Renderer* renderer, int x, int y, int w, int h, int fps, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	animationClip.y = 0;
	animationClip.w = w;
	animationClip.h = h;

    animationClip.x = w * (animationTimer1.getTicks() / fps);

    if(animationClip.x > 3 * animationClip.w)
    {
            animationTimer1.start();
            animationClip.x = 3 * animationClip.w;
    }

	//Render to screen
	SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
    SDL_RenderCopy( renderer, texture, &animationClip, &renderQuad);
}


void Texture::renderAmmo(int clip, SDL_Renderer* renderer, int x, int y)
{
    animationClip.x = 53 * clip;
    SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
    SDL_RenderCopy( renderer, texture, &animationClip, &renderQuad);
}

void Texture::renderAmmo2(int clip, SDL_Renderer* renderer, int x, int y)
{
    animationClip.w = 23 * clip;
    animationClip.h = 23;
    SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
    SDL_RenderCopy( renderer, texture, &animationClip, &renderQuad);
}

void Texture::renderPlayer(int animation, SDL_Renderer* renderer, int x, int y, SDL_RendererFlip flip,  Timer bounceTimer, int bounceDirection)
{
    if(animation != 8)
    {
        animationTimer6.stop();
    }
    if(animation != 9)
    {
        animationTimer8.stop();
    }
    if(animation != 11)
    {
        animationTimer9.stop();
        animationTimer10.stop();
    }

    switch(animation)
    {
        case 0:
            {
                animationClip.y = 0;
                animationClip.w = 65;
                animationClip.h = 136;

                if(animationTimer1.getTicks() / 150 > 3)
                {
                    animationTimer1.start();
                }

                switch(animationTimer1.getTicks() / 150)
                {
                    case 0: gunOffset = 0;
                    break;
                    case 1:
                    case 3: gunOffset = 1;
                    break;
                    case 2: gunOffset = 2;
                    break;
                }

                animationClip.x = 65 * (animationTimer1.getTicks() / 150);

                SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
            }
            break;
        case 2:
            {
                animationClip.y = 144;
                animationClip.w = 66;
                animationClip.h = 136;

                if(animationTimer2.getTicks() / 90 > 10)
                {
                    animationTimer2.start();
                }

                animationClip.x = 66 * (animationTimer2.getTicks() / 90);

                gunOffset2 = 0;
                switch(animationTimer2.getTicks() / 90)
                {
                    case 4:
                    case 5: gunOffset2 = 1; //using gunoffset2 for the Face image offset
                }

                SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
            }
            break;
        case 3:
            {
                animationClip.y = 145;
                animationClip.x = 198;
                animationClip.w = 66;
                animationClip.h = 135;

                SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
            }
            break;
        case 4:
            {
                animationClip.y = 300;
                animationClip.x = 0;
                animationClip.w = 64;
                animationClip.h = 106;

                SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
            }
            break;
        case 5:
            {
                animationClip.y = 300;
                animationClip.w = 64;
                animationClip.h = 106;

                if(animationTimer3.getTicks() / 50 > 4)
                {
                    animationTimer3.start();
                }
                animationClip.x = 64 * (animationTimer3.getTicks() / 50);

                SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
            }
            break;
        case 6:
            {
                animationClip.y = 413;
                animationClip.w = 112;
                animationClip.h = 136;

                if(!animationTimer4.isStarted())
                {
                    animationTimer4.start();
                }

                if(animationTimer4.isStarted())
                {
                    if(animationTimer4.getTicks() / 95 > 18 )  ///arrggh all this shit is so complicated //but it works
                     {
                         animationTimer4.stop();
                         animationTimer5.stop();
                     }

                     if(animationTimer4.getTicks() / 95 > 9)
                     {
                         animationTimer4.pause();
                     }

                     if(animationTimer4.isPaused() && !animationTimer5.isStarted())
                     {
                         animationTimer5.start();
                     }

                     if(animationTimer5.getTicks() > 850)
                     {
                         animationTimer4.unpause();
                     }

                     animationClip.x = 112 * (animationTimer4.getTicks() / 95);

                     SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
                     SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                }
                break;
        case 7:
            {
                animationClip.w = 100;
                animationClip.h = 81;
                animationClip.y = 549;
                animationClip.x = 0;

                if(!flip)
                {
                    SDL_Rect renderQuad = { x -25, y, animationClip.w, animationClip.h };
                    SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                    return;
                }
                else
                {
                    SDL_Rect renderQuad = { x -7, y, animationClip.w, animationClip.h };
                    SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                    return;
                }
            }

                SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
            }
            break;
        case 8:
            {
                if(!animationTimer6.isStarted())
                {
                    animationTimer6.start();
                }



                animationClip.w = 148;
                animationClip.h = 86;
                animationClip.y = 626;
                animationClip.x = 148 * (animationTimer6.getTicks() / 50);
                if(animationClip.x > 296)
                {
                    animationClip.x = 296;
                }

                if(flip)
                {
                     SDL_Rect renderQuad = { x - 45 , y -20, animationClip.w, animationClip.h };
                    SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                }
                else
                {
                     SDL_Rect renderQuad = { x -45, y -20, animationClip.w, animationClip.h };
                    SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                }
            }
            break;
        case 9:
            {
                if(!animationTimer8.isStarted())
                {
                    animationTimer8.start();
                }



                animationClip.w = 162;
                animationClip.h = 173;
                animationClip.y = 712;
                animationClip.x = 162 * (animationTimer8.getTicks() / 60);
                if(animationClip.x > 1944)
                {
                    animationClip.x = 1944;
                }

                if(flip)
                {
                     SDL_Rect renderQuad = { x - 45 , y - 18 , animationClip.w, animationClip.h };
                    SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                }
                else
                {
                     SDL_Rect renderQuad = { x -51, y - 18, animationClip.w, animationClip.h };
                    SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                }
            }
            break;
            case 10:
            {
                if(animationClip.x > 708)
                {
                    animationTimer1.start();
                }

                animationClip.w = 118;
                animationClip.h = 70;
                animationClip.y = 885;
                animationClip.x = 118 * (animationTimer1.getTicks() / 60);


                if(flip)
                {
                    SDL_Rect renderQuad = { x -10, y, animationClip.w, animationClip.h };
                    SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                }
                else
                {
                    SDL_Rect renderQuad = { x - 45, y, animationClip.w, animationClip.h };
                    SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                }
            }
            break;
            case 11:
            {
                if(!animationTimer9.isStarted())
                {
                    animationTimer9.start();
                }

                if(!animationTimer10.isStarted())
                {
                    animationTimer10.start();
                }

                if(animationTimer10.getTicks() > 60)
                {
                    animationClip.w = 127;
                    animationClip.h = 91;
                    animationClip.y = 955;
                    animationClip.x = 254;
                }
                else
                {
                    if(animationTimer10.getTicks() > 30)
                    {
                        animationClip.w = 127;
                        animationClip.h = 91;
                        animationClip.y = 955;
                        animationClip.x = 127;
                    }
                    else
                    {
                        animationClip.w = 127;
                        animationClip.h = 91;
                        animationClip.y = 955;
                        animationClip.x = 0;
                    }
                }



                if(flip)
                {
                    if(animationClip.x > 254)
                    {
                        //animationTimer9.start();
                        animationClip.x = 254;
                    }

                    SDL_Rect renderQuad = { x - 33, y + 5, animationClip.w, animationClip.h }; //y shuld be + 12 REMOVEZ
                    SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                }
                else
                {
                    if(animationClip.x > 254)
                    {
                        //animationTimer9.start();
                        animationClip.x = 254;
                    }

                    SDL_Rect renderQuad = { x - 26, y + 5, animationClip.w, animationClip.h };
                    SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                }


                break;
                case 12:
                {
                    animationClip.w = 215;
                    animationClip.h = 133;

                    switch(bounceDirection)
                    {
                        case 1:
                        {
                            animationClip.y = 1046;
                        }
                        break;
                        case 2:
                        {
                            animationClip.y = 1179;
                        }
                        break;
                        case 3:
                        {
                            animationClip.y = 1312;
                        }
                        break;
                        case 4:
                        {
                            animationClip.y = 1445;
                        }
                        break;
                    }

                    animationClip.x = 215 * (bounceTimer.getTicks() / 60);

                    if(animationClip.x > 430)
                    {
                        animationClip.x = 430;
                    }

                    if(bounceTimer.getTicks() < 1)
                    {
                        animationClip.x = 430;
                    }

                    if(flip)
                    {
                        SDL_Rect renderQuad = { x - 66, y - 26, animationClip.w, animationClip.h }; //y shuld be + 12 REMOVEZ
                        SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, SDL_FLIP_NONE);
                    }
                    else
                    {
                        SDL_Rect renderQuad = { x - 66, y - 26, animationClip.w, animationClip.h };
                        SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, SDL_FLIP_NONE);
                    }

                }
                break;
            }
    }


}

void Texture::renderFace(SDL_Renderer* renderer, int x, int y, SDL_RendererFlip flip, bool succing, int idleOffset, int animation, int runningOffset)
{
        if(animationBool)
        {
            animationClip.y = 23;
        }
        else
        {
            animationClip.x = 0;
            animationClip.y = 0;
        }
        animationClip.w = 20;
        animationClip.h = 23;


        if(animationBool)
        {
            if(animationTimer1.getTicks() > 2800)
            {
                animationTimer1.start();
            }

            animationClip.x = 20 * (animationTimer1.getTicks() / 100);

            if(animationTimer1.getTicks() / 100 > 2)
            {
                animationClip.x = 0;
            }
        }

        switch(animation)
        {
            case 0:
            {
                if(flip)
                {
                    SDL_Rect renderQuad = { x + 19, y + 35 + idleOffset, animationClip.w, animationClip.h };
                    SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                }
                else
                {
                    SDL_Rect renderQuad = { x + 26, y + 35 + idleOffset, animationClip.w, animationClip.h };
                    SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                }
                break;
                }
            case 1:
                {
                    break;
                }
            case 2:
                {
                    if(flip)
                    {
                        SDL_Rect renderQuad = { x + 21, y + 36 + runningOffset, animationClip.w, animationClip.h };
                        SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                    }
                    else
                    {
                        SDL_Rect renderQuad = { x + 25, y + 36 + runningOffset, animationClip.w, animationClip.h };
                        SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                    }
                    break;
                }
            case 3:
                {
                    if(flip)
                    {
                        SDL_Rect renderQuad = { x + 21, y + 35, animationClip.w, animationClip.h };
                        SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                    }
                    else
                    {
                        SDL_Rect renderQuad = { x + 25, y + 35, animationClip.w, animationClip.h };
                        SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                    }
                    break;
                }
            case 4:
            case 5:
                    if(flip)
                    {
                        SDL_Rect renderQuad = { x + 19, y + 35, animationClip.w, animationClip.h };
                        SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                    }
                    else
                    {
                        SDL_Rect renderQuad = { x + 25, y + 35, animationClip.w, animationClip.h };
                        SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                    }
                    break;
        }







}

void Texture::renderHair(SDL_Renderer* renderer, int x, int y, SDL_RendererFlip flip)
{
    if(animationTimer1.getTicks() / 50 > 10)
    {
        animationTimer1.stop();
    }

    if(animationTimer1.isStarted())
    {
        animationClip.x = 86 * (animationTimer1.getTicks() / 50);
        SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
        SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
    }
}

void Texture::renderRobot(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, bool leftActive, bool alive, int bodyPart)
{
    if(animationClip.x == 89 * 2 && !alive) //if it reaches the 3rd frame of the alive animation and its dead. start the falling animation
    {
        animationTimer2.start();
    }

    if(bodyPart == 1 && !alive && animationTimer2.isStarted()) //where the falling animation starts
    {
        if(animationTimer2.isStarted())
        {
            animationClip.y = 65;
            animationClip.w = 63;
            animationClip.h = 54;



            animationClip.x = 63 * (animationTimer2.getTicks() / 100);

            if(animationTimer2.getTicks() / 100 > 2)
            {
                animationClip.x = 63 * 1;
            }

            SDL_Rect renderQuad = { x + 35, y + 11, animationClip.w, animationClip.h };
            SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
        }
    }
    else //alive animation
    {
        animationTimer2.stop();

        switch( bodyPart )
        {
            case 1:
            {
                    animationClip.y = 0;
            }break;
            case 2:
            {
                    animationClip.y = 125;
            }break;
            case 3:
            {
                    animationClip.y = 190;
            } break;
        }
        animationClip.w = 89;
        animationClip.h = 65;

        if(leftActive)
        {
            if((animationTimer1.getTicks() / 100) > 11)
            {
                animationTimer1.start();
            }
            animationClip.x = 89 * (animationTimer1.getTicks() / 100);

            SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
            SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
        }
        else
        {
            SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
            SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
        }
        robotAnimation = animationClip.x;
    }

}

void Texture::renderSnowman(SDL_Renderer* renderer, int x, int y, bool alive, bool time2shoot, bool inFlight)  ///THIS WHOLE FUCKING THING IS A GIGANTICC SPIDDERWEB SPAGHHETTII CODE SPAGHETTI CODE MESS OCTOPUSS
{
        if(!time2shoot && alive){
        animationClip.x = 0;
        animationClip.y = 0;
        animationClip.w = 157;
        animationClip.h = 112;

        animationTimer6.stop();

        SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
        SDL_RenderCopy( renderer, texture, &animationClip, &renderQuad); return; }

        if(!alive)
        {
            if(!animationTimer6.isStarted() && !animationTimer6.isPaused()) //when snowman dies start playing his death animation
            {
                animationTimer6.start();
                animationClip.y = 0;
                animationClip.w = 157;
                animationClip.h = 112;
            }

            if(animationTimer6.getTicks() / 50 > 23) //somehow this makes it pause on the 25th frame of the death animation?
            {
                animationTimer6.pause();
            }

            animationClip.x = 157 * (animationTimer6.getTicks() / 50);

            animationBool = false;
            animationBool2 = false;
            animationTimer4.stop();
            animationTimer5.stop();


            SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
            SDL_RenderCopy( renderer, texture, &animationClip, &renderQuad);
            return;
        }
        else if(time2shoot)
        {
            animationTimer6.stop();

            if(!animationTimer4.isStarted() && !inFlight)
            {
                animationTimer4.start();
                animationBool = true;
                animationClip.y = 120;
                animationClip.w = 81;
                animationClip.h = 86;
            }

            animationClip.x = 81 * (animationTimer4.getTicks() / 50); ///OKAY WHICH ONE OF THESE HAS TO DO WITH THE RELOAD ANIMATION FUCK

            if(animationClip.x == 891)
            {
                animationTimer4.pause();
                animationBool2 = true;
            }
            else
            {
                animationBool2 = false;
            }

            if(!animationTimer5.isStarted() && animationClip.x == 891)
            {
                animationTimer5.start();
            }

            if(animationTimer5.getTicks() > 430)
            {
                animationTimer4.unpause();
                animationTimer5.stop();
            }

            animationBool3 = false;

            if(animationClip.x == 81 * 17)
            {
                animationTimer4.stop();
                animationBool = false;
                animationBool3 = true; //nose is loaded
            }


            SDL_Rect renderQuad = { x + 35, y + 26, animationClip.w, animationClip.h };
            SDL_RenderCopy( renderer, texture, &animationClip, &renderQuad);

            return;
        }
}

void Texture::renderSnowmanNose(SDL_Renderer* renderer, int x, int y, bool inPosition, int rotation, bool time2explode, bool flying, bool exploding)
{
    animationClip.x = 9000;


   if(inPosition && !animationTimer4.isStarted())
   {
        animationTimer4.start();
   }

   if(animationTimer4.isStarted() && !animationTimer5.isStarted())
   {
        animationClip.w = 11;
        animationClip.h = 24;

        animationClip.y = 0;
        animationClip.x = 0;
   }

   if(animationTimer4.getTicks() >= 700 && !animationTimer5.isStarted())
   {
       animationTimer5.start();
   }

   if(animationTimer5.isStarted())
    {
        animationClip.x = 11 * (animationTimer5.getTicks() / 50);
    }

    if(animationClip.x >= 143 && animationTimer5.isStarted())  //i should really comment useful things more
    {
        animationClip.w = 8;
        animationClip.h = 21;
        animationBool = true;
        animationClip.y = 26; //cool number
        animationTimer5.stop();
        animationTimer4.stop();
    }

    if(flying)
    {
        animationClip.w = 8;
        animationClip.y = 26;

        //printf("taaa");
        if((animationTimer1.getTicks() / 50) > 1)
        {
            animationTimer1.start();
            animationTimer5.stop();
            animationTimer4.stop();
        }
        animationClip.x = 8 * (animationTimer1.getTicks() / 50);
    }

    if(time2explode)
    {
        animationBool = false;
        animationTimer4.stop();
        animationTimer5.stop();
        animationTimer1.stop();
    }

    if(exploding)
    {
        animationClip.w = 30;
        animationClip.h = 21;
        animationClip.x = 56;
        animationClip.y = 26;
    }

    SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
    SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, rotation, 0, flip1);
}

void Texture::renderZipline(SDL_Renderer* renderer, int x, int y, bool docked, bool carrying, bool powertimerstarted)
{
    if(docked)
    {
        animationClip.y = 0;
        animationClip.w = 61;
        animationClip.h = 64;

        if(animationTimer1.getTicks() / 100 > 17)
        {
            animationTimer1.pause();
        }
        animationClip.x = 61 * (animationTimer1.getTicks() / 100);
        if(animationClip.x > 1098)
        {
            animationClip.x = 1098;
        }

        SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
        SDL_RenderCopy( renderer, texture, &animationClip, &renderQuad);
    }
    else if(carrying)
    {
        if(!powertimerstarted)
        {
            animationClip.y = 64;
            animationClip.w = 62;
            animationClip.h = 65;
            if(animationTimer1.getTicks() / 100 > 2)
            {
                animationTimer1.start();
            }
            animationClip.x = 62 * (animationTimer1.getTicks() / 100);

            SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
            SDL_RenderCopy( renderer, texture, &animationClip, &renderQuad);
        }
        else
        {
            animationClip.y = 64;
            animationClip.w = 62;
            animationClip.h = 65;
            if(animationTimer1.getTicks() / 100 > 2)
            {
                animationTimer1.start();
            }
            switch ( animationTimer1.getTicks() / 100 )
                    {
                        case 0: animationClip.x = 124;
                            break;                                                        //+1?
                        case 1: animationClip.x = 62;          ///i have no idea where this ^ comment came from
                            break;
                        case 2: animationClip.x = 0;
                            break;
                    }
            SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
            SDL_RenderCopy( renderer, texture, &animationClip, &renderQuad);
        }
    }
    else
    {
		if(animationTimer1.getTicks() / 100 > 17)
        {
            animationTimer1.start();
        }
        SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
        SDL_RenderCopy( renderer, texture, &animationClip, &renderQuad);
    }
}

void Texture::renderSemi(SDL_Renderer* renderer, int x, int y, SDL_RendererFlip flip, int shootticks, int reloadticks, int offset, int direction, int animation, bool holdthefuckup)
{
    if(animation == 8 || animation == 9 || animation == 10 || animation == 11 || animation == 12)
    {
      return;
    }

    if(animation == 7) ///double jump animation shit
    {
        if(shootticks > 0)
        {
                animationClip.w = 110;
                animationClip.h = 41;
            if(flip == SDL_FLIP_NONE)
            {
                animationClip.y = 218;
                animationClip.x = 110 * (shootticks / 50);
                if(shootticks / 50 > 2)
                {
                    animationClip.x = 220;
                }
                SDL_Rect renderQuad = { x - 10, y -21, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
            }
            else
            {
                animationClip.y = 218;
                animationClip.x = 110 * (shootticks / 50);
                if(shootticks / 50 > 2)
                {
                    animationClip.x = 220;
                }
                SDL_Rect renderQuad = { x - 32, y -21, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
            }
        }
        else
        {
            if(flip == SDL_FLIP_NONE)
            {
                animationClip.x = 0;
                animationClip.y = 177;
                animationClip.w = 110;
                animationClip.h = 41;

                SDL_Rect renderQuad = { x - 10, y - 21, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
            }
            else
            {
                animationClip.x = 0;
                animationClip.y = 177;
                animationClip.w = 110;
                animationClip.h = 41;

                SDL_Rect renderQuad = { x - 32, y - 21, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
            }
        }
        return;
    }

    if(holdthefuckup)      ///starts a timer when you raise your gun upward
    {
        if(!animationTimer1.isStarted())
        {
            animationTimer1.start();
        }
    }
    else
    {
        animationTimer1.stop();
    }

    if(!holdthefuckup)      ///starts a timer when you lower your gun from being up
    {
        if(!animationTimer2.isStarted())
        {
            animationTimer2.start();
        }
    }
    else
    {
        animationTimer2.stop();
    }



    if(direction == 1) ///GUN POINTING UP
    {
        animationClip.w = 19;
        animationClip.h = 107;

        if(flip == SDL_FLIP_NONE)
        {
            gunOffset2 = 0;
            if(animation > 1 && animation < 4)
            {
                    gunOffset2 += 2;
            }
        }
        else
        {
            gunOffset2 = -28;

            if(animation > 3)
            {
                    gunOffset2 += 1;
            }

            if(animation > 1 && animation < 4)
            {
                    gunOffset2 -= 2;
            }
        }

        if(shootticks > 0)
        {
            animationClip.y = 65;
            animationClip.x = 19 * (shootticks / 50);
            if(shootticks / 50 > 2)
            {
                animationClip.x = 38;
            }
            SDL_Rect renderQuad = { x - gunOffset2 + 9, y + offset - 95, animationClip.w, animationClip.h };
            SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
        }
        else
        {
            ///FUCK YOU
            ///fUCK YOU
            //hey calm down
            if(animationTimer1.getTicks() < 44)   //this one needs to be first lol
            {
                if(flip == SDL_FLIP_NONE)
                {
                    animationClip.w = 21;
                    animationClip.h = 108;
                    animationClip.y = 65;
                    animationClip.x = 58;
                    SDL_Rect renderQuad = { x - gunOffset2 + 11, y + offset - 95, animationClip.w, animationClip.h };
                    SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                }
                else
                {
                    animationClip.w = 21;
                    animationClip.h = 108;
                    animationClip.y = 65;
                    animationClip.x = 58;
                    SDL_Rect renderQuad = { x - gunOffset2 + 5, y + offset - 95, animationClip.w, animationClip.h };
                    SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
                }
            }
            else
            {
                animationClip.x = 0;
                animationClip.y = 65;
                SDL_Rect renderQuad = { x - gunOffset2 + 9, y + offset - 95, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
            }
        }
    }
    else ///GUN POINTING LEFT OR RIGHT
    {
        if(flip == SDL_FLIP_NONE)
        {
            gunOffset2 = 0;
        }
        else
        {
            gunOffset2 = 34;
        }

        if(shootticks > 0)
        {
            animationClip.w = 99;
            animationClip.h = 21;
            animationClip.y = 0;
            animationClip.x = 99 * (shootticks / 50);
            if(shootticks / 50 > 2)
            {
                animationClip.x = 198;
            }
            SDL_Rect renderQuad = { x - gunOffset2, y + offset, animationClip.w, animationClip.h };
            SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
        }
        else if(reloadticks > 0)
        {
            animationClip.w = 99;
            animationClip.h = 32;
            animationClip.y = 33;
            animationClip.x = 99 * (reloadticks / 90);
            if(reloadticks / 90 > 16)
            {
                animationClip.x = 99 * 16;
            }
            SDL_Rect renderQuad = { x - gunOffset2, y + offset + 2, animationClip.w, animationClip.h };
            SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
        }
        else if(animationTimer2.getTicks() < 152) ///melee animation
        {
            animationClip.w = 99;
            animationClip.h = 112;
            animationClip.y = 65;
            animationClip.x = 99 * (animationTimer2.getTicks() / 38) + 95;

            SDL_Rect renderQuad = { x - gunOffset2, y + offset - 90, animationClip.w, animationClip.h };
            SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
        }
        else
        {
            animationClip.w = 99;
            animationClip.h = 21;
            animationClip.x = 0;
            animationClip.y = 0;
            SDL_Rect renderQuad = { x - gunOffset2, y + offset, animationClip.w, animationClip.h };
            SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip);
        }
    }
}

void Texture::renderArmoredTruck(SDL_Renderer* renderer, SDL_Texture* orig,int x, int y, Uint32 triggerTimer, bool charging, bool reversing)
{
    if(triggerTimer > 0)
    {
        animationClip.w = 302;
        animationClip.h = 163;
        animationClip.y = 158;

        animationClip.x = 302 * (triggerTimer / 40);

        if(animationClip.x > 3925)
        {
            animationClip.x = 3926;
        }

        SDL_Rect renderQuad = { x -10, y-5 , animationClip.w, animationClip.h };
        SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip1);
    }
    else if(reversing)
    {
        animationClip.w = 287;
        animationClip.h = 158;
        animationClip.y = 0;


        animationClip.x = 287 * (animationTimer1.getTicks() / 80);

        if(animationClip.x > 861)
        {
            animationTimer1.start();
            animationClip.x = 861;
        }

        switch ( animationClip.x )
                    {
                            case 861: animationClip.x = 287 * 0;
                                break;                                                        //+1?
                            case 574: animationClip.x = 287 * 1;
                                break;
                            case 287: ; animationClip.x = 287 * 2;
                                break;
                            case 0: animationClip.x = 287 * 3;
                                break;
                    }

        SDL_Rect renderQuad = { x, y , animationClip.w, animationClip.h };
        SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip1);
    }
    else if(charging)
    {
        animationClip.w = 287;
        animationClip.h = 158;
        animationClip.y = 0;

        animationClip.x = 287 * (animationTimer1.getTicks() / 80);

        if(animationClip.x > 861  )
        {
            animationClip.x = 861;
            animationTimer1.start();
        }

        SDL_Rect renderQuad = { x, y , animationClip.w, animationClip.h };
        SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip1);
    }
    else
    {
        animationClip.w = 287;
        animationClip.h = 158;
        animationClip.y = 0;
        animationClip.x = 0;

        SDL_Rect renderQuad = { x, y , animationClip.w, animationClip.h };
        SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip1);
    }


}

void Texture::renderArmoredTruckCracks(SDL_Renderer* renderer, SDL_Texture* orig,int x, int y, char health)
{
    switch(health)
    {
            case 0: animationClip.y = 50;
                    break;
            case 1: animationClip.y = 25;
                    break;                                                        //+1?
            case 2: animationClip.y = 0;
                    break;
            case 3: animationClip.y = 75;
                    break;
    }
    SDL_Rect renderQuad = { x + 98, y + 43 , animationClip.w, animationClip.h };
    SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip1);
}

void Texture::renderArmoredTruckDevil(SDL_Renderer* renderer, SDL_Texture* orig,int x, int y)
{
    SDL_Rect renderQuad = { x + 130, y + 51, animationClip.w, animationClip.h };
    SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip1);
}

void Texture::renderTargets(SDL_Renderer* renderer, int x, int y, int color, bool alive)
{
    if(alive)
    {
        switch(color)
        {
            case 1:
                animationClip.w = 50;
                animationClip.h = 50;
                animationClip.y = 22;
                animationClip.x = 30;
            break;
            case 2:
                animationClip.w = 30;
                animationClip.h = 30;
                animationClip.y = 32;
                animationClip.x = 0;
            break;
            case 3:
                animationClip.w = 13;
                animationClip.h = 13;
                animationClip.y = 19;
                animationClip.x = 0;
            break;
        }
    }
    else
    {
        switch(color)
        {
            case 1:
                animationClip.w = 20;
                animationClip.h = 22;
                animationClip.y = 0;
                animationClip.x = 33;
            break;
            case 2:
                animationClip.w = 20;
                animationClip.h = 22;
                animationClip.y = 0;
                animationClip.x = 13;
            break;
            case 3:
                animationClip.w = 13;
                animationClip.h = 19;
                animationClip.y = 0;
                animationClip.x = 0;
            break;
        }
    }



    SDL_Rect renderQuad = { x , y, animationClip.w, animationClip.h };
    SDL_RenderCopyEx( renderer, texture, &animationClip, &renderQuad, 0, 0, flip1);
}

void Texture::renderSharkRobots(SDL_Renderer* renderer, SDL_Texture* orig,int x, int y, int bsitfTicks)
{
    if(bsitfTicks < 1)
    {
        animationTimer2.stop();

        animationClip.x = 82 * (animationTimer1.getTicks() / 100);
        if(animationClip.x > 246)
        {
            animationTimer1.start();
            animationClip.x = 246;
        }

        animationClip.w = 82;
        animationClip.h = 48;
        animationClip.y = 0;

        SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
        SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
    }
    else
    {
        animationClip.w = 82;
        animationClip.h = 47;
        animationClip.y = 48;

        if(animationTimer2.getTicks() < 1)
        {
            animationTimer2.start();
        }

        animationClip.x = 82 * (animationTimer2.getTicks() / 150);

        if(animationClip.x == 574)
        {
            animationClip.x = 164;
        }

        if(animationClip.x >= 738)
        {
            animationClip.x = 246;
        }

        SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
        SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
    }
}

void Texture::renderKultentas(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, bool dead, int deadTicks)
{
    if(dead)
    {
        animationClip.w = 166;
        animationClip.h = 156;
        animationClip.y = 329;

        animationClip.x = 166 * (deadTicks / 30);

        if(animationClip.x > 1992)
        {
            animationClip.x = 1992;
        }

        SDL_Rect renderQuad = { x -75, y -5, animationClip.w, animationClip.h };
        SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
    }
    else
    {

        animationClip.x = 107 * (animationTimer1.getTicks() / 125);

        if(animationClip.x > 963)
        {
            animationTimer1.start();
            animationClip.x = 963;
        }

        animationClip.w = 107;
        animationClip.h = 133;
        animationClip.y = 0;

        SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
        SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
    }

}

void Texture::renderKultentaEvilArm(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, int xclip, bool dead,bool why)
{
    if(!dead)
    {
        animationClip.x = xclip;
        if(why)
        {
            animationClip.y = 133;
            animationClip.w = 43;
            animationClip.h = 124;

            SDL_Rect renderQuad = { x -23, y - 5, animationClip.w, animationClip.h };
            SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
        }
        else
        {
            animationClip.y = 257;
            animationClip.w = 51;
            animationClip.h = 72;

            SDL_Rect renderQuad = { x -48, y - 16, animationClip.w, animationClip.h };
            SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
        }
    }
}

void Texture::renderTNTgent(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, int animation, int hp)
{
    if(hp < 1)
    {
        animationTimer1.pause();
        animationTimer2.pause();
        animationTimer3.pause();
        animationTimer7.pause();
    }

    switch(animation)
        {
            case 0: ///standing
                animationClip.y = 0;
                animationClip.w = 57;
                animationClip.h = 81;
                animationClip.x = 0;
                animationTimer1.stop();
                animationTimer2.stop();
                animationTimer7.stop();

                robotAnimation = 0;

            break;
            case 1: ///jumping
                animationBool = false;
                animationClip.y = 81;
                animationClip.w = 57;
                animationClip.h = 83;
                animationTimer2.stop();

                robotAnimation = 0;

                if(!animationTimer1.isStarted() && hp > 0)
                {
                    animationTimer1.start();
                }

                animationClip.x = 57 * (animationTimer1.getTicks() / 80);

                if(animationClip.x > 113)
                {
                    animationClip.x = 114;
                }
            break;
            case 2: ///crouching
                animationClip.y = 164;
                animationClip.w = 110;
                animationClip.h = 106;
                animationTimer1.stop();

                if(!animationTimer2.isStarted() && hp > 0)
                {
                    animationTimer2.start();
                }

                animationClip.x = 110 * (animationTimer2.getTicks() / 50);

                if(animationClip.x > 110)
                {
                    animationClip.x = 220;
                    animationBool = true; ///do the uncrouch frame
                }

                if(animationClip.x == 0)
                {
                    robotAnimation = 1;
                }
                else if(animationClip.x == 110)
                {
                    robotAnimation = 2;
                }
                else
                {
                    robotAnimation = 0;
                }

            break;
            case 5: ///tossing
            case 6:
            animationClip.y = 317;
            animationClip.w = 141;
            animationClip.h = 130;
            animationTimer1.stop();
            animationTimer2.stop();
            animationTimer3.stop();

            if(!animationTimer7.isStarted() && hp > 0)
            {
                animationTimer7.start();
            }

            animationClip.x = 141 * (animationTimer7.getTicks() / 70);

            if(animationClip.x > 1550 && animationClip.x < 2679)
            {
                animationInt = 2;
            }

            if(animationClip.x > 1550 && animationClip.x < 2679)
            {
                animationInt = 2;
            }
            else if(animationClip.x < 3384)
            {
                animationInt = 1;
            }

            if(animationClip.x == 3384)
            {
                animationInt = 5;
            }

            if(animationClip.x > 3666)
            {
                animationClip.x = 3666;
            }
            break;
        }

        if(animationBool && animation == 0) ///bullshit for the 1 crouch frame i think
        { //this is all so bizarrely and horribly programmed holy shit
            animationClip.y = 164;
            animationClip.w = 110;
            animationClip.h = 106;
            animationClip.x = 0;

            if(!animationTimer3.isStarted() && hp > 0)
            {
                animationTimer3.start();
            }

            if(animationTimer3.getTicks() > 50)
            {
                animationBool = false;
            }

            SDL_Rect renderQuad = { x - 47, y - 24, animationClip.w, animationClip.h };
            SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);

            if(animationClip.x == 0)
                {
                    robotAnimation = 1;
                }
                else if(animationClip.x == 110)
                {
                    robotAnimation = 2;
                }
                else
                {
                    robotAnimation = 0;
                }
        }else
        {
            animationTimer3.stop();

            if(animation == 2)
            {
                SDL_Rect renderQuad = { x - 43, y - 69, animationClip.w, animationClip.h };
                SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);  ///WHY THE FUCK CANT I PUT THESE IN THE SWITCH CASE??? FUUUUCK C++
            }
            else if(animation == 5 || animation == 6)
            {
                SDL_Rect renderQuad = { x - 35, y - 46, animationClip.w, animationClip.h };
                SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
            }
            else
            {
                SDL_Rect renderQuad = { x - 5, y, animationClip.w, animationClip.h };
                SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
            }
        }
}

void Texture::renderTNTgentHead(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, int animation, int speakFrame, int headX, int headY)
{
    if(animation == 6 || animation == 7) ///play explosion animation
    {
        /*animationClip.y = 447;
        animationClip.w = 50;
        animationClip.h = 50;

        if(!animationTimer4.isStarted())
        {
            animationTimer4.start();
        }

        animationClip.x = 50 * (animationTimer4.getTicks() / 400);

        SDL_Rect renderQuad = { headX - 16, headY - 5, animationClip.w, animationClip.h };
        SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);*/
    }
    else
    {
    animationTimer4.stop();
    animationClip.y = 270;
    animationClip.w = 19;
    animationClip.h = 47;

        switch(speakFrame)
            {
                case 0: animationClip.x = 0;  ///IDLE FRAME
                break;
                case 1: animationClip.x = 19; ///FRAME 000 OF 038
                break;
                case 2: animationClip.x = 38;
                break;
                case 3: animationClip.x = 19;
                break;
                case 4: animationClip.x = 19;
                break;
                case 5: animationClip.x = 38;
                break;
                case 6: animationClip.x = 19;
                break;
                case 7: animationClip.x = 19;
                break;
                case 8: animationClip.x = 0;
                break;
                case 9: animationClip.x = 19;
                break;
                case 10: animationClip.x = 38;
                break;
                case 11: animationClip.x = 19;
                break;
                case 12: animationClip.x = 38;
                break;
                case 13: animationClip.x = 38;
                break;
                case 14: animationClip.x = 19;
                break;
                case 15: animationClip.x = 19;
                break;
                case 16: animationClip.x = 19;
                break;
                case 17: animationClip.x = 19;
                break;
                case 18: animationClip.x = 0;
                break;
                case 19: animationClip.x = 38;
                break;
                case 20: animationClip.x = 0;
                break;
                case 21:animationClip.x = 38;
                break;
                case 22: animationClip.x = 19;
                break;
                case 23: animationClip.x = 38;
                break;
                case 24: animationClip.x = 38;
                break;
                case 25: animationClip.x = 19;
                break;
                case 26: animationClip.x = 19;
                break;
                case 27: animationClip.x = 0;
                break;
                case 28: animationClip.x = 38;
                break;
                case 29: animationClip.x = 0;
                break;
                case 30:animationClip.x = 38;
                break;
                case 31: animationClip.x = 19;
                break;
                case 32: animationClip.x = 38;
                break;
                case 33: animationClip.x = 38;
                break;
                case 34: animationClip.x = 0;
                break;
                case 35: animationClip.x = 0;
                break;
                case 36: animationClip.x = 0;
                break;
                case 37: animationClip.x = 0;
                break;
                case 38: animationClip.x = 0;
                break;
                case 39: animationClip.x = 0;
                break;
            }



    switch(animation)
        {
            case 0: ///standing
                gunOffset = -25;
                gunOffset2 = 47;
            break;
            case 1: ///jumping
                gunOffset = -25;
                gunOffset2 = 47;
            break;
            case 2: ///crouching
                if(robotAnimation == 1) ///robot animation is shared witht he normal tnt gent render
                {
                    gunOffset = 7;
                    gunOffset2 = 69;
                }
                else if(robotAnimation == 2)
                {
                    gunOffset = 19;
                    gunOffset2 = 40;
                }
                else
                {
                    gunOffset = 20;
                    gunOffset2 = 37;
                }
                break;
        }
    if(animationInt == 5) ///tossing animation for the head
    {
        animationClip.x = 145;
        animationClip.y = 275;
        animationClip.w = 42;
        animationClip.h = 37;

        SDL_Rect renderQuad = { headX, headY, animationClip.w, animationClip.h };
        SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
    }
    else
    {
        if(animationBool && robotAnimation == 1)
        {
            gunOffset = 11;
            gunOffset2 = 24;
        }

        if(animation != 5)
        {
                ///im using gunoffset just to give it an offset fjasdfijgag
            SDL_Rect renderQuad = { x - gunOffset, y - gunOffset2, animationClip.w, animationClip.h };
            SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);  ///WHY THE FUCK CANT I PUT THESE IN THE SWITCH CASE??? FUUUUCK C++
        }
    }
    }

}

void Texture::renderTNTgentExplosion(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y)
{
    animationClip.w = 97;
    animationClip.h = 93;
    animationClip.y = 28;
    animationClip.x = 574;

    SDL_Rect renderQuad = { x + 10, y, animationClip.w, animationClip.h };
    SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
}

void Texture::renderWermbl(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, int state, int footSpeed, SDL_RendererFlip flip)
{
    switch(state)
    {
        case 0:
                    animationClip.w = 98;
                    animationClip.h = 67;
                    animationClip.y = 0;

                    animationClip.x = 98 * (animationTimer1.getTicks() / 80);
                    if(animationClip.x > 588)
                    {
                        if(!animationTimer1.isPaused())
                        {
                            animationTimer1.start();
                        }
                        animationClip.x = 588;
                    }

                    switch(animationClip.x)
                    {
                        case 0: gunOffset = 2;
                                gunOffset2 = 2;
                        break;
                        case 98: gunOffset = 1;
                                 gunOffset2 = 1;
                        break;
                        case 196: gunOffset = 3;
                                  gunOffset2 = 5;
                        break;
                        case 294: gunOffset = 2;
                                  gunOffset2 = 2;
                        break;
                        case 392: gunOffset = 0;
                                  gunOffset2 = 0;
                        break;
                        case 490: gunOffset = 2;
                                  gunOffset2 = 2;
                        break;
                        case 588: gunOffset = 3;
                                  gunOffset2 = 3;
                        break;
                    }
        break;
        case 10:
                    if(!animationTimer5.isStarted())
                    {
                        animationTimer5.start();
                    }
                    animationClip.w = 297;
                    animationClip.h = 134;
                    animationClip.y = 168;

                    animationClip.x = 297 * (animationTimer5.getTicks() / 300);

                    if(animationClip.x > 0)
                    {
                        animationClip.w = 297;
                        animationClip.h = 134;
                        animationClip.y = 168;
                        animationClip.x = 297;
                    }
        break;
        case 20:
        case 21:
        case 22:
        case 23:
        case 24:
        case 25:
                gunOffset = 0;
                gunOffset2 = 2;

                animationClip.w = 98;
                animationClip.h = 67;
                animationClip.y = 104;

                animationClip.x = 98 * (animationTimer2.getTicks() / footSpeed);
                if(animationClip.x > 686)
                {
                    animationTimer2.start();
                    animationClip.x = 686;
                }
        break;
        case 30:
        case 31:
        case 32:
        case 33:
        case 34:
        case 35:
        case 36:
                gunOffset = 0;
                gunOffset2 = 2;

                animationClip.w = 98;
                animationClip.h = 67;
                animationClip.y = 104;

                animationClip.x = 98 * (animationTimer2.getTicks() / 40);
                if(animationClip.x > 686)
                {
                    animationTimer2.start();
                    animationClip.x = 686;
                }
        break;
        case 37:
                animationTimer2.pause();

                gunOffset = 0;
                gunOffset2 = 2;

                animationClip.w = 98;
                animationClip.h = 67;
                animationClip.y = 104;

                animationClip.x = 98 * (animationTimer2.getTicks() / 40);
        break;
    }

    if(state == 10)
    {
        SDL_Rect renderQuad = { x - 100, y -70, animationClip.w, animationClip.h };
        SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip);
    }
    else
    {
        SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
        SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip);
    }

}

void Texture::renderWermblFace(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, int state, SDL_RendererFlip flip)
{
    animationClip.w = 16;
    animationClip.h = 18;
    animationClip.y = 67;
    animationClip.x = 0;

    if(animationTimer4.isStarted())
    {
        animationClip.x = 16 * (animationTimer4.getTicks() / 50) + 16;
        if(animationClip.x > 80)
        {
            animationClip.x = 80;
        }
    }

    if(state > 33 && state < 37)
    {
        animationClip.w = 16;
        animationClip.h = 18;
        animationClip.y = 67;

        if(!animationTimer5.isStarted())
        {
            animationTimer5.start();
        }

        animationClip.x = 16 * (animationTimer5.getTicks() / 40);

        if(animationClip.x > 16)
        {
            animationClip.x = 32;
        }

        switch(animationClip.x)
        {
            case 0: animationClip.x = 64;
            break;
            case 16: animationClip.x = 48;
            break;
        }
    }

    if(state == 37)
    {
        animationClip.w = 16;
        animationClip.h = 18;
        animationClip.y = 67;
        animationClip.x = 80;
    }

    SDL_Rect renderQuad = { x, y + gunOffset, animationClip.w, animationClip.h };
    SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip);
}

void Texture::remderWermblGacha(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, int type)
{
    animationClip.w = 18;
    animationClip.h = 19;
    animationClip.y = 85;

    switch(type)
    {
        case 1: animationClip.x = 0;
        break;
        case 2: animationClip.x = 18;
        break;
        case 3: animationClip.x = 36;
        break;
    }

    SDL_Rect renderQuad = { x, y + gunOffset2, animationClip.w, animationClip.h };
    SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
}

void Texture::renderGarbageGaurdian(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, int animation)
{
    animationClip.w = 171;
    animationClip.h = 145;


    switch(animation)
    {
        case 1: animationClip.x = 0;
                animationClip.y = 0;
        break;
        case 2: animationClip.x = 171;
                animationClip.y = 0;
        break;
        case 3: animationClip.x = 342;
                animationClip.y = 0;
        break;
        case 4: animationClip.x = 513;
                animationClip.y = 0;
        break;
        case 5: animationClip.x = 684;
                animationClip.y = 0;
        break;
        case 6: animationClip.x = 855;
                animationClip.y = 0;
        break;
    }


    SDL_Rect renderQuad = { x - 95, y -72, animationClip.w, animationClip.h };
    SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
}

void Texture::renderGarbageGaurdianSword(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, bool flying, bool grounded)
{
    if(flying)
    {
        animationClip.w = 63;
        animationClip.h = 44;
        animationClip.x = 0;
        animationClip.y = 145;

        SDL_Rect renderQuad = { x, y - 20, animationClip.w, animationClip.h };
        SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
    }
    else if(grounded)
    {
        animationClip.w = 48;
        animationClip.h = 59;
        animationClip.x = 0;
        animationClip.y = 189;

        SDL_Rect renderQuad = { x, y -33, animationClip.w, animationClip.h };
        SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
    }
}

void Texture::renderGarbageGaurdianShield(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, bool flying)
{
    if(flying)
    {
        animationClip.w = 49;
        animationClip.h = 17;
        animationClip.y = 248;

        animationClip.x = 49 * (animationTimer3.getTicks() / 40);

        if(animationClip.x > 294)
        {
            animationClip.x = 294;
            animationTimer3.start();
        }

        SDL_Rect renderQuad = { x, y - 5, animationClip.w, animationClip.h };
        SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
    }
}

void Texture::renderKongaLee(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, int angle, int speed, bool spit, int type, bool sploded)
{
    if(type == 0) ///I CANT USE SWITCH CASE CUZ SDL IS DUMB
    {
        if(!spit)
        {
            animationClip.w = 54;
            animationClip.h = 54;
            animationClip.y = 0;
            animationTimer4.stop();

            animationClip.x = 54 * (animationTimer1.getTicks() / speed);

            if(animationClip.x > 216)
            {
                animationClip.x = 216;
                animationTimer1.start();
            }

            SDL_Rect renderQuad = { x - 8, y - 8, animationClip.w, animationClip.h };
            SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, angle, 0, flip1);
        }
        else
        {
            animationClip.w = 63;
            animationClip.h = 53;
            animationClip.y = 54;

            if(!animationTimer4.isStarted())
            {
                animationTimer4.start();
            }

            animationClip.x = 63 * (animationTimer4.getTicks() / 50);

            if(animationClip.x > 315)
            {
                animationClip.x = 315;
            }

            SDL_Rect renderQuad = { x - 8, y - 8, animationClip.w, animationClip.h };
            SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, angle, 0, flip1);
        }
    }
    else if(type == 1)
    {
        animationClip.w = 54;
        animationClip.h = 54;
        animationClip.y = 162;
        animationTimer4.stop();

        animationClip.x = 54 * (animationTimer1.getTicks() / speed);

        if(animationClip.x > 216)
        {
            animationClip.x = 216;
            animationTimer1.start();
        }

        SDL_Rect renderQuad = { x - 8, y - 8, animationClip.w, animationClip.h };
        SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, angle, 0, flip1);
    }
    else if(type == 2)
    {
        if(!sploded)
        {
            animationClip.w = 54;
            animationClip.h = 54;
            animationClip.y = 108;
            animationTimer4.stop();

            animationClip.x = 54 * (animationTimer1.getTicks() / speed);

            if(animationClip.x > 216)
            {
                animationClip.x = 216;
                animationTimer1.start();
            }

            SDL_Rect renderQuad = { x - 8, y - 8, animationClip.w, animationClip.h };
            SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, angle, 0, flip1);
        }
        else
        {
            animationClip.w = 54;
            animationClip.h = 54;
            animationClip.y = 108;
            animationClip.x = 270;

            SDL_Rect renderQuad = { x - 8, y - 8, animationClip.w, animationClip.h };
            SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, angle, 0, flip1);
        }
    }
}

void Texture::renderKongaLeeExplosion(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, bool sploding)
{
    if(sploding)
    {
        animationClip.w = 320;
        animationClip.h = 320;
        animationClip.y = 216;

        animationClip.x = 320 * (animationTimer2.getTicks() / 150);

        if(animationClip.x > 320)
        {
            animationClip.x = 320;
            animationTimer2.start();
        }

        SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
        SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
    }
}

void Texture::renderSorse(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, SDL_RendererFlip flip, int summonticks, int attackticks, bool alive, bool collapsing)
{
    if(alive)
    {
        if(summonticks > 0)
        {
            animationTimer1.start();

            animationClip.w = 103;
            animationClip.h = 113;
            animationClip.y = 113;

            animationClip.x = 103 * (summonticks / 40);

            if(animationClip.x > 4326)
            {
                animationClip.x = 4326;
            }

            if(flip == SDL_FLIP_HORIZONTAL)
            {
                SDL_Rect renderQuad = { x - 21, y, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip);
            }
            else
            {
                SDL_Rect renderQuad = { x + 12, y, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip);
            }
        }
        else if(attackticks > 0)
        {
            animationTimer1.start();

            animationClip.w = 124;
            animationClip.h = 216;
            animationClip.y = 226;

            animationClip.x = 124 * (attackticks / 40);

            if(animationClip.x > 4712)
            {
                animationClip.x = 4712;
            }

            if(flip == SDL_FLIP_HORIZONTAL)
            {
                SDL_Rect renderQuad = { x - 16, y - 0, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip);
            }
            else
            {
                SDL_Rect renderQuad = { x  - 15, y - 0, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip);
            }
        }
        else //idle
        {
            animationClip.w = 94;
            animationClip.h = 113;
            animationClip.y = 0;

            animationClip.x = 94 * (animationTimer1.getTicks() / 50);

            if(animationClip.x > 1410)
            {
                animationClip.x = 1410;
                animationTimer1.start();

                animationInt++;
                if(animationInt > 2) {animationInt = 0;}
            }

            if(animationClip.x > 563 && animationClip.x < 1222)
            {
                animationBool = false;
            }
            else
            {
                animationBool =  true;
            }

            if(flip == SDL_FLIP_HORIZONTAL)
            {
                SDL_Rect renderQuad = { x - 12, y, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip);
            }
            else
            {
                SDL_Rect renderQuad = { x + 12, y, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip);
            }
        }
    }
    else
    {
        if(collapsing)
        {
            animationTimer3.stop();

            animationClip.w = 67;
            animationClip.h = 128;
            animationClip.y = 703;
            animationClip.x = 0;

             if(flip == SDL_FLIP_HORIZONTAL)
            {
                SDL_Rect renderQuad = { x + 16, y - 23, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip);
            }
            else
            {
                SDL_Rect renderQuad = { x + 12, y - 23, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip);
            }
        }
        else
        {
            if(!animationTimer3.isStarted())
            {
                animationTimer3.start();
            }

            animationClip.w = 67;
            animationClip.h = 128;
            animationClip.y = 703;

            animationClip.x = 67 * (animationTimer3.getTicks() / 55) + 67;

            if(animationClip.x > 402)
            {
                animationClip.x = 402;
            }

            if(flip == SDL_FLIP_HORIZONTAL)
            {
                SDL_Rect renderQuad = { x + 16, y - 23, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip);
            }
            else
            {
                SDL_Rect renderQuad = { x + 12, y - 23, animationClip.w, animationClip.h };
                SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip);
            }
        }
    }

}

void Texture::renderSorseBlackHole(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, bool flying)
{
    if(flying)
    {
        animationClip.w = 30;
        animationClip.h = 30;
        animationClip.x = 2035;
        animationClip.y = 512;

        /*animationClip.x = 94 * (animationTimer1.getTicks() / 50);

        if(animationClip.x > 1410)
        {
            animationClip.x = 1410;
            animationTimer1.start();
        }*/

        SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
        SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
    }
    else
    {
        animationClip.w = 280;
        animationClip.h = 140;
        animationClip.y = 564;

        animationClip.x = 280 * (animationTimer2.getTicks() / 90);

        if(animationClip.x > 560)
        {
            animationClip.x = 560;
            animationTimer2.start();
        }

        SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
        SDL_RenderCopy( renderer, orig, &animationClip, &renderQuad);
    }
}

void Texture::renderSorseWeakpoint(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, SDL_RendererFlip flip, bool alive)
{
    if(alive)
    {
        animationClip.w = 94;
        animationClip.h = 113;
        animationClip.y = 0;

        switch(animationInt)
        {
            case 0: animationClip.x = 1700;
            break;
            case 1: animationClip.x = 1794;
            break;
            case 2: animationClip.x = 1888;
            break;
        }


        if(flip == SDL_FLIP_HORIZONTAL)
        {
            SDL_Rect renderQuad = { x - 12, y, animationClip.w, animationClip.h };
            SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip);
        }
        else
        {
            SDL_Rect renderQuad = { x + 12, y, animationClip.w, animationClip.h };
            SDL_RenderCopyEx( renderer, orig, &animationClip, &renderQuad, 0, 0, flip);
        }
    }
}

void Texture::renderMenu(SDL_Renderer* renderer, int x, int y, int page)
{
        animationClip.y = 0;
        animationClip.w = 176;
        animationClip.h = 338;

        switch(page)
        {
            case 0: animationClip.x = 0;
            break;
            case 1: animationClip.x = 176;
            break;
            case 2: animationClip.x = 352;
            break;
        }

        SDL_Rect renderQuad = { x, y, animationClip.w, animationClip.h };
        SDL_RenderCopy( renderer, texture, &animationClip, &renderQuad);
}

void Texture::setAsRenderTarget( SDL_Renderer* renderer )
{
	//Make self render target
	SDL_SetRenderTarget( renderer, texture );
}

int Texture::getWidth()
{
	return width;
}

int Texture::getHeight()
{
	return height;
}
