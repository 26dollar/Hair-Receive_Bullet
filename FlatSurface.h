#ifndef FLATSURFACE_H_INCLUDED
#define FLATSURFACE_H_INCLUDED

#include <SDL.h>

///2 positions is fine
class FlatSurface
{
public:
    FlatSurface( int x, int y, int width, int height, int xpos, int ypos );
    FlatSurface();
    ~FlatSurface();

    int xPos, yPos;

    bool activated;

    SDL_Rect* hitBox;
};

#endif // FLATSURFACE_H_INCLUDED
