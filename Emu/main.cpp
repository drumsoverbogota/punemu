//
//  main.cpp
//  Emu
//
//  Created by Sergio Mancera on 30/06/15.
//  Copyright (c) 2015 Sergio Mancera. All rights reserved.
//
#ifndef SDL
#define SDL

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>

SDL_Window* gWindow = NULL;
SDL_Surface* gScreenSurface = NULL;
SDL_Renderer* gRenderer;
TTF_Font* gFont;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

#endif

#include "emu.hpp"
#include "LTexture.hpp"
#include "cleanup.h"


// Setup
std::string int_to_hex( unsigned short i );

bool InitEverything();
void quit();
bool InitSDL();
bool CreateWindow();
bool SetupTTF( const std::string &fontName );
void escribir(const std::string &message,int x, int y);
SDL_Texture* renderText(const std::string &message,SDL_Color color,int fontSize, SDL_Renderer *renderer);

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr);

int main(int argc, const char * argv[]) {

    std::string rom = argv[2];
    
    
    bool graphics = false;

    if (graphics) {
        if(!InitEverything())
            return -1;
    }

    int i = 0;
    

    
    bool exit = false;
    SDL_Event e;
   
    emu c_emu(rom);
    c_emu.init(true);
    
    bool continuar = true;
    while (!exit) {
        
        i++;
        if (continuar){
            exit = !c_emu.emulateCycle();
            //continuar = false;
        }
        if (graphics){
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                {
                    exit = true;
                }
                else if (e.type == SDL_KEYDOWN){
                    switch( e.key.keysym.sym ){
                        case SDLK_c:
                            continuar = !continuar;
                            
                    }
                    
                    
                }
                
            }
            
            std::string AF = "AF:" + int_to_hex(c_emu.getAF());
            std::string BC = "BC:" + int_to_hex(c_emu.getBC());
            std::string DE = "DE:" + int_to_hex(c_emu.getDE());
            std::string HL = "HL:" + int_to_hex(c_emu.getHL());
            std::string SP = "SP:" + int_to_hex(c_emu.getSP());
            std::string PC = "PC:" + int_to_hex(c_emu.getPC());
            
            std::string next = "NEXT:" + c_emu.nextopcode();
            
            SDL_RenderClear(gRenderer);
            //We can draw our message as we do any other texture, since it's been
            //rendered to a texture
            
            escribir(AF,10,10);
            escribir(BC,10,30);
            escribir(DE,10,50);
            escribir(HL,10,70);
            escribir(SP,10,90);
            escribir(PC,10,110);
            escribir(next,10,130);
            
            SDL_RenderPresent(gRenderer);
            //SDL_Delay(200);
            
        }
        
    }
    
    quit();
    
    std::cout<<"Fin de la emulación";
    return 0;
}

bool InitEverything()
{
    if ( !InitSDL() )
        return false;
    
    if ( !CreateWindow() )
        return false;

    
    if ( !SetupTTF( "/Library/Fonts/Courier New.ttf" ) )
        return false;
    
    return true;
}

void quit(){
    
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
    gFont = TTF_OpenFont( fontName.c_str(), 16 );
    
    // Error check
    if ( gFont == nullptr )
    {
        std::cout << " Failed to load font : " << TTF_GetError() << std::endl;
        return false;
    }
    
    return true;
}

bool CreateWindow()
{
    gWindow = SDL_CreateWindow( "Server",0,0, SCREEN_WIDTH, SCREEN_HEIGHT, 0 );
    
    if ( gWindow == nullptr )
    {
        std::cout << "Failed to create window : " << SDL_GetError();
        return false;
    }
    
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (gRenderer == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        cleanup(gWindow);
        return false;
    }
    return true;
}

void logSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}

void escribir(const std::string &message,int x, int y){
    SDL_Color color = { 255, 255, 255, 255 };
    SDL_Texture *image = renderText(message.c_str(), color, 14, gRenderer);
    renderTexture(image, gRenderer, x, y);
    SDL_DestroyTexture(image);
    
}


SDL_Texture* renderText(const std::string &message,SDL_Color color,
                        int fontSize, SDL_Renderer *renderer)
{
    //Open the font
    /*TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (font == nullptr){
        logSDLError(std::cout, "TTF_OpenFont");
        return nullptr;
    }*/
    //We need to first render to a surface as that's what TTF_RenderText returns, then
    //load that surface into a texture
    SDL_Surface *surf = TTF_RenderText_Blended(gFont, message.c_str(), color);
    if (surf == nullptr){
        TTF_CloseFont(gFont);
        logSDLError(std::cout, "TTF_RenderText");
        return nullptr;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (texture == nullptr){
        logSDLError(std::cout, "CreateTexture");
    }
    //Clean up the surface and font
    SDL_FreeSurface(surf);
    //TTF_CloseFont(gFont);
    return texture;
}
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr){
    SDL_RenderCopy(ren, tex, clip, &dst);
}

void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip){
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if (clip != nullptr){
        dst.w = clip->w;
        dst.h = clip->h;
    }
    else {
        SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    }
    renderTexture(tex, ren, dst, clip);
}

std::string int_to_hex( unsigned short i )
{
    std::stringstream sstream;
    sstream << std::hex << i;
    std::string r = sstream.str();
    while(r.length() < 4){
        r = "0" + r;
    }
    return r;
}