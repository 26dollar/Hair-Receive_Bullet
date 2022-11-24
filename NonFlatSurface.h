#ifndef NONFLATSURFACE_H_INCLUDED
#define NONFLATSURFACE_H_INCLUDED

#include <SDL.h>

class NonFlatSurface
{
public:
    NonFlatSurface( int x, int y, int length, int theslope, int xpos, int ypos );
    NonFlatSurface();
    ~NonFlatSurface();

    int x1;
    int y1;

    int x2;
    int y2;

    int slope;

    int xPos;
    int yPos;

    bool slopingUp;
};

#endif // NONFLATSURFACE_H_INCLUDED
