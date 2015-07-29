
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

#ifndef STD
#define STD

#include <iostream>
#endif
#include <sstream>
#include <fstream>
#include <string>

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
    unsigned char vram[8192];
    unsigned char hram[127];
    unsigned char IO[128];
    unsigned char IE;
    unsigned char rombank;
    unsigned char R[8];
    unsigned char opcode;
    unsigned short SP;
    unsigned short PC;

    unsigned char addition8bit(unsigned char a,unsigned char b,unsigned int z);
    unsigned char substract8bit(unsigned char a,unsigned char b,unsigned int z);
    unsigned char or8bit(unsigned char a,unsigned char b);
    unsigned char xor8bit(unsigned char a,unsigned char b);
    void inline r_rotation(unsigned char* r);
    
    unsigned char inline ReadMemory(unsigned short address);
    void inline WriteMemory(unsigned short address, unsigned char value);

    
    bool cpuNULL();
    
    //Instrucciones
    
    bool NOP();         //0x00
    bool LD_BC_d16();   //0x01
    bool DEC_B();       //0x05
    bool LD_B_d8();     //0x06
    bool DEC_BC();      //0x0B
    bool DEC_C();       //0x0D
    bool LD_C_d8();     //0x0E

    bool DEC_D();       //0x15
    bool LD_E_d8();     //0x1E
    bool RRA();         //0x1F
    
    bool JR_NZ_r8();    //0x20
    bool LD_HL_d16();   //0x21
    bool LDI_A_pHL();   //0x2A
    
    bool LD_SP_d16();   //0x31
    bool LDD_pHL_A();   //0x32
    bool LD_pHL_d8();   //0x36
    bool LD_A_d8();     //0x3E

    bool LD_A_B();      //0x78
    bool LD_A_D();      //0x7A
    
    bool ADC_A_C();     //0x89
    
    bool XOR_A();       //0xAF
    
    bool OR_C();        //0xB1
    
    bool JP_a16();      //0xC3
    bool RET();         //0xC9
    bool CALL_a16();    //0xCD
    
    bool LDH_pa8_A();   //0xE0
    bool LD_pC_A();     //0xE2
    bool JP_pHL();      //0xE9
    bool LD_pa16_A();   //0xEA
    
    bool LDH_A_pa8();   //0xF0
    bool DI();          //0xF3
    bool CP_d8();       //0xFE

    
    

    bool (emu::*table[16*16])() = {
        //0x0X
        &emu::NOP, &emu::LD_BC_d16, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::DEC_B, &emu::LD_B_d8, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::DEC_BC, &emu::cpuNULL, &emu::DEC_D, &emu::LD_C_d8, &emu::cpuNULL,
        
        //0x1X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::DEC_D, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::LD_E_d8, &emu::RRA,
        
        //0x2X
        &emu::JR_NZ_r8, &emu::LD_HL_d16, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::LDI_A_pHL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0x3X
        &emu::cpuNULL, &emu::LD_SP_d16, &emu::LDD_pHL_A, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::LD_pHL_d8, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::LD_A_d8, &emu::cpuNULL,
        
        //0x4X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0x5X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0x6X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0x7X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::LD_A_B, &emu::cpuNULL, &emu::LD_A_D, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0x8X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::ADC_A_C, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0x9X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0xAX
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::XOR_A,
        
        //0xBX
        &emu::cpuNULL, &emu::OR_C, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0xCX
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::JP_a16, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::RET, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::CALL_a16, &emu::cpuNULL, &emu::cpuNULL,
        
        //0xDX
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0xEX
        &emu::LDH_pa8_A, &emu::cpuNULL, &emu::LD_pC_A, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::JP_pHL, &emu::LD_pa16_A, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0xFX
        &emu::LDH_A_pa8, &emu::cpuNULL, &emu::cpuNULL, &emu::DI, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::CP_d8, &emu::cpuNULL,
        
        
    };
    
public:
    
    emu();
    emu(std::string file);
    void init(bool debug);
    bool emulateCycle();
    
    
    unsigned short getAF();
    unsigned short getBC();
    unsigned short getDE();
    unsigned short getHL();
    unsigned short getPC();
    unsigned short getSP();
    
    

};


#endif
