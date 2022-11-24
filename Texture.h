#ifndef TEXTURE_H_INCLUDED
#define TEXTURE_H_INCLUDED

#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "Timer.h"

class Texture ///make different class thats the exact same as this one but with less timers
{
	public:
		Texture();
		Texture(int w, int h);
		~Texture();

		bool loadFile( std::string path, SDL_Renderer* renderer );

		bool loadFromRenderedText( std::string textureText, SDL_Color textColor, TTF_Font* font, SDL_Renderer* renderer );

		void free();

		void setColor( short red, short green, short blue );

		void setBlendMode( SDL_BlendMode blending );

		void setAlpha( Uint8 alpha );

		void render( SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
		void render2( SDL_Renderer* renderer, int x, int y, int w, int h, int ax, int ay, SDL_Rect* clip = NULL,  double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
		void render3( SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);
		void setAsRenderTarget( SDL_Renderer* renderer );
		void defaultAnimate( SDL_Renderer* renderer, int x, int y, int w, int h, int fps, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

		int getWidth();
		int getHeight();

        //private:
		SDL_Texture* texture;

		int width;
		int height;

		//retardery
        SDL_Rect animationClip;
        SDL_Rect test;

		void renderPlayer(int animation, SDL_Renderer* renderer, int x, int y, SDL_RendererFlip flip, Timer bounceTimer, int bounceDirection);
		SDL_Rect playerClip;
		Timer animationTimer1;
		Timer animationTimer2;
		Timer animationTimer3;
		Timer animationTimer7;

		Timer animationTimer4; //this one isnt retarded and starts up automatically, dunoo why i ever did that shit fuck fuck fuck ///okay i figured out why i did it that way and it makes more sense
		Timer animationTimer5;

		Timer animationTimer6;
		Timer animationTimer8;



		Timer animationTimer9;
		Timer animationTimer10;


		int gunOffset; //wtf are these ints
		int gunOffset2;
		int robotAnimation;

		bool animationBool;
		bool animationBool2;
		bool animationBool3;
		int animationInt;

		//SDL_Point structure = {27, 27}; ///need this shit for rotating stuf
		//const SDL_Point* applepie = &structure;

		void renderAmmo(int clip, SDL_Renderer* renderer, int x, int y);
		void renderAmmo2(int clip, SDL_Renderer* renderer, int x, int y);
		void renderHair(SDL_Renderer* renderer, int x, int y, SDL_RendererFlip flip);

		void renderSemi(SDL_Renderer* renderer, int x, int y, SDL_RendererFlip flip, int shootticks, int reloadticks, int offset, int direction, int animation, bool holdthefuckup);

		void renderFace(SDL_Renderer* renderer, int x, int y, SDL_RendererFlip flip, bool succing, int idleOffset, int animation, int runningOffset);

		void renderZipline(SDL_Renderer* renderer, int x, int y, bool docked, bool carrying, bool powertimerstarted);

		void renderRobot(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, bool leftActive, bool alive, int bodyPart);

		void renderSnowman(SDL_Renderer* renderer, int x, int y, bool alive, bool time2shoot, bool inFlight);
		///200 combo!
		void renderSnowmanNose(SDL_Renderer* renderer, int x, int y, bool inPosition, int rotation, bool time2explode, bool flying, bool exploding);

		void renderArmoredTruck(SDL_Renderer* renderer, SDL_Texture* orig,int x, int y, Uint32 triggerTimer, bool charging, bool reversing);

		void renderArmoredTruckCracks(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, char health);

		void renderArmoredTruckDevil(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y);

		void renderTargets(SDL_Renderer* renderer, int x, int y, int color, bool alive);

		void renderSharkRobots(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, int bsitfTicks);

		void renderKultentas(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, bool dead, int deadTicks);
		void renderKultentaEvilArm(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, int xclip, bool dead,bool why);

		void renderTNTgent(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, int animation, int hp);
		void renderTNTgentHead(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, int animation, int speakFrame, int headX, int headY);
		void renderTNTgentExplosion(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y);

		void renderWermbl(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, int state, int footSpeed, SDL_RendererFlip flip);
		void renderWermblFace(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, int state, SDL_RendererFlip flip);
		void remderWermblGacha(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, int type);

		void renderGarbageGaurdian(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, int animation);
		void renderGarbageGaurdianSword(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, bool flying, bool grounded);
		void renderGarbageGaurdianShield(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, bool flying);

		void renderKongaLee(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, int angle, int speed, bool spit, int type, bool sploded);
		void renderKongaLeeExplosion(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, bool sploding);

		void renderSorse(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, SDL_RendererFlip flip, int summonticks, int attackticks, bool alive, bool collapsing);
		void renderSorseBlackHole(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, bool flying);
		void renderSorseWeakpoint(SDL_Renderer* renderer, SDL_Texture* orig, int x, int y, SDL_RendererFlip flip, bool alive);

		void renderMenu(SDL_Renderer* renderer, int x, int y, int page);

		SDL_RendererFlip flip1;
};



#endif // TEXTURE_H_INCLUDED
