#ifndef MENUS_H_INCLUDED
#define MENUS_H_INCLUDED

#include <SDL.h>
#include "Menus.h"

#include <stdio.h>

class Menus
{
    public:
		Menus(int cameraW, int cameraH);
		~Menus();

		void update();

		int numberOfHitboxes;
        SDL_Rect* hitBoxes[12];
        bool hitBoxActivated[12];

        SDL_Rect* DraggableBox;
        bool dragging;

        int cameraWidth;
        int cameraHeight;

        int menuPage;
};

#endif // MENUS_H_INCLUDED
