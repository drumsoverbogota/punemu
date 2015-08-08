

#include "emu.hpp"

emu::emu(){
    
}

emu::emu(std::string file){

    cart.open(file,std::ios::in|std::ios::binary);
    if(cart.is_open()){
        
        // get length of file:
        cart.seekg (0, cart.end);
        std::streampos length = cart.tellg();
        cart.seekg (0, cart.beg);
        
        rom = new unsigned char [length];
        
        std::cout << "Reading " << length << " characters... ";
        // read data as a block:
        cart.read (reinterpret_cast<char*>(rom),length);
        
        if (cart)
            std::cout << "all characters read successfully."<<std::endl;
        else
            std::cout << "error: only " << cart.gcount() << " could be read";
        cart.close();
        

        int title_lenght = 0x142 - 0x134;
        char subbuff[title_lenght];
        memcpy( subbuff, &rom[0x134], title_lenght );
        subbuff[title_lenght] = '\0';
        
        title = subbuff;
        unsigned int gbc = rom[0x143];

        if(gbc==0x80)
            isgbcolor = true;
        else
            isgbcolor = false;
        
        cart_type = rom[0x147];

        
    }
    else{

        std::cout <<"nonas"<<std::endl;
    }
    
    std::cout<<title<<std::endl;
    
    if (isgbcolor)
        std::cout<<"Game Boy Color title"<<std::endl;
    else
        std::cout<<"Not Game Boy Color title"<<std::endl;
    
    switch (cart_type) {
        case 0x00:
            std::cout<<"ROM ONLY"<<std::endl;
            break;
        case 0x1B:
            std::cout<<"MBC5+RAM+BATTERY"<<std::endl;
            break;
        default:
            break;
    }

}


void emu::init(bool debug){
    
    this->debug = debug;
    
    
    PC = 0x100;
    SP = 0xFFFE;
    opcode = 0x0;
    
    R[A] = 0x01;
    R[F] = 0xb0;
    
    R[B] = 0x00;
    R[C] = 0x13;
    
    R[D] = 0x00;
    R[E] = 0xd8;

    R[H] = 0x01;
    R[L] = 0x4d;
    
    clock_m = 0;
    clock_t = 0;

    curr_m = 0;
    curr_t = 0;
    
    gpu_m = 0;
    gpu_t = 0;
}

void emu::WriteMemory(unsigned short address, unsigned char value){
    int a = address;
    
    
    if (a >= 0xe000 && a <= 0xfdff)      // RAM echo
        ram[a-0xe000] = value;
    else if (a >= 0xc000 && a <= 0xdfff) // RAM
        ram[a-0xc000] = value;
    else if (a >= 0x8000 && a <= 0x9fff) // VRAM
        vram[a-0x8000] = value;
    else if (a >= 0xFF00 && a <= 0xFF7F) // IO
        IO[a-0xFF00] = value;
    else if (a >= 0x2000 && a <= 0x3FFF) // ROM bank number
        rombank = value;
    else if (a >= 0xFF80 && a <= 0xFFFE) // HRAM
        hram[a-0xFF80] = value;
    else if (a == 0xFFFF)                //IE
        IE = value;
    else
        std::cout<<"ERROR: CAN'T WRITE ADDRESS:"<<std::hex<<int(a)<<std::endl;

}

unsigned char emu::ReadMemory(unsigned short address){
 
    int a = address;
    
    
    if (a >= 0xe000 && a <= 0xfdff)
        return ram[a-0xe000];
    else if (a >= 0xc000 && a <= 0xdfff) //ECHO
        return ram[a-0xc000];
    else if (a >= 0x8000 && a <= 0x9fff) //VRAM
        return vram[a-0x8000];
    else if (a >= 0xFF00 && a <= 0xFF7F) //IO
        return IO[a-0xFF00];
    else if (a >= 0xFF80 && a <= 0xFFFE) //HRAM
        return hram[a-0xFF80];
    else if (a >= 0x0000 && a <= 0x7FFF) //ROM
        return rom[a];
    else if (a == 0xFFFF)                //IE
        return IE;
    else{
        std::cout<<"ERROR: CAN'T READ ADDRESS:"<<std::hex<<int(a)<<std::endl;
        return 0x00;
    }
    
}

unsigned char emu::and8bit(unsigned char a,unsigned char b){
    unsigned char result = a & b;
    R[F] |= 0x20;
    if (result == 0x0)
        R[F] |= 0xAF;
    else
        R[F] |= 0x7F;


    return result;
}


unsigned char emu::or8bit(unsigned char a,unsigned char b){
    unsigned char result = a | b;
    if (result == 0x0)
        R[F] |= 0x8F;
    else
        R[F] &= 0x0F;
    return result;
}


unsigned char emu::xor8bit(unsigned char a,unsigned char b){
    unsigned char result = a ^ b;
    if (result == 0x0)
        R[F] &= 0x8F;
    else
        R[F] &= 0x0F;
    return result;
}

unsigned char emu::swap8bit(unsigned char a){
    unsigned char result = ((a&0xF0)>>4) | ((a&0x0F)<<4) ;
    if (result == 0x0)
        R[F] |= 0x8F;
    else
        R[F] &= 0x0F;
    return result;
}

unsigned char emu::addition8bit(unsigned char a,unsigned char b, unsigned int z){
    
    unsigned int Z = z;
    unsigned char result = 0x0;
    
    for (int i=0; i<8; i++) {
        int X = (a & (1<<i)) != 0;
        int Y = (b & (1<<i)) != 0;
        
        unsigned int d = X ^ Y ^ Z;
        Z = (Z & (X ^ Y)) | (X & Y);
        
        if (i == 3){
            if (Z == 1)
                R[F] |= 0x20; //set H flag to 1
            else
                R[F] &= 0xDF; //set H flag to 0
        }
        if (i == 7){
            if (Z == 1)
                R[F] |= 0x10; //set C flag to 1
            else
                R[F] &= 0xEF; //set C flag to 0
        }
        
        result = (result) | (d << i);
    }
    
    R[F] &= 0xB0; //set N flag to 1
    if (int(result)==0)
        R[F] |= 0x80; //set Z flag to 1
    else
        R[F] &= 0x7F; //set Z flag to 0
    
    return result;
}


unsigned char emu::substract8bit(unsigned char a,unsigned char b, unsigned int z){
    
    unsigned int Z = z;
    unsigned char result = 0x0;
    
    //std::cout<<"FLAG:"<<std::hex<<int(flag)<<std::endl;
    
    for (int i=0; i<8; i++) {
        int X = (a & (1<<i)) != 0;
        int Y = (b & (1<<i)) != 0;
    
        unsigned int d = X ^ Y ^ Z;
        Z = (~X & (Y | Z)) | (Y & Z);
        
        if (i == 3){
            if (Z == 1)
                R[F] |= 0x20; //set H flag to 1
            else
                R[F] &= 0xDF; //set H flag to 0
        }
        if (i == 7){
            if (Z == 1)
                R[F] |= 0x10; //set C flag to 1
            else
                R[F] &= 0xEF; //set C flag to 0
        }
    
        result = (result) | (d << i);
    }
    
    R[F] |= 0x40; //set N flag to 1
    if (int(result)==0)
        R[F] |= 0x80; //set Z flag to 1
    else
        R[F] &= 0x7F; //set Z flag to 0
    
    return result;
}


void emu::r_rotation(unsigned char *r){

    unsigned char reg = *r;
    
    if( (reg & 0x01)> 0)  //si el último bit es 1 la bandera C queda en 1, si no en 0
        R[F] |= 0x10;
    else
        R[F] &= 0xE0;
     
    reg = reg>>1;
    
    if (reg==0)
        R[F] |= 0x80; //set Z flag to 1
    else
        R[F] &= 0x7F; //set Z flag to 0
    
    *r = reg;
    

    R[F] &= 0x9F; // Poner N y H en 0
    //std::cout<<"Rot "<<std::hex<<int(R[F])<<std::endl;
    PC ++;
    
}

////////////////////////OPCODES////////////////////////




//+++++++++++++++++++++++++++8-bit loads +++++++++++++++++++++++++++//

bool emu::LD_A_B(){
    if (debug)
        std::cout<<"LD A B"<<std::endl;
    R[A] = R[B];
    PC ++;
    add_counter();
    return true;
}


bool emu::LD_A_C(){
    if (debug)
        std::cout<<"LD A D "<<std::endl;
    R[A] = R[D];
    PC ++;
    add_counter();
    return true;
}


bool emu::LD_A_D(){
    if (debug)
        std::cout<<"LD A D "<<std::endl;
    R[A] = R[D];
    PC ++;
    add_counter();
    return true;
}


bool emu::LD_A_d8(){
    if (debug)
        std::cout<<"LD A, "<<std::hex<<int(ReadMemory(PC + 1))<<std::endl;
    R[A] = ReadMemory(PC + 1);
    PC += 2;
    add_counter(8);
    return true;
}


bool emu::LD_B_d8(){
    if (debug)
        std::cout<<"LD B, "<<std::hex<<int(ReadMemory(PC + 1))<<std::endl;
    R[B] = ReadMemory(PC + 1);
    PC += 2;
    add_counter(8);
    return true;
}

bool emu::LD_C_A(){
    if (debug)
        std::cout<<"LD C A "<<std::endl;
    R[C] = R[A];
    PC ++;
    add_counter();
    return true;
}

bool emu::LD_C_d8(){
    if (debug)
        std::cout<<"LD C, "<<std::hex<<int(ReadMemory(PC + 1))<<std::endl;
    R[C] = ReadMemory(PC + 1);
    PC += 2;
    add_counter(8);
    return true;
}

bool emu::LD_E_d8(){
    if (debug)
        std::cout<<"LD E, "<<std::hex<<int(ReadMemory(PC + 1))<<std::endl;
    R[E] = ReadMemory(PC + 1);
    PC += 2;
    add_counter(8);
    return true;
}


bool emu::LD_pHL_d8(){
    if (debug)
        std::cout<<"LD (HL),"<<std::hex<<int(ReadMemory(PC + 1))<<std::endl;
    WriteMemory(R[H] << 8 | R[L], ReadMemory(PC+1));
    PC += 2;
    add_counter(12);
    return true;
}

bool emu::LD_A_pa16(){
    if (debug)
        std::cout<<"LD A,("<<std::hex<<int(ReadMemory(PC + 2) << 8 |  ReadMemory(PC + 1))<<")"<<std::endl;
    
    unsigned short a16 = ReadMemory(PC + 2) << 8 |  ReadMemory(PC + 1);
    R[A] = ReadMemory(a16);
    
    PC+=3;
    add_counter(16);
    return true;
}

bool emu::LD_pa16_A(){
    if (debug)
        std::cout<<"LD ("<<std::hex<<int(ReadMemory(PC + 2) << 8 |  ReadMemory(PC + 1))<<"),A"<<std::endl;
    
    unsigned short a16 = ReadMemory(PC + 2) << 8 |  ReadMemory(PC + 1);
    WriteMemory(a16, R[A]);
    
    PC+=3;
    add_counter(16);
    return true;
}


bool emu::LD_pC_A(){
    if (debug)
        std::cout<<"LD (C),A"<<std::endl;
    
    WriteMemory(0xFF00+R[C], R[A]);
    
    PC++;
    add_counter(8);
    return true;
}

bool emu::LDD_pHL_A(){
    if (debug)
        std::cout<<"LD (HL-),A"<<std::endl;
    unsigned short HL = R[H] << 8 | R[L];
    
    WriteMemory(HL, R[A]);
    
    HL -= 0x1;
    R[H] = (HL & 0xFF00)>>8;
    R[L] = HL & 0x00FF;
    PC++;
    add_counter(8);
    return true;
}

bool emu::LDI_A_pHL(){
    if (debug)
        std::cout<<"LD A,(HL+)"<<std::endl;
    unsigned short HL = R[H] << 8 | R[L];
    
    R[A] = ReadMemory(HL);
    
    HL += 0x1;
    R[H] = (HL & 0xFF00)>>8;
    R[L] = HL & 0x00FF;
    PC++;
    add_counter(8);
    return true;
}

bool emu::LDH_pa8_A(){
    if (debug)
        std::cout<<"LDH ("<<std::hex<<int(ReadMemory(PC + 1))<<"),A"<<std::endl;
    
    WriteMemory(0xFF00+ReadMemory(PC + 1), R[A]);
    
    PC+=2;
    
    add_counter(12);
    return true;
}

bool emu::LDH_A_pa8(){
    if (debug)
        std::cout<<"LDH A,("<<std::hex<<int(ReadMemory(PC + 1))<<")"<<std::endl;
    
    R[A] = ReadMemory(0xFF00+ ReadMemory(PC + 1));
    PC+=2;
    
    add_counter(12);
    return true;
}

//+++++++++++++++++++++++++++16-bit loads +++++++++++++++++++++++++++//

bool emu::LD_BC_d16(){
    if (debug)
        std::cout<<"LD BC, "<<std::hex<<int(ReadMemory(PC + 2) << 8 |  ReadMemory(PC + 1))<<std::endl;
    R[B] = ReadMemory(PC + 2);
    R[C] = ReadMemory(PC + 1);
    PC += 3;
    add_counter(12);
    return true;
}

bool emu::LD_HL_d16(){
    if (debug)
        std::cout<<"LD HL, "<<std::hex<<int(ReadMemory(PC + 2) << 8 |  ReadMemory(PC + 1))<<std::endl;
    R[H] = ReadMemory(PC + 2);
    R[L] = ReadMemory(PC + 1);
    PC += 3;
    add_counter(12);
    return true;
}

bool emu::LD_SP_d16(){
    if (debug)
        std::cout<<"LD SP, "<<std::hex<<int(ReadMemory(PC + 2) << 8 |  ReadMemory(PC + 1))<<std::endl;
    SP = ReadMemory(PC + 2) << 8 |  ReadMemory(PC + 1);
    PC += 3;
    add_counter(12);
    return true;
}

bool emu::PUSH_AF(){
    if (debug)
        std::cout<<"PUSH AF"<<std::endl;
    
    std::cout<<"AF"<<getAF()<<std::endl;
    
    WriteMemory(SP-1, R[A]);
    WriteMemory(SP-2, R[F]);
    SP -= 2;
    PC++;
    add_counter(16);
    return true;
}

bool emu::PUSH_BC(){
    if (debug)
        std::cout<<"PUSH BC"<<std::endl;
    WriteMemory(SP-1, R[B]);
    WriteMemory(SP-2, R[C]);
    SP -= 2;
    PC++;
    add_counter(16);
    return true;
}

bool emu::PUSH_DE(){
    if (debug)
        std::cout<<"PUSH DE"<<std::endl;
    WriteMemory(SP-1, R[D]);
    WriteMemory(SP-2, R[E]);
    SP -= 2;
    PC++;
    add_counter(16);
    return true;
}

bool emu::PUSH_HL(){
    if (debug)
        std::cout<<"PUSH HL"<<std::endl;
    
    std::cout<<"------"<<std::endl;
    std::cout<<"AF"<<getAF()<<std::endl;
    std::cout<<"BC"<<getBC()<<std::endl;
    std::cout<<"DE"<<getDE()<<std::endl;
    std::cout<<"HL"<<getHL()<<std::endl;

    WriteMemory(SP-1, R[H]);
    WriteMemory(SP-2, R[L]);
    SP -= 2;
    PC++;
    add_counter(16);
    return true;
}

bool emu::POP_AF(){
    if (debug)
        std::cout<<"POP AF"<<std::endl;
    
    std::cout<<"------"<<std::endl;
    std::cout<<"AF"<<getAF()<<std::endl;
    std::cout<<"BC"<<getBC()<<std::endl;
    std::cout<<"DE"<<getDE()<<std::endl;
    std::cout<<"HL"<<getHL()<<std::endl;
    
    R[A] = ReadMemory(SP+1);
    R[F] = ReadMemory(SP);
    
    SP += 2;
    PC++;
    add_counter(12);
    
    return true;
}

bool emu::POP_BC(){
    if (debug)
        std::cout<<"POP HL"<<std::endl;
    
    R[B] = ReadMemory(SP+1);
    R[C] = ReadMemory(SP);
    
    SP += 2;
    PC++;
    add_counter(12);
    
    return true;
}

bool emu::POP_DE(){
    if (debug)
        std::cout<<"POP HL"<<std::endl;
    
    R[D] = ReadMemory(SP+1);
    R[E] = ReadMemory(SP);
    
    SP += 2;
    PC++;
    add_counter(12);
    
    return true;
}

bool emu::POP_HL(){
    if (debug)
        std::cout<<"POP HL"<<std::endl;
    
    R[H] = ReadMemory(SP+1);
    R[L] = ReadMemory(SP);
    
    SP += 2;
    PC++;
    add_counter(12);
    
    return true;
}


//+++++++++++++++++++++++++++8-bit ALU +++++++++++++++++++++++++++//

bool emu::ADC_A_C(){
    if (debug)
        std::cout<<"ADC A,C "<<std::endl;
    R[A] = addition8bit(R[A], R[C], (R[F]&0x10)>>4);
    PC++;
    add_counter();
    return true;
}

bool emu::AND_d8(){
    if (debug)
        std::cout<<"AND "<<std::hex<<int(ReadMemory(PC + 1))<<std::endl;
    R[A] = and8bit(R[A], ReadMemory(PC+1));
    PC+=2;
    add_counter(8);
    return true;
}

bool emu::AND_A(){
    if (debug)
        std::cout<<"AND A"<<std::endl;
    R[A] = and8bit(R[A], R[A]);
    PC++;
    add_counter();
    return true;
}

bool emu::AND_C(){
    if (debug)
        std::cout<<"AND C"<<std::endl;
    R[A] = and8bit(R[A], R[C]);
    PC++;
    add_counter();
    return true;
}

bool emu::OR_B(){
    if (debug)
        std::cout<<"OR B"<<std::endl;
    R[A] = or8bit(R[A], R[B]);
    PC++;
    add_counter();
    return true;
}

bool emu::OR_C(){
    if (debug)
        std::cout<<"OR C"<<std::endl;
    R[A] = or8bit(R[A], R[C]);
    PC++;
    add_counter();
    return true;
}

bool emu::XOR_A(){
    if (debug)
        std::cout<<"XOR A"<<std::endl;
    R[A] = xor8bit(R[A], R[A]);
    PC++;
    add_counter();
    return true;
}

bool emu::XOR_C(){
    if (debug)
        std::cout<<"XOR C"<<std::endl;
    R[A] = xor8bit(R[A], R[C]);
    PC++;
    add_counter();
    return true;
}

bool emu::CP_d8(){
    if (debug)
        std::cout<<"CP "<<std::hex<<int(ReadMemory(PC + 1))<<std::endl;
    substract8bit(R[A], ReadMemory(PC+1), 0);
    PC+=2;
    add_counter(8);
    return true;
}

bool emu::INC_A(){
    if (debug)
        std::cout<<"INC A"<<std::endl;
    R[A] = addition8bit(R[A], 0x1,0);
    PC++;
    add_counter();
    return true;
}

bool emu::INC_C(){
    if (debug)
        std::cout<<"INC C"<<std::endl;
    R[C] = addition8bit(R[C], 0x1,0);
    PC++;
    add_counter();
    return true;
}

bool emu::INC_pHL(){
    if (debug)
        std::cout<<"INC (HL)"<<std::endl;
    const unsigned short HL = R[H] << 8 | R[L];
    WriteMemory(HL, addition8bit(ReadMemory(HL), 0x1, 0) );
    PC++;
    add_counter(12);
    return true;
}

bool emu::DEC_A(){
    if (debug)
        std::cout<<"DEC A"<<std::endl;
    R[A] = substract8bit(R[A], 0x1,0);
    PC++;
    add_counter();
    return true;
}

bool emu::DEC_B(){
    if (debug)
        std::cout<<"DEC B"<<std::endl;
    R[B] = substract8bit(R[B], 0x1,0);
    PC++;
    add_counter();
    return true;
}

bool emu::DEC_C(){
    if (debug)
        std::cout<<"DEC C"<<std::endl;
    R[C] = substract8bit(R[C], 0x1,0);
    PC++;
    add_counter();
    return true;
}

bool emu::DEC_D(){
    if (debug)
        std::cout<<"DEC D"<<std::endl;
    R[D] = substract8bit(R[D], 0x1,0);
    PC++;
    add_counter();
    return true;
}

//+++++++++++++++++++++++++++16-bit Arithmetic +++++++++++++++++++++++++++//

bool emu::DEC_BC(){
    
    if (debug)
        std::cout<<"DEC BC"<<std::endl;
    
    unsigned short BC = R[B] << 8 | R[C];
    
    BC -= 0x1;
    
    R[B] = (BC & 0xFF00)>>8;
    R[C] = BC & 0x00FF;
    PC++;
    add_counter(8);
    return true;
}

//+++++++++++++++++++++++++++Miscellaneous +++++++++++++++++++++++++++//

bool emu::CB(){
    PC++;
    opcode = ReadMemory(PC);
    return  (this->*cb[opcode])();
}

bool emu::SWAP_A(){
    if (debug)
        std::cout<<"SWAP A"<<std::endl;
    R[A] = swap8bit(R[A]);
    PC += 2;
    return true;
}


bool emu::CPL(){
    if (debug)
        std::cout<<"CPL"<<std::endl;

    R[A] = ~R[A];
    R[F] |= 0x60;
    PC++;
    return true;
}

bool emu::NOP(){
    if (debug)
        std::cout<<"NOP"<<std::endl;
    PC++;
    add_counter();
    return true;
}


bool emu::DI(){
    if (debug)
        std::cout<<"DI"<<std::endl;
    ime=false;
    PC++;
    add_counter();
    return true;
}

bool emu::EI(){
    if (debug)
        std::cout<<"EI"<<std::endl;
    PC++;
    add_counter();
    ime=true;
    return true;
}

//+++++++++++++++++++++++++++Rotates & Shifts +++++++++++++++++++++++++++//

bool emu::RRA(){
    if (debug)
        std::cout<<"RRA"<<std::endl;
    
    r_rotation(&R[A]);
    add_counter();
    return true;
}


//+++++++++++++++++++++++++++Bit Opcodes +++++++++++++++++++++++++++//

//+++++++++++++++++++++++++++Jumps +++++++++++++++++++++++++++//


bool emu::JP_a16(){
    if (debug)
        std::cout<<"JP "<<std::hex<<int(ReadMemory(PC + 2) << 8 |  ReadMemory(PC + 1))<<std::endl;
    PC = ReadMemory(PC + 2) << 8 |  ReadMemory(PC + 1);
    add_counter(16);
    return true;
}

bool emu::JP_pHL(){
    if (debug)
        std::cout<<"JP (HL)"<<std::endl;
    PC = R[H] << 8 | R[L];
    add_counter();
    return true;
}

bool emu::JR_NZ_r8(){
    if (debug)
        std::cout<<"JR NZ "<<std::hex<<int(ReadMemory(PC + 1))<<std::endl;
    signed char add = ReadMemory(PC + 1);
    PC += 2;
    if ((R[F] & 0x80) == 0x00){
        PC += add;
        add_counter(12);
    }
    else
        add_counter(8);
    return true;
}

bool emu::JR_Z_r8(){
    if (debug)
        std::cout<<"JR Z "<<std::hex<<int(ReadMemory(PC + 1))<<std::endl;
    signed char add = ReadMemory(PC + 1);
    PC += 2;
    if ((R[F] & 0x80) == 0x80){
        PC += add;
        add_counter(12);
    }
    else
        add_counter(8);
    return true;
}
//+++++++++++++++++++++++++++Calls +++++++++++++++++++++++++++//

bool emu::CALL_a16(){
    if (debug)
        std::cout<<"CALL "<<int(ReadMemory(PC + 2) << 8 |  ReadMemory(PC + 1))<<std::endl;

    
    WriteMemory(SP-1, (PC+3 & 0xFF00)>>8);
    WriteMemory(SP-2, PC+3 & 0x00FF);

    PC = ReadMemory(PC + 2) << 8 |  ReadMemory(PC + 1);
    SP -= 2;
    add_counter(24);
    return true;
}

bool emu::RST(unsigned char addr){
    
    WriteMemory(SP-1, (PC & 0xFF00)>>8);
    WriteMemory(SP-2, PC & 0x00FF);
    
    PC = addr;
    SP -= 2;
    
    add_counter(12);
    return true;
}

//+++++++++++++++++++++++++++Restarts +++++++++++++++++++++++++++//

//+++++++++++++++++++++++++++Returns +++++++++++++++++++++++++++//

bool emu::RET(){
    
    if (debug)
        std::cout<<"RET"<<std::endl;
    PC = 0;
    PC |= ReadMemory(SP);
    PC |= ReadMemory(SP+1)<<8;
    
    SP += 2;
    add_counter(16);
    return true;
}

bool emu::RETI(){
    
    if (debug)
        std::cout<<"RETI"<<std::endl;
    
    PC = 0;
    PC |= ReadMemory(SP);
    PC |= ReadMemory(SP+1)<<8;
    

    SP += 2;
    add_counter(16);
    ime=true;
    return true;
}

bool emu::RET_Z(){
    
    if (debug)
        std::cout<<"RET Z"<<std::endl;
    //std::cout<<"SP: "<<std::hex<<int(ReadMemory(SP+1)<<8 | ReadMemory(SP))<<std::endl;
    if ((R[F] & 0x80) == 0x80){
        
        PC = 0;
        PC |= ReadMemory(SP);
        PC |= ReadMemory(SP+1)<<8;
        
        SP += 2;
        add_counter(16);
    }
    
    else{
        PC++;
        add_counter(4);
    }
    return true;
}

bool emu::RET_NZ(){
    
    if (debug)
        std::cout<<"RET NZ"<<std::endl;
    //std::cout<<"SP: "<<std::hex<<int(ReadMemory(SP+1)<<8 | ReadMemory(SP))<<std::endl;
    if ((R[F] & 0x80) == 0x00){
        
        PC = 0;
        PC |= ReadMemory(SP);
        PC |= ReadMemory(SP+1)<<8;
    
        SP += 2;
        add_counter(16);
    }
    
    else{
        PC++;
        add_counter(4);
    }
    return true;
}

bool emu::cpuNULL(){
    std::cout<<"operación no implementada: "<<std::hex<<int(opcode)<<std::endl;
    //std::cout<<"PC: "<<std::hex<<int(PC)<<std::endl;
    return false;
}

bool emu::cpuCBNULL(){
    std::cout<<"operación no implementada: cb"<<std::hex<<int(opcode)<<std::endl;
    return false;
}

bool emu::emulateCycle(){

    std::cout<<"PC:"<<std::hex<<int(PC)<<" ";
    
    opcode = ReadMemory(PC);
    bool result = (this->*table[opcode])();
    
    
    if (ime) {
        unsigned char flag = ReadMemory(0xFF0F) & ReadMemory(0xFFFF);
        if ((flag & 0x01) > 0) {
            test++;
            WriteMemory(0xFF0F, 0xE0);
            ime = false;
            RST(0x40);
            
        }
    }
    
    if (test == 3 ) {
        result = false;
        std::cout<<"INTERRUPT"<<std::endl;
    }
    return result;
    
}

void emu::add_counter(int c){
    curr_m = c/4;
    curr_t = c;
    
    if ((ReadMemory(0xff40) & 0x80) != 0x0) {
        int mode = ReadMemory(0xFF41);
        gpu_t += curr_t;
        switch (mode & 0x03) {
            case S_OAM:
                if (gpu_t >= 77) {
                    mode ++;
                    WriteMemory(0xFF41, mode);
                    gpu_t = 0;
                }
                break;
            case S_VRAM:
                if (gpu_t >= 169) {
                    mode -= 3;
                    WriteMemory(0xFF41, mode);
                    gpu_t = 0;
                }
                break;
            case HBLANK:
                if (gpu_t >= 201) {
                    mode ++;
                    WriteMemory(0xFF41, mode);
                    gpu_t = 0;
                }
                break;
                
            case VBLANK:
                if (gpu_t >= 456) {
                    gpu_t = 0;
                    WriteMemory(0xFF44,ReadMemory(0xFF44)+1);
                    if (ReadMemory(0xFF44) > 156) {
                        WriteMemory(0xFF44, 0);
                        mode++;
                        WriteMemory(0xFF41, mode);
                    }
                }
                break;
                
            default:
                break;
        }

    }
    
}

unsigned short emu::getAF(){
    return R[A] << 8 | R[F];
}

unsigned short emu::getBC(){
    return R[B] << 8 | R[C];
}

unsigned short emu::getDE(){
    return R[D] << 8 | R[E];
}

unsigned short emu::getHL(){
    return R[H] << 8 | R[L];
}

unsigned short emu::getPC(){
    return PC;
}

unsigned short emu::getSP(){
    return SP;
}

std::string emu::nextopcode(){
    return opcodes[ReadMemory(PC)];
}

