#include "NonFlatSurface.h"

NonFlatSurface::NonFlatSurface(int x, int y, int length, int theslope, int xpos, int ypos)
{
    xPos = xpos;
    yPos = ypos;

    x1 = x;
    y1 = y;

    slope = theslope;

    x2 = x1 + (theslope * length);
    y2 = y1 - (length + 0);

    if(y1 > y2)
    {
        slopingUp = true;
    }
    else
    {
        slopingUp = false;
    }
}

NonFlatSurface::NonFlatSurface()
{
    xPos = -999;
    yPos = -999;

    x1 = -999;
    y1 = -999;

    slope = 0;

    x2 = -999;
    y2 = -999;

    slopingUp = true;
    slopingUp = false;
}

NonFlatSurface::~NonFlatSurface()
{
    //uh
}
