#ifndef STD
#define STD

#include <iostream>
#endif

#ifndef SDL
#define SDL

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

SDL_Window* gWindow;
SDL_Renderer* gRenderer;
TTF_Font* gFont;


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

#endif
