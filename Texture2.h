#ifndef TEXTURE2_H_INCLUDED
#define TEXTURE2_H_INCLUDED

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "Timer.h"

class Texture2
{
	public:
		Texture2();
		Texture2(int w, int h);
		~Texture2();

		bool loadFile( std::string path, SDL_Renderer* renderer );

		bool loadFromRenderedText( std::string textureText, SDL_Color textColor, TTF_Font* font, SDL_Renderer* renderer );

		void free();

		void setColor( short red, short green, short blue );

		void setBlendMode( SDL_BlendMode blending );

		void setAlpha( Uint8 alpha );

		void render( SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
		void render2( SDL_Renderer* renderer, int x, int y, int w, int h, int ax, int ay, SDL_Rect* clip = NULL,  double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
		void setAsRenderTarget( SDL_Renderer* renderer );

		int getWidth();
		int getHeight();

        //private:
		SDL_Texture* texture;

		int width;
		int height;

		//retardery
        SDL_Rect animationClip;

		void renderPlayer(int animation, SDL_Renderer* renderer, int x, int y, SDL_RendererFlip flip);
		SDL_Rect playerClip;

		SDL_RendererFlip flip1;
};



#endif // TEXTURE_H_INCLUDED
