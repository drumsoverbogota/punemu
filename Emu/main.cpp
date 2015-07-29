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

SDL_Window* window = NULL;
SDL_Renderer* renderer;
TTF_Font* font;
SDL_Texture* SurfaceToTexture( SDL_Surface* surf );
SDL_Texture* solidTexture;

// Setup
void Render();
bool InitEverything();
bool InitSDL();
bool CreateWindow();
bool CreateRenderer();
void SetupRenderer();
void CreateTextTextures();

// Our new function for setting uo SDL_TTF
bool SetupTTF( const std::string &fontName );

int main(int argc, const char * argv[]) {
    
    std::string rom = argv[2];

    if(!InitEverything())
        return -1;
    
    emu c_emu(rom);
    c_emu.init(false);
            Render();
    for (;;) {

        if(!c_emu.emulateCycle()){
            
            std::cout<<"Fin de la emulación";
            break;
        }
    }
    return 0;
}

void Render()
{
    // Clear the window and make it all red
    SDL_RenderClear( renderer );
    
    SDL_Rect solidRect;
    solidRect.x = 0;
    solidRect.y = 0;
    
    // Render our text objects ( like normal )
    SDL_RenderCopy( renderer, solidTexture, nullptr, &solidRect );
    
    // Render the changes above
    SDL_RenderPresent( renderer);
}


bool InitEverything()
{
    if ( !InitSDL() )
        return false;
    
    if ( !CreateWindow() )
        return false;
    
    if ( !CreateRenderer() )
        return false;
    
    SetupRenderer();
    
    if ( !SetupTTF( "/Library/Fonts/Arial.ttf" ) )
        return false;
    
    CreateTextTextures();
    
    return true;
}

void CreateTextTextures(){
    SDL_Surface* solid = TTF_RenderText_Solid( font, "solid", {255,255,255,0} );
    solidTexture = SurfaceToTexture( solid );
}

SDL_Texture* SurfaceToTexture( SDL_Surface* surf )
{
    SDL_Texture* text;
    
    text = SDL_CreateTextureFromSurface( renderer, surf );
    
    SDL_FreeSurface( surf );
    
    return text;
}

bool InitSDL()
{
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        std::cout << " Failed to initialize SDL : " << SDL_GetError() << std::endl;
        return false;
    }
    
    return true;
}

bool SetupTTF( const std::string &fontName)
{
    // SDL2_TTF needs to be initialized just like SDL2
    if ( TTF_Init() == -1 )
    {
        std::cout << " Failed to initialize TTF : " << TTF_GetError() << std::endl;
        return false;
    }
    
    // Load our fonts, with a huge size
    font = TTF_OpenFont( fontName.c_str(), 90 );
    
    // Error check
    if ( font == nullptr )
    {
        std::cout << " Failed to load font : " << TTF_GetError() << std::endl;
        return false;
    }
    
    return true;
}

bool CreateWindow()
{
    window = SDL_CreateWindow( "Server",0,0, SCREEN_WIDTH, SCREEN_HEIGHT, 0 );
    
    if ( window == nullptr )
    {
        std::cout << "Failed to create window : " << SDL_GetError();
        return false;
    }
    
    return true;
}
bool CreateRenderer()
{
    renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
    
    if ( renderer == nullptr )
    {
        std::cout << "Failed to create renderer : " << SDL_GetError();
        return false;
    }
    
    return true;
}
void SetupRenderer()
{
    // Set size of renderer to the same as window
    SDL_RenderSetLogicalSize( renderer, SCREEN_WIDTH, SCREEN_HEIGHT );
    
    // Set color of renderer to red
    SDL_SetRenderDrawColor( renderer, 0, 0, 0, 255 );
}

