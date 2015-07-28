//
//  main.cpp
//  Emu
//
//  Created by Sergio Mancera on 30/06/15.
//  Copyright (c) 2015 Sergio Mancera. All rights reserved.
//
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

#include "emu.hpp"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


bool init(){
    bool exito = true;
    //The window we'll be rendering to
    SDL_Window* window = NULL;
    
    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;
    
    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        exito = false;
    }
    else{
        window = SDL_CreateWindow( "PunBoy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            exito = false;
        }
        else
        {
            //Get window surface
            screenSurface = SDL_GetWindowSurface( window );
    
            
            /*
             Dibuja un pixel, lo guardo pa' luego
             
             Uint32 colour;
            
            colour = SDL_MapRGB( screenSurface->format, 50, 50, 50 );
            
            Uint32 *pixmem32;
            
            pixmem32 = (Uint32*) screenSurface->pixels;
            
            int y= 50;
            int x= 50;
            pixmem32[(y*screenSurface->w)+x] = colour;
            */
            
        }
    }
    
    return exito;
}


int main(int argc, const char * argv[]) {
    
    std::string rom = argv[2];

    init();
    
    emu c_emu(rom);
    c_emu.init(false);
    for (;;) {
        if(!c_emu.emulateCycle()){
            std::cout<<"Fin de la emulación";
            break;
        }
    }
    return 0;
}
