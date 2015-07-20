
#ifndef EMU
#define EMU

#define A 0
#define F 1
#define B 2
#define C 3
#define D 4
#define E 5
#define H 6
#define L 7


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>

class emu{

    
private:

    //Propiedades del juego
    std::string title;
    bool isgbcolor;
    char cart_type;

    
    std::ifstream cart;
    unsigned char * rom;
    
    
    
    unsigned char ram[8192];
    unsigned char vram  [8192];
    unsigned char R[8];
    unsigned char opcode;
    unsigned short SP;
    unsigned short PC;
    
    unsigned char substract8bit(unsigned char a,unsigned char b,unsigned char* f);
    unsigned char xor8bit(unsigned char a,unsigned char b,unsigned char* f);
    void inline WriteMemory(unsigned short address, unsigned char value);
    
    
public:
    emu();
    emu(std::string file);
    void init();
    bool emulateCycle(bool debug);

};

#endif
