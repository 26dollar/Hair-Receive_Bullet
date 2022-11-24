#include "Menus.h"

Menus::Menus(int cameraW, int cameraH)
{
    numberOfHitboxes = 12;

    for(int i = 0; i < numberOfHitboxes; i++)
    {
        hitBoxes[i] = new SDL_Rect;
        hitBoxes[i]->w = 7;
        hitBoxes[i]->h = 7;
        hitBoxes[i]->x = -7;
        hitBoxes[i]->y = -7;
    }

    hitBoxes[0]->w = 75;
    hitBoxes[0]->h = 50;
    hitBoxes[0]->x = cameraW - 200;
    hitBoxes[0]->y = +50;

    hitBoxes[1]->w = 75;
    hitBoxes[1]->h = 50;
    hitBoxes[1]->x = hitBoxes[0]->x + 101;
    hitBoxes[1]->y = hitBoxes[0]->y;

    hitBoxes[2]->w = 75;
    hitBoxes[2]->h = 50;
    hitBoxes[2]->x = hitBoxes[0]->x;
    hitBoxes[2]->y = hitBoxes[0]->y + 61;

    hitBoxes[3]->w = 75;
    hitBoxes[3]->h = 50;
    hitBoxes[3]->x = hitBoxes[1]->x;
    hitBoxes[3]->y = hitBoxes[2]->y;

    hitBoxes[4]->w = 75;
    hitBoxes[4]->h = 50;
    hitBoxes[4]->x = hitBoxes[0]->x;
    hitBoxes[4]->y = hitBoxes[2]->y + 61;

    hitBoxes[5]->w = 75;
    hitBoxes[5]->h = 50;
    hitBoxes[5]->x = hitBoxes[1]->x;
    hitBoxes[5]->y = hitBoxes[4]->y;

    hitBoxes[6]->w = 75;
    hitBoxes[6]->h = 50;
    hitBoxes[6]->x = hitBoxes[0]->x;
    hitBoxes[6]->y = hitBoxes[4]->y + 61;

    hitBoxes[7]->w = 75;
    hitBoxes[7]->h = 50;
    hitBoxes[7]->x = hitBoxes[1]->x;
    hitBoxes[7]->y = hitBoxes[6]->y;

    hitBoxes[8]->w = 75;
    hitBoxes[8]->h = 50;
    hitBoxes[8]->x = hitBoxes[0]->x;
    hitBoxes[8]->y = hitBoxes[6]->y + 61;

    hitBoxes[9]->w = 75;
    hitBoxes[9]->h = 50;
    hitBoxes[9]->x = hitBoxes[1]->x;
    hitBoxes[9]->y = hitBoxes[8]->y;

    hitBoxes[10]->w = 75;
    hitBoxes[10]->h = 28;
    hitBoxes[10]->x = hitBoxes[0]->x;
    hitBoxes[10]->y = hitBoxes[9]->y +51;

    hitBoxes[11]->w = 75;
    hitBoxes[11]->h = 28;
    hitBoxes[11]->x = hitBoxes[1]->x;
    hitBoxes[11]->y = hitBoxes[10]->y;

    for(int i = 0; i < numberOfHitboxes; i++)
    {
        hitBoxActivated[i] = false;
    }

    DraggableBox = new SDL_Rect;
    DraggableBox->w = 83;
    DraggableBox->h = 42;
    DraggableBox->x = 1200;
    DraggableBox->y = 400;

    dragging = false;

    cameraWidth = cameraW;
    cameraHeight = cameraH;
}

Menus::~Menus()
{
    for(int i = 0; i < 5; i++)
    {
        hitBoxes[i] = NULL;
    }
}

void Menus::update()
{
    if(menuPage > 2)
    {
        menuPage = 0;
    }

    if(menuPage < 0)
    {
        menuPage = 2;
    }
}
