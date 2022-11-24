#ifndef GRAPHICS_H_INCLUDED
#define GRAPHICS_H_INCLUDED

#include <SDL.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class Graphics
{
public:
    Graphics();
    ~Graphics();

    bool Initialize();

    SDL_Window* gWindow;
    SDL_Renderer* gRenderer;

    int getHeight();
    int getWidth();

    int SCREEN_HEIGHT;
    int SCREEN_WIDTH;


    bool success;
};
#endif // GRAPHICS_H_INCLUDED
