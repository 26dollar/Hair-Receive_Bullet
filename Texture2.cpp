#include "Texture2.h"

Texture2::Texture2()
{
	//Initialize
	texture = NULL;
	width = 0;
	height = 0;

	animationClip = {0, 0, 0, 0};

    flip1 =  SDL_FLIP_NONE;
}

Texture2::Texture2(int w, int h)
{
	//Initialize
	texture = NULL;
	width = 0;
	height = 0;


	animationClip = {0, 0, w, h};
    flip1 =  SDL_FLIP_NONE;
}


Texture2::~Texture2()
{
	//Deallocate
	free();
}

bool Texture2::loadFile( std::string path, SDL_Renderer* renderer )
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

bool Texture2::loadFromRenderedText( std::string textureText, SDL_Color textColor, TTF_Font* font, SDL_Renderer* renderer )
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

void Texture2::free()
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

void Texture2::setColor( short red, short green, short blue )
{
	SDL_SetTextureColorMod( texture, red, green, blue );
}

void Texture2::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( texture, blending );
}

void Texture2::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( texture, alpha );
}

void Texture2::render( SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
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

void Texture2::render2( SDL_Renderer* renderer, int x, int y, int wee, int hee, int ax, int ay, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
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

void Texture2::setAsRenderTarget( SDL_Renderer* renderer )
{
	//Make self render target
	SDL_SetRenderTarget( renderer, texture );
}

int Texture2::getWidth()
{
	return width;
}

int Texture2::getHeight()
{
	return height;
}
