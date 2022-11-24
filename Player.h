#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED

#include <SDL.h>
#include "Timer.h"
#include <stdio.h>


class Player
{
    public:
		Player();
		~Player();

		short int speed = 250;
		short int health;

		void handleEvent( SDL_Event& event );
		void handleMenuEvents( SDL_Event& event );

		void move( float timeStep ); //stuff before collision detection
		void update();

		void render(SDL_Renderer* renderer, SDL_Texture* texture);

		float xPos; //coordinates of player graphic
		float yPos; //coordinates of player graphic
		float velocity; //useless number so the hitbox doesn't sperg out
		float yVelocity; //useless number so the hitbox doesn't sperg out

		Timer jumpTimer;
		void jump(float timeStep);
		bool jumping;
		bool doubleJumping;
		bool doubleDucking; //cut feature
		bool doubleDuckingUnlocked;
		bool canDoubleJump; //le sigh
		bool doubleFalling; //le sigh
		bool gunBouncing;
		bool bounceHit;
		Timer gunBounceTimer; ///end this timer when you land during a gunbounce
		int gunBounceDirection;
		bool fallen;
		bool rising;
		bool missileToRight;
		bool missileToLeft;
		void missileRight();
		void missileLeft();
		int missileJuice;
		bool ASSsoundMissile;
		bool ASSsoundRising;
		bool ASSsoundLand;

		Timer risingTimer;
		bool released;
		bool onFlatSurface;
		bool foo; ///ほげ
        bool bar; ///ぴよ
        bool baz;
        bool ect;
        bool cantGoRight;
        bool cantGoLeft;
        bool goinLeft;
		bool goinRight;

		bool hasGun;

		void kbTest();
		Timer testLeftTimer;
		Timer testRightTimer;
		bool testingLeft;
		bool testingRight;

		void ledgeLeap();
		bool ledgeLeaping;
		Timer ledgeTimer;

        short int gunDirection;
        bool holdingUp;
        Timer gunTimer;
        short int bullet;
        bool shoot; ///make a bool SHOULDSHOOT for input buffering
        bool shootBuffer;
        int clip;
        int clipSize;
        int reserve;
        void reload();
        Timer reloadTimer;

        bool uncrouch;
        bool canUncrouch;
        bool crouched; //only used for animations for now ///what no its not i think
        Timer hairHitboxTimerLol;

        bool onZipline;

        bool nearOxygen;
        bool succingOxygen;

        bool ledgeHanging;

        float walkToX;
        bool walkingToLocation;
        int whatToDoWhenAtLocation; //man these fucking variable names
        void walkToLocation();

        bool controllable;

        Timer meleeTimer; ///im pretty sure this doesnt actually do anything??

        short int animation;
        short int getAnimation();
        SDL_RendererFlip flip;

        SDL_Rect* hitBox;
		SDL_Rect* rectangleForTheHair;
		SDL_Rect* meleeHitbox;
		SDL_Rect* muzzleHitbox;

		SDL_Rect* granadoHitbox;
		bool granadoActive;
		bool granadoLeft;
		bool granadoRight;
		bool granadoOnFlatSurface;
		float granadoXpos;
		float granadoYpos;
		Timer granadoTimer;
		float granadoVelocityX;  ///have to make these fucking retarded floats because SDL_Rect doesnt know what a fucking float is for some gay ass reason
		float granadoVelocityY;
		float granadoSpeed;
		float granadoSpeedY;
		int granadoSpin;
		int granadoColor;
		bool granadoExploding;
		bool time2explodeGranado;
		bool time2sizzle; //i think this is just for the sound effect
		bool granadoContact;

		Timer blastJumpTimer;
		void blastJump();
		bool blastJumping;
		bool blastJumpingRight;
		bool blastJumpingLeft;
        float blastJumpSpeed;

        Timer damageColor;

		bool meleeHit; //honestly i forgot what this bool even does
		bool endMeleeHit;
                        /// ///problem with using this is the player can only melee 1 enemy at a time, which may be fine
                       ///, but if the player melees two at the same time it will always just hit whichever one gets checked for collision first, which is kinda gay
                       // maybe just update it to = false AFTER all collision detection is done
                       ///i think i did all of these things and fixed it perfectly

        ///int sound that should be played
        ///bool shoud play current sound
        bool paused;
        bool clicky;
        bool unclicky;
        bool clicking;

		bool secretUnlocked;
		bool secretSpeed;

		bool moonGravity;

		bool nearItemButtonOrMachine;
		bool useItemButtonOrMachine;
};

#endif // CHARACTER_H_INCLUDED
