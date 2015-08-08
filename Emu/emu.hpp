
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


#define HBLANK 0
#define VBLANK 1
#define S_OAM 2
#define S_VRAM 3

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

    int test = 0;
    
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
    
    int clock_m;
    int clock_t;

    int gpu_m;
    int gpu_t;
    
    int curr_m;
    int curr_t;

    bool ime;

    void add_counter(int c=4);
    
    unsigned char addition8bit(unsigned char a,unsigned char b,unsigned int z);
    unsigned char substract8bit(unsigned char a,unsigned char b,unsigned int z);
    unsigned char and8bit(unsigned char a,unsigned char b);
    unsigned char or8bit(unsigned char a,unsigned char b);
    unsigned char xor8bit(unsigned char a,unsigned char b);
    unsigned char swap8bit(unsigned char a);
    void inline r_rotation(unsigned char* r);
    
    unsigned char inline ReadMemory(unsigned short address);
    void inline WriteMemory(unsigned short address, unsigned char value);

    
    bool cpuNULL();
    bool cpuCBNULL();
    
    //Instrucciones
    
    bool NOP();         //0x00
    bool LD_BC_d16();   //0x01
    bool DEC_B();       //0x05
    bool LD_B_d8();     //0x06
    bool DEC_BC();      //0x0B
    bool INC_C();       //0x0C
    bool DEC_C();       //0x0D
    bool LD_C_d8();     //0x0E

    bool DEC_D();       //0x15
    bool LD_E_d8();     //0x1E
    bool RRA();         //0x1F
    
    bool JR_NZ_r8();    //0x20
    bool LD_HL_d16();   //0x21
    bool JR_Z_r8();     //0x28
    bool LDI_A_pHL();   //0x2A
    bool CPL();         //0x2F
    
    bool LD_SP_d16();   //0x31
    bool LDD_pHL_A();   //0x32
    bool INC_pHL();     //0x34
    bool LD_pHL_d8();   //0x36
    bool INC_A();       //0x3C
    bool DEC_A();       //0x3D
    bool LD_A_d8();     //0x3E

    bool LD_C_A();      //0x4F
    
    bool LD_A_B();      //0x78
    bool LD_A_C();      //0x79
    bool LD_A_D();      //0x7A
    
    bool ADC_A_C();     //0x89

    bool AND_C();       //0xA1
    bool AND_A();       //0xA7
    bool XOR_C();       //0xA9
    bool XOR_A();       //0xAF

    bool OR_B();        //0xB0
    bool OR_C();        //0xB1
    
    bool RET_NZ();      //0xC0
    bool POP_BC();      //0xC1
    bool JP_a16();      //0xC3
    bool PUSH_BC();     //0xC5
    bool RET_Z();       //0xC8
    bool RET();         //0xC9
    bool CB();          //0xCB
    bool CALL_a16();    //0xCD
    
    bool POP_DE();      //0xD1
    bool PUSH_DE();     //0xD5
    bool RETI();        //0xD9
    
    bool LDH_pa8_A();   //0xE0
    bool POP_HL();      //0xE1
    bool LD_pC_A();     //0xE2
    bool PUSH_HL();     //0xE5
    bool AND_d8();      //0xE6
    bool JP_pHL();      //0xE9
    bool LD_pa16_A();   //0xEA
    
    bool LDH_A_pa8();   //0xF0
    bool POP_AF();      //0xF1
    bool DI();          //0xF3
    bool PUSH_AF();     //0xF5
    bool LD_A_pa16();   //0xFA
    bool EI();          //0xFB
    bool CP_d8();       //0xFE
    
    //CB
    
    bool SWAP_A();      //0xCB37

    bool RST(unsigned char addr);

    std::string opcodes [0x100] = {"0x0,NOP",
        "0x01 n n,LD BC,nn",
        "0x2,LD (BC),A",
        "0x3,INC BC",
        "0x4,INC B",
        "0x5,DEC B",
        "0x06 n,LD B,n",
        "0x7,RLCA",
        "0x8,LD   (nn),SP",
        "0x9,ADD HL,BC",
        "0x0A,LD A,(BC)",
        "0x0B,DEC BC",
        "0x0C,INC C",
        "0x0D,DEC C",
        "0x0E n,LD C,n",
        "0x0F,RRCA",
        "0x10 e,STOP",
        "0x11 n n,LD DE,nn",
        "0x12,LD (DE),A",
        "0x13,INC DE",
        "0x14,INC D",
        "0x15,DEC D",
        "0x16 n,LD D,n",
        "0x17,RLA",
        "0x18 e,JR (PC+e)",
        "0x19,ADD HL,DE",
        "0x1A,LD A,(DE)",
        "0x1B,DEC DE",
        "0x1C,INC E",
        "0x1D,DEC E",
        "0x1E n,LD E,n",
        "0x1F,RRA",
        "0x20 e,JR NZ,(PC+e)",
        "0x21 n n,LD HL,nn",
        "0x22,LDI  (HL),A",
        "0x23,INC HL",
        "0x24,INC H",
        "0x25,DEC H",
        "0x26 n,LD H,n",
        "0x27,DAA",
        "0x28 e,JR Z,(PC+e)",
        "0x29,ADD HL,HL",
        "0x2A,LDI  A,(HL)",
        "0x2B,DEC HL",
        "0x2C,INC L",
        "0x2D,DEC L",
        "0x2E n,LD L,n",
        "0x2F,CPL",
        "0x30 e,JR NC,(PC+e)",
        "0x31 n n,LD SP,nn",
        "0x32,LDD  (HL),A",
        "0x33,INC SP",
        "0x34,INC (HL)",
        "0x35,DEC (HL)",
        "0x36 n,LD (HL),n",
        "0x37,SCF",
        "0x38 e,JR C,(PC+e)",
        "0x39,ADD HL,SP",
        "0x3A,LDD  A,(HL)",
        "0x3B,DEC SP",
        "0x3C,INC A",
        "0x3D,DEC A",
        "0x3E n,LD A,n",
        "0x3F,CCF",
        "0x40,LD B,B",
        "0x41,LD B,C",
        "0x42,LD B,D",
        "0x43,LD B,E",
        "0x44,LD B,H",
        "0x45,LD B,L",
        "0x46,LD B,(HL)",
        "0x47,LD B,A",
        "0x48,LD C,B",
        "0x49,LD C,C",
        "0x4A,LD C,D",
        "0x4B,LD C,E",
        "0x4C,LD C,H",
        "0x4D,LD C,L",
        "0x4E,LD C,(HL)",
        "0x4F,LD C,A",
        "0x50,LD D,B",
        "0x51,LD D,C",
        "0x52,LD D,D",
        "0x53,LD D,E",
        "0x54,LD D,H",
        "0x55,LD D,L",
        "0x56,LD D,(HL)",
        "0x57,LD D,A",
        "0x58,LD E,B",
        "0x59,LD E,C",
        "0x5A,LD E,D",
        "0x5B,LD E,E",
        "0x5C,LD E,H",
        "0x5D,LD E,L",
        "0x5E,LD E,(HL)",
        "0x5F,LD E,A",
        "0x60,LD H,B",
        "0x61,LD H,C",
        "0x62,LD H,D",
        "0x63,LD H,E",
        "0x64,LD H,H",
        "0x65,LD H,L",
        "0x66,LD H,(HL)",
        "0x67,LD H,A",
        "0x68,LD L,B",
        "0x69,LD L,C",
        "0x6A,LD L,D",
        "0x6B,LD L,E",
        "0x6C,LD L,H",
        "0x6D,LD L,L",
        "0x6E,LD L,(HL)",
        "0x6F,LD L,A",
        "0x70,LD (HL),B",
        "0x71,LD (HL),C",
        "0x72,LD (HL),D",
        "0x73,LD (HL),E",
        "0x74,LD (HL),H",
        "0x75,LD (HL),L",
        "0x76,HALT",
        "0x77,LD (HL),A",
        "0x78,LD A,B",
        "0x79,LD A,C",
        "0x7A,LD A,D",
        "0x7B,LD A,E",
        "0x7C,LD A,H",
        "0x7D,LD A,L",
        "0x7E,LD A,(HL)",
        "0x7F,LD A,A",
        "0x80,ADD A,B",
        "0x81,ADD A,C",
        "0x82,ADD A,D",
        "0x83,ADD A,E",
        "0x84,ADD A,H",
        "0x85,ADD A,L",
        "0x86,ADD A,(HL)",
        "0x87,ADD A,A",
        "0x88,ADC A,B",
        "0x89,ADC A,C",
        "0x8A,ADC A,D",
        "0x8B,ADC A,E",
        "0x8C,ADC A,H",
        "0x8D,ADC A,L",
        "0x8E,ADC A,(HL)",
        "0x8F,ADC A,A",
        "0x90,SUB B",
        "0x91,SUB C",
        "0x92,SUB D",
        "0x93,SUB E",
        "0x94,SUB H",
        "0x95,SUB L",
        "0x96,SUB (HL)",
        "0x97,SUB A",
        "0x98,SBC A,B",
        "0x99,SBC A,C",
        "0x9A,SBC A,D",
        "0x9B,SBC A,E",
        "0x9C,SBC A,H",
        "0x9D,SBC A,L",
        "0x9E,SBC A,(HL)",
        "0x9F,SBC A,A",
        "0xA0,AND B",
        "0xA1,AND C",
        "0xA2,AND D",
        "0xA3,AND E",
        "0xA4,AND H",
        "0xA5,AND L",
        "0xA6,AND (HL)",
        "0xA7,AND A",
        "0xA8,XOR B",
        "0xA9,XOR C",
        "0xAA,XOR D",
        "0xAB,XOR E",
        "0xAC,XOR H",
        "0xAD,XOR L",
        "0xAE,XOR (HL)",
        "0xAF,XOR A",
        "0xB0,OR B",
        "0xB1,OR C",
        "0xB2,OR D",
        "0xB3,OR E",
        "0xB4,OR H",
        "0xB5,OR L",
        "0xB6,OR (HL)",
        "0xB7,OR A",
        "0xB8,CP B",
        "0xB9,CP C",
        "0xBA,CP D",
        "0xBB,CP E",
        "0xBC,CP H",
        "0xBD,CP L",
        "0xBE,CP (HL)",
        "0xBF,CP A",
        "0xC0,RET NZ",
        "0xC1,POP BC",
        "0xC2 n n,JP NZ,(nn)",
        "0xC3 n n,JP (nn)",
        "0xC4 n n,CALL NZ,(nn)",
        "0xC5,PUSH BC",
        "0xC6 n,ADD A,n",
        "0xC7,RST 0H",
        "0xC8,RET Z",
        "0xC9,RET",
        "0xCA n n,JP Z,(nn)",
        "0xCB",
        "0xCC n n,CALL Z,(nn)",
        "0xCD n n,CALL (nn)",
        "0xCE n,ADC A,n",
        "0xCF,RST 8H",
        "0xD0,RET NC",
        "0xD1,POP DE",
        "0xD2 n n,JP NC,(nn)",
        "0xD3 n,-",
        "0xD4 n n,CALL NC,(nn)",
        "0xD5,PUSH DE",
        "0xD6 n,SUB n",
        "0xD7,RST 10H",
        "0xD8,RET C",
        "0xD9,RETI",
        "0xDA n n,JP C,(nn)",
        "0xDB n,-",
        "0xDC n n,CALL C,(nn)",
        "0xDD,-",
        "0xDE n,SBC A,n",
        "0xDF,RST 18H",
        "0xE0 n,LD   (FF00+n),A",
        "0xE1,POP HL",
        "0xE2,LD   (FF00+C),A",
        "0xE3,-",
        "0xE4 n n,-",
        "0xE5,PUSH HL",
        "0xE6 n,AND n",
        "0xE7,RST 20H",
        "0xE8,ADD  SP,dd",
        "0xE9,JP (HL)",
        "0xEA n n,LD   (nn),A",
        "0xEB,-",
        "0xEC n n,-",
        "0xED,-",
        "0xEE n,XOR n",
        "0xEF,RST 28H",
        "0xF0,LD   A,(FF00+n)",
        "0xF1,POP AF",
        "0xF2,LD   A,(FF00+C)",
        "0xF3,DI",
        "0xF4 n n,-",
        "0xF5,PUSH AF",
        "0xF6 n,OR n",
        "0xF7,RST 30H",
        "0xF8,LD   HL,SP+dd",
        "0xF9,LD SP,HL",
        "0xFA n n,LD   A,(nn)",
        "0xFB,EI",
        "0xFC,-",
        "0xFD,-",
        "0xFE n,CP n",
        "0xFF,RST 38H"};
    

    

    bool (emu::*table[16*16])() = {
        //0x0X
        &emu::NOP, &emu::LD_BC_d16, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::DEC_B, &emu::LD_B_d8, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::DEC_BC, &emu::INC_C, &emu::DEC_C, &emu::LD_C_d8, &emu::cpuNULL,
        
        //0x1X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::DEC_D, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::LD_E_d8, &emu::RRA,
        
        //0x2X
        &emu::JR_NZ_r8, &emu::LD_HL_d16, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::JR_Z_r8, &emu::cpuNULL, &emu::LDI_A_pHL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::CPL,
        
        //0x3X
        &emu::cpuNULL, &emu::LD_SP_d16, &emu::LDD_pHL_A, &emu::cpuNULL, &emu::INC_pHL, &emu::cpuNULL, &emu::LD_pHL_d8, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::INC_A, &emu::DEC_A, &emu::LD_A_d8, &emu::cpuNULL,
        
        //0x4X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::LD_C_A,
        
        //0x5X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0x6X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0x7X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::LD_A_B, &emu::LD_A_C, &emu::LD_A_D, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0x8X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::ADC_A_C, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0x9X
        &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0xAX
        &emu::cpuNULL, &emu::AND_C, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::AND_A, &emu::cpuNULL, &emu::XOR_C, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::XOR_A,
        
        //0xBX
        &emu::OR_B, &emu::OR_C, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0xCX
        &emu::RET_NZ, &emu::POP_BC, &emu::cpuNULL, &emu::JP_a16, &emu::cpuNULL, &emu::PUSH_BC, &emu::cpuNULL, &emu::cpuNULL, &emu::RET_Z, &emu::RET, &emu::cpuNULL, &emu::CB, &emu::cpuNULL, &emu::CALL_a16, &emu::cpuNULL, &emu::cpuNULL,
        
        //0xDX
        &emu::cpuNULL, &emu::POP_DE, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::PUSH_DE, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::RETI, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0xEX
        &emu::LDH_pa8_A, &emu::POP_HL, &emu::LD_pC_A, &emu::cpuNULL, &emu::cpuNULL, &emu::PUSH_HL, &emu::AND_d8, &emu::cpuNULL, &emu::cpuNULL, &emu::JP_pHL, &emu::LD_pa16_A, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL,
        
        //0xFX
        &emu::LDH_A_pa8, &emu::POP_AF, &emu::cpuNULL, &emu::DI, &emu::cpuNULL, &emu::PUSH_AF, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::cpuNULL, &emu::LD_A_pa16, &emu::EI, &emu::cpuNULL, &emu::cpuNULL, &emu::CP_d8, &emu::cpuNULL,
        
        
    };
    
    
    bool (emu::*cb[16*16])() = {

        //0x0X
        &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL,
        
        //0x1X
        &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL,
        
        //0x2X
        &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL,
        
        //0x3X
        &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::SWAP_A, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL,
        
        //0x4X
        &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL,
        
        //0x5X
        &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL,
        
        //0x6X
        &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL,
        
        //0x7X
        &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL,
        
        //0x8X
        &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL,
        
        //0x9X
        &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL,
        
        //0xAX
        &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL,
        
        //0xBX
        &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL,
        
        //0xCX
        &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL,
        
        //0xDX
        &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL,
        
        //0xEX
        &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL,
        
        //0xFX
        &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL, &emu::cpuCBNULL,
        
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
    std::string nextopcode();
    
    

};


#endif
