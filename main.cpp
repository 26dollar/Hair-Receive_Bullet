#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>

#include <stdio.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Graphics.h"
#include "Texture.h"
#include "Texture2.h"
#include "Player.h"
#include "Timer.h"
#include "FlatSurface.h"
#include "NonFlatSurface.h"
#include "Bullet.h"
#include "Zipline.h"
#include "Enemy.h"
#include "Snowman.h"
#include "Ledge.h"
#include "ArmoredTruck.h"
#include "RoboMaggot.h"
#include "Targets.h"
#include "TargetMaster.h"
#include "SharkRobot.h"
#include "Kultenta.h"
#include "TNTgent.h"
#include "Menus.h"
#include "Wermbl.h"
#include "GarbageGaurdian.h"
#include "KongaLee.h"
#include "Sorse.h"
#include "Door.h"


TTF_Font *gFont = NULL;


const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000.f / SCREEN_FPS;

Graphics window;

///WHO'S CALLING MY CODE SHADY! I PITY THE FOOL WHO'S CALLIN' 26'S CODE SHADY! PITY I SAY!


//Loads media
//bool processMedia();

//Frees media and shuts down SDL
//void close();

/*double getTimeScalar() ///best function  ///this isnt even used at all anymoroe and never will be and it didnt work
{
    static double lastticks = 0;

    double ticks = SDL_GetTicks();
    double tickssince = ticks - lastticks;

    double scalar = tickssince/1000;

    lastticks = ticks;

   return scalar;
}*/

int checkCollision( SDL_Rect* a, SDL_Rect* b, int mode )
{
    ///mode 1 = normal collision detection
    ///mode 2 = patented 4 side box collisionm [broken]
    ///mode 3 = only checks top side
    ///mode 4 = unpatented 4 side box collision beta [surprisingly less broken?] //okay no wait this is way more broken delete this shit
    ///mode 5 = only check right side
    ///mode 6 = only check left side
    ///mode 7 = don't check top

    //rectangles?
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //squares?
    leftA = a->x;
    rightA = a->x + a->w;
    topA = a->y;
    bottomA = a->y + a->h;

    //boxes?
    leftB = b->x;
    rightB = b->x + b->w;
    topB = b->y;
    bottomB = b->y + b->h;

    if(mode == 1) ///why the fuck am i using else if and not switch case
    {
        if( bottomA <= topB )
        {
            return false;
        }

        if( topA >= bottomB )
        {
            return false;
        }

        if( rightA <= leftB )
        {
            return false;
        }

        if( leftA >= rightB )
        {
            return false;
        }
        return 1; //true, collided
    }
    else if (mode == 4)
    {
        if( rightA < rightB && bottomA > topB && rightA > leftB && topA < bottomB && rightA < leftB + 10) //if pushing right through left side
        {
            return 2; //needs pushed left
        }                                   // + ?
        else if( leftA > leftB &&  bottomA > topB && leftA < rightB && topA < bottomB && leftA > rightB - 10 ) //if pushing left through right side
        {
            return 3; //needs pushed right
        }
        else if( topA < topB && bottomA > topB && rightA > leftB && leftA < rightB) //if falling through top
        {
            return 1; //needs pushed up
        }                                                   ///make this number bigger if shit gets weird vvvvvv
        else if(rightA > leftB && leftA < rightB && topA < bottomB && bottomA > bottomB) //if pushing up through bottom
        {
            return 4; //needs pushed down
        }                                    // + ?
    }
    else if(mode == 3)
    {
        if( topA < topB && bottomA >= topB && rightA > leftB && leftA < rightB && bottomA < topB + 20) //if falling through top
        {
            return 1; //needs pushed up
        }
    }
    else if(mode == 2)
    {                        ///isnt this redundant??      ///make this number bigger if shit gets weird vvvvvv
        if( topA < topB && bottomA > topB && rightA > leftB +4 && leftA < rightB - 4 && bottomA < topB + 14) //if falling through top
        {
            return 1; //needs pushed up
        }                   ///del this?         ///del this?                                               ///negative instead?
        else if( rightA > leftB +18 && leftA < rightB - 18 && topA < bottomB && bottomA > bottomB &&  topA < bottomB + 14) //if falling through bottom
        {
            return 4; //needs pushed down
        }                                   // + ?
        else if( rightA < rightB && bottomA > topB + 5 && rightA > leftB && topA < bottomB ) //if pushing right through left side
        {
            return 2; //needs pushed left
        }                                   // + ?
        else if( leftA > leftB &&  bottomA > topB  + 5 && leftA < rightB && topA < bottomB ) //if pushing left through right side
        {
            return 3; //needs pushed right
        }
    }
    else if (mode == 5)
    {
        if( leftA > leftB &&  bottomA > topB && leftA < rightB && topA < bottomB ) //if pushing left through right side
        {
            return 1; //needs pushed right
        }
    }
    else if (mode == 6)
    {
        if( rightA < rightB && bottomA > topB && rightA > leftB && topA < bottomB ) //if pushing right through left side
        {
            return 1; //needs pushed left
        }
    }
    else if(mode == 7)
    {
        if(rightA > leftB && leftA < rightB && topA < bottomB && bottomA > bottomB && topA > bottomB - 14) //if pushing up through bottom
        {
            return 4; //needs pushed down
        }                                    // + ?
        else if( rightA < rightB && bottomA > topB && rightA > leftB && topA < bottomB ) //if pushing right through left side
        {
            return 2; //needs pushed left
        }                                   // + ?
        else if( leftA > leftB &&  bottomA > topB && leftA < rightB && topA < bottomB ) //if pushing left through right side
        {
            return 3; //needs pushed right
        }
    }
    return 5; //do nothing
}

int main( int argc, char* args[] )
{
    SDL_Rect* camera = new SDL_Rect{ 0, 0, window.getWidth(), window.getHeight()};
    Player player1;
    Menus menu(camera->w, camera->h);

    printf("0046:fixme:winediag:start_process Hair Receive Bullet Staging 4.21 is a testing version containing experimental patches.\n\
0046:fixme:winediag:start_process Please mention your exact version when filing bug reports on HairBulletGame.org.\n\
0046:err:dc:CreateDCW no driver found for L\"\\\\\\\\.\\\\DISPLAY2\\\\Monitor0\"\n\
0046:fixme:win:RegisterTouchWindow (0x1025c 00000003): stub\n\
0046:fixme:imm:ImmReleaseContext (0001025C, 0018DEE0): stub\n\
0046:fixme:msctf:ThreadMgr_ActivateEx Unimplemented flags 0x4\n\
0046:fixme:imm:ImeSetCompositionString PROBLEM: This only sets the wine level string\n\
0046:fixme:imm:ImeSetCompositionString Reading string unimplemented\n\
0046:fixme:imm:NotifyIME NI_CLOSECANDIDATE\n\
fixme:d3d9nine:DRIPresentGroup_GetMultiheadCount (0x1918e0), stub!\n\
fixme:d3d9nine:DRIPresentGroup_GetMultiheadCount (0x1918e0), stub!\n\
0046:err:module:find_forwarded_export module not found for forward 'boardgame.drv.DriverProc' used by L\"C:\\\\windows\\\\system32\\\\boardgame.drv\"\n\
0046:err:winediag:MIDIMAP_drvOpen No software synthesizer midi port found, Midi sound output probably won't work.\n");

printf(
":.:?PPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPGB&@@@&BPPGGGGGPPPPPPPPGG?  .....  :777\n\
:.:?PPPPPPPPPPPPPPPPPPPPPPPPP5PPPPPPPP5PB#@@@@@&BG&@@@@&#GPPPPPPP#Y. ...... :7?7\n\
:.:?PPPPPP55555PPPPPPPPPPPPPPPPPPPPPPP5P#B55G@@@@&#&@@@@@&B5PPPPPGGP!...... :???\n\
:.:?PPPP5PPGBGGP5PPPPPPPPPPGBBGPPPPPPP5P#5:.:YP55J::?B@@&#G5PPPPP5GBBY:.... ^JJJ\n\
:.:?PP5PPGBBBGGP5PPPPPPPPP5PG##BP5PPPP5P#5:........ .:?&@&B5PPPPPP55BG!.... ^JYJ\n\
:.:?PPGB##GPPPPPP5555PPPPPPPG#&&P5PPPP5P#5:.......... ^#@@#PPPPPPPPPPGG?... ^YYY\n\
:.:Y#B#&BPP5PPP5PPPPGB#GPPPPGGBBPPPPPP5P#5:.......... :G&@&BP5PPPPPP5G#Y. . ^Y5Y\n\
:.:Y#&#GPPPPP5PB##GP5GBGPPPPGPJYGPPPPP5P#5:.......... .~G@&BP5PPPPPPPP#Y... ^555\n\
:::?G&#P5PPPPPB&B7^:.:7PBPPPB5:!#G5PPP5P#5:.......... .~G@@&GPPPPPPP5P#G!:  ^555\n\
:::?GBBP5PP5PB#P!. ....5#PGB?:.7#G5PPP5P#5:............:Y@@@BPP5PPPP5P#P~.  ^5P5\n\
:::?PGGPPPPPG&#~.......5&B57:..^5GGP5P5P#5:............ ~#@@BPP5PPPPPPPGGY!^!5P5\n\
:::?PGPPPPPG#&B^.... .!5##J  . :JGGP5PPGG?............. ~#@@BPP5PPPPPPPPPGGGP55P\n\
::^?PP5P5PBBBBGY~... ^5BB5!... :JGGP5PGGY~. ........... ~#@@BPP5PPPPPG&&BBB#GP5P\n\
^:^JPPPPPG#BPPGBJ. . ^P#G~.... :JGGP5PBG!:. ..........  ~#@@#GPPPPPPPG#5:::.!PPP\n\
^^^JPP55B#GPPPPBJ. . ^Y5J. ....^5GGPPBP?. .~JJYJYYYYJ^. ~#@@@&#G5PPP5G#J. . ^PPP\n\
^^^5BPPGB#GPPPPBJ. .......   ..7#GPGGP~..^J#@@@@@&&&@B?.~#@@@@&B5PPPPG#J. . ^PPP\n\
^^^P#PPG##P5PPPBJ. .....:^7GBP7Y#BGBY:..:^!G&@@@G!.!G7..!#@@@@@&BP5P5PB5!.  ~PPP\n\
~^~P#PG&&#G5PPGBJ. .^?P&@@@@@&BB#P?^.......^Y@@G7:^?P^.~P@@@B#&&#P5P5PGPJ:  ~PGP\n\
~~~P&B#@&BP5PGP?^..^YG#@@@&B?7PGY^..........^PBP?!~:. :5&@@&GGB##P5PPPPGG!. ~PGP\n\
~~~P&#G5B#PPGPJ. ....:J#@@#7..5J................:.... ^#@@&BP5PGGPPPP55B#?. ~PGP\n\
~~!PBYJ5PGPPGP?. ......J##BG5YP?..................... .?B@&BP5P55PPPP55GBJ^.^PGP\n\
!~!PG:~BGPPPGP?. .................................... .~P@@&GPPPPPPPPPPPPB5.^PGP\n\
!!!5Y.~BGP5PGP?. .......................   .......... .~P@@@&#PPPPPPPPPPPBP~!PGP\n\
!!!5Y.~BGP5PGP?. ......................:7J?~......... .~P@@@@@#GPPPPPPP55BG7?PPP\n\
7!75Y.~BGP5PGP?. ........~7??777!!75B####BB#G: .........!#@@@@@&BP5PPPP55BG!7PGP\n\
7775Y.~BGPPPGPJ. ......:J&@&#P?~^^^^^^^:...JP: ........ ~#@@@@@&#P5PPPP55BG77PGP\n\
7775Y.~BGPPPPBG^.......^J&G^.............  ?5: ...... .:Y&@@@@@@&BPPPPP55BG!7PGP\n\
77?55~J&#GP55BB~........:5?. ............  ?5: ......:?#@@@@@@@@@BPP5P5GB5!.~PGP\n\
???YG&@@&BP5PPGB?. .......~J~............ .?5:.......Y&@@@@@@@@@&GPPP55B#7. ^PGP\n\
???P#@@@&BP5P5G&P:. ..... :~7!:..        ^J?~. . .~?P&@@@@@@@@@&#P5PP55G#7. ~PGP\n\
???P#@@@&BP5P5G&&B5!. ......^7YJ!~~~~~~~!77^. .~JG&@@@@@@@@@@@@&#P5PP55B#?. ~PGP\n\
??JP#@@@&BP5P5G&@@@@#57~:.....::^~~~~~~~~~!YGGB&@@@@@@@@@@@@@&#BGPPP5PGG5~. ~PGP\n\
J?JP#@@@&BP5P5G&@@@@@@@@@#GGPPP5J^.... :Y#@@@@@@@@@@@@@@@@@@BPP5PPPP5PGPJ:  ~PGP\n\
JJJG&@@&G5PPPPB@@@@@@@@@@@@@@#?^..    :J&@@@@@@@@@@@@@@@@@@@BPP555PPPPG5!.  ^PGP\n\
JJJ5PBG555555PGBBBBBBBBBBB##B57!777777YGB##################BGPPPPPPPPPG5?777JPPP\n\
JJJJJJYYY55555555555555555555PPPPPPPGGPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPPGGGGGPPP\n\
-=- error messages below -=- (clicking inside this window will make the game crash sometimes lol)\n");

	//Start up SDL and create window
	if( !window.Initialize() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if(0) //!loadMedia() )
		{
			//printf( "Failed to load media!\n" );
		}
		else
		{


			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			if( Mix_PlayingMusic() == 0 )
            {
                //Play the music
                //Mix_PlayMusic( gMusic, -1 );
                Mix_VolumeMusic(20);
            }
            //Mix_Volume(-1, 50);





            SDL_Color textColor = { 0, 255, 255, 255 };
            SDL_Color textColor2 = { 90, 255, 50, 255 };

            std::stringstream timeText; ///entirely useless stringstream. don't delete it or everything will break for NO REASON. ,nvm, fixed
            SDL_Point mouse; /// stores the mouse position, duh
            SDL_Rect* mouseHitbox;
            mouseHitbox = new SDL_Rect;
            mouseHitbox->w = 10;
            mouseHitbox->h = 10;
            mouseHitbox->x = 10;
            mouseHitbox->y = 10;

            int countedFrames = 0;
			//The frames per second timer
            Timer fpsTimer;
            //The frames per second cap timer
            Timer capTimer;

            fpsTimer.start();
            bool debug_bool_delete_this_later_idiot = false;
            double timeStep = 0;

            ///FUCK
            double last = 0;
            double now = 0;
            double passed = 0;
            double accumulator = 0;
            double dt = 0;

            Timer timeSpentWhilePausedLol;

            bool area_debug = true;
            bool area_loading = false;
            bool area_1 = false;

            bool needToSpawnShit = true;
            bool needToLoadShit = false;
            short int destination = 0;
            int loadBGalpha = 0;
            int loadTimer = 0;
            //Timer loadScreenTimer;

			//While application is running
			while( !quit )
			{
                while(area_debug && !quit)
                {
                        //Scene textures
                        Texture gPlayer(65, 136);
                        Texture gPlayerPixelated(65, 136);
                        Texture gPlayerArmed(65, 136);
                        Texture gPlayerSemi;
                        Texture gPlayerFace;
                        Texture gGround;
                        Texture gTestBox;
                        Texture gGroundBox;
                        Texture gGroundBox2;
                        Texture ammo1(53, 59);
                        Texture ammo2;
                        Texture hud_head(507, 118);
                        Texture hud_baseplate(347, 27);
                        Texture hud_ammo(0, 23);
                        Texture gWall;
                        Texture gTarget;
                        Texture gBullet;
                        Texture gHair(86, 33);
                        Texture gSemi_Auto;
                        Texture gZipline;
                        Texture gZiplineBase;
                        Texture gTutorialZipline1;
                        Texture gTutorialEnemy;
                        static const int numberOfRobots = 4;
                        Texture gRobot[numberOfRobots];
                        Texture gRobotArmLeft[numberOfRobots];
                        Texture gRobotArmRight[numberOfRobots];
                        Texture gRobotOrig;
                        Texture gRobotArmLeftOrig;
                        Texture gRobotArmRightOrig;
                        Texture gSnowman;
                        Texture gSnowmanNose;
                        Texture gOxygenTank;
                        Texture gLedge;
                        Texture gGranado;
                        Texture gExplosionGeneric;
                        static const int numberOfTrucks = 2;
                        Texture gArmoredTruck[numberOfTrucks];
                        Texture gArmoredTruckCracks(17, 25);
                        Texture gArmoredTruckDevil(97, 12);
                        Texture2 gArmoredTruckOrig;
                        Texture2 gArmoredTruckCracksOrig;
                        Texture2 gArmoredTruckDevilOrig;
                        Texture gSpinner;
                        Texture gBulletCavern;
                        Texture gBulletCavernCage;
                        Texture gBulletCavern2;
                        Texture gNewMove;
                        Texture gBullet2;
                        Texture gRoboMaggot;
                        Texture gTargets;
                        static const short  int numberOfSharkRobots = 2;
                        Texture gSharkRobot[numberOfSharkRobots];
                        Texture gSharkRobotOrig;
                        static const short  int numberOfKultentas = 2;
                        Texture gKultenta[numberOfKultentas];
                        Texture gKultentaEvilArm[numberOfKultentas];
                        Texture gKultentaGun[numberOfKultentas];
                        Texture gKultentaOrig;
                        static const short  int numberOfTNTgent = 1;
                        Texture gTNTgent[numberOfTNTgent];
                        Texture gTNTgentOrig;
                        static const short  int numberOfWermbls = 1;
                        Texture gWermbls[numberOfWermbls];
                        Texture gWermblsOrig;
                        static const short  int numberOfGarbageGaurdians = 1;
                        Texture gGarbageGaurdians[numberOfGarbageGaurdians];
                        Texture gGarbageGaurdianOrig;
                        static const short  int numberOfKongaLees = 2;
                        Texture gKongaLees[numberOfKongaLees];
                        Texture gKongaLeeOrig;
                        static const short  int numberOfSorse = 1;
                        Texture gSorse[numberOfSorse];
                        Texture gSorseOrig;

                        Texture gCeeButton;
                        Texture gPauseMenu;
                        Texture gDebugMenu;
                        Texture gDebugSpawner;
                        Texture gDemoDay;
                        Texture gTutorial;
                        Texture gTutorial2;
                        Texture gTutorialbg;
                        Texture gTutorialbg2;
                        Texture gTutorial3;
                        Texture gKongaCage;
                        Texture gDarunggarBox;
                        Texture gHubWorld;
                        Texture gHubWorld_BG;

                        Texture gLoading;
                        Texture gLoadingBG;

                        Texture gLeverOn;
                        Texture gLeverOff;

                        Texture text;
                        Texture healthHUD;
                        Texture level2Score;

                        Mix_Music *gMusic = NULL;
                        Mix_Chunk *aExplosion = NULL;
                        Mix_Chunk *aSpeedBuff = NULL;
                        Mix_Chunk *aTvSize = NULL;
                        Mix_Chunk *aledgeGrab = NULL;
                        Mix_Chunk *aReload = NULL;
                        Mix_Chunk *aShootSemi = NULL;
                        Mix_Chunk *aShootEmpty = NULL;
                        Mix_Chunk *aGranadoPrimed = NULL;
                        Mix_Chunk *aExplosion1 = NULL;
                        Mix_Chunk *aCrack1 = NULL;
                        Mix_Chunk *aCrack2 = NULL;
                        Mix_Chunk *aCrack3 = NULL;
                        Mix_Chunk *aMissile = NULL;
                        Mix_Chunk *aRising = NULL;
                        Mix_Chunk *aLand = NULL;
                        Mix_Chunk *aShootMaggot = NULL;
                        Mix_Chunk *aTeleMaggotRight = NULL;
                        Mix_Chunk *aTeleMaggotLeft = NULL;
                        Mix_Chunk *aBreak1 = NULL;
                        Mix_Chunk *aBreak2 = NULL;
                        Mix_Chunk *aBreak3 = NULL;
                        Mix_Chunk *aSelect = NULL;
                        Mix_Chunk *aJellyDeath1 = NULL;
                        Mix_Chunk *aJellyDeath2 = NULL;
                        Mix_Chunk *aTNTgent_alert = NULL;
                        Mix_Chunk *aTNTgent_toss = NULL;
                        Mix_Chunk *aWermblONE = NULL;
                        Mix_Chunk *aWermblTWO = NULL;
                        Mix_Chunk *aWermblTHREE = NULL;


                        Texture gBackground;

                        if(1) ///LOAD ALL THE SHIT
                        {
                            //Loading success flag
                            bool success = true;

                            if( !gPlayer.loadFile("boardgame/player1_unarmed_clean.png", window.gRenderer) )
                            {  printf("Failed to load player1spritesheet.png");
                                success = false;
                            }

                            if( !gPlayerPixelated.loadFile("boardgame/test2_tvsize.png", window.gRenderer) )
                            {  printf("Failed to load test2_tvsize.png");
                                success = false;
                            }

                            if( !gPlayerArmed.loadFile("boardgame/player1_gunbody.png", window.gRenderer) )
                            {  printf("Failed to load player1_gunbody.png");
                                success = false;
                            }

                            if( !gPlayerSemi.loadFile("boardgame/playersemi.png", window.gRenderer) )
                            {  printf("Failed to load semi_auto_sheet_test.png");
                                success = false;
                            }

                            if( !gPlayerFace.loadFile("boardgame/player1_face.png", window.gRenderer) )
                            {  printf("Failed to load player1_face2.png");
                                success = false;
                            }

                            if( !ammo1.loadFile("boardgame/delete_this.png", window.gRenderer) )
                            {
                                printf("Failed to load delete_this.png");
                                success = false;
                            }

                            if( !ammo2.loadFile("boardgame/delete_this2.png", window.gRenderer) )
                            {
                                printf("Failed to load delete_this2.png");
                                success = false;
                            }

                            if( !hud_head.loadFile("boardgame/hud_head.png", window.gRenderer) )
                            {
                                printf("Failed to load hud_head.png");
                                success = false;
                            }

                            if( !hud_baseplate.loadFile("boardgame/hud_baseplate.png", window.gRenderer) )
                            {
                                printf("Failed to load hud_baseplate.png");
                                success = false;
                            }

                            if( !hud_ammo.loadFile("boardgame/hud_ammo.png", window.gRenderer) )
                            {
                                printf("Failed to load hud_ammo.png");
                                success = false;
                            }

                            if( !gBackground.loadFile("boardgame/tfijaf7.png", window.gRenderer) )
                            {
                                printf("Failed to load tfijaf.png");
                                success = false;
                            }

                            if( !gGround.loadFile("boardgame/ground.png", window.gRenderer) )
                            {
                                printf("Failed to load ground.png");
                                success = false;
                            }

                            if( !gWall.loadFile("boardgame/wall.png", window.gRenderer) )
                            {
                                printf("Failed to load wa;;.png");
                                success = false;
                            }

                            if( !gTarget.loadFile("boardgame/aggydagger.png", window.gRenderer) )
                            {
                                printf("Failed to load teargpng");
                                success = false;
                            }

                            if( !gTestBox.loadFile("boardgame/testbox.png", window.gRenderer) )
                            {
                                printf("Failed to load testbox.png");
                                success = false;
                            }

                            if( !gGroundBox.loadFile("boardgame/groundbox.png", window.gRenderer) )
                            {
                                printf("Failed to load groundbox.png");
                                success = false;
                            }

                            if( !gGroundBox2.loadFile("boardgame/groundbox2.png", window.gRenderer) )
                            {
                                printf("Failed to load groundbox2.png");
                                success = false;
                            }

                            if( !gBullet.loadFile("boardgame/bullet_round.png", window.gRenderer) )
                            {
                                printf("Failed to load bejjytr.png");
                                success = false;
                            }

                            if( !gHair.loadFile("boardgame/hair_black.png", window.gRenderer) )
                            {  printf("Failed to load hair.png");
                                success = false;
                            }

                            if( !gSemi_Auto.loadFile("boardgame/semi_auto2.png", window.gRenderer) )
                            {  printf("Failed to load semi_auto.png");
                                success = false;
                            }

                            if( !gZipline.loadFile("boardgame/zipline_spritesheet.png", window.gRenderer) )
                            {  printf("Failed to load zipline_spritesheet.png");
                                success = false;
                            }

                            if( !gZiplineBase.loadFile("boardgame/useless_wire_delete_this.png", window.gRenderer) )
                            {  printf("Failed to load useless_aesthetic_wire_delete_this.png");
                                success = false;
                            }

                            if( !gTutorialZipline1.loadFile("boardgame/tutorialzipline1.png", window.gRenderer) )
                            {  printf("Failed to load tutorialzipline1.png");
                                success = false;
                            }
                            if( !gTutorialEnemy.loadFile("boardgame/tutorialenemy.png", window.gRenderer) )
                            {  printf("Failed to load tutorialenemy.png");
                                success = false;
                            }

                            if( !gRobotOrig.loadFile("boardgame/robot_body_deadtest.png", window.gRenderer) )
                            {  printf("Failed to load robot_body.png");
                                success = false;
                            }

                            if( !gRobotArmLeftOrig.loadFile("boardgame/robot_leftarm.png", window.gRenderer) )
                            {  printf("Failed to load robot_leftarm.png");
                                success = false;
                            }

                            if( !gRobotArmRightOrig.loadFile("boardgame/robot_rightarm.png", window.gRenderer) )
                            {  printf("Failed to load robot_rightarm.png");
                                success = false;
                            }

                            if( !gSnowman.loadFile("boardgame/snowman.png", window.gRenderer) )
                            {  printf("Failed to load snowman.png");
                                success = false;
                            }

                            if( !gSnowmanNose.loadFile("boardgame/snowman_nose.png", window.gRenderer) )
                            {  printf("Failed to load snowman_nose.png");
                                success = false;
                            }

                            if( !gOxygenTank.loadFile("boardgame/oxygen_tank.png", window.gRenderer) )
                            {  printf("Failed to load oxygen_tank.png");
                                success = false;
                            }

                            if( !gDemoDay.loadFile("boardgame/demoday.png", window.gRenderer) )
                            {  printf("Failed to load demoday.png");
                                success = false;
                            }

                            if( !gTutorial.loadFile("boardgame/tutorial.png", window.gRenderer) )
                            {  printf("Failed to load tutorial.png");
                                success = false;
                            }

                            if( !gTutorial2.loadFile("boardgame/tutorial2.png", window.gRenderer) )
                            {  printf("Failed to load tutorial2.png");
                                success = false;
                            }

                            if( !gTutorialbg.loadFile("boardgame/tutorialbg.png", window.gRenderer) )
                            {  printf("Failed to load tutorialbg.png");
                                success = false;
                            }

                            if( !gTutorialbg2.loadFile("boardgame/tutorialbg2.png", window.gRenderer) )
                            {  printf("Failed to load tutorialbg2.png");
                                success = false;
                            }

                            if( !gTutorial3.loadFile("boardgame/tutorial3.png", window.gRenderer) )
                            {  printf("Failed to load tutorial3.png");
                                success = false;
                            }

                            if( !gKongaCage.loadFile("boardgame/kongacage.png", window.gRenderer) )
                            {  printf("Failed to load kongacage.png");
                                success = false;
                            }

                            if( !gDarunggarBox.loadFile("boardgame/darunggar.png", window.gRenderer) )
                            {  printf("Failed to load darunggar.png");
                                success = false;
                            }

                            if( !gCeeButton.loadFile("boardgame/c_button_indicator_thingy.png", window.gRenderer) )
                            {  printf("Failed to load c_button_indicator_thingy.png");
                                success = false;
                            }

                            if( !gLedge.loadFile("boardgame/ledge.png", window.gRenderer) )
                            {  printf("Failed to load ledge.png");
                                success = false;
                            }

                            if( !gGranado.loadFile("boardgame/granado.png", window.gRenderer) )
                            {  printf("Failed to load granado.png");
                                success = false;
                            }

                            if( !gExplosionGeneric.loadFile("boardgame/explosion_generic.png", window.gRenderer) )
                            {  printf("Failed to load explosion_generic.png");
                                success = false;
                            }

                            if( !gArmoredTruckOrig.loadFile("boardgame/armored_truck2.png", window.gRenderer) )
                            {  printf("Failed to load aaaaa.png");
                                success = false;
                            }

                            if( !gArmoredTruckCracksOrig.loadFile("boardgame/armored_truck_crack1.png", window.gRenderer) )
                            {  printf("Failed to load aaaaa.png");
                                success = false;
                            }

                            if( !gArmoredTruckDevilOrig.loadFile("boardgame/devilman_and_crybaby.png", window.gRenderer) )
                            {  printf("Failed to load devilman_and_crybaby.png.png");
                                success = false;
                            }

                            if( !gPauseMenu.loadFile("boardgame/pause_menu.png", window.gRenderer) )
                            {  printf("Failed to load pause_menu.png");
                                success = false;
                            }

                            if( !gDebugMenu.loadFile("boardgame/debug_menu.png", window.gRenderer) )
                            {  printf("Failed to load debug_menu.png");
                                success = false;
                            }

                            if( !gDebugSpawner.loadFile("boardgame/debug_spawner.png", window.gRenderer) )
                            {  printf("Failed to load debug_spawner.png");
                                success = false;
                            }

                            if( !gSpinner.loadFile("boardgame/loading_screen_icon.png", window.gRenderer) )
                            {  printf("Failed to load loading_screen_icon.png");
                                success = false;
                            }

                            if( !gBulletCavernCage.loadFile("boardgame/bullet_cavern_cage.png", window.gRenderer) )
                            {  printf("Failed to load bullet_cavern_cage.png");
                                success = false;
                            }

                            if( !gBulletCavern2.loadFile("boardgame/bullet_cavern2.png", window.gRenderer) )
                            {  printf("Failed to load bullet_cavern2.png");
                                success = false;
                            }

                            if( !gBulletCavern.loadFile("boardgame/bullet_cavern.png", window.gRenderer) )
                            {  printf("Failed to load bullet_cavern.png");
                                success = false;
                            }

                            if( !gNewMove.loadFile("boardgame/newmove.png", window.gRenderer) )
                            {  printf("Failed to load newmove.png");
                                success = false;
                            }

                            if( !gBullet2.loadFile("boardgame/bullet_enemy.png", window.gRenderer) )
                            {  printf("Failed to load bullet_enemy.png");
                                success = false;
                            }

                            if( !gRoboMaggot.loadFile("boardgame/robo_maggot.png", window.gRenderer) )
                            {  printf("Failed to load robo_maggot.png");
                                success = false;
                            }

                            if( !gTargets.loadFile("boardgame/targets_clean.png", window.gRenderer) )
                            {  printf("Failed to load targets_clean.png");
                                success = false;
                            }

                            if( !gSharkRobotOrig.loadFile("boardgame/SharkRobot.png", window.gRenderer) )
                            {  printf("Failed to load SharkRobot.png");
                                success = false;
                            }

                            if( !gKultentaOrig.loadFile("boardgame/kultenta.png", window.gRenderer) )
                            {  printf("Failed to load kultenta.png");
                                success = false;
                            }

                            if( !gTNTgentOrig.loadFile("boardgame/TNTgent_clean.png", window.gRenderer) )
                            {  printf("Failed to load TNTgent.png");
                                success = false;
                            }

                            if( !gWermblsOrig.loadFile("boardgame/Wermbl_clean.png", window.gRenderer) )
                            {  printf("Failed to load Wermbl_clean.png");
                                success = false;
                            }

                            if( !gGarbageGaurdianOrig.loadFile("boardgame/Garbage_Gaurdian.png", window.gRenderer) )
                            {  printf("Failed to load Garbage_Gaurdian.png");
                                success = false;
                            }

                            if( !gKongaLeeOrig.loadFile("boardgame/KongaLee.png", window.gRenderer) )
                            {  printf("Failed to load KongaLee_clean.png");
                                success = false;
                            }

                            if( !gSorseOrig.loadFile("boardgame/Sorse_Clean.png", window.gRenderer) )
                            {  printf("Failed to load sorse_clean.png");
                                success = false;
                            }

                            if( !gLeverOff.loadFile("boardgame/leveroff.png", window.gRenderer) )
                            {  printf("Failed to load leveroff.png");
                                success = false;
                            }

                            if( !gLeverOn.loadFile("boardgame/leveron.png", window.gRenderer) )
                            {  printf("Failed to load leveron.png");
                                success = false;
                            }

                            ///NEW SHIT ADDED AFTER HRB-8

                            if( !gHubWorld.loadFile("boardgame/hubworld.png", window.gRenderer) )
                            {  printf("Failed to load hubworld.png");
                                success = false;
                            }

                            if( !gHubWorld_BG.loadFile("boardgame/hubworld_BG.png", window.gRenderer) )
                            {  printf("Failed to load hubworld_BG.png");
                                success = false;
                            }

                            if( !gLoading.loadFile("boardgame/loading_screen_icon2.png", window.gRenderer) )
                            {  printf("Failed to load loading_screen_icon2.png");
                                success = false;
                            }

                            if( !gLoadingBG.loadFile("boardgame/loadingbg.png", window.gRenderer) )
                            {  printf("Failed to load loadingbg.png");
                                success = false;
                            }

                            //Open the font
                            gFont = TTF_OpenFont( "boardgame/FreeSans.ttf", 28 );
                            if( gFont == NULL )
                            {
                                printf( "Failed to load FreeSans.ttf SDL_ttf Error: %s\n", TTF_GetError() );
                                success = false;
                            }

                            ///SOUUNNNDd
                            //SSOOOOYBD
                            /////sound
                            //Load music
                            gMusic = Mix_LoadMUS( "boardgame/HAPPY AGDG DEMO DAY 3!!.mp3" );  ///uncomment this l8r
                            if( gMusic == NULL )
                            {
                                printf( "Failed to load psycho music! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aExplosion = Mix_LoadWAV( "boardgame/bang.wav" );
                            if( aExplosion == NULL )
                            {
                                printf( "Failed to load explosion sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aSpeedBuff = Mix_LoadWAV( "boardgame/Peace Vox.wav");
                            if( aSpeedBuff == NULL )
                            {
                                printf( "Failed to load speed buff sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aTvSize = Mix_LoadWAV( "boardgame/Blue Synth.wav");
                            if( aTvSize == NULL )
                            {
                                printf( "Failed to load tv size sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aledgeGrab = Mix_LoadWAV( "boardgame/ledgegrab.wav");
                            if( aledgeGrab == NULL )
                            {
                                printf( "Failed to load ledge grab sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aReload = Mix_LoadWAV( "boardgame/reloading.wav");
                            if( aReload == NULL )
                            {
                                printf( "Failed to load reloading sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aShootSemi = Mix_LoadWAV( "boardgame/shoot_semi.wav");
                            if( aShootSemi == NULL )
                            {
                                printf( "Failed to load semi shoot sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aShootEmpty = Mix_LoadWAV( "boardgame/shoot_empty.wav");
                            if( aShootEmpty == NULL )
                            {
                                printf( "Failed to load empty gun shoot sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aGranadoPrimed = Mix_LoadWAV( "boardgame/grenado_primed.wav");
                            if( aGranadoPrimed == NULL )
                            {
                                printf( "Failed to load grenado_primed.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aExplosion1 = Mix_LoadWAV( "boardgame/explosion_1.wav");
                            if( aExplosion1 == NULL )
                            {
                                printf( "Failed to load explosion_1.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aCrack1 = Mix_LoadWAV( "boardgame/crack1.mp3");
                            if( aCrack1 == NULL )
                            {
                                printf( "Failed to load crack1.mp3! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aCrack2 = Mix_LoadWAV( "boardgame/crack2.mp3");
                            if( aCrack2 == NULL )
                            {
                                printf( "Failed to load crack2.mp3! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aCrack3 = Mix_LoadWAV( "boardgame/crack3.mp3");
                            if( aCrack3 == NULL )
                            {
                                printf( "Failed to load crack3.mp3! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aMissile = Mix_LoadWAV( "boardgame/audio_player_spinkick.wav");
                            if( aMissile == NULL )
                            {
                                printf( "Failed to load audio_player_spinkick.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aRising = Mix_LoadWAV( "boardgame/audio_player_getup.mp3");
                            if( aRising == NULL )
                            {
                                printf( "Failed to load audio_player_getup.mp3! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aLand = Mix_LoadWAV( "boardgame/Hit_Hurt16.wav");
                            if( aLand == NULL )
                            {
                                printf( "Failed to load Hit_Hurt16.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aShootMaggot = Mix_LoadWAV( "boardgame/maggot_shoot.wav");
                            if( aShootMaggot == NULL )
                            {
                                printf( "Failed to load maggot_shoot.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aTeleMaggotRight = Mix_LoadWAV( "boardgame/maggot_tele_right.wav");
                            if( aTeleMaggotRight == NULL )
                            {
                                printf( "Failed to load maggot_tele_right.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aTeleMaggotLeft = Mix_LoadWAV( "boardgame/maggot_tele_left.wav");
                            if( aTeleMaggotLeft == NULL )
                            {
                                printf( "Failed to load maggot_tele_left.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aBreak1 = Mix_LoadWAV( "boardgame/break1.wav");
                            if( aBreak1 == NULL )
                            {
                                printf( "Failed to load break1.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aBreak2 = Mix_LoadWAV( "boardgame/break2.wav");
                            if( aBreak2 == NULL )
                            {
                                printf( "Failed to load break2.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aBreak3 = Mix_LoadWAV( "boardgame/break3.wav");
                            if( aBreak3 == NULL )
                            {
                                printf( "Failed to load break3.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aSelect = Mix_LoadWAV( "boardgame/select.wav");
                            if( aSelect == NULL )
                            {
                                printf( "Failed to load select.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aJellyDeath1 = Mix_LoadWAV( "boardgame/jellydeath1.wav");
                            if( aJellyDeath1 == NULL )
                            {
                                printf( "Failed to load jellydeath1.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aJellyDeath2 = Mix_LoadWAV( "boardgame/jellydeath2.wav");
                            if( aJellyDeath2 == NULL )
                            {
                                printf( "Failed to load jellydeath2.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aTNTgent_alert = Mix_LoadWAV( "boardgame/aTNTgent_alert.wav");
                            if( aTNTgent_alert == NULL )
                            {
                                printf( "Failed to load aTNTgent_alert.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aTNTgent_toss = Mix_LoadWAV( "boardgame/aTNTgent_toss.wav");
                            if( aTNTgent_toss == NULL )
                            {
                                printf( "Failed to load aTNTgent_toss.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aWermblONE = Mix_LoadWAV( "boardgame/a_Wermbl_One.wav");
                            if( aWermblONE == NULL )
                            {
                                printf( "Failed to load a_Wermbl_One.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aWermblTWO = Mix_LoadWAV( "boardgame/a_Wermbl_Two.wav");
                            if( aWermblTWO == NULL )
                            {
                                printf( "Failed to load a_Wermbl_Two.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aWermblTHREE = Mix_LoadWAV( "boardgame/a_Wermbl_Three.wav");
                            if( aWermblTHREE == NULL )
                            {
                                printf( "Failed to load a_Wermbl_Three.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            if( !success )
                            {
                                printf( "Failed to load media!\n" );
                                quit = true;
                            }
                        }



                        FlatSurface ground(10, 450, 1400, 50, 0, 449);
                        FlatSurface ground2(2245, 845, 1400, 50, 2235, 844);
                        FlatSurface ground3(3740, 845, 1400, 50, 3730, 844);
                        FlatSurface leftWall(0, 0, 20, 450, 0, 0);
                        FlatSurface target(-228, -580, 20, 10, -238, -1616);
                        FlatSurface Secrettarget(-3007, -900, 30, 30, -325, 297);
                        FlatSurface SecretSpeed(5267, -125, 30, 30, 0, 0);
                        FlatSurface semi_auto(-2920, -550, 20, 20, -2920, -550); ///DELETE LITERALLY EVERY ONE OF THESE HOLY SHIT
                        FlatSurface oxygentank( 3840, 774, 150, 70, 3888, 776);
                        FlatSurface bulletCageTrigger( 7600, -360, 150, 450, 0, 776);
                        FlatSurface level1Trigger( 7085, -363, 30, 20, 0, 0);
                        FlatSurface level2Trigger( 8127, -356, 30, 20, 0, 0);
                        FlatSurface tutorialSecret( -4046, -924, 100, 240, 0, 0);

                        FlatSurface lever(3100, 400, 80, 50, 3100, 400);

                        static const short  int NumberOfTestBoxes = 31;
                        FlatSurface testBox[NumberOfTestBoxes] =
                        {
                            FlatSurface(700, 30, 100, 90, 700, 30),
                            FlatSurface(650, 360, 100, 90, 650, 360),
                            FlatSurface(700, 270, 100, 90, 700, 270),
                            FlatSurface(4542, 764, 100, 90, 4542, 764),
                            FlatSurface(4910, 732, 100, 90, 4910, 732),
                            FlatSurface(4942, 485, 1653, 103, 4942, 485),
                            FlatSurface(4942, 85, 403, 103, 4942, 85),
                            FlatSurface(5642, 84, 1256, 103, 0, 0),
                            FlatSurface(2700, 360, 100, 90, 2700, 360),
                            FlatSurface(6888, 85, 1553, 90, 6888, -480),
                            FlatSurface(6914, 35, 128, 50, 0, -0),
                            FlatSurface(8274, 35, 128, 50, 0, -0),
                            FlatSurface(6920, -463, 1471, 90, 0, -0),
                            FlatSurface(6920, -373, 120, 276, 0, -0),
                            FlatSurface(8273, -373, 120, 276, 0, -0),
                            FlatSurface(-4320, -449, 10578, 262, 0, 0), //tutorial
                            FlatSurface(-4320, -1052, 8798, 133, 0, -0), //tutorial
                            FlatSurface(-4315, -695, 400, 300, 0, -0), //tutorial
                            FlatSurface(-4319, -920, 271, 232, 0, -0),//tutorial
                            FlatSurface(-2936, -927, 108, 218, 0, -0),//tutorial
                            FlatSurface(-2941, -532, 148, 85, 0, -0),//tutorial
                            FlatSurface(-1309, -545, 126, 101, 0, -0),//tutorial
                            FlatSurface(3770, -1185, 50, 20, 3770, -1185),//trashcan test

                            FlatSurface(3055, -1448, 174, 28, 0, -0), //konga lee cage bottom
                            FlatSurface(3028, -1540, 28, 120, 0, -0), //konga cage
                            FlatSurface(3201, -1567, 28, 120, 0, -0), //kongacage
                            FlatSurface(3028, -1567, 174, 28, 3028, -1567), //konga lee cage top

                            FlatSurface(-3900, -2102, 799, 56, 0, 0), //hub world bottom
                            FlatSurface(-3980, -2513, 80, 467, -3980, -2513), //hub world left
                            FlatSurface(-3101, -2513, 49, 467, 0, 0), //hub world right
                            FlatSurface(-3900, -2513, 799, 94, 0, 0) //hub world top
                        };

                        static const short  int NumberOfTestSlopes = 2;
                        NonFlatSurface testSlopes[NumberOfTestSlopes] =
                        {
                            NonFlatSurface(3821, -1052, 55, 8, 0, 0),
                            NonFlatSurface(4260, -1107, 55, 4, 0, 0)
                        };

                        Ledge ledges[2] =
                        {
                            Ledge(4500, 500, true),
                            Ledge(4900, 500, false)
                        };

                        TargetMaster targetGame(7050, -150); //8225, 50 /// 1175, 200

                        Bullet player1bullets[20];
                        Bullet enemyBullets[20];

                        Zipline zipline1(1372, 200);
                        RoboMaggot maggot(8254, -27);

                        static const short  int NumberOfDoors = 3;
                        Door doors[NumberOfDoors] =
                        {
                            Door(-3680, -2243, 0), //debug
                            Door(-3498, -2243, 1), //level 1
                            Door(-3316, -2243, 2) //tutorial
                        };

                        Enemy robots[numberOfRobots] =
                        {
                            Enemy(2656, 679),
                            Enemy(2856, 679),
                            Enemy(-1729, -565),
                            Enemy(4143, -546)
                        };

                        Snowman snowman1(3600, 679);

                        //static const short int ass = 1;
                        ArmoredTruck trucks[numberOfTrucks] =
                        {
                          ArmoredTruck(5410, 330),
                          ArmoredTruck(6110, 330)
                        };

                        SharkRobot sharkRobots[numberOfSharkRobots] =
                        {
                          SharkRobot(574, -1179),
                          SharkRobot(1000, -1179)
                        };

                        Kultenta kultentas[numberOfKultentas] =
                        {
                          Kultenta(1574, -1188),
                          Kultenta(2000, -1188)
                        };

                        TNTgent gents[numberOfTNTgent] =
                        {
                          TNTgent(2430, -1188)
                        };

                        Wermbl wermbls[numberOfWermbls] =
                        {
                          Wermbl(2774, -1188)
                        };

                        GarbageGaurdian garbageGaurdians[numberOfGarbageGaurdians] =
                        {
                          GarbageGaurdian(3544, -1198)
                        };

                        KongaLee kongaLees[numberOfKongaLees] =
                        {
                          KongaLee(3130, -2800, 2),
                          KongaLee(3135, -1525, 0)
                        };

                        Sorse sorse[numberOfSorse] =
                        {
                          Sorse(2890, -1270)
                        };
                        needToSpawnShit = false;

                while(!needToSpawnShit && !quit)
                {
			    now = SDL_GetTicks();
			    passed = now - last;
			    last = now;
			    accumulator += passed;

			    capTimer.start();

                while (accumulator >= dt) {

                player1.clicky = false; ///if i don't put these here SDL will just assume the mouse is still clicking if there's no other SDL_PollEvents to update it and make it actually fucking check if its still clicking, idfk this shit sux
                player1.unclicky = false;
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					if(!player1.paused)
                    {
                        player1.handleEvent( e );
                        Mix_ResumeMusic();

                    }
                    else
                    {
                        player1.handleMenuEvents( e );
                        Mix_PauseMusic();
                    }


				}

				if(player1.paused) ///count the time spent while game is paused
                {
                    if(timeSpentWhilePausedLol.isStarted())
                    {
                        timeSpentWhilePausedLol.unpause();
                    }
                    else
                    {
                        timeSpentWhilePausedLol.start();
                    }
                }
                else
                {
                    timeSpentWhilePausedLol.pause();
                }

                ///this lets the timers know how much time the game spent while paused,
				///ya this is rly the best solution i could come up with for this basically fuck fuck fuck

				///PLAYER TIMERS
				player1.reloadTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				player1.jumpTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				player1.testLeftTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				player1.testRightTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				player1.gunTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				player1.meleeTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				player1.granadoTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				player1.blastJumpTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				player1.risingTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				player1.ledgeTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerSemi.animationTimer2.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerSemi.animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayer.animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayer.animationTimer2.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayer.animationTimer3.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerArmed.animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerArmed.animationTimer2.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerArmed.animationTimer3.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerArmed.animationTimer4.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerArmed.animationTimer5.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerArmed.animationTimer6.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerArmed.animationTimer7.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerArmed.animationTimer8.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerFace.animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gHair.animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				for(int i = 0; i < 19; i++)
                {
                    player1bullets[i].testTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    player1bullets[i].deletionTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                }


				///SNOWMAN TIMERS
				snowman1.explosionTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				snowman1.respawnTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				snowman1.shootTime.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gSnowman.animationTimer4.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gSnowman.animationTimer5.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gSnowman.animationTimer6.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gSnowmanNose.animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gSnowmanNose.animationTimer4.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gSnowmanNose.animationTimer5.gamePausedTicks = timeSpentWhilePausedLol.getTicks();

				///ROBOT TIMERS
				for(int i = 0; i < numberOfRobots; i++)
                {
                    gRobot[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gRobotArmLeft[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gRobotArmRight[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                }

                ///ZIPLINE TIMERS
                gZipline.animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();

                ///ARMORED TRUCK TIMERS
                for(int i = 0; i < numberOfTrucks; i++)
                {
                    trucks[i].triggeredTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    trucks[i].respawnTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gArmoredTruck[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                }

                ///level 1 & 2 target timers
                for(int i = 0; i < 15; i++)
                {
                    targetGame.targets[i].funTimerlol.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                }
                targetGame.funTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                maggot.volleyTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                maggot.bufferTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();

                ///robot shark timers
                for(int i = 0; i < numberOfSharkRobots; i++)
                {
                    gSharkRobot[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gSharkRobot[i].animationTimer2.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    sharkRobots[i].jumpTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    sharkRobots[i].beingShotInTheFaceTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                }

                ///kultenta timers
                for(int i = 0; i < numberOfKultentas; i++)
                {
                    gKultenta[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gKultentaEvilArm[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gKultentaGun[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    kultentas[i].animationTime.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    kultentas[i].rootyTootyTimetoShootyTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    kultentas[i].replaceTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    kultentas[i].replaceTimer2.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    kultentas[i].replaceTimer3.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    kultentas[i].replaceTimer4.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    kultentas[i].replaceTimer5.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                }

                ///TNT gent timers
                for(int i = 0; i < numberOfTNTgent; i++)
                {
                    gTNTgent[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gTNTgent[i].animationTimer2.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gTNTgent[i].animationTimer3.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gTNTgent[i].animationTimer4.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gTNTgent[i].animationTimer7.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gents[i].programTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gents[i].speakingTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gents[i].hitColorHead.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gents[i].explosionTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    //uhhh this should be all of them
                }

                ///Wermbl timers
                for(int i = 0; i < numberOfWermbls; i++)
                {
                    gWermbls[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gWermbls[i].animationTimer2.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gWermbls[i].animationTimer4.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gWermbls[i].animationTimer5.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    wermbls[i].gachaActivationTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    wermbls[i].shootTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    wermbls[i].explosionTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                }

                ///Garbage Gaurdian timers
                for(int i = 0; i < numberOfGarbageGaurdians; i++)
                {
                    gGarbageGaurdians[i].animationTimer3.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    garbageGaurdians[i].animationTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    garbageGaurdians[i].hitColorBody.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                }

                ///Konga Lee timers
                for(int i = 0; i < numberOfKongaLees; i++)
                {
                    gKongaLees[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gKongaLees[i].animationTimer2.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gKongaLees[i].animationTimer4.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    kongaLees[i].explosionTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    kongaLees[i].shootTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                }




				/*********TIME SHIT*********/
                //Calculate and correct fps
                float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
                if( avgFPS > 2000000 )
                {
                        avgFPS = 0;
                }

				//Calculate time step
				//timeStep = getTimeScalar();
				dt = 1000 / 60;
				timeStep = 0.016;
                /*********TIME SHIT*********/

                /********* everything *********/

                if(!player1.paused)
                {

                player1.move( timeStep );
                if(player1.shoot  && player1.clip > 0)
                {
                    player1bullets[player1.bullet].restart(player1.hitBox->x, player1.hitBox->y, player1.gunDirection, player1.flip);
                    player1.clip--;
                }


                gPlayerSemi.setColor(0xFF, 0xFF, 0xFF);
                gPlayerArmed.setColor(0xFF, 0xFF, 0xFF);



                for(int i = 0; i < numberOfRobots; i++)
                {
                    robots[i].move();
                }
                snowman1.move(gSnowman.animationBool, gSnowmanNose.animationBool, player1.hitBox->x, player1.hitBox->y, gSnowman.animationBool3);
                for(int i = 0; i < 19; i++)
                {
                    player1bullets[i].move( timeStep );
                }

                zipline1.move( timeStep );

                if(player1.hitBox->y > 1000)
                {
                    player1.velocity = -3800;
                    player1.yVelocity = -790;
                    player1.hasGun = false;

                    zipline1.docked = 1;
                    zipline1.xVelocity = zipline1.linePosX;
                    zipline1.yVelocity = zipline1.linePosY;
                }

                for(int i = 0; i < numberOfTrucks; i++)
                {
                    trucks[i].move();
                }

                for(int i = 0; i < numberOfSharkRobots; i++)
                {
                    sharkRobots[i].move();
                }

                for(int i = 0; i < numberOfKultentas; i++)
                {
                    kultentas[i].move();
                }

                for(int i = 0; i < numberOfTNTgent; i++)
                {
                    gents[i].move(gTNTgent[i].animationInt);
                }

                for(int i = 0; i < numberOfWermbls; i++)
                {
                    wermbls[i].move();
                }

                for(int i = 0; i < numberOfGarbageGaurdians; i++)
                {
                    garbageGaurdians[i].move();
                }

                for(int i = 0; i < numberOfKongaLees; i++)
                {
                    kongaLees[i].move();
                }

                for(int i = 0; i < numberOfSorse; i++)
                {
                    sorse[i].move();
                }

                maggot.move();
                targetGame.move();
                for(int i = 0; i < 15; i++)
                {
                    targetGame.targets[i].move();
                }
                if(maggot.shoot )
                {
                    if(maggot.flip == SDL_FLIP_NONE)
                    {
                        enemyBullets[maggot.bullet].restart2(maggot.hitBox->x, maggot.hitBox->y, maggot.direction);
                    }
                    else
                    {
                        enemyBullets[maggot.bullet].restart2(maggot.hitBox->x + 20, maggot.hitBox->y, maggot.direction);
                    }
                    Mix_PlayChannel( -1, aShootMaggot, 0 );
                }
                for(int i = 0; i < 19; i++)
                {
                    enemyBullets[i].move2();
                }

                ///shady ass collision code
                //shady ass collision code
                ///shady ass collision code
                for(int i = 0; i < NumberOfTestBoxes; i++)
                {
                  /*  if(!player1.released && player1.jumping)
                    {
                        switch ( checkCollision(player1.hitBox, testBox[i].hitBox, 7) ) ///collision for player hitting test boxes
                        {
                            case 1: player1.yVelocity = testBox[i].hitBox->y - player1.hitBox->h + 1;
                                    player1.foo = true;
                                    if(player1.jumpTimer.getTicks() > 40) {player1.jumping = false; player1.jumpTimer.stop(); }
                                break;                                                        //+1?
                            case 2: player1.velocity = testBox[i].hitBox->x - player1.hitBox->w +1;
                                    player1.bar = true;
                                break;
                            case 3: player1.velocity = testBox[i].hitBox->x + testBox[i].hitBox->w -1;
                                    player1.baz = true;
                                break;
                            case 4: player1.yVelocity = testBox[i].hitBox->y + testBox[i].hitBox->h;
                                    player1.released = true;
                                break;
                            case 5: //do nothing
                                break;
                        }
                    }
                    else
                    {*/
                        switch ( checkCollision(player1.hitBox, testBox[i].hitBox, 2) ) ///collision for player and test boxes
                        {
                            case 1: player1.yVelocity = testBox[i].hitBox->y - player1.hitBox->h + 1;
                                    player1.foo = true;
                                    if(player1.jumpTimer.getTicks() > 40) {player1.jumping = false; player1.jumpTimer.stop(); }
                                break;                                                        //+1?
                            case 2: player1.velocity = testBox[i].hitBox->x - player1.hitBox->w +1;
                                    player1.bar = true;
                                break;
                            case 3: player1.velocity = testBox[i].hitBox->x + testBox[i].hitBox->w -1;
                                    player1.baz = true;
                                break;
                            case 4: player1.yVelocity = testBox[i].hitBox->y + testBox[i].hitBox->h + 0;
                                    player1.released = true;
                                    player1.ect = true;
                                    if(player1.blastJumping) { gPlayerSemi.setColor(255, 0, 0); gPlayerArmed.setColor(255, 0, 0); player1.blastJumpTimer.stop(); player1.blastJumping = false; player1.ledgeLeaping = false; player1.ledgeTimer.stop(); };
                                break;
                            case 5: //do nothing

                                break;
                       // }
                    }

                }

                for(int i = 0; i < NumberOfTestSlopes; i++)
                {
                    if(player1.hitBox->x + player1.hitBox->w / 2 > testSlopes[i].x1 && player1.velocity + player1.hitBox->w / 2 < testSlopes[i].x2) ///collision for player and test slopes
                    {
                        if(player1.hitBox->y + player1.hitBox->h > (testSlopes[i].y1 - 15) - ((player1.hitBox->x - testSlopes[i].x1) / testSlopes[i].slope) + 1  && (player1.hitBox->y + player1.hitBox->h < (testSlopes[i].y1 - 15) - ((player1.hitBox->x - testSlopes[i].x1) / testSlopes[i].slope) + 85) )  ///this is complicated
                        {                                                                                                                                                                                                                     /// i think 85 means 85 pixesl below it
                            player1.yVelocity = (testSlopes[i].y1 - 15) - ((player1.hitBox->x - testSlopes[i].x1) / testSlopes[i].slope) - player1.hitBox->h + 11;
                            player1.foo = true;
                            if(player1.released) { player1.jumping = false;} //uhhh i dont think this does anything anymore but iduno
                        }
                    }
                }

                if( checkCollision(player1.rectangleForTheHair, testBox[2].hitBox, 1) == 1) ///collision for hair hitbox dealing with uncrouching
                {
                    player1.canUncrouch = false;
                }
                else
                {
                    player1.canUncrouch = true;
                }

                for(int i = 0; i < 19; i++)    ///collision for bullets and testboxes i think
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < NumberOfTestBoxes; a++)
                        {
                            if(checkCollision(player1bullets[i].hitBox, testBox[a].hitBox, 1) == 1)
                            {
                                player1bullets[i].active = false;
                            }
                        }
                    }
                }

                if(checkCollision(player1.hitBox, ground.hitBox, 3) == 1)       ///collision for player and the ground, walls, and GUN
                {
                    player1.yVelocity = ground.hitBox->y - player1.hitBox->h +1;
                    player1.foo = true;
                }

                if(checkCollision(player1.hitBox, ground2.hitBox, 3) == 1)
                {
                    player1.yVelocity = ground2.hitBox->y - player1.hitBox->h +1;
                    player1.foo = true;
                }

                if(checkCollision(player1.hitBox, ground3.hitBox, 3) == 1)
                {
                    player1.yVelocity = ground3.hitBox->y - player1.hitBox->h +1;
                    player1.foo = true;
                }

                if(checkCollision(player1.hitBox, leftWall.hitBox, 5) == 1)
                {
                    player1.velocity = leftWall.hitBox->x + leftWall.hitBox->w;
                }

                if(checkCollision(player1.hitBox, semi_auto.hitBox, 1) == 1)
                {
                    player1.secretUnlocked = false;
                    player1.hasGun = true;
                }

                for(int i = 0; i < 19; i++) ///collision for player bullets hitting target
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < 3; a++)
                        {
                            if(checkCollision(player1bullets[i].hitBox, target.hitBox, 1) == 1)
                            {
                                player1bullets[i].direction = 4;
                            }
                        }
                    }
                }

                for(int i = 0; i < 19; i++) ///collision for player bullets hitting the secret target
                {
                    if(player1bullets[i].active)
                    {
                        if(checkCollision(player1bullets[i].hitBox,  Secrettarget.hitBox, 1) == 1)
                        {
                            player1bullets[i].active = false;
                            player1.hasGun = false;
                            player1.secretUnlocked = true;
                            Mix_PlayChannel( -1, aTvSize, 0 );
                        }
                    }
                }

                for(int i = 0; i < 19; i++) ///collision for player bullets hitting the secret SPEEDY target
                {
                    if(player1bullets[i].active)
                    {
                        if(checkCollision(player1bullets[i].hitBox, SecretSpeed.hitBox, 1) == 1)
                        {
                            player1bullets[i].active = false;
                            if(!player1.secretSpeed)
                            {
                                aSpeedBuff->volume = 100;
                                player1.secretSpeed = true;
                                gPlayerFace.setColor(255, 155, 50);
                                Mix_PlayChannel( -1, aSpeedBuff, 0 );
                                gPlayerFace.animationBool = false;
                            }
                            else
                            {
                                player1.secretSpeed = false;
                                gPlayerFace.setColor(0xFF, 0xFF, 0xFF);
                                Mix_PlayChannel( -1, aSpeedBuff, 0 );
                            }
                        }
                    }
                }

                ///collision for player touching earraep tutorial
                if(checkCollision(player1.hitBox, tutorialSecret.hitBox, 1) == 1)
                {
                    Mix_VolumeMusic(200);
                }
                else
                {
                    Mix_VolumeMusic(20);
                }

                for(int i = 0; i < 19; i++) ///collision for player bullets hitting the lever
                {
                    if(player1bullets[i].active)
                    {
                        if(checkCollision(player1bullets[i].hitBox,  lever.hitBox, 1) == 1)
                        {
                            player1bullets[i].active = false;
                            if(debug_bool_delete_this_later_idiot)
                            {
                                debug_bool_delete_this_later_idiot = false;
                            }
                            else
                            {
                                debug_bool_delete_this_later_idiot = true;
                            }
                        }
                    }
                }

                for(int i = 0; i < 19; i++) ///collision for player bullets hitting player hair
                {
                    if(player1bullets[i].active && !player1bullets[i].following)
                    {
                        //for(int a = 0; a < 3; a++) WHY THE FUCK DID IT ORIGINALLY CHECK FOR COLLISION THREE TIMES?
                        ///--ask tony
                        //{s
                            if(checkCollision(player1bullets[i].hitBox, player1.rectangleForTheHair, 1) == 1)
                            {
                                if(player1.clip < 15 && player1bullets[i].active)
                                {
                                    player1.clip++;
                                    gHair.animationTimer1.start();Mix_PlayChannel( -1, aTvSize, 0 );
                                }
                                player1bullets[i].active = false;
                            }
                        //}
                    }
                }

                for(int i = 0; i < 19; i++) ///collision for enemy bullets hitting player hair
                {
                    if(enemyBullets[i].active)
                    {
                        //for(int a = 0; a < 3; a++) WHY THE FUCK DID IT ORIGINALLY CHECK FOR COLLISION THREE TIMES?
                        ///--ask tony
                        //{s
                            if(checkCollision(enemyBullets[i].hitBox, player1.rectangleForTheHair, 1) == 1)
                            {
                                if(player1.clip < 15 && enemyBullets[i].active)
                                {
                                    player1.clip++;
                                }
                                gHair.animationTimer1.start();Mix_PlayChannel( -1, aTvSize, 0 );
                                enemyBullets[i].active = false;
                                maggot.score++;
                            }
                        //}
                    }
                }

                if(!player1.missileToLeft && !player1.missileJuice) ///ur invincible if you missile left or right because knockback + missile speed makes u go thru walls
                {
                    for(int i = 0; i < 19; i++) ///collision for enemy bullets hitting player
                    {
                        if(enemyBullets[i].active)
                        {
                            //for(int a = 0; a < 3; a++) WHY THE FUCK DID IT ORIGINALLY CHECK FOR COLLISION THREE TIMES?
                            ///--ask tony
                            //{s
                                if(checkCollision(enemyBullets[i].hitBox, player1.hitBox, 1) == 1)
                                {
                                    if(enemyBullets[i].direction == 4)
                                    {
                                        player1.testingLeft = true;
                                        player1.damageColor.start();
                                        player1.health--;
                                    }
                                    else if(enemyBullets[i].direction == 2)
                                    {
                                        player1.testingRight = true;
                                        player1.damageColor.start();
                                        player1.health--;
                                    }
                                    enemyBullets[i].active = false;
                                }
                            //}
                        }
                    }
                }

                for(int i = 0; i < 19; i++) ///collision for player bullets hitting zipline
                {
                    if(player1bullets[i].active)
                    {
                        if(checkCollision(player1bullets[i].hitBox, zipline1.hitBox, 1) == 1)
                        {
                            zipline1.powerTimer.start();
                            player1bullets[i].active = false;
                        }
                    }
                }

                ///COLLISION FOR ROBOT AND MELEE HITBOX                                               ///okay so later i need to move all of these checks into the player class, but if i use gunDirection = 1 instead of !holdingup then i can do multiple hits with one melee for some reason, I THINK because gundirection is changed AFTER collision detection
                 for(int i = 0; i < numberOfRobots; i++)
                 {
                    if(!player1.holdingUp && checkCollision(player1.meleeHitbox, robots[i].hitBox, 1) == 1 && gPlayerSemi.animationTimer2.getTicks() < 249 && gPlayerSemi.animationTimer2.getTicks() > 160) ///collicion for
                    {                                                                                                                                                                                         //wtf does this mean?
                        if(!robots[i].respawnTimer.isStarted() && !player1.meleeHit) ///switch this around or somethign this is retarded
                        {
                            if(!robots[i].hitColorBody.isStarted())
                            {
                                robots[i].hitColorBody.start();
                            }

                            robots[i].health--;
                            player1.endMeleeHit = true;
                        }
                    }
                    else if(player1.gunBouncing && !player1.meleeHit && player1.gunBounceTimer.isStarted())///gunbounce
                    {
                        if(!robots[i].respawnTimer.isStarted() && checkCollision(player1.meleeHitbox, robots[i].hitBox, 1) == 1)
                        {
                            if(!robots[i].hitColorBody.isStarted())
                            {
                                robots[i].hitColorBody.start();
                            }

                            robots[i].health--;
                            player1.endMeleeHit = true;
                            player1.bounceHit = true;
                        }
                    }

                 }

                ///collision for TEST BOXES and GUN BOUNCE
                if(player1.gunBouncing && !player1.meleeHit && player1.gunBounceTimer.isStarted())
                    {
                        for(int i = 0; i < NumberOfTestBoxes; i++)
                        {
                            if(checkCollision(player1.meleeHitbox, testBox[i].hitBox, 1) == 1)
                            {
                                printf("ffffffffffff");
                                player1.endMeleeHit = true;
                                player1.bounceHit = true;
                            }
                        }
                    }

                ///COLLISION FOR ROBOT body AND player bullets
                for(int p = 0; p < numberOfRobots; p++)
                {
                    if(robots[p].alive)
                    {
                        for(int i = 0; i < 19; i++)
                        {
                            if(player1bullets[i].active)
                            {
                                if(checkCollision(player1bullets[i].hitBox, robots[p].hitBox, 1) == 1)
                                {
                                    player1bullets[i].testing = true;
                                    if(!player1bullets[i].testTimer.isStarted())
                                    {
                                        if(player1bullets[i].direction == 2)
                                        {
                                            player1bullets[i].direction = 4;
                                        }
                                        else if(player1bullets[i].direction == 4)
                                        {
                                            player1bullets[i].direction = 2;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                ///COLLISION FOR ROBOT LEFT CLAW AND player bullets
                for(int p = 0; p < numberOfRobots; p++)
                {
                    if(robots[p].leftActive)
                    {
                        for(int i = 0; i < 19; i++)
                        {
                            if(player1bullets[i].active)
                            {
                                if(checkCollision(player1bullets[i].hitBox, robots[p].hitBoxLeft, 1) == 1)
                                {
                                    if(!robots[p].hitColorLeft.isStarted())
                                    {
                                        robots[p].hitColorLeft.start();
                                    }
                                    player1bullets[i].active = false;
                                    robots[p].leftHealth--;
                                }
                            }
                        }
                    }
                }

                ///COLLISION FOR PLAYER AND LEFT ROBOT CLAW
                for(int i = 0; i < numberOfRobots; i++)
                {
                    if(robots[i].leftActive)
                    {
                        if(checkCollision(player1.hitBox, robots[i].hitBoxLeft, 1) == 1)
                        {
                            if(!player1.damageColor.isStarted())
                            {
                               player1.testingLeft = true;
                                player1.damageColor.start();
                                player1.health--;
                            }
                        }
                    }
                }
                ///COLLISION FOR ROBOT RIGHT CLAW AND player bullets
                for(int p = 0; p < numberOfRobots; p++)
                {
                    if(robots[p].rightActive)
                    {
                        for(int i = 0; i < 19; i++)
                        {
                            if(player1bullets[i].active)
                            {
                                if(checkCollision(player1bullets[i].hitBox, robots[p].hitBoxRight, 1) == 1)
                                {
                                    if(!robots[p].hitColorRight.isStarted())
                                    {
                                        robots[p].hitColorRight.start();
                                    }
                                    player1bullets[i].active = false;
                                    robots[p].rightHealth--;
                                }
                            }
                        }
                    }
                }



                ///COLLISION FOR PLAYER AND RIGHT ROBOT CLAW
                for(int i = 0; i < numberOfRobots; i++)
                {
                    if(robots[i].rightActive)
                    {
                        if(checkCollision(player1.hitBox, robots[i].hitBoxRight, 1) == 1)
                        {
                            if(!player1.damageColor.isStarted())
                            {
                                player1.testingRight = true;
                                player1.damageColor.start();
                                player1.health--;
                            }
                        }
                    }
                }


                //std::cout << "dddd: " << player1.meleeTimer.getTicks() << std::endl;
                //std::cout << "dxxx: " << gPlayerSemi.animationTimer2.getTicks() << std::endl;
                //std::cout << "yyyy: " << zipline1.yPos << std::endl;
                //std::cout << "yyy: " << enemy1.hitBoxLeft->y << std::endl;
                //std::cout << "yyy: " << gEnemy.animationTimer1.getTicks() / 100 << std::endl;

             //std::cout << player1.paused << std::endl;
                //std::cout << player1bullets[1].testTimer.getTicks() << std::endl;

                 std::cout << "xxx: " << doors[1].destinationX << std::endl;
               //std::cout << "yyyy: " << player1.gunDirection << std::endl;


                /*std::cout << "yyyy: " << zipline1.xPos << std::endl;
                std::cout << "yyyy: " << zipline1.yPos << std::endl;
                std::cout << "yyyy: " << zipline1.docked << std::endl;*/

                //std::cout << "xxxx: " << sorse[0].blackHoleSuccer.x << std::endl;
                //std::cout << "xxxx: " << sorse[0].blackHoleSuccer.y << std::endl;
                //std::cout << "yyyy: " << kongaLees[0].bullets[0]->y << std::endl;
                //std::cout << "yyyy: " << player1.hitBox->y << std::endl;

                //std::cout << "foo: " << player1.foo << std::endl;
                //std::cout << "bar: " << player1.bar << std::endl;
                //std::cout << "baz: " << player1.baz << std::endl;
                //std::cout << "ect: " << player1.ect << std::endl;

                //std::cout << "x: " << player1.useItemButtonOrMachine << std::endl;
                /*std::cout << "x1: " << testSlopes[0].x1 << std::endl;
                std::cout << "y1: " << testSlopes[0].y1 << std::endl;
                std::cout << "x2: " << testSlopes[0].x2 << std::endl;
                std::cout << "y2: " << testSlopes[0].y2 << std::endl;*/
                //mouseHitbox->x = mouse.x;
                //mouseHitbox->y = mouse.y;



               // *SDL_Point mouse;

                SDL_GetMouseState(&mouse.x, &mouse.y);

                mouse.x += camera->x;
                mouse.y += camera->y;
                std::cout << "x: " << mouse.x  << std::endl;
                std::cout << "y: " << mouse.y  << std::endl;
                mouse.x -= camera->x;
                mouse.y -= camera->y; ///uncomment this shit, its epic





                    if(zipline1.xVelocity >= 2284)                                                      ///collision for the zipline and player
                    {
                        zipline1.xVelocity = 2284;
                        zipline1.yVelocity = 656;
                        //zipline1.xPos = 2284;
                        //zipline1.yPos = 656;
                        zipline1.docked = true;
                    }
                    else
                    {
                        zipline1.docked = false;
                    }

                    if(zipline1.xVelocity <= 1372)
                    {
                        zipline1.xVelocity = 1372;
                        zipline1.yVelocity = 200;
                        //zipline1.xPos = 1372;
                        //zipline1.yPos = 200;
                        zipline1.powerTimer.stop();
                        zipline1.docked = true;
                    }

                    if( player1.hasGun && player1.gunDirection == 1 && checkCollision(player1.muzzleHitbox, zipline1.hitBox, 1) == 1)
                    {
                        if(zipline1.carrying)
                        {
                            player1.onZipline = true;
                            if(!player1.flip)
                            {
                                 player1.velocity = zipline1.xVelocity + 20;
                                player1.yVelocity = zipline1.yVelocity + 50;
                            }
                            else
                            {
                                player1.velocity = zipline1.xVelocity - 20;
                                player1.yVelocity = zipline1.yVelocity + 50;
                            }
                        }
                        else
                        {
                            zipline1.docked = false;
                            zipline1.carrying = true;
                        }
                    }
                    else
                    {
                        zipline1.carrying = false;
                        player1.onZipline = false;
                    }

                for(int i = 0; i < numberOfRobots; i++) ///collision with robots and testboxes DICKLE
                {
                    for(int a = 0; a < NumberOfTestBoxes; a++)
                    {
                        //robots[i].onFlatSurface = false;
                        switch ( checkCollision(robots[i].hitBox, testBox[a].hitBox, 2) )
                        {
                            case 1: robots[i].hitBox->y = testBox[a].hitBox->y - robots[i].hitBox->h -0;
                                    robots[i].onFlatSurface = true;
                                break;
                            case 2: //hitting left wall
                                    robots[i].home -= 30;
                                break;
                            case 3: //hitting right wall
                                    robots[i].home += 30;
                                break;
                            case 4: //robots should never hit a ceiling
                                break;
                            case 5: //do nothing

                                break;
                        }


                        if(robots[i].leftFalling)
                        {
                            if(checkCollision(robots[i].hitBoxLeft, testBox[a].hitBox, 1))
                            {
                                robots[i].leftFalling = false;
                            }
                        }
                        if(robots[i].rightFalling)
                        {
                            if(checkCollision(robots[i].hitBoxRight, testBox[a].hitBox, 1))
                            {
                                   robots[i].rightFalling = false;
                            }
                        }
                    }
                }
                                                                                // why is this out of place
                /*if(checkCollision(robots[0].hitBox, ground2.hitBox, 3) == 1)///collision for ROBOT with the groundses and the walls
                {
                    robots[0].hitBox->y = ground2.hitBox->y - robots[0].hitBox->h +0;
                    robots[0].onFlatSurface = true;
                }

                if(checkCollision(robots[0].hitBoxLeft, ground2.hitBox, 1) == 1)
                {
                    robots[0].leftFalling = false;
                }

                if(checkCollision(robots[0].hitBoxRight, ground2.hitBox, 1) == 1)
                {
                    robots[0].rightFalling = false;
                }*/

                for(int i = 0; i < numberOfRobots; i++) ///robots and testslopes
                {
                    for(int a = 0; a < NumberOfTestSlopes; a++)
                    {
                        //if(robots[i].alive)
                        //{
                            if(robots[i].hitBox->x + robots[i].hitBox->w / 2 > testSlopes[a].x1 && robots[i].hitBox->x + robots[i].hitBox->w/ 2 < testSlopes[a].x2)
                            {
                                if(robots[i].hitBox->y + robots[i].hitBox->h > (testSlopes[a].y1 - (robots[i].hitBox->h / 2)) - ((robots[i].hitBox->x - testSlopes[a].x1) / testSlopes[a].slope) + 1  && (robots[i].hitBox->y + robots[i].hitBox->h < (testSlopes[a].y1 - (robots[i].hitBox->w / 2)) - ((robots[i].hitBox->x - testSlopes[a].x1) / testSlopes[a].slope) + 85) )  ///this is complicated
                                {
                                    robots[i].hitBox->y = (testSlopes[a].y1 - (robots[i].hitBox->h / 2)) - ((robots[i].hitBox->x - testSlopes[a].x1) / testSlopes[a].slope) - robots[i].hitBox->h + 29;
                                    robots[i].onFlatSurface = true;
                                }
                            }
                        //}
                    }

                    ///make arms collide with slopes here lol
                }



                ///snowman collision
                                                                                                                       ///snowman collision

                if(snowman1.time2explode)
                {
                    if(snowman1.hitBoxNose->x > player1.hitBox->x + 30)
                    {
                        player1.testingLeft = true;
                        gPlayerSemi.setColor(255, 0, 0);
                        gPlayerArmed.setColor(255, 0, 0);
                    }
                    else
                    {
                        player1.testingRight = true;
                        gPlayerSemi.setColor(255, 0, 0);
                        gPlayerArmed.setColor(255, 0, 0);
                    }
                }


                if(checkCollision(snowman1.hitBoxNose, ground2.hitBox, 1) == 1 && snowman1.noseActive)///collision for snowman nose and the floor
                {
                    snowman1.goingDown = false;
                    gSnowmanNose.animationBool = false;
                    snowman1.noseActive = false;
                    snowman1.time2explode = true; ///explosion!!!
                    snowman1.rotation = 0;

                    snowman1.explosionTimer.start();
                }

                if(checkCollision(snowman1.hitBoxNose, player1.rectangleForTheHair, 1) == 1 && snowman1.noseActive)///collision for snowman nose and player hair
                {
                    snowman1.hitBoxNose->y = -9000;
                    snowman1.goingDown = false;
                    gSnowmanNose.animationBool = false;
                    snowman1.noseActive = false;
                    //snowman1.time2explode = true; ///explosion!!!
                    snowman1.rotation = 0;

                    snowman1.explosionTimer.start();

                    if(player1.clip < 15)
                    {
                        player1.clip++;
                    }
                    gHair.animationTimer1.start();Mix_PlayChannel( -1, aTvSize, 0 );
                }

                if(snowman1.hitBoxNose->y > ground2.hitBox->y +50 && snowman1.noseActive) ///too lazy to just make it explode after a certain amount of time after being alive so if it gets too low it explodes lol
                {
                    snowman1.goingDown = false;
                    gSnowmanNose.animationBool = false;
                    snowman1.noseActive = false;
                    snowman1.time2explode = true; ///explosion!!!
                    snowman1.rotation = 0;

                    snowman1.explosionTimer.start();
                }


                if(snowman1.alive)
                {
                    if(checkCollision(snowman1.hitBox, ground2.hitBox, 3) == 1)///collision for snowman with the groundses and the walls
                    {
                        snowman1.hitBox->y = ground2.hitBox->y - snowman1.hitBox->h +0;
                        snowman1.onFlatSurface = true;
                    }

                    for(int i = 0; i < 19; i++) ///collision for snowman body and player bullets
                    {
                        if(player1bullets[i].active)
                        {
                            if(checkCollision(player1bullets[i].hitBox, snowman1.hitBox, 1) == 1)
                            {
                                player1bullets[i].active = false;
                            }
                        }
                    }

                    if(checkCollision(camera, snowman1.hitBox, 1)) ///if snowman is in view of the camera
                    {
                       snowman1.openFire = true;
                    }
                    else
                    {
                        snowman1.openFire = false;
                    }

                    for(int i = 0; i < 19; i++) ///collision for snowman head and player bullets
                    {
                        if(player1bullets[i].active)
                        {
                            if(checkCollision(player1bullets[i].hitBox, snowman1.hitBoxHead, 1) == 1)
                            {
                                snowman1.alive = false;
                            }
                        }
                    }

                }                                                                                                                          ///end of snowman collision


                ///OXYGEN TANK COLLISION
                                                                                                                                                                                ///OXYGEN TANK COLLISION
                if(player1.hasGun && checkCollision(player1.hitBox, oxygentank.hitBox, 1) == 1)
                {
                    player1.nearOxygen = true;
                    player1.walkToX = oxygentank.xPos - 45;
                    player1.whatToDoWhenAtLocation = 1;
                }
                else
                {
                    player1.nearOxygen = false;
                }

                ///LEDGE GRAB COLLISION                                                                                 ///LEDGE GRAB COLLISION
                if(player1.hasGun && player1.gunDirection == 1)
                {
                    for(int i = 0; i < 2; i++)
                    {
                        if(checkCollision(player1.muzzleHitbox, ledges[i].hitBox, 1) == 1)
                        {
                            if(ledges[i].facingRight && player1.flip)
                            {
                                player1.velocity = ledges[i].hitBox->x - 27;
                                player1.yVelocity = ledges[i].hitBox->y + 42;
                                if(!player1.ledgeHanging){Mix_PlayChannel( -1, aledgeGrab, 0 ); };
                                player1.ledgeHanging = true;
                            }
                            else if(!ledges[i].facingRight && !player1.flip)
                            {
                                player1.velocity = ledges[i].hitBox->x + 12;
                                player1.yVelocity = ledges[i].hitBox->y + 42;
                                if(!player1.ledgeHanging){Mix_PlayChannel( -1, aledgeGrab, 0 ); };
                                player1.ledgeHanging = true;
                            }
                        }
                    }
                }

                ///GRANADO COLLISION with ground                                                                               ///GRANADO COLLISION



                gGranado.setColor(player1.granadoColor, 0, 0);


                if(checkCollision(player1.granadoHitbox, ground.hitBox, 3) == 1)
                {
                   player1.granadoVelocityY = ground.hitBox->y - player1.granadoHitbox->h -4;
                   player1.granadoOnFlatSurface = true;
                }

                if(checkCollision(player1.granadoHitbox, ground2.hitBox, 3) == 1)
                {
                    player1.granadoVelocityY = ground2.hitBox->y - player1.granadoHitbox->h -4;
                    player1.granadoOnFlatSurface = true;
                }

                if(checkCollision(player1.granadoHitbox, ground3.hitBox, 3) == 1)
                {
                    player1.granadoVelocityY = ground3.hitBox->y - player1.granadoHitbox->h -4;
                    player1.granadoOnFlatSurface = true;
                }


				for(int i = 0; i < NumberOfTestBoxes; i++) ///granado and testboxes?
                {
                    /*if(checkCollision(player1.granadoHitbox, testBox[i].hitBox, 3) == 1)
                    {
                        player1.granadoVelocityY = testBox[i].hitBox->y - player1.granadoHitbox->h -4;
                        player1.granadoOnFlatSurface = true;
                    }*/
                    switch ( checkCollision(player1.granadoHitbox, testBox[i].hitBox, 2) ) ///collision for granado and test boxes
                        {
                            case 1: player1.granadoVelocityY = testBox[i].hitBox->y - player1.granadoHitbox->h -4;
                                    player1.granadoOnFlatSurface = true;
                                break;                                                        //+1?
                            case 2:
                                    if(player1.granadoTimer.getTicks() < 3000 && player1.granadoRight)
                                    {
                                        player1.granadoContact = true;
                                    }
                                break;
                            case 3: if(player1.granadoTimer.getTicks() < 3000 && player1.granadoLeft)
                                    {
                                        player1.granadoContact = true;
                                    }
                                break;
                            case 4:
                                break;
                            case 5: //do nothing

                                break;
                        }
                }

                for(int i = 0; i < NumberOfTestSlopes; i++) ///granado and testslopes
                {
                    if(!player1.granadoExploding)
                    {
                        if(player1.granadoVelocityX + player1.granadoHitbox->w / 2 > testSlopes[i].x1 && player1.granadoVelocityX + player1.granadoHitbox->w / 2 < testSlopes[i].x2)
                        {
                            if(player1.granadoHitbox->y + player1.granadoHitbox->h > (testSlopes[i].y1 - (player1.granadoHitbox->h / 2)) - ((player1.granadoVelocityX - testSlopes[i].x1) / testSlopes[i].slope) + 1  && (player1.granadoHitbox->y + player1.granadoHitbox->h < (testSlopes[i].y1 - (player1.granadoHitbox->w / 2)) - ((player1.granadoVelocityX - testSlopes[i].x1) / testSlopes[i].slope) + 85) )  ///this is complicated
                            {                                                                                                                                                                                                                     /// i think 85 means 85 pixesl below it
                                player1.granadoVelocityY = (testSlopes[i].y1 - (player1.granadoHitbox->h / 2)) - ((player1.granadoVelocityX - testSlopes[i].x1) / testSlopes[i].slope) - player1.granadoHitbox->h + 0;
                                player1.granadoOnFlatSurface = true;
                                if(player1.granadoLeft)
                                {
                                    player1.granadoSpeed = player1.granadoSpeed + 3;
                                }
                                if(player1.granadoRight)
                                {
                                    player1.granadoSpeed = player1.granadoSpeed - 3;
                                }
                            }
                        }
                    }
                }

                ///GRANADO COLLISION WITH PLAYER
                if(player1.granadoOnFlatSurface)  ///i should probably just make it add +100 to the speed and cap the speed at 100, and only check for if player is goinleft or goinright to decide where grenade is kicked
                {
                    if(player1.granadoRight)
                    {
                        if(checkCollision(player1.granadoHitbox, player1.hitBox, 1) == 1)
                        {
                            if(player1.hitBox->x < player1.granadoHitbox->x)
                            {
                                player1.granadoSpeed = player1.granadoSpeed + 55;
                            }
                            else
                            {
                                player1.granadoSpeed = player1.granadoSpeed + 55;
                                player1.granadoLeft = true;
                                player1.granadoRight = false;
                            }
                        }
                    }
                    else
                    {
                        if(checkCollision(player1.granadoHitbox, player1.hitBox, 1) == 1)
                        {
                            if(player1.hitBox->x < player1.granadoHitbox->x)
                            {
                                player1.granadoSpeed = player1.granadoSpeed + 55;
                                player1.granadoLeft = false;
                                player1.granadoRight = true;
                            }
                            else
                            {
                                player1.granadoSpeed = player1.granadoSpeed + 55;
                            }
                        }
                    }
                }

                /// collision for granado and robot
                for(int i = 0; i < numberOfRobots; i++)
                {
                    if(robots[i].health > 0)
                    {
                        if(checkCollision(player1.granadoHitbox, robots[i].hitBox, 1) == 1)
                        {
                            robots[i].health = 0;

                            player1.granadoContact = true;
                        }
                    }
                }

                /// grenade jumping shit
                if(player1.granadoExploding)
                {

                        if(checkCollision(player1.granadoHitbox, player1.hitBox, 1) == 1)
                        {
                            if(!player1.onFlatSurface)
                            {
                                player1.blastJumping = true;
                            }

                            if(player1.hitBox->x > player1.granadoHitbox->x + player1.granadoHitbox->w - 30)
                            {
                                player1.blastJumpingRight = true;
                                player1.blastJumpSpeed = 80;
                            }
                            else if(player1.hitBox->x + player1.hitBox->w < player1.granadoHitbox->x + 30)
                            {
                                player1.blastJumpingLeft = true;
                                player1.blastJumpSpeed = 80;
                            }
                        }

                }

                ///COLLISION FOR ARMORED TRUCK                              ARMORED TRUCK


                /*for(int i = 0; i < numberOfTrucks; i++) ///collision for armored truck and ground
                {
                    trucks[i].onFlatSurface = false;
                    if(checkCollision(trucks[i].hitBoxBottom, testBox[5].hitBox, 3) == 1)
                    {
                        trucks[i].onFlatSurface = true;
                        trucks[i].yVelocity = testBox[5].hitBox->y - trucks[i].hitBoxBottom->h;
                    }///never becomes false
                }*/

                for(int i = 0; i < numberOfTrucks; i++) ///collision for armored truck and testboxes
                {
                    trucks[i].onFlatSurface = false;
                    for(int a = 0; a < NumberOfTestBoxes; a++)
                    {
                        if(checkCollision(trucks[i].hitBoxBottom, testBox[a].hitBox, 3) == 1)
                        {
                            trucks[i].onFlatSurface = true;
                            trucks[i].yVelocity = testBox[a].hitBox->y - trucks[i].hitBoxBottom->h;
                            trucks[i].hitBoxTrigger2->y = trucks[i].yVelocity - 50;
                        }
                    }
                }

                for(int i = 0; i < numberOfTrucks; i++)
                {
                    switch ( checkCollision(player1.hitBox, trucks[i].hitBoxBottom, 2) ) ///collision for player and armored truck bottom
                        {
                            case 1: player1.yVelocity = trucks[i].hitBoxBottom->y - player1.hitBox->h + 1;
                                    player1.foo = true;
                                    if(player1.jumpTimer.getTicks() > 40) {player1.jumping = false; player1.jumpTimer.stop(); }
                                break;                                                        //+1?
                            case 2: player1.velocity = trucks[i].hitBoxBottom->x - player1.hitBox->w +1;
                                    player1.bar = true;
                                break;
                            case 3: player1.velocity = trucks[i].hitBoxBottom->x + trucks[i].hitBoxBottom->w -1;
                                    player1.baz = true;
                                break;
                            case 4: player1.yVelocity = trucks[i].hitBoxBottom->y + trucks[i].hitBoxBottom->h;
                                    player1.released = true;
                                break;
                            case 5: //do nothing

                                break;
                       // }
                    }
                }

                for(int i = 0; i < numberOfTrucks; i++)
                {
                    switch ( checkCollision(player1.hitBox, trucks[i].hitBoxTop, 2) ) ///collision for player and armored truck top
                        {
                            case 1: player1.yVelocity = trucks[i].hitBoxTop->y - player1.hitBox->h + 1;
                                    player1.foo = true;
                                    if(player1.jumpTimer.getTicks() > 40) {player1.jumping = false; player1.jumpTimer.stop(); }
                                break;                                                        //+1?
                            case 2: player1.velocity = trucks[i].hitBoxTop->x - player1.hitBox->w +1;
                                    player1.bar = true;
                                break;
                            case 3: player1.velocity = trucks[i].hitBoxTop->x + trucks[i].hitBoxTop->w -1;
                                    player1.baz = true;
                                break;
                            case 4: player1.yVelocity = trucks[i].hitBoxTop->y + trucks[i].hitBoxTop->h;
                                    player1.released = true;
                                break;
                            case 5: //do nothing

                                break;
                       // }
                    }
                }

                ///collision for armored truck and player bullets
                for(int p = 0; p < numberOfTrucks; p++)
                {
                    for(int i = 0; i < 19; i++)
                    {
                        if(player1bullets[i].active)
                        {
                            if(checkCollision(player1bullets[i].hitBox, trucks[p].hitBoxBottom, 1) == 1) ///for bottom of truck
                            {
                                if(trucks[p].health > 0)
                                {
                                    player1bullets[i].testing = true;
                                    if(!player1bullets[i].testTimer.isStarted())
                                    {
                                        if(player1bullets[i].direction == 2)
                                        {
                                            player1bullets[i].direction = 4;
                                        }
                                        else if(player1bullets[i].direction == 4)
                                        {
                                            player1bullets[i].direction = 2;
                                        }
                                    }
                                }
                                else
                                {
                                    player1bullets[i].active = false;
                                }
                            }
                        }
                    }
                }

                for(int p = 0; p < numberOfTrucks; p++)
                {
                    for(int i = 0; i < 19; i++)
                    {
                        if(player1bullets[i].active)
                        {
                            if(checkCollision(player1bullets[i].hitBox, trucks[p].hitBoxTop, 1) == 1) ///for top of truck
                            {
                                if(trucks[p].health > 0)
                                {
                                    player1bullets[i].testing = true;
                                    if(!player1bullets[i].testTimer.isStarted())
                                    {
                                        if(player1bullets[i].direction == 2)
                                        {
                                            player1bullets[i].direction = 4;
                                        }
                                        else if(player1bullets[i].direction == 4)
                                        {
                                            player1bullets[i].direction = 2;
                                        }
                                    }
                                }
                                else
                                {
                                    player1bullets[i].active = false;
                                }
                            }
                        }
                    }
                }

                for(int p = 0; p < numberOfTrucks; p++)
                {
                    for(int i = 0; i < 19; i++)
                    {
                        if(player1bullets[i].active)
                        {
                            if(checkCollision(player1bullets[i].hitBox, trucks[p].hitBoxGlass, 1) == 1) ///for truck glass
                            {
                                if(trucks[p].health > 0)
                                {
                                    switch(trucks[p].health)
                                    {
                                            case 1: Mix_PlayChannel( -1, aCrack3, 0 );
                                                    break;                                                        //+1?
                                            case 2: Mix_PlayChannel( -1, aCrack2, 0 );
                                                    break;
                                            case 3: Mix_PlayChannel( -1, aCrack1, 0 );
                                                    break;
                                    }
                                    trucks[p].health -= 1;
                                }
                                player1bullets[i].active = false;
                            }
                        }
                    }
                }

                ///collision for armored truck TRIGGER and player
                for(int i = 0; i < numberOfTrucks; i++)
                {
                    if(trucks[i].health > 0)
                    {
                        if(checkCollision(player1.hitBox, trucks[i].hitBoxTrigger, 1) == 1)
                        {
                            if(!trucks[i].triggeredTimer.isStarted())
                            {
                                trucks[i].triggeredTimer.start();
                            }
                            else if(trucks[i].triggeredTimer.getTicks() > 150 && trucks[i].triggeredTimer.getTicks() < 250)
                            {
                                player1.blastJumping = true;
                            }
                        }
                    }
                }

                ///collision for armored truck TRIGGER 2 and player
                for(int i = 0; i < numberOfTrucks; i++)
                {
                    if(trucks[i].health > 0)
                    {
                        if(player1.hitBox->x < trucks[i].hitBoxBottom->x - 40)
                        {
                            if(checkCollision(player1.hitBox, trucks[i].hitBoxTrigger2, 1) == 1)
                            {
                                if(!trucks[i].charging)
                                {
                                    trucks[i].charging = true;
                                }
                            }
                        }
                    }
                }

                ///collision for BULLET CAGE TRIGGER
                if(maggot.firstTry)
                {
                    if(checkCollision(player1.hitBox, bulletCageTrigger.hitBox, 1) == 1)
                    {
                        bulletCageTrigger.activated = true;
                        maggot.firstTry = false;

                        maggot.volleyTimer.start();
                        maggot.sequence = 1;
                        maggot.score = 0;
                        maggot.hitBox->x = 8254;
                        maggot.hitBox->y = -27;
                    }
                } ///UNCOMMENT THIS
                else
                {
                    bulletCageTrigger.activated = true;
                    if(maggot.sequence == 0 || maggot.sequence == 44)
                    {
                        if(!maggot.firstTry && !targetGame.funTimer.isStarted())
                        {
                             bulletCageTrigger.activated = false;
                        }
                    }
                }

                if(bulletCageTrigger.activated)
                {
                    testBox[13].hitBox->h = 400;
                    testBox[14].hitBox->h = 400;
                }
                else
                {
                    testBox[13].hitBox->h = 276;
                    testBox[14].hitBox->h = 276;
                }



                ///collision for LEVEL 2 TRIGGER
                if(maggot.sequence == 44 || maggot.sequence == 0)
                {
                    if(!targetGame.funTimer.isStarted() && !maggot.firstTry)
                    {
                       for(int i = 0; i < 19; i++)
                        {
                            if(player1bullets[i].active)
                            {
                                if(checkCollision(player1bullets[i].hitBox, level2Trigger.hitBox, 1) == 1)
                                {
                                    targetGame.funTimer.start();
                                    targetGame.twoSecCounter = 0;
                                    targetGame.score = 0;
                                    bulletCageTrigger.activated = true;
                                    player1.clip = 15;
                                    maggot.sequence = 0;
                                }
                            }
                        }
                    }
                }


                if(maggot.score == 40 && targetGame.score == 29)
                {
                    player1.doubleDuckingUnlocked = true;
                }

                ///collision for LEVEL 1 TRIGGER
                if(!targetGame.funTimer.isStarted() && !maggot.firstTry)
                {
                    if(maggot.sequence == 44 || maggot.sequence == 0)
                    {
                        for(int i = 0; i < 19; i++)
                        {
                            if(player1bullets[i].active)
                            {
                                if(checkCollision(player1bullets[i].hitBox, level1Trigger.hitBox, 1) == 1)
                                {
                                    maggot.volleyTimer.start();
                                    maggot.sequence = 1;
                                    maggot.score = 0;
                                    maggot.hitBox->x = 8254;
                                    maggot.hitBox->y = -27;
                                    /*if(!maggot.firstTry)
                                    {
                                       bulletCageTrigger.activated = true;
                                    }*/
                                }
                            }
                        }
                    }
                }

                ///collision for level 2 targets and player bullets
                for(int i = 0; i < 19; i++)
                {
                    if(player1bullets[i].active)
                    {
                        for(int p = 0; p < 15; p++)
                        {
                            if(targetGame.targets[p].alive)
                            {
                                if(checkCollision(player1bullets[i].hitBox, targetGame.targets[p].hitBox, 1) == 1)
                                {
                                    targetGame.targets[p].alive = false;
                                    targetGame.score += targetGame.targets[p].color;
                                    targetGame.targets[p].alpha = 255;

                                    /*if(player1bullets[i].direction == 4)
                                    {
                                        player1bullets[i].direction = 2;
                                    }
                                    else if(player1bullets[i].direction == 2)
                                    {
                                        player1bullets[i].direction = 4;
                                    }
                                    else if(player1bullets[i].direction == 1)
                                    {
                                        player1bullets[i].direction = 3;
                                    }*/
                                    if(targetGame.targets[p].color == 3)
                                    {
                                        Mix_PlayChannel( -1, aBreak3, 0 );
                                    }
                                    else if(targetGame.targets[p].color == 2)
                                    {
                                        Mix_PlayChannel( -1, aBreak2, 0 );
                                    }
                                    else
                                    {
                                        Mix_PlayChannel( -1, aBreak1, 0 );
                                    }

                                    player1bullets[i].active = false;

                                    //targetGame.targets[p].funTimerlol.start();
                                }
                            }
                        }
                    }
                }

                ///collision for ROBO MAGGOT and player bullets
                 for(int i = 0; i < 19; i++)
                {
                    if(player1bullets[i].active)
                    {
                        if(checkCollision(player1bullets[i].hitBox, maggot.hitBox, 1) == 1)
                        {
                            player1bullets[i].testing2 = true;
                        }
                    }
                }

                for(int i = 0; i < 19; i++)    ///collision for enemy bullets and testboxes i think
                {
                    if(enemyBullets[i].active)
                    {
                        for(int a = 0; a < NumberOfTestBoxes; a++)
                        {
                            if(checkCollision(enemyBullets[i].hitBox, testBox[a].hitBox, 1) == 1)
                            {
                                enemyBullets[i].active = false;
                            }
                        }
                    }
                }

                for(int i = 0; i < numberOfSharkRobots; i++) ///collision for shark robots and test boxes
                {
                    for(int a = 0; a < NumberOfTestBoxes; a++)
                    {
                        switch ( checkCollision(sharkRobots[i].hitBox, testBox[a].hitBox, 2) )
                        {
                            case 1: //floor
                                    if(sharkRobots[i].health > 0)
                                    {
                                        sharkRobots[i].jumpTimer.start();
                                    }
                                    else
                                    {
                                        sharkRobots[i].dead = true;
                                    }

                                break;                                                        //+1?
                            case 2: //hitting left wall
                                break;
                            case 3: //hitting right wall
                                    sharkRobots[i].health = 0;
                                break;
                            case 4:
                                    //ceiling
                                break;
                            case 5: //do nothing
                                break;
                        }
                    }
                }

                for(int i = 0; i < numberOfSharkRobots; i++) ///collision for shark robots and player
                {
                    if(checkCollision(sharkRobots[i].hitBox, player1.hitBox, 1) == 1)
                    {
                        player1.testingLeft = true;
                        player1.damageColor.start();
                        player1.health--;
                    }
                }

                for(int i = 0; i < 19; i++)    ///collision for player bullets and shark robots
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < numberOfSharkRobots; a++)
                        {
                            if(!sharkRobots[a].dead)
                            {
                                if(checkCollision(sharkRobots[a].hitBox, player1bullets[i].hitBox, 1) == 1)
                                {
                                    player1bullets[i].active = false;
                                    sharkRobots[a].jumpTimer.start();
                                    sharkRobots[a].beingShotInTheFaceTimer.start();
                                    sharkRobots[a].health--;
                                }
                            }
                        }
                    }
                }

                for(int i = 0; i < 19; i++)    ///collision for player bullets and kultentas
                {
                    if(player1bullets[i].active && !player1bullets[i].following)
                    {
                        for(int a = 0; a < numberOfKultentas; a++)
                        {
                            if(!kultentas[a].dead)
                            {
                                if(checkCollision(kultentas[a].hitBox, player1bullets[i].hitBox, 1) == 1)
                                {
                                    player1bullets[i].deletionTimer.start();
                                    kultentas[a].rootyTootyTimetoShootyTimer.start(); /// test

                                    if(!kultentas[a].storage1Full)
                                    {
                                        kultentas[a].replaceTimer1.start();
                                        kultentas[a].storage1Full = true;
                                        player1bullets[i].jellySpeed = 300;
                                        player1bullets[i].following = true;
                                        player1bullets[i].followingX = kultentas[a].bulletStorage1.x;
                                        player1bullets[i].followingY = kultentas[a].bulletStorage1.y;
                                    }
                                    else if(!kultentas[a].storage2Full)
                                    {
                                        kultentas[a].replaceTimer2.start();
                                        kultentas[a].storage2Full = true;
                                        player1bullets[i].jellySpeed = 300;
                                        player1bullets[i].following = true;
                                        player1bullets[i].followingX = kultentas[a].bulletStorage2.x;
                                        player1bullets[i].followingY = kultentas[a].bulletStorage2.y;
                                    }
                                    else if(!kultentas[a].storage3Full)
                                    {
                                        kultentas[a].replaceTimer3.start();
                                        kultentas[a].storage3Full = true;
                                        player1bullets[i].jellySpeed = 300;
                                        player1bullets[i].following = true;
                                        player1bullets[i].followingX = kultentas[a].bulletStorage3.x;
                                        player1bullets[i].followingY = kultentas[a].bulletStorage3.y;
                                    }
                                    else if(!kultentas[a].storage4Full)
                                    {
                                        kultentas[a].replaceTimer4.start();
                                        kultentas[a].storage4Full = true;
                                        player1bullets[i].jellySpeed = 300;
                                        player1bullets[i].following = true;
                                        player1bullets[i].followingX = kultentas[a].bulletStorage4.x;
                                        player1bullets[i].followingY = kultentas[a].bulletStorage4.y;
                                    }
                                    else if(!kultentas[a].storage5Full)
                                    {
                                        kultentas[a].replaceTimer5.start();
                                        kultentas[a].storage5Full = true;
                                        player1bullets[i].jellySpeed = 300;
                                        player1bullets[i].following = true;
                                        player1bullets[i].followingX = kultentas[a].bulletStorage5.x;
                                        player1bullets[i].followingY = kultentas[a].bulletStorage5.y;
                                    }
                                    else if(kultentas[a].storage1Full && kultentas[a].storage2Full && kultentas[a].storage3Full && kultentas[a].storage4Full && kultentas[a].storage5Full)
                                    {
                                        if(kultentas[a].imadethisbooltosaveoncpu == false)
                                        {
                                            /*kultentas[a].dead = true;
                                            kultentas[a].hitBox->x += 17;
                                            kultentas[a].hitBox->y -= 19;
                                            kultentas[a].beingDeadTimer.start();*/
                                            kultentas[a].imadethisbooltosaveoncpu = true;
                                            kultentas[a].hitColor.start();
                                            Mix_PlayChannel( -1, aJellyDeath1, 0 );
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                for(int a = 0; a < numberOfKultentas; a++) ///collision for kultenta bullets and player hair
                {
                    for(int i = 0; i < 8; i++)
                    {
                        if(kultentas[a].bulletactive[i])
                        {
                            if(checkCollision(kultentas[a].bullets[i], player1.rectangleForTheHair, 1) == 1)
                            {
                                if(player1.clip < 15)
                                {
                                    player1.clip++;
                                }
                                gHair.animationTimer1.start();Mix_PlayChannel( -1, aTvSize, 0 );
                                kultentas[a].bulletactive[i] = false;
                            }
                        }
                    }
                }

                for(int a = 0; a < numberOfKultentas; a++) ///collision for kultenta bullets and player
                {
                        for(int i = 0; i < 8; i++)
                        {
                            if(kultentas[a].bulletactive[i])
                            {
                                if(checkCollision(kultentas[a].bullets[i], player1.hitBox, 1) == 1)
                                {
                                    player1.testingLeft = true;
                                    player1.damageColor.start();
                                    player1.health--;
                                    kultentas[a].bulletactive[i] = false;
                                }
                            }
                        }
                }

                for(int a = 0; a < numberOfKultentas; a++) ///collision for kultentas and test boxes
                {
                    if(kultentas[a].fallingToDEath)
                    {
                        for(int i = 0; i < NumberOfTestBoxes; i++)
                        {
                            if(checkCollision(kultentas[a].hitBox, testBox[i].hitBox, 1))
                            {
                                kultentas[a].fallingToDEath = false;
                            }
                        }
                    }
                }

                for(int i = 0; i < numberOfTNTgent; i++) ///collision for TNT gents and test boxes
                {
                    for(int a = 0; a < NumberOfTestBoxes; a++)
                    {
                        switch ( checkCollision(gents[i].hitBox, testBox[a].hitBox, 2) )
                        {
                            case 1: //floor
                                    gents[i].hitBox->y = testBox[a].hitBox->y - gents[i].hitBox->h + 1;
                                    gents[i].onFlatSurface = true;
                                break;                                                        //+1?
                            case 2: //hitting left wall
                                break;
                            case 3: //hitting right wall
                                break;
                            case 4:
                                    //ceiling
                                break;
                            case 5: //do nothing
                                break;
                        }
                    }
                }

                for(int i = 0; i < numberOfTNTgent; i++) ///collision for player and TNT gents trigger
                {
                    if(!gents[i].programTimer.isStarted() && gents[i].health > 0)
                    {
                        if(checkCollision(gents[i].trigger, player1.hitBox, 1) == 1)
                        {
                            /*gents[i].programTimer.start();
                            gents[i].sequence = rand() % 2 + 1;
                            gents[i].check = 1;*/

                            /*if(!gents[i].speakingTimer.isStarted() && gents[i].speakingFrame == 0)
                            {
                                gents[i].speakingTimer.start();
                                Mix_PlayChannel( -1, aTNTgent_alert, 0 );
                            }*/
                                gents[i].programTimer.start();
                                gents[i].sequence = rand() % 3 + 1;
                                gents[i].check = 1;
                                if(!gents[i].speakingTimer.isStarted())
                                {
                                    gents[i].speakingFrame = 0;
                                    gents[i].speakingTimer.start();
                                    Mix_PlayChannel( -1, aTNTgent_alert, 0 );
                                }
                        }
                    }

                    /*if(checkCollision(gents[i].trigger, player1.hitBox, 1) == 1)
                    {
                        if(!gents[i].speakingTimer.isStarted() && gents[i].speakingFrame == 0)
                        {
                            gents[i].speakingTimer.start();
                            Mix_PlayChannel( -1, aTNTgent_alert, 0 );
                        }
                    }*/
                    if(gents[i].bullshitsoundboolno1352 == false)
                    {
                        if(gents[i].animation == 5)
                        {
                            Mix_PlayChannel( -1, aTNTgent_toss, 0 );
                            gents[i].bullshitsoundboolno1352 = true;
                        }
                    }
                }



                for(int i = 0; i < 19; i++)    ///collision for player bullets and tnt gent head
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < numberOfTNTgent; a++)
                        {
                            if(gents[a].headThrown == 2 || gents[a].health < 1)
                            {

                            }
                            else
                            {
                                if(checkCollision(gents[a].hitBoxHead, player1bullets[i].hitBox, 1) == 1)
                                {
                                    player1bullets[i].active = false;
                                    gents[a].hitColorHead.start();
                                    gents[a].health--;
                                    if(gents[a].health == 0)
                                    {
                                        Mix_PlayChannel( -1, aExplosion, 0 );
                                        gents[a].explosionTimer.start();
                                    }
                                }
                            }
                        }
                    }
                }

                for(int i = 0; i < 19; i++)    ///collision for player bullets and tnt gent body
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < numberOfTNTgent; a++)
                        {
                            if(!gents[a].dead && gents[a].health > 0)
                            {
                                if(checkCollision(gents[a].hitBox, player1bullets[i].hitBox, 1) == 1)
                                {
                                    player1bullets[i].testing = true;
                                    if(!player1bullets[i].testTimer.isStarted())
                                    {
                                        if(player1bullets[i].direction == 2)
                                        {
                                            player1bullets[i].direction = 4;
                                        }
                                        else if(player1bullets[i].direction == 4)
                                        {
                                            player1bullets[i].direction = 2;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                for(int a = 0; a < numberOfTNTgent; a++) ///collision for TNTgent bullets and player hair
                {
                    for(int i = 0; i < 5; i++)
                    {
                        if(gents[a].bulletactive[i])
                        {
                            if(checkCollision(gents[a].bullets[i], player1.rectangleForTheHair, 1) == 1)
                            {
                                if(player1.clip < 15)
                                {
                                    player1.clip++;
                                }
                                gHair.animationTimer1.start();Mix_PlayChannel( -1, aTvSize, 0 );
                                gents[a].bulletactive[i] = false;
                            }
                        }
                    }
                }

                for(int a = 0; a < numberOfTNTgent; a++) ///collision for TNTgent bullets and player
                {
                    for(int i = 0; i < 5; i++)
                    {
                        if(gents[a].bulletactive[i])
                        {
                            if(checkCollision(gents[a].bullets[i], player1.hitBox, 1) == 1)
                            {
                                player1.testingLeft = true;
                                player1.damageColor.start();
                                player1.health--;
                                gents[a].bulletactive[i] = false;
                            }
                        }
                    }
                }

                for(int i = 0; i < numberOfTNTgent; i++) /// thrown TNT gent head and testboxes
                {
                    if(gents[i].headThrown == 1 && gents[i].health > 0)
                    {
                        for(int a = 0; a < NumberOfTestBoxes; a++)
                        {
                            if( checkCollision(gents[i].hitBoxHead, testBox[a].hitBox, 1) )
                            {
                                Mix_PlayChannel( -1, aExplosion, 0 );
                                gents[i].headThrown = 2;
                                gents[i].exploded = true;
                                gents[i].explosionTimer.start();
                            }
                        }
                    }
                }

                for(int a = 0; a < numberOfTNTgent; a++) ///collision for TNT gent explosion and player
                {
                    if(gents[a].explosionTimer.isStarted())
                    {
                        if(checkCollision(gents[a].explosionHitbox, player1.hitBox, 1) == 1)
                        {
                            player1.blastJumping = true;

                            if(player1.hitBox->x > gents[a].hitBoxHead->x + gents[a].hitBoxHead->w)
                            {
                                player1.blastJumpingRight = true;
                                player1.blastJumpSpeed = 80;
                            }
                            else if(player1.hitBox->x + player1.hitBox->w < gents[a].hitBoxHead->x)
                            {
                                player1.blastJumpingLeft = true;
                                player1.blastJumpSpeed = 80;
                            }
                        }
                    }
                }

                ///WERMBL COLLISION
                for(int i = 0; i < numberOfWermbls; i++) ///collision with wermbls and testboxes
                {
                    for(int a = 0; a < NumberOfTestBoxes; a++)
                    {
                        switch ( checkCollision(wermbls[i].hitBox, testBox[a].hitBox, 2) )
                        {
                            case 1:     if(!wermbls[i].flying)
                                        {
                                            wermbls[i].hitBox->y = testBox[a].hitBox->y - wermbls[i].hitBox->h -0;
                                            wermbls[i].onFlatSurface = true;
                                        }
                                break;
                            case 2: //hitting left wall
                                    if(wermbls[i].walkingRight)
                                    {
                                        wermbls[i].flip = SDL_FLIP_NONE;
                                        wermbls[i].walking = true;
                                        wermbls[i].walkingRight = false;
                                    }
                                break;
                            case 3: //hitting right wall
                                    if(wermbls[i].walking)
                                    {
                                        wermbls[i].flip = SDL_FLIP_HORIZONTAL;
                                        wermbls[i].walking = false;
                                        wermbls[i].walkingRight = true;
                                    }
                                break;
                            case 4: //wermbls should never hit a ceiling
                                break;
                            case 5: //do nothing

                                break;
                        }
                    }
                }

                for(int i = 0; i < 19; i++)    ///collision for player bullets and wermbl face
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < numberOfWermbls; a++)
                        {

                            if(checkCollision(wermbls[a].hitBoxHead, player1bullets[i].hitBox, 1) == 1)
                            {
                                if(wermbls[a].numberOfBulletsEaten < 3 && wermbls[a].gachAvtivationState == 0)
                                {
                                                                                    if(wermbls[a].numberOfBulletsEaten == 1)
                                                                                    {
                                                                                        Mix_PlayChannel( -1, aWermblTWO, 0 );
                                                                                    }
                                                                                    else if(wermbls[a].numberOfBulletsEaten == 2)
                                                                                    {
                                                                                        Mix_PlayChannel( -1, aWermblTHREE, 0 );
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                       Mix_PlayChannel( -1, aWermblONE, 0 );;
                                                                                    }

                                    player1bullets[i].active = false;
                                    wermbls[a].walking = false;
                                    wermbls[a].walkingRight = false;
                                    gWermbls[a].animationTimer4.start();
                                    gWermbls[a].animationTimer1.pause();
                                    wermbls[a].numberOfBulletsEaten++;
                                    wermbls[a].gachaActivationTimer.start();
                                }
                                else
                                {
                                    player1bullets[i].testing = true;
                                    if(!player1bullets[i].testTimer.isStarted())
                                    {
                                        if(player1bullets[i].direction == 2)
                                        {
                                            player1bullets[i].direction = 4;
                                        }
                                        else if(player1bullets[i].direction == 4)
                                        {
                                            player1bullets[i].direction = 2;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                for(int i = 0; i < 19; i++)    ///collision for player bullets and wermbl body
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < numberOfWermbls; a++)
                        {
                            if(checkCollision(wermbls[a].hitBox, player1bullets[i].hitBox, 1) == 1)
                            {
                                player1bullets[i].testing = true;
                                if(!player1bullets[i].testTimer.isStarted())
                                {
                                    if(player1bullets[i].direction == 2)
                                    {
                                        player1bullets[i].direction = 4;
                                    }
                                    else if(player1bullets[i].direction == 4)
                                    {
                                        player1bullets[i].direction = 2;
                                    }
                                }
                            }
                        }
                    }
                }

                for(int i = 0; i < numberOfWermbls; i++)
                {
                        switch ( checkCollision(player1.hitBox, wermbls[i].hitBox, 2) ) ///collision for player and wermbl body
                        {
                            case 1: player1.yVelocity = wermbls[i].hitBox->y - player1.hitBox->h + 1;
                                    player1.foo = true;
                                    if(wermbls[i].walking)
                                    {
                                        player1.velocity -=1;
                                    }
                                    if(player1.jumpTimer.getTicks() > 40) {player1.jumping = false; player1.jumpTimer.stop(); }

                                    if(!player1.ect)
                                    {
                                        wermbls[i].flyingUp = true;
                                    }
                                    wermbls[i].flyingSpeed = 20;
                                break;
                            case 2: player1.velocity = wermbls[i].hitBox->x - player1.hitBox->w +1;
                                    player1.bar = true;
                                    if(player1.baz)
                                    {
                                        wermbls[i].walking = false;
                                        wermbls[i].walkingRight = true;
                                        wermbls[i].flip = SDL_FLIP_HORIZONTAL;
                                    }
                                break;
                            case 3: player1.velocity = wermbls[i].hitBox->x + wermbls[i].hitBox->w -1;
                                    player1.baz = true;
                                    if(player1.bar)
                                    {
                                        wermbls[i].walking = true;
                                        wermbls[i].walkingRight = false;
                                        wermbls[i].flip = SDL_FLIP_NONE;
                                    }
                                break;
                            case 4:
                                break;
                            case 5: //do nothing

                                break;
                       // }
                    }

                }

                for(int a = 0; a < numberOfWermbls; a++) ///collision for wermbl bullets and player hair
                {
                    for(int i = 0; i < 3; i++)
                    {
                        if(wermbls[a].bulletactive[i])
                        {
                            if(checkCollision(wermbls[a].bullets[i], player1.rectangleForTheHair, 1) == 1)
                            {
                                if(player1.clip < 15)
                                {
                                    player1.clip++;
                                }
                                gHair.animationTimer1.start();Mix_PlayChannel( -1, aTvSize, 0 );
                                wermbls[a].bulletactive[i] = false;
                            }
                        }
                    }
                }

                for(int a = 0; a < numberOfWermbls; a++) ///collision for wermbl bullets and player
                {
                    for(int i = 0; i < 3; i++)
                    {
                        if(wermbls[a].bulletactive[i])
                        {
                            if(checkCollision(wermbls[a].bullets[i], player1.hitBox, 1) == 1)
                            {
                                player1.testingLeft = true;
                                player1.damageColor.start();
                                player1.health--;
                                wermbls[a].bulletactive[i] = false;
                            }
                        }
                    }
                }

                for(int a = 0; a < numberOfTNTgent; a++) ///collision for wermbl explosion and player
                {
                    if(wermbls[a].explosionSoundBoolUgh)
                    {
                        Mix_PlayChannel( -1, aExplosion1, 0 );
                    }

                    if(wermbls[a].explosionTimer.isStarted())
                    {
                        if(checkCollision(wermbls[a].explosionHitbox, player1.hitBox, 1) == 1)
                        {
                            player1.blastJumping = true;

                            if(player1.hitBox->x > wermbls[a].hitBox->x + wermbls[a].hitBox->w)
                            {
                                player1.blastJumpingRight = true;
                                player1.blastJumpSpeed = 80;
                            }
                            else if(player1.hitBox->x + player1.hitBox->w < wermbls[a].hitBox->x)
                            {
                                player1.blastJumpingLeft = true;
                                player1.blastJumpSpeed = 80;
                            }
                        }
                    }
                }

                ///GARBAGE GAURDIAN COLLISION
                for(int i = 0; i < numberOfGarbageGaurdians; i++) ///collision with garbage gaurdians and testboxes
                {
                    for(int a = 0; a < NumberOfTestBoxes; a++)
                    {
                        if( checkCollision(garbageGaurdians[i].hitBox, testBox[a].hitBox, 1) )
                        {
                            garbageGaurdians[i].onFlatSurface = true;
                        }
                    }
                }

                for(int i = 0; i < numberOfGarbageGaurdians; i++)
                {
                        switch ( checkCollision(player1.hitBox, garbageGaurdians[i].hitBox, 2) ) ///collision for player and garbage gaurdian can
                        {
                            case 1: player1.yVelocity = garbageGaurdians[i].hitBox->y - player1.hitBox->h + 1;
                                    player1.foo = true;
                                    if(player1.jumpTimer.getTicks() > 40) {player1.jumping = false; player1.jumpTimer.stop(); }
                                break;
                            case 2: player1.velocity = garbageGaurdians[i].hitBox->x - player1.hitBox->w +1;
                                    player1.bar = true;
                                break;
                            case 3: player1.velocity = garbageGaurdians[i].hitBox->x + garbageGaurdians[i].hitBox->w -1;
                                    player1.baz = true;
                                break;
                            case 4:
                                break;
                            case 5: //do nothing

                                break;
                    }
                }

                for(int i = 0; i < 19; i++)    ///collision for player bullets and garbage gaurdian can
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < numberOfGarbageGaurdians; a++)
                        {
                            if(checkCollision(garbageGaurdians[a].hitBox, player1bullets[i].hitBox, 1) == 1)
                            {
                                player1bullets[i].testing = true;
                                if(!player1bullets[i].testTimer.isStarted())
                                {
                                    if(player1bullets[i].direction == 2)
                                    {
                                        player1bullets[i].direction = 4;
                                    }
                                    else if(player1bullets[i].direction == 4)
                                    {
                                        player1bullets[i].direction = 2;
                                    }
                                }
                            }
                        }
                    }
                }

                for(int i = 0; i < numberOfGarbageGaurdians; i++)
                {
                    if(garbageGaurdians[i].shieldFlying)
                    {
                        switch ( checkCollision(player1.hitBox, garbageGaurdians[i].shieldHitbox, 2) ) ///collision for player and garbage gaurdian shield
                        {
                            case 1: player1.yVelocity = garbageGaurdians[i].shieldHitbox->y - player1.hitBox->h + 1;
                                    player1.foo = true;
                                    if(player1.jumpTimer.getTicks() > 40) {player1.jumping = false; player1.jumpTimer.stop(); }

                                    player1.velocity -= 7;
                                break;
                            case 2: player1.velocity = garbageGaurdians[i].shieldHitbox->x - player1.hitBox->w +1;
                                    player1.bar = true;
                                    garbageGaurdians[i].shieldHealth--;
                                    player1.damageColor.start();
                                    player1.health--;
                                    player1.testingLeft = true;
                                break;
                            case 3: player1.velocity = garbageGaurdians[i].shieldHitbox->x + garbageGaurdians[i].shieldHitbox->w -1;
                                    player1.baz = true;
                                    garbageGaurdians[i].shieldHealth--;
                                    player1.damageColor.start();
                                    player1.health--;
                                    player1.testingLeft = true;
                                break;
                            case 4:
                                    ///you should prolly never be able to jump into it
                                break;
                            case 5: //do nothing

                                break;
                        }
                    }
                }

                for(int i = 0; i < numberOfGarbageGaurdians; i++) ///collision with shield and player hair
                {
                    if(garbageGaurdians[i].shieldFlying && !player1.gunBouncing)
                    {
                        if( checkCollision(garbageGaurdians[i].shieldHitbox, player1.rectangleForTheHair, 1) )
                        {
                            garbageGaurdians[i].shieldHealth--;
                            player1.damageColor.start();
                            player1.health--;
                            player1.testingLeft = true;
                            ///crash
                        }
                    }
                }

                for(int i = 0; i < numberOfGarbageGaurdians; i++) ///collision with garbage gaurdian sword and testboxes
                {
                    for(int a = 0; a < NumberOfTestBoxes; a++)
                    {
                        if( checkCollision(garbageGaurdians[i].swordHitbox, testBox[a].hitBox, 1) )
                        {
                            garbageGaurdians[i].swordFlying = false;
                            garbageGaurdians[i].swordInGround = true;
                        }
                    }
                }

                for(int i = 0; i < numberOfGarbageGaurdians; i++) ///collision with garbage gaurdian sword and player
                {
                    if(garbageGaurdians[i].swordFlying)
                    {
                        if( checkCollision(garbageGaurdians[i].swordHitbox, player1.hitBox, 1) )
                        {
                            player1.damageColor.start();
                            player1.health--;
                            player1.testingLeft = true;
                        }
                    }
                }

                for(int i = 0; i < numberOfGarbageGaurdians; i++)
                {
                    if(garbageGaurdians[i].swordInGround)
                    {
                        if(player1.hitBox->x + player1.hitBox->w / 2 > garbageGaurdians[i].swordRamp.x1 && player1.velocity + player1.hitBox->w / 2 < garbageGaurdians[i].swordRamp.x2) ///collision for player and sword ramp
                        {
                            if(player1.hitBox->y + player1.hitBox->h > (garbageGaurdians[i].swordRamp.y1 - 15) - ((player1.hitBox->x - garbageGaurdians[i].swordRamp.x1) / garbageGaurdians[i].swordRamp.slope) + 1  && (player1.hitBox->y + player1.hitBox->h < (garbageGaurdians[i].swordRamp.y1 - 15) - ((player1.hitBox->x - garbageGaurdians[i].swordRamp.x1) / garbageGaurdians[i].swordRamp.slope) + 85) )  ///this is complicated
                            {                                                                                                                                                                                                                     /// i think 85 means 85 pixesl below it
                                player1.yVelocity = (garbageGaurdians[i].swordRamp.y1 - 15) - ((player1.hitBox->x - garbageGaurdians[i].swordRamp.x1) / garbageGaurdians[i].swordRamp.slope) - player1.hitBox->h + 11;
                                player1.foo = true;
                                if(player1.released) { player1.jumping = false;} //uhhh i dont think this does anything anymore but iduno
                            }
                        }
                    }
                }

                for(int i = 0; i < 19; i++)    ///collision for player bullets and garbage gaurdian body
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < numberOfGarbageGaurdians; a++)
                        {
                            if(garbageGaurdians[a].state > 1 && garbageGaurdians[a].health > 0)
                            {
                                if(checkCollision(garbageGaurdians[a].hitBoxBody, player1bullets[i].hitBox, 1) == 1)
                                {
                                    garbageGaurdians[a].hitColorBody.start();
                                    garbageGaurdians[a].health--;

                                    player1bullets[i].active = false;
                                }
                            }
                        }
                    }
                }

                for(int i = 0; i < 19; i++)    ///collision for player bullets and garbage gaurdian shield
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < numberOfGarbageGaurdians; a++)
                        {
                            if(garbageGaurdians[a].state > 1) //&& garbageGaurdians[a].health > 0) uncomment this l8r
                            {
                                if(checkCollision(garbageGaurdians[a].shieldHitbox, player1bullets[i].hitBox, 1) == 1)
                                {
                                    player1bullets[i].testing = true;
                                    if(!player1bullets[i].testTimer.isStarted())
                                    {
                                        if(player1bullets[i].direction == 2)
                                        {
                                            player1bullets[i].direction = 4;
                                        }
                                        else if(player1bullets[i].direction == 4)
                                        {
                                            player1bullets[i].direction = 2;
                                        }
                                    }
                                }
                            }
                            else if(garbageGaurdians[a].shieldFlying)
                            {
                                garbageGaurdians[a].shieldHealth--;
                            }
                        }
                    }
                }

                ///KONGA LEE COLLISION
                for(int i = 0; i < numberOfKongaLees; i++) ///collision with konga lees and testboxes
                {
                     if(kongaLees[i].freeFall)
                     {
                        for(int a = 0; a < NumberOfTestBoxes; a++)
                        {
                            if( checkCollision(kongaLees[i].hitBoxBody, testBox[a].hitBox, 1) )
                            {
                                kongaLees[i].freeFall = false;
                                kongaLees[i].onFloorGoingLeft = true;
                                kongaLees[i].footHasDetectedASurface = true;
                            }
                        }
                    }
                }

                for(int i = 0; i < numberOfKongaLees; i++) ///collision with konga lees feets and testboxes
                {
                    for(int a = 0; a < NumberOfTestBoxes; a++)
                    {
                        if( checkCollision(kongaLees[i].hitBoxFoot, testBox[a].hitBox, 1) )
                        {
                            kongaLees[i].footHasDetectedASurface = true;
                        }
                    }
                }

                for(int i = 0; i < numberOfKongaLees; i++) ///collision with konga lees face and testboxes
                {
                    for(int a = 0; a < NumberOfTestBoxes; a++)
                    {
                        if( checkCollision(kongaLees[i].hitBoxFace, testBox[a].hitBox, 1) )
                        {
                            kongaLees[i].faceHasDetectedASurface = true;
                        }
                    }
                }

                for(int i = 0; i < 19; i++)    ///collision for player bullets and KongaLee
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < numberOfKongaLees; a++)
                        {
                            if(checkCollision(kongaLees[a].hitBoxBody, player1bullets[i].hitBox, 1) == 1)
                            {
                                switch ( kongaLees[a].variant )
                                {
                                    case 0: ///banana konga lee
                                            if(kongaLees[a].alive)
                                            {
                                                kongaLees[a].spitting = true;
                                                kongaLees[a].alive = false;
                                                player1bullets[i].active = false;
                                                kongaLees[a].shootTimer.start();
                                                kongaLees[a].executingTurn_GOINGLEFT_ONFLOOR_TO_LEFTWALL = false;
                                                kongaLees[a].executingTurn_GOINGDOWN_ONLEFTWALL_TO_CEILING = false;
                                                kongaLees[a].executingTurn_GOINGRIGHT_ONCEILING_TO_RIGHTWALL = false;
                                                kongaLees[a].executingTurn_GOINGUP_ONRIGHTWALL_TO_FLOOR = false;

                                                kongaLees[a].executingTurn_GOINGLEFT_ONFLOOR_TO_RIGHTWALL = false;
                                                kongaLees[a].executingTurn_GOINGUP_ONRIGHTWALL_TO_CEILING = false;
                                                kongaLees[a].executingTurn_GOINGRIGHT_ONCEILING_TO_LEFTWALL = false;
                                                kongaLees[a].executingTurn_GOINGDOWN_ONLEFTWALL_TO_FLOOR = false;
                                            }
                                        break;
                                    case 1: ///strong konga lee
                                            player1bullets[i].testing = true;
                                            if(!player1bullets[i].testTimer.isStarted())
                                            {
                                                if(player1bullets[i].direction == 2)
                                                {
                                                    player1bullets[i].direction = 4;
                                                }
                                                else if(player1bullets[i].direction == 4)
                                                {
                                                    player1bullets[i].direction = 2;
                                                }
                                            }
                                        break;
                                    case 2: ///bomba konga lee
                                            if(kongaLees[a].alive)
                                            {
                                                kongaLees[a].exploded = true;
                                                kongaLees[a].alive = false;
                                                kongaLees[a].explosionTimer.start();
                                                Mix_PlayChannel( -1, aExplosion, 0 );
                                            }
                                        break;
                                }

                            }
                        }
                    }
                }


                for(int a = 0; a < numberOfKongaLees; a++) ///collision for konga lee bullets and player hair
                {
                    for(int i = 0; i < 3; i++)
                    {
                       if(kongaLees[a].bulletactive[i])
                        {
                            if(checkCollision(kongaLees[a].bullets[i], player1.rectangleForTheHair, 1) == 1)
                            {
                                if(player1.clip < 15)
                                {
                                    player1.clip++;
                                }
                                gHair.animationTimer1.start();Mix_PlayChannel( -1, aTvSize, 0 );
                                kongaLees[a].bulletactive[i] = false;
                            }
                       }
                    }
                }

                for(int a = 0; a < numberOfKongaLees; a++) ///collision for konga lee bullets and player body
                {
                    for(int i = 0; i < 3; i++)
                    {
                       if(kongaLees[a].bulletactive[i])
                        {
                            if(checkCollision(kongaLees[a].bullets[i], player1.hitBox, 1) == 1)
                            {
                                kongaLees[a].bulletactive[i] = false;
                                player1.damageColor.start();
                                player1.health--;
                            }
                       }
                    }
                }

                for(int a = 0; a < numberOfKongaLees; a++) ///collision for konga lee explosion and player
                {
                    if(kongaLees[a].explosionTimer.isStarted())
                    {
                        if(checkCollision(kongaLees[a].explosionHitbox, player1.hitBox, 1) == 1)
                        {
                            player1.blastJumping = true;

                            if(player1.hitBox->x > kongaLees[a].hitBoxBody->x + kongaLees[a].hitBoxBody->w + 10)
                            {
                                player1.blastJumpingRight = true;
                                player1.blastJumpSpeed = 80;
                            }
                            else if(player1.hitBox->x + player1.hitBox->w < kongaLees[a].hitBoxBody->x - 10)
                            {
                                player1.blastJumpingLeft = true;
                                player1.blastJumpSpeed = 80;
                            }
                        }
                    }
                }

                ///SORSE COLLISION DETECT

                for(int i = 0; i < numberOfSorse; i++) ///uhhhh decides which way the sorse faces
                {
                    if(sorse[i].alive && !sorse[i].summonAnimation.isStarted() && !sorse[i].attackAnimation.isStarted())
                    {
                        if(player1.hitBox->x + (player1.hitBox->w / 2) < sorse[i].hitBoxBody->x + (sorse[i].hitBoxBody->w / 2))
                        {
                            sorse[i].flip = SDL_FLIP_HORIZONTAL;
                        }
                        else
                        {
                            sorse[i].flip = SDL_FLIP_NONE;
                        }
                    }
                }

                player1.moonGravity = false; //move this to bottom of move
                for(int i = 0; i < numberOfSorse; i++) ///collision with sorse black hole and player
                {
                    if(sorse[i].blackHoleActive)
                    {
                        if(checkCollision(sorse[i].hitBoxBlackHole, player1.hitBox, 1) == 1)
                        {
                            if(player1.hitBox->x + (player1.hitBox->w / 2) < sorse[i].hitBoxBlackHole->x + (sorse[i].hitBoxBlackHole->w / 2))
                            {
                                if(player1.hitBox->x + (player1.hitBox->w / 2) < sorse[i].hitBoxBlackHole->x + (sorse[i].hitBoxBlackHole->w / 2) - 120)
                                {
                                    player1.velocity += 1;
                                    printf("a");
                                }
                                else if(player1.hitBox->x + (player1.hitBox->w / 2) < sorse[i].hitBoxBlackHole->x + (sorse[i].hitBoxBlackHole->w / 2) - 90)
                                {
                                    player1.velocity += 2;
                                    printf("b");
                                }
                                else if(player1.hitBox->x + (player1.hitBox->w / 2) < sorse[i].hitBoxBlackHole->x + (sorse[i].hitBoxBlackHole->w / 2) - 60)
                                {
                                    player1.velocity += 3;
                                    printf("c");
                                }
                                else if(player1.hitBox->x + (player1.hitBox->w / 2) < sorse[i].hitBoxBlackHole->x + (sorse[i].hitBoxBlackHole->w / 2) - 30)
                                {
                                    player1.velocity += 4;
                                    printf("d");
                                }
                                else
                                {
                                    player1.velocity += 4.5;
                                    printf("e");
                                }
                            }
                            else
                            {
                                if(player1.hitBox->x + (player1.hitBox->w / 2) > sorse[i].hitBoxBlackHole->x + (sorse[i].hitBoxBlackHole->w / 2) + 120)
                                {
                                    player1.velocity -= 1;
                                    printf("a");
                                }
                                else if(player1.hitBox->x + (player1.hitBox->w / 2) > sorse[i].hitBoxBlackHole->x + (sorse[i].hitBoxBlackHole->w / 2) + 90)
                                {
                                    player1.velocity -= 2;
                                    printf("b");
                                }
                                else if(player1.hitBox->x + (player1.hitBox->w / 2) > sorse[i].hitBoxBlackHole->x + (sorse[i].hitBoxBlackHole->w / 2) + 60)
                                {
                                    player1.velocity -= 3;
                                    printf("c");
                                }
                                else if(player1.hitBox->x + (player1.hitBox->w / 2) > sorse[i].hitBoxBlackHole->x + (sorse[i].hitBoxBlackHole->w / 2) + 30)
                                {
                                    player1.velocity -= 4;
                                    printf("d");
                                }
                                else
                                {
                                    player1.velocity -= 4.5;
                                    printf("e");
                                }
                            }
                            player1.moonGravity = true;
                        }
                    }
                }

                for(int i = 0; i < numberOfSorse; i++) ///collision with sorse trigger and player
                {
                    if(sorse[i].alive)
                    {
                        if(checkCollision(sorse[i].hitBoxTrigger, player1.hitBox, 1) == 1)
                        {
                            if(!sorse[i].blackHoleActive && !sorse[i].summonAnimation.isStarted() && !sorse[i].blackHoleFlying)
                            {
                                if(gSorse[i].animationBool)
                                {
                                    sorse[i].summonAnimation.start();
                                }
                            }

                            if(player1.velocity > sorse[i].hitBoxBody->x + sorse[i].hitBoxBody->w)
                            {
                                sorse[i].followRight = true;
                            }
                            else if(player1.velocity - player1.hitBox->w < sorse[i].hitBoxBody->x)
                            {
                                sorse[i].followLeft = true;
                            }
                        }
                    }
                }

                for(int i = 0; i < numberOfSorse; i++) ///collision with sorse feet and testboxes
                {
                    if(sorse[i].alive)
                    {
                        for(int a = 0; a < NumberOfTestBoxes; a++)
                        {
                            if( checkCollision(sorse[i].hitBoxFeet, testBox[a].hitBox, 1) )
                            {
                                sorse[i].onFlatSurface = true;
                            }
                        }
                    }
                }

                for(int i = 0; i < numberOfSorse; i++) ///collision with sorse feet and player
                {
                    if(sorse[i].alive)
                    {
                        if(checkCollision(sorse[i].hitBoxFeet, player1.hitBox, 1) == 1)
                        {
                            if(!sorse[i].attackAnimation.isStarted() && !sorse[i].summonAnimation.isStarted())
                            {
                                if(gSorse[i].animationBool)
                                {
                                    sorse[i].attackAnimation.start();
                                }
                            }
                        }
                    }
                }

                for(int a = 0; a < numberOfSorse; a++) ///collision for sorse explosion and player
                {
                    if(sorse[a].alive && sorse[a].exploding)
                    {
                        if(checkCollision(sorse[a].hitBoxExplosion, player1.hitBox, 1) == 1)
                        {
                            player1.blastJumping = true;
                            player1.damageColor.start();
                            player1.health--;

                            if(player1.hitBox->x + (player1.hitBox->w / 2) > sorse[a].hitBoxExplosion->x + (sorse[a].hitBoxExplosion->w / 2))
                            {
                                player1.blastJumpingRight = true;
                                player1.blastJumpSpeed = 80;
                            }
                            else if(player1.hitBox->x + (player1.hitBox->w / 2) < sorse[a].hitBoxExplosion->x + (sorse[a].hitBoxExplosion->w / 2))
                            {
                                player1.blastJumpingLeft = true;
                                player1.blastJumpSpeed = 80;
                            }
                        }
                    }
                }



                    for(int a = 0; a < numberOfSorse; a++)
                    {
                        if(sorse[a].alive)
                        {
                            switch(gSorse[a].animationInt) ///yup this is where im gonna actually freakin move the eyes around
                            {
                                case 0: sorse[a].hitBoxEyes->y = sorse[a].hitBoxBody->y + 76;
                                break;
                                case 1: sorse[a].hitBoxEyes->y = sorse[a].hitBoxBody->y + 45;
                                break;
                                case 2: sorse[a].hitBoxEyes->y = sorse[a].hitBoxBody->y + 18;
                                break;
                            }

                            for(int i = 0; i < 19; i++)    ///collision for player bullets and Sorse weakpoint
                            {
                                if(player1bullets[i].active)
                                {
                                    if(checkCollision(sorse[a].hitBoxEyes, player1bullets[i].hitBox, 1) == 1)
                                    {
                                        player1bullets[i].active = false;
                                        sorse[a].hitColor.start();
                                        sorse[a].health--;

                                        if(sorse[a].health < 1)
                                        {
                                            sorse[a].collapsing = true;
                                            sorse[a].hitBoxBody->h = 103;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    for(int i = 0; i < numberOfSorse; i++) ///collision for collappsing sorse body and testboxes
                    {
                        if(sorse[i].collapsing)
                        {
                            for(int a = 0; a < NumberOfTestBoxes; a++)
                            {
                                if( checkCollision(sorse[i].hitBoxBody, testBox[a].hitBox, 1) )
                                {
                                    sorse[i].collapsing = false;
                                }
                            }
                        }
                    }

                    for(int a = 0; a < numberOfSorse; a++)
                    {
                        if(sorse[a].blackHoleActive)
                        {
                            for(int i = 0; i < 19; i++)    ///collision for player bullets and Sorse weakpoint
                            {
                                if(player1bullets[i].active)
                                {
                                    if(checkCollision(sorse[a].hitBoxBlackHole, player1bullets[i].hitBox, 1) == 1)
                                    {
                                        player1bullets[i].inBlackHole = true;
                                        player1bullets[i].blackHoleX = sorse[a].blackHoleSuccer.x;
                                        player1bullets[i].blackHoleY = sorse[a].blackHoleSuccer.y;
                                    }
                                }
                            }
                        }
                        else
                        {
                            for(int i = 0; i < 19; i++)    ///collision for player bullets and Sorse weakpoint
                            {
                                if(player1bullets[i].active)
                                {
                                    if(checkCollision(sorse[a].hitBoxBlackHole, player1bullets[i].hitBox, 1) == 1)
                                    {
                                        player1bullets[i].inBlackHole = false;
                                    }
                                }
                            }
                        }
                    }

                    for(int a = 0; a < numberOfSorse; a++)
                    {
                        if(sorse[a].blackHoleActive && player1.granadoExploding)
                        {
                            if(checkCollision(sorse[a].hitBoxBlackHole, player1.granadoHitbox, 1) == 1)
                            {
                                sorse[a].blackHoleActive = false;
                            }
                        }
                    }

                ///DOOR COLLISION
                for(int i = 0; i < NumberOfDoors; i++) ///collisionw ith player and doors
                {
                    if(checkCollision(doors[i].hitBox, player1.hitBox, 1) == 1)
                    {
                        player1.nearItemButtonOrMachine = true;

                        if(player1.useItemButtonOrMachine)
                        {
                            if(doors[i].destinationX != 0)
                            {
                                player1.velocity = doors[i].destinationX;
                                player1.yVelocity = doors[i].destinationY;
                            }

                            switch(doors[i].IDN)
                            {
                                case 0:  //debug area

                                break;
                                case 1:  //level 1
                                         destination = 1;
                                         if(loadBGalpha == 0) {loadBGalpha = 2;}
                                         gLoadingBG.setBlendMode(SDL_BLENDMODE_BLEND);
                                         printf("Entering Level 1");
                                break;
                                case 2:  //tutorial

                                break;
                            }
                        }
                    }
                }

                ///SHADY ASS COLLISION CODE
                ////////////////////////////SHADY ASS COLLISION CODE
                ///SHADY ASS COLLISION CODE

                ///this is where i'll handle sounds i guess???
                Mix_AllocateChannels(16);
                aTvSize->volume = 10;
                aTeleMaggotLeft->volume = 50;
                aTeleMaggotRight->volume = 50;

                if(player1.ASSsoundMissile)
                {
                    Mix_PlayChannel( -1, aMissile, 0 );
                }

                if(player1.ASSsoundRising)
                {
                    Mix_PlayChannel( -1, aRising, 0 );
                }

                if(player1.ASSsoundLand)
                {
                    Mix_PlayChannel( -1, aLand, 0 );
                }

                if(snowman1.time2explode)
                {
                    Mix_PlayChannel( -1, aExplosion, 0 );
                }

                if(maggot.aTeleSound)
                {
                    if(maggot.direction == 2)
                    {
                        Mix_PlayChannel( -1, aTeleMaggotLeft, 0 );
                    }
                    else
                    {
                        Mix_PlayChannel( -1, aTeleMaggotRight, 0 );
                    }
                }

                if(!Mix_Playing(7))
                {
                    if(player1.reloadTimer.getTicks() > 400 && player1.reloadTimer.getTicks() < 1000)
                    {
                        Mix_PlayChannel( 7, aReload, 0 );
                    }

                    if(player1.gunTimer.getTicks() > 0)
                    {
                        if(player1.clip == 0)
                        {
                            Mix_PlayChannel( 7, aShootEmpty, 0 );
                        }
                        else
                        {
                            Mix_PlayChannel( 7, aShootSemi, 0 );
                        }
                    }
                }

                if(player1.time2explodeGranado)
                {
                    if(player1.granadoTimer.getTicks() > 3000)
                    {
                        Mix_PlayChannel( -1, aExplosion1, 0 );
                    }
                }

                if(player1.time2sizzle)
                {
                    Mix_PlayChannel( -1, aGranadoPrimed, 0 );
                }

                for(int i = 0; i < numberOfKultentas; i++)
                {
                    if(kultentas[i].gayAssSoundBool)
                    {
                        Mix_PlayChannel( -1, aJellyDeath2, 0 );
                    }
                }
                ///end of sound stuff






                //SDL_RenderDrawLine(window.gRenderer, testSlopes[0].x1 - camera->x , testSlopes[0].y1 - camera->y, testSlopes[0].x2 - camera->x , testSlopes[0].y2 - camera->y);  ///DA 2nd SLOPE



                //SDL_RenderDrawLine(window.gRenderer, 4626 - camera->x , 767 - camera->y, 4913 - camera->x , 732 - camera->y);  ///DA SLOPE

                if(player1.hitBox->x + player1.hitBox->w / 2 > 4626 && player1.velocity + player1.hitBox->w / 2 < 4913)
                {             ///middle of the player          x1
                                                              ///y1 - p.hitbox->w / 2 - (where you're at - x1? / slope)
                    if(player1.hitBox->y + player1.hitBox->h > (767 - 15) - ((player1.hitBox->x - 4626) / 8) + 1  && (player1.hitBox->y + player1.hitBox->h < (767 - 15) - ((player1.hitBox->x - 4626) / 8) + 85) )  ///this is complicated
                    {                                                                                                                                                                                                   //you're tellin me
                        player1.yVelocity = (767 - 15) - ((player1.hitBox->x - 4626) / 8) - player1.hitBox->h + 11;
                        player1.foo = true;
                        if(player1.released) { player1.jumping = false;} //if i don't put it here then u still jump for a frame and the screen shakes :(
                    }
                }

                player1.update();
                zipline1.update();
                for(int i = 0; i < numberOfRobots; i++)
                {
                   robots[i].update(gRobot[i].robotAnimation);
                }
                for(int i = 0; i < 19; i++)
                {
                    player1bullets[i].update();
                }

                maggot.update();
                for(int i = 0; i < 19; i++)
                {
                    enemyBullets[i].update();
                }

                for(int i = 0; i < numberOfSharkRobots; i++)
                {
                   sharkRobots[i].update();
                }

                for(int i = 0; i < numberOfKultentas; i++)
                {
                   kultentas[i].update();
                }

                for(int i = 0; i < numberOfTNTgent; i++)
                {
                   gents[i].update();
                }

                for(int i = 0; i < numberOfWermbls; i++)
                {
                   wermbls[i].update();
                }

                for(int i = 0; i < numberOfGarbageGaurdians; i++)
                {
                   garbageGaurdians[i].update();
                }

                for(int i = 0; i < numberOfKongaLees; i++)
                {
                   kongaLees[i].update();
                }

                for(int i = 0; i < numberOfSorse; i++)
                {
                   sorse[i].update();
                }

                camera->x = ( player1.hitBox->x + player1.hitBox->w / 2 ) - camera->w / 2;
               // if(!player1.crouched) {camera->y = ( player1.hitBox->y - player1.hitBox->h / 2 ) - camera->h / 2;}
                //else {camera->y = ( player1.hitBox->y - 30 - player1.hitBox->h / 2 ) - camera->h / 2;}


                if(!player1.fallen)
                {
                    if(camera->y != player1.hitBox->y)
                    {
                        camera->y = (camera->y + 0.20 * (player1.hitBox->y - camera->y)) - (window.SCREEN_HEIGHT / 9); ///so basically this is lerp
                    }
                }
                else
                {
                    if(camera->y != player1.hitBox->y)
                    {
                        camera->y = (camera->y + 0.20 * (player1.hitBox->y - camera->y)) - (window.SCREEN_HEIGHT / 9) - 10; ///so basically this is lerp
                    }
                }



                }
        accumulator -= dt;




        }

                //if(!player1.paused)
                if(1)
                {
				/******** rendering ********/
				SDL_SetRenderDrawColor( window.gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( window.gRenderer );

				gBackground.render(window.gRenderer, 0 , 0);
				gDemoDay.render(window.gRenderer, 30 - camera->x, 60 - camera->y);///discover the secrets of the human genome project
				gTutorialbg.render(window.gRenderer, -4320 - camera->x, -1052 - camera->y);
				gTutorialbg2.render(window.gRenderer, 680 - camera->x, -1052 - camera->y);
				gTutorial3.render(window.gRenderer, -3850 - camera->x, -1622 - camera->y);
				gHubWorld_BG.render(window.gRenderer, -3980 - camera->x, -2513 - camera->y);
				gKongaCage.render(window.gRenderer, testBox[26].xPos - camera->x, testBox[26].yPos - camera->y);
				gDarunggarBox.render(window.gRenderer, testBox[22].xPos - camera->x, testBox[22].yPos - camera->y);
				gSpinner.defaultAnimate(window.gRenderer, 5000 - camera->x, -30 - camera->y, 55, 82, 80);
				if(player1.nearOxygen && !player1.crouched && !player1.succingOxygen && !player1.walkingToLocation && !player1.fallen) { gCeeButton.render(window.gRenderer, player1.hitBox->x - 5 - camera->x, player1.hitBox->y - 65 - camera->y); }
				if(player1.nearItemButtonOrMachine) { gCeeButton.render(window.gRenderer, player1.hitBox->x - 5 - camera->x, player1.hitBox->y - 65 - camera->y); }
				gTutorialZipline1.render(window.gRenderer, 1400 - camera->x, 300 - camera->y);
				gTutorialZipline1.render(window.gRenderer, 4903 - camera->x, 480 - camera->y);
				gTutorialEnemy.render(window.gRenderer, 2656 - camera->x, 679 - camera->y);
				gBulletCavern.render(window.gRenderer, testBox[9].xPos - camera->x, testBox[9].yPos - camera->y);
                    if(maggot.sequence == 0 || maggot.sequence == 44)
                    {
                        if(!maggot.firstTry && !targetGame.funTimer.isStarted())
                        {
                             gBulletCavern2.render(window.gRenderer, testBox[9].xPos - camera->x, testBox[9].yPos - camera->y);
                        }
                    }
				if(bulletCageTrigger.activated) {gBulletCavernCage.render(window.gRenderer, testBox[9].xPos + 11 - camera->x, testBox[9].yPos + 370 - camera->y);}
				else { gBulletCavernCage.render(window.gRenderer, testBox[9].xPos + 11 - camera->x, testBox[9].yPos + 240 - camera->y); }
				if(targetGame.score == 29 && maggot.score == 40) {gNewMove.render(window.gRenderer, 7290 - camera->x, -239 - camera->y); }


				if(player1.animation == 6 && gPlayerArmed.animationTimer4.getTicks() / 95 > 1) {oxygentank.activated = true;}
                    if(player1.animation == 6 && gPlayerArmed.animationTimer4.getTicks() / 95 > 16) {oxygentank.activated = false;}
				if(!oxygentank.activated)
                { gOxygenTank.render2(window.gRenderer, oxygentank.xPos + 15 - camera->x, oxygentank.yPos - camera->y, 22, 70, 22, 0); }
                else
                { gOxygenTank.render2(window.gRenderer, oxygentank.xPos + 15 - camera->x, oxygentank.yPos - camera->y, 22, 70, 0, 0);}

				if(debug_bool_delete_this_later_idiot)
                {
                    gLeverOff.render(window.gRenderer, lever.xPos - camera->x, lever.yPos - camera->y);
                }
                else
                {
                    gLeverOn.render(window.gRenderer, lever.xPos - camera->x, lever.yPos - camera->y);
                }

                ///render trucks
                for(int i = 0; i < numberOfTrucks; i++) { if(trucks[i].health > 0) {gArmoredTruckDevil.renderArmoredTruckDevil(window.gRenderer, gArmoredTruckDevilOrig.texture, trucks[i].xPos - camera->x, trucks[i].yPos - camera->y);}}
                for(int i = 0; i < numberOfTrucks; i++) { gArmoredTruck[i].renderArmoredTruck(window.gRenderer, gArmoredTruckOrig.texture,trucks[i].xPos - camera->x, trucks[i].yPos - camera->y,  trucks[i].triggeredTimer.getTicks(), trucks[i].charging, trucks[i].reversing);}
                for(int i = 0; i < numberOfTrucks; i++) { gArmoredTruckCracks.renderArmoredTruckCracks(window.gRenderer, gArmoredTruckCracksOrig.texture,trucks[i].xPos - camera->x, trucks[i].yPos - camera->y,  trucks[i].health);}

                gRoboMaggot.render(window.gRenderer, maggot.xPos - camera->x, maggot.yPos - camera->y, NULL , 0, NULL, maggot.flip);

                ///render Sorse
                for(int i = 0; i < numberOfSorse; i++)
                {
                    gSorse[i].renderSorseBlackHole(window.gRenderer, gSorseOrig.texture, sorse[i].hitBoxBlackHole->x - camera->x, sorse[i].hitBoxBlackHole->y - camera->y, sorse[i].blackHoleFlying);
                    if(sorse[i].hitColor.isStarted()) {gSorseOrig.setColor(190, 0, 0);}
                    gSorse[i].renderSorse(window.gRenderer, gSorseOrig.texture, sorse[i].xPos- camera->x, sorse[i].yPos - camera->y, sorse[i].flip, sorse[i].summonAnimation.getTicks(), sorse[i].attackAnimation.getTicks(), sorse[i].alive, sorse[i].collapsing);
                    gSorseOrig.setColor(0xFF, 0xFF, 0xFF);
                    if(sorse[i].health == 1) {gSorseOrig.setColor(210, 110, 0);}
                    gSorse[i].renderSorseWeakpoint(window.gRenderer, gSorseOrig.texture, sorse[i].xPos- camera->x, sorse[i].yPos - camera->y, sorse[i].flip, sorse[i].alive);
                    gSorseOrig.setColor(0xFF, 0xFF, 0xFF);
               }

				if(!player1.hasGun)
                {
                    gSemi_Auto.render(window.gRenderer, semi_auto.xPos - camera->x, semi_auto.yPos - camera->y);
                }

				for(int i = 0; i < 19; i++)
                {
                    if(player1bullets[i].active)
                    {
                        gBullet.render(window.gRenderer, player1bullets[i].xPos - camera->x, player1bullets[i].yPos - camera->y);
                    }
                }

                for(int i = 0; i < 19; i++)
                {
                    if(enemyBullets[i].active)
                    {
                        gBullet2.render(window.gRenderer, enemyBullets[i].xPos - camera->x, enemyBullets[i].yPos - camera->y);
                    }
                }

                for(int i = 0; i < 15; i++)
                {
                    gTargets.setAlpha(targetGame.targets[i].alpha);
                    gTargets.renderTargets(window.gRenderer, targetGame.targets[i].hitBox->x - camera->x, targetGame.targets[i].hitBox->y - camera->y, targetGame.targets[i].color, targetGame.targets[i].alive);
                }

                ///render snowman
                if(snowman1.noseActive || snowman1.exploding) { gSnowmanNose.renderSnowmanNose(window.gRenderer, snowman1.xPosNose - camera->x, snowman1.yPosNose - camera->y, gSnowman.animationBool2, snowman1.rotation, snowman1.time2explode, snowman1.inFlight, snowman1.exploding); }
                gSnowman.renderSnowman(window.gRenderer, snowman1.xPos - camera->x, snowman1.yPos - camera->y, snowman1.alive, snowman1.time2shoot, snowman1.inFlight);

                ///render Garbage Gaurdians
                for(int i = 0; i < numberOfGarbageGaurdians; i++)
                {
                    if(garbageGaurdians[i].hitColorBody.isStarted()) { gGarbageGaurdianOrig.setColor(255, 0, 0);}
                    gGarbageGaurdians[i].renderGarbageGaurdian(window.gRenderer, gGarbageGaurdianOrig.texture, garbageGaurdians[i].xPos- camera->x, garbageGaurdians[i].yPos - camera->y, garbageGaurdians[i].animation);
                    gGarbageGaurdianOrig.setColor(0xFF, 0xFF, 0xFF);
                    gGarbageGaurdians[i].renderGarbageGaurdianSword(window.gRenderer, gGarbageGaurdianOrig.texture, garbageGaurdians[i].swordHitbox->x - camera->x, garbageGaurdians[i].swordHitbox->y - camera->y, garbageGaurdians[i].swordFlying, garbageGaurdians[i].swordInGround);
                    gGarbageGaurdians[i].renderGarbageGaurdianShield(window.gRenderer, gGarbageGaurdianOrig.texture, garbageGaurdians[i].shieldHitbox->x - camera->x, garbageGaurdians[i].shieldHitbox->y - camera->y, garbageGaurdians[i].shieldFlying);
                }

                ///render player before walls
                if(player1.damageColor.isStarted()) {gPlayerArmed.setColor(255, 0, 0); gPlayerSemi.setColor(255, 0, 0);}
                if(!player1.fallen || player1.rising)
                {
                    if(player1.secretUnlocked)
                    {
                        gPlayerPixelated.renderPlayer(player1.getAnimation(), window.gRenderer, (int)player1.xPos - camera->x, (int)player1.yPos - camera->y, player1.flip, player1.gunBounceTimer, player1.gunBounceDirection);
                    }
                    else if(player1.hasGun)
                    {
                        if(player1.animation == 6 && gPlayerArmed.animationTimer4.getTicks() / 95 > 18) { player1.succingOxygen = false; player1.controllable = true; gPlayerFace.animationBool = true;}
                        gPlayerArmed.renderPlayer(player1.getAnimation(), window.gRenderer, (int)player1.xPos - camera->x, (int)player1.yPos - camera->y, player1.flip, player1.gunBounceTimer, player1.gunBounceDirection);
                        if(player1.getAnimation() != 6) { gPlayerSemi.renderSemi(window.gRenderer, player1.xPos - 0 - camera->x, player1.yPos + 72 - camera->y, player1.flip, player1.gunTimer.getTicks(), player1.reloadTimer.getTicks() - 400, gPlayerArmed.gunOffset, player1.gunDirection, player1.animation, player1.holdingUp); } ///doesn't render the held weapon during certain animations

                                                                                                                                                                                             //WHAT???
                        gPlayerFace.renderFace(window.gRenderer, player1.xPos - camera->x, player1.yPos - camera->y, player1.flip, player1.succingOxygen, gPlayerArmed.gunOffset, player1.getAnimation(), gPlayerArmed.gunOffset2);
                    }

                    else
                    {
                        gPlayer.renderPlayer(player1.getAnimation(), window.gRenderer, (int)player1.xPos - camera->x, (int)player1.yPos - camera->y, player1.flip, player1.gunBounceTimer, player1.gunBounceDirection);

                    }
                }

                ///render robot body
                for(int i = 0; i < numberOfRobots; i++)
                {
                   if(robots[i].hitColorBody.isStarted()) {gRobotOrig.setColor(255, 0, 0);}
                    gRobot[i].renderRobot(window.gRenderer, gRobotOrig.texture, robots[i].xPos - camera->x, robots[i].yPos - camera->y, 1, robots[i].alive, 1);
                    gRobotOrig.setColor(0xFF, 0xFF, 0xFF);
                }
                ///render robot left arm
                for(int i = 0; i < numberOfRobots; i++)
                {
                    if(robots[i].hitColorLeft.isStarted()) { gRobotOrig.setColor(0, 100, 255);}
                    if(robots[i].leftActive)
                    {
                        gRobotArmLeft[i].renderRobot(window.gRenderer, gRobotOrig.texture, robots[i].xPos - camera->x, robots[i].yPos - camera->y, robots[i].leftActive, robots[i].alive, 2);
                    }
                    else
                    {
                        gRobotArmLeft[i].renderRobot(window.gRenderer, gRobotOrig.texture, robots[i].xPosLeft - camera->x, robots[i].yPosLeft - camera->y, robots[i].leftActive, 1, 2);
                    }
                    gRobotOrig.setColor(0xFF, 0xFF, 0xFF);
                }
				///render robot right arm
                for(int i = 0; i < numberOfRobots; i++)
                {
                    if(robots[i].hitColorRight.isStarted()) {gRobotOrig.setColor(0, 100, 255);};
                    if(robots[i].rightActive)
                    {
                        gRobotArmRight[i].renderRobot(window.gRenderer, gRobotOrig.texture, robots[i].xPos - camera->x, robots[i].yPos - camera->y, robots[i].rightActive, robots[i].alive, 3);
                    }
                    else
                    {
                        gRobotArmRight[i].renderRobot(window.gRenderer, gRobotOrig.texture, robots[i].xPosRight - camera->x, robots[i].yPosRight - camera->y, robots[i].rightActive, 1, 3);
                    }
                    gRobotOrig.setColor(0xFF, 0xFF, 0xFF);
                }

                gTutorial.render(window.gRenderer, -4320 - camera->x, -1052 - camera->y );
                gTutorial2.render(window.gRenderer, 680 - camera->x, -1052 - camera->y );
                gHubWorld.render(window.gRenderer, -3980 - camera->x, -2513 - camera->y);
                gLedge.render(window.gRenderer, ledges[0].xPos - camera->x, ledges[0].yPos - camera->y, NULL, 0, 0, ledges[0].flip);
                gLedge.render(window.gRenderer, ledges[1].xPos - camera->x, ledges[1].yPos - camera->y, NULL, 0, 0, ledges[1].flip);
				gTestBox.render(window.gRenderer, testBox[0].xPos - camera->x, testBox[0].yPos - camera->y );
				gTestBox.render(window.gRenderer, testBox[1].xPos - camera->x, testBox[1].yPos - camera->y );
				gTestBox.render(window.gRenderer, testBox[2].xPos - camera->x, testBox[2].yPos - camera->y );
				gTestBox.render(window.gRenderer, testBox[3].xPos - camera->x, testBox[3].yPos - camera->y );
				gTestBox.render(window.gRenderer, testBox[4].xPos - camera->x, testBox[4].yPos - camera->y );
				gGroundBox.render(window.gRenderer, testBox[5].xPos - camera->x, testBox[5].yPos - camera->y );
				gGroundBox2.render(window.gRenderer, testBox[6].xPos - camera->x, testBox[6].yPos - camera->y );
				gTestBox.render(window.gRenderer, testBox[8].xPos - camera->x, testBox[8].yPos - camera->y );
				gGround.render(window.gRenderer, ground.xPos - camera->x, ground.yPos - camera->y );
				gWall.render(window.gRenderer, leftWall.xPos - camera->x, leftWall.yPos - camera->y);
				gTarget.render(window.gRenderer, target.xPos - camera->x, target.yPos - camera->y);
                gZiplineBase.render(window.gRenderer, 1372 - camera->x, 196 - camera->y);
				gHair.renderHair(window.gRenderer, player1.rectangleForTheHair->x - 28 - camera->x, player1.rectangleForTheHair->y - 10 - camera->y, player1.flip);
				gZipline.renderZipline(window.gRenderer, zipline1.xPos - camera->x, zipline1.yPos - camera->y, zipline1.docked, zipline1.carrying, zipline1.powerTimer.isStarted());
				if(!player1.granadoExploding) { gGranado.render(window.gRenderer, player1.granadoXpos - camera->x, player1.granadoYpos - camera->y, NULL, player1.granadoSpin, 0, ledges[0].flip); }
				else { gExplosionGeneric.render(window.gRenderer, player1.granadoXpos - camera->x, player1.granadoYpos - camera->y);}


                gGround.render(window.gRenderer, ground2.xPos - camera->x, ground2.yPos - camera->y );
				gGround.render(window.gRenderer, ground3.xPos - camera->x, ground3.yPos - camera->y );

				///render SHARK ROBOTS
                for(int i = 0; i < numberOfSharkRobots; i++)
                {
                    if(sharkRobots[i].beingShotInTheFaceTimer.getTicks() < 100 && sharkRobots[i].beingShotInTheFaceTimer.getTicks() > 0) { gSharkRobotOrig.setColor(255, 0, 0);}
                   gSharkRobot[i].renderSharkRobots(window.gRenderer, gSharkRobotOrig.texture, sharkRobots[i].xPos - camera->x, sharkRobots[i].yPos - camera->y, sharkRobots[i].beingShotInTheFaceTimer.getTicks());
                   gSharkRobotOrig.setColor(0xFF, 0xFF, 0xFF);
                }

                ///render KULTENTAS
                for(int i = 0; i < numberOfKultentas; i++)
                {
                    if(kultentas[i].hitColor.isStarted()) { gKultentaOrig.setColor(255, 0, 0); }
                    for(int a = 0; a < 8; a++)
                    {
                        if(kultentas[i].bulletactive[a])
                        {
                            gBullet.render(window.gRenderer, kultentas[i].bullets[a]->x - camera->x, kultentas[i].bullets[a]->y - camera->y);
                        }
                    }
                    if(kultentas[i].storage1visibol) { gBullet.render(window.gRenderer, kultentas[i].bulletStorage1.x - camera->x, kultentas[i].bulletStorage1.y - camera->y); }
                    if(kultentas[i].storage2visibol) { gBullet.render(window.gRenderer, kultentas[i].bulletStorage2.x - camera->x, kultentas[i].bulletStorage2.y - camera->y); }
                    if(kultentas[i].storage3visibol) { gBullet.render(window.gRenderer, kultentas[i].bulletStorage3.x - camera->x, kultentas[i].bulletStorage3.y - camera->y); }
                    if(kultentas[i].storage4visibol) { gBullet.render(window.gRenderer, kultentas[i].bulletStorage4.x - camera->x, kultentas[i].bulletStorage4.y - camera->y); }
                    if(kultentas[i].storage5visibol) { gBullet.render(window.gRenderer, kultentas[i].bulletStorage5.x - camera->x, kultentas[i].bulletStorage5.y - camera->y); }
                    gKultentaGun[i].renderKultentaEvilArm(window.gRenderer, gKultentaOrig.texture, kultentas[i].xPos - camera->x, kultentas[i].yPos - camera->y, kultentas[i].clip1, kultentas[i].dead ,0);
                    gKultentaEvilArm[i].renderKultentaEvilArm(window.gRenderer, gKultentaOrig.texture, kultentas[i].xPos - camera->x, kultentas[i].yPos - camera->y, kultentas[i].clip,kultentas[i].dead ,1);
                    gKultenta[i].renderKultentas(window.gRenderer, gKultentaOrig.texture, kultentas[i].xPos - camera->x, kultentas[i].yPos - camera->y, kultentas[i].dead, kultentas[i].beingDeadTimer.getTicks());
                    gKultentaOrig.setColor(0xFF, 0xFF, 0xFF);
                }

                ///render TNT GENTS
                for(int i = 0; i < numberOfTNTgent; i++)
                {
                    for(int a = 0; a < 5; a++)
                    {
                        if(gents[i].bulletactive[a])
                        {
                            gBullet.render(window.gRenderer, gents[i].bullets[a]->x - camera->x, gents[i].bullets[a]->y - camera->y);
                        }
                    }
                    if(gents[i].hitColorHead.isStarted()) {gTNTgentOrig.setColor(255, 0, 0);}
                    if(gents[i].health < 1 && gents[i].headThrown == 0) {gTNTgentOrig.setColor(15, 15, 15);}
                    gTNTgent[i].renderTNTgent(window.gRenderer, gTNTgentOrig.texture, gents[i].xPos - camera->x, gents[i].yPos - camera->y, gents[i].animation, gents[i].health);
                    gTNTgentOrig.setColor(0xFF, 0xFF, 0xFF);
                    if(gents[i].health > 0) { gTNTgent[i].renderTNTgentHead(window.gRenderer, gTNTgentOrig.texture, gents[i].xPos - camera->x, gents[i].yPos - camera->y, gents[i].animation, gents[i].speakingFrame, gents[i].hitBoxHead->x - camera->x, gents[i].hitBoxHead->y - camera->y);}
                    if(gents[i].explosionTimer.isStarted()) { gTNTgent[i].renderTNTgentExplosion(window.gRenderer, gTNTgentOrig.texture, gents[i].explosionHitbox->x - camera->x, gents[i].explosionHitbox->y - camera->y);}
                }

                ///render Wermbls
                for(int i = 0; i < numberOfWermbls; i++)
                {
                    for(int a = 0; a < 3; a++)
                    {
                        if(wermbls[i].bulletactive[a])
                        {
                            gBullet.render(window.gRenderer, wermbls[i].bullets[a]->x - camera->x, wermbls[i].bullets[a]->y - camera->y);
                        }
                    }

                    if (wermbls[i].gachaActivationTimer.isStarted()) {gWermblsOrig.setColor(0, wermbls[i].activatingColor, 0);}
                    gWermbls[i].renderWermbl(window.gRenderer, gWermblsOrig.texture, wermbls[i].xPos- camera->x, wermbls[i].yPos - camera->y, wermbls[i].gachAvtivationState, wermbls[i].flyingSpeed, wermbls[i].flip);
                    gWermblsOrig.setColor(0xFF, 0xFF, 0xFF);

                    if(wermbls[i].gachAvtivationState != 10){
                    gWermbls[i].renderWermblFace(window.gRenderer, gWermblsOrig.texture, wermbls[i].xPosFace- camera->x, wermbls[i].yPosFace - camera->y, wermbls[i].gachAvtivationState, wermbls[i].flip);
                    if(wermbls[i].numberOfBulletsEaten == 1) {gWermblsOrig.setColor(0, 250, 0);}
                    gWermbls[i].remderWermblGacha(window.gRenderer, gWermblsOrig.texture, wermbls[i].gacha1x- camera->x, wermbls[i].gacha1y - camera->y, wermbls[i].gacha1);
                    gWermblsOrig.setColor(0xFF, 0xFF, 0xFF);
                    if(wermbls[i].numberOfBulletsEaten == 2) {gWermblsOrig.setColor(0, 250, 0);}
                    gWermbls[i].remderWermblGacha(window.gRenderer, gWermblsOrig.texture, wermbls[i].gacha2x- camera->x, wermbls[i].gacha2y - camera->y, wermbls[i].gacha2);
                    gWermblsOrig.setColor(0xFF, 0xFF, 0xFF);
                    if(wermbls[i].numberOfBulletsEaten == 3) {gWermblsOrig.setColor(0, 250, 0);}
                    gWermbls[i].remderWermblGacha(window.gRenderer, gWermblsOrig.texture, wermbls[i].gacha3x- camera->x, wermbls[i].gacha3y - camera->y, wermbls[i].gacha3);
                    gWermblsOrig.setColor(0xFF, 0xFF, 0xFF);}
                }

                ///render Konga Lees
                for(int i = 0; i < numberOfKongaLees; i++)
                {
                    gKongaLees[i].renderKongaLee(window.gRenderer, gKongaLeeOrig.texture, kongaLees[i].xPos- camera->x, kongaLees[i].yPos - camera->y, kongaLees[i].angle, kongaLees[i].footSpeed, kongaLees[i].spitting, kongaLees[i].variant, kongaLees[i].exploded);
                    gKongaLees[i].renderKongaLeeExplosion(window.gRenderer, gKongaLeeOrig.texture, kongaLees[i].explosionHitbox->x - camera->x, kongaLees[i].explosionHitbox->y - camera->y, kongaLees[i].explosionTimer.isStarted() );
                    for(int a = 0; a < 3; a++)
                    {
                        if(kongaLees[i].bulletactive[a])
                        {
                            gBullet.render(window.gRenderer, kongaLees[i].bullets[a]->x - camera->x, kongaLees[i].bullets[a]->y - camera->y);
                        }
                    }
                }

                ///render player in front of ground
				if(player1.damageColor.isStarted()) {gPlayerArmed.setColor(255, 0, 0); printf("teeest"); gPlayerSemi.setColor(255, 0, 0);}
				if(player1.fallen || player1.rising)
                {
                    if(player1.secretUnlocked)
                    {
                        gPlayerPixelated.renderPlayer(player1.getAnimation(), window.gRenderer, (int)player1.xPos - camera->x, (int)player1.yPos - camera->y, player1.flip, player1.gunBounceTimer, player1.gunBounceDirection);
                    }
                    else if(player1.hasGun)
                    {
                        if(player1.animation == 6 && gPlayerArmed.animationTimer4.getTicks() / 95 > 18) { player1.succingOxygen = false; player1.controllable = true; gPlayerFace.animationBool = true;}
                        gPlayerArmed.renderPlayer(player1.getAnimation(), window.gRenderer, (int)player1.xPos - camera->x, (int)player1.yPos - camera->y, player1.flip, player1.gunBounceTimer, player1.gunBounceDirection);
                        if(player1.getAnimation() != 6) { gPlayerSemi.renderSemi(window.gRenderer, player1.xPos - 0 - camera->x, player1.yPos + 72 - camera->y, player1.flip, player1.gunTimer.getTicks(), player1.reloadTimer.getTicks() - 400, gPlayerArmed.gunOffset, player1.gunDirection, player1.animation, player1.holdingUp); } ///doesn't render the held weapon during certain animations

                                                                                                                                                                                             //WHAT???
                        gPlayerFace.renderFace(window.gRenderer, player1.xPos - camera->x, player1.yPos - camera->y, player1.flip, player1.succingOxygen, gPlayerArmed.gunOffset, player1.getAnimation(), gPlayerArmed.gunOffset2);
                    }

                    else
                    {
                        gPlayer.renderPlayer(player1.getAnimation(), window.gRenderer, (int)player1.xPos - camera->x, (int)player1.yPos - camera->y, player1.flip, player1.gunBounceTimer, player1.gunBounceDirection);

                    }
                }

                if(maggot.score == 40)
                {
                    timeText.str( "a" );
                    timeText << maggot.score << "/40";

                    if( !text.loadFromRenderedText( timeText.str().c_str(), textColor2, gFont, window.gRenderer ) )
                    {
                        printf( "Unable to render FPS texture!\n" );
                    }
                }
                else
                {
                    timeText.str( "a" );
                    timeText << maggot.score << "/40";

                    if( !text.loadFromRenderedText( timeText.str().c_str(), textColor, gFont, window.gRenderer ) )
                    {
                        printf( "Unable to render FPS texture!\n" );
                    }
                }

                if(targetGame.score == 29)
                {
                    timeText.str( "a" );
                    timeText << targetGame.score << "/29";

                    if( !level2Score.loadFromRenderedText( timeText.str().c_str(), textColor2, gFont, window.gRenderer ) )
                    {
                        printf( "Unable to render FPS texture!\n" );
                    }
                }
                else
                {
                   timeText.str( "a" );
                    timeText << targetGame.score << "/29";

                    if( !level2Score.loadFromRenderedText( timeText.str().c_str(), textColor, gFont, window.gRenderer ) )
                    {
                        printf( "Unable to render FPS texture!\n" );
                    }
                }


                timeText.str( "a" );
                timeText << "HP: " << player1.health;

                if( !healthHUD.loadFromRenderedText( timeText.str().c_str(), textColor, gFont, window.gRenderer ) )
                {
                    printf( "Unable to render FPS texture!\n" );
                }

                text.render(window.gRenderer, 8165 - camera->x, -205 - camera->y );
                level2Score.render(window.gRenderer, 8165 - camera->x, -165 - camera->y );
                healthHUD.render(window.gRenderer, 100, 80);



				///RECTANGLE RENDERING
				SDL_SetRenderDrawColor( window.gRenderer, 0, 250, 0, 250 ); ///green?


				if(debug_bool_delete_this_later_idiot)
                {
                    player1.hitBox->x -= camera->x;
                    player1.hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, player1.hitBox );
                    player1.hitBox->x += camera->x;
                    player1.hitBox->y += camera->y;

                    tutorialSecret.hitBox->x -= camera->x;
                    tutorialSecret.hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, tutorialSecret.hitBox );
                    tutorialSecret.hitBox->x += camera->x;
                    tutorialSecret.hitBox->y += camera->y;

                    trucks[0].hitBoxGlass->x -= camera->x;
                    trucks[0].hitBoxGlass->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, trucks[0].hitBoxGlass );
                    trucks[0].hitBoxGlass->x += camera->x;
                    trucks[0].hitBoxGlass->y += camera->y;

                    maggot.hitBox->x -= camera->x;
                    maggot.hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, maggot.hitBox );
                    maggot.hitBox->x += camera->x;
                    maggot.hitBox->y += camera->y;

                    sharkRobots[0].hitBox->x -= camera->x;
                    sharkRobots[0].hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, sharkRobots[0].hitBox );
                    sharkRobots[0].hitBox->x += camera->x;
                    sharkRobots[0].hitBox->y += camera->y;

                    kultentas[0].hitBox->x -= camera->x;
                    kultentas[0].hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, kultentas[0].hitBox );
                    kultentas[0].hitBox->x += camera->x;
                    kultentas[0].hitBox->y += camera->y;

                    for(int i = 0; i < 15; i++)
                    {
                        if(targetGame.targets[i].alive)
                        {
                            if(targetGame.targets[i].color == 1)
                            {
                                SDL_SetRenderDrawColor( window.gRenderer, 250, 0, 0, 250 );
                            }
                            else if(targetGame.targets[i].color == 3)
                            {
                                SDL_SetRenderDrawColor( window.gRenderer, 0, 0, 250, 250 );
                            }

                            targetGame.targets[i].hitBox->x -= camera->x;
                            targetGame.targets[i].hitBox->y -= camera->y;
                            SDL_RenderDrawRect( window.gRenderer, targetGame.targets[i].hitBox );
                            targetGame.targets[i].hitBox->x += camera->x;
                            targetGame.targets[i].hitBox->y += camera->y;

                            SDL_SetRenderDrawColor( window.gRenderer, 0, 250, 0, 250 );
                        }
                    }
                }


               /* if(player1.hitBox->x + player1.hitBox->w / 2 > 340 && player1.velocity + player1.hitBox->w / 2 < 627)
                {
                    if(player1.yVelocity > 315 - (player1.hitBox->x / 8) )//&& player1.yVelocity < 345 - (player1.hitBox->x / 8))
                    {
                        player1.yVelocity = 320 - (player1.hitBox->x / 8); ///YVELOCITY SHOULD HAVE + hitbox HEIGHT
                        player1.foo = true;
                    }
                }*/

                /*if(player1.hitBox->x + player1.hitBox->w / 2 > 340 && player1.velocity + player1.hitBox->w / 2 < 627)
                {                                              ///y1 - p.hitbox->w / 2 - (where you're at - x1? / slope)
                    if(player1.hitBox->y + player1.hitBox->h > (397 - 15) - ((player1.hitBox->x - 340) / 8) + 1 )  ///this is complicated
                    {
                        player1.yVelocity = (397 - 15) - ((player1.hitBox->x - 340) / 8) - player1.hitBox->h + 11;
                        player1.foo = true;
                        if(player1.released) { player1.jumping = false;} //if i don't put it here then u still jump for a frame and the screen shakes :(
                    }
                }*/


///cockzz

                    level1Trigger.hitBox->x -= camera->x;
                    level1Trigger.hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, level1Trigger.hitBox );
                    level1Trigger.hitBox->x += camera->x;
                    level1Trigger.hitBox->y += camera->y;

                    level2Trigger.hitBox->x -= camera->x;
                    level2Trigger.hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, level2Trigger.hitBox );
                    level2Trigger.hitBox->x += camera->x;
                    level2Trigger.hitBox->y += camera->y;

                if(debug_bool_delete_this_later_idiot)
                {
                    oxygentank.hitBox->x -= camera->x;
                    oxygentank.hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, oxygentank.hitBox );
                    oxygentank.hitBox->x += camera->x;
                    oxygentank.hitBox->y += camera->y;

                    zipline1.hitBox->x -= camera->x;
                    zipline1.hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, zipline1.hitBox );
                    zipline1.hitBox->x += camera->x;
                    zipline1.hitBox->y += camera->y;


                    if(robots[0].rightActive && robots[0].alive)
                    {
                    robots[0].hitBoxRight->x -= camera->x;
                    robots[0].hitBoxRight->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, robots[0].hitBoxRight );
                    robots[0].hitBoxRight->x += camera->x;
                    robots[0].hitBoxRight->y += camera->y;
                   }

                   if(robots[0].leftActive && robots[0].alive)
                    {
                    robots[0].hitBoxLeft->x -= camera->x;
                    robots[0].hitBoxLeft->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, robots[0].hitBoxLeft );
                    robots[0].hitBoxLeft->x += camera->x;
                    robots[0].hitBoxLeft->y += camera->y;
                    }

                    if(robots[0].alive)
                    {
                    robots[0].hitBox->x -= camera->x;
                    robots[0].hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, robots[0].hitBox );
                    robots[0].hitBox->x += camera->x;
                    robots[0].hitBox->y += camera->y;
                    }


                    for(int i = 0; i < 19; i++)
                    {
                        if(player1bullets[i].active)
                        {
                            player1bullets[i].hitBox->x -= camera->x;
                            player1bullets[i].hitBox->y -= camera->y;
                            SDL_RenderDrawRect( window.gRenderer, player1bullets[i].hitBox );
                            player1bullets[i].hitBox->x += camera->x;
                            player1bullets[i].hitBox->y += camera->y;
                        }
                    }

                    for(int i = 0; i < NumberOfDoors; i++)
                    {
                        doors[i].hitBox->x -= camera->x;
                        doors[i].hitBox->y -= camera->y;
                        SDL_RenderDrawRect( window.gRenderer, doors[i].hitBox );
                        doors[i].hitBox->x += camera->x;
                        doors[i].hitBox->y += camera->y;
                    }

                    /*for(int i = 0; i < 19; i++)
                    {
                        if(enemyBullets[i].active)
                        {
                            enemyBullets[i].hitBox->x -= camera->x;
                            enemyBullets[i].hitBox->y -= camera->y;
                            SDL_RenderDrawRect( window.gRenderer, enemyBullets[i].hitBox );
                            enemyBullets[i].hitBox->x += camera->x;
                            enemyBullets[i].hitBox->y += camera->y;
                        }
                    }*/

                    for(int i = 0; i < 2; i++)
                    {
                        ledges[i].hitBox->x -= camera->x;
                        ledges[i].hitBox->y -= camera->y;
                        SDL_RenderDrawRect( window.gRenderer, ledges[i].hitBox );
                        ledges[i].hitBox->x += camera->x;
                        ledges[i].hitBox->y += camera->y;
                    }


                }

				SDL_SetRenderDrawColor( window.gRenderer, 250, 0, 60, 250 ); ///red i think

				if(debug_bool_delete_this_later_idiot)
                {
                    snowman1.hitBox->x -= camera->x;
                    snowman1.hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, snowman1.hitBox );
                    snowman1.hitBox->x += camera->x;
                    snowman1.hitBox->y += camera->y;

                    snowman1.hitBoxHead->x -= camera->x;
                    snowman1.hitBoxHead->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, snowman1.hitBoxHead );
                    snowman1.hitBoxHead->x += camera->x;
                    snowman1.hitBoxHead->y += camera->y;






                    trucks[0].hitBoxBottom->x -= camera->x;
                    trucks[0].hitBoxBottom->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, trucks[0].hitBoxBottom );
                    trucks[0].hitBoxBottom->x += camera->x;
                    trucks[0].hitBoxBottom->y += camera->y;

                    trucks[0].hitBoxTop->x -= camera->x;
                    trucks[0].hitBoxTop->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, trucks[0].hitBoxTop );
                    trucks[0].hitBoxTop->x += camera->x;
                    trucks[0].hitBoxTop->y += camera->y;

                    trucks[0].hitBoxTrigger->x -= camera->x;
                    trucks[0].hitBoxTrigger->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, trucks[0].hitBoxTrigger );
                    trucks[0].hitBoxTrigger->x += camera->x;
                    trucks[0].hitBoxTrigger->y += camera->y;

                    trucks[0].hitBoxTrigger2->x -= camera->x;
                    trucks[0].hitBoxTrigger2->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, trucks[0].hitBoxTrigger2 );
                    trucks[0].hitBoxTrigger2->x += camera->x;
                    trucks[0].hitBoxTrigger2->y += camera->y;
                }

                SDL_SetRenderDrawColor( window.gRenderer, 0, 0, 250, 250 ); /// blue i think

                target.hitBox->x -= camera->x;
                target.hitBox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, target.hitBox );
                target.hitBox->x += camera->x;
                target.hitBox->y += camera->y;

                if(debug_bool_delete_this_later_idiot)
                {
                    SDL_RenderDrawPoint(window.gRenderer, kultentas[0].gunPoint.x - camera->x, kultentas[0].gunPoint.y - camera->y);
                    SDL_RenderDrawPoint(window.gRenderer, kultentas[0].gunPoint.x - 1 - camera->x, kultentas[0].gunPoint.y - camera->y);
                    SDL_RenderDrawPoint(window.gRenderer, kultentas[0].gunPoint.x - 2 - camera->x, kultentas[0].gunPoint.y - camera->y);

                    SDL_RenderDrawPoint(window.gRenderer, kultentas[0].bulletStorage1.x - camera->x, kultentas[0].bulletStorage1.y - camera->y);
                    SDL_RenderDrawPoint(window.gRenderer, kultentas[0].bulletStorage2.x - camera->x, kultentas[0].bulletStorage2.y - camera->y);
                    SDL_RenderDrawPoint(window.gRenderer, kultentas[0].bulletStorage3.x - camera->x, kultentas[0].bulletStorage3.y - camera->y);
                    SDL_RenderDrawPoint(window.gRenderer, kultentas[0].bulletStorage4.x - camera->x, kultentas[0].bulletStorage4.y - camera->y);
                    SDL_RenderDrawPoint(window.gRenderer, kultentas[0].bulletStorage5.x - camera->x, kultentas[0].bulletStorage5.y - camera->y);

                    //SDL_RenderDrawPoint(window.gRenderer, sorse[0].blackHoleSuccer.x - camera->x, sorse[0].blackHoleSuccer.y - camera->y);

                    player1.granadoHitbox->x -= camera->x;
                    player1.granadoHitbox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, player1.granadoHitbox );
                    player1.granadoHitbox->x += camera->x;
                    player1.granadoHitbox->y += camera->y;


                    snowman1.hitBoxNose->x -= camera->x;
                    snowman1.hitBoxNose->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, snowman1.hitBoxNose );
                    snowman1.hitBoxNose->x += camera->x;
                    snowman1.hitBoxNose->y += camera->y;

				player1.rectangleForTheHair->x -= camera->x;
				player1.rectangleForTheHair->y -= camera->y;
				SDL_RenderDrawRect( window.gRenderer, player1.rectangleForTheHair );
                player1.rectangleForTheHair->x += camera->x;
				player1.rectangleForTheHair->y += camera->y;

				player1.muzzleHitbox->x -= camera->x;
				player1.muzzleHitbox->y -= camera->y;
				SDL_RenderDrawRect( window.gRenderer, player1.muzzleHitbox );
                player1.muzzleHitbox->x += camera->x;
				player1.muzzleHitbox->y += camera->y;

				for(int i = 0; i < NumberOfTestBoxes; i++)
                {
                    testBox[i].hitBox->x -= camera->x;
                    testBox[i].hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, testBox[i].hitBox );
                    testBox[i].hitBox->x += camera->x;
                    testBox[i].hitBox->y += camera->y;
                }

                //SDL_RenderDrawRect( window.gRenderer, testBox[0].hitBox ); //wow this might be some of the oldest undisturbed code
                //SDL_RenderDrawRect( window.gRenderer, testBox[1].hitBox );
                //SDL_RenderDrawRect( window.gRenderer, testBox[2].hitBox );

                ground.hitBox->x -= camera->x;
                ground.hitBox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, ground.hitBox );
                ground.hitBox->x += camera->x;
                ground.hitBox->y += camera->y;

                bulletCageTrigger.hitBox->x -= camera->x;
                bulletCageTrigger.hitBox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, bulletCageTrigger.hitBox );
                bulletCageTrigger.hitBox->x += camera->x;
                bulletCageTrigger.hitBox->y += camera->y;

                if(player1.gunBouncing)
                {
                    if(player1.gunBounceTimer.getTicks() > 0 && player1.gunBounceTimer.getTicks() < 400)
                    {
                        player1.meleeHitbox->x -= camera->x;
                        player1.meleeHitbox->y -= camera->y;
                        SDL_RenderDrawRect( window.gRenderer, player1.meleeHitbox);
                        player1.meleeHitbox->x += camera->x;
                        player1.meleeHitbox->y += camera->y;
                    }
                }
                else
                {
                   if(gPlayerSemi.animationTimer2.getTicks() < 249 && gPlayerSemi.animationTimer2.getTicks() > 160 && !player1.holdingUp)
                    {
                        player1.meleeHitbox->x -= camera->x;
                        player1.meleeHitbox->y -= camera->y;
                        SDL_RenderDrawRect( window.gRenderer, player1.meleeHitbox);
                        player1.meleeHitbox->x += camera->x;
                        player1.meleeHitbox->y += camera->y;
                    }
                }

                gents[0].hitBox->x -= camera->x;
                gents[0].hitBox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, gents[0].hitBox );
                gents[0].hitBox->x += camera->x;
                gents[0].hitBox->y += camera->y;

                gents[0].hitBoxHead->x -= camera->x;
                gents[0].hitBoxHead->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, gents[0].hitBoxHead );
                gents[0].hitBoxHead->x += camera->x;
                gents[0].hitBoxHead->y += camera->y;

                gents[0].trigger->x -= camera->x;
                gents[0].trigger->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, gents[0].trigger );
                gents[0].trigger->x += camera->x;
                gents[0].trigger->y += camera->y;

                if(gents[0].explosionTimer.isStarted())
                {
                    gents[0].explosionHitbox->x -= camera->x;
                    gents[0].explosionHitbox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, gents[0].explosionHitbox );
                    gents[0].explosionHitbox->x += camera->x;
                    gents[0].explosionHitbox->y += camera->y;
                }

                wermbls[0].hitBox->x -= camera->x;
                wermbls[0].hitBox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, wermbls[0].hitBox );
                wermbls[0].hitBox->x += camera->x;
                wermbls[0].hitBox->y += camera->y;

                wermbls[0].hitBoxHead->x -= camera->x;
                wermbls[0].hitBoxHead->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, wermbls[0].hitBoxHead );
                wermbls[0].hitBoxHead->x += camera->x;
                wermbls[0].hitBoxHead->y += camera->y;

                if(wermbls[0].explosionTimer.isStarted())
                {
                    wermbls[0].explosionHitbox->x -= camera->x;
                    wermbls[0].explosionHitbox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, wermbls[0].explosionHitbox );
                    wermbls[0].explosionHitbox->x += camera->x;
                    wermbls[0].explosionHitbox->y += camera->y;
                }

                garbageGaurdians[0].hitBox->x -= camera->x;
                garbageGaurdians[0].hitBox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, garbageGaurdians[0].hitBox );
                garbageGaurdians[0].hitBox->x += camera->x;
                garbageGaurdians[0].hitBox->y += camera->y;

                garbageGaurdians[0].shieldHitbox->x -= camera->x;
                garbageGaurdians[0].shieldHitbox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, garbageGaurdians[0].shieldHitbox );
                garbageGaurdians[0].shieldHitbox->x += camera->x;
                garbageGaurdians[0].shieldHitbox->y += camera->y;

                garbageGaurdians[0].swordHitbox->x -= camera->x;
                garbageGaurdians[0].swordHitbox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, garbageGaurdians[0].swordHitbox );
                garbageGaurdians[0].swordHitbox->x += camera->x;
                garbageGaurdians[0].swordHitbox->y += camera->y;

                if(garbageGaurdians[0].state > 1)
                {
                    garbageGaurdians[0].hitBoxBody->x -= camera->x;
                    garbageGaurdians[0].hitBoxBody->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, garbageGaurdians[0].hitBoxBody );
                    garbageGaurdians[0].hitBoxBody->x += camera->x;
                    garbageGaurdians[0].hitBoxBody->y += camera->y;
                }

                if(garbageGaurdians[0].swordInGround)
                {
                    SDL_RenderDrawLine(window.gRenderer, garbageGaurdians[0].swordRamp.x1 - camera->x , garbageGaurdians[0].swordRamp.y1 - camera->y, garbageGaurdians[0].swordRamp.x2 - camera->x , garbageGaurdians[0].swordRamp.y2 - camera->y);
                }

                kongaLees[0].hitBoxBody->x -= camera->x;
                kongaLees[0].hitBoxBody->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, kongaLees[0].hitBoxBody );
                kongaLees[0].hitBoxBody->x += camera->x;
                kongaLees[0].hitBoxBody->y += camera->y;

                kongaLees[0].hitBoxFace->x -= camera->x;
                kongaLees[0].hitBoxFace->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, kongaLees[0].hitBoxFace );
                kongaLees[0].hitBoxFace->x += camera->x;
                kongaLees[0].hitBoxFace->y += camera->y;

                kongaLees[0].hitBoxFoot->x -= camera->x;
                kongaLees[0].hitBoxFoot->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, kongaLees[0].hitBoxFoot );
                kongaLees[0].hitBoxFoot->x += camera->x;
                kongaLees[0].hitBoxFoot->y += camera->y;

                if(kongaLees[0].explosionTimer.isStarted())
                {
                    kongaLees[0].explosionHitbox->x -= camera->x;
                    kongaLees[0].explosionHitbox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, kongaLees[0].explosionHitbox );
                    kongaLees[0].explosionHitbox->x += camera->x;
                    kongaLees[0].explosionHitbox->y += camera->y;
                }


                /*for(int i = 0; i < 3; i++)
                {
                    kongaLees[i].bullets->x -= camera->x;
                    kongaLees[i].bullets->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, kongaLees[i].bullets[i] );
                    kongaLees[i].bullets->x += camera->x;
                    kongaLees[i].bullets->y += camera->y;
                }*/

                sorse[0].hitBoxBody->x -= camera->x;
                sorse[0].hitBoxBody->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, sorse[0].hitBoxBody );
                sorse[0].hitBoxBody->x += camera->x;
                sorse[0].hitBoxBody->y += camera->y;

                sorse[0].hitBoxBlackHole->x -= camera->x;
                sorse[0].hitBoxBlackHole->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, sorse[0].hitBoxBlackHole );
                sorse[0].hitBoxBlackHole->x += camera->x;
                sorse[0].hitBoxBlackHole->y += camera->y;

                sorse[0].hitBoxTrigger->x -= camera->x;
                sorse[0].hitBoxTrigger->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, sorse[0].hitBoxTrigger );
                sorse[0].hitBoxTrigger->x += camera->x;
                sorse[0].hitBoxTrigger->y += camera->y;

                sorse[0].hitBoxFeet->x -= camera->x;
                sorse[0].hitBoxFeet->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, sorse[0].hitBoxFeet );
                sorse[0].hitBoxFeet->x += camera->x;
                sorse[0].hitBoxFeet->y += camera->y;

                sorse[0].hitBoxEyes->x -= camera->x;
                sorse[0].hitBoxEyes->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, sorse[0].hitBoxEyes );
                sorse[0].hitBoxEyes->x += camera->x;
                sorse[0].hitBoxEyes->y += camera->y;

                if(sorse[0].exploding)
                {
                    sorse[0].hitBoxExplosion->x -= camera->x;
                    sorse[0].hitBoxExplosion->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, sorse[0].hitBoxExplosion );
                    sorse[0].hitBoxExplosion->x += camera->x;
                    sorse[0].hitBoxExplosion->y += camera->y;
                }

                SDL_RenderDrawPoint(window.gRenderer, gents[0].gunPoint.x - camera->x, gents[0].gunPoint.y - camera->y);

                }
                for(int i = 0; i < NumberOfTestSlopes; i++)
                {
                    SDL_RenderDrawLine(window.gRenderer, testSlopes[i].x1 - camera->x , testSlopes[i].y1 - camera->y, testSlopes[i].x2 - camera->x , testSlopes[i].y2 - camera->y);
                }

                ///my retarded ammo think is actually here
                ammo1.renderAmmo(player1.clip, window.gRenderer, window.getWidth() - 120, window.getHeight() - ammo1.getHeight());
				ammo2.render(window.gRenderer, window.getWidth() - ammo2.getWidth(), window.getHeight() - ammo2.getHeight());

				hud_head.render(window.gRenderer, 40, 695);
				hud_baseplate.render(window.gRenderer, 121, 742);
				hud_ammo.renderAmmo2(player1.clip, window.gRenderer, 122, 743);

				if(loadBGalpha > 1 )
                {
                    if(loadBGalpha > 250)
                    {
                        loadBGalpha = 250;
                        gLoadingBG.render(window.gRenderer, 0, 0);
                        switch(destination)
                        {
                            case 0:
                            break;
                            case 1: area_debug = false;//level 1
                                    area_loading = true;
                                    needToLoadShit = true;
                                    needToSpawnShit = true;
                                    loadTimer = 200;
                            break;
                            case 2:
                            break;
                        }
                    }
                    else
                    {
                        loadBGalpha+=7;
                        gLoadingBG.setAlpha(loadBGalpha);
                        gLoadingBG.render(window.gRenderer, 0, 0);
                    }
                }

                }
                //else ///while game is paused
                if(player1.paused)
                {
                    gPauseMenu.render(window.gRenderer, player1.xPos - camera->x - 150, player1.yPos - camera->y - 300);
                    gDebugMenu.renderMenu(window.gRenderer, menu.hitBoxes[0]->x, menu.hitBoxes[0]->y - 18, menu.menuPage);
                    gDebugSpawner.render(window.gRenderer, menu.DraggableBox->x, menu.DraggableBox->y);



                    SDL_SetRenderDrawColor( window.gRenderer, 0, 250, 50, 250 ); /// green i think


                    SDL_GetMouseState(&mouse.x, &mouse.y);
                    mouseHitbox->x = mouse.x - 5;
                    mouseHitbox->y = mouse.y - 5;

                    ///pause collision

                    for(int i = 0; i < menu.numberOfHitboxes; i++) ///collision for DEBUG MENU HITBOXES
                    {
                        if(checkCollision(menu.hitBoxes[i], mouseHitbox, 1))
                        {
                            switch(menu.menuPage)
                            {
                                case 0: ///page 0
                                        switch(i)
                                        {
                                            case 0:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[0] );
                                                if(player1.unclicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    gents[0].reset();
                                                    gTNTgent[0].animationTimer1.start();
                                                    gTNTgent[0].animationTimer2.start();
                                                    gTNTgent[0].animationTimer3.start();
                                                    gTNTgent[0].animationTimer7.start();
                                                    gTNTgent[0].robotAnimation = 0;
                                                    gTNTgent[0].animationBool = 0;
                                                    gTNTgent[0].animationInt = 0;
                                                }
                                            }break;
                                            case 1:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[1] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    printf("TNT gent spawned\n");
                                                    gents[0].hitBox->x =  camera->x + menu.DraggableBox->x + 40;
                                                    gents[0].hitBox->y =   camera->y + menu.DraggableBox->y + 40;
                                                }
                                            }break;
                                            case 2:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[2] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    kultentas[0].reset();
                                                }
                                            }break;
                                            case 3:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[3] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    printf("Kultenta spawned\n");
                                                    kultentas[0].hitBox->x = camera->x + menu.DraggableBox->x + 40;
                                                    kultentas[0].hitBox->y = camera->y + menu.DraggableBox->y + 40;
                                                }
                                            }break;
                                            case 4:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[4] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    robots[0].reset();
                                                    gRobot[0].animationTimer1.start();
                                                    gRobot[0].animationTimer2.stop();
                                                    gRobot[0].robotAnimation = 0;
                                                    gRobotArmLeft[0].animationTimer1.start();
                                                    gRobotArmRight[0].animationTimer1.start();
                                                }
                                            }break;
                                            case 5:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[5] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    printf("Row Bit spawned\n");
                                                    robots[0].hitBox->x = camera->x + menu.DraggableBox->x + 40;
                                                    robots[0].hitBox->y = camera->y + menu.DraggableBox->y + 40;
                                                    robots[0].home = camera->x + menu.DraggableBox->x + 40;
                                                }
                                            }break;
                                            case 6:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[6] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    sharkRobots[0].reset();
                                                }
                                            }break;
                                            case 7:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[7] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    printf("Shark Robot spawned\n");
                                                    sharkRobots[0].hitBox->x = camera->x + menu.DraggableBox->x + 40;
                                                    sharkRobots[0].hitBox->y = camera->y + menu.DraggableBox->y + 40;
                                                }
                                            }break;
                                            case 8:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[8] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    trucks[0].reset();
                                                }
                                            }break;
                                            case 9:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[9] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    printf("Truck spawned\n");
                                                    trucks[0].xVelocity = camera->x + menu.DraggableBox->x + 40;
                                                    trucks[0].yVelocity = camera->y + menu.DraggableBox->y + 40;
                                                    trucks[0].hitBoxTrigger2->x = trucks[0].xVelocity - 440;
                                                    trucks[0].hitBoxTrigger2->y = trucks[0].yVelocity - 50;
                                                    trucks[0].home = camera->x + menu.DraggableBox->x + 40;
                                                }
                                            }break;
                                            case 10:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[10] );
                                                if(player1.clicky)
                                                {
                                                    menu.menuPage--;
                                                    Mix_PlayChannel( -1, aSelect, 0 );

                                                }
                                            }break;
                                            case 11:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[11] );
                                                if(player1.clicky)
                                                {
                                                    menu.menuPage++;
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;

                                        }
                                        break;
                                    case 1: ///page 1
                                        switch(i)
                                        {
                                            case 0:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[0] );
                                                if(player1.unclicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    if(debug_bool_delete_this_later_idiot)
                                                    {
                                                        debug_bool_delete_this_later_idiot = false;
                                                        printf("Hitboxes turned off\n");
                                                    }
                                                    else
                                                    {
                                                        printf("Hitboxes turned on\n");
                                                        debug_bool_delete_this_later_idiot = true;
                                                    }
                                                }
                                            }break;
                                            case 1:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[1] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 2:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[2] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 3:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[3] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    player1.velocity = camera->x + menu.DraggableBox->x + 40;
                                                    player1.yVelocity = camera->y + menu.DraggableBox->y + 40;
                                                }
                                            }break;
                                            case 4:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[4] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    wermbls[0].reset();
                                                    gWermbls[0].animationTimer1.start();
                                                    gWermbls[0].animationTimer2.start();
                                                    gWermbls[0].animationTimer4.stop();
                                                    gWermbls[0].animationTimer5.stop();
                                                }
                                            }break;
                                            case 5:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[5] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    printf("Wermbl spawned\n");
                                                    wermbls[0].hitBox->x = camera->x + menu.DraggableBox->x + 40;
                                                    wermbls[0].hitBox->y = camera->y + menu.DraggableBox->y + 40;
                                                }
                                            }break;
                                            case 6:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[6] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );

                                                    garbageGaurdians[0].reset();
                                                }
                                            }break;
                                            case 7:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[7] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    printf("Trash Tutelar spawned\n");
                                                    garbageGaurdians[0].hitBox->x = camera->x + menu.DraggableBox->x + 40;
                                                    garbageGaurdians[0].hitBox->y = camera->y + menu.DraggableBox->y + 40;
                                                }
                                            }break;
                                            case 8:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[8] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 9:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[9] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    printf("Konga Lee spawned\n");
                                                    kongaLees[0].hitBoxBody->x = camera->x + menu.DraggableBox->x + 40;
                                                    kongaLees[0].hitBoxBody->y = camera->y + menu.DraggableBox->y + 40;
                                                    kongaLees[0].reset();
                                                }
                                            }break;
                                            case 10:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[10] );
                                                if(player1.clicky)
                                                {
                                                    menu.menuPage--;
                                                    Mix_PlayChannel( -1, aSelect, 0 );

                                                }
                                            }break;
                                            case 11:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[11] );
                                                if(player1.clicky)
                                                {
                                                    menu.menuPage++;
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;

                                        }
                                    break;
                                    case 2: ///page 1
                                        switch(i)
                                        {
                                            case 0:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[0] );
                                                if(player1.unclicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    sorse[0].reset();
                                                }
                                            }break;
                                            case 1:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[1] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    printf("Sorse spawned\n");
                                                    sorse[0].hitBoxBody->x = camera->x + menu.DraggableBox->x + 40;
                                                    sorse[0].hitBoxBody->y = camera->y + menu.DraggableBox->y + 40;
                                                }
                                            }break;
                                            case 2:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[2] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 3:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[3] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 4:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[4] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 5:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[5] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 6:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[6] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 7:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[7] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 8:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[8] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 9:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[9] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 10:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[10] );
                                                if(player1.clicky)
                                                {
                                                    menu.menuPage--;
                                                    Mix_PlayChannel( -1, aSelect, 0 );

                                                }
                                            }break;
                                            case 11:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[11] );
                                                if(player1.clicky)
                                                {
                                                    menu.menuPage++;
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;

                                        }
                                        break;
                            }
                        }
                    }

                    ///collision for DRAGGABLE SPAWNER
                    if(!menu.dragging)
                    {
                        if(checkCollision(menu.DraggableBox, mouseHitbox, 1))
                        {
                            SDL_RenderDrawRect( window.gRenderer, menu.DraggableBox );
                            if(player1.clicking)
                            {
                                menu.dragging = true;
                            }
                        }
                    }
                    else
                    {
                        if(!player1.clicking)
                        {
                            menu.dragging = false;
                        }
                    }

                    if(menu.dragging)
                    {
                        menu.DraggableBox->x = mouseHitbox->x - 40;
                        menu.DraggableBox->y = mouseHitbox->y - 20;
                    }

                    ///END OF PAUSE COLLISION

                    menu.update();

                    //SDL_RenderDrawRect( window.gRenderer, mouseHitbox );
                }



                Secrettarget.hitBox->x -= camera->x;
                Secrettarget.hitBox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, Secrettarget.hitBox );
                Secrettarget.hitBox->x += camera->x;
                Secrettarget.hitBox->y += camera->y;

                SecretSpeed.hitBox->x -= camera->x;
                SecretSpeed.hitBox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, SecretSpeed.hitBox );
                SecretSpeed.hitBox->x += camera->x;
                SecretSpeed.hitBox->y += camera->y;





                SDL_RenderDrawLine(window.gRenderer, 4626 - camera->x , 767 - camera->y, 4913 - camera->x , 732 - camera->y);


				//Update screen
				SDL_RenderPresent( window.gRenderer );
				/******** rendering ********/

				++countedFrames;

				//If frame finished early
				int frameTicks = capTimer.getTicks();
				if( frameTicks < SCREEN_TICK_PER_FRAME )
				{
					//Wait remaining time
					SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
				}

				if(quit || !area_debug)
                {
                    gPlayer.free();
                    gPlayerPixelated.free();
                    gPlayerArmed.free();
                    gPlayerSemi.free();
                    gPlayerFace.free();
                    gGround.free();
                    gWall.free();
                    gTestBox.free();
                    gGroundBox.free();
                    gGroundBox2.free();
                    gBackground.free();
                    gTarget.free();
                    gBullet.free();
                    gHair.free();
                    gSemi_Auto.free();
                    ammo1.free();
                    ammo2.free();
                    hud_head.free();
                    hud_ammo.free();
                    hud_baseplate.free();
                    gZipline.free();
                    gZiplineBase.free();
                    gRobotOrig.free();
                    gRobotArmLeftOrig.free();
                    gRobotArmRightOrig.free();
                    gSnowman.free();
                    gSnowmanNose.free();
                    gOxygenTank.free();
                    gDemoDay.free();
                    gTutorial.free();
                    gTutorial2.free();
                    gTutorialbg.free();
                    gTutorialbg2.free();
                    gTutorial3.free();
                    gKongaCage.free();
                    gDarunggarBox.free();
                    gCeeButton.free();
                    gLedge.free();
                    gGranado.free();
                    gExplosionGeneric.free();
                    gArmoredTruckOrig.free();
                    gArmoredTruckCracksOrig.free();
                    gArmoredTruckDevilOrig.free();
                    gPauseMenu.free();
                    gDebugMenu.free();
                    gDebugSpawner.free();
                    gSpinner.free();
                    gBulletCavern.free();
                    gBulletCavernCage.free();
                    gBulletCavern2.free();
                    gNewMove.free();
                    gBullet2.free();
                    gRoboMaggot.free();
                    gTargets.free();
                    gSharkRobotOrig.free();
                    gKultentaOrig.free();
                    gTNTgentOrig.free();
                    gWermblsOrig.free();
                    gGarbageGaurdianOrig.free();
                    gKongaLeeOrig.free();
                    gLeverOff.free();
                    gLeverOn.free();
                    gTutorialZipline1.free();
                    gTutorialEnemy.free();
                    gHubWorld.free();
                    gHubWorld_BG.free();

                    text.free();
                    healthHUD.free();
                    level2Score.free();

                    //Free the music
                    Mix_FreeMusic( gMusic );
                    gMusic = NULL;

                    Mix_FreeChunk( aExplosion );
                    aExplosion = NULL;
                    Mix_FreeChunk( aSpeedBuff );
                    aSpeedBuff = NULL;
                    Mix_FreeChunk( aTvSize );
                    aTvSize = NULL;
                    Mix_FreeChunk( aledgeGrab );
                    aledgeGrab = NULL;
                    Mix_FreeChunk( aReload );
                    aReload = NULL;
                    Mix_FreeChunk( aShootSemi );
                    aShootSemi = NULL;
                    Mix_FreeChunk( aShootEmpty );
                    aShootEmpty = NULL;
                    Mix_FreeChunk( aGranadoPrimed );
                    aGranadoPrimed = NULL;
                    Mix_FreeChunk( aExplosion1 );
                    aExplosion1 = NULL;
                    Mix_FreeChunk( aCrack1 );
                    aCrack1 = NULL;
                    Mix_FreeChunk( aCrack2 );
                    aCrack2 = NULL;
                    Mix_FreeChunk( aCrack3 );
                    aCrack3 = NULL;
                    Mix_FreeChunk( aMissile );
                    aMissile = NULL;
                    Mix_FreeChunk( aRising );
                    aRising = NULL;
                    Mix_FreeChunk( aLand );
                    aLand = NULL;
                    Mix_FreeChunk( aShootMaggot );
                    aShootMaggot = NULL;
                    Mix_FreeChunk( aTeleMaggotLeft );
                    aTeleMaggotLeft = NULL;
                    Mix_FreeChunk( aTeleMaggotRight );
                    aTeleMaggotRight = NULL;
                    Mix_FreeChunk( aBreak1 );
                    aBreak1 = NULL;
                    Mix_FreeChunk( aBreak2 );
                    aBreak2 = NULL;
                    Mix_FreeChunk( aBreak3 );
                    aBreak3 = NULL;
                    Mix_FreeChunk( aSelect );
                    aSelect = NULL;
                    Mix_FreeChunk( aJellyDeath1 );
                    aJellyDeath1 = NULL;
                    Mix_FreeChunk( aJellyDeath2 );
                    aJellyDeath2 = NULL;
                    Mix_FreeChunk( aTNTgent_alert );
                    aTNTgent_alert = NULL;
                    Mix_FreeChunk( aTNTgent_toss );
                    aTNTgent_toss = NULL;
                    Mix_FreeChunk( aWermblONE );
                    aWermblONE = NULL;
                    Mix_FreeChunk( aWermblTWO );
                    aWermblTWO = NULL;
                    Mix_FreeChunk( aWermblTHREE );
                    aWermblTHREE = NULL;

                    //TTF_CloseFont( gFont );
                    gFont = NULL;
                }
            }
			}///end of debug area
			while(area_loading)
            {
                Texture gLoading;
                Texture gLoadingBG;

                if( !gLoading.loadFile("boardgame/loading_screen_icon2.png", window.gRenderer) )
                {
                    printf("Failed to load loading_screen_icon2.png");
                }

                if( !gLoadingBG.loadFile("boardgame/loadingbg.png", window.gRenderer) )
                {
                    printf("Failed to load loadingbg.png");
                }
                needToLoadShit = false;

                while(!needToLoadShit)
                {
                    now = SDL_GetTicks();
                    passed = now - last;
                    last = now;
                    accumulator += passed;

                    capTimer.start();

                    while (accumulator >= dt) {

                    float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
                    if( avgFPS > 2000000 )
                    {
                            avgFPS = 0;
                    }

                    //Calculate time step
                    //timeStep = getTimeScalar();
                    dt = 1000 / 60;
                    timeStep = 0.016;

                    while( SDL_PollEvent( &e ) != 0 )
                    {
                        //User requests quit
                        if( e.type == SDL_QUIT )
                        {
                            quit = true;
                        }
                        if(!player1.paused)
                        {
                            player1.handleEvent( e );
                            Mix_ResumeMusic();

                        }
                        else
                        {
                            player1.handleMenuEvents( e );
                            Mix_PauseMusic();
                        }
                    }

                    if(needToLoadShit)
                    {
                        switch(destination)
                        {
                            case 0:
                            break;
                            case 1: //processMedia(2); ///free memory from debug area
                                    //process_AREA1_media(1); ///load level 1
                                    destination = 0;
                            break;
                            case 2:
                            break;
                        }

                    }

                    accumulator -= dt;
                    }

                    SDL_RenderClear( window.gRenderer );
                    gLoadingBG.render(window.gRenderer, 0, 0);
                    gLoading.defaultAnimate(window.gRenderer, 1245, 690, 79, 86, 100);
                    SDL_RenderPresent( window.gRenderer );

                    if(loadTimer > 5)
                    {
                        loadTimer--;
                    }
                    else
                    {
                        area_loading = false;
                        area_1 = true;
                        needToSpawnShit = true;
                        needToLoadShit = true;
                        loadTimer = 0;
                        loadBGalpha = 0;
                        gLoading.free();
                        gLoadingBG.free();
                        printf("DJKDFASFGSBGHN how did i get here?");
                    }


                    ++countedFrames;

                    //If frame finished early
                    int frameTicks = capTimer.getTicks();
                    if( frameTicks < SCREEN_TICK_PER_FRAME )
                    {
                        //Wait remaining time
                        SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
                    }
                }
            }
            while(area_1 && !quit)
            {
                //{
                        //Scene textures
                        Texture gPlayer(65, 136);
                        Texture gPlayerPixelated(65, 136);
                        Texture gPlayerArmed(65, 136);
                        Texture gPlayerSemi;
                        Texture gPlayerFace;
                        Texture gGround;
                        Texture gTestBox;
                        Texture gGroundBox;
                        Texture gGroundBox2;
                        Texture ammo1(53, 59);
                        Texture ammo2;
                        Texture hud_head(507, 118);
                        Texture hud_baseplate(347, 27);
                        Texture hud_ammo(0, 23);
                        Texture gWall;
                        Texture gTarget;
                        Texture gBullet;
                        Texture gHair(86, 33);
                        Texture gSemi_Auto;
                        Texture gZipline;
                        Texture gZiplineBase;
                        Texture gTutorialZipline1;
                        Texture gTutorialEnemy;
                        printf("before static");
                        static const int numberOfRobots = 4;
                        printf("after static");
                        Texture gRobot[numberOfRobots];
                        Texture gRobotArmLeft[numberOfRobots];
                        Texture gRobotArmRight[numberOfRobots];
                        Texture gRobotOrig;
                        Texture gRobotArmLeftOrig;
                        Texture gRobotArmRightOrig;
                        Texture gSnowman;
                        Texture gSnowmanNose;
                        Texture gOxygenTank;
                        Texture gLedge;
                        Texture gGranado;
                        Texture gExplosionGeneric;
                        static const int numberOfTrucks = 2;
                        Texture gArmoredTruck[numberOfTrucks];
                        Texture gArmoredTruckCracks(17, 25);
                        Texture gArmoredTruckDevil(97, 12);
                        Texture2 gArmoredTruckOrig;
                        Texture2 gArmoredTruckCracksOrig;
                        Texture2 gArmoredTruckDevilOrig;
                        Texture gSpinner;
                        Texture gBulletCavern;
                        Texture gBulletCavernCage;
                        Texture gBulletCavern2;
                        Texture gNewMove;
                        Texture gBullet2;
                        Texture gRoboMaggot;
                        Texture gTargets;
                        static const short  int numberOfSharkRobots = 2;
                        Texture gSharkRobot[numberOfSharkRobots];
                        Texture gSharkRobotOrig;
                        static const short  int numberOfKultentas = 2;
                        Texture gKultenta[numberOfKultentas];
                        Texture gKultentaEvilArm[numberOfKultentas];
                        Texture gKultentaGun[numberOfKultentas];
                        Texture gKultentaOrig;
                        static const short  int numberOfTNTgent = 1;
                        Texture gTNTgent[numberOfTNTgent];
                        Texture gTNTgentOrig;
                        static const short  int numberOfWermbls = 1;
                        Texture gWermbls[numberOfWermbls];
                        Texture gWermblsOrig;
                        static const short  int numberOfGarbageGaurdians = 1;
                        Texture gGarbageGaurdians[numberOfGarbageGaurdians];
                        Texture gGarbageGaurdianOrig;
                        static const short  int numberOfKongaLees = 2;
                        Texture gKongaLees[numberOfKongaLees];
                        Texture gKongaLeeOrig;
                        static const short  int numberOfSorse = 1;
                        Texture gSorse[numberOfSorse];
                        Texture gSorseOrig;

                        Texture gCeeButton;
                        Texture gPauseMenu;
                        Texture gDebugMenu;
                        Texture gDebugSpawner;
                        Texture gDemoDay;
                        Texture gTutorial;
                        Texture gTutorial2;
                        Texture gTutorialbg;
                        Texture gTutorialbg2;
                        Texture gTutorial3;
                        Texture gKongaCage;
                        Texture gDarunggarBox;
                        Texture gHubWorld;
                        Texture gHubWorld_BG;

                        Texture gLoading;
                        Texture gLoadingBG;

                        Texture gLeverOn;
                        Texture gLeverOff;

                        Texture text;
                        Texture healthHUD;
                        Texture level2Score;

                        Mix_Music *gMusic = NULL;
                        Mix_Chunk *aExplosion = NULL;
                        Mix_Chunk *aSpeedBuff = NULL;
                        Mix_Chunk *aTvSize = NULL;
                        Mix_Chunk *aledgeGrab = NULL;
                        Mix_Chunk *aReload = NULL;
                        Mix_Chunk *aShootSemi = NULL;
                        Mix_Chunk *aShootEmpty = NULL;
                        Mix_Chunk *aGranadoPrimed = NULL;
                        Mix_Chunk *aExplosion1 = NULL;
                        Mix_Chunk *aCrack1 = NULL;
                        Mix_Chunk *aCrack2 = NULL;
                        Mix_Chunk *aCrack3 = NULL;
                        Mix_Chunk *aMissile = NULL;
                        Mix_Chunk *aRising = NULL;
                        Mix_Chunk *aLand = NULL;
                        Mix_Chunk *aShootMaggot = NULL;
                        Mix_Chunk *aTeleMaggotRight = NULL;
                        Mix_Chunk *aTeleMaggotLeft = NULL;
                        Mix_Chunk *aBreak1 = NULL;
                        Mix_Chunk *aBreak2 = NULL;
                        Mix_Chunk *aBreak3 = NULL;
                        Mix_Chunk *aSelect = NULL;
                        Mix_Chunk *aJellyDeath1 = NULL;
                        Mix_Chunk *aJellyDeath2 = NULL;
                        Mix_Chunk *aTNTgent_alert = NULL;
                        Mix_Chunk *aTNTgent_toss = NULL;
                        Mix_Chunk *aWermblONE = NULL;
                        Mix_Chunk *aWermblTWO = NULL;
                        Mix_Chunk *aWermblTHREE = NULL;


                        Texture gBackground;

                        if(1) ///LOAD ALL THE SHIT
                        {
                            //Loading success flag
                            bool success = true;

                            if( !gPlayer.loadFile("boardgame/player1_unarmed_clean.png", window.gRenderer) )
                            {  printf("Failed to load player1spritesheet.png");
                                success = false;
                            }

                            if( !gPlayerPixelated.loadFile("boardgame/test2_tvsize.png", window.gRenderer) )
                            {  printf("Failed to load test2_tvsize.png");
                                success = false;
                            }

                            if( !gPlayerArmed.loadFile("boardgame/player1_gunbody.png", window.gRenderer) )
                            {  printf("Failed to load player1_gunbody.png");
                                success = false;
                            }

                            if( !gPlayerSemi.loadFile("boardgame/playersemi.png", window.gRenderer) )
                            {  printf("Failed to load semi_auto_sheet_test.png");
                                success = false;
                            }

                            if( !gPlayerFace.loadFile("boardgame/player1_face.png", window.gRenderer) )
                            {  printf("Failed to load player1_face2.png");
                                success = false;
                            }

                            if( !ammo1.loadFile("boardgame/delete_this.png", window.gRenderer) )
                            {
                                printf("Failed to load delete_this.png");
                                success = false;
                            }

                            if( !ammo2.loadFile("boardgame/delete_this2.png", window.gRenderer) )
                            {
                                printf("Failed to load delete_this2.png");
                                success = false;
                            }

                            if( !hud_head.loadFile("boardgame/hud_head.png", window.gRenderer) )
                            {
                                printf("Failed to load hud_head.png");
                                success = false;
                            }

                            if( !hud_baseplate.loadFile("boardgame/hud_baseplate.png", window.gRenderer) )
                            {
                                printf("Failed to load hud_baseplate.png");
                                success = false;
                            }

                            if( !hud_ammo.loadFile("boardgame/hud_ammo.png", window.gRenderer) )
                            {
                                printf("Failed to load hud_ammo.png");
                                success = false;
                            }

                            if( !gBackground.loadFile("boardgame/tfijaf7.png", window.gRenderer) )
                            {
                                printf("Failed to load tfijaf.png");
                                success = false;
                            }

                            if( !gGround.loadFile("boardgame/ground.png", window.gRenderer) )
                            {
                                printf("Failed to load ground.png");
                                success = false;
                            }

                            if( !gWall.loadFile("boardgame/wall.png", window.gRenderer) )
                            {
                                printf("Failed to load wa;;.png");
                                success = false;
                            }

                            if( !gTarget.loadFile("boardgame/aggydagger.png", window.gRenderer) )
                            {
                                printf("Failed to load teargpng");
                                success = false;
                            }

                            if( !gTestBox.loadFile("boardgame/testbox.png", window.gRenderer) )
                            {
                                printf("Failed to load testbox.png");
                                success = false;
                            }

                            if( !gGroundBox.loadFile("boardgame/groundbox.png", window.gRenderer) )
                            {
                                printf("Failed to load groundbox.png");
                                success = false;
                            }

                            if( !gGroundBox2.loadFile("boardgame/groundbox2.png", window.gRenderer) )
                            {
                                printf("Failed to load groundbox2.png");
                                success = false;
                            }

                            if( !gBullet.loadFile("boardgame/bullet_round.png", window.gRenderer) )
                            {
                                printf("Failed to load bejjytr.png");
                                success = false;
                            }

                            if( !gHair.loadFile("boardgame/hair_black.png", window.gRenderer) )
                            {  printf("Failed to load hair.png");
                                success = false;
                            }

                            if( !gSemi_Auto.loadFile("boardgame/semi_auto2.png", window.gRenderer) )
                            {  printf("Failed to load semi_auto.png");
                                success = false;
                            }

                            if( !gZipline.loadFile("boardgame/zipline_spritesheet.png", window.gRenderer) )
                            {  printf("Failed to load zipline_spritesheet.png");
                                success = false;
                            }

                            if( !gZiplineBase.loadFile("boardgame/useless_wire_delete_this.png", window.gRenderer) )
                            {  printf("Failed to load useless_aesthetic_wire_delete_this.png");
                                success = false;
                            }

                            if( !gTutorialZipline1.loadFile("boardgame/tutorialzipline1.png", window.gRenderer) )
                            {  printf("Failed to load tutorialzipline1.png");
                                success = false;
                            }
                            if( !gTutorialEnemy.loadFile("boardgame/tutorialenemy.png", window.gRenderer) )
                            {  printf("Failed to load tutorialenemy.png");
                                success = false;
                            }

                            if( !gRobotOrig.loadFile("boardgame/robot_body_deadtest.png", window.gRenderer) )
                            {  printf("Failed to load robot_body.png");
                                success = false;
                            }

                            if( !gRobotArmLeftOrig.loadFile("boardgame/robot_leftarm.png", window.gRenderer) )
                            {  printf("Failed to load robot_leftarm.png");
                                success = false;
                            }

                            if( !gRobotArmRightOrig.loadFile("boardgame/robot_rightarm.png", window.gRenderer) )
                            {  printf("Failed to load robot_rightarm.png");
                                success = false;
                            }

                            if( !gSnowman.loadFile("boardgame/snowman.png", window.gRenderer) )
                            {  printf("Failed to load snowman.png");
                                success = false;
                            }

                            if( !gSnowmanNose.loadFile("boardgame/snowman_nose.png", window.gRenderer) )
                            {  printf("Failed to load snowman_nose.png");
                                success = false;
                            }

                            if( !gOxygenTank.loadFile("boardgame/oxygen_tank.png", window.gRenderer) )
                            {  printf("Failed to load oxygen_tank.png");
                                success = false;
                            }

                            if( !gDemoDay.loadFile("boardgame/demoday.png", window.gRenderer) )
                            {  printf("Failed to load demoday.png");
                                success = false;
                            }

                            if( !gTutorial.loadFile("boardgame/tutorial.png", window.gRenderer) )
                            {  printf("Failed to load tutorial.png");
                                success = false;
                            }

                            if( !gTutorial2.loadFile("boardgame/tutorial2.png", window.gRenderer) )
                            {  printf("Failed to load tutorial2.png");
                                success = false;
                            }

                            if( !gTutorialbg.loadFile("boardgame/tutorialbg.png", window.gRenderer) )
                            {  printf("Failed to load tutorialbg.png");
                                success = false;
                            }

                            if( !gTutorialbg2.loadFile("boardgame/tutorialbg2.png", window.gRenderer) )
                            {  printf("Failed to load tutorialbg2.png");
                                success = false;
                            }

                            if( !gTutorial3.loadFile("boardgame/tutorial3.png", window.gRenderer) )
                            {  printf("Failed to load tutorial3.png");
                                success = false;
                            }

                            if( !gKongaCage.loadFile("boardgame/kongacage.png", window.gRenderer) )
                            {  printf("Failed to load kongacage.png");
                                success = false;
                            }

                            if( !gDarunggarBox.loadFile("boardgame/darunggar.png", window.gRenderer) )
                            {  printf("Failed to load darunggar.png");
                                success = false;
                            }

                            if( !gCeeButton.loadFile("boardgame/c_button_indicator_thingy.png", window.gRenderer) )
                            {  printf("Failed to load c_button_indicator_thingy.png");
                                success = false;
                            }

                            if( !gLedge.loadFile("boardgame/ledge.png", window.gRenderer) )
                            {  printf("Failed to load ledge.png");
                                success = false;
                            }

                            if( !gGranado.loadFile("boardgame/granado.png", window.gRenderer) )
                            {  printf("Failed to load granado.png");
                                success = false;
                            }

                            if( !gExplosionGeneric.loadFile("boardgame/explosion_generic.png", window.gRenderer) )
                            {  printf("Failed to load explosion_generic.png");
                                success = false;
                            }

                            if( !gArmoredTruckOrig.loadFile("boardgame/armored_truck2.png", window.gRenderer) )
                            {  printf("Failed to load aaaaa.png");
                                success = false;
                            }

                            if( !gArmoredTruckCracksOrig.loadFile("boardgame/armored_truck_crack1.png", window.gRenderer) )
                            {  printf("Failed to load aaaaa.png");
                                success = false;
                            }

                            if( !gArmoredTruckDevilOrig.loadFile("boardgame/devilman_and_crybaby.png", window.gRenderer) )
                            {  printf("Failed to load devilman_and_crybaby.png.png");
                                success = false;
                            }

                            if( !gPauseMenu.loadFile("boardgame/pause_menu.png", window.gRenderer) )
                            {  printf("Failed to load pause_menu.png");
                                success = false;
                            }

                            if( !gDebugMenu.loadFile("boardgame/debug_menu.png", window.gRenderer) )
                            {  printf("Failed to load debug_menu.png");
                                success = false;
                            }

                            if( !gDebugSpawner.loadFile("boardgame/debug_spawner.png", window.gRenderer) )
                            {  printf("Failed to load debug_spawner.png");
                                success = false;
                            }

                            if( !gSpinner.loadFile("boardgame/loading_screen_icon.png", window.gRenderer) )
                            {  printf("Failed to load loading_screen_icon.png");
                                success = false;
                            }

                            if( !gBulletCavernCage.loadFile("boardgame/bullet_cavern_cage.png", window.gRenderer) )
                            {  printf("Failed to load bullet_cavern_cage.png");
                                success = false;
                            }

                            if( !gBulletCavern2.loadFile("boardgame/bullet_cavern2.png", window.gRenderer) )
                            {  printf("Failed to load bullet_cavern2.png");
                                success = false;
                            }

                            if( !gBulletCavern.loadFile("boardgame/bullet_cavern.png", window.gRenderer) )
                            {  printf("Failed to load bullet_cavern.png");
                                success = false;
                            }

                            if( !gNewMove.loadFile("boardgame/newmove.png", window.gRenderer) )
                            {  printf("Failed to load newmove.png");
                                success = false;
                            }

                            if( !gBullet2.loadFile("boardgame/bullet_enemy.png", window.gRenderer) )
                            {  printf("Failed to load bullet_enemy.png");
                                success = false;
                            }

                            if( !gRoboMaggot.loadFile("boardgame/robo_maggot.png", window.gRenderer) )
                            {  printf("Failed to load robo_maggot.png");
                                success = false;
                            }

                            if( !gTargets.loadFile("boardgame/targets_clean.png", window.gRenderer) )
                            {  printf("Failed to load targets_clean.png");
                                success = false;
                            }

                            if( !gSharkRobotOrig.loadFile("boardgame/SharkRobot.png", window.gRenderer) )
                            {  printf("Failed to load SharkRobot.png");
                                success = false;
                            }

                            if( !gKultentaOrig.loadFile("boardgame/kultenta.png", window.gRenderer) )
                            {  printf("Failed to load kultenta.png");
                                success = false;
                            }

                            if( !gTNTgentOrig.loadFile("boardgame/TNTgent_clean.png", window.gRenderer) )
                            {  printf("Failed to load TNTgent.png");
                                success = false;
                            }

                            if( !gWermblsOrig.loadFile("boardgame/Wermbl_clean.png", window.gRenderer) )
                            {  printf("Failed to load Wermbl_clean.png");
                                success = false;
                            }

                            if( !gGarbageGaurdianOrig.loadFile("boardgame/Garbage_Gaurdian.png", window.gRenderer) )
                            {  printf("Failed to load Garbage_Gaurdian.png");
                                success = false;
                            }

                            if( !gKongaLeeOrig.loadFile("boardgame/KongaLee.png", window.gRenderer) )
                            {  printf("Failed to load KongaLee_clean.png");
                                success = false;
                            }

                            if( !gSorseOrig.loadFile("boardgame/Sorse_Clean.png", window.gRenderer) )
                            {  printf("Failed to load sorse_clean.png");
                                success = false;
                            }

                            if( !gLeverOff.loadFile("boardgame/leveroff.png", window.gRenderer) )
                            {  printf("Failed to load leveroff.png");
                                success = false;
                            }

                            if( !gLeverOn.loadFile("boardgame/leveron.png", window.gRenderer) )
                            {  printf("Failed to load leveron.png");
                                success = false;
                            }

                            ///NEW SHIT ADDED AFTER HRB-8

                            if( !gHubWorld.loadFile("boardgame/hubworld.png", window.gRenderer) )
                            {  printf("Failed to load hubworld.png");
                                success = false;
                            }

                            if( !gHubWorld_BG.loadFile("boardgame/hubworld_BG.png", window.gRenderer) )
                            {  printf("Failed to load hubworld_BG.png");
                                success = false;
                            }

                            if( !gLoading.loadFile("boardgame/loading_screen_icon2.png", window.gRenderer) )
                            {  printf("Failed to load loading_screen_icon2.png");
                                success = false;
                            }

                            if( !gLoadingBG.loadFile("boardgame/loadingbg.png", window.gRenderer) )
                            {  printf("Failed to load loadingbg.png");
                                success = false;
                            }

                            //Open the font
                            gFont = TTF_OpenFont( "boardgame/FreeSans.ttf", 28 );
                            if( gFont == NULL )
                            {
                                printf( "Failed to load FreeSans.ttf SDL_ttf Error: %s\n", TTF_GetError() );
                                success = false;
                            }

                            ///SOUUNNNDd
                            //SSOOOOYBD
                            /////sound
                            //Load music
                            gMusic = Mix_LoadMUS( "boardgame/HAPPY AGDG DEMO DAY 3!!.mp3" );  ///uncomment this l8r
                            if( gMusic == NULL )
                            {
                                printf( "Failed to load psycho music! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aExplosion = Mix_LoadWAV( "boardgame/bang.wav" );
                            if( aExplosion == NULL )
                            {
                                printf( "Failed to load explosion sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aSpeedBuff = Mix_LoadWAV( "boardgame/Peace Vox.wav");
                            if( aSpeedBuff == NULL )
                            {
                                printf( "Failed to load speed buff sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aTvSize = Mix_LoadWAV( "boardgame/Blue Synth.wav");
                            if( aTvSize == NULL )
                            {
                                printf( "Failed to load tv size sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aledgeGrab = Mix_LoadWAV( "boardgame/ledgegrab.wav");
                            if( aledgeGrab == NULL )
                            {
                                printf( "Failed to load ledge grab sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aReload = Mix_LoadWAV( "boardgame/reloading.wav");
                            if( aReload == NULL )
                            {
                                printf( "Failed to load reloading sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aShootSemi = Mix_LoadWAV( "boardgame/shoot_semi.wav");
                            if( aShootSemi == NULL )
                            {
                                printf( "Failed to load semi shoot sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aShootEmpty = Mix_LoadWAV( "boardgame/shoot_empty.wav");
                            if( aShootEmpty == NULL )
                            {
                                printf( "Failed to load empty gun shoot sound effect! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aGranadoPrimed = Mix_LoadWAV( "boardgame/grenado_primed.wav");
                            if( aGranadoPrimed == NULL )
                            {
                                printf( "Failed to load grenado_primed.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aExplosion1 = Mix_LoadWAV( "boardgame/explosion_1.wav");
                            if( aExplosion1 == NULL )
                            {
                                printf( "Failed to load explosion_1.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aCrack1 = Mix_LoadWAV( "boardgame/crack1.mp3");
                            if( aCrack1 == NULL )
                            {
                                printf( "Failed to load crack1.mp3! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aCrack2 = Mix_LoadWAV( "boardgame/crack2.mp3");
                            if( aCrack2 == NULL )
                            {
                                printf( "Failed to load crack2.mp3! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aCrack3 = Mix_LoadWAV( "boardgame/crack3.mp3");
                            if( aCrack3 == NULL )
                            {
                                printf( "Failed to load crack3.mp3! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aMissile = Mix_LoadWAV( "boardgame/audio_player_spinkick.wav");
                            if( aMissile == NULL )
                            {
                                printf( "Failed to load audio_player_spinkick.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aRising = Mix_LoadWAV( "boardgame/audio_player_getup.mp3");
                            if( aRising == NULL )
                            {
                                printf( "Failed to load audio_player_getup.mp3! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aLand = Mix_LoadWAV( "boardgame/Hit_Hurt16.wav");
                            if( aLand == NULL )
                            {
                                printf( "Failed to load Hit_Hurt16.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aShootMaggot = Mix_LoadWAV( "boardgame/maggot_shoot.wav");
                            if( aShootMaggot == NULL )
                            {
                                printf( "Failed to load maggot_shoot.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aTeleMaggotRight = Mix_LoadWAV( "boardgame/maggot_tele_right.wav");
                            if( aTeleMaggotRight == NULL )
                            {
                                printf( "Failed to load maggot_tele_right.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aTeleMaggotLeft = Mix_LoadWAV( "boardgame/maggot_tele_left.wav");
                            if( aTeleMaggotLeft == NULL )
                            {
                                printf( "Failed to load maggot_tele_left.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aBreak1 = Mix_LoadWAV( "boardgame/break1.wav");
                            if( aBreak1 == NULL )
                            {
                                printf( "Failed to load break1.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aBreak2 = Mix_LoadWAV( "boardgame/break2.wav");
                            if( aBreak2 == NULL )
                            {
                                printf( "Failed to load break2.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aBreak3 = Mix_LoadWAV( "boardgame/break3.wav");
                            if( aBreak3 == NULL )
                            {
                                printf( "Failed to load break3.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aSelect = Mix_LoadWAV( "boardgame/select.wav");
                            if( aSelect == NULL )
                            {
                                printf( "Failed to load select.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aJellyDeath1 = Mix_LoadWAV( "boardgame/jellydeath1.wav");
                            if( aJellyDeath1 == NULL )
                            {
                                printf( "Failed to load jellydeath1.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aJellyDeath2 = Mix_LoadWAV( "boardgame/jellydeath2.wav");
                            if( aJellyDeath2 == NULL )
                            {
                                printf( "Failed to load jellydeath2.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aTNTgent_alert = Mix_LoadWAV( "boardgame/aTNTgent_alert.wav");
                            if( aTNTgent_alert == NULL )
                            {
                                printf( "Failed to load aTNTgent_alert.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aTNTgent_toss = Mix_LoadWAV( "boardgame/aTNTgent_toss.wav");
                            if( aTNTgent_toss == NULL )
                            {
                                printf( "Failed to load aTNTgent_toss.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aWermblONE = Mix_LoadWAV( "boardgame/a_Wermbl_One.wav");
                            if( aWermblONE == NULL )
                            {
                                printf( "Failed to load a_Wermbl_One.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aWermblTWO = Mix_LoadWAV( "boardgame/a_Wermbl_Two.wav");
                            if( aWermblTWO == NULL )
                            {
                                printf( "Failed to load a_Wermbl_Two.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            aWermblTHREE = Mix_LoadWAV( "boardgame/a_Wermbl_Three.wav");
                            if( aWermblTHREE == NULL )
                            {
                                printf( "Failed to load a_Wermbl_Three.wav! SDL_mixer Error: %s\n", Mix_GetError() );
                                success = false;
                            }

                            if( !success )
                            {
                                printf( "Failed to load media!\n" );
                                quit = true;
                            }
                        }

                        FlatSurface ground(10, 450, 1400, 50, 0, 449);
                        FlatSurface ground2(2245, 845, 1400, 50, 2235, 844);
                        FlatSurface ground3(3740, 845, 1400, 50, 3730, 844);
                        FlatSurface leftWall(0, 0, 20, 450, 0, 0);
                        FlatSurface target(-228, -580, 20, 10, -238, -1616);
                        FlatSurface Secrettarget(-3007, -900, 30, 30, -325, 297);
                        //FlatSurface SecretSpeed(5267, -125, 30, 30, 0, 0);
                        FlatSurface semi_auto(-2920, -550, 20, 20, -2920, -550); ///DELETE LITERALLY EVERY ONE OF THESE HOLY SHIT
                        FlatSurface oxygentank( 3840, 774, 150, 70, 3888, 776);
                        FlatSurface bulletCageTrigger( 7600, -360, 150, 450, 0, 776);
                        FlatSurface level1Trigger( 7085, -363, 30, 20, 0, 0);
                        FlatSurface level2Trigger( 8127, -356, 30, 20, 0, 0);
                        FlatSurface tutorialSecret( -4046, -924, 100, 240, 0, 0);


                        FlatSurface lever(3100, 400, 80, 50, 3100, 400);

                        static const short  int NumberOfTestBoxes =1;
                        FlatSurface testBox[NumberOfTestBoxes] =
                        {
                            FlatSurface(-3900, -2102, 799, 56, 0, 0), //hub world bottom
                        };

                        static const short  int NumberOfTestSlopes = 2;
                        NonFlatSurface testSlopes[NumberOfTestSlopes] =
                        {
                            NonFlatSurface(3821, -1052, 55, 8, 0, 0),
                            NonFlatSurface(4260, -1107, 55, 4, 0, 0)
                        };

                        Ledge ledges[2] =
                        {
                            Ledge(4500, 500, true),
                            Ledge(4900, 500, false)
                        };

                        TargetMaster targetGame(7050, -150); //8225, 50 /// 1175, 200

                        Bullet player1bullets[20];
                        Bullet enemyBullets[20];

                        Zipline zipline1(1372, 200);
                        RoboMaggot maggot(8254, -27);

                        static const short  int NumberOfDoors = 3;
                        Door doors[NumberOfDoors] =
                        {
                            Door(-3680, -2243, 0), //debug
                            Door(-3498, -2243, 1), //level 1
                            Door(-3316, -2243, 2) //tutorial
                        };

                        Enemy robots[numberOfRobots] =
                        {
                            Enemy(2656, 679),
                            Enemy(2856, 679),
                            Enemy(-1729, -565),
                            Enemy(4143, -546)
                        };

                        Snowman snowman1(3600, 679);

                        //static const short int ass = 1;
                        ArmoredTruck trucks[numberOfTrucks] =
                        {
                          ArmoredTruck(5410, 330),
                          ArmoredTruck(6110, 330)
                        };

                        SharkRobot sharkRobots[numberOfSharkRobots] =
                        {
                          SharkRobot(574, -1179),
                          SharkRobot(1000, -1179)
                        };

                        Kultenta kultentas[numberOfKultentas] =
                        {
                          Kultenta(1574, -1188),
                          Kultenta(2000, -1188)
                        };

                        TNTgent gents[numberOfTNTgent] =
                        {
                          TNTgent(2430, -1188)
                        };

                        Wermbl wermbls[numberOfWermbls] =
                        {
                          Wermbl(2774, -1188)
                        };

                        GarbageGaurdian garbageGaurdians[numberOfGarbageGaurdians] =
                        {
                          GarbageGaurdian(3544, -1198)
                        };

                        KongaLee kongaLees[numberOfKongaLees] =
                        {
                          KongaLee(3130, -2800, 2),
                          KongaLee(3135, -1525, 0)
                        };

                        Sorse sorse[numberOfSorse] =
                        {
                          Sorse(2890, -1270)
                        };
                        needToSpawnShit = false;
                        needToLoadShit = false;
                        printf("done loading level 1");

                while(!needToSpawnShit && !quit)
                {
			    now = SDL_GetTicks();
			    passed = now - last;
			    last = now;
			    accumulator += passed;

			    capTimer.start();

                while (accumulator >= dt) {

                player1.clicky = false; ///if i don't put these here SDL will just assume the mouse is still clicking if there's no other SDL_PollEvents to update it and make it actually fucking check if its still clicking, idfk this shit sux
                player1.unclicky = false;
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					if(!player1.paused)
                    {
                        player1.handleEvent( e );
                        Mix_ResumeMusic();

                    }
                    else
                    {
                        player1.handleMenuEvents( e );
                        Mix_PauseMusic();
                    }


				}

				if(player1.paused) ///count the time spent while game is paused
                {
                    if(timeSpentWhilePausedLol.isStarted())
                    {
                        timeSpentWhilePausedLol.unpause();
                    }
                    else
                    {
                        timeSpentWhilePausedLol.start();
                    }
                }
                else
                {
                    timeSpentWhilePausedLol.pause();
                }

                ///this lets the timers know how much time the game spent while paused,
				///ya this is rly the best solution i could come up with for this basically fuck fuck fuck

				///PLAYER TIMERS
				player1.reloadTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				player1.jumpTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				player1.testLeftTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				player1.testRightTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				player1.gunTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				player1.meleeTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				player1.granadoTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				player1.blastJumpTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				player1.risingTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				player1.ledgeTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerSemi.animationTimer2.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerSemi.animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayer.animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayer.animationTimer2.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayer.animationTimer3.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerArmed.animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerArmed.animationTimer2.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerArmed.animationTimer3.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerArmed.animationTimer4.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerArmed.animationTimer5.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerArmed.animationTimer6.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerArmed.animationTimer7.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerArmed.animationTimer8.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gPlayerFace.animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gHair.animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				for(int i = 0; i < 19; i++)
                {
                    player1bullets[i].testTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    player1bullets[i].deletionTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                }


				///SNOWMAN TIMERS
				snowman1.explosionTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				snowman1.respawnTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				snowman1.shootTime.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gSnowman.animationTimer4.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gSnowman.animationTimer5.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gSnowman.animationTimer6.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gSnowmanNose.animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gSnowmanNose.animationTimer4.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
				gSnowmanNose.animationTimer5.gamePausedTicks = timeSpentWhilePausedLol.getTicks();

				///ROBOT TIMERS
				for(int i = 0; i < numberOfRobots; i++)
                {
                    gRobot[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gRobotArmLeft[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gRobotArmRight[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                }

                ///ZIPLINE TIMERS
                gZipline.animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();

                ///ARMORED TRUCK TIMERS
                for(int i = 0; i < numberOfTrucks; i++)
                {
                    trucks[i].triggeredTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    trucks[i].respawnTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gArmoredTruck[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                }

                ///level 1 & 2 target timers
                for(int i = 0; i < 15; i++)
                {
                    targetGame.targets[i].funTimerlol.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                }
                targetGame.funTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                maggot.volleyTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                maggot.bufferTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();

                ///robot shark timers
                for(int i = 0; i < numberOfSharkRobots; i++)
                {
                    gSharkRobot[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gSharkRobot[i].animationTimer2.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    sharkRobots[i].jumpTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    sharkRobots[i].beingShotInTheFaceTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                }

                ///kultenta timers
                for(int i = 0; i < numberOfKultentas; i++)
                {
                    gKultenta[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gKultentaEvilArm[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gKultentaGun[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    kultentas[i].animationTime.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    kultentas[i].rootyTootyTimetoShootyTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    kultentas[i].replaceTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    kultentas[i].replaceTimer2.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    kultentas[i].replaceTimer3.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    kultentas[i].replaceTimer4.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    kultentas[i].replaceTimer5.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                }

                ///TNT gent timers
                for(int i = 0; i < numberOfTNTgent; i++)
                {
                    gTNTgent[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gTNTgent[i].animationTimer2.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gTNTgent[i].animationTimer3.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gTNTgent[i].animationTimer4.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gTNTgent[i].animationTimer7.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gents[i].programTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gents[i].speakingTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gents[i].hitColorHead.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gents[i].explosionTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    //uhhh this should be all of them
                }

                ///Wermbl timers
                for(int i = 0; i < numberOfWermbls; i++)
                {
                    gWermbls[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gWermbls[i].animationTimer2.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gWermbls[i].animationTimer4.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gWermbls[i].animationTimer5.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    wermbls[i].gachaActivationTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    wermbls[i].shootTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    wermbls[i].explosionTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                }

                ///Garbage Gaurdian timers
                for(int i = 0; i < numberOfGarbageGaurdians; i++)
                {
                    gGarbageGaurdians[i].animationTimer3.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    garbageGaurdians[i].animationTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    garbageGaurdians[i].hitColorBody.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                }

                ///Konga Lee timers
                for(int i = 0; i < numberOfKongaLees; i++)
                {
                    gKongaLees[i].animationTimer1.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gKongaLees[i].animationTimer2.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    gKongaLees[i].animationTimer4.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    kongaLees[i].explosionTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                    kongaLees[i].shootTimer.gamePausedTicks = timeSpentWhilePausedLol.getTicks();
                }




				/*********TIME SHIT*********/
                //Calculate and correct fps
                float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
                if( avgFPS > 2000000 )
                {
                        avgFPS = 0;
                }

				//Calculate time step
				//timeStep = getTimeScalar();
				dt = 1000 / 60;
				timeStep = 0.016;
                /*********TIME SHIT*********/

                /********* everything *********/

                if(!player1.paused)
                {
                player1.move( timeStep );
                if(player1.shoot  && player1.clip > 0)
                {
                    player1bullets[player1.bullet].restart(player1.hitBox->x, player1.hitBox->y, player1.gunDirection, player1.flip);
                    player1.clip--;
                }


                gPlayerSemi.setColor(0xFF, 0xFF, 0xFF);
                gPlayerArmed.setColor(0xFF, 0xFF, 0xFF);



                for(int i = 0; i < numberOfRobots; i++)
                {
                    robots[i].move();
                }
                snowman1.move(gSnowman.animationBool, gSnowmanNose.animationBool, player1.hitBox->x, player1.hitBox->y, gSnowman.animationBool3);
                for(int i = 0; i < 19; i++)
                {
                    player1bullets[i].move( timeStep );
                }

                zipline1.move( timeStep );

                if(player1.hitBox->y > 1000)
                {
                    player1.velocity = -3800;
                    player1.yVelocity = -790;
                    player1.hasGun = false;

                    zipline1.docked = 1;
                    zipline1.xVelocity = zipline1.linePosX;
                    zipline1.yVelocity = zipline1.linePosY;
                }

                for(int i = 0; i < numberOfTrucks; i++)
                {
                    trucks[i].move();
                }

                for(int i = 0; i < numberOfSharkRobots; i++)
                {
                    sharkRobots[i].move();
                }

                for(int i = 0; i < numberOfKultentas; i++)
                {
                    kultentas[i].move();
                }

                for(int i = 0; i < numberOfTNTgent; i++)
                {
                    gents[i].move(gTNTgent[i].animationInt);
                }

                for(int i = 0; i < numberOfWermbls; i++)
                {
                    wermbls[i].move();
                }

                for(int i = 0; i < numberOfGarbageGaurdians; i++)
                {
                    garbageGaurdians[i].move();
                }

                for(int i = 0; i < numberOfKongaLees; i++)
                {
                    kongaLees[i].move();
                }

                for(int i = 0; i < numberOfSorse; i++)
                {
                    sorse[i].move();
                }

                maggot.move();
                targetGame.move();
                for(int i = 0; i < 15; i++)
                {
                    targetGame.targets[i].move();
                }
                if(maggot.shoot )
                {
                    if(maggot.flip == SDL_FLIP_NONE)
                    {
                        enemyBullets[maggot.bullet].restart2(maggot.hitBox->x, maggot.hitBox->y, maggot.direction);
                    }
                    else
                    {
                        enemyBullets[maggot.bullet].restart2(maggot.hitBox->x + 20, maggot.hitBox->y, maggot.direction);
                    }
                    Mix_PlayChannel( -1, aShootMaggot, 0 );
                }
                for(int i = 0; i < 19; i++)
                {
                    enemyBullets[i].move2();
                }

                ///shady ass collision code
                //shady ass collision code
                ///shady ass collision code
                for(int i = 0; i < NumberOfTestBoxes; i++)
                {
                  /*  if(!player1.released && player1.jumping)
                    {
                        switch ( checkCollision(player1.hitBox, testBox[i].hitBox, 7) ) ///collision for player hitting test boxes
                        {
                            case 1: player1.yVelocity = testBox[i].hitBox->y - player1.hitBox->h + 1;
                                    player1.foo = true;
                                    if(player1.jumpTimer.getTicks() > 40) {player1.jumping = false; player1.jumpTimer.stop(); }
                                break;                                                        //+1?
                            case 2: player1.velocity = testBox[i].hitBox->x - player1.hitBox->w +1;
                                    player1.bar = true;
                                break;
                            case 3: player1.velocity = testBox[i].hitBox->x + testBox[i].hitBox->w -1;
                                    player1.baz = true;
                                break;
                            case 4: player1.yVelocity = testBox[i].hitBox->y + testBox[i].hitBox->h;
                                    player1.released = true;
                                break;
                            case 5: //do nothing
                                break;
                        }
                    }
                    else
                    {*/
                        switch ( checkCollision(player1.hitBox, testBox[i].hitBox, 2) ) ///collision for player and test boxes
                        {
                            case 1: player1.yVelocity = testBox[i].hitBox->y - player1.hitBox->h + 1;
                                    player1.foo = true;
                                    if(player1.jumpTimer.getTicks() > 40) {player1.jumping = false; player1.jumpTimer.stop(); }
                                break;                                                        //+1?
                            case 2: player1.velocity = testBox[i].hitBox->x - player1.hitBox->w +1;
                                    player1.bar = true;
                                break;
                            case 3: player1.velocity = testBox[i].hitBox->x + testBox[i].hitBox->w -1;
                                    player1.baz = true;
                                break;
                            case 4: player1.yVelocity = testBox[i].hitBox->y + testBox[i].hitBox->h + 0;
                                    player1.released = true;
                                    player1.ect = true;
                                    if(player1.blastJumping) { gPlayerSemi.setColor(255, 0, 0); gPlayerArmed.setColor(255, 0, 0); player1.blastJumpTimer.stop(); player1.blastJumping = false; player1.ledgeLeaping = false; player1.ledgeTimer.stop(); };
                                break;
                            case 5: //do nothing

                                break;
                       // }
                    }

                }

                for(int i = 0; i < NumberOfTestSlopes; i++)
                {
                    if(player1.hitBox->x + player1.hitBox->w / 2 > testSlopes[i].x1 && player1.velocity + player1.hitBox->w / 2 < testSlopes[i].x2) ///collision for player and test slopes
                    {
                        if(player1.hitBox->y + player1.hitBox->h > (testSlopes[i].y1 - 15) - ((player1.hitBox->x - testSlopes[i].x1) / testSlopes[i].slope) + 1  && (player1.hitBox->y + player1.hitBox->h < (testSlopes[i].y1 - 15) - ((player1.hitBox->x - testSlopes[i].x1) / testSlopes[i].slope) + 85) )  ///this is complicated
                        {                                                                                                                                                                                                                     /// i think 85 means 85 pixesl below it
                            player1.yVelocity = (testSlopes[i].y1 - 15) - ((player1.hitBox->x - testSlopes[i].x1) / testSlopes[i].slope) - player1.hitBox->h + 11;
                            player1.foo = true;
                            if(player1.released) { player1.jumping = false;} //uhhh i dont think this does anything anymore but iduno
                        }
                    }
                }


                for(int i = 0; i < 19; i++)    ///collision for bullets and testboxes i think
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < NumberOfTestBoxes; a++)
                        {
                            if(checkCollision(player1bullets[i].hitBox, testBox[a].hitBox, 1) == 1)
                            {
                                player1bullets[i].active = false;
                            }
                        }
                    }
                }

                if(checkCollision(player1.hitBox, ground.hitBox, 3) == 1)       ///collision for player and the ground, walls, and GUN
                {
                    player1.yVelocity = ground.hitBox->y - player1.hitBox->h +1;
                    player1.foo = true;
                }

                if(checkCollision(player1.hitBox, ground2.hitBox, 3) == 1)
                {
                    player1.yVelocity = ground2.hitBox->y - player1.hitBox->h +1;
                    player1.foo = true;
                }

                if(checkCollision(player1.hitBox, ground3.hitBox, 3) == 1)
                {
                    player1.yVelocity = ground3.hitBox->y - player1.hitBox->h +1;
                    player1.foo = true;
                }

                if(checkCollision(player1.hitBox, leftWall.hitBox, 5) == 1)
                {
                    player1.velocity = leftWall.hitBox->x + leftWall.hitBox->w;
                }

                if(checkCollision(player1.hitBox, semi_auto.hitBox, 1) == 1)
                {
                    player1.secretUnlocked = false;
                    player1.hasGun = true;
                }

                for(int i = 0; i < 19; i++) ///collision for player bullets hitting target
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < 3; a++)
                        {
                            if(checkCollision(player1bullets[i].hitBox, target.hitBox, 1) == 1)
                            {
                                player1bullets[i].direction = 4;
                            }
                        }
                    }
                }

                for(int i = 0; i < 19; i++) ///collision for player bullets hitting the secret target
                {
                    if(player1bullets[i].active)
                    {
                        if(checkCollision(player1bullets[i].hitBox,  Secrettarget.hitBox, 1) == 1)
                        {
                            player1bullets[i].active = false;
                            player1.hasGun = false;
                            player1.secretUnlocked = true;
                            Mix_PlayChannel( -1, aTvSize, 0 );
                        }
                    }
                }



                ///collision for player touching earraep tutorial
                if(checkCollision(player1.hitBox, tutorialSecret.hitBox, 1) == 1)
                {
                    Mix_VolumeMusic(200);
                }
                else
                {
                    Mix_VolumeMusic(20);
                }

                for(int i = 0; i < 19; i++) ///collision for player bullets hitting the lever
                {
                    if(player1bullets[i].active)
                    {
                        if(checkCollision(player1bullets[i].hitBox,  lever.hitBox, 1) == 1)
                        {
                            player1bullets[i].active = false;
                            if(debug_bool_delete_this_later_idiot)
                            {
                                debug_bool_delete_this_later_idiot = false;
                            }
                            else
                            {
                                debug_bool_delete_this_later_idiot = true;
                            }
                        }
                    }
                }

                for(int i = 0; i < 19; i++) ///collision for player bullets hitting player hair
                {
                    if(player1bullets[i].active && !player1bullets[i].following)
                    {
                        //for(int a = 0; a < 3; a++) WHY THE FUCK DID IT ORIGINALLY CHECK FOR COLLISION THREE TIMES?
                        ///--ask tony
                        //{s
                            if(checkCollision(player1bullets[i].hitBox, player1.rectangleForTheHair, 1) == 1)
                            {
                                if(player1.clip < 15 && player1bullets[i].active)
                                {
                                    player1.clip++;
                                    gHair.animationTimer1.start();Mix_PlayChannel( -1, aTvSize, 0 );
                                }
                                player1bullets[i].active = false;
                            }
                        //}
                    }
                }

                for(int i = 0; i < 19; i++) ///collision for enemy bullets hitting player hair
                {
                    if(enemyBullets[i].active)
                    {
                        //for(int a = 0; a < 3; a++) WHY THE FUCK DID IT ORIGINALLY CHECK FOR COLLISION THREE TIMES?
                        ///--ask tony
                        //{s
                            if(checkCollision(enemyBullets[i].hitBox, player1.rectangleForTheHair, 1) == 1)
                            {
                                if(player1.clip < 15 && enemyBullets[i].active)
                                {
                                    player1.clip++;
                                }
                                gHair.animationTimer1.start();Mix_PlayChannel( -1, aTvSize, 0 );
                                enemyBullets[i].active = false;
                                maggot.score++;
                            }
                        //}
                    }
                }

                if(!player1.missileToLeft && !player1.missileJuice) ///ur invincible if you missile left or right because knockback + missile speed makes u go thru walls
                {
                    for(int i = 0; i < 19; i++) ///collision for enemy bullets hitting player
                    {
                        if(enemyBullets[i].active)
                        {
                            //for(int a = 0; a < 3; a++) WHY THE FUCK DID IT ORIGINALLY CHECK FOR COLLISION THREE TIMES?
                            ///--ask tony
                            //{s
                                if(checkCollision(enemyBullets[i].hitBox, player1.hitBox, 1) == 1)
                                {
                                    if(enemyBullets[i].direction == 4)
                                    {
                                        player1.testingLeft = true;
                                        player1.damageColor.start();
                                        player1.health--;
                                    }
                                    else if(enemyBullets[i].direction == 2)
                                    {
                                        player1.testingRight = true;
                                        player1.damageColor.start();
                                        player1.health--;
                                    }
                                    enemyBullets[i].active = false;
                                }
                            //}
                        }
                    }
                }

                for(int i = 0; i < 19; i++) ///collision for player bullets hitting zipline
                {
                    if(player1bullets[i].active)
                    {
                        if(checkCollision(player1bullets[i].hitBox, zipline1.hitBox, 1) == 1)
                        {
                            zipline1.powerTimer.start();
                            player1bullets[i].active = false;
                        }
                    }
                }

                ///COLLISION FOR ROBOT AND MELEE HITBOX                                               ///okay so later i need to move all of these checks into the player class, but if i use gunDirection = 1 instead of !holdingup then i can do multiple hits with one melee for some reason, I THINK because gundirection is changed AFTER collision detection
                 for(int i = 0; i < numberOfRobots; i++)
                 {
                    if(!player1.holdingUp && checkCollision(player1.meleeHitbox, robots[i].hitBox, 1) == 1 && gPlayerSemi.animationTimer2.getTicks() < 249 && gPlayerSemi.animationTimer2.getTicks() > 160) ///collicion for
                    {                                                                                                                                                                                         //wtf does this mean?
                        if(!robots[i].respawnTimer.isStarted() && !player1.meleeHit) ///switch this around or somethign this is retarded
                        {
                            if(!robots[i].hitColorBody.isStarted())
                            {
                                robots[i].hitColorBody.start();
                            }

                            robots[i].health--;
                            player1.endMeleeHit = true;
                        }
                    }
                    else if(player1.gunBouncing && !player1.meleeHit && player1.gunBounceTimer.isStarted())///gunbounce
                    {
                        if(!robots[i].respawnTimer.isStarted() && checkCollision(player1.meleeHitbox, robots[i].hitBox, 1) == 1)
                        {
                            if(!robots[i].hitColorBody.isStarted())
                            {
                                robots[i].hitColorBody.start();
                            }

                            robots[i].health--;
                            player1.endMeleeHit = true;
                            player1.bounceHit = true;
                        }
                    }

                 }

                ///collision for TEST BOXES and GUN BOUNCE
                if(player1.gunBouncing && !player1.meleeHit && player1.gunBounceTimer.isStarted())
                    {
                        for(int i = 0; i < NumberOfTestBoxes; i++)
                        {
                            if(checkCollision(player1.meleeHitbox, testBox[i].hitBox, 1) == 1)
                            {
                                printf("ffffffffffff");
                                player1.endMeleeHit = true;
                                player1.bounceHit = true;
                            }
                        }
                    }

                ///COLLISION FOR ROBOT body AND player bullets
                for(int p = 0; p < numberOfRobots; p++)
                {
                    if(robots[p].alive)
                    {
                        for(int i = 0; i < 19; i++)
                        {
                            if(player1bullets[i].active)
                            {
                                if(checkCollision(player1bullets[i].hitBox, robots[p].hitBox, 1) == 1)
                                {
                                    player1bullets[i].testing = true;
                                    if(!player1bullets[i].testTimer.isStarted())
                                    {
                                        if(player1bullets[i].direction == 2)
                                        {
                                            player1bullets[i].direction = 4;
                                        }
                                        else if(player1bullets[i].direction == 4)
                                        {
                                            player1bullets[i].direction = 2;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                ///COLLISION FOR ROBOT LEFT CLAW AND player bullets
                for(int p = 0; p < numberOfRobots; p++)
                {
                    if(robots[p].leftActive)
                    {
                        for(int i = 0; i < 19; i++)
                        {
                            if(player1bullets[i].active)
                            {
                                if(checkCollision(player1bullets[i].hitBox, robots[p].hitBoxLeft, 1) == 1)
                                {
                                    if(!robots[p].hitColorLeft.isStarted())
                                    {
                                        robots[p].hitColorLeft.start();
                                    }
                                    player1bullets[i].active = false;
                                    robots[p].leftHealth--;
                                }
                            }
                        }
                    }
                }

                ///COLLISION FOR PLAYER AND LEFT ROBOT CLAW
                for(int i = 0; i < numberOfRobots; i++)
                {
                    if(robots[i].leftActive)
                    {
                        if(checkCollision(player1.hitBox, robots[i].hitBoxLeft, 1) == 1)
                        {
                            if(!player1.damageColor.isStarted())
                            {
                               player1.testingLeft = true;
                                player1.damageColor.start();
                                player1.health--;
                            }
                        }
                    }
                }
                ///COLLISION FOR ROBOT RIGHT CLAW AND player bullets
                for(int p = 0; p < numberOfRobots; p++)
                {
                    if(robots[p].rightActive)
                    {
                        for(int i = 0; i < 19; i++)
                        {
                            if(player1bullets[i].active)
                            {
                                if(checkCollision(player1bullets[i].hitBox, robots[p].hitBoxRight, 1) == 1)
                                {
                                    if(!robots[p].hitColorRight.isStarted())
                                    {
                                        robots[p].hitColorRight.start();
                                    }
                                    player1bullets[i].active = false;
                                    robots[p].rightHealth--;
                                }
                            }
                        }
                    }
                }



                ///COLLISION FOR PLAYER AND RIGHT ROBOT CLAW
                for(int i = 0; i < numberOfRobots; i++)
                {
                    if(robots[i].rightActive)
                    {
                        if(checkCollision(player1.hitBox, robots[i].hitBoxRight, 1) == 1)
                        {
                            if(!player1.damageColor.isStarted())
                            {
                                player1.testingRight = true;
                                player1.damageColor.start();
                                player1.health--;
                            }
                        }
                    }
                }


                //std::cout << "dddd: " << player1.meleeTimer.getTicks() << std::endl;
                //std::cout << "dxxx: " << gPlayerSemi.animationTimer2.getTicks() << std::endl;
                //std::cout << "yyyy: " << zipline1.yPos << std::endl;
                //std::cout << "yyy: " << enemy1.hitBoxLeft->y << std::endl;
                //std::cout << "yyy: " << gEnemy.animationTimer1.getTicks() / 100 << std::endl;

             //std::cout << player1.paused << std::endl;
                //std::cout << player1bullets[1].testTimer.getTicks() << std::endl;

                 //std::cout << "xxx: " << player1.hitBox->x << std::endl;
               //std::cout << "yyyy: " << player1.gunDirection << std::endl;


                /*std::cout << "yyyy: " << zipline1.xPos << std::endl;
                std::cout << "yyyy: " << zipline1.yPos << std::endl;
                std::cout << "yyyy: " << zipline1.docked << std::endl;*/

                //std::cout << "xxxx: " << sharkRobots[0].hitBox->x << std::endl;
                //std::cout << "xxxx: " << sharkRobots[0].hitBox->y << std::endl;
                //std::cout << "yyyy: " << kongaLees[0].bullets[0]->y << std::endl;
                //std::cout << "yyyy: " << player1.hitBox->y << std::endl;

                //std::cout << "foo: " << player1.foo << std::endl;
                //std::cout << "bar: " << player1.bar << std::endl;
                //std::cout << "baz: " << player1.baz << std::endl;
                //std::cout << "ect: " << player1.ect << std::endl;

                //std::cout << "x: " << player1.useItemButtonOrMachine << std::endl;
                /*std::cout << "x1: " << testSlopes[0].x1 << std::endl;
                std::cout << "y1: " << testSlopes[0].y1 << std::endl;
                std::cout << "x2: " << testSlopes[0].x2 << std::endl;
                std::cout << "y2: " << testSlopes[0].y2 << std::endl;*/
                //mouseHitbox->x = mouse.x;
                //mouseHitbox->y = mouse.y;




               // *SDL_Point mouse;

                /*SDL_GetMouseState(&mouse.x, &mouse.y);

                mouse.x += camera->x;
                mouse.y += camera->y;
                std::cout << "x: " << mouse.x  << std::endl;
                std::cout << "y: " << mouse.y  << std::endl;
                mouse.x -= camera->x;
                mouse.y -= camera->y; ///uncomment this shit, its epic*/





                    if(zipline1.xVelocity >= 2284)                                                      ///collision for the zipline and player
                    {
                        zipline1.xVelocity = 2284;
                        zipline1.yVelocity = 656;
                        //zipline1.xPos = 2284;
                        //zipline1.yPos = 656;
                        zipline1.docked = true;
                    }
                    else
                    {
                        zipline1.docked = false;
                    }

                    if(zipline1.xVelocity <= 1372)
                    {
                        zipline1.xVelocity = 1372;
                        zipline1.yVelocity = 200;
                        //zipline1.xPos = 1372;
                        //zipline1.yPos = 200;
                        zipline1.powerTimer.stop();
                        zipline1.docked = true;
                    }

                    if( player1.hasGun && player1.gunDirection == 1 && checkCollision(player1.muzzleHitbox, zipline1.hitBox, 1) == 1)
                    {
                        if(zipline1.carrying)
                        {
                            player1.onZipline = true;
                            if(!player1.flip)
                            {
                                 player1.velocity = zipline1.xVelocity + 20;
                                player1.yVelocity = zipline1.yVelocity + 50;
                            }
                            else
                            {
                                player1.velocity = zipline1.xVelocity - 20;
                                player1.yVelocity = zipline1.yVelocity + 50;
                            }
                        }
                        else
                        {
                            zipline1.docked = false;
                            zipline1.carrying = true;
                        }
                    }
                    else
                    {
                        zipline1.carrying = false;
                        player1.onZipline = false;
                    }

                for(int i = 0; i < numberOfRobots; i++) ///collision with robots and testboxes DICKLE
                {
                    for(int a = 0; a < NumberOfTestBoxes; a++)
                    {
                        //robots[i].onFlatSurface = false;
                        switch ( checkCollision(robots[i].hitBox, testBox[a].hitBox, 2) )
                        {
                            case 1: robots[i].hitBox->y = testBox[a].hitBox->y - robots[i].hitBox->h -0;
                                    robots[i].onFlatSurface = true;
                                break;
                            case 2: //hitting left wall
                                    robots[i].home -= 30;
                                break;
                            case 3: //hitting right wall
                                    robots[i].home += 30;
                                break;
                            case 4: //robots should never hit a ceiling
                                break;
                            case 5: //do nothing

                                break;
                        }


                        if(robots[i].leftFalling)
                        {
                            if(checkCollision(robots[i].hitBoxLeft, testBox[a].hitBox, 1))
                            {
                                robots[i].leftFalling = false;
                            }
                        }
                        if(robots[i].rightFalling)
                        {
                            if(checkCollision(robots[i].hitBoxRight, testBox[a].hitBox, 1))
                            {
                                   robots[i].rightFalling = false;
                            }
                        }
                    }
                }
                                                                                // why is this out of place
                /*if(checkCollision(robots[0].hitBox, ground2.hitBox, 3) == 1)///collision for ROBOT with the groundses and the walls
                {
                    robots[0].hitBox->y = ground2.hitBox->y - robots[0].hitBox->h +0;
                    robots[0].onFlatSurface = true;
                }

                if(checkCollision(robots[0].hitBoxLeft, ground2.hitBox, 1) == 1)
                {
                    robots[0].leftFalling = false;
                }

                if(checkCollision(robots[0].hitBoxRight, ground2.hitBox, 1) == 1)
                {
                    robots[0].rightFalling = false;
                }*/

                for(int i = 0; i < numberOfRobots; i++) ///robots and testslopes
                {
                    for(int a = 0; a < NumberOfTestSlopes; a++)
                    {
                        //if(robots[i].alive)
                        //{
                            if(robots[i].hitBox->x + robots[i].hitBox->w / 2 > testSlopes[a].x1 && robots[i].hitBox->x + robots[i].hitBox->w/ 2 < testSlopes[a].x2)
                            {
                                if(robots[i].hitBox->y + robots[i].hitBox->h > (testSlopes[a].y1 - (robots[i].hitBox->h / 2)) - ((robots[i].hitBox->x - testSlopes[a].x1) / testSlopes[a].slope) + 1  && (robots[i].hitBox->y + robots[i].hitBox->h < (testSlopes[a].y1 - (robots[i].hitBox->w / 2)) - ((robots[i].hitBox->x - testSlopes[a].x1) / testSlopes[a].slope) + 85) )  ///this is complicated
                                {
                                    robots[i].hitBox->y = (testSlopes[a].y1 - (robots[i].hitBox->h / 2)) - ((robots[i].hitBox->x - testSlopes[a].x1) / testSlopes[a].slope) - robots[i].hitBox->h + 29;
                                    robots[i].onFlatSurface = true;
                                }
                            }
                        //}
                    }

                    ///make arms collide with slopes here lol
                }



                ///snowman collision
                                                                                                                       ///snowman collision

                if(snowman1.time2explode)
                {
                    if(snowman1.hitBoxNose->x > player1.hitBox->x + 30)
                    {
                        player1.testingLeft = true;
                        gPlayerSemi.setColor(255, 0, 0);
                        gPlayerArmed.setColor(255, 0, 0);
                    }
                    else
                    {
                        player1.testingRight = true;
                        gPlayerSemi.setColor(255, 0, 0);
                        gPlayerArmed.setColor(255, 0, 0);
                    }
                }


                if(checkCollision(snowman1.hitBoxNose, ground2.hitBox, 1) == 1 && snowman1.noseActive)///collision for snowman nose and the floor
                {
                    snowman1.goingDown = false;
                    gSnowmanNose.animationBool = false;
                    snowman1.noseActive = false;
                    snowman1.time2explode = true; ///explosion!!!
                    snowman1.rotation = 0;

                    snowman1.explosionTimer.start();
                }

                if(checkCollision(snowman1.hitBoxNose, player1.rectangleForTheHair, 1) == 1 && snowman1.noseActive)///collision for snowman nose and player hair
                {
                    snowman1.hitBoxNose->y = -9000;
                    snowman1.goingDown = false;
                    gSnowmanNose.animationBool = false;
                    snowman1.noseActive = false;
                    //snowman1.time2explode = true; ///explosion!!!
                    snowman1.rotation = 0;

                    snowman1.explosionTimer.start();

                    if(player1.clip < 15)
                    {
                        player1.clip++;
                    }
                    gHair.animationTimer1.start();Mix_PlayChannel( -1, aTvSize, 0 );
                }

                if(snowman1.hitBoxNose->y > ground2.hitBox->y +50 && snowman1.noseActive) ///too lazy to just make it explode after a certain amount of time after being alive so if it gets too low it explodes lol
                {
                    snowman1.goingDown = false;
                    gSnowmanNose.animationBool = false;
                    snowman1.noseActive = false;
                    snowman1.time2explode = true; ///explosion!!!
                    snowman1.rotation = 0;

                    snowman1.explosionTimer.start();
                }


                if(snowman1.alive)
                {
                    if(checkCollision(snowman1.hitBox, ground2.hitBox, 3) == 1)///collision for snowman with the groundses and the walls
                    {
                        snowman1.hitBox->y = ground2.hitBox->y - snowman1.hitBox->h +0;
                        snowman1.onFlatSurface = true;
                    }

                    for(int i = 0; i < 19; i++) ///collision for snowman body and player bullets
                    {
                        if(player1bullets[i].active)
                        {
                            if(checkCollision(player1bullets[i].hitBox, snowman1.hitBox, 1) == 1)
                            {
                                player1bullets[i].active = false;
                            }
                        }
                    }

                    if(checkCollision(camera, snowman1.hitBox, 1)) ///if snowman is in view of the camera
                    {
                       snowman1.openFire = true;
                    }
                    else
                    {
                        snowman1.openFire = false;
                    }

                    for(int i = 0; i < 19; i++) ///collision for snowman head and player bullets
                    {
                        if(player1bullets[i].active)
                        {
                            if(checkCollision(player1bullets[i].hitBox, snowman1.hitBoxHead, 1) == 1)
                            {
                                snowman1.alive = false;
                            }
                        }
                    }

                }                                                                                                                          ///end of snowman collision


                ///OXYGEN TANK COLLISION
                                                                                                                                                                                ///OXYGEN TANK COLLISION
                if(player1.hasGun && checkCollision(player1.hitBox, oxygentank.hitBox, 1) == 1)
                {
                    player1.nearOxygen = true;
                    player1.walkToX = oxygentank.xPos - 45;
                    player1.whatToDoWhenAtLocation = 1;
                }
                else
                {
                    player1.nearOxygen = false;
                }

                ///LEDGE GRAB COLLISION                                                                                 ///LEDGE GRAB COLLISION
                if(player1.hasGun && player1.gunDirection == 1)
                {
                    for(int i = 0; i < 2; i++)
                    {
                        if(checkCollision(player1.muzzleHitbox, ledges[i].hitBox, 1) == 1)
                        {
                            if(ledges[i].facingRight && player1.flip)
                            {
                                player1.velocity = ledges[i].hitBox->x - 27;
                                player1.yVelocity = ledges[i].hitBox->y + 42;
                                if(!player1.ledgeHanging){Mix_PlayChannel( -1, aledgeGrab, 0 ); };
                                player1.ledgeHanging = true;
                            }
                            else if(!ledges[i].facingRight && !player1.flip)
                            {
                                player1.velocity = ledges[i].hitBox->x + 12;
                                player1.yVelocity = ledges[i].hitBox->y + 42;
                                if(!player1.ledgeHanging){Mix_PlayChannel( -1, aledgeGrab, 0 ); };
                                player1.ledgeHanging = true;
                            }
                        }
                    }
                }

                ///GRANADO COLLISION with ground                                                                               ///GRANADO COLLISION



                gGranado.setColor(player1.granadoColor, 0, 0);


                if(checkCollision(player1.granadoHitbox, ground.hitBox, 3) == 1)
                {
                   player1.granadoVelocityY = ground.hitBox->y - player1.granadoHitbox->h -4;
                   player1.granadoOnFlatSurface = true;
                }

                if(checkCollision(player1.granadoHitbox, ground2.hitBox, 3) == 1)
                {
                    player1.granadoVelocityY = ground2.hitBox->y - player1.granadoHitbox->h -4;
                    player1.granadoOnFlatSurface = true;
                }

                if(checkCollision(player1.granadoHitbox, ground3.hitBox, 3) == 1)
                {
                    player1.granadoVelocityY = ground3.hitBox->y - player1.granadoHitbox->h -4;
                    player1.granadoOnFlatSurface = true;
                }


				for(int i = 0; i < NumberOfTestBoxes; i++) ///granado and testboxes?
                {
                    /*if(checkCollision(player1.granadoHitbox, testBox[i].hitBox, 3) == 1)
                    {
                        player1.granadoVelocityY = testBox[i].hitBox->y - player1.granadoHitbox->h -4;
                        player1.granadoOnFlatSurface = true;
                    }*/
                    switch ( checkCollision(player1.granadoHitbox, testBox[i].hitBox, 2) ) ///collision for granado and test boxes
                        {
                            case 1: player1.granadoVelocityY = testBox[i].hitBox->y - player1.granadoHitbox->h -4;
                                    player1.granadoOnFlatSurface = true;
                                break;                                                        //+1?
                            case 2:
                                    if(player1.granadoTimer.getTicks() < 3000 && player1.granadoRight)
                                    {
                                        player1.granadoContact = true;
                                    }
                                break;
                            case 3: if(player1.granadoTimer.getTicks() < 3000 && player1.granadoLeft)
                                    {
                                        player1.granadoContact = true;
                                    }
                                break;
                            case 4:
                                break;
                            case 5: //do nothing

                                break;
                        }
                }

                for(int i = 0; i < NumberOfTestSlopes; i++) ///granado and testslopes
                {
                    if(!player1.granadoExploding)
                    {
                        if(player1.granadoVelocityX + player1.granadoHitbox->w / 2 > testSlopes[i].x1 && player1.granadoVelocityX + player1.granadoHitbox->w / 2 < testSlopes[i].x2)
                        {
                            if(player1.granadoHitbox->y + player1.granadoHitbox->h > (testSlopes[i].y1 - (player1.granadoHitbox->h / 2)) - ((player1.granadoVelocityX - testSlopes[i].x1) / testSlopes[i].slope) + 1  && (player1.granadoHitbox->y + player1.granadoHitbox->h < (testSlopes[i].y1 - (player1.granadoHitbox->w / 2)) - ((player1.granadoVelocityX - testSlopes[i].x1) / testSlopes[i].slope) + 85) )  ///this is complicated
                            {                                                                                                                                                                                                                     /// i think 85 means 85 pixesl below it
                                player1.granadoVelocityY = (testSlopes[i].y1 - (player1.granadoHitbox->h / 2)) - ((player1.granadoVelocityX - testSlopes[i].x1) / testSlopes[i].slope) - player1.granadoHitbox->h + 0;
                                player1.granadoOnFlatSurface = true;
                                if(player1.granadoLeft)
                                {
                                    player1.granadoSpeed = player1.granadoSpeed + 3;
                                }
                                if(player1.granadoRight)
                                {
                                    player1.granadoSpeed = player1.granadoSpeed - 3;
                                }
                            }
                        }
                    }
                }

                ///GRANADO COLLISION WITH PLAYER
                if(player1.granadoOnFlatSurface)  ///i should probably just make it add +100 to the speed and cap the speed at 100, and only check for if player is goinleft or goinright to decide where grenade is kicked
                {
                    if(player1.granadoRight)
                    {
                        if(checkCollision(player1.granadoHitbox, player1.hitBox, 1) == 1)
                        {
                            if(player1.hitBox->x < player1.granadoHitbox->x)
                            {
                                player1.granadoSpeed = player1.granadoSpeed + 55;
                            }
                            else
                            {
                                player1.granadoSpeed = player1.granadoSpeed + 55;
                                player1.granadoLeft = true;
                                player1.granadoRight = false;
                            }
                        }
                    }
                    else
                    {
                        if(checkCollision(player1.granadoHitbox, player1.hitBox, 1) == 1)
                        {
                            if(player1.hitBox->x < player1.granadoHitbox->x)
                            {
                                player1.granadoSpeed = player1.granadoSpeed + 55;
                                player1.granadoLeft = false;
                                player1.granadoRight = true;
                            }
                            else
                            {
                                player1.granadoSpeed = player1.granadoSpeed + 55;
                            }
                        }
                    }
                }

                /// collision for granado and robot
                for(int i = 0; i < numberOfRobots; i++)
                {
                    if(robots[i].health > 0)
                    {
                        if(checkCollision(player1.granadoHitbox, robots[i].hitBox, 1) == 1)
                        {
                            robots[i].health = 0;

                            player1.granadoContact = true;
                        }
                    }
                }

                /// grenade jumping shit
                if(player1.granadoExploding)
                {

                        if(checkCollision(player1.granadoHitbox, player1.hitBox, 1) == 1)
                        {
                            if(!player1.onFlatSurface)
                            {
                                player1.blastJumping = true;
                            }

                            if(player1.hitBox->x > player1.granadoHitbox->x + player1.granadoHitbox->w - 30)
                            {
                                player1.blastJumpingRight = true;
                                player1.blastJumpSpeed = 80;
                            }
                            else if(player1.hitBox->x + player1.hitBox->w < player1.granadoHitbox->x + 30)
                            {
                                player1.blastJumpingLeft = true;
                                player1.blastJumpSpeed = 80;
                            }
                        }

                }

                ///COLLISION FOR ARMORED TRUCK                              ARMORED TRUCK


                /*for(int i = 0; i < numberOfTrucks; i++) ///collision for armored truck and ground
                {
                    trucks[i].onFlatSurface = false;
                    if(checkCollision(trucks[i].hitBoxBottom, testBox[5].hitBox, 3) == 1)
                    {
                        trucks[i].onFlatSurface = true;
                        trucks[i].yVelocity = testBox[5].hitBox->y - trucks[i].hitBoxBottom->h;
                    }///never becomes false
                }*/

                for(int i = 0; i < numberOfTrucks; i++) ///collision for armored truck and testboxes
                {
                    trucks[i].onFlatSurface = false;
                    for(int a = 0; a < NumberOfTestBoxes; a++)
                    {
                        if(checkCollision(trucks[i].hitBoxBottom, testBox[a].hitBox, 3) == 1)
                        {
                            trucks[i].onFlatSurface = true;
                            trucks[i].yVelocity = testBox[a].hitBox->y - trucks[i].hitBoxBottom->h;
                            trucks[i].hitBoxTrigger2->y = trucks[i].yVelocity - 50;
                        }
                    }
                }

                for(int i = 0; i < numberOfTrucks; i++)
                {
                    switch ( checkCollision(player1.hitBox, trucks[i].hitBoxBottom, 2) ) ///collision for player and armored truck bottom
                        {
                            case 1: player1.yVelocity = trucks[i].hitBoxBottom->y - player1.hitBox->h + 1;
                                    player1.foo = true;
                                    if(player1.jumpTimer.getTicks() > 40) {player1.jumping = false; player1.jumpTimer.stop(); }
                                break;                                                        //+1?
                            case 2: player1.velocity = trucks[i].hitBoxBottom->x - player1.hitBox->w +1;
                                    player1.bar = true;
                                break;
                            case 3: player1.velocity = trucks[i].hitBoxBottom->x + trucks[i].hitBoxBottom->w -1;
                                    player1.baz = true;
                                break;
                            case 4: player1.yVelocity = trucks[i].hitBoxBottom->y + trucks[i].hitBoxBottom->h;
                                    player1.released = true;
                                break;
                            case 5: //do nothing

                                break;
                       // }
                    }
                }

                for(int i = 0; i < numberOfTrucks; i++)
                {
                    switch ( checkCollision(player1.hitBox, trucks[i].hitBoxTop, 2) ) ///collision for player and armored truck top
                        {
                            case 1: player1.yVelocity = trucks[i].hitBoxTop->y - player1.hitBox->h + 1;
                                    player1.foo = true;
                                    if(player1.jumpTimer.getTicks() > 40) {player1.jumping = false; player1.jumpTimer.stop(); }
                                break;                                                        //+1?
                            case 2: player1.velocity = trucks[i].hitBoxTop->x - player1.hitBox->w +1;
                                    player1.bar = true;
                                break;
                            case 3: player1.velocity = trucks[i].hitBoxTop->x + trucks[i].hitBoxTop->w -1;
                                    player1.baz = true;
                                break;
                            case 4: player1.yVelocity = trucks[i].hitBoxTop->y + trucks[i].hitBoxTop->h;
                                    player1.released = true;
                                break;
                            case 5: //do nothing

                                break;
                       // }
                    }
                }

                ///collision for armored truck and player bullets
                for(int p = 0; p < numberOfTrucks; p++)
                {
                    for(int i = 0; i < 19; i++)
                    {
                        if(player1bullets[i].active)
                        {
                            if(checkCollision(player1bullets[i].hitBox, trucks[p].hitBoxBottom, 1) == 1) ///for bottom of truck
                            {
                                if(trucks[p].health > 0)
                                {
                                    player1bullets[i].testing = true;
                                    if(!player1bullets[i].testTimer.isStarted())
                                    {
                                        if(player1bullets[i].direction == 2)
                                        {
                                            player1bullets[i].direction = 4;
                                        }
                                        else if(player1bullets[i].direction == 4)
                                        {
                                            player1bullets[i].direction = 2;
                                        }
                                    }
                                }
                                else
                                {
                                    player1bullets[i].active = false;
                                }
                            }
                        }
                    }
                }

                for(int p = 0; p < numberOfTrucks; p++)
                {
                    for(int i = 0; i < 19; i++)
                    {
                        if(player1bullets[i].active)
                        {
                            if(checkCollision(player1bullets[i].hitBox, trucks[p].hitBoxTop, 1) == 1) ///for top of truck
                            {
                                if(trucks[p].health > 0)
                                {
                                    player1bullets[i].testing = true;
                                    if(!player1bullets[i].testTimer.isStarted())
                                    {
                                        if(player1bullets[i].direction == 2)
                                        {
                                            player1bullets[i].direction = 4;
                                        }
                                        else if(player1bullets[i].direction == 4)
                                        {
                                            player1bullets[i].direction = 2;
                                        }
                                    }
                                }
                                else
                                {
                                    player1bullets[i].active = false;
                                }
                            }
                        }
                    }
                }

                for(int p = 0; p < numberOfTrucks; p++)
                {
                    for(int i = 0; i < 19; i++)
                    {
                        if(player1bullets[i].active)
                        {
                            if(checkCollision(player1bullets[i].hitBox, trucks[p].hitBoxGlass, 1) == 1) ///for truck glass
                            {
                                if(trucks[p].health > 0)
                                {
                                    switch(trucks[p].health)
                                    {
                                            case 1: Mix_PlayChannel( -1, aCrack3, 0 );
                                                    break;                                                        //+1?
                                            case 2: Mix_PlayChannel( -1, aCrack2, 0 );
                                                    break;
                                            case 3: Mix_PlayChannel( -1, aCrack1, 0 );
                                                    break;
                                    }
                                    trucks[p].health -= 1;
                                }
                                player1bullets[i].active = false;
                            }
                        }
                    }
                }

                ///collision for armored truck TRIGGER and player
                for(int i = 0; i < numberOfTrucks; i++)
                {
                    if(trucks[i].health > 0)
                    {
                        if(checkCollision(player1.hitBox, trucks[i].hitBoxTrigger, 1) == 1)
                        {
                            if(!trucks[i].triggeredTimer.isStarted())
                            {
                                trucks[i].triggeredTimer.start();
                            }
                            else if(trucks[i].triggeredTimer.getTicks() > 150 && trucks[i].triggeredTimer.getTicks() < 250)
                            {
                                player1.blastJumping = true;
                            }
                        }
                    }
                }

                ///collision for armored truck TRIGGER 2 and player
                for(int i = 0; i < numberOfTrucks; i++)
                {
                    if(trucks[i].health > 0)
                    {
                        if(player1.hitBox->x < trucks[i].hitBoxBottom->x - 40)
                        {
                            if(checkCollision(player1.hitBox, trucks[i].hitBoxTrigger2, 1) == 1)
                            {
                                if(!trucks[i].charging)
                                {
                                    trucks[i].charging = true;
                                }
                            }
                        }
                    }
                }

                ///collision for BULLET CAGE TRIGGER
                if(maggot.firstTry)
                {
                    if(checkCollision(player1.hitBox, bulletCageTrigger.hitBox, 1) == 1)
                    {
                        bulletCageTrigger.activated = true;
                        maggot.firstTry = false;

                        maggot.volleyTimer.start();
                        maggot.sequence = 1;
                        maggot.score = 0;
                        maggot.hitBox->x = 8254;
                        maggot.hitBox->y = -27;
                    }
                } ///UNCOMMENT THIS
                else
                {
                    bulletCageTrigger.activated = true;
                    if(maggot.sequence == 0 || maggot.sequence == 44)
                    {
                        if(!maggot.firstTry && !targetGame.funTimer.isStarted())
                        {
                             bulletCageTrigger.activated = false;
                        }
                    }
                }





                ///collision for LEVEL 2 TRIGGER
                if(maggot.sequence == 44 || maggot.sequence == 0)
                {
                    if(!targetGame.funTimer.isStarted() && !maggot.firstTry)
                    {
                       for(int i = 0; i < 19; i++)
                        {
                            if(player1bullets[i].active)
                            {
                                if(checkCollision(player1bullets[i].hitBox, level2Trigger.hitBox, 1) == 1)
                                {
                                    targetGame.funTimer.start();
                                    targetGame.twoSecCounter = 0;
                                    targetGame.score = 0;
                                    bulletCageTrigger.activated = true;
                                    player1.clip = 15;
                                    maggot.sequence = 0;
                                }
                            }
                        }
                    }
                }


                if(maggot.score == 40 && targetGame.score == 29)
                {
                    player1.doubleDuckingUnlocked = true;
                }

                ///collision for LEVEL 1 TRIGGER
                if(!targetGame.funTimer.isStarted() && !maggot.firstTry)
                {
                    if(maggot.sequence == 44 || maggot.sequence == 0)
                    {
                        for(int i = 0; i < 19; i++)
                        {
                            if(player1bullets[i].active)
                            {
                                if(checkCollision(player1bullets[i].hitBox, level1Trigger.hitBox, 1) == 1)
                                {
                                    maggot.volleyTimer.start();
                                    maggot.sequence = 1;
                                    maggot.score = 0;
                                    maggot.hitBox->x = 8254;
                                    maggot.hitBox->y = -27;
                                    /*if(!maggot.firstTry)
                                    {
                                       bulletCageTrigger.activated = true;
                                    }*/
                                }
                            }
                        }
                    }
                }

                ///collision for level 2 targets and player bullets
                for(int i = 0; i < 19; i++)
                {
                    if(player1bullets[i].active)
                    {
                        for(int p = 0; p < 15; p++)
                        {
                            if(targetGame.targets[p].alive)
                            {
                                if(checkCollision(player1bullets[i].hitBox, targetGame.targets[p].hitBox, 1) == 1)
                                {
                                    targetGame.targets[p].alive = false;
                                    targetGame.score += targetGame.targets[p].color;
                                    targetGame.targets[p].alpha = 255;

                                    /*if(player1bullets[i].direction == 4)
                                    {
                                        player1bullets[i].direction = 2;
                                    }
                                    else if(player1bullets[i].direction == 2)
                                    {
                                        player1bullets[i].direction = 4;
                                    }
                                    else if(player1bullets[i].direction == 1)
                                    {
                                        player1bullets[i].direction = 3;
                                    }*/
                                    if(targetGame.targets[p].color == 3)
                                    {
                                        Mix_PlayChannel( -1, aBreak3, 0 );
                                    }
                                    else if(targetGame.targets[p].color == 2)
                                    {
                                        Mix_PlayChannel( -1, aBreak2, 0 );
                                    }
                                    else
                                    {
                                        Mix_PlayChannel( -1, aBreak1, 0 );
                                    }

                                    player1bullets[i].active = false;

                                    //targetGame.targets[p].funTimerlol.start();
                                }
                            }
                        }
                    }
                }

                ///collision for ROBO MAGGOT and player bullets
                 for(int i = 0; i < 19; i++)
                {
                    if(player1bullets[i].active)
                    {
                        if(checkCollision(player1bullets[i].hitBox, maggot.hitBox, 1) == 1)
                        {
                            player1bullets[i].testing2 = true;
                        }
                    }
                }

                for(int i = 0; i < 19; i++)    ///collision for enemy bullets and testboxes i think
                {
                    if(enemyBullets[i].active)
                    {
                        for(int a = 0; a < NumberOfTestBoxes; a++)
                        {
                            if(checkCollision(enemyBullets[i].hitBox, testBox[a].hitBox, 1) == 1)
                            {
                                enemyBullets[i].active = false;
                            }
                        }
                    }
                }

                for(int i = 0; i < numberOfSharkRobots; i++) ///collision for shark robots and test boxes
                {
                    for(int a = 0; a < NumberOfTestBoxes; a++)
                    {
                        switch ( checkCollision(sharkRobots[i].hitBox, testBox[a].hitBox, 2) )
                        {
                            case 1: //floor
                                    if(sharkRobots[i].health > 0)
                                    {
                                        sharkRobots[i].jumpTimer.start();
                                    }
                                    else
                                    {
                                        sharkRobots[i].dead = true;
                                    }

                                break;                                                        //+1?
                            case 2: //hitting left wall
                                break;
                            case 3: //hitting right wall
                                    sharkRobots[i].health = 0;
                                break;
                            case 4:
                                    //ceiling
                                break;
                            case 5: //do nothing
                                break;
                        }
                    }
                }

                for(int i = 0; i < numberOfSharkRobots; i++) ///collision for shark robots and player
                {
                    if(checkCollision(sharkRobots[i].hitBox, player1.hitBox, 1) == 1)
                    {
                        player1.testingLeft = true;
                        player1.damageColor.start();
                        player1.health--;
                    }
                }

                for(int i = 0; i < 19; i++)    ///collision for player bullets and shark robots
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < numberOfSharkRobots; a++)
                        {
                            if(!sharkRobots[a].dead)
                            {
                                if(checkCollision(sharkRobots[a].hitBox, player1bullets[i].hitBox, 1) == 1)
                                {
                                    player1bullets[i].active = false;
                                    sharkRobots[a].jumpTimer.start();
                                    sharkRobots[a].beingShotInTheFaceTimer.start();
                                    sharkRobots[a].health--;
                                }
                            }
                        }
                    }
                }

                for(int i = 0; i < 19; i++)    ///collision for player bullets and kultentas
                {
                    if(player1bullets[i].active && !player1bullets[i].following)
                    {
                        for(int a = 0; a < numberOfKultentas; a++)
                        {
                            if(!kultentas[a].dead)
                            {
                                if(checkCollision(kultentas[a].hitBox, player1bullets[i].hitBox, 1) == 1)
                                {
                                    player1bullets[i].deletionTimer.start();
                                    kultentas[a].rootyTootyTimetoShootyTimer.start(); /// test

                                    if(!kultentas[a].storage1Full)
                                    {
                                        kultentas[a].replaceTimer1.start();
                                        kultentas[a].storage1Full = true;
                                        player1bullets[i].jellySpeed = 300;
                                        player1bullets[i].following = true;
                                        player1bullets[i].followingX = kultentas[a].bulletStorage1.x;
                                        player1bullets[i].followingY = kultentas[a].bulletStorage1.y;
                                    }
                                    else if(!kultentas[a].storage2Full)
                                    {
                                        kultentas[a].replaceTimer2.start();
                                        kultentas[a].storage2Full = true;
                                        player1bullets[i].jellySpeed = 300;
                                        player1bullets[i].following = true;
                                        player1bullets[i].followingX = kultentas[a].bulletStorage2.x;
                                        player1bullets[i].followingY = kultentas[a].bulletStorage2.y;
                                    }
                                    else if(!kultentas[a].storage3Full)
                                    {
                                        kultentas[a].replaceTimer3.start();
                                        kultentas[a].storage3Full = true;
                                        player1bullets[i].jellySpeed = 300;
                                        player1bullets[i].following = true;
                                        player1bullets[i].followingX = kultentas[a].bulletStorage3.x;
                                        player1bullets[i].followingY = kultentas[a].bulletStorage3.y;
                                    }
                                    else if(!kultentas[a].storage4Full)
                                    {
                                        kultentas[a].replaceTimer4.start();
                                        kultentas[a].storage4Full = true;
                                        player1bullets[i].jellySpeed = 300;
                                        player1bullets[i].following = true;
                                        player1bullets[i].followingX = kultentas[a].bulletStorage4.x;
                                        player1bullets[i].followingY = kultentas[a].bulletStorage4.y;
                                    }
                                    else if(!kultentas[a].storage5Full)
                                    {
                                        kultentas[a].replaceTimer5.start();
                                        kultentas[a].storage5Full = true;
                                        player1bullets[i].jellySpeed = 300;
                                        player1bullets[i].following = true;
                                        player1bullets[i].followingX = kultentas[a].bulletStorage5.x;
                                        player1bullets[i].followingY = kultentas[a].bulletStorage5.y;
                                    }
                                    else if(kultentas[a].storage1Full && kultentas[a].storage2Full && kultentas[a].storage3Full && kultentas[a].storage4Full && kultentas[a].storage5Full)
                                    {
                                        if(kultentas[a].imadethisbooltosaveoncpu == false)
                                        {
                                            /*kultentas[a].dead = true;
                                            kultentas[a].hitBox->x += 17;
                                            kultentas[a].hitBox->y -= 19;
                                            kultentas[a].beingDeadTimer.start();*/
                                            kultentas[a].imadethisbooltosaveoncpu = true;
                                            kultentas[a].hitColor.start();
                                            Mix_PlayChannel( -1, aJellyDeath1, 0 );
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                for(int a = 0; a < numberOfKultentas; a++) ///collision for kultenta bullets and player hair
                {
                    for(int i = 0; i < 8; i++)
                    {
                        if(kultentas[a].bulletactive[i])
                        {
                            if(checkCollision(kultentas[a].bullets[i], player1.rectangleForTheHair, 1) == 1)
                            {
                                if(player1.clip < 15)
                                {
                                    player1.clip++;
                                }
                                gHair.animationTimer1.start();Mix_PlayChannel( -1, aTvSize, 0 );
                                kultentas[a].bulletactive[i] = false;
                            }
                        }
                    }
                }

                for(int a = 0; a < numberOfKultentas; a++) ///collision for kultenta bullets and player
                {
                        for(int i = 0; i < 8; i++)
                        {
                            if(kultentas[a].bulletactive[i])
                            {
                                if(checkCollision(kultentas[a].bullets[i], player1.hitBox, 1) == 1)
                                {
                                    player1.testingLeft = true;
                                    player1.damageColor.start();
                                    player1.health--;
                                    kultentas[a].bulletactive[i] = false;
                                }
                            }
                        }
                }

                for(int a = 0; a < numberOfKultentas; a++) ///collision for kultentas and test boxes
                {
                    if(kultentas[a].fallingToDEath)
                    {
                        for(int i = 0; i < NumberOfTestBoxes; i++)
                        {
                            if(checkCollision(kultentas[a].hitBox, testBox[i].hitBox, 1))
                            {
                                kultentas[a].fallingToDEath = false;
                            }
                        }
                    }
                }

                for(int i = 0; i < numberOfTNTgent; i++) ///collision for TNT gents and test boxes
                {
                    for(int a = 0; a < NumberOfTestBoxes; a++)
                    {
                        switch ( checkCollision(gents[i].hitBox, testBox[a].hitBox, 2) )
                        {
                            case 1: //floor
                                    gents[i].hitBox->y = testBox[a].hitBox->y - gents[i].hitBox->h + 1;
                                    gents[i].onFlatSurface = true;
                                break;                                                        //+1?
                            case 2: //hitting left wall
                                break;
                            case 3: //hitting right wall
                                break;
                            case 4:
                                    //ceiling
                                break;
                            case 5: //do nothing
                                break;
                        }
                    }
                }

                for(int i = 0; i < numberOfTNTgent; i++) ///collision for player and TNT gents trigger
                {
                    if(!gents[i].programTimer.isStarted() && gents[i].health > 0)
                    {
                        if(checkCollision(gents[i].trigger, player1.hitBox, 1) == 1)
                        {
                            /*gents[i].programTimer.start();
                            gents[i].sequence = rand() % 2 + 1;
                            gents[i].check = 1;*/

                            /*if(!gents[i].speakingTimer.isStarted() && gents[i].speakingFrame == 0)
                            {
                                gents[i].speakingTimer.start();
                                Mix_PlayChannel( -1, aTNTgent_alert, 0 );
                            }*/
                                gents[i].programTimer.start();
                                gents[i].sequence = rand() % 3 + 1;
                                gents[i].check = 1;
                                if(!gents[i].speakingTimer.isStarted())
                                {
                                    gents[i].speakingFrame = 0;
                                    gents[i].speakingTimer.start();
                                    Mix_PlayChannel( -1, aTNTgent_alert, 0 );
                                }
                        }
                    }

                    /*if(checkCollision(gents[i].trigger, player1.hitBox, 1) == 1)
                    {
                        if(!gents[i].speakingTimer.isStarted() && gents[i].speakingFrame == 0)
                        {
                            gents[i].speakingTimer.start();
                            Mix_PlayChannel( -1, aTNTgent_alert, 0 );
                        }
                    }*/
                    if(gents[i].bullshitsoundboolno1352 == false)
                    {
                        if(gents[i].animation == 5)
                        {
                            Mix_PlayChannel( -1, aTNTgent_toss, 0 );
                            gents[i].bullshitsoundboolno1352 = true;
                        }
                    }
                }



                for(int i = 0; i < 19; i++)    ///collision for player bullets and tnt gent head
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < numberOfTNTgent; a++)
                        {
                            if(gents[a].headThrown == 2 || gents[a].health < 1)
                            {

                            }
                            else
                            {
                                if(checkCollision(gents[a].hitBoxHead, player1bullets[i].hitBox, 1) == 1)
                                {
                                    player1bullets[i].active = false;
                                    gents[a].hitColorHead.start();
                                    gents[a].health--;
                                    if(gents[a].health == 0)
                                    {
                                        Mix_PlayChannel( -1, aExplosion, 0 );
                                        gents[a].explosionTimer.start();
                                    }
                                }
                            }
                        }
                    }
                }

                for(int i = 0; i < 19; i++)    ///collision for player bullets and tnt gent body
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < numberOfTNTgent; a++)
                        {
                            if(!gents[a].dead && gents[a].health > 0)
                            {
                                if(checkCollision(gents[a].hitBox, player1bullets[i].hitBox, 1) == 1)
                                {
                                    player1bullets[i].testing = true;
                                    if(!player1bullets[i].testTimer.isStarted())
                                    {
                                        if(player1bullets[i].direction == 2)
                                        {
                                            player1bullets[i].direction = 4;
                                        }
                                        else if(player1bullets[i].direction == 4)
                                        {
                                            player1bullets[i].direction = 2;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                for(int a = 0; a < numberOfTNTgent; a++) ///collision for TNTgent bullets and player hair
                {
                    for(int i = 0; i < 5; i++)
                    {
                        if(gents[a].bulletactive[i])
                        {
                            if(checkCollision(gents[a].bullets[i], player1.rectangleForTheHair, 1) == 1)
                            {
                                if(player1.clip < 15)
                                {
                                    player1.clip++;
                                }
                                gHair.animationTimer1.start();Mix_PlayChannel( -1, aTvSize, 0 );
                                gents[a].bulletactive[i] = false;
                            }
                        }
                    }
                }

                for(int a = 0; a < numberOfTNTgent; a++) ///collision for TNTgent bullets and player
                {
                    for(int i = 0; i < 5; i++)
                    {
                        if(gents[a].bulletactive[i])
                        {
                            if(checkCollision(gents[a].bullets[i], player1.hitBox, 1) == 1)
                            {
                                player1.testingLeft = true;
                                player1.damageColor.start();
                                player1.health--;
                                gents[a].bulletactive[i] = false;
                            }
                        }
                    }
                }

                for(int i = 0; i < numberOfTNTgent; i++) /// thrown TNT gent head and testboxes
                {
                    if(gents[i].headThrown == 1 && gents[i].health > 0)
                    {
                        for(int a = 0; a < NumberOfTestBoxes; a++)
                        {
                            if( checkCollision(gents[i].hitBoxHead, testBox[a].hitBox, 1) )
                            {
                                Mix_PlayChannel( -1, aExplosion, 0 );
                                gents[i].headThrown = 2;
                                gents[i].exploded = true;
                                gents[i].explosionTimer.start();
                            }
                        }
                    }
                }

                for(int a = 0; a < numberOfTNTgent; a++) ///collision for TNT gent explosion and player
                {
                    if(gents[a].explosionTimer.isStarted())
                    {
                        if(checkCollision(gents[a].explosionHitbox, player1.hitBox, 1) == 1)
                        {
                            player1.blastJumping = true;

                            if(player1.hitBox->x > gents[a].hitBoxHead->x + gents[a].hitBoxHead->w)
                            {
                                player1.blastJumpingRight = true;
                                player1.blastJumpSpeed = 80;
                            }
                            else if(player1.hitBox->x + player1.hitBox->w < gents[a].hitBoxHead->x)
                            {
                                player1.blastJumpingLeft = true;
                                player1.blastJumpSpeed = 80;
                            }
                        }
                    }
                }

                ///WERMBL COLLISION
                for(int i = 0; i < numberOfWermbls; i++) ///collision with wermbls and testboxes
                {
                    for(int a = 0; a < NumberOfTestBoxes; a++)
                    {
                        switch ( checkCollision(wermbls[i].hitBox, testBox[a].hitBox, 2) )
                        {
                            case 1:     if(!wermbls[i].flying)
                                        {
                                            wermbls[i].hitBox->y = testBox[a].hitBox->y - wermbls[i].hitBox->h -0;
                                            wermbls[i].onFlatSurface = true;
                                        }
                                break;
                            case 2: //hitting left wall
                                    if(wermbls[i].walkingRight)
                                    {
                                        wermbls[i].flip = SDL_FLIP_NONE;
                                        wermbls[i].walking = true;
                                        wermbls[i].walkingRight = false;
                                    }
                                break;
                            case 3: //hitting right wall
                                    if(wermbls[i].walking)
                                    {
                                        wermbls[i].flip = SDL_FLIP_HORIZONTAL;
                                        wermbls[i].walking = false;
                                        wermbls[i].walkingRight = true;
                                    }
                                break;
                            case 4: //wermbls should never hit a ceiling
                                break;
                            case 5: //do nothing

                                break;
                        }
                    }
                }

                for(int i = 0; i < 19; i++)    ///collision for player bullets and wermbl face
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < numberOfWermbls; a++)
                        {

                            if(checkCollision(wermbls[a].hitBoxHead, player1bullets[i].hitBox, 1) == 1)
                            {
                                if(wermbls[a].numberOfBulletsEaten < 3 && wermbls[a].gachAvtivationState == 0)
                                {
                                                                                    if(wermbls[a].numberOfBulletsEaten == 1)
                                                                                    {
                                                                                        Mix_PlayChannel( -1, aWermblTWO, 0 );
                                                                                    }
                                                                                    else if(wermbls[a].numberOfBulletsEaten == 2)
                                                                                    {
                                                                                        Mix_PlayChannel( -1, aWermblTHREE, 0 );
                                                                                    }
                                                                                    else
                                                                                    {
                                                                                       Mix_PlayChannel( -1, aWermblONE, 0 );;
                                                                                    }

                                    player1bullets[i].active = false;
                                    wermbls[a].walking = false;
                                    wermbls[a].walkingRight = false;
                                    gWermbls[a].animationTimer4.start();
                                    gWermbls[a].animationTimer1.pause();
                                    wermbls[a].numberOfBulletsEaten++;
                                    wermbls[a].gachaActivationTimer.start();
                                }
                                else
                                {
                                    player1bullets[i].testing = true;
                                    if(!player1bullets[i].testTimer.isStarted())
                                    {
                                        if(player1bullets[i].direction == 2)
                                        {
                                            player1bullets[i].direction = 4;
                                        }
                                        else if(player1bullets[i].direction == 4)
                                        {
                                            player1bullets[i].direction = 2;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }

                for(int i = 0; i < 19; i++)    ///collision for player bullets and wermbl body
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < numberOfWermbls; a++)
                        {
                            if(checkCollision(wermbls[a].hitBox, player1bullets[i].hitBox, 1) == 1)
                            {
                                player1bullets[i].testing = true;
                                if(!player1bullets[i].testTimer.isStarted())
                                {
                                    if(player1bullets[i].direction == 2)
                                    {
                                        player1bullets[i].direction = 4;
                                    }
                                    else if(player1bullets[i].direction == 4)
                                    {
                                        player1bullets[i].direction = 2;
                                    }
                                }
                            }
                        }
                    }
                }

                for(int i = 0; i < numberOfWermbls; i++)
                {
                        switch ( checkCollision(player1.hitBox, wermbls[i].hitBox, 2) ) ///collision for player and wermbl body
                        {
                            case 1: player1.yVelocity = wermbls[i].hitBox->y - player1.hitBox->h + 1;
                                    player1.foo = true;
                                    if(wermbls[i].walking)
                                    {
                                        player1.velocity -=1;
                                    }
                                    if(player1.jumpTimer.getTicks() > 40) {player1.jumping = false; player1.jumpTimer.stop(); }

                                    if(!player1.ect)
                                    {
                                        wermbls[i].flyingUp = true;
                                    }
                                    wermbls[i].flyingSpeed = 20;
                                break;
                            case 2: player1.velocity = wermbls[i].hitBox->x - player1.hitBox->w +1;
                                    player1.bar = true;
                                    if(player1.baz)
                                    {
                                        wermbls[i].walking = false;
                                        wermbls[i].walkingRight = true;
                                        wermbls[i].flip = SDL_FLIP_HORIZONTAL;
                                    }
                                break;
                            case 3: player1.velocity = wermbls[i].hitBox->x + wermbls[i].hitBox->w -1;
                                    player1.baz = true;
                                    if(player1.bar)
                                    {
                                        wermbls[i].walking = true;
                                        wermbls[i].walkingRight = false;
                                        wermbls[i].flip = SDL_FLIP_NONE;
                                    }
                                break;
                            case 4:
                                break;
                            case 5: //do nothing

                                break;
                       // }
                    }

                }

                for(int a = 0; a < numberOfWermbls; a++) ///collision for wermbl bullets and player hair
                {
                    for(int i = 0; i < 3; i++)
                    {
                        if(wermbls[a].bulletactive[i])
                        {
                            if(checkCollision(wermbls[a].bullets[i], player1.rectangleForTheHair, 1) == 1)
                            {
                                if(player1.clip < 15)
                                {
                                    player1.clip++;
                                }
                                gHair.animationTimer1.start();Mix_PlayChannel( -1, aTvSize, 0 );
                                wermbls[a].bulletactive[i] = false;
                            }
                        }
                    }
                }

                for(int a = 0; a < numberOfWermbls; a++) ///collision for wermbl bullets and player
                {
                    for(int i = 0; i < 3; i++)
                    {
                        if(wermbls[a].bulletactive[i])
                        {
                            if(checkCollision(wermbls[a].bullets[i], player1.hitBox, 1) == 1)
                            {
                                player1.testingLeft = true;
                                player1.damageColor.start();
                                player1.health--;
                                wermbls[a].bulletactive[i] = false;
                            }
                        }
                    }
                }

                for(int a = 0; a < numberOfTNTgent; a++) ///collision for wermbl explosion and player
                {
                    if(wermbls[a].explosionSoundBoolUgh)
                    {
                        Mix_PlayChannel( -1, aExplosion1, 0 );
                    }

                    if(wermbls[a].explosionTimer.isStarted())
                    {
                        if(checkCollision(wermbls[a].explosionHitbox, player1.hitBox, 1) == 1)
                        {
                            player1.blastJumping = true;

                            if(player1.hitBox->x > wermbls[a].hitBox->x + wermbls[a].hitBox->w)
                            {
                                player1.blastJumpingRight = true;
                                player1.blastJumpSpeed = 80;
                            }
                            else if(player1.hitBox->x + player1.hitBox->w < wermbls[a].hitBox->x)
                            {
                                player1.blastJumpingLeft = true;
                                player1.blastJumpSpeed = 80;
                            }
                        }
                    }
                }

                ///GARBAGE GAURDIAN COLLISION
                for(int i = 0; i < numberOfGarbageGaurdians; i++) ///collision with garbage gaurdians and testboxes
                {
                    for(int a = 0; a < NumberOfTestBoxes; a++)
                    {
                        if( checkCollision(garbageGaurdians[i].hitBox, testBox[a].hitBox, 1) )
                        {
                            garbageGaurdians[i].onFlatSurface = true;
                        }
                    }
                }

                for(int i = 0; i < numberOfGarbageGaurdians; i++)
                {
                        switch ( checkCollision(player1.hitBox, garbageGaurdians[i].hitBox, 2) ) ///collision for player and garbage gaurdian can
                        {
                            case 1: player1.yVelocity = garbageGaurdians[i].hitBox->y - player1.hitBox->h + 1;
                                    player1.foo = true;
                                    if(player1.jumpTimer.getTicks() > 40) {player1.jumping = false; player1.jumpTimer.stop(); }
                                break;
                            case 2: player1.velocity = garbageGaurdians[i].hitBox->x - player1.hitBox->w +1;
                                    player1.bar = true;
                                break;
                            case 3: player1.velocity = garbageGaurdians[i].hitBox->x + garbageGaurdians[i].hitBox->w -1;
                                    player1.baz = true;
                                break;
                            case 4:
                                break;
                            case 5: //do nothing

                                break;
                    }
                }

                for(int i = 0; i < 19; i++)    ///collision for player bullets and garbage gaurdian can
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < numberOfGarbageGaurdians; a++)
                        {
                            if(checkCollision(garbageGaurdians[a].hitBox, player1bullets[i].hitBox, 1) == 1)
                            {
                                player1bullets[i].testing = true;
                                if(!player1bullets[i].testTimer.isStarted())
                                {
                                    if(player1bullets[i].direction == 2)
                                    {
                                        player1bullets[i].direction = 4;
                                    }
                                    else if(player1bullets[i].direction == 4)
                                    {
                                        player1bullets[i].direction = 2;
                                    }
                                }
                            }
                        }
                    }
                }

                for(int i = 0; i < numberOfGarbageGaurdians; i++)
                {
                    if(garbageGaurdians[i].shieldFlying)
                    {
                        switch ( checkCollision(player1.hitBox, garbageGaurdians[i].shieldHitbox, 2) ) ///collision for player and garbage gaurdian shield
                        {
                            case 1: player1.yVelocity = garbageGaurdians[i].shieldHitbox->y - player1.hitBox->h + 1;
                                    player1.foo = true;
                                    if(player1.jumpTimer.getTicks() > 40) {player1.jumping = false; player1.jumpTimer.stop(); }

                                    player1.velocity -= 7;
                                break;
                            case 2: player1.velocity = garbageGaurdians[i].shieldHitbox->x - player1.hitBox->w +1;
                                    player1.bar = true;
                                    garbageGaurdians[i].shieldHealth--;
                                    player1.damageColor.start();
                                    player1.health--;
                                    player1.testingLeft = true;
                                break;
                            case 3: player1.velocity = garbageGaurdians[i].shieldHitbox->x + garbageGaurdians[i].shieldHitbox->w -1;
                                    player1.baz = true;
                                    garbageGaurdians[i].shieldHealth--;
                                    player1.damageColor.start();
                                    player1.health--;
                                    player1.testingLeft = true;
                                break;
                            case 4:
                                    ///you should prolly never be able to jump into it
                                break;
                            case 5: //do nothing

                                break;
                        }
                    }
                }

                for(int i = 0; i < numberOfGarbageGaurdians; i++) ///collision with shield and player hair
                {
                    if(garbageGaurdians[i].shieldFlying && !player1.gunBouncing)
                    {
                        if( checkCollision(garbageGaurdians[i].shieldHitbox, player1.rectangleForTheHair, 1) )
                        {
                            garbageGaurdians[i].shieldHealth--;
                            player1.damageColor.start();
                            player1.health--;
                            player1.testingLeft = true;
                            ///crash
                        }
                    }
                }

                for(int i = 0; i < numberOfGarbageGaurdians; i++) ///collision with garbage gaurdian sword and testboxes
                {
                    for(int a = 0; a < NumberOfTestBoxes; a++)
                    {
                        if( checkCollision(garbageGaurdians[i].swordHitbox, testBox[a].hitBox, 1) )
                        {
                            garbageGaurdians[i].swordFlying = false;
                            garbageGaurdians[i].swordInGround = true;
                        }
                    }
                }

                for(int i = 0; i < numberOfGarbageGaurdians; i++) ///collision with garbage gaurdian sword and player
                {
                    if(garbageGaurdians[i].swordFlying)
                    {
                        if( checkCollision(garbageGaurdians[i].swordHitbox, player1.hitBox, 1) )
                        {
                            player1.damageColor.start();
                            player1.health--;
                            player1.testingLeft = true;
                        }
                    }
                }

                for(int i = 0; i < numberOfGarbageGaurdians; i++)
                {
                    if(garbageGaurdians[i].swordInGround)
                    {
                        if(player1.hitBox->x + player1.hitBox->w / 2 > garbageGaurdians[i].swordRamp.x1 && player1.velocity + player1.hitBox->w / 2 < garbageGaurdians[i].swordRamp.x2) ///collision for player and sword ramp
                        {
                            if(player1.hitBox->y + player1.hitBox->h > (garbageGaurdians[i].swordRamp.y1 - 15) - ((player1.hitBox->x - garbageGaurdians[i].swordRamp.x1) / garbageGaurdians[i].swordRamp.slope) + 1  && (player1.hitBox->y + player1.hitBox->h < (garbageGaurdians[i].swordRamp.y1 - 15) - ((player1.hitBox->x - garbageGaurdians[i].swordRamp.x1) / garbageGaurdians[i].swordRamp.slope) + 85) )  ///this is complicated
                            {                                                                                                                                                                                                                     /// i think 85 means 85 pixesl below it
                                player1.yVelocity = (garbageGaurdians[i].swordRamp.y1 - 15) - ((player1.hitBox->x - garbageGaurdians[i].swordRamp.x1) / garbageGaurdians[i].swordRamp.slope) - player1.hitBox->h + 11;
                                player1.foo = true;
                                if(player1.released) { player1.jumping = false;} //uhhh i dont think this does anything anymore but iduno
                            }
                        }
                    }
                }

                for(int i = 0; i < 19; i++)    ///collision for player bullets and garbage gaurdian body
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < numberOfGarbageGaurdians; a++)
                        {
                            if(garbageGaurdians[a].state > 1 && garbageGaurdians[a].health > 0)
                            {
                                if(checkCollision(garbageGaurdians[a].hitBoxBody, player1bullets[i].hitBox, 1) == 1)
                                {
                                    garbageGaurdians[a].hitColorBody.start();
                                    garbageGaurdians[a].health--;

                                    player1bullets[i].active = false;
                                }
                            }
                        }
                    }
                }

                for(int i = 0; i < 19; i++)    ///collision for player bullets and garbage gaurdian shield
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < numberOfGarbageGaurdians; a++)
                        {
                            if(garbageGaurdians[a].state > 1) //&& garbageGaurdians[a].health > 0) uncomment this l8r
                            {
                                if(checkCollision(garbageGaurdians[a].shieldHitbox, player1bullets[i].hitBox, 1) == 1)
                                {
                                    player1bullets[i].testing = true;
                                    if(!player1bullets[i].testTimer.isStarted())
                                    {
                                        if(player1bullets[i].direction == 2)
                                        {
                                            player1bullets[i].direction = 4;
                                        }
                                        else if(player1bullets[i].direction == 4)
                                        {
                                            player1bullets[i].direction = 2;
                                        }
                                    }
                                }
                            }
                            else if(garbageGaurdians[a].shieldFlying)
                            {
                                garbageGaurdians[a].shieldHealth--;
                            }
                        }
                    }
                }

                ///KONGA LEE COLLISION
                for(int i = 0; i < numberOfKongaLees; i++) ///collision with konga lees and testboxes
                {
                     if(kongaLees[i].freeFall)
                     {
                        for(int a = 0; a < NumberOfTestBoxes; a++)
                        {
                            if( checkCollision(kongaLees[i].hitBoxBody, testBox[a].hitBox, 1) )
                            {
                                kongaLees[i].freeFall = false;
                                kongaLees[i].onFloorGoingLeft = true;
                                kongaLees[i].footHasDetectedASurface = true;
                            }
                        }
                    }
                }

                for(int i = 0; i < numberOfKongaLees; i++) ///collision with konga lees feets and testboxes
                {
                    for(int a = 0; a < NumberOfTestBoxes; a++)
                    {
                        if( checkCollision(kongaLees[i].hitBoxFoot, testBox[a].hitBox, 1) )
                        {
                            kongaLees[i].footHasDetectedASurface = true;
                        }
                    }
                }

                for(int i = 0; i < numberOfKongaLees; i++) ///collision with konga lees face and testboxes
                {
                    for(int a = 0; a < NumberOfTestBoxes; a++)
                    {
                        if( checkCollision(kongaLees[i].hitBoxFace, testBox[a].hitBox, 1) )
                        {
                            kongaLees[i].faceHasDetectedASurface = true;
                        }
                    }
                }

                for(int i = 0; i < 19; i++)    ///collision for player bullets and KongaLee
                {
                    if(player1bullets[i].active)
                    {
                        for(int a = 0; a < numberOfKongaLees; a++)
                        {
                            if(checkCollision(kongaLees[a].hitBoxBody, player1bullets[i].hitBox, 1) == 1)
                            {
                                switch ( kongaLees[a].variant )
                                {
                                    case 0: ///banana konga lee
                                            if(kongaLees[a].alive)
                                            {
                                                kongaLees[a].spitting = true;
                                                kongaLees[a].alive = false;
                                                player1bullets[i].active = false;
                                                kongaLees[a].shootTimer.start();
                                                kongaLees[a].executingTurn_GOINGLEFT_ONFLOOR_TO_LEFTWALL = false;
                                                kongaLees[a].executingTurn_GOINGDOWN_ONLEFTWALL_TO_CEILING = false;
                                                kongaLees[a].executingTurn_GOINGRIGHT_ONCEILING_TO_RIGHTWALL = false;
                                                kongaLees[a].executingTurn_GOINGUP_ONRIGHTWALL_TO_FLOOR = false;

                                                kongaLees[a].executingTurn_GOINGLEFT_ONFLOOR_TO_RIGHTWALL = false;
                                                kongaLees[a].executingTurn_GOINGUP_ONRIGHTWALL_TO_CEILING = false;
                                                kongaLees[a].executingTurn_GOINGRIGHT_ONCEILING_TO_LEFTWALL = false;
                                                kongaLees[a].executingTurn_GOINGDOWN_ONLEFTWALL_TO_FLOOR = false;
                                            }
                                        break;
                                    case 1: ///strong konga lee
                                            player1bullets[i].testing = true;
                                            if(!player1bullets[i].testTimer.isStarted())
                                            {
                                                if(player1bullets[i].direction == 2)
                                                {
                                                    player1bullets[i].direction = 4;
                                                }
                                                else if(player1bullets[i].direction == 4)
                                                {
                                                    player1bullets[i].direction = 2;
                                                }
                                            }
                                        break;
                                    case 2: ///bomba konga lee
                                            if(kongaLees[a].alive)
                                            {
                                                kongaLees[a].exploded = true;
                                                kongaLees[a].alive = false;
                                                kongaLees[a].explosionTimer.start();
                                                Mix_PlayChannel( -1, aExplosion, 0 );
                                            }
                                        break;
                                }

                            }
                        }
                    }
                }


                for(int a = 0; a < numberOfKongaLees; a++) ///collision for konga lee bullets and player hair
                {
                    for(int i = 0; i < 3; i++)
                    {
                       if(kongaLees[a].bulletactive[i])
                        {
                            if(checkCollision(kongaLees[a].bullets[i], player1.rectangleForTheHair, 1) == 1)
                            {
                                if(player1.clip < 15)
                                {
                                    player1.clip++;
                                }
                                gHair.animationTimer1.start();Mix_PlayChannel( -1, aTvSize, 0 );
                                kongaLees[a].bulletactive[i] = false;
                            }
                       }
                    }
                }

                for(int a = 0; a < numberOfKongaLees; a++) ///collision for konga lee bullets and player body
                {
                    for(int i = 0; i < 3; i++)
                    {
                       if(kongaLees[a].bulletactive[i])
                        {
                            if(checkCollision(kongaLees[a].bullets[i], player1.hitBox, 1) == 1)
                            {
                                kongaLees[a].bulletactive[i] = false;
                                player1.damageColor.start();
                                player1.health--;
                            }
                       }
                    }
                }

                for(int a = 0; a < numberOfKongaLees; a++) ///collision for konga lee explosion and player
                {
                    if(kongaLees[a].explosionTimer.isStarted())
                    {
                        if(checkCollision(kongaLees[a].explosionHitbox, player1.hitBox, 1) == 1)
                        {
                            player1.blastJumping = true;

                            if(player1.hitBox->x > kongaLees[a].hitBoxBody->x + kongaLees[a].hitBoxBody->w + 10)
                            {
                                player1.blastJumpingRight = true;
                                player1.blastJumpSpeed = 80;
                            }
                            else if(player1.hitBox->x + player1.hitBox->w < kongaLees[a].hitBoxBody->x - 10)
                            {
                                player1.blastJumpingLeft = true;
                                player1.blastJumpSpeed = 80;
                            }
                        }
                    }
                }

                ///SORSE COLLISION DETECT

                for(int i = 0; i < numberOfSorse; i++) ///uhhhh decides which way the sorse faces
                {
                    if(sorse[i].alive && !sorse[i].summonAnimation.isStarted() && !sorse[i].attackAnimation.isStarted())
                    {
                        if(player1.hitBox->x + (player1.hitBox->w / 2) < sorse[i].hitBoxBody->x + (sorse[i].hitBoxBody->w / 2))
                        {
                            sorse[i].flip = SDL_FLIP_HORIZONTAL;
                        }
                        else
                        {
                            sorse[i].flip = SDL_FLIP_NONE;
                        }
                    }
                }

                player1.moonGravity = false; //move this to bottom of move
                for(int i = 0; i < numberOfSorse; i++) ///collision with sorse black hole and player
                {
                    if(sorse[i].blackHoleActive)
                    {
                        if(checkCollision(sorse[i].hitBoxBlackHole, player1.hitBox, 1) == 1)
                        {
                            if(player1.hitBox->x + (player1.hitBox->w / 2) < sorse[i].hitBoxBlackHole->x + (sorse[i].hitBoxBlackHole->w / 2))
                            {
                                if(player1.hitBox->x + (player1.hitBox->w / 2) < sorse[i].hitBoxBlackHole->x + (sorse[i].hitBoxBlackHole->w / 2) - 120)
                                {
                                    player1.velocity += 1;
                                    printf("a");
                                }
                                else if(player1.hitBox->x + (player1.hitBox->w / 2) < sorse[i].hitBoxBlackHole->x + (sorse[i].hitBoxBlackHole->w / 2) - 90)
                                {
                                    player1.velocity += 2;
                                    printf("b");
                                }
                                else if(player1.hitBox->x + (player1.hitBox->w / 2) < sorse[i].hitBoxBlackHole->x + (sorse[i].hitBoxBlackHole->w / 2) - 60)
                                {
                                    player1.velocity += 3;
                                    printf("c");
                                }
                                else if(player1.hitBox->x + (player1.hitBox->w / 2) < sorse[i].hitBoxBlackHole->x + (sorse[i].hitBoxBlackHole->w / 2) - 30)
                                {
                                    player1.velocity += 4;
                                    printf("d");
                                }
                                else
                                {
                                    player1.velocity += 4.5;
                                    printf("e");
                                }
                            }
                            else
                            {
                                if(player1.hitBox->x + (player1.hitBox->w / 2) > sorse[i].hitBoxBlackHole->x + (sorse[i].hitBoxBlackHole->w / 2) + 120)
                                {
                                    player1.velocity -= 1;
                                    printf("a");
                                }
                                else if(player1.hitBox->x + (player1.hitBox->w / 2) > sorse[i].hitBoxBlackHole->x + (sorse[i].hitBoxBlackHole->w / 2) + 90)
                                {
                                    player1.velocity -= 2;
                                    printf("b");
                                }
                                else if(player1.hitBox->x + (player1.hitBox->w / 2) > sorse[i].hitBoxBlackHole->x + (sorse[i].hitBoxBlackHole->w / 2) + 60)
                                {
                                    player1.velocity -= 3;
                                    printf("c");
                                }
                                else if(player1.hitBox->x + (player1.hitBox->w / 2) > sorse[i].hitBoxBlackHole->x + (sorse[i].hitBoxBlackHole->w / 2) + 30)
                                {
                                    player1.velocity -= 4;
                                    printf("d");
                                }
                                else
                                {
                                    player1.velocity -= 4.5;
                                    printf("e");
                                }
                            }
                            player1.moonGravity = true;
                        }
                    }
                }

                for(int i = 0; i < numberOfSorse; i++) ///collision with sorse trigger and player
                {
                    if(sorse[i].alive)
                    {
                        if(checkCollision(sorse[i].hitBoxTrigger, player1.hitBox, 1) == 1)
                        {
                            if(!sorse[i].blackHoleActive && !sorse[i].summonAnimation.isStarted() && !sorse[i].blackHoleFlying)
                            {
                                if(gSorse[i].animationBool)
                                {
                                    sorse[i].summonAnimation.start();
                                }
                            }

                            if(player1.velocity > sorse[i].hitBoxBody->x + sorse[i].hitBoxBody->w)
                            {
                                sorse[i].followRight = true;
                            }
                            else if(player1.velocity - player1.hitBox->w < sorse[i].hitBoxBody->x)
                            {
                                sorse[i].followLeft = true;
                            }
                        }
                    }
                }

                for(int i = 0; i < numberOfSorse; i++) ///collision with sorse feet and testboxes
                {
                    if(sorse[i].alive)
                    {
                        for(int a = 0; a < NumberOfTestBoxes; a++)
                        {
                            if( checkCollision(sorse[i].hitBoxFeet, testBox[a].hitBox, 1) )
                            {
                                sorse[i].onFlatSurface = true;
                            }
                        }
                    }
                }

                for(int i = 0; i < numberOfSorse; i++) ///collision with sorse feet and player
                {
                    if(sorse[i].alive)
                    {
                        if(checkCollision(sorse[i].hitBoxFeet, player1.hitBox, 1) == 1)
                        {
                            if(!sorse[i].attackAnimation.isStarted() && !sorse[i].summonAnimation.isStarted())
                            {
                                if(gSorse[i].animationBool)
                                {
                                    sorse[i].attackAnimation.start();
                                }
                            }
                        }
                    }
                }

                for(int a = 0; a < numberOfSorse; a++) ///collision for sorse explosion and player
                {
                    if(sorse[a].alive && sorse[a].exploding)
                    {
                        if(checkCollision(sorse[a].hitBoxExplosion, player1.hitBox, 1) == 1)
                        {
                            player1.blastJumping = true;
                            player1.damageColor.start();
                            player1.health--;

                            if(player1.hitBox->x + (player1.hitBox->w / 2) > sorse[a].hitBoxExplosion->x + (sorse[a].hitBoxExplosion->w / 2))
                            {
                                player1.blastJumpingRight = true;
                                player1.blastJumpSpeed = 80;
                            }
                            else if(player1.hitBox->x + (player1.hitBox->w / 2) < sorse[a].hitBoxExplosion->x + (sorse[a].hitBoxExplosion->w / 2))
                            {
                                player1.blastJumpingLeft = true;
                                player1.blastJumpSpeed = 80;
                            }
                        }
                    }
                }



                    for(int a = 0; a < numberOfSorse; a++)
                    {
                        if(sorse[a].alive)
                        {
                            switch(gSorse[a].animationInt) ///yup this is where im gonna actually freakin move the eyes around
                            {
                                case 0: sorse[a].hitBoxEyes->y = sorse[a].hitBoxBody->y + 76;
                                break;
                                case 1: sorse[a].hitBoxEyes->y = sorse[a].hitBoxBody->y + 45;
                                break;
                                case 2: sorse[a].hitBoxEyes->y = sorse[a].hitBoxBody->y + 18;
                                break;
                            }

                            for(int i = 0; i < 19; i++)    ///collision for player bullets and Sorse weakpoint
                            {
                                if(player1bullets[i].active)
                                {
                                    if(checkCollision(sorse[a].hitBoxEyes, player1bullets[i].hitBox, 1) == 1)
                                    {
                                        player1bullets[i].active = false;
                                        sorse[a].hitColor.start();
                                        sorse[a].health--;

                                        if(sorse[a].health < 1)
                                        {
                                            sorse[a].collapsing = true;
                                            sorse[a].hitBoxBody->h = 103;
                                        }
                                    }
                                }
                            }
                        }
                    }

                    for(int i = 0; i < numberOfSorse; i++) ///collision for collappsing sorse body and testboxes
                    {
                        if(sorse[i].collapsing)
                        {
                            for(int a = 0; a < NumberOfTestBoxes; a++)
                            {
                                if( checkCollision(sorse[i].hitBoxBody, testBox[a].hitBox, 1) )
                                {
                                    sorse[i].collapsing = false;
                                }
                            }
                        }
                    }

                    for(int a = 0; a < numberOfSorse; a++)
                    {
                        if(sorse[a].blackHoleActive)
                        {
                            for(int i = 0; i < 19; i++)    ///collision for player bullets and Sorse weakpoint
                            {
                                if(player1bullets[i].active)
                                {
                                    if(checkCollision(sorse[a].hitBoxBlackHole, player1bullets[i].hitBox, 1) == 1)
                                    {
                                        player1bullets[i].inBlackHole = true;
                                        player1bullets[i].blackHoleX = sorse[a].blackHoleSuccer.x;
                                        player1bullets[i].blackHoleY = sorse[a].blackHoleSuccer.y;
                                    }
                                }
                            }
                        }
                        else
                        {
                            for(int i = 0; i < 19; i++)    ///collision for player bullets and Sorse weakpoint
                            {
                                if(player1bullets[i].active)
                                {
                                    if(checkCollision(sorse[a].hitBoxBlackHole, player1bullets[i].hitBox, 1) == 1)
                                    {
                                        player1bullets[i].inBlackHole = false;
                                    }
                                }
                            }
                        }
                    }

                    for(int a = 0; a < numberOfSorse; a++)
                    {
                        if(sorse[a].blackHoleActive && player1.granadoExploding)
                        {
                            if(checkCollision(sorse[a].hitBoxBlackHole, player1.granadoHitbox, 1) == 1)
                            {
                                sorse[a].blackHoleActive = false;
                            }
                        }
                    }

                ///DOOR COLLISION
                for(int i = 0; i < NumberOfDoors; i++) ///collisionw ith player and doors
                {
                    if(checkCollision(doors[i].hitBox, player1.hitBox, 1) == 1)
                    {
                        player1.nearItemButtonOrMachine = true;

                        if(player1.useItemButtonOrMachine)
                        {
                            if(doors[i].destinationX != 0)
                            {
                                player1.velocity = doors[i].destinationX;
                                player1.yVelocity = doors[i].destinationY;
                            }

                            switch(doors[i].IDN)
                            {
                                case 0:  //debug area

                                break;
                                case 1:  //level 1
                                         destination = 1;
                                         if(loadBGalpha == 0) {loadBGalpha = 2;}
                                         gLoadingBG.setBlendMode(SDL_BLENDMODE_BLEND);
                                         printf("Entering Level 1");
                                break;
                                case 2:  //tutorial

                                break;
                            }
                        }
                    }
                }

                ///SHADY ASS COLLISION CODE
                ////////////////////////////SHADY ASS COLLISION CODE
                ///SHADY ASS COLLISION CODE

                ///this is where i'll handle sounds i guess???
                Mix_AllocateChannels(16);
                aTvSize->volume = 10;
                aTeleMaggotLeft->volume = 50;
                aTeleMaggotRight->volume = 50;

                if(player1.ASSsoundMissile)
                {
                    Mix_PlayChannel( -1, aMissile, 0 );
                }

                if(player1.ASSsoundRising)
                {
                    Mix_PlayChannel( -1, aRising, 0 );
                }

                if(player1.ASSsoundLand)
                {
                    Mix_PlayChannel( -1, aLand, 0 );
                }

                if(snowman1.time2explode)
                {
                    Mix_PlayChannel( -1, aExplosion, 0 );
                }

                if(maggot.aTeleSound)
                {
                    if(maggot.direction == 2)
                    {
                        Mix_PlayChannel( -1, aTeleMaggotLeft, 0 );
                    }
                    else
                    {
                        Mix_PlayChannel( -1, aTeleMaggotRight, 0 );
                    }
                }

                if(!Mix_Playing(7))
                {
                    if(player1.reloadTimer.getTicks() > 400 && player1.reloadTimer.getTicks() < 1000)
                    {
                        Mix_PlayChannel( 7, aReload, 0 );
                    }

                    if(player1.gunTimer.getTicks() > 0)
                    {
                        if(player1.clip == 0)
                        {
                            Mix_PlayChannel( 7, aShootEmpty, 0 );
                        }
                        else
                        {
                            Mix_PlayChannel( 7, aShootSemi, 0 );
                        }
                    }
                }

                if(player1.time2explodeGranado)
                {
                    if(player1.granadoTimer.getTicks() > 3000)
                    {
                        Mix_PlayChannel( -1, aExplosion1, 0 );
                    }
                }

                if(player1.time2sizzle)
                {
                    Mix_PlayChannel( -1, aGranadoPrimed, 0 );
                }

                for(int i = 0; i < numberOfKultentas; i++)
                {
                    if(kultentas[i].gayAssSoundBool)
                    {
                        Mix_PlayChannel( -1, aJellyDeath2, 0 );
                    }
                }
                ///end of sound stuff






                //SDL_RenderDrawLine(window.gRenderer, testSlopes[0].x1 - camera->x , testSlopes[0].y1 - camera->y, testSlopes[0].x2 - camera->x , testSlopes[0].y2 - camera->y);  ///DA 2nd SLOPE



                //SDL_RenderDrawLine(window.gRenderer, 4626 - camera->x , 767 - camera->y, 4913 - camera->x , 732 - camera->y);  ///DA SLOPE

                if(player1.hitBox->x + player1.hitBox->w / 2 > 4626 && player1.velocity + player1.hitBox->w / 2 < 4913)
                {             ///middle of the player          x1
                                                              ///y1 - p.hitbox->w / 2 - (where you're at - x1? / slope)
                    if(player1.hitBox->y + player1.hitBox->h > (767 - 15) - ((player1.hitBox->x - 4626) / 8) + 1  && (player1.hitBox->y + player1.hitBox->h < (767 - 15) - ((player1.hitBox->x - 4626) / 8) + 85) )  ///this is complicated
                    {                                                                                                                                                                                                   //you're tellin me
                        player1.yVelocity = (767 - 15) - ((player1.hitBox->x - 4626) / 8) - player1.hitBox->h + 11;
                        player1.foo = true;
                        if(player1.released) { player1.jumping = false;} //if i don't put it here then u still jump for a frame and the screen shakes :(
                    }
                }

                player1.update();
                zipline1.update();
                for(int i = 0; i < numberOfRobots; i++)
                {
                   robots[i].update(gRobot[i].robotAnimation);
                }
                for(int i = 0; i < 19; i++)
                {
                    player1bullets[i].update();
                }

                maggot.update();
                for(int i = 0; i < 19; i++)
                {
                    enemyBullets[i].update();
                }

                for(int i = 0; i < numberOfSharkRobots; i++)
                {
                   sharkRobots[i].update();
                }

                for(int i = 0; i < numberOfKultentas; i++)
                {
                   kultentas[i].update();
                }

                for(int i = 0; i < numberOfTNTgent; i++)
                {
                   gents[i].update();
                }

                for(int i = 0; i < numberOfWermbls; i++)
                {
                   wermbls[i].update();
                }

                for(int i = 0; i < numberOfGarbageGaurdians; i++)
                {
                   garbageGaurdians[i].update();
                }

                for(int i = 0; i < numberOfKongaLees; i++)
                {
                   kongaLees[i].update();
                }

                for(int i = 0; i < numberOfSorse; i++)
                {
                   sorse[i].update();
                }

                camera->x = ( player1.hitBox->x + player1.hitBox->w / 2 ) - camera->w / 2;
               // if(!player1.crouched) {camera->y = ( player1.hitBox->y - player1.hitBox->h / 2 ) - camera->h / 2;}
                //else {camera->y = ( player1.hitBox->y - 30 - player1.hitBox->h / 2 ) - camera->h / 2;}


                if(!player1.fallen)
                {
                    if(camera->y != player1.hitBox->y)
                    {
                        camera->y = (camera->y + 0.20 * (player1.hitBox->y - camera->y)) - (window.SCREEN_HEIGHT / 9); ///so basically this is lerp
                    }
                }
                else
                {
                    if(camera->y != player1.hitBox->y)
                    {
                        camera->y = (camera->y + 0.20 * (player1.hitBox->y - camera->y)) - (window.SCREEN_HEIGHT / 9) - 10; ///so basically this is lerp
                    }
                }



                }
        accumulator -= dt;




        }

                //if(!player1.paused)
                if(1)
                {
				/******** rendering ********/
				SDL_SetRenderDrawColor( window.gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( window.gRenderer );

				gBackground.render(window.gRenderer, 0 , 0);
				gDemoDay.render(window.gRenderer, 30 - camera->x, 60 - camera->y);///discover the secrets of the human genome project
				gTutorialbg.render(window.gRenderer, -4320 - camera->x, -1052 - camera->y);
				gTutorialbg2.render(window.gRenderer, 680 - camera->x, -1052 - camera->y);
				gTutorial3.render(window.gRenderer, -3850 - camera->x, -1622 - camera->y);
				gHubWorld_BG.render(window.gRenderer, -3980 - camera->x, -2513 - camera->y);
				gSpinner.defaultAnimate(window.gRenderer, 5000 - camera->x, -30 - camera->y, 55, 82, 80);
				if(player1.nearOxygen && !player1.crouched && !player1.succingOxygen && !player1.walkingToLocation && !player1.fallen) { gCeeButton.render(window.gRenderer, player1.hitBox->x - 5 - camera->x, player1.hitBox->y - 65 - camera->y); }
				if(player1.nearItemButtonOrMachine) { gCeeButton.render(window.gRenderer, player1.hitBox->x - 5 - camera->x, player1.hitBox->y - 65 - camera->y); }
				gTutorialZipline1.render(window.gRenderer, 1400 - camera->x, 300 - camera->y);
				gTutorialZipline1.render(window.gRenderer, 4903 - camera->x, 480 - camera->y);
				gTutorialEnemy.render(window.gRenderer, 2656 - camera->x, 679 - camera->y);



				if(player1.animation == 6 && gPlayerArmed.animationTimer4.getTicks() / 95 > 1) {oxygentank.activated = true;}
                    if(player1.animation == 6 && gPlayerArmed.animationTimer4.getTicks() / 95 > 16) {oxygentank.activated = false;}
				if(!oxygentank.activated)
                { gOxygenTank.render2(window.gRenderer, oxygentank.xPos + 15 - camera->x, oxygentank.yPos - camera->y, 22, 70, 22, 0); }
                else
                { gOxygenTank.render2(window.gRenderer, oxygentank.xPos + 15 - camera->x, oxygentank.yPos - camera->y, 22, 70, 0, 0);}

				if(debug_bool_delete_this_later_idiot)
                {
                    gLeverOff.render(window.gRenderer, lever.xPos - camera->x, lever.yPos - camera->y);
                }
                else
                {
                    gLeverOn.render(window.gRenderer, lever.xPos - camera->x, lever.yPos - camera->y);
                }

                ///render trucks
                for(int i = 0; i < numberOfTrucks; i++) { if(trucks[i].health > 0) {gArmoredTruckDevil.renderArmoredTruckDevil(window.gRenderer, gArmoredTruckDevilOrig.texture, trucks[i].xPos - camera->x, trucks[i].yPos - camera->y);}}
                for(int i = 0; i < numberOfTrucks; i++) { gArmoredTruck[i].renderArmoredTruck(window.gRenderer, gArmoredTruckOrig.texture,trucks[i].xPos - camera->x, trucks[i].yPos - camera->y,  trucks[i].triggeredTimer.getTicks(), trucks[i].charging, trucks[i].reversing);}
                for(int i = 0; i < numberOfTrucks; i++) { gArmoredTruckCracks.renderArmoredTruckCracks(window.gRenderer, gArmoredTruckCracksOrig.texture,trucks[i].xPos - camera->x, trucks[i].yPos - camera->y,  trucks[i].health);}

                gRoboMaggot.render(window.gRenderer, maggot.xPos - camera->x, maggot.yPos - camera->y, NULL , 0, NULL, maggot.flip);

                ///render Sorse
                for(int i = 0; i < numberOfSorse; i++)
                {
                    gSorse[i].renderSorseBlackHole(window.gRenderer, gSorseOrig.texture, sorse[i].hitBoxBlackHole->x - camera->x, sorse[i].hitBoxBlackHole->y - camera->y, sorse[i].blackHoleFlying);
                    if(sorse[i].hitColor.isStarted()) {gSorseOrig.setColor(190, 0, 0);}
                    gSorse[i].renderSorse(window.gRenderer, gSorseOrig.texture, sorse[i].xPos- camera->x, sorse[i].yPos - camera->y, sorse[i].flip, sorse[i].summonAnimation.getTicks(), sorse[i].attackAnimation.getTicks(), sorse[i].alive, sorse[i].collapsing);
                    gSorseOrig.setColor(0xFF, 0xFF, 0xFF);
                    if(sorse[i].health == 1) {gSorseOrig.setColor(210, 110, 0);}
                    gSorse[i].renderSorseWeakpoint(window.gRenderer, gSorseOrig.texture, sorse[i].xPos- camera->x, sorse[i].yPos - camera->y, sorse[i].flip, sorse[i].alive);
                    gSorseOrig.setColor(0xFF, 0xFF, 0xFF);
               }

				if(!player1.hasGun)
                {
                    gSemi_Auto.render(window.gRenderer, semi_auto.xPos - camera->x, semi_auto.yPos - camera->y);
                }

				for(int i = 0; i < 19; i++)
                {
                    if(player1bullets[i].active)
                    {
                        gBullet.render(window.gRenderer, player1bullets[i].xPos - camera->x, player1bullets[i].yPos - camera->y);
                    }
                }

                for(int i = 0; i < 19; i++)
                {
                    if(enemyBullets[i].active)
                    {
                        gBullet2.render(window.gRenderer, enemyBullets[i].xPos - camera->x, enemyBullets[i].yPos - camera->y);
                    }
                }

                for(int i = 0; i < 15; i++)
                {
                    gTargets.setAlpha(targetGame.targets[i].alpha);
                    gTargets.renderTargets(window.gRenderer, targetGame.targets[i].hitBox->x - camera->x, targetGame.targets[i].hitBox->y - camera->y, targetGame.targets[i].color, targetGame.targets[i].alive);
                }

                ///render snowman
                if(snowman1.noseActive || snowman1.exploding) { gSnowmanNose.renderSnowmanNose(window.gRenderer, snowman1.xPosNose - camera->x, snowman1.yPosNose - camera->y, gSnowman.animationBool2, snowman1.rotation, snowman1.time2explode, snowman1.inFlight, snowman1.exploding); }
                gSnowman.renderSnowman(window.gRenderer, snowman1.xPos - camera->x, snowman1.yPos - camera->y, snowman1.alive, snowman1.time2shoot, snowman1.inFlight);

                ///render Garbage Gaurdians
                for(int i = 0; i < numberOfGarbageGaurdians; i++)
                {
                    if(garbageGaurdians[i].hitColorBody.isStarted()) { gGarbageGaurdianOrig.setColor(255, 0, 0);}
                    gGarbageGaurdians[i].renderGarbageGaurdian(window.gRenderer, gGarbageGaurdianOrig.texture, garbageGaurdians[i].xPos- camera->x, garbageGaurdians[i].yPos - camera->y, garbageGaurdians[i].animation);
                    gGarbageGaurdianOrig.setColor(0xFF, 0xFF, 0xFF);
                    gGarbageGaurdians[i].renderGarbageGaurdianSword(window.gRenderer, gGarbageGaurdianOrig.texture, garbageGaurdians[i].swordHitbox->x - camera->x, garbageGaurdians[i].swordHitbox->y - camera->y, garbageGaurdians[i].swordFlying, garbageGaurdians[i].swordInGround);
                    gGarbageGaurdians[i].renderGarbageGaurdianShield(window.gRenderer, gGarbageGaurdianOrig.texture, garbageGaurdians[i].shieldHitbox->x - camera->x, garbageGaurdians[i].shieldHitbox->y - camera->y, garbageGaurdians[i].shieldFlying);
                }

                ///render player before walls
                if(player1.damageColor.isStarted()) {gPlayerArmed.setColor(255, 0, 0); gPlayerSemi.setColor(255, 0, 0);}
                if(!player1.fallen || player1.rising)
                {
                    if(player1.secretUnlocked)
                    {
                        gPlayerPixelated.renderPlayer(player1.getAnimation(), window.gRenderer, (int)player1.xPos - camera->x, (int)player1.yPos - camera->y, player1.flip, player1.gunBounceTimer, player1.gunBounceDirection);
                    }
                    else if(player1.hasGun)
                    {
                        if(player1.animation == 6 && gPlayerArmed.animationTimer4.getTicks() / 95 > 18) { player1.succingOxygen = false; player1.controllable = true; gPlayerFace.animationBool = true;}
                        gPlayerArmed.renderPlayer(player1.getAnimation(), window.gRenderer, (int)player1.xPos - camera->x, (int)player1.yPos - camera->y, player1.flip, player1.gunBounceTimer, player1.gunBounceDirection);
                        if(player1.getAnimation() != 6) { gPlayerSemi.renderSemi(window.gRenderer, player1.xPos - 0 - camera->x, player1.yPos + 72 - camera->y, player1.flip, player1.gunTimer.getTicks(), player1.reloadTimer.getTicks() - 400, gPlayerArmed.gunOffset, player1.gunDirection, player1.animation, player1.holdingUp); } ///doesn't render the held weapon during certain animations

                                                                                                                                                                                             //WHAT???
                        gPlayerFace.renderFace(window.gRenderer, player1.xPos - camera->x, player1.yPos - camera->y, player1.flip, player1.succingOxygen, gPlayerArmed.gunOffset, player1.getAnimation(), gPlayerArmed.gunOffset2);
                    }

                    else
                    {
                        gPlayer.renderPlayer(player1.getAnimation(), window.gRenderer, (int)player1.xPos - camera->x, (int)player1.yPos - camera->y, player1.flip, player1.gunBounceTimer, player1.gunBounceDirection);

                    }
                }

                ///render robot body
                for(int i = 0; i < numberOfRobots; i++)
                {
                   if(robots[i].hitColorBody.isStarted()) {gRobotOrig.setColor(255, 0, 0);}
                    gRobot[i].renderRobot(window.gRenderer, gRobotOrig.texture, robots[i].xPos - camera->x, robots[i].yPos - camera->y, 1, robots[i].alive, 1);
                    gRobotOrig.setColor(0xFF, 0xFF, 0xFF);
                }
                ///render robot left arm
                for(int i = 0; i < numberOfRobots; i++)
                {
                    if(robots[i].hitColorLeft.isStarted()) { gRobotOrig.setColor(0, 100, 255);}
                    if(robots[i].leftActive)
                    {
                        gRobotArmLeft[i].renderRobot(window.gRenderer, gRobotOrig.texture, robots[i].xPos - camera->x, robots[i].yPos - camera->y, robots[i].leftActive, robots[i].alive, 2);
                    }
                    else
                    {
                        gRobotArmLeft[i].renderRobot(window.gRenderer, gRobotOrig.texture, robots[i].xPosLeft - camera->x, robots[i].yPosLeft - camera->y, robots[i].leftActive, 1, 2);
                    }
                    gRobotOrig.setColor(0xFF, 0xFF, 0xFF);
                }
				///render robot right arm
                for(int i = 0; i < numberOfRobots; i++)
                {
                    if(robots[i].hitColorRight.isStarted()) {gRobotOrig.setColor(0, 100, 255);};
                    if(robots[i].rightActive)
                    {
                        gRobotArmRight[i].renderRobot(window.gRenderer, gRobotOrig.texture, robots[i].xPos - camera->x, robots[i].yPos - camera->y, robots[i].rightActive, robots[i].alive, 3);
                    }
                    else
                    {
                        gRobotArmRight[i].renderRobot(window.gRenderer, gRobotOrig.texture, robots[i].xPosRight - camera->x, robots[i].yPosRight - camera->y, robots[i].rightActive, 1, 3);
                    }
                    gRobotOrig.setColor(0xFF, 0xFF, 0xFF);
                }

                gTutorial.render(window.gRenderer, -4320 - camera->x, -1052 - camera->y );
                gTutorial2.render(window.gRenderer, 680 - camera->x, -1052 - camera->y );
                gHubWorld.render(window.gRenderer, -3980 - camera->x, -2513 - camera->y);
                gLedge.render(window.gRenderer, ledges[0].xPos - camera->x, ledges[0].yPos - camera->y, NULL, 0, 0, ledges[0].flip);
                gLedge.render(window.gRenderer, ledges[1].xPos - camera->x, ledges[1].yPos - camera->y, NULL, 0, 0, ledges[1].flip);
				gGround.render(window.gRenderer, ground.xPos - camera->x, ground.yPos - camera->y );
				gWall.render(window.gRenderer, leftWall.xPos - camera->x, leftWall.yPos - camera->y);
				gTarget.render(window.gRenderer, target.xPos - camera->x, target.yPos - camera->y);
                gZiplineBase.render(window.gRenderer, 1372 - camera->x, 196 - camera->y);
				gHair.renderHair(window.gRenderer, player1.rectangleForTheHair->x - 28 - camera->x, player1.rectangleForTheHair->y - 10 - camera->y, player1.flip);
				gZipline.renderZipline(window.gRenderer, zipline1.xPos - camera->x, zipline1.yPos - camera->y, zipline1.docked, zipline1.carrying, zipline1.powerTimer.isStarted());
				if(!player1.granadoExploding) { gGranado.render(window.gRenderer, player1.granadoXpos - camera->x, player1.granadoYpos - camera->y, NULL, player1.granadoSpin, 0, ledges[0].flip); }
				else { gExplosionGeneric.render(window.gRenderer, player1.granadoXpos - camera->x, player1.granadoYpos - camera->y);}


                gGround.render(window.gRenderer, ground2.xPos - camera->x, ground2.yPos - camera->y );
				gGround.render(window.gRenderer, ground3.xPos - camera->x, ground3.yPos - camera->y );

				///render SHARK ROBOTS
                for(int i = 0; i < numberOfSharkRobots; i++)
                {
                    if(sharkRobots[i].beingShotInTheFaceTimer.getTicks() < 100 && sharkRobots[i].beingShotInTheFaceTimer.getTicks() > 0) { gSharkRobotOrig.setColor(255, 0, 0);}
                   gSharkRobot[i].renderSharkRobots(window.gRenderer, gSharkRobotOrig.texture, sharkRobots[i].xPos - camera->x, sharkRobots[i].yPos - camera->y, sharkRobots[i].beingShotInTheFaceTimer.getTicks());
                   gSharkRobotOrig.setColor(0xFF, 0xFF, 0xFF);
                }

                ///render KULTENTAS
                for(int i = 0; i < numberOfKultentas; i++)
                {
                    if(kultentas[i].hitColor.isStarted()) { gKultentaOrig.setColor(255, 0, 0); }
                    for(int a = 0; a < 8; a++)
                    {
                        if(kultentas[i].bulletactive[a])
                        {
                            gBullet.render(window.gRenderer, kultentas[i].bullets[a]->x - camera->x, kultentas[i].bullets[a]->y - camera->y);
                        }
                    }
                    if(kultentas[i].storage1visibol) { gBullet.render(window.gRenderer, kultentas[i].bulletStorage1.x - camera->x, kultentas[i].bulletStorage1.y - camera->y); }
                    if(kultentas[i].storage2visibol) { gBullet.render(window.gRenderer, kultentas[i].bulletStorage2.x - camera->x, kultentas[i].bulletStorage2.y - camera->y); }
                    if(kultentas[i].storage3visibol) { gBullet.render(window.gRenderer, kultentas[i].bulletStorage3.x - camera->x, kultentas[i].bulletStorage3.y - camera->y); }
                    if(kultentas[i].storage4visibol) { gBullet.render(window.gRenderer, kultentas[i].bulletStorage4.x - camera->x, kultentas[i].bulletStorage4.y - camera->y); }
                    if(kultentas[i].storage5visibol) { gBullet.render(window.gRenderer, kultentas[i].bulletStorage5.x - camera->x, kultentas[i].bulletStorage5.y - camera->y); }
                    gKultentaGun[i].renderKultentaEvilArm(window.gRenderer, gKultentaOrig.texture, kultentas[i].xPos - camera->x, kultentas[i].yPos - camera->y, kultentas[i].clip1, kultentas[i].dead ,0);
                    gKultentaEvilArm[i].renderKultentaEvilArm(window.gRenderer, gKultentaOrig.texture, kultentas[i].xPos - camera->x, kultentas[i].yPos - camera->y, kultentas[i].clip,kultentas[i].dead ,1);
                    gKultenta[i].renderKultentas(window.gRenderer, gKultentaOrig.texture, kultentas[i].xPos - camera->x, kultentas[i].yPos - camera->y, kultentas[i].dead, kultentas[i].beingDeadTimer.getTicks());
                    gKultentaOrig.setColor(0xFF, 0xFF, 0xFF);
                }

                ///render TNT GENTS
                for(int i = 0; i < numberOfTNTgent; i++)
                {
                    for(int a = 0; a < 5; a++)
                    {
                        if(gents[i].bulletactive[a])
                        {
                            gBullet.render(window.gRenderer, gents[i].bullets[a]->x - camera->x, gents[i].bullets[a]->y - camera->y);
                        }
                    }
                    if(gents[i].hitColorHead.isStarted()) {gTNTgentOrig.setColor(255, 0, 0);}
                    if(gents[i].health < 1 && gents[i].headThrown == 0) {gTNTgentOrig.setColor(15, 15, 15);}
                    gTNTgent[i].renderTNTgent(window.gRenderer, gTNTgentOrig.texture, gents[i].xPos - camera->x, gents[i].yPos - camera->y, gents[i].animation, gents[i].health);
                    gTNTgentOrig.setColor(0xFF, 0xFF, 0xFF);
                    if(gents[i].health > 0) { gTNTgent[i].renderTNTgentHead(window.gRenderer, gTNTgentOrig.texture, gents[i].xPos - camera->x, gents[i].yPos - camera->y, gents[i].animation, gents[i].speakingFrame, gents[i].hitBoxHead->x - camera->x, gents[i].hitBoxHead->y - camera->y);}
                    if(gents[i].explosionTimer.isStarted()) { gTNTgent[i].renderTNTgentExplosion(window.gRenderer, gTNTgentOrig.texture, gents[i].explosionHitbox->x - camera->x, gents[i].explosionHitbox->y - camera->y);}
                }

                ///render Wermbls
                for(int i = 0; i < numberOfWermbls; i++)
                {
                    for(int a = 0; a < 3; a++)
                    {
                        if(wermbls[i].bulletactive[a])
                        {
                            gBullet.render(window.gRenderer, wermbls[i].bullets[a]->x - camera->x, wermbls[i].bullets[a]->y - camera->y);
                        }
                    }

                    if (wermbls[i].gachaActivationTimer.isStarted()) {gWermblsOrig.setColor(0, wermbls[i].activatingColor, 0);}
                    gWermbls[i].renderWermbl(window.gRenderer, gWermblsOrig.texture, wermbls[i].xPos- camera->x, wermbls[i].yPos - camera->y, wermbls[i].gachAvtivationState, wermbls[i].flyingSpeed, wermbls[i].flip);
                    gWermblsOrig.setColor(0xFF, 0xFF, 0xFF);

                    if(wermbls[i].gachAvtivationState != 10){
                    gWermbls[i].renderWermblFace(window.gRenderer, gWermblsOrig.texture, wermbls[i].xPosFace- camera->x, wermbls[i].yPosFace - camera->y, wermbls[i].gachAvtivationState, wermbls[i].flip);
                    if(wermbls[i].numberOfBulletsEaten == 1) {gWermblsOrig.setColor(0, 250, 0);}
                    gWermbls[i].remderWermblGacha(window.gRenderer, gWermblsOrig.texture, wermbls[i].gacha1x- camera->x, wermbls[i].gacha1y - camera->y, wermbls[i].gacha1);
                    gWermblsOrig.setColor(0xFF, 0xFF, 0xFF);
                    if(wermbls[i].numberOfBulletsEaten == 2) {gWermblsOrig.setColor(0, 250, 0);}
                    gWermbls[i].remderWermblGacha(window.gRenderer, gWermblsOrig.texture, wermbls[i].gacha2x- camera->x, wermbls[i].gacha2y - camera->y, wermbls[i].gacha2);
                    gWermblsOrig.setColor(0xFF, 0xFF, 0xFF);
                    if(wermbls[i].numberOfBulletsEaten == 3) {gWermblsOrig.setColor(0, 250, 0);}
                    gWermbls[i].remderWermblGacha(window.gRenderer, gWermblsOrig.texture, wermbls[i].gacha3x- camera->x, wermbls[i].gacha3y - camera->y, wermbls[i].gacha3);
                    gWermblsOrig.setColor(0xFF, 0xFF, 0xFF);}
                }

                ///render Konga Lees
                for(int i = 0; i < numberOfKongaLees; i++)
                {
                    gKongaLees[i].renderKongaLee(window.gRenderer, gKongaLeeOrig.texture, kongaLees[i].xPos- camera->x, kongaLees[i].yPos - camera->y, kongaLees[i].angle, kongaLees[i].footSpeed, kongaLees[i].spitting, kongaLees[i].variant, kongaLees[i].exploded);
                    gKongaLees[i].renderKongaLeeExplosion(window.gRenderer, gKongaLeeOrig.texture, kongaLees[i].explosionHitbox->x - camera->x, kongaLees[i].explosionHitbox->y - camera->y, kongaLees[i].explosionTimer.isStarted() );
                    for(int a = 0; a < 3; a++)
                    {
                        if(kongaLees[i].bulletactive[a])
                        {
                            gBullet.render(window.gRenderer, kongaLees[i].bullets[a]->x - camera->x, kongaLees[i].bullets[a]->y - camera->y);
                        }
                    }
                }

                ///render player in front of ground
				if(player1.damageColor.isStarted()) {gPlayerArmed.setColor(255, 0, 0); printf("teeest"); gPlayerSemi.setColor(255, 0, 0);}
				if(player1.fallen || player1.rising)
                {
                    if(player1.secretUnlocked)
                    {
                        gPlayerPixelated.renderPlayer(player1.getAnimation(), window.gRenderer, (int)player1.xPos - camera->x, (int)player1.yPos - camera->y, player1.flip, player1.gunBounceTimer, player1.gunBounceDirection);
                    }
                    else if(player1.hasGun)
                    {
                        if(player1.animation == 6 && gPlayerArmed.animationTimer4.getTicks() / 95 > 18) { player1.succingOxygen = false; player1.controllable = true; gPlayerFace.animationBool = true;}
                        gPlayerArmed.renderPlayer(player1.getAnimation(), window.gRenderer, (int)player1.xPos - camera->x, (int)player1.yPos - camera->y, player1.flip, player1.gunBounceTimer, player1.gunBounceDirection);
                        if(player1.getAnimation() != 6) { gPlayerSemi.renderSemi(window.gRenderer, player1.xPos - 0 - camera->x, player1.yPos + 72 - camera->y, player1.flip, player1.gunTimer.getTicks(), player1.reloadTimer.getTicks() - 400, gPlayerArmed.gunOffset, player1.gunDirection, player1.animation, player1.holdingUp); } ///doesn't render the held weapon during certain animations

                                                                                                                                                                                             //WHAT???
                        gPlayerFace.renderFace(window.gRenderer, player1.xPos - camera->x, player1.yPos - camera->y, player1.flip, player1.succingOxygen, gPlayerArmed.gunOffset, player1.getAnimation(), gPlayerArmed.gunOffset2);
                    }

                    else
                    {
                        gPlayer.renderPlayer(player1.getAnimation(), window.gRenderer, (int)player1.xPos - camera->x, (int)player1.yPos - camera->y, player1.flip, player1.gunBounceTimer, player1.gunBounceDirection);

                    }
                }

                if(maggot.score == 40)
                {
                    timeText.str( "a" );
                    timeText << maggot.score << "/40";

                    if( !text.loadFromRenderedText( timeText.str().c_str(), textColor2, gFont, window.gRenderer ) )
                    {
                        printf( "Unable to render FPS texture!\n" );
                    }
                }
                else
                {
                    timeText.str( "a" );
                    timeText << maggot.score << "/40";

                    if( !text.loadFromRenderedText( timeText.str().c_str(), textColor, gFont, window.gRenderer ) )
                    {
                        printf( "Unable to render FPS texture!\n" );
                    }
                }

                if(targetGame.score == 29)
                {
                    timeText.str( "a" );
                    timeText << targetGame.score << "/29";

                    if( !level2Score.loadFromRenderedText( timeText.str().c_str(), textColor2, gFont, window.gRenderer ) )
                    {
                        printf( "Unable to render FPS texture!\n" );
                    }
                }
                else
                {
                   timeText.str( "a" );
                    timeText << targetGame.score << "/29";

                    if( !level2Score.loadFromRenderedText( timeText.str().c_str(), textColor, gFont, window.gRenderer ) )
                    {
                        printf( "Unable to render FPS texture!\n" );
                    }
                }


                timeText.str( "a" );
                timeText << "HP: " << player1.health;

                if( !healthHUD.loadFromRenderedText( timeText.str().c_str(), textColor, gFont, window.gRenderer ) )
                {
                    printf( "Unable to render FPS texture!\n" );
                }

                text.render(window.gRenderer, 8165 - camera->x, -205 - camera->y );
                level2Score.render(window.gRenderer, 8165 - camera->x, -165 - camera->y );
                healthHUD.render(window.gRenderer, 100, 80);



				///RECTANGLE RENDERING
				SDL_SetRenderDrawColor( window.gRenderer, 0, 250, 0, 250 ); ///green?


				if(debug_bool_delete_this_later_idiot)
                {
                    player1.hitBox->x -= camera->x;
                    player1.hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, player1.hitBox );
                    player1.hitBox->x += camera->x;
                    player1.hitBox->y += camera->y;

                    tutorialSecret.hitBox->x -= camera->x;
                    tutorialSecret.hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, tutorialSecret.hitBox );
                    tutorialSecret.hitBox->x += camera->x;
                    tutorialSecret.hitBox->y += camera->y;

                    trucks[0].hitBoxGlass->x -= camera->x;
                    trucks[0].hitBoxGlass->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, trucks[0].hitBoxGlass );
                    trucks[0].hitBoxGlass->x += camera->x;
                    trucks[0].hitBoxGlass->y += camera->y;

                    maggot.hitBox->x -= camera->x;
                    maggot.hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, maggot.hitBox );
                    maggot.hitBox->x += camera->x;
                    maggot.hitBox->y += camera->y;

                    sharkRobots[0].hitBox->x -= camera->x;
                    sharkRobots[0].hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, sharkRobots[0].hitBox );
                    sharkRobots[0].hitBox->x += camera->x;
                    sharkRobots[0].hitBox->y += camera->y;

                    kultentas[0].hitBox->x -= camera->x;
                    kultentas[0].hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, kultentas[0].hitBox );
                    kultentas[0].hitBox->x += camera->x;
                    kultentas[0].hitBox->y += camera->y;

                    for(int i = 0; i < 15; i++)
                    {
                        if(targetGame.targets[i].alive)
                        {
                            if(targetGame.targets[i].color == 1)
                            {
                                SDL_SetRenderDrawColor( window.gRenderer, 250, 0, 0, 250 );
                            }
                            else if(targetGame.targets[i].color == 3)
                            {
                                SDL_SetRenderDrawColor( window.gRenderer, 0, 0, 250, 250 );
                            }

                            targetGame.targets[i].hitBox->x -= camera->x;
                            targetGame.targets[i].hitBox->y -= camera->y;
                            SDL_RenderDrawRect( window.gRenderer, targetGame.targets[i].hitBox );
                            targetGame.targets[i].hitBox->x += camera->x;
                            targetGame.targets[i].hitBox->y += camera->y;

                            SDL_SetRenderDrawColor( window.gRenderer, 0, 250, 0, 250 );
                        }
                    }
                }


               /* if(player1.hitBox->x + player1.hitBox->w / 2 > 340 && player1.velocity + player1.hitBox->w / 2 < 627)
                {
                    if(player1.yVelocity > 315 - (player1.hitBox->x / 8) )//&& player1.yVelocity < 345 - (player1.hitBox->x / 8))
                    {
                        player1.yVelocity = 320 - (player1.hitBox->x / 8); ///YVELOCITY SHOULD HAVE + hitbox HEIGHT
                        player1.foo = true;
                    }
                }*/

                /*if(player1.hitBox->x + player1.hitBox->w / 2 > 340 && player1.velocity + player1.hitBox->w / 2 < 627)
                {                                              ///y1 - p.hitbox->w / 2 - (where you're at - x1? / slope)
                    if(player1.hitBox->y + player1.hitBox->h > (397 - 15) - ((player1.hitBox->x - 340) / 8) + 1 )  ///this is complicated
                    {
                        player1.yVelocity = (397 - 15) - ((player1.hitBox->x - 340) / 8) - player1.hitBox->h + 11;
                        player1.foo = true;
                        if(player1.released) { player1.jumping = false;} //if i don't put it here then u still jump for a frame and the screen shakes :(
                    }
                }*/


///cockzz

                    level1Trigger.hitBox->x -= camera->x;
                    level1Trigger.hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, level1Trigger.hitBox );
                    level1Trigger.hitBox->x += camera->x;
                    level1Trigger.hitBox->y += camera->y;

                    level2Trigger.hitBox->x -= camera->x;
                    level2Trigger.hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, level2Trigger.hitBox );
                    level2Trigger.hitBox->x += camera->x;
                    level2Trigger.hitBox->y += camera->y;

                if(debug_bool_delete_this_later_idiot)
                {
                    oxygentank.hitBox->x -= camera->x;
                    oxygentank.hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, oxygentank.hitBox );
                    oxygentank.hitBox->x += camera->x;
                    oxygentank.hitBox->y += camera->y;

                    zipline1.hitBox->x -= camera->x;
                    zipline1.hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, zipline1.hitBox );
                    zipline1.hitBox->x += camera->x;
                    zipline1.hitBox->y += camera->y;


                    if(robots[0].rightActive && robots[0].alive)
                    {
                    robots[0].hitBoxRight->x -= camera->x;
                    robots[0].hitBoxRight->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, robots[0].hitBoxRight );
                    robots[0].hitBoxRight->x += camera->x;
                    robots[0].hitBoxRight->y += camera->y;
                   }

                   if(robots[0].leftActive && robots[0].alive)
                    {
                    robots[0].hitBoxLeft->x -= camera->x;
                    robots[0].hitBoxLeft->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, robots[0].hitBoxLeft );
                    robots[0].hitBoxLeft->x += camera->x;
                    robots[0].hitBoxLeft->y += camera->y;
                    }

                    if(robots[0].alive)
                    {
                    robots[0].hitBox->x -= camera->x;
                    robots[0].hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, robots[0].hitBox );
                    robots[0].hitBox->x += camera->x;
                    robots[0].hitBox->y += camera->y;
                    }


                    for(int i = 0; i < 19; i++)
                    {
                        if(player1bullets[i].active)
                        {
                            player1bullets[i].hitBox->x -= camera->x;
                            player1bullets[i].hitBox->y -= camera->y;
                            SDL_RenderDrawRect( window.gRenderer, player1bullets[i].hitBox );
                            player1bullets[i].hitBox->x += camera->x;
                            player1bullets[i].hitBox->y += camera->y;
                        }
                    }

                    for(int i = 0; i < NumberOfDoors; i++)
                    {
                        doors[i].hitBox->x -= camera->x;
                        doors[i].hitBox->y -= camera->y;
                        SDL_RenderDrawRect( window.gRenderer, doors[i].hitBox );
                        doors[i].hitBox->x += camera->x;
                        doors[i].hitBox->y += camera->y;
                    }

                    /*for(int i = 0; i < 19; i++)
                    {
                        if(enemyBullets[i].active)
                        {
                            enemyBullets[i].hitBox->x -= camera->x;
                            enemyBullets[i].hitBox->y -= camera->y;
                            SDL_RenderDrawRect( window.gRenderer, enemyBullets[i].hitBox );
                            enemyBullets[i].hitBox->x += camera->x;
                            enemyBullets[i].hitBox->y += camera->y;
                        }
                    }*/

                    for(int i = 0; i < 2; i++)
                    {
                        ledges[i].hitBox->x -= camera->x;
                        ledges[i].hitBox->y -= camera->y;
                        SDL_RenderDrawRect( window.gRenderer, ledges[i].hitBox );
                        ledges[i].hitBox->x += camera->x;
                        ledges[i].hitBox->y += camera->y;
                    }


                }

				SDL_SetRenderDrawColor( window.gRenderer, 250, 0, 60, 250 ); ///red i think

				if(debug_bool_delete_this_later_idiot)
                {
                    snowman1.hitBox->x -= camera->x;
                    snowman1.hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, snowman1.hitBox );
                    snowman1.hitBox->x += camera->x;
                    snowman1.hitBox->y += camera->y;

                    snowman1.hitBoxHead->x -= camera->x;
                    snowman1.hitBoxHead->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, snowman1.hitBoxHead );
                    snowman1.hitBoxHead->x += camera->x;
                    snowman1.hitBoxHead->y += camera->y;






                    trucks[0].hitBoxBottom->x -= camera->x;
                    trucks[0].hitBoxBottom->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, trucks[0].hitBoxBottom );
                    trucks[0].hitBoxBottom->x += camera->x;
                    trucks[0].hitBoxBottom->y += camera->y;

                    trucks[0].hitBoxTop->x -= camera->x;
                    trucks[0].hitBoxTop->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, trucks[0].hitBoxTop );
                    trucks[0].hitBoxTop->x += camera->x;
                    trucks[0].hitBoxTop->y += camera->y;

                    trucks[0].hitBoxTrigger->x -= camera->x;
                    trucks[0].hitBoxTrigger->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, trucks[0].hitBoxTrigger );
                    trucks[0].hitBoxTrigger->x += camera->x;
                    trucks[0].hitBoxTrigger->y += camera->y;

                    trucks[0].hitBoxTrigger2->x -= camera->x;
                    trucks[0].hitBoxTrigger2->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, trucks[0].hitBoxTrigger2 );
                    trucks[0].hitBoxTrigger2->x += camera->x;
                    trucks[0].hitBoxTrigger2->y += camera->y;
                }

                SDL_SetRenderDrawColor( window.gRenderer, 0, 0, 250, 250 ); /// blue i think

                target.hitBox->x -= camera->x;
                target.hitBox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, target.hitBox );
                target.hitBox->x += camera->x;
                target.hitBox->y += camera->y;

                if(debug_bool_delete_this_later_idiot)
                {
                    SDL_RenderDrawPoint(window.gRenderer, kultentas[0].gunPoint.x - camera->x, kultentas[0].gunPoint.y - camera->y);
                    SDL_RenderDrawPoint(window.gRenderer, kultentas[0].gunPoint.x - 1 - camera->x, kultentas[0].gunPoint.y - camera->y);
                    SDL_RenderDrawPoint(window.gRenderer, kultentas[0].gunPoint.x - 2 - camera->x, kultentas[0].gunPoint.y - camera->y);

                    SDL_RenderDrawPoint(window.gRenderer, kultentas[0].bulletStorage1.x - camera->x, kultentas[0].bulletStorage1.y - camera->y);
                    SDL_RenderDrawPoint(window.gRenderer, kultentas[0].bulletStorage2.x - camera->x, kultentas[0].bulletStorage2.y - camera->y);
                    SDL_RenderDrawPoint(window.gRenderer, kultentas[0].bulletStorage3.x - camera->x, kultentas[0].bulletStorage3.y - camera->y);
                    SDL_RenderDrawPoint(window.gRenderer, kultentas[0].bulletStorage4.x - camera->x, kultentas[0].bulletStorage4.y - camera->y);
                    SDL_RenderDrawPoint(window.gRenderer, kultentas[0].bulletStorage5.x - camera->x, kultentas[0].bulletStorage5.y - camera->y);

                    //SDL_RenderDrawPoint(window.gRenderer, sorse[0].blackHoleSuccer.x - camera->x, sorse[0].blackHoleSuccer.y - camera->y);

                    player1.granadoHitbox->x -= camera->x;
                    player1.granadoHitbox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, player1.granadoHitbox );
                    player1.granadoHitbox->x += camera->x;
                    player1.granadoHitbox->y += camera->y;


                    snowman1.hitBoxNose->x -= camera->x;
                    snowman1.hitBoxNose->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, snowman1.hitBoxNose );
                    snowman1.hitBoxNose->x += camera->x;
                    snowman1.hitBoxNose->y += camera->y;

				player1.rectangleForTheHair->x -= camera->x;
				player1.rectangleForTheHair->y -= camera->y;
				SDL_RenderDrawRect( window.gRenderer, player1.rectangleForTheHair );
                player1.rectangleForTheHair->x += camera->x;
				player1.rectangleForTheHair->y += camera->y;

				player1.muzzleHitbox->x -= camera->x;
				player1.muzzleHitbox->y -= camera->y;
				SDL_RenderDrawRect( window.gRenderer, player1.muzzleHitbox );
                player1.muzzleHitbox->x += camera->x;
				player1.muzzleHitbox->y += camera->y;

				for(int i = 0; i < NumberOfTestBoxes; i++)
                {
                    testBox[i].hitBox->x -= camera->x;
                    testBox[i].hitBox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, testBox[i].hitBox );
                    testBox[i].hitBox->x += camera->x;
                    testBox[i].hitBox->y += camera->y;
                }

                //SDL_RenderDrawRect( window.gRenderer, testBox[0].hitBox ); //wow this might be some of the oldest undisturbed code
                //SDL_RenderDrawRect( window.gRenderer, testBox[1].hitBox );
                //SDL_RenderDrawRect( window.gRenderer, testBox[2].hitBox );

                ground.hitBox->x -= camera->x;
                ground.hitBox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, ground.hitBox );
                ground.hitBox->x += camera->x;
                ground.hitBox->y += camera->y;

                bulletCageTrigger.hitBox->x -= camera->x;
                bulletCageTrigger.hitBox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, bulletCageTrigger.hitBox );
                bulletCageTrigger.hitBox->x += camera->x;
                bulletCageTrigger.hitBox->y += camera->y;

                if(player1.gunBouncing)
                {
                    if(player1.gunBounceTimer.getTicks() > 0 && player1.gunBounceTimer.getTicks() < 400)
                    {
                        player1.meleeHitbox->x -= camera->x;
                        player1.meleeHitbox->y -= camera->y;
                        SDL_RenderDrawRect( window.gRenderer, player1.meleeHitbox);
                        player1.meleeHitbox->x += camera->x;
                        player1.meleeHitbox->y += camera->y;
                    }
                }
                else
                {
                   if(gPlayerSemi.animationTimer2.getTicks() < 249 && gPlayerSemi.animationTimer2.getTicks() > 160 && !player1.holdingUp)
                    {
                        player1.meleeHitbox->x -= camera->x;
                        player1.meleeHitbox->y -= camera->y;
                        SDL_RenderDrawRect( window.gRenderer, player1.meleeHitbox);
                        player1.meleeHitbox->x += camera->x;
                        player1.meleeHitbox->y += camera->y;
                    }
                }

                gents[0].hitBox->x -= camera->x;
                gents[0].hitBox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, gents[0].hitBox );
                gents[0].hitBox->x += camera->x;
                gents[0].hitBox->y += camera->y;

                gents[0].hitBoxHead->x -= camera->x;
                gents[0].hitBoxHead->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, gents[0].hitBoxHead );
                gents[0].hitBoxHead->x += camera->x;
                gents[0].hitBoxHead->y += camera->y;

                gents[0].trigger->x -= camera->x;
                gents[0].trigger->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, gents[0].trigger );
                gents[0].trigger->x += camera->x;
                gents[0].trigger->y += camera->y;

                if(gents[0].explosionTimer.isStarted())
                {
                    gents[0].explosionHitbox->x -= camera->x;
                    gents[0].explosionHitbox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, gents[0].explosionHitbox );
                    gents[0].explosionHitbox->x += camera->x;
                    gents[0].explosionHitbox->y += camera->y;
                }

                wermbls[0].hitBox->x -= camera->x;
                wermbls[0].hitBox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, wermbls[0].hitBox );
                wermbls[0].hitBox->x += camera->x;
                wermbls[0].hitBox->y += camera->y;

                wermbls[0].hitBoxHead->x -= camera->x;
                wermbls[0].hitBoxHead->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, wermbls[0].hitBoxHead );
                wermbls[0].hitBoxHead->x += camera->x;
                wermbls[0].hitBoxHead->y += camera->y;

                if(wermbls[0].explosionTimer.isStarted())
                {
                    wermbls[0].explosionHitbox->x -= camera->x;
                    wermbls[0].explosionHitbox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, wermbls[0].explosionHitbox );
                    wermbls[0].explosionHitbox->x += camera->x;
                    wermbls[0].explosionHitbox->y += camera->y;
                }

                garbageGaurdians[0].hitBox->x -= camera->x;
                garbageGaurdians[0].hitBox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, garbageGaurdians[0].hitBox );
                garbageGaurdians[0].hitBox->x += camera->x;
                garbageGaurdians[0].hitBox->y += camera->y;

                garbageGaurdians[0].shieldHitbox->x -= camera->x;
                garbageGaurdians[0].shieldHitbox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, garbageGaurdians[0].shieldHitbox );
                garbageGaurdians[0].shieldHitbox->x += camera->x;
                garbageGaurdians[0].shieldHitbox->y += camera->y;

                garbageGaurdians[0].swordHitbox->x -= camera->x;
                garbageGaurdians[0].swordHitbox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, garbageGaurdians[0].swordHitbox );
                garbageGaurdians[0].swordHitbox->x += camera->x;
                garbageGaurdians[0].swordHitbox->y += camera->y;

                if(garbageGaurdians[0].state > 1)
                {
                    garbageGaurdians[0].hitBoxBody->x -= camera->x;
                    garbageGaurdians[0].hitBoxBody->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, garbageGaurdians[0].hitBoxBody );
                    garbageGaurdians[0].hitBoxBody->x += camera->x;
                    garbageGaurdians[0].hitBoxBody->y += camera->y;
                }

                if(garbageGaurdians[0].swordInGround)
                {
                    SDL_RenderDrawLine(window.gRenderer, garbageGaurdians[0].swordRamp.x1 - camera->x , garbageGaurdians[0].swordRamp.y1 - camera->y, garbageGaurdians[0].swordRamp.x2 - camera->x , garbageGaurdians[0].swordRamp.y2 - camera->y);
                }

                kongaLees[0].hitBoxBody->x -= camera->x;
                kongaLees[0].hitBoxBody->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, kongaLees[0].hitBoxBody );
                kongaLees[0].hitBoxBody->x += camera->x;
                kongaLees[0].hitBoxBody->y += camera->y;

                kongaLees[0].hitBoxFace->x -= camera->x;
                kongaLees[0].hitBoxFace->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, kongaLees[0].hitBoxFace );
                kongaLees[0].hitBoxFace->x += camera->x;
                kongaLees[0].hitBoxFace->y += camera->y;

                kongaLees[0].hitBoxFoot->x -= camera->x;
                kongaLees[0].hitBoxFoot->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, kongaLees[0].hitBoxFoot );
                kongaLees[0].hitBoxFoot->x += camera->x;
                kongaLees[0].hitBoxFoot->y += camera->y;

                if(kongaLees[0].explosionTimer.isStarted())
                {
                    kongaLees[0].explosionHitbox->x -= camera->x;
                    kongaLees[0].explosionHitbox->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, kongaLees[0].explosionHitbox );
                    kongaLees[0].explosionHitbox->x += camera->x;
                    kongaLees[0].explosionHitbox->y += camera->y;
                }


                /*for(int i = 0; i < 3; i++)
                {
                    kongaLees[i].bullets->x -= camera->x;
                    kongaLees[i].bullets->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, kongaLees[i].bullets[i] );
                    kongaLees[i].bullets->x += camera->x;
                    kongaLees[i].bullets->y += camera->y;
                }*/

                sorse[0].hitBoxBody->x -= camera->x;
                sorse[0].hitBoxBody->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, sorse[0].hitBoxBody );
                sorse[0].hitBoxBody->x += camera->x;
                sorse[0].hitBoxBody->y += camera->y;

                sorse[0].hitBoxBlackHole->x -= camera->x;
                sorse[0].hitBoxBlackHole->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, sorse[0].hitBoxBlackHole );
                sorse[0].hitBoxBlackHole->x += camera->x;
                sorse[0].hitBoxBlackHole->y += camera->y;

                sorse[0].hitBoxTrigger->x -= camera->x;
                sorse[0].hitBoxTrigger->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, sorse[0].hitBoxTrigger );
                sorse[0].hitBoxTrigger->x += camera->x;
                sorse[0].hitBoxTrigger->y += camera->y;

                sorse[0].hitBoxFeet->x -= camera->x;
                sorse[0].hitBoxFeet->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, sorse[0].hitBoxFeet );
                sorse[0].hitBoxFeet->x += camera->x;
                sorse[0].hitBoxFeet->y += camera->y;

                sorse[0].hitBoxEyes->x -= camera->x;
                sorse[0].hitBoxEyes->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, sorse[0].hitBoxEyes );
                sorse[0].hitBoxEyes->x += camera->x;
                sorse[0].hitBoxEyes->y += camera->y;

                if(sorse[0].exploding)
                {
                    sorse[0].hitBoxExplosion->x -= camera->x;
                    sorse[0].hitBoxExplosion->y -= camera->y;
                    SDL_RenderDrawRect( window.gRenderer, sorse[0].hitBoxExplosion );
                    sorse[0].hitBoxExplosion->x += camera->x;
                    sorse[0].hitBoxExplosion->y += camera->y;
                }

                SDL_RenderDrawPoint(window.gRenderer, gents[0].gunPoint.x - camera->x, gents[0].gunPoint.y - camera->y);

                }
                for(int i = 0; i < NumberOfTestSlopes; i++)
                {
                    SDL_RenderDrawLine(window.gRenderer, testSlopes[i].x1 - camera->x , testSlopes[i].y1 - camera->y, testSlopes[i].x2 - camera->x , testSlopes[i].y2 - camera->y);
                }

                ///my retarded ammo think is actually here
                ammo1.renderAmmo(player1.clip, window.gRenderer, window.getWidth() - 120, window.getHeight() - ammo1.getHeight());
				ammo2.render(window.gRenderer, window.getWidth() - ammo2.getWidth(), window.getHeight() - ammo2.getHeight());

				hud_head.render(window.gRenderer, 40, 695);
				hud_baseplate.render(window.gRenderer, 121, 742);
				hud_ammo.renderAmmo2(player1.clip, window.gRenderer, 122, 743);

				if(loadBGalpha > 1 )
                {
                    if(loadBGalpha > 250)
                    {
                        loadBGalpha = 250;
                        gLoadingBG.render(window.gRenderer, 0, 0);
                        switch(destination)
                        {
                            case 0:
                            break;
                            case 1: area_debug = false;//level 1
                                    area_loading = true;
                                    needToLoadShit = true;
                                    needToSpawnShit = true;
                                    loadTimer = 200;
                            break;
                            case 2:
                            break;
                        }
                    }
                    else
                    {
                        loadBGalpha+=7;
                        gLoadingBG.setAlpha(loadBGalpha);
                        gLoadingBG.render(window.gRenderer, 0, 0);
                    }
                }

                }
                //else ///while game is paused
                if(player1.paused)
                {
                    gPauseMenu.render(window.gRenderer, player1.xPos - camera->x - 150, player1.yPos - camera->y - 300);
                    gDebugMenu.renderMenu(window.gRenderer, menu.hitBoxes[0]->x, menu.hitBoxes[0]->y - 18, menu.menuPage);
                    gDebugSpawner.render(window.gRenderer, menu.DraggableBox->x, menu.DraggableBox->y);



                    SDL_SetRenderDrawColor( window.gRenderer, 0, 250, 50, 250 ); /// green i think


                    SDL_GetMouseState(&mouse.x, &mouse.y);
                    mouseHitbox->x = mouse.x - 5;
                    mouseHitbox->y = mouse.y - 5;

                    ///pause collision

                    for(int i = 0; i < menu.numberOfHitboxes; i++) ///collision for DEBUG MENU HITBOXES
                    {
                        if(checkCollision(menu.hitBoxes[i], mouseHitbox, 1))
                        {
                            switch(menu.menuPage)
                            {
                                case 0: ///page 0
                                        switch(i)
                                        {
                                            case 0:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[0] );
                                                if(player1.unclicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    gents[0].reset();
                                                    gTNTgent[0].animationTimer1.start();
                                                    gTNTgent[0].animationTimer2.start();
                                                    gTNTgent[0].animationTimer3.start();
                                                    gTNTgent[0].animationTimer7.start();
                                                    gTNTgent[0].robotAnimation = 0;
                                                    gTNTgent[0].animationBool = 0;
                                                    gTNTgent[0].animationInt = 0;
                                                }
                                            }break;
                                            case 1:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[1] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    printf("TNT gent spawned\n");
                                                    gents[0].hitBox->x =  camera->x + menu.DraggableBox->x + 40;
                                                    gents[0].hitBox->y =   camera->y + menu.DraggableBox->y + 40;
                                                }
                                            }break;
                                            case 2:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[2] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    kultentas[0].reset();
                                                }
                                            }break;
                                            case 3:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[3] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    printf("Kultenta spawned\n");
                                                    kultentas[0].hitBox->x = camera->x + menu.DraggableBox->x + 40;
                                                    kultentas[0].hitBox->y = camera->y + menu.DraggableBox->y + 40;
                                                }
                                            }break;
                                            case 4:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[4] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    robots[0].reset();
                                                    gRobot[0].animationTimer1.start();
                                                    gRobot[0].animationTimer2.stop();
                                                    gRobot[0].robotAnimation = 0;
                                                    gRobotArmLeft[0].animationTimer1.start();
                                                    gRobotArmRight[0].animationTimer1.start();
                                                }
                                            }break;
                                            case 5:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[5] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    printf("Row Bit spawned\n");
                                                    robots[0].hitBox->x = camera->x + menu.DraggableBox->x + 40;
                                                    robots[0].hitBox->y = camera->y + menu.DraggableBox->y + 40;
                                                    robots[0].home = camera->x + menu.DraggableBox->x + 40;
                                                }
                                            }break;
                                            case 6:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[6] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    sharkRobots[0].reset();
                                                }
                                            }break;
                                            case 7:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[7] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    printf("Shark Robot spawned\n");
                                                    sharkRobots[0].hitBox->x = camera->x + menu.DraggableBox->x + 40;
                                                    sharkRobots[0].hitBox->y = camera->y + menu.DraggableBox->y + 40;
                                                }
                                            }break;
                                            case 8:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[8] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    trucks[0].reset();
                                                }
                                            }break;
                                            case 9:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[9] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    printf("Truck spawned\n");
                                                    trucks[0].xVelocity = camera->x + menu.DraggableBox->x + 40;
                                                    trucks[0].yVelocity = camera->y + menu.DraggableBox->y + 40;
                                                    trucks[0].hitBoxTrigger2->x = trucks[0].xVelocity - 440;
                                                    trucks[0].hitBoxTrigger2->y = trucks[0].yVelocity - 50;
                                                    trucks[0].home = camera->x + menu.DraggableBox->x + 40;
                                                }
                                            }break;
                                            case 10:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[10] );
                                                if(player1.clicky)
                                                {
                                                    menu.menuPage--;
                                                    Mix_PlayChannel( -1, aSelect, 0 );

                                                }
                                            }break;
                                            case 11:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[11] );
                                                if(player1.clicky)
                                                {
                                                    menu.menuPage++;
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;

                                        }
                                        break;
                                    case 1: ///page 1
                                        switch(i)
                                        {
                                            case 0:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[0] );
                                                if(player1.unclicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    if(debug_bool_delete_this_later_idiot)
                                                    {
                                                        debug_bool_delete_this_later_idiot = false;
                                                        printf("Hitboxes turned off\n");
                                                    }
                                                    else
                                                    {
                                                        printf("Hitboxes turned on\n");
                                                        debug_bool_delete_this_later_idiot = true;
                                                    }
                                                }
                                            }break;
                                            case 1:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[1] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 2:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[2] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 3:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[3] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    player1.velocity = camera->x + menu.DraggableBox->x + 40;
                                                    player1.yVelocity = camera->y + menu.DraggableBox->y + 40;
                                                }
                                            }break;
                                            case 4:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[4] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    wermbls[0].reset();
                                                    gWermbls[0].animationTimer1.start();
                                                    gWermbls[0].animationTimer2.start();
                                                    gWermbls[0].animationTimer4.stop();
                                                    gWermbls[0].animationTimer5.stop();
                                                }
                                            }break;
                                            case 5:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[5] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    printf("Wermbl spawned\n");
                                                    wermbls[0].hitBox->x = camera->x + menu.DraggableBox->x + 40;
                                                    wermbls[0].hitBox->y = camera->y + menu.DraggableBox->y + 40;
                                                }
                                            }break;
                                            case 6:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[6] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );

                                                    garbageGaurdians[0].reset();
                                                }
                                            }break;
                                            case 7:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[7] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    printf("Trash Tutelar spawned\n");
                                                    garbageGaurdians[0].hitBox->x = camera->x + menu.DraggableBox->x + 40;
                                                    garbageGaurdians[0].hitBox->y = camera->y + menu.DraggableBox->y + 40;
                                                }
                                            }break;
                                            case 8:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[8] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 9:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[9] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    printf("Konga Lee spawned\n");
                                                    kongaLees[0].hitBoxBody->x = camera->x + menu.DraggableBox->x + 40;
                                                    kongaLees[0].hitBoxBody->y = camera->y + menu.DraggableBox->y + 40;
                                                    kongaLees[0].reset();
                                                }
                                            }break;
                                            case 10:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[10] );
                                                if(player1.clicky)
                                                {
                                                    menu.menuPage--;
                                                    Mix_PlayChannel( -1, aSelect, 0 );

                                                }
                                            }break;
                                            case 11:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[11] );
                                                if(player1.clicky)
                                                {
                                                    menu.menuPage++;
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;

                                        }
                                    break;
                                    case 2: ///page 1
                                        switch(i)
                                        {
                                            case 0:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[0] );
                                                if(player1.unclicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    sorse[0].reset();
                                                }
                                            }break;
                                            case 1:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[1] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                    printf("Sorse spawned\n");
                                                    sorse[0].hitBoxBody->x = camera->x + menu.DraggableBox->x + 40;
                                                    sorse[0].hitBoxBody->y = camera->y + menu.DraggableBox->y + 40;
                                                }
                                            }break;
                                            case 2:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[2] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 3:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[3] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 4:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[4] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 5:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[5] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 6:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[6] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 7:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[7] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 8:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[8] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 9:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[9] );
                                                if(player1.clicky)
                                                {
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;
                                            case 10:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[10] );
                                                if(player1.clicky)
                                                {
                                                    menu.menuPage--;
                                                    Mix_PlayChannel( -1, aSelect, 0 );

                                                }
                                            }break;
                                            case 11:
                                            {
                                                SDL_RenderDrawRect( window.gRenderer, menu.hitBoxes[11] );
                                                if(player1.clicky)
                                                {
                                                    menu.menuPage++;
                                                    Mix_PlayChannel( -1, aSelect, 0 );
                                                }
                                            }break;

                                        }
                                        break;
                            }
                        }
                    }

                    ///collision for DRAGGABLE SPAWNER
                    if(!menu.dragging)
                    {
                        if(checkCollision(menu.DraggableBox, mouseHitbox, 1))
                        {
                            SDL_RenderDrawRect( window.gRenderer, menu.DraggableBox );
                            if(player1.clicking)
                            {
                                menu.dragging = true;
                            }
                        }
                    }
                    else
                    {
                        if(!player1.clicking)
                        {
                            menu.dragging = false;
                        }
                    }

                    if(menu.dragging)
                    {
                        menu.DraggableBox->x = mouseHitbox->x - 40;
                        menu.DraggableBox->y = mouseHitbox->y - 20;
                    }

                    ///END OF PAUSE COLLISION

                    menu.update();

                    //SDL_RenderDrawRect( window.gRenderer, mouseHitbox );
                }



                Secrettarget.hitBox->x -= camera->x;
                Secrettarget.hitBox->y -= camera->y;
                SDL_RenderDrawRect( window.gRenderer, Secrettarget.hitBox );
                Secrettarget.hitBox->x += camera->x;
                Secrettarget.hitBox->y += camera->y;





                SDL_RenderDrawLine(window.gRenderer, 4626 - camera->x , 767 - camera->y, 4913 - camera->x , 732 - camera->y);


				//Update screen
				SDL_RenderPresent( window.gRenderer );
				/******** rendering ********/

				++countedFrames;

				//If frame finished early
				int frameTicks = capTimer.getTicks();
				if( frameTicks < SCREEN_TICK_PER_FRAME )
				{
					//Wait remaining time
					SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
				}

				if(quit || !area_1)
                {
                    gPlayer.free();
                    gPlayerPixelated.free();
                    gPlayerArmed.free();
                    gPlayerSemi.free();
                    gPlayerFace.free();
                    gGround.free();
                    gWall.free();
                    gTestBox.free();
                    gGroundBox.free();
                    gGroundBox2.free();
                    gBackground.free();
                    gTarget.free();
                    gBullet.free();
                    gHair.free();
                    gSemi_Auto.free();
                    ammo1.free();
                    ammo2.free();
                    hud_head.free();
                    hud_ammo.free();
                    hud_baseplate.free();
                    gZipline.free();
                    gZiplineBase.free();
                    gRobotOrig.free();
                    gRobotArmLeftOrig.free();
                    gRobotArmRightOrig.free();
                    gSnowman.free();
                    gSnowmanNose.free();
                    gOxygenTank.free();
                    gDemoDay.free();
                    gTutorial.free();
                    gTutorial2.free();
                    gTutorialbg.free();
                    gTutorialbg2.free();
                    gTutorial3.free();
                    gKongaCage.free();
                    gDarunggarBox.free();
                    gCeeButton.free();
                    gLedge.free();
                    gGranado.free();
                    gExplosionGeneric.free();
                    gArmoredTruckOrig.free();
                    gArmoredTruckCracksOrig.free();
                    gArmoredTruckDevilOrig.free();
                    gPauseMenu.free();
                    gDebugMenu.free();
                    gDebugSpawner.free();
                    gSpinner.free();
                    gBulletCavern.free();
                    gBulletCavernCage.free();
                    gBulletCavern2.free();
                    gNewMove.free();
                    gBullet2.free();
                    gRoboMaggot.free();
                    gTargets.free();
                    gSharkRobotOrig.free();
                    gKultentaOrig.free();
                    gTNTgentOrig.free();
                    gWermblsOrig.free();
                    gGarbageGaurdianOrig.free();
                    gKongaLeeOrig.free();
                    gLeverOff.free();
                    gLeverOn.free();
                    gTutorialZipline1.free();
                    gTutorialEnemy.free();
                    gHubWorld.free();
                    gHubWorld_BG.free();

                    text.free();
                    healthHUD.free();
                    level2Score.free();

                    //Free the music
                    Mix_FreeMusic( gMusic );
                    gMusic = NULL;

                    Mix_FreeChunk( aExplosion );
                    aExplosion = NULL;
                    Mix_FreeChunk( aSpeedBuff );
                    aSpeedBuff = NULL;
                    Mix_FreeChunk( aTvSize );
                    aTvSize = NULL;
                    Mix_FreeChunk( aledgeGrab );
                    aledgeGrab = NULL;
                    Mix_FreeChunk( aReload );
                    aReload = NULL;
                    Mix_FreeChunk( aShootSemi );
                    aShootSemi = NULL;
                    Mix_FreeChunk( aShootEmpty );
                    aShootEmpty = NULL;
                    Mix_FreeChunk( aGranadoPrimed );
                    aGranadoPrimed = NULL;
                    Mix_FreeChunk( aExplosion1 );
                    aExplosion1 = NULL;
                    Mix_FreeChunk( aCrack1 );
                    aCrack1 = NULL;
                    Mix_FreeChunk( aCrack2 );
                    aCrack2 = NULL;
                    Mix_FreeChunk( aCrack3 );
                    aCrack3 = NULL;
                    Mix_FreeChunk( aMissile );
                    aMissile = NULL;
                    Mix_FreeChunk( aRising );
                    aRising = NULL;
                    Mix_FreeChunk( aLand );
                    aLand = NULL;
                    Mix_FreeChunk( aShootMaggot );
                    aShootMaggot = NULL;
                    Mix_FreeChunk( aTeleMaggotLeft );
                    aTeleMaggotLeft = NULL;
                    Mix_FreeChunk( aTeleMaggotRight );
                    aTeleMaggotRight = NULL;
                    Mix_FreeChunk( aBreak1 );
                    aBreak1 = NULL;
                    Mix_FreeChunk( aBreak2 );
                    aBreak2 = NULL;
                    Mix_FreeChunk( aBreak3 );
                    aBreak3 = NULL;
                    Mix_FreeChunk( aSelect );
                    aSelect = NULL;
                    Mix_FreeChunk( aJellyDeath1 );
                    aJellyDeath1 = NULL;
                    Mix_FreeChunk( aJellyDeath2 );
                    aJellyDeath2 = NULL;
                    Mix_FreeChunk( aTNTgent_alert );
                    aTNTgent_alert = NULL;
                    Mix_FreeChunk( aTNTgent_toss );
                    aTNTgent_toss = NULL;
                    Mix_FreeChunk( aWermblONE );
                    aWermblONE = NULL;
                    Mix_FreeChunk( aWermblTWO );
                    aWermblTWO = NULL;
                    Mix_FreeChunk( aWermblTHREE );
                    aWermblTHREE = NULL;

                    //TTF_CloseFont( gFont );
                    gFont = NULL;
                }
            }
            //}
            }///end of area 1 loop
        }
        }
	}

	//Free resources and close SDL
	camera = NULL;

printf("Fatal error: [File:Unknown] [Line: 826]");
printf("Rendering thread exception:");
printf("Fatal error: [File:Unknown] [Line: 111]");
printf("GPU has crashed");
printf("0x000007fefd4da06d KERNELBASE.dll!UnknownFunction []");
printf("0x00000001408645f9 HairBulletClient-Win64-Shipping.exe!UnknownFunction []");
printf("0x00000001407faf5b HairBulletClient-Win64-Shipping.exe!UnknownFunction []");
printf("0x00000001407c6858 HairBulletClient-Win64-Shipping.exe!UnknownFunction []");
printf("0x0000000140c4e885 HairBulletClient-Win64-Shipping.exe!UnknownFunction []");
printf("0x0000000140c5a5c8 HairBulletClient-Win64-Shipping.exe!UnknownFunction []");
printf("0x0000000140f61aff HairBulletClient-Win64-Shipping.exe!UnknownFunction []");
printf("0x0000000140f76fa8 HairBulletClient-Win64-Shipping.exe!UnknownFunction []");
printf("0x0000000140f7b3ab HairBulletClient-Win64-Shipping.exe!UnknownFunction []");
printf("0x0000000140f70fb5 HairBulletClient-Win64-Shipping.exe!UnknownFunction []");
printf("0x0000000140f79927 HairBulletClient-Win64-Shipping.exe!UnknownFunction []");
printf("0x0000000140d1cced HairBulletClient-Win64-Shipping.exe!UnknownFunction []");
printf("0x0000000140f8268e HairBulletClient-Win64-Shipping.exe!UnknownFunction []");
printf("0x0000000140f75ac8 HairBulletClient-Win64-Shipping.exe!UnknownFunction []");
printf("0x000000014073dec1 HairBulletClient-Win64-Shipping.exe!UnknownFunction []");
printf("0x00000001410ca0d2 HairBulletClient-Win64-Shipping.exe!UnknownFunction []");
printf("0x00000001410ca158 HairBulletClient-Win64-Shipping.exe!UnknownFunction []");
printf("0x000000014083b7b7 HairBulletClient-Win64-Shipping.exe!UnknownFunction []");
printf("0x0000000140838bcc HairBulletClient-Win64-Shipping.exe!UnknownFunction []");
printf("0x00000000775359cd kernel32.dll!UnknownFunction []");
printf("0x000000007766a561 ntdll.dll!UnknownFunction []*");
	return 0;
}
