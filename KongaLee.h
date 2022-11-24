#ifndef KONGALEE
#define KONGALEE_H_INCLUDED

#include <SDL.h>
#include "Timer.h"
#include <stdio.h>
#include <cstdlib>

class KongaLee
{
    public:
		KongaLee(int x, int y, int type);
		~KongaLee();
		void reset();

		int variant;

		void move();
		void update();

		float xPos;
		float yPos;

        SDL_Rect* hitBoxBody;
        SDL_Rect* hitBoxFace;
        SDL_Rect* hitBoxFoot;
        SDL_Rect* hitBoxBackFoot;

        bool freeFall; ///state they enter upon spawning

        bool alive;
        bool onFloorGoingLeft;
        bool onFloorGoingRight;
        bool onLeftWallGoingUp;
        bool onLeftWallGoingDown;
        bool onRightWallGoingUp;
        bool onRightWallGoingDown;
        bool onCeilingGoingLeft;
        bool onCeilingGoingRight;

        bool faceHasDetectedASurface;
        bool footHasDetectedASurface;

        short int step;
        short int angle;
        //const SDL_Point* test = {27, 27};
        short int footSpeed;

        bool spitting;
        SDL_Rect* bullets[3];
        bool bulletactive[3];
        int bulletSpeedX[3];
        int bulletSpeedY[3];
        Timer shootTimer;

        bool exploded;
        Timer explosionTimer;
        SDL_Rect* explosionHitbox;

        //what the fuck is this janky shit
        bool executingTurn_GOINGLEFT_ONFLOOR_TO_LEFTWALL;
        void goingleftonfloortoleftwall();

        bool executingTurn_GOINGDOWN_ONLEFTWALL_TO_CEILING;
        void goingdownonleftwalltoceling();

        bool executingTurn_GOINGRIGHT_ONCEILING_TO_RIGHTWALL;
        void goingrightonceilingtorightwall();

        bool executingTurn_GOINGUP_ONRIGHTWALL_TO_FLOOR;
        void goinguponrightwalltofloor();
        ///1

        bool executingTurn_GOINGLEFT_ONFLOOR_TO_RIGHTWALL;
        void goingleftonfloortorightwall();

        bool executingTurn_GOINGUP_ONRIGHTWALL_TO_CEILING;
        void goinguponrightwalltoceiling();

        bool executingTurn_GOINGRIGHT_ONCEILING_TO_LEFTWALL;
        void goingrightonceilingtoleftwall();

        bool executingTurn_GOINGDOWN_ONLEFTWALL_TO_FLOOR;
        void goingdownonleftwalltofloor();

        ///2
};

#endif // KONGALEE_H_INCLUDED
