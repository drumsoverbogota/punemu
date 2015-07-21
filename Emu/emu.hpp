
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

#define CALL_MEMBER_FN(object,ptrToMember)  ()

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <bitset>

//typedef void (*GameBoyTable[16*16])();



class emu{

    
private:

    bool debug;
    
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
    
    unsigned char substract8bit(unsigned char a,unsigned char b);
    unsigned char xor8bit(unsigned char a,unsigned char b);
    void inline WriteMemory(unsigned short address, unsigned char value);
    void inline r_rotation(unsigned char* r);
    
    bool cpuNULL();
    
    //Instrucciones
    
    bool NOP();         //0x00
    bool DEC_B();       //0x05
    bool LD_B_d8();     //0x06
    bool LD_C_d8();     //0x0E

    bool LD_E_d8();     //0x1E
    bool RRA();         //0x1F
    
    bool JR_NZ_r8();    //0x20
    bool LD_HL_d16();   //0x21
    
    bool LDD_HL_A();    //0x32
    
    bool XOR_A();       //0xAF
    
    bool JP_a16();      //0xC3

    
    

    bool (emu::*table[16*16])() = {
        //0x0X
        &emu::NOP, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::DEC_B, &emu::LD_B_d8, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::LD_C_d8, &emu::cpuNULL,
        
        //0x1X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::LD_E_d8, &emu::RRA,
        
        //0x2X
        &emu::JR_NZ_r8, &emu::LD_HL_d16, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0x3X
        &emu::cpuNULL, &emu::cpuNULL, &emu::LDD_HL_A, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0x4X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0x5X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0x6X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0x7X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0x8X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0x9X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0xAX
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::XOR_A,
        
        //0xBX
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0xCX
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::JP_a16, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0xDX
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0xEX
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0xFX
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        
    };
    
public:
    

    
    emu();
    emu(std::string file);
    void init(bool debug);
    bool emulateCycle();

};


#endif
