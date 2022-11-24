#ifndef SNOWMAN_H_INCLUDED
#define SNOWMAN_H_INCLUDED

#include <SDL.h>
#include "Timer.h"
#include <stdio.h>

class Snowman
{
    public:                        ///my body lies over the ocean, my body lies over the sea, i'm no good at astral projection, so bring back my body to me
		Snowman(int x, int y);
		~Snowman();

		short int health; //fuckin pointless
		bool onFlatSurface;

		void move(bool animationOn, bool time2fly, float playerPositionX, float playerPositionY, bool loadedNose); //stuff to do every frame BEFORE collision detection
		void update(); //stuff to do every frame AFTER collision detection

		//cheeky ;))))

		float xPos;
		float yPos;

		float xPosNose;
		float yPosNose;

        SDL_Rect* hitBox;
        SDL_Rect* hitBoxHead;
        SDL_Rect* hitBoxNose;

        int rotation;

        bool alive;
        bool openFire;
        bool time2shoot; //okay these names are confusing]
        bool firstStrike;
        bool noseActive;
        bool goingDown;
        bool time2explode;
        bool inFlight;
        Timer shootTime;
        Timer explosionTimer;
        Timer respawnTimer;
        bool exploding;
        bool noseLoaded;
};


#endif // SNOWMAN_H_INCLUDED
